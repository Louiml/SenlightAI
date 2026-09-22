// Implement a standalone C++ function `stochasticProximityEmbedding` that performs a simplified version of the code above: given an \( n \times n \) symmetric distance matrix `distMatrix` (with zeros on the diagonal) and a target output dimension `outputDim`, return an \( n \times outputDim \) matrix `Y` of embedded coordinates using a stochastic gradient descent approach. The function must initialize `Y` with random values in [0,1], then for `max_iter` iterations perform the following: pick `s` random unordered pairs of indices (distinct), compute their Euclidean distance in embedded space, compute the relative error `(realDistance − embeddedDistance) / (embeddedDistance + tol)`, and update both points’ coordinates along the direction between them scaled by a decreasing learning rate `lambda` (starting at 1, decreasing linearly to near 0 over iterations). The number of iterations must be `max_iter = 20000 + round(0.04 * n * n)`, and if a boolean `global` is true, multiply `max_iter` by 3. Use `tol = 1e-5`, `s = 100` (but if `n <= 100`, use `s = n-1`), and ensure pairs are chosen without replacement each iteration (i.e., draw a random permutation of all indices and take consecutive pairs from it). Return the final `Y` matrix. Do not use external libraries beyond `<vector>`, `<random>`, `<cmath>`, `<algorithm>`, and `<cstddef>`.

// The algorithm is a stochastic approximation of multidimensional scaling (MDS). For each iteration, we pick `s` random disjoint pairs of points. For each pair (i1,i2), we compute the Euclidean distance `D` between their current embedded vectors. We also look up the true distance `Rt` from the input matrix. The update rule moves both points along the vector connecting them: each point moves toward the other if the embedded distance is too small (D < Rt) and away if too large (D > Rt). The magnitude of movement is `lambda * (Rt - D) / (D + tol)`, scaled by 0.5 per point. The learning rate `lambda` decreases linearly from 1 to near 0. The number of iterations is proportional to n² (cubic if global). Important edge cases: if `n <= 1`, return an empty or single-row matrix (since no pairs exist). If `outputDim` is 0 or negative, return empty. If `n == 2`, only one pair exists, so `s` becomes 1; the random permutation approach still works by taking the first pair. Time complexity is O(max_iter * s * outputDim), which with typical n is large but acceptable for this task; space complexity is O(n²) for the distance matrix plus O(n * outputDim) for Y.

#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <cstddef>

// Stochastic Proximity Embedding: map n points to outputDim dimensions.
// distMatrix is an n x n symmetric matrix with zeros on diagonal.
// Returns an n x outputDim matrix Y (as vector<vector<double>>).
std::vector<std::vector<double>> stochasticProximityEmbedding(
    const std::vector<std::vector<double>>& distMatrix,
    int outputDim,
    bool global = false)
{
    const std::size_t n = distMatrix.size();
    if (n == 0 || outputDim <= 0) return {};

    // If n==1, just return a single point of zeros (or random? spec says random init, but no updates)
    if (n == 1) {
        return std::vector<std::vector<double>>(1, std::vector<double>(outputDim, 0.0));
    }

    const double tol = 1e-5;
    const int s = (n > 100) ? 100 : static_cast<int>(n) - 1; // number of pairs per iteration
    int max_iter = 20000 + static_cast<int>(std::round(0.04 * static_cast<double>(n) * static_cast<double>(n)));
    if (global) max_iter *= 3;
    double lambda = 1.0;

    // Initialize Y randomly in [0,1]
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::vector<std::vector<double>> Y(n, std::vector<double>(outputDim));
    for (std::size_t i = 0; i < n; ++i)
        for (int j = 0; j < outputDim; ++j)
            Y[i][j] = dist(gen);

    // Pre-allocate indices and arrays for pairs
    std::vector<int> indices(n);
    for (std::size_t i = 0; i < n; ++i) indices[i] = static_cast<int>(i);
    std::vector<int> ind1(s), ind2(s);
    std::vector<double> D(s), Rt(s), W(s);

    for (int iter = 0; iter < max_iter; ++iter) {
        // Random permutation of all indices
        std::shuffle(indices.begin(), indices.end(), gen);

        // Fill ind1 and ind2 with consecutive pairs from permutation
        for (int p = 0; p < s; ++p) {
            ind1[p] = indices[2 * p];
            ind2[p] = indices[2 * p + 1];
        }

        // Compute embedded distances and get true distances
        for (int p = 0; p < s; ++p) {
            int i1 = ind1[p], i2 = ind2[p];
            double sum_sq = 0.0;
            for (int j = 0; j < outputDim; ++j) {
                double diff = Y[i1][j] - Y[i2][j];
                sum_sq += diff * diff;
            }
            D[p] = std::sqrt(sum_sq);
            Rt[p] = distMatrix[i1][i2];
            W[p] = (Rt[p] - D[p]) / (D[p] + tol);
        }

        // Update positions
        double factor = lambda * 0.5;
        for (int p = 0; p < s; ++p) {
            int i1 = ind1[p], i2 = ind2[p];
            double w = W[p];
            for (int j = 0; j < outputDim; ++j) {
                double diff = Y[i1][j] - Y[i2][j];
                double new_i1 = Y[i1][j] + factor * w * diff;
                Y[i2][j] += factor * w * (Y[i2][j] - Y[i1][j]);
                Y[i1][j] = new_i1;
            }
        }

        // Decay learning rate
        lambda -= lambda / max_iter;
    }

    return Y;
}

#include <cassert>
#include <vector>
#include <cmath>

// Helper to compute Euclidean distance between two rows of Y
double euclid(const std::vector<double>& a, const std::vector<double>& b) {
    double sum = 0.0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        double d = a[i] - b[i];
        sum += d * d;
    }
    return std::sqrt(sum);
}

int main() {
    // Test 1: n=2, outputDim=1, trivial distance 2 (points should end up roughly 2 apart)
    {
        std::vector<std::vector<double>> dist = {{0, 2}, {2, 0}};
        auto Y = stochasticProximityEmbedding(dist, 1, false);
        assert(Y.size() == 2);
        assert(Y[0].size() == 1);
        double d = std::abs(Y[0][0] - Y[1][0]);
        assert(d > 1.5 && d < 2.5);
    }

    // Test 2: n=3, outputDim=2, equilateral triangle side 1 (distances all 1)
    {
        std::vector<std::vector<double>> dist = {
            {0, 1, 1},
            {1, 0, 1},
            {1, 1, 0}
        };
        auto Y = stochasticProximityEmbedding(dist, 2, false);
        assert(Y.size() == 3);
        assert(Y[0].size() == 2);
        double d01 = euclid(Y[0], Y[1]);
        double d02 = euclid(Y[0], Y[2]);
        double d12 = euclid(Y[1], Y[2]);
        assert(d01 > 0.5 && d01 < 1.5);
        assert(d02 > 0.5 && d02 < 1.5);
        assert(d12 > 0.5 && d12 < 1.5);
    }

    // Test 3: n=1, returns single row of zeros (not in spec but safe)
    {
        std::vector<std::vector<double>> dist = {{0}};
        auto Y = stochasticProximityEmbedding(dist, 3, false);
        assert(Y.size() == 1);
        assert(Y[0].size() == 3);
        assert(Y[0][0] == 0.0 && Y[0][1] == 0.0 && Y[0][2] == 0.0);
    }

    // Test 4: outputDim=0 returns empty
    {
        std::vector<std::vector<double>> dist = {{0,1},{1,0}};
        auto Y = stochasticProximityEmbedding(dist, 0, false);
        assert(Y.empty());
    }

    // Test 5: n=4, outputDim=3, all distances zero (all points same) -> embedded distances near zero
    {
        std::vector<std::vector<double>> dist(4, std::vector<double>(4, 0.0));
        auto Y = stochasticProximityEmbedding(dist, 3, false);
        assert(Y.size() == 4);
        for (std::size_t i = 0; i < 4; ++i)
            for (std::size_t j = i+1; j < 4; ++j)
                assert(euclid(Y[i], Y[j]) < 0.1);
    }

    // Test 6: n=100, s should be 99 (since n <=100), run quickly with global=false
    {
        std::vector<std::vector<double>> dist(100, std::vector<double>(100, 0.0));
        for (int i = 0; i < 100; ++i)
            for (int j = i+1; j < 100; ++j)
                dist[i][j] = dist[j][i] = 1.0;
        auto Y = stochasticProximityEmbedding(dist, 2, false);
        assert(Y.size() == 100);
        assert(Y[0].size() == 2);
    }

    // Test 7: symmetric check: output dimensions count matches
    {
        std::vector<std::vector<double>> dist = {{0,1},{1,0}};
        auto Y = stochasticProximityEmbedding(dist, 5, true);
        assert(Y.size() == 2);
        assert(Y[0].size() == 5);
    }

    return 0;
}
