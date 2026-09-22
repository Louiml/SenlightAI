/*
Write a standalone C++ function named `improveCacheLocality` that takes a vector of triangular faces (each represented as a `std::array<unsigned int, 3>` of vertex indices), the number of vertices in the mesh, and a positive integer cache size (the number of vertex slots in the LRU-like FIFO cache). The function must reorder the faces in-place (i.e., modify the input vector) to reduce the average cache miss ratio (ACMR), where ACMR is defined as the total number of cache misses divided by the number of faces, using a fixed FIFO cache of the given size. The algorithm must follow the "Tipsy" greedy approach: maintain per-vertex live triangle counts, a dead-end stack, caching timestamps, an emitted-face flag, and a candidate list. The function must return the new ACMR after reordering. The input is guaranteed to be a valid triangle mesh (each face has three distinct indices, all indices are in `[0, vertexCount)`, and `vertexCount >= 3`). Handle edge cases: if any vertex has no incident triangles or if the mesh is already optimally ordered, still return the correct ACMR. You may use standard containers (`std::vector`, `std::array`, `std::stack`). Do not implement the adjacency structure manually; instead, compute the adjacency on the fly from the current face vector as needed (you may precompute a vertex-to-face adjacency list). Time complexity: \(O(F \cdot \text{avg degree})\) for building adjacency and \(O(V + F \cdot \log V)\) for the reordering if you use a simple linear scan for candidate selection; the expected practical complexity is near-linear for meshes with bounded degree. Space complexity: \(O(V + F)\).
*/
#include <vector>
#include <array>
#include <stack>
#include <algorithm>
#include <cstddef>

/**
 * Reorders triangular faces in-place to improve cache locality using a greedy Tipsy-style algorithm.
 *
 * @param faces         Vector of triangles, each holding three vertex indices. Modified in-place.
 * @param vertexCount   Total number of vertices in the mesh (indices are in [0, vertexCount)).
 * @param cacheSize     Number of distinct vertices that fit in the FIFO cache (must be >= 1).
 * @return              The resulting ACMR (cache misses / number of faces) after reordering.
 */
double improveCacheLocality(std::vector<std::array<unsigned int, 3>>& faces,
                            unsigned int vertexCount,
                            unsigned int cacheSize) {
    if (faces.empty()) {
        return 0.0;
    }

    // Build vertex-to-face adjacency: for each vertex, list of face indices that use it.
    std::vector<std::vector<unsigned int>> adjacency(vertexCount);
    for (unsigned int f = 0; f < faces.size(); ++f) {
        for (unsigned int idx : faces[f]) {
            adjacency[idx].push_back(f);
        }
    }

    // Live triangle count per vertex (number of not-yet-emitted triangles referencing it).
    std::vector<unsigned int> liveCount(vertexCount, 0);
    for (const auto& face : faces) {
        for (unsigned int idx : face) {
            liveCount[idx]++;
        }
    }

    // Caching timestamp per vertex; 0 means never cached yet.
    std::vector<unsigned int> cacheStamp(vertexCount, 0);
    unsigned int currentStamp = 1;

    // Flags for whether a face has been emitted.
    std::vector<bool> emitted(faces.size(), false);

    // Output index buffer (same size as input, 3 indices per face).
    std::vector<unsigned int> output;
    output.reserve(faces.size() * 3);

    // Dead-end stack for vertices with no more adjacent unemitted triangles.
    std::stack<unsigned int, std::vector<unsigned int>> deadEnd;

    // Temporary candidate list for the next fan vertex selection.
    std::vector<unsigned int> candidates;
    candidates.reserve(vertexCount);

    unsigned int cacheMisses = 0;

    // Start with vertex 0, or the first vertex that has live triangles.
    int fanVertex = -1;
    for (unsigned int v = 0; v < vertexCount; ++v) {
        if (liveCount[v] > 0) {
            fanVertex = static_cast<int>(v);
            break;
        }
    }

    // Linear scan position for fallback when no candidate or dead-end vertex works.
    unsigned int scanPos = 0;

    while (fanVertex >= 0) {
        unsigned int v = static_cast<unsigned int>(fanVertex);

        // Process all unemitted triangles adjacent to v.
        // We must copy the adjacency list because it might be modified? Actually it's static,
        // but we only read it; we use a local copy to avoid iterating over a vector that
        // gets reallocated (it doesn't, since adjacency is const).
        // Careful: after emitting a triangle, we decrement liveCount for other vertices,
        // but the adjacency list remains unchanged (we only mark faces as emitted).
        // So we can iterate directly over adjacency[v].
        for (unsigned int fidx : adjacency[v]) {
            if (!emitted[fidx]) {
                emitted[fidx] = true;
                const auto& face = faces[fidx];

                for (unsigned int idx : face) {
                    // Append to output buffer.
                    output.push_back(idx);

                    // If idx is not the current fan vertex, it may still have live triangles later.
                    if (idx != v) {
                        // Add to dead-end stack and candidate list.
                        deadEnd.push(idx);
                        candidates.push_back(idx);

                        // Decrease live count for this vertex.
                        if (liveCount[idx] > 0) {
                            liveCount[idx]--;
                        }
                    }

                    // Update cache: if vertex is not in cache (based on timestamp difference),
                    // mark it as cached and increment miss count.
                    if (currentStamp - cacheStamp[idx] > cacheSize) {
                        cacheStamp[idx] = currentStamp++;
                        ++cacheMisses;
                    }
                }
            }
        }

        // After processing all neighbors, the fan vertex has no more live triangles.
        liveCount[v] = 0;

        // Select the next fan vertex from candidates.
        int nextVertex = -1;
        int bestPriority = -1;
        for (unsigned int cand : candidates) {
            if (liveCount[cand] > 0) {
                // Compute priority: if vertex is already in cache and will stay there,
                // priority is the time it has been in cache; otherwise 0.
                unsigned int timeInCache = currentStamp - cacheStamp[cand];
                int priority = 0;
                if (timeInCache + 2 * liveCount[cand] <= cacheSize) {
                    priority = static_cast<int>(timeInCache);
                }
                if (priority > bestPriority) {
                    bestPriority = priority;
                    nextVertex = static_cast<int>(cand);
                }
            }
        }

        // If no candidate, try the dead-end stack.
        if (nextVertex < 0) {
            while (!deadEnd.empty()) {
                unsigned int cand = deadEnd.top();
                deadEnd.pop();
                if (liveCount[cand] > 0) {
                    nextVertex = static_cast<int>(cand);
                    break;
                }
            }
        }

        // If still no vertex, fall back to linear scan.
        if (nextVertex < 0) {
            while (scanPos < vertexCount) {
                if (liveCount[scanPos] > 0) {
                    nextVertex = static_cast<int>(scanPos);
                    break;
                }
                ++scanPos;
            }
        }

        fanVertex = nextVertex;

        // Clear candidates for the next iteration.
        candidates.clear();
    }

    // Copy the output index buffer back into the faces vector.
    auto outIt = output.begin();
    for (auto& face : faces) {
        for (auto& idx : face) {
            idx = *outIt++;
        }
    }

    // Compute ACMR.
    return static_cast<double>(cacheMisses) / static_cast<double>(faces.size());
}
#include <cassert>
#include <vector>
#include <array>

// The function under test is declared elsewhere; here we assume it is included.
// double improveCacheLocality(std::vector<std::array<unsigned int, 3>>&, unsigned int, unsigned int);

int main() {
    // Test 1: Single triangle, cache size >= 3 -> all vertices miss on first fan, then no more misses.
    {
        std::vector<std::array<unsigned int, 3>> faces = {{{0, 1, 2}}};
        double acmr = improveCacheLocality(faces, 3, 4);
        assert(acmr == 1.0);
        assert(faces[0] == std::array<unsigned int, 3>{0, 1, 2}); // order unchanged
    }

    // Test 2: Two triangles sharing an edge, cache size 3 -> second triangle's shared vertices hit.
    {
        std::vector<std::array<unsigned int, 3>> faces = {{{0, 1, 2}, {0, 2, 3}}};
        double acmr = improveCacheLocality(faces, 4, 3);
        // Optimal order: emit triangle (0,1,2) -> misses for 0,1,2 (3 misses).
        // Then emit (0,2,3) -> 0 and 2 are in cache (hit), 3 is miss -> 1 miss.
        // Total 4 misses / 2 faces = 2.0 ACMR. Reordering should achieve exactly this.
        assert(acmr == 2.0);
        // The reordered faces should still contain the same triangles.
        // Just check that the total index multiset is preserved.
        std::vector<unsigned int> allIndices;
        for (auto& f : faces) for (auto idx : f) allIndices.push_back(idx);
        std::sort(allIndices.begin(), allIndices.end());
        assert(allIndices == (std::vector<unsigned int>{0,0,1,2,2,3}));
    }

    // Test 3: Cube-like 8 vertices, 12 triangles (two per face, but we use a simple 6-face triangulation).
    // Use a known mesh: a tetrahedron with 4 vertices and 4 faces.
    {
        std::vector<std::array<unsigned int, 3>> faces = {
            {0, 1, 2}, {0, 3, 1}, {0, 2, 3}, {1, 3, 2}
        };
        double acmr = improveCacheLocality(faces, 4, 3);
        // Optimal: first triangle misses 3, second triangle has 2 hits (0,1) and 1 miss (3), third has some hits, etc.
        // With cache size 3, every vertex is in cache after first fan, so total misses = 3 (first triangle) + 0? Actually:
        // After first triangle {0,1,2}, cache has {0,1,2}. Second triangle {0,3,1}: 0 hit, 3 miss, 1 hit -> 1 miss.
        // Third {0,2,3}: all in cache -> 0 misses. Fourth {1,3,2}: all in cache -> 0 misses.
        // Total misses = 3 + 1 = 4, faces=4, ACMR=1.0.
        assert(acmr == 1.0);
    }

    // Test 4: Empty mesh.
    {
        std::vector<std::array<unsigned int, 3>> faces;
        double acmr = improveCacheLocality(faces, 0, 4);
        assert(acmr == 0.0);
    }

    // Test 5: All vertices unique in each triangle, disjoint triangles, cache size 1.
    {
        std::vector<std::array<unsigned int, 3>> faces = {
            {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {9, 10, 11}
        };
        // With cache size 1, every vertex miss is certain because each triangle uses disjoint vertices.
        // Each triangle causes 3 misses, total 12 misses / 4 faces = 3.0.
        double acmr = improveCacheLocality(faces, 12, 1);
        assert(acmr == 3.0);
    }

    // Test 6: Cache larger than vertex count -> only first triangle misses, all others hit.
    {
        std::vector<std::array<unsigned int, 3>> faces = {
            {0, 1, 2}, {1, 2, 3}, {2, 3, 4}, {3, 4, 0}
        };
        double acmr = improveCacheLocality(faces, 5, 10);
        // First triangle: 3 misses. Subsequent triangles: all vertices already in cache -> 0 misses.
        // Total 3/4 = 0.75.
        assert(acmr == 0.75);
    }

    // Test 7: Triangle strip pattern, cache size 2 (only 2 vertices can be in cache).
    // This is a stress test; we just verify the return value is between 1.0 and 3.0.
    {
        std::vector<std::array<unsigned int, 3>> faces;
        for (unsigned int i = 0; i < 100; ++i) {
            faces.push_back({i, i+1, i+2});
        }
        double acmr = improveCacheLocality(faces, 102, 2);
        assert(acmr >= 1.0 && acmr <= 3.0);
    }

    // Test 8: Single vertex repeated in triangle (invalid in practice, but we assume valid input).
    // This is a sanity check for graceful behavior; we skip because input is guaranteed valid.

    return 0;
}
// The core idea is to minimize cache misses by reordering triangle rendering so that recently used vertices remain in a fixed-size FIFO cache. We simulate the cache with a simple timestamp-based approach: each vertex has a "caching timestamp" set when it is first brought into cache; a vertex is considered in cache if the difference between the current timestamp and its stamp is ≤ cache size. We maintain a count of live (unemitted) triangles per vertex. Starting from an arbitrary vertex (we can choose vertex 0), we repeatedly fan out: for every unemitted triangle adjacent to the current fan vertex, we emit its three indices to the output buffer (which will replace the original order later), mark the triangle as emitted, decrement the live counts of its vertices (except the fan vertex itself, which becomes zero after processing), and push the non-fan vertices onto a dead-end stack and a candidate list. After processing all neighbors of the current fan vertex, we select the next fan vertex from the candidates: among vertices with positive live triangles, compute a priority = (current timestamp - vertex's cache stamp) if that value plus twice the live triangle count is within cache size (meaning the vertex will stay in cache), otherwise priority is 0; pick the highest priority. If no candidate works, pop the dead-end stack until a vertex with live triangles is found; if the stack is empty, scan vertex indices linearly for the first with positive live triangles. This ensures we prefer vertices already in cache (reducing misses) and break ties by recency. After all faces are emitted, copy the output index buffer back into the original face array (since face count and vertex count per face are unchanged). The new ACMR is computed as the number of cache misses divided by the number of faces. Edge cases: if a vertex has zero incident triangles, it is ignored; if the mesh has no faces, return 0.0; if all vertices fit in cache, the ACMR will be 1.0 (every face's first vertex misses, the next two are in cache). The algorithm terminates because each face is emitted exactly once.
