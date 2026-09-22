// Write a standalone C++ function named `simulateCategoricalNoise` that accepts a numeric matrix `S` (representing parent category values, with dimensions `q-1` by `p`), a numeric matrix `A` (representing probability weights, with dimensions `n` by `q`), and an integer `K` (the number of categories, assumed ≥1). For each row `i` (0-indexed) and each column `j`, the function must generate a random integer category in the range `{0, 1, …, K-1}` according to the following categorical distribution: start with a base probability `A(i, q-1) / K` (the last column of `A` is the noise weight divided equally among all categories). Then, for each row index `h` from `0` to `q-2` (i.e., the first `q-1` rows of `S`), if `S(h, j) == k` for a candidate category `k`, add `A(i, h)` to the probability of category `k`. The function must return an `n` by `p` integer matrix (represented as a `std::vector<std::vector<int>>` or a 2D array struct) containing the sampled categories, where each cell is an integer between `0` and `K-1` inclusive. The sampling must use a uniform random number generator, and probabilities are not required to sum to 1 (they will be normalized by the sampling mechanism, but ensure no negative probabilities – if any probability is negative, clamp it to 0). The input matrices are provided as flat `std::vector<double>` with dimensions given separately (`qMinus1` rows for `S`, `n` rows for `A`). The function signature must be: `std::vector<std::vector<int>> simulateCategoricalNoise(const std::vector<double>& S, int S_rows, int S_cols, const std::vector<double>& A, int A_rows, int A_cols, int K)`. Assume all dimensions are valid: `S_rows == A_cols - 1`, `S_cols == A_rows`? Actually check: `S` has dimensions `(q-1) x p` and `A` has dimensions `n x q`, so `S_rows == A_cols - 1`, `S_cols == p`, `A_rows == n`, `A_cols == q`. The function must be deterministic given a fixed random seed, so accept an additional `int seed` parameter as the last argument (default 0) to seed a `std::mt19937` generator. The output matrix must have exactly `A_rows` rows and `S_cols` columns.
#include <cassert>
#include <cmath>

int main() {
    // Test 1: K=1, anything maps to 0
    {
        std::vector<double> S = {0, 1, 2}; // S_rows=1, S_cols=3 (but K=1 so S values ignored)
        std::vector<double> A = {0.5, 0.5, 0.2, 0.8}; // 2 rows, 2 cols (q=2, so A_cols-1=1 matches S_rows)
        auto res = simulateCategoricalNoise(S, 1, 3, A, 2, 2, 1, 42);
        for (auto& row : res) for (int v : row) assert(v == 0);
    }

    // Test 2: Simple deterministic case with K=2, one parent row, one column
    // S = [[0]] (parent category 0 for the single column)
    // A = [[1.0, 0.0], [0.0, 1.0]] (row0: parent weight 1, base noise 0; row1: parent weight 0, base noise 1)
    // With K=2, row0 should give category 0 always (prob[0] = 0/2 + 1 = 1, prob[1]=0)
    // Row1 should give uniform between 0 and 1 (since base noise weight 1, /2=0.5 each)
    {
        std::vector<double> S = {0}; // 1 row, 1 col
        std::vector<double> A = {1.0, 0.0, 0.0, 1.0}; // 2 rows, 2 cols
        auto res = simulateCategoricalNoise(S, 1, 1, A, 2, 2, 2, 0);
        assert(res.size() == 2);
        assert(res[0].size() == 1);
        assert(res[0][0] == 0); // Always 0 from parent weight
        // For row1, we can't assert exact value, but just assert it's 0 or 1
        assert(res[1][0] == 0 || res[1][0] == 1);
    }

    // Test 3: K=3, S has two parents (S_rows=2), one column, A has 3 cols (q=3)
    // S = [[0], [1]] (parent0 = 0, parent1 = 1)
    // A = [[1.0, 0.0, 0.0]] (only one row, parent0 weight 1, parent1 weight 0, base noise 0)
    // So for category 0: prob = 0/3 + A(i,0) = 1; category 1: prob = 0 + A(i,1)=0; category 2: prob=0
    // Should always output 0
    {
        std::vector<double> S = {0, 1}; // 2 rows, 1 col
        std::vector<double> A = {1.0, 0.0, 0.0}; // 1 row, 3 cols
        auto res = simulateCategoricalNoise(S, 2, 1, A, 1, 3, 3, 123);
        assert(res.size() == 1);
        assert(res[0][0] == 0);
    }

    // Test 4: Negative probability clamping: A has a negative weight
    // S = [[0]] (1 row, 1 col)
    // A = [[-0.5, 1.0]] (row0: parent weight -0.5, base noise 1.0)
    // K=2, prob[0] = 1/2 + (-0.5) = 0.0, prob[1] = 1/2 = 0.5 -> actually prob[0] is 0, but after clamp still 0
    // sum=0.5, so only category 1 can be chosen
    {
        std::vector<double> S = {0};
        std::vector<double> A = {-0.5, 1.0}; // 1 row, 2 cols
        auto res = simulateCategoricalNoise(S, 1, 1, A, 1, 2, 2, 7);
        assert(res[0][0] == 1);
    }

    // Test 5: All probabilities zero -> uniform fallback
    // S = [[0]] (1 row, 1 col)
    // A = [[0.0, 0.0]] (both weights zero)
    // K=3, should be uniform, but we just check result is in range [0,2]
    {
        std::vector<double> S = {0};
        std::vector<double> A = {0.0, 0.0};
        auto res = simulateCategoricalNoise(S, 1, 1, A, 1, 2, 3, 99);
        assert(res[0][0] >= 0 && res[0][0] <= 2);
    }

    // Test 6: Reproducibility with same seed
    {
        std::vector<double> S = {0, 1, 2}; // 1 row, 3 cols? Actually S_rows=3? Let's set properly: S_rows=3, S_cols=1
        S = {0, 1, 2};
        std::vector<double> A = {0.2, 0.3, 0.5, 0.0}; // 2 rows? Let's keep simple: one row, A_cols=4 (so S_rows=3)
        A = {0.1, 0.2, 0.3, 0.4}; // 1 row, 4 cols
        auto r1 = simulateCategoricalNoise(S, 3, 1, A, 1, 4, 4, 555);
        auto r2 = simulateCategoricalNoise(S, 3, 1, A, 1, 4, 4, 555);
        assert(r1 == r2);
    }

    // Test 7: Larger matrix with multiple rows and columns, check dimensions
    {
        std::vector<double> S = {0, 1, 1, 0}; // 2 rows (q-1), 2 cols (p)
        std::vector<double> A = {0.5, 0.5, 0.2, 0.8, 0.3, 0.7, 0.1, 0.9}; // 2 rows, 4 cols? Wait A_cols=4, S_rows should be 3, but we have S_rows=2. Let's adjust.
        // Let's do: S_rows=3, S_cols=2 => S size 6
        S = {0,1,2,0,1,2}; // 3 rows, 2 cols
        A = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8}; // 2 rows, 4 cols -> A_cols=4, so S_rows should be 3. Yes, 3 rows in S.
        auto res = simulateCategoricalNoise(S, 3, 2, A, 2, 4, 3, 1);
        assert(res.size() == 2);
        assert(res[0].size() == 2);
        assert(res[1].size() == 2);
        for (auto& row : res) {
            for (int v : row) assert(v >= 0 && v <= 2);
        }
    }

    // Test 8: K=1 with negative noise weight, should still be 0
    {
        std::vector<double> S = {5}; // any value, ignored
        std::vector<double> A = {-2.0, 0.0}; // negative weight
        auto res = simulateCategoricalNoise(S, 1, 1, A, 1, 2, 1, 3);
        assert(res[0][0] == 0);
    }

    return 0;
}
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>

/**
 * Simulate categorical noise based on parent categories and weight matrix.
 *
 * S: flat vector of size S_rows * S_cols, representing parent categories (each entry is an integer 0..K-1)
 * A: flat vector of size A_rows * A_cols, where each row i contains weights for each parent (first A_cols-1) plus a base noise weight (last column)
 * K: number of categories (>=1)
 * seed: random seed for reproducibility
 *
 * Returns a matrix (vector of vectors) of size A_rows x S_cols with sampled categories in {0..K-1}.
 */
std::vector<std::vector<int>> simulateCategoricalNoise(
    const std::vector<double>& S, int S_rows, int S_cols,
    const std::vector<double>& A, int A_rows, int A_cols,
    int K, int seed = 0) {

    // Validate dimensions (optional, provided for safety)
    // S_rows should be A_cols - 1, and S_cols is the number of columns
    // A_rows is the number of rows

    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::vector<std::vector<int>> out(A_rows, std::vector<int>(S_cols));

    // Pre-check: if K <= 0, return empty? Assume K>=1 per spec.
    if (K <= 0) return out;

    for (int i = 0; i < A_rows; ++i) {
        for (int j = 0; j < S_cols; ++j) {
            // Compute probability vector for this cell
            std::vector<double> prob(K, 0.0);
            double base = A[i * A_cols + (A_cols - 1)] / static_cast<double>(K);
            for (int k = 0; k < K; ++k) {
                prob[k] = base;
                for (int h = 0; h < S_rows; ++h) {
                    // S has S_rows rows, each of length S_cols
                    double s_val = S[h * S_cols + j];
                    if (static_cast<int>(s_val) == k) {
                        prob[k] += A[i * A_cols + h];
                    }
                }
                // Clamp negative probabilities to 0
                if (prob[k] < 0.0) prob[k] = 0.0;
            }

            // Check if all probabilities are zero, then fallback to uniform
            double sum = std::accumulate(prob.begin(), prob.end(), 0.0);
            if (sum <= 0.0) {
                for (int k = 0; k < K; ++k) prob[k] = 1.0 / K;
                sum = 1.0;
            }

            // Build cumulative distribution
            std::vector<double> cumulative(K);
            std::partial_sum(prob.begin(), prob.end(), cumulative.begin());

            // Sample a random value
            double u = dist(gen) * sum; // scale to total sum
            int chosen = 0;
            for (int k = 0; k < K; ++k) {
                if (u < cumulative[k]) {
                    chosen = k;
                    break;
                }
                chosen = k; // fallback to last if not found (u >= all)
            }
            out[i][j] = chosen;
        }
    }

    return out;
}
// The core algorithm computes, for each cell `(i,j)`, a probability vector of length `K`. We initialize all probabilities to `A[i * A_cols + (A_cols-1)] / K` (the base noise). Then for each category `k` from 0 to `K-1`, we iterate over all rows `h` from 0 to `S_rows-1` (which equals `A_cols - 2`), and if `S[h * S_cols + j] == k` (note: the original code checks `S(h,j)==(k)` and then adds `A(i,h)`), we add `A[i * A_cols + h]` to `prob[k]`. After computing all probabilities, we clamp any negative values to 0, then sample from the categorical distribution using a single uniform random draw. To sample efficiently, we can build the cumulative sum of probabilities and use `std::uniform_real_distribution<double>` to get a value in [0,1) and find the first index where the cumulative sum exceeds that value. Edge cases include `K=1` (only one category always), possible all-zero probabilities (then we choose category 0 or handle by uniform fallback), and empty matrices (but dimensions are assumed valid). Time complexity: for each cell, we compute K probabilities, each requiring O(S_rows) additions, so O(A_rows * S_cols * K * S_rows). Space: we need O(K) for the probability vector and O(A_rows * S_cols) for the output, so total O(A_rows * S_cols + K). The use of a seeded RNG ensures reproducibility. The solution must handle floating-point rounding; clamp negative probabilities and if all probabilities are zero, set all to 1/K.
