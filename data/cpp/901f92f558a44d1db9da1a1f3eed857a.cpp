/*
Write a standalone C++ function that performs mesh smoothing on a triangle mesh described by flat arrays of vertex coordinates and triangle connectivity. The mesh is given as `numPoints`, `x`, `y`, `z` coordinate arrays (each of size `numPoints`), `numTriangles`, and a flattened triangle index list `triangles` of size `3 * numTriangles` containing vertex indices (0-based). Implement a simplified version of the Gaussian smoothing algorithm from the provided snippet: for each vertex, compute the average of its direct neighbor vertex positions (vertices sharing at least one triangle edge with it), then move the vertex toward that average by a user-supplied factor `lambda` (in (0,1)). Repeat this process for a specified number of `iterations`. Vertices with no neighbors (isolated) must remain unchanged. The function should modify the coordinate arrays in-place and return `void`. Do not include any file I/O, module infrastructure, or other domain-specific types. Use standard C++ containers only where necessary (e.g., `std::vector`) and avoid dynamic memory allocation beyond what is needed.
*/

#include <vector>
#include <set>
#include <algorithm>

/**
 * Perform Gaussian smoothing on a triangle mesh.
 * 
 * @param numPoints     Number of vertices.
 * @param x, y, z       Coordinate arrays (size numPoints) modified in-place.
 * @param numTriangles  Number of triangles.
 * @param triangles     Flattened triangle vertex indices (size 3*numTriangles).
 * @param iterations    Number of smoothing passes.
 * @param lambda        Smoothing factor in (0,1).
 */
void smoothMeshGaussian(int numPoints,
                        std::vector<float>& x,
                        std::vector<float>& y,
                        std::vector<float>& z,
                        int numTriangles,
                        const std::vector<int>& triangles,
                        int iterations,
                        float lambda) {
    // Build adjacency list: for each vertex, a set of unique neighbor indices.
    std::vector<std::set<int>> neighbors(numPoints);
    for (int t = 0; t < numTriangles; ++t) {
        int i0 = triangles[3 * t + 0];
        int i1 = triangles[3 * t + 1];
        int i2 = triangles[3 * t + 2];
        neighbors[i0].insert(i1);
        neighbors[i0].insert(i2);
        neighbors[i1].insert(i0);
        neighbors[i1].insert(i2);
        neighbors[i2].insert(i0);
        neighbors[i2].insert(i1);
    }

    // Convert sets to vectors for faster iteration.
    std::vector<std::vector<int>> adj(numPoints);
    for (int i = 0; i < numPoints; ++i) {
        adj[i].assign(neighbors[i].begin(), neighbors[i].end());
    }

    // Temporary storage for deltas.
    std::vector<float> dx(numPoints, 0.0f), dy(numPoints, 0.0f), dz(numPoints, 0.0f);

    for (int iter = 0; iter < iterations; ++iter) {
        // Compute deltas based on current positions.
        for (int i = 0; i < numPoints; ++i) {
            int deg = static_cast<int>(adj[i].size());
            if (deg == 0) {
                // Isolated vertex; leave unchanged.
                dx[i] = dy[i] = dz[i] = 0.0f;
                continue;
            }
            float sumX = 0.0f, sumY = 0.0f, sumZ = 0.0f;
            for (int j : adj[i]) {
                sumX += x[j];
                sumY += y[j];
                sumZ += z[j];
            }
            float invDeg = 1.0f / static_cast<float>(deg);
            // delta = neighborMean - currentPos
            dx[i] = sumX * invDeg - x[i];
            dy[i] = sumY * invDeg - y[i];
            dz[i] = sumZ * invDeg - z[i];
        }
        // Update all vertices simultaneously (Jacobi-style).
        for (int i = 0; i < numPoints; ++i) {
            x[i] += lambda * dx[i];
            y[i] += lambda * dy[i];
            z[i] += lambda * dz[i];
        }
    }
}

#include <cassert>
#include <cmath>
#include <vector>

// Function declaration (included for completeness; the solution above defines it).
void smoothMeshGaussian(int numPoints,
                        std::vector<float>& x,
                        std::vector<float>& y,
                        std::vector<float>& z,
                        int numTriangles,
                        const std::vector<int>& triangles,
                        int iterations,
                        float lambda);

bool approxEqual(float a, float b, float eps = 1e-4f) {
    return std::fabs(a - b) < eps;
}

int main() {
    // Test 1: Single isolated vertex, should remain unchanged.
    {
        int n = 1;
        std::vector<float> x = {1.0f}, y = {2.0f}, z = {3.0f};
        std::vector<int> tris; // no triangles
        smoothMeshGaussian(n, x, y, z, 0, tris, 5, 0.5f);
        assert(approxEqual(x[0], 1.0f) && approxEqual(y[0], 2.0f) && approxEqual(z[0], 3.0f));
    }

    // Test 2: Two vertices connected by an edge (degenerate mesh but valid neighbor relationship).
    // Each vertex has the other as neighbor; with lambda=1, they swap? Actually one iteration:
    // v0 neighbors: v1 => mean = v1, delta = v1 - v0; v0' = v0 + 1*(v1-v0) = v1.
    // Similarly v1' = v0. So after one iteration they swap positions.
    {
        int n = 2;
        std::vector<float> x = {0.0f, 10.0f}, y = {0.0f, 0.0f}, z = {0.0f, 0.0f};
        std::vector<int> tris = {0, 1, 0}; // degenerate triangle just to create edge (0-1)
        smoothMeshGaussian(n, x, y, z, 1, tris, 1, 1.0f);
        assert(approxEqual(x[0], 10.0f) && approxEqual(x[1], 0.0f));
    }

    // Test 3: Triangle with three vertices, one iteration, lambda=1/3.
    // Each vertex has two neighbors, mean = average of the other two.
    // For equilateral triangle (0,0), (2,0), (1, sqrt(3))? Use simple:
    // v0=(0,0,0), v1=(2,0,0), v2=(0,2,0)
    // v0 neighbors: v1,v2 -> mean=(1,1,0), delta=(1,1,0); v0' = (0+1/3*1, 0+1/3*1) = (1/3,1/3)
    {
        int n = 3;
        std::vector<float> x = {0.0f, 2.0f, 0.0f};
        std::vector<float> y = {0.0f, 0.0f, 2.0f};
        std::vector<float> z = {0.0f, 0.0f, 0.0f};
        std::vector<int> tris = {0, 1, 2};
        smoothMeshGaussian(n, x, y, z, 1, tris, 1, 1.0f/3.0f);
        assert(approxEqual(x[0], 1.0f/3.0f) && approxEqual(y[0], 1.0f/3.0f) && approxEqual(z[0], 0.0f));
        // Vertex 1: neighbors 0 and 2 -> mean=(0,1,0), delta=(-2,1,0), new x = 2 + (1/3)*(-2) = 4/3, y=1/3
        assert(approxEqual(x[1], 4.0f/3.0f) && approxEqual(y[1], 1.0f/3.0f));
        // Vertex 2: similar
        assert(approxEqual(x[2], 1.0f/3.0f) && approxEqual(y[2], 4.0f/3.0f));
    }

    // Test 4: Multiple iterations on a square (two triangles) converge toward centroid.
    {
        int n = 4;
        std::vector<float> x = {0.0f, 1.0f, 1.0f, 0.0f};
        std::vector<float> y = {0.0f, 0.0f, 1.0f, 1.0f};
        std::vector<float> z = {0.0f, 0.0f, 0.0f, 0.0f};
        std::vector<int> tris = {0, 1, 2, 0, 2, 3}; // two triangles
        // After many iterations with lambda=0.5, all points should move toward average (0.5,0.5).
        smoothMeshGaussian(n, x, y, z, 2, tris, 100, 0.5f);
        for (int i = 0; i < n; ++i) {
            assert(approxEqual(x[i], 0.5f, 1e-3f));
            assert(approxEqual(y[i], 0.5f, 1e-3f));
            assert(approxEqual(z[i], 0.0f, 1e-3f));
        }
    }

    // Test 5: Isolated vertex among connected ones remains fixed.
    {
        int n = 4;
        std::vector<float> x = {0.0f, 1.0f, 1.0f, 0.0f}; // first three connected, last isolated
        std::vector<float> y = {0.0f, 0.0f, 1.0f, 5.0f};
        std::vector<float> z = {0.0f, 0.0f, 0.0f, 0.0f};
        std::vector<int> tris = {0, 1, 2}; // only triangle among 0,1,2
        smoothMeshGaussian(n, x, y, z, 1, tris, 3, 0.5f);
        // Last vertex unchanged
        assert(approxEqual(x[3], 0.0f) && approxEqual(y[3], 5.0f) && approxEqual(z[3], 0.0f));
        // Other vertices have moved (they shrink toward each other)
        assert(!(approxEqual(x[0], 0.0f) && approxEqual(y[0], 0.0f))); // at least one changed
    }

    // Test 6: Zero iterations leaves mesh unchanged.
    {
        int n = 3;
        std::vector<float> x = {0.0f, 2.0f, 0.0f};
        std::vector<float> y = {0.0f, 0.0f, 2.0f};
        std::vector<float> z = {1.0f, -1.0f, 0.0f};
        std::vector<int> tris = {0, 1, 2};
        smoothMeshGaussian(n, x, y, z, 1, tris, 0, 0.8f);
        assert(approxEqual(x[0], 0.0f) && approxEqual(y[1], 0.0f) && approxEqual(z[2], 0.0f));
    }

    return 0;
}

// The core algorithm is a fixed-point iterative smoothing: each iteration computes for every vertex the arithmetic mean of the positions of its adjacent vertices, then updates the vertex position as `pos = pos + lambda * (neighborMean - pos)`. To find neighbors, build an adjacency list from the triangle list: for each triangle, for each of its three edges, add the two endpoint vertices to each other's neighbor set. To avoid duplicates (since a vertex may share multiple triangles with the same neighbor), use a `std::set` per vertex or, more efficiently, a `std::vector<std::vector<int>>` and later deduplicate or use a temporary boolean visited array per vertex during accumulation. Since the number of vertices and triangles is moderate in typical tasks, a simple approach: for each iteration, for each vertex, use a `std::vector<bool>` of size `numPoints` to mark visited neighbors, then accumulate sums only from unique neighbors. However, that would be O(N*iterations*neighbors) with an extra O(N) reset per vertex, which is acceptable. Better: pre-build a neighbor list once (deduplicated) using a `std::vector<std::set<int>>` and then each iteration iterates over that list. Complexity: building the adjacency list is O(numTriangles * 3) time, deduplication using sets adds a log factor; if using vectors and sort+unique, it's O(E log E) where E is the number of distinct edges. Each smoothing iteration is O(N + totalNeighbors) time. Total time is O(numTriangles + iterations * (N + totalNeighbors)). Space is O(N + totalNeighbors) for the adjacency list. Edge cases: isolated vertices (no neighbors) must not move; ensure we skip them. The lambda must be strictly between 0 and 1; the function can clamp or simply use as given, but for correctness we assume caller provides valid values. The function modifies input arrays in place.
