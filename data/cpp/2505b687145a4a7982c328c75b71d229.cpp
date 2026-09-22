Write a C++ function `rbfKernelMatrix` that takes a vector of vector<double> representing dense feature vectors, a positive double `gamma`, and returns a vector<vector<double>> containing the RBF (radial basis function) kernel matrix. For each pair of samples `i` and `j`, the entry at `[i][j]` must equal `exp(-gamma * ||x_i - x_j||^2)`, where `||x_i - x_j||^2` is the squared Euclidean distance. The matrix must be symmetric and the diagonal entries must be exactly `1.0` (since distance of a vector to itself is zero). Handle an empty input vector by returning an empty matrix. Ensure all computations use double precision.

#include <cassert>
#include <vector>
#include <cmath>

// Declare the function from the solution (duplicate here for compilation)
std::vector<std::vector<double>> rbfKernelMatrix(
    const std::vector<std::vector<double>>& features, double gamma);

int main() {
    // Empty input returns empty matrix
    assert(rbfKernelMatrix({}, 0.5).empty());

    // Single feature vector -> 1x1 matrix with value 1.0
    {
        auto m = rbfKernelMatrix({{1.0, 2.0}}, 0.3);
        assert(m.size() == 1);
        assert(m[0].size() == 1);
        assert(std::fabs(m[0][0] - 1.0) < 1e-12);
    }

    // Two identical vectors -> all entries 1.0
    {
        std::vector<std::vector<double>> feat = {{1.0, 2.0}, {1.0, 2.0}};
        auto m = rbfKernelMatrix(feat, 1.0);
        assert(m.size() == 2);
        assert(std::fabs(m[0][0] - 1.0) < 1e-12);
        assert(std::fabs(m[1][1] - 1.0) < 1e-12);
        assert(std::fabs(m[0][1] - 1.0) < 1e-12);
        assert(std::fabs(m[1][0] - 1.0) < 1e-12);
    }

    // Two distinct vectors: verify known value
    // x0 = (0,0), x1 = (1,0), gamma=1 -> exp(-1) ≈ 0.367879
    {
        std::vector<std::vector<double>> feat = {{0.0, 0.0}, {1.0, 0.0}};
        auto m = rbfKernelMatrix(feat, 1.0);
        double expected = std::exp(-1.0);
        assert(std::fabs(m[0][1] - expected) < 1e-12);
        assert(std::fabs(m[1][0] - expected) < 1e-12);
        assert(std::fabs(m[0][0] - 1.0) < 1e-12);
        assert(std::fabs(m[1][1] - 1.0) < 1e-12);
    }

    // Three points with gamma=0.5, check symmetry and diagonal
    {
        std::vector<std::vector<double>> feat = {{1.0, 1.0}, {2.0, 2.0}, {3.0, 3.0}};
        auto m = rbfKernelMatrix(feat, 0.5);
        assert(m.size() == 3);
        for (size_t i = 0; i < 3; ++i) {
            assert(std::fabs(m[i][i] - 1.0) < 1e-12);
            for (size_t j = 0; j < 3; ++j) {
                assert(std::fabs(m[i][j] - m[j][i]) < 1e-12);
            }
        }
        // Dist between (1,1) and (2,2) is sqrt(2), squared = 2, gamma=0.5 -> exp(-1)
        double expected12 = std::exp(-1.0);
        assert(std::fabs(m[0][1] - expected12) < 1e-12);
        // Dist between (1,1) and (3,3) is sqrt(8), squared=8, gamma=0.5 -> exp(-4)
        double expected13 = std::exp(-4.0);
        assert(std::fabs(m[0][2] - expected13) < 1e-12);
        // Dist between (2,2) and (3,3) is sqrt(2), squared=2 -> exp(-1)
        assert(std::fabs(m[1][2] - expected12) < 1e-12);
    }

    // Larger gamma produces smaller off-diagonal values
    {
        std::vector<std::vector<double>> feat = {{0.0}, {1.0}};
        auto m1 = rbfKernelMatrix(feat, 0.1);
        auto m2 = rbfKernelMatrix(feat, 10.0);
        assert(m1[0][1] > m2[0][1]);
    }

    return 0;
}

#include <vector>
#include <cmath>
#include <stdexcept>

// Compute the RBF kernel matrix for dense feature vectors.
// Input: rows of feature vectors (vector of vector<double>)
// gamma: positive scaling parameter
// Returns an n x n matrix where entry (i, j) = exp(-gamma * ||xi - xj||^2)
std::vector<std::vector<double>> rbfKernelMatrix(
    const std::vector<std::vector<double>>& features,
    double gamma) 
{
    if (features.empty()) {
        return {};
    }

    const size_t n = features.size();
    const size_t dim = features[0].size();

    // Validate uniform dimension (reasonable for dense data)
    for (size_t i = 1; i < n; ++i) {
        if (features[i].size() != dim) {
            throw std::invalid_argument("All feature vectors must have the same length");
        }
    }

    // Precompute squared norms of each row: ||x_i||^2
    std::vector<double> norm_sq(n, 0.0);
    for (size_t i = 0; i < n; ++i) {
        double sum = 0.0;
        for (size_t k = 0; k < dim; ++k) {
            sum += features[i][k] * features[i][k];
        }
        norm_sq[i] = sum;
    }

    std::vector<std::vector<double>> kernel(n, std::vector<double>(n, 0.0));

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i; j < n; ++j) {
            double dot = 0.0;
            for (size_t k = 0; k < dim; ++k) {
                dot += features[i][k] * features[j][k];
            }
            double sq_dist = norm_sq[i] + norm_sq[j] - 2.0 * dot;
            // Avoid tiny negative values due to floating point rounding
            if (sq_dist < 0.0 && sq_dist > -1e-12) sq_dist = 0.0;
            double val = std::exp(-gamma * sq_dist);
            kernel[i][j] = val;
            kernel[j][i] = val; // symmetric
        }
    }

    return kernel;
}

// The solution computes the squared Euclidean distance between every pair of rows. For each row `i` and row `j`, we sum `(x[i][k] - x[j][k])^2` over all features, and apply `exp(-gamma * sum)`. A key optimization is to precompute each row's squared norm (dot product of the row with itself) so that the squared distance can be obtained via the identity `||a - b||^2 = ||a||^2 + ||b||^2 - 2 * dot(a, b)`. This avoids recomputing the full difference per pair. However, for clarity and correctness, the direct difference loop is simple and acceptable for the task's scope; complexity is `O(n^2 * d)` for `n` samples and `d` features. We must handle the case where feature vectors may have differing lengths — the task implicitly assumes uniform lengths given dense data, but we can defensively use the minimum length or throw; here we assume all rows have equal length and use that length directly. Important edge cases: empty input → empty output; a single sample → a 1x1 matrix with value `1.0`; gamma must be positive (if not, we could return empty or assert, but the task specifies a positive gamma so no need). Time complexity is `O(n^2 * d)` and space complexity is `O(n^2)` for the output matrix.
