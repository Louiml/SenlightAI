// Given a non-empty vector of integers representing class labels (1-indexed, in the range 1 to c, where c is the number of classes) and a symmetric positive semi-definite kernel matrix K of size n×n (stored in column-major order), and model parameters V (size c×n) and weight vector w (size n, each 1/n), write a standalone C++ function `computeMultiLogisticProbabilities` that computes the c×n probability matrix P using the formula P = softmax(V × K) — that is, matrix multiply V (c×n) by K (n×n) to get scores S (c×n), then apply the multiclass logistic (softmax) transformation column-wise: for each column j, subtract the maximum score in that column (for numerical stability), take exponentials, and normalize so each column sums to 1. The function must return the resulting probability matrix as a `std::vector<double>` in column-major order (size c*n). The input parameters are: `const std::vector<double>& V`, `const std::vector<double>& K`, `int c`, `int n`. The function should be self-contained, include only necessary headers (`vector`, `cmath`, `algorithm`), and be `const`-correct (accept const references). Edge cases: c≥1, n≥1, inputs have exactly c*n and n*n elements respectively; if a column's maximum is -∞ (should not happen), handle gracefully by setting all probabilities in that column to 1/c.

The problem is essentially implementing a matrix multiplication followed by a column-wise softmax transformation, which is a core operation in multiclass logistic regression. The main algorithm: first, initialize a result vector `scores` of size c*n with zeros. Perform standard triple-loop matrix multiplication: for each column j (0..n-1) and each row i (0..c-1), compute `scores[i + j*c] = sum_{k=0}^{n-1} V[i + k*c] * K[k + j*n]` — note column-major storage: element (row, col) is at index `row + col*rows`. This is O(c*n^2) time and O(c*n) auxiliary space for the scores, plus the returned vector. Then, for each column j, find the maximum score among rows i: `maxVal = max(scores[i+j*c] for i=0..c-1)`. Subtract maxVal from each entry in the column, compute `exp(score - maxVal)`, sum these exponentials, and divide each by the sum. Edge cases: if c=1, softmax over 1 element always returns 1.0; if all scores are -∞ (extremely unlikely, but possible if inputs are huge negative values, though exp handles underflow to 0, and if maxVal is -∞ then all exp(score-maxVal) become NaN; to be safe, check `std::isfinite(maxVal)` and if not, set all probabilities in that column to 1.0/c). Also, handle input validation minimally: assume the caller provides correct sizes. Time complexity O(c*n^2) for the multiplication, plus O(c*n) for softmax. Space complexity O(c*n) for the scores vector (which is also the returned vector).

#include <vector>
#include <cmath>
#include <algorithm>

/**
 * Computes the multiclass logistic (softmax) probabilities given model parameters V
 * and kernel matrix K. Returns a column-major probability matrix of size c*n.
 * 
 * @param V Model parameters, size c*n, column-major (row-major index: i + j*c)
 * @param K Kernel matrix, size n*n, column-major (row-major index: i + j*n)
 * @param c Number of classes
 * @param n Number of samples
 * @return Probability matrix P of size c*n, column-major, each column sums to 1
 */
std::vector<double> computeMultiLogisticProbabilities(
    const std::vector<double>& V,
    const std::vector<double>& K,
    int c,
    int n) {
    
    std::vector<double> scores(c * n, 0.0);
    
    // Matrix multiplication: scores = V * K, where V is c x n, K is n x n
    // Column-major: scores[i + j*c] = sum_{k=0}^{n-1} V[i + k*c] * K[k + j*n]
    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < c; ++i) {
            double sum = 0.0;
            for (int k = 0; k < n; ++k) {
                sum += V[i + k * c] * K[k + j * n];
            }
            scores[i + j * c] = sum;
        }
    }
    
    // Apply softmax column-wise
    for (int j = 0; j < n; ++j) {
        // Find maximum score in this column for numerical stability
        double maxVal = scores[0 + j * c];
        for (int i = 1; i < c; ++i) {
            maxVal = std::max(maxVal, scores[i + j * c]);
        }
        
        // Handle degenerate case where maxVal is not finite (e.g., -inf)
        if (!std::isfinite(maxVal)) {
            // All probabilities equal to 1/c
            for (int i = 0; i < c; ++i) {
                scores[i + j * c] = 1.0 / c;
            }
            continue;
        }
        
        // Compute exponentials and sum
        double sumExp = 0.0;
        for (int i = 0; i < c; ++i) {
            double val = std::exp(scores[i + j * c] - maxVal);
            scores[i + j * c] = val;
            sumExp += val;
        }
        
        // Normalize
        for (int i = 0; i < c; ++i) {
            scores[i + j * c] /= sumExp;
        }
    }
    
    return scores;
}

#include <cassert>
#include <cmath>
#include <vector>

// Declaration of the function under test
std::vector<double> computeMultiLogisticProbabilities(
    const std::vector<double>& V,
    const std::vector<double>& K,
    int c,
    int n);

int main() {
    // Test 1: Simple 2 classes, 2 samples
    // V = [[0, 0], [0, 0]] (all zeros), K = identity-ish
    {
        std::vector<double> V = {0, 0, 0, 0}; // c=2, n=2
        std::vector<double> K = {1, 0, 0, 1}; // identity 2x2
        int c = 2, n = 2;
        auto P = computeMultiLogisticProbabilities(V, K, c, n);
        // Each column should be [0.5, 0.5]
        assert(std::abs(P[0] - 0.5) < 1e-9);
        assert(std::abs(P[1] - 0.5) < 1e-9);
        assert(std::abs(P[2] - 0.5) < 1e-9);
        assert(std::abs(P[3] - 0.5) < 1e-9);
    }
    
    // Test 2: 3 classes, 1 sample, check softmax shifts
    {
        std::vector<double> V = {1, 2, 3}; // c=3, n=1
        std::vector<double> K = {2}; // 1x1
        int c = 3, n = 1;
        auto P = computeMultiLogisticProbabilities(V, K, c, n);
        // scores = [2, 4, 6]; softmax = [e^0/(e^0+e^2+e^4), e^2/(...), e^4/(...)]
        double e0 = 1.0, e2 = std::exp(2.0), e4 = std::exp(4.0);
        double sum = e0 + e2 + e4;
        assert(std::abs(P[0] - e0/sum) < 1e-9);
        assert(std::abs(P[1] - e2/sum) < 1e-9);
        assert(std::abs(P[2] - e4/sum) < 1e-9);
        // Check sum to 1
        assert(std::abs(P[0] + P[1] + P[2] - 1.0) < 1e-9);
    }
    
    // Test 3: Edge case with single class
    {
        std::vector<double> V = {5.0}; // c=1, n=1
        std::vector<double> K = {1.0};
        int c = 1, n = 1;
        auto P = computeMultiLogisticProbabilities(V, K, c, n);
        assert(std::abs(P[0] - 1.0) < 1e-9);
    }
    
    // Test 4: Larger random-like case, verify column sums
    {
        int c = 4, n = 3;
        std::vector<double> V = {
            0.1, -0.2, 0.3, 0.4,  // sample 0
            0.5, -0.1, 0.2, -0.3, // sample 1
            0.0, 0.0, 0.0, 0.0    // sample 2
        };
        std::vector<double> K = {
            1.0, 0.5, 0.0,
            0.5, 1.0, 0.5,
            0.0, 0.5, 1.0
        };
        auto P = computeMultiLogisticProbabilities(V, K, c, n);
        // Check that each column sums to 1
        for (int j = 0; j < n; ++j) {
            double sum = 0.0;
            for (int i = 0; i < c; ++i) {
                sum += P[i + j * c];
                assert(P[i + j * c] >= 0.0 && P[i + j * c] <= 1.0);
            }
            assert(std::abs(sum - 1.0) < 1e-9);
        }
    }
    
    // Test 5: Numerical stability with very large scores
    {
        std::vector<double> V = {1000.0, 1000.0, 999.0}; // c=3, n=1
        std::vector<double> K = {1.0};
        auto P = computeMultiLogisticProbabilities(V, K, 3, 1);
        // After subtracting max (1000), scores become [0,0,-1] -> softmax [~0.422, ~0.422, ~0.155]
        double e0 = std::exp(0.0);
        double en1 = std::exp(-1.0);
        double sum = e0 + e0 + en1;
        assert(std::abs(P[0] - e0/sum) < 1e-9);
        assert(std::abs(P[1] - e0/sum) < 1e-9);
        assert(std::abs(P[2] - en1/sum) < 1e-9);
        assert(std::abs(P[0] + P[1] + P[2] - 1.0) < 1e-9);
    }
    
    return 0;
}
