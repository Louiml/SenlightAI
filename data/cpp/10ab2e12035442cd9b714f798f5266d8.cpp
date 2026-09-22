/*
Write a C++ function named `findKNearestL2` that, given a set of `nx` query vectors and a set of `ny` database vectors (all stored as contiguous `float` arrays in row-major order with dimension `d`), returns for each query the indices and squared L2 distances of its `k` nearest neighbors in the database. The function must compute the Euclidean squared distance `||x_i - y_j||^2` exactly (without using BLAS or SIMD), and must return the neighbors sorted by increasing distance; if two distances are equal, the neighbor with the smaller database index must come first. The function signature must be:

```cpp
void findKNearestL2(
    const float* x, size_t nx,   // queries, shape (nx, d)
    const float* y, size_t ny,   // database, shape (ny, d)
    size_t d, size_t k,          // dimension and number of neighbors
    std::vector<std::vector<float>>& distances,   // output: nx vectors of length k
    std::vector<std::vector<int64_t>>& indices    // output: nx vectors of length k
);
```

Assume `nx`, `ny`, `d`, and `k` are all positive, and `k <= ny`. The function must be self-contained (only standard library headers) and should handle large inputs efficiently enough for a typical teaching exercise (do not use OpenMP or advanced BLAS — a straightforward nested-loop approach is fine). Return the distances and indices as vectors of vectors, one inner vector per query.
*/

#include <vector>
#include <cstdint>
#include <algorithm>
#include <cmath>
#include <cstddef>

// Compute squared L2 distance between two vectors of length d.
static float squaredL2(const float* a, const float* b, size_t d) {
    float sum = 0.0f;
    for (size_t i = 0; i < d; ++i) {
        float diff = a[i] - b[i];
        sum += diff * diff;
    }
    return sum;
}

// Find the k nearest neighbors (by squared L2 distance) for each query in x
// among the database vectors in y. The vectors are stored row-major.
// Outputs: for each query i, distances[i] and indices[i] are sorted by
// increasing distance; ties are broken by smaller index.
void findKNearestL2(
    const float* x, size_t nx,   // queries, shape (nx, d)
    const float* y, size_t ny,   // database, shape (ny, d)
    size_t d, size_t k,          // dimension and number of neighbors
    std::vector<std::vector<float>>& distances,   // output: nx vectors of length k
    std::vector<std::vector<int64_t>>& indices    // output: nx vectors of length k
) {
    // Resize output to hold results for each query.
    distances.resize(nx);
    indices.resize(nx);

    for (size_t qi = 0; qi < nx; ++qi) {
        const float* x_i = x + qi * d;

        // Collect all (distance, index) pairs.
        std::vector<std::pair<float, int64_t>> candidates;
        candidates.reserve(ny);
        for (size_t j = 0; j < ny; ++j) {
            const float* y_j = y + j * d;
            float dist = squaredL2(x_i, y_j, d);
            candidates.emplace_back(dist, static_cast<int64_t>(j));
        }

        // Sort by distance, then by index.
        std::sort(candidates.begin(), candidates.end(),
                  [](const std::pair<float, int64_t>& a,
                     const std::pair<float, int64_t>& b) {
                      if (a.first != b.first) {
                          return a.first < b.first;
                      }
                      return a.second < b.second;
                  });

        // Take the first k results.
        distances[qi].resize(k);
        indices[qi].resize(k);
        for (size_t r = 0; r < k; ++r) {
            distances[qi][r] = candidates[r].first;
            indices[qi][r] = candidates[r].second;
        }
    }
}

#include <cassert>
#include <vector>
#include <cstdint>
#include <cmath>

// The solution function is declared here (or include the header).
void findKNearestL2(...); // (declaration omitted for brevity, but assumed present)

int main() {
    // Test 1: simple 2D points, k=1
    {
        std::vector<float> x = {0.0f, 0.0f};  // one query at origin
        std::vector<float> y = {1.0f, 0.0f,   // database points
                                0.0f, 1.0f,
                                -1.0f, 0.0f};
        size_t nx = 1, ny = 3, d = 2, k = 1;
        std::vector<std::vector<float>> dist;
        std::vector<std::vector<int64_t>> idx;
        findKNearestL2(x.data(), nx, y.data(), ny, d, k, dist, idx);
        assert(dist.size() == 1 && idx.size() == 1);
        assert(dist[0].size() == 1 && idx[0].size() == 1);
        assert(std::fabs(dist[0][0] - 1.0f) < 1e-5);
        assert(idx[0][0] == 0);  // all database points have distance 1, smallest index is 0
    }

    // Test 2: tie-breaking by smaller index
    {
        std::vector<float> x = {0.0f, 0.0f};
        std::vector<float> y = {1.0f, 0.0f,
                                0.0f, 1.0f,
                                0.0f, 0.0f,   // exact match (distance 0)
                                0.5f, 0.5f};
        size_t nx = 1, ny = 4, d = 2, k = 2;
        std::vector<std::vector<float>> dist;
        std::vector<std::vector<int64_t>> idx;
        findKNearestL2(x.data(), nx, y.data(), ny, d, k, dist, idx);
        // Expected nearest: index 2 (distance 0), then index 0 (distance 1)
        assert(dist[0][0] < 1e-5 && idx[0][0] == 2);
        assert(std::fabs(dist[0][1] - 1.0f) < 1e-5 && idx[0][1] == 0);
    }

    // Test 3: multiple queries, k > 1
    {
        std::vector<float> x = {0.0f, 0.0f,
                                2.0f, 0.0f};   // two queries
        std::vector<float> y = {1.0f, 0.0f,
                                0.0f, 1.0f,
                                2.0f, 0.0f,
                                3.0f, 0.0f};
        size_t nx = 2, ny = 4, d = 2, k = 2;
        std::vector<std::vector<float>> dist;
        std::vector<std::vector<int64_t>> idx;
        findKNearestL2(x.data(), nx, y.data(), ny, d, k, dist, idx);
        // Query 0: distances to y are [1,1,4,9] -> sorted indexes [0,1] (tie)
        assert(dist[0].size() == 2 && idx[0].size() == 2);
        assert(std::fabs(dist[0][0] - 1.0f) < 1e-5 && idx[0][0] == 0);
        assert(std::fabs(dist[0][1] - 1.0f) < 1e-5 && idx[0][1] == 1);
        // Query 1: distances are [1,5,0,1] -> sorted indexes [2,0] (tie at 1)
        assert(std::fabs(dist[1][0] - 0.0f) < 1e-5 && idx[1][0] == 2);
        assert(std::fabs(dist[1][1] - 1.0f) < 1e-5 && idx[1][1] == 0);
    }

    // Test 4: k equals ny (all vectors returned)
    {
        std::vector<float> x = {1.0f, 1.0f};
        std::vector<float> y = {2.0f, 2.0f,
                                0.0f, 0.0f};
        size_t nx = 1, ny = 2, d = 2, k = 2;
        std::vector<std::vector<float>> dist;
        std::vector<std::vector<int64_t>> idx;
        findKNearestL2(x.data(), nx, y.data(), ny, d, k, dist, idx);
        assert(dist[0][0] == 0.0f && idx[0][0] == 1);  // (0,0) has distance 2
        assert(std::fabs(dist[0][1] - 2.0f) < 1e-5 && idx[0][1] == 0);
    }

    // Test 5: larger dimension, ensures correct sum
    {
        std::vector<float> x = {1.0f, 2.0f, 3.0f, 4.0f};  // one query, d=4
        std::vector<float> y = {1.0f, 2.0f, 3.0f, 4.0f,     // exact match
                                1.0f, 2.0f, 3.0f, 5.0f};    // distance 1
        size_t nx = 1, ny = 2, d = 4, k = 1;
        std::vector<std::vector<float>> dist;
        std::vector<std::vector<int64_t>> idx;
        findKNearestL2(x.data(), nx, y.data(), ny, d, k, dist, idx);
        assert(std::fabs(dist[0][0] - 0.0f) < 1e-5 && idx[0][0] == 0);
    }

    return 0;
}

// The core algorithm is a brute-force exhaustive search over all query-database pairs. For each query vector `x_i`, compute the squared L2 distance to every database vector `y_j` by iterating over each dimension and accumulating `(a - b)^2` with floating-point arithmetic. Collect all `ny` distances and corresponding indices. Then select the `k` smallest distances, breaking ties by smaller index. A simple approach is to store pairs `(distance, index)` in a vector, sort them with `std::sort` using a comparator that first compares distance and then index, and then take the first `k` entries. This yields correct results and is easy to understand. Edge cases: ensure that when computing the squared distance, the sum is initialized to `0.0f` and that each term is computed as `float` to avoid overflow (the problem size is modest). If `k == ny`, all database vectors are returned for each query. Time complexity is `O(nx * ny * d)` for distance computations plus `O(nx * ny log(ny))` for sorting each query's distances, which is acceptable for teaching. Space complexity is `O(ny)` per query to store distances plus `O(k)` for the output, so total `O(ny + k)` auxiliary per query, and `O(nx * k)` for the output.
