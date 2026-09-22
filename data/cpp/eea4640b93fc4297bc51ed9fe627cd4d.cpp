Write a C++ function that simulates a simple GPU vertex cache analysis. Given an array of triangle indices (`indices` with `index_count` entries, where each triangle is 3 consecutive indices), a vertex count, a cache size (minimum 3), and an optional warp size (minimum 3, or 0 to disable), compute and return a `VertexCacheStatistics` struct containing `vertices_transformed`, `warps_executed`, `acmr` (average cache miss ratio = vertices transformed per triangle), and `atvr` (average transformed vertices per unique vertex). The simulation processes triangles in order. For each triangle, the three vertices are considered cache misses if their last access timestamp is older than `cache_size` steps. A miss increments `vertices_transformed`. A "warp" is a batch of triangles whose total missed vertices (sum of misses from each triangle in the batch) does not exceed `warp_size` (or unlimited if `warp_size==0`). When the current warp would exceed the limit (or when `primgroup_size` is nonzero and a primitive group count reaches that limit), the warp is flushed: if any triangle contributed to the current warp, increment `warps_executed`, reset warp and primitive group counters, and simulate a cache flush by advancing a global timestamp by `cache_size+1` (so all previously cached vertices become misses). The function must handle edge cases: empty index count (return zeros), warps with zero misses (e.g., already-cached vertices), and count final partial warp. The returned `acmr` is `vertices_transformed / (index_count/3)`, and `atvr` is `vertices_transformed / count_of_unique_vertices_that_appear_at_least_once`. The unique vertex count is determined by how many vertices received at least one timestamp (i.e., were accessed at least once). Use `unsigned int` for all counters. Provide the function with name `analyzeVertexCache` and return a `VertexCacheStatistics` struct as defined below.

// The core approach is a timestamp-based LRU simulation. Maintain an array `cache_timestamps` of size `vertex_count`, initialized to zero. A global `timestamp` starts at `cache_size+1`. For each triangle `(a,b,c)`, compute whether each vertex is a miss: `timestamp - cache_timestamps[v] > cache_size`. For each miss, set `cache_timestamps[v] = timestamp++` and increment `vertices_transformed` and a `warp_offset` counter. Before processing a triangle, check if adding its misses would exceed the warp limit or if the primitive group limit is reached. If so, flush: if `warp_offset>0` then increment `warps_executed`, reset `warp_offset=0` and `primgroup_offset=0`, and advance `timestamp += cache_size+1` to simulate cache reset. If `primgroup_size==0`, ignore the primitive group condition. If `warp_size==0`, disable warp limit entirely (treat as unlimited). After processing all triangles, flush any remaining partial warp. Then count unique vertices as those with `cache_timestamps[i]>0`. Compute `acmr` as `vertices_transformed / (index_count/3)` (cast to float, guard division by zero), and `atvr` as `vertices_transformed / unique_vertex_count`. Edge cases: empty index list → all fields zero except `acmr` and `atvr` zero. Warp size less than 3? Assert but allow 0 to disable; assume warp_size>=3 if nonzero. Primitive group size can be any positive integer; if 0, treat as unlimited (similar to warp). The timestamp can overflow unsigned int after many triangles? For realistic inputs it’s fine; we rely on wraparound only for correctness of miss detection (`(timestamp - cache_timestamps[v])` works even if wraparound because unsigned subtraction wraps modulo 2^32, and we only compare against `cache_size` which is small relative to 2^32). Time complexity O(index_count + vertex_count), space O(vertex_count). The algorithm must handle large vertex counts efficiently.

#include <assert.h>
#include <string.h>
#include <stddef.h>

// Statistics from vertex cache simulation
struct VertexCacheStatistics {
    unsigned int vertices_transformed;
    unsigned int warps_executed;
    float acmr;  // average cache miss ratio per triangle
    float atvr;  // average transformed vertices per unique vertex
};

/**
 * Simulate a GPU vertex cache with FIFO-like eviction using timestamps.
 * 
 * @param indices        Pointer to triangle index array (size index_count, multiple of 3).
 * @param index_count    Total number of indices (triangles = index_count/3).
 * @param vertex_count   Number of unique vertices (vertices numbered 0..vertex_count-1).
 * @param cache_size     Cache capacity (>=3). Miss if not accessed within last cache_size accesses.
 * @param warp_size      Max misses allowed in a warp; 0 disables warp grouping.
 * @param primgroup_size Max triangles in a primitive group; 0 disables grouping.
 * @return Statistics about the simulation.
 */
VertexCacheStatistics analyzeVertexCache(
    const unsigned int* indices,
    size_t index_count,
    size_t vertex_count,
    unsigned int cache_size,
    unsigned int warp_size,
    unsigned int primgroup_size)
{
    assert(index_count % 3 == 0);
    assert(cache_size >= 3);
    assert(warp_size == 0 || warp_size >= 3);

    VertexCacheStatistics result = {};
    result.vertices_transformed = 0;
    result.warps_executed = 0;

    // Allocate and initialize timestamps
    unsigned int* cache_timestamps = new unsigned int[vertex_count]();
    // Alternative: memset(cache_timestamps, 0, vertex_count * sizeof(unsigned int));

    unsigned int timestamp = cache_size + 1;
    unsigned int warp_offset = 0;
    unsigned int primgroup_offset = 0;

    size_t triangle_count = index_count / 3;

    for (size_t i = 0; i < index_count; i += 3)
    {
        unsigned int a = indices[i + 0];
        unsigned int b = indices[i + 1];
        unsigned int c = indices[i + 2];
        assert(a < vertex_count && b < vertex_count && c < vertex_count);

        bool ac = (timestamp - cache_timestamps[a]) > cache_size;
        bool bc = (timestamp - cache_timestamps[b]) > cache_size;
        bool cc = (timestamp - cache_timestamps[c]) > cache_size;

        // Check if this triangle would overflow warp or primitive group
        if ((primgroup_size != 0 && primgroup_offset == primgroup_size) ||
            (warp_size != 0 && warp_offset + ac + bc + cc > warp_size))
        {
            if (warp_offset > 0)
                result.warps_executed++;
            warp_offset = 0;
            primgroup_offset = 0;
            // Simulate cache flush by advancing timestamp beyond cache_size
            timestamp += cache_size + 1;
        }

        // Process the triangle's vertices
        unsigned int indices_j[3] = {a, b, c};
        for (int j = 0; j < 3; ++j)
        {
            unsigned int index = indices_j[j];
            if (timestamp - cache_timestamps[index] > cache_size)
            {
                cache_timestamps[index] = timestamp++;
                result.vertices_transformed++;
                warp_offset++;
            }
        }

        primgroup_offset++;
    }

    // Flush final partial warp
    if (warp_offset > 0)
        result.warps_executed++;

    // Count unique vertices that were ever accessed
    size_t unique_vertex_count = 0;
    for (size_t i = 0; i < vertex_count; ++i)
        if (cache_timestamps[i] > 0)
            unique_vertex_count++;

    delete[] cache_timestamps;

    // Compute ratios (guarding division by zero)
    result.acmr = triangle_count == 0 ? 0.0f : (float)result.vertices_transformed / (float)triangle_count;
    result.atvr = unique_vertex_count == 0 ? 0.0f : (float)result.vertices_transformed / (float)unique_vertex_count;

    return result;
}

#include <assert.h>
#include <stddef.h>

// Assume VertexCacheStatistics and analyzeVertexCache are defined above.

int main() {
    // Test 1: Simple two triangles, small cache
    unsigned int indices1[] = {0,1,2, 2,1,3};
    VertexCacheStatistics s1 = analyzeVertexCache(indices1, 6, 4, 4, 0, 0);
    // Process: tri0 misses 0,1,2 -> verts=3, warp_offset=3
    // tri1: a=2 (cached? timestamp diff after 3 increments? Let's compute:
    // Start timestamp=5. tri0: miss 0 -> ts[0]=5, ts=6; miss 1 -> ts[1]=6, ts=7; miss 2 -> ts[2]=7, ts=8.
    // tri1: a=2 diff=8-7=1<=4 -> not miss; b=1 diff=8-6=2<=4 -> not miss; c=3 miss -> ts[3]=8, ts=9, verts=4.
    // No warp flush, final warp executed=1. unique=4. acmr=4/2=2.0, atvr=4/4=1.0
    assert(s1.vertices_transformed == 4);
    assert(s1.warps_executed == 1);
    assert(s1.acmr == 2.0f);
    assert(s1.atvr == 1.0f);

    // Test 2: Warp limit forces flush
    // Cache size=3, warp_size=3. Triangle 0: 3 misses -> warp_offset=3. Triangle 1: also 3 misses -> would exceed warp (3+3>3) -> flush before tri1.
    unsigned int indices2[] = {0,1,2, 3,4,5};
    VertexCacheStatistics s2 = analyzeVertexCache(indices2, 6, 6, 3, 3, 0);
    // tri0: misses 0,1,2 -> verts=3, warp_offset=3, timestamp becomes 4+3=7 (start ts=4)
    // before tri1: warp_offset+3=6>3? Actually warp_size=3, warp_offset=3, adding 3 misses => 3+3>3 => flush.
    // Increment warps_executed=1, reset warp_offset=0, timestamp += 4 => timestamp=11.
    // tri1: all misses -> verts=6, warp_offset=3, final flush -> warps_executed=2.
    assert(s2.vertices_transformed == 6);
    assert(s2.warps_executed == 2);
    assert(s2.acmr == 3.0f); // 6/2 triangles
    assert(s2.atvr == 1.0f); // 6 unique vertices all transformed

    // Test 3: primitive group flush
    // primgroup_size=1 forces flush after each triangle
    unsigned int indices3[] = {0,1,2, 0,1,2};
    VertexCacheStatistics s3 = analyzeVertexCache(indices3, 6, 3, 4, 0, 1);
    // tri0: misses all 3 -> verts=3. After triangle, primgroup_offset=1 == primgroup_size=1.
    // Before tri1: flush? No, flush happens at start of next triangle. Let's simulate:
    // Start of tri1: condition (primgroup_offset==primgroup_size) is true -> flush, warps_executed=1, reset warp_offset=0, timestamp += 5 (cache_size+1=5). Now timestamp was 4+3=7 then +5=12. Actually let's recalc: start ts=5. tri0 misses -> ts[0]=5,ts=6; ts[1]=6,ts=7; ts[2]=7,ts=8. primgroup_offset=1.
    // tri1 start: flush -> warps=1, timestamp +=5 => ts=13. tri1: all vertices diff 13-7=6>4? For v0 ts=5 diff=8>4 -> miss, reset ts[0]=13, ts=14; v1 ts=6 diff=8>4 -> miss, ts[1]=14, ts=15; v2 ts=7 diff=8>4 -> miss, ts[2]=15, ts=16. verts=6. Final flush -> warps=2.
    assert(s3.vertices_transformed == 6);
    assert(s3.warps_executed == 2);
    assert(s3.acmr == 3.0f); // 6/2
    assert(s3.atvr == 2.0f); // 6/3 unique

    // Test 4: Empty input
    VertexCacheStatistics s4 = analyzeVertexCache(nullptr, 0, 5, 4, 0, 0);
    assert(s4.vertices_transformed == 0);
    assert(s4.warps_executed == 0);
    assert(s4.acmr == 0.0f);
    assert(s4.atvr == 0.0f);

    // Test 5: Reuse cached vertices without misses
    unsigned int indices5[] = {0,1,2, 2,1,0};
    VertexCacheStatistics s5 = analyzeVertexCache(indices5, 6, 3, 4, 0, 0);
    // tri0: 3 misses -> verts=3, timestamp from 5 to 8.
    // tri1: all three diffs = (8-7=1, 8-6=2, 8-5=3) <=4 => no misses. verts stays 3. unique=3. warps=1.
    assert(s5.vertices_transformed == 3);
    assert(s5.warps_executed == 1);
    assert(s5.acmr == 1.5f);
    assert(s5.atvr == 1.0f);

    return 0;
}
