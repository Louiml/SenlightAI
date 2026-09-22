Write a standalone C++ function that, given an array of triangle indices and a mesh stored as three parallel arrays (vertex coordinates and triangle-to-vertex connectivity), returns a sorted array of unique vertex IDs that appear in at least one valid triangle from the input list. The mesh is represented by: a `vertexCount` and `triangleCount`, an array `triangleVertices` of `int` triplets (each entry is a vertex ID, and valid vertex IDs are in `[0, vertexCount-1]`), and an array `triangleValid` of `bool` flags indicating whether each triangle is considered valid. Input triangle indices may be out of range or refer to invalid triangles; such entries must be skipped. The output must be sorted ascending and contain no duplicates. The function should be efficient for both small input lists (few triangles) and large ones (up to millions of triangles), avoiding unnecessary memory allocation or linear scans per insertion.

// The solution must collect unique vertex IDs from all valid triangles referenced by the input triangle index list. The key algorithm: first, iterate through the input triangle indices. For each index, check if it is within `[0, triangleCount-1]` and if `triangleValid[index]` is true. If so, extract the three vertex IDs from the `triangleVertices` array (which is a flat array of length `triangleCount*3`, storing each triangle’s three vertices consecutively). Add these vertex IDs to a set-like structure that supports fast lookup and insertion. For small input sizes (e.g., fewer than 25 triangles), a simple vector with `std::find` for uniqueness is acceptable and avoids hash-set overhead. For larger inputs, a `std::unordered_set` (or `std::set` if ordering is needed during insertion) is appropriate. After collecting all unique vertex IDs, sort the resulting vector (if using `unordered_set`) or directly copy from `set`. Edge cases: input may be empty; a triangle may be referenced multiple times; invalid triangle indices must be ignored; vertex IDs may repeat across different triangles; the output must be sorted regardless of input order. Time complexity: for small `n` (number of triangle references), O(n*k) where k is the number of unique vertices added (using linear search), but k is constant (max 3 per triangle), so O(n). For large n, using `unordered_set` gives average O(1) insertion, total O(n), then sorting O(m log m) where m is number of unique vertices, which is at most 3*validTriangleCount. Space: O(m) for the output and set.

#include <vector>
#include <algorithm>
#include <unordered_set>
#include <cstddef>

// Given mesh data (vertexCount, triangleCount, triangleVertices as flat array of size triangleCount*3,
// triangleValid flags array of size triangleCount), and a list of triangle indices,
// return a sorted vector of unique vertex IDs that appear in at least one valid triangle.
std::vector<int> uniqueVerticesFromTriangles(
    int vertexCount,
    int triangleCount,
    const std::vector<int>& triangleVertices, // size triangleCount*3
    const std::vector<bool>& triangleValid,   // size triangleCount
    const std::vector<int>& triangleIndices)
{
    std::vector<int> result;
    int n = static_cast<int>(triangleIndices.size());

    // For small input, use linear search to keep it simple and fast.
    if (n < 25) {
        for (int idx : triangleIndices) {
            if (idx >= 0 && idx < triangleCount && triangleValid[idx]) {
                int base = idx * 3;
                int v0 = triangleVertices[base];
                int v1 = triangleVertices[base+1];
                int v2 = triangleVertices[base+2];
                // Add each vertex if not already in result
                for (int v : {v0, v1, v2}) {
                    if (v >= 0 && v < vertexCount && std::find(result.begin(), result.end(), v) == result.end()) {
                        result.push_back(v);
                    }
                }
            }
        }
        std::sort(result.begin(), result.end());
        return result;
    }

    // Large input: use hash set for O(1) average insertions.
    std::unordered_set<int> vertexSet;
    vertexSet.reserve(static_cast<size_t>(n) * 3);
    for (int idx : triangleIndices) {
        if (idx >= 0 && idx < triangleCount && triangleValid[idx]) {
            int base = idx * 3;
            int v0 = triangleVertices[base];
            int v1 = triangleVertices[base+1];
            int v2 = triangleVertices[base+2];
            if (v0 >= 0 && v0 < vertexCount) vertexSet.insert(v0);
            if (v1 >= 0 && v1 < vertexCount) vertexSet.insert(v1);
            if (v2 >= 0 && v2 < vertexCount) vertexSet.insert(v2);
        }
    }
    result.assign(vertexSet.begin(), vertexSet.end());
    std::sort(result.begin(), result.end());
    return result;
}

#include <cassert>
#include <vector>
#include <algorithm>

// The solution function is declared above; include it or paste here.

int main() {
    // Simple mesh: 4 vertices, 2 triangles
    // Triangle 0: vertices (0,1,2), valid
    // Triangle 1: vertices (1,2,3), valid
    int vertexCount = 4;
    int triangleCount = 2;
    std::vector<int> triVerts = {0,1,2, 1,2,3};
    std::vector<bool> triValid = {true, true};

    // Test 1: Single triangle
    std::vector<int> out = uniqueVerticesFromTriangles(vertexCount, triangleCount, triVerts, triValid, {0});
    assert((out == std::vector<int>{0,1,2}));

    // Test 2: Both triangles, duplicate vertices
    out = uniqueVerticesFromTriangles(vertexCount, triangleCount, triVerts, triValid, {0,1});
    assert((out == std::vector<int>{0,1,2,3}));

    // Test 3: Invalid triangle index ignored
    out = uniqueVerticesFromTriangles(vertexCount, triangleCount, triVerts, triValid, {2, 0});
    assert((out == std::vector<int>{0,1,2}));

    // Test 4: Invalid flag triangle ignored
    std::vector<bool> triValid2 = {false, true};
    out = uniqueVerticesFromTriangles(vertexCount, triangleCount, triVerts, triValid2, {0,1});
    assert((out == std::vector<int>{1,2,3}));

    // Test 5: Empty input
    out = uniqueVerticesFromTriangles(vertexCount, triangleCount, triVerts, triValid, {});
    assert(out.empty());

    // Test 6: Large input, 100 triangles all valid, 300 unique vertices (cyclic)
    int bigTriCount = 100;
    int bigVertCount = 300;
    std::vector<int> bigTriVerts(bigTriCount * 3);
    for (int i = 0; i < bigTriCount; ++i) {
        bigTriVerts[i*3] = i % bigVertCount;
        bigTriVerts[i*3+1] = (i+1) % bigVertCount;
        bigTriVerts[i*3+2] = (i+2) % bigVertCount;
    }
    std::vector<bool> bigTriValid(bigTriCount, true);
    std::vector<int> bigIndices(bigTriCount);
    for (int i = 0; i < bigTriCount; ++i) bigIndices[i] = i;
    out = uniqueVerticesFromTriangles(bigVertCount, bigTriCount, bigTriVerts, bigTriValid, bigIndices);
    // Since triangles cover vertices 0..99, but only 100 unique (0..99) because beyond 100 not used
    // Actually triangle i uses vertices i, i+1, i+2 mod 300, so vertices 0..101 appear; but let's compute: max index 99+2=101, so 0..101
    assert(out == std::vector<int>({0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101}));

    // Test 7: Vertex ID out of range ignored
    std::vector<int> badTriVerts = {0,1,5, 2,3,4}; // vertex 5 invalid (since vertexCount=4)
    std::vector<bool> badTriValid = {true, true};
    out = uniqueVerticesFromTriangles(4, 2, badTriVerts, badTriValid, {0});
    assert((out == std::vector<int>{0,1}));

    // Test 8: Repeated triangle index
    out = uniqueVerticesFromTriangles(vertexCount, triangleCount, triVerts, triValid, {0,0,0});
    assert((out == std::vector<int>{0,1,2}));

    // Test 9: Check sorting even when input unsorted
    out = uniqueVerticesFromTriangles(vertexCount, triangleCount, triVerts, triValid, {1,0});
    assert((out == std::vector<int>{0,1,2,3}));

    // Test 10: Large input with invalid triangles mixed
    std::vector<bool> mixedValid(bigTriCount, true);
    mixedValid[10] = false; mixedValid[20] = false;
    std::vector<int> mixedIndices = {0, 10, 20, 30}; // skipping invalid 10 and 20
    out = uniqueVerticesFromTriangles(bigVertCount, bigTriCount, bigTriVerts, mixedValid, mixedIndices);
    // Triangle 0: 0,1,2; Triangle 30: 30,31,32
    assert((out == std::vector<int>{0,1,2,30,31,32}));

    return 0;
}
