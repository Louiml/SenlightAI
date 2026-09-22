// Given a sequence of sample weights and a boolean flag indicating whether each corresponding sample was classified correctly, write a C++ function that computes the AdaBoost-style classifier weight update factor `beta` after one boosting iteration. The function should accept two vectors of equal length: a `std::vector<double>` of non-negative sample weights and a `std::vector<bool>` of prediction outcomes (where `true` means correct, `false` means incorrect). It must return the value `beta = error / (1 - error)`, where `error` is the sum of weights of incorrectly classified samples divided by the total sum of all weights. Handle edge cases: if the total weight sum is zero, return `0.0`; if the error is exactly `1.0` (all samples misclassified), return a very large value (e.g., `1e9`) to avoid division by zero; if the error is exactly `0.0` (all correct), return `0.0`. The function must be const-correct and operate without modifying the input vectors, and should not use global state.
#include <cassert>
#include <vector>

// The solution function is declared above or included here.
int main() {
    // Basic case: equal weights, one misclassified out of four.
    std::vector<double> w1 = {1.0, 1.0, 1.0, 1.0};
    std::vector<bool> o1 = {true, false, true, true};
    double result = compute_boosting_beta(w1, o1);
    // error = 0.25, beta = 0.25/0.75 = 1/3.
    assert(result > 0.3333 && result < 0.3334);

    // All correct: beta should be 0.
    std::vector<double> w2 = {0.5, 1.5, 2.0};
    std::vector<bool> o2 = {true, true, true};
    assert(compute_boosting_beta(w2, o2) == 0.0);

    // All incorrect: beta should be large (sentinel).
    std::vector<double> w3 = {1.0, 2.0, 3.0};
    std::vector<bool> o3 = {false, false, false};
    assert(compute_boosting_beta(w3, o3) == 1e9);

    // Weighted error: only large-weight sample misclassified.
    std::vector<double> w4 = {0.1, 0.1, 0.8};
    std::vector<bool> o4 = {true, true, false};
    // error = 0.8, beta = 0.8/0.2 = 4.0.
    assert(compute_boosting_beta(w4, o4) > 3.9999 && compute_boosting_beta(w4, o4) < 4.0001);

    // Zero total weight: return 0.
    std::vector<double> w5 = {0.0, 0.0, 0.0};
    std::vector<bool> o5 = {false, true, false};
    assert(compute_boosting_beta(w5, o5) == 0.0);

    // Empty vectors (both empty): min length 0, total_sum = 0, return 0.
    std::vector<double> w6;
    std::vector<bool> o6;
    assert(compute_boosting_beta(w6, o6) == 0.0);

    // Mismatched lengths: use shorter (first 2 weights, first 2 outcomes).
    std::vector<double> w7 = {1.0, 1.0, 100.0};
    std::vector<bool> o7 = {false, true};
    // error = 0.5, beta = 0.5/0.5 = 1.0.
    assert(compute_boosting_beta(w7, o7) == 1.0);

    // Negative weights (though not expected, the code still works): 
    // total_sum = -1, error_sum = -1 (if all false), so total <=0 returns 0.
    std::vector<double> w8 = {-1.0, -1.0};
    std::vector<bool> o8 = {false, false};
    assert(compute_boosting_beta(w8, o8) == 0.0);

    // Single sample, correct with weight 5.
    std::vector<double> w9 = {5.0};
    std::vector<bool> o9 = {true};
    assert(compute_boosting_beta(w9, o9) == 0.0);

    // Single sample, incorrect with weight 5.
    std::vector<double> w10 = {5.0};
    std::vector<bool> o10 = {false};
    assert(compute_boosting_beta(w10, o10) == 1e9);

    return 0;
}
#include <vector>
#include <algorithm>

// Compute the AdaBoost beta value for one iteration.
// weights: sample weights (non-negative), outcomes: true if correctly classified, false otherwise.
// Returns beta = error / (1 - error), with special handling for edge cases.
double compute_boosting_beta(const std::vector<double>& weights, const std::vector<bool>& outcomes) {
    // Assume inputs have equal length; if not, the shorter length is used.
    const size_t n = std::min(weights.size(), outcomes.size());

    double total_sum = 0.0;
    double error_sum = 0.0;

    for (size_t i = 0; i < n; ++i) {
        total_sum += weights[i];
        if (!outcomes[i]) { // incorrect classification
            error_sum += weights[i];
        }
    }

    // If no total weight, no meaningful error.
    if (total_sum <= 0.0) {
        return 0.0;
    }

    double error = error_sum / total_sum;

    // Perfect classifier: beta = 0.
    if (error == 0.0) {
        return 0.0;
    }

    // All misclassified: beta tends to infinity, return a large sentinel.
    if (error >= 1.0) {
        return 1e9;
    }

    // Standard beta formula.
    return error / (1.0 - error);
}
// The solution computes the total weight and the weighted error by iterating through both vectors simultaneously. For each index `i`, if the outcome is false (incorrect), add `weights[i]` to `error_sum`; always add `weights[i]` to `total_sum`. After the loop, compute the error rate as `error_sum / total_sum` if `total_sum > 0`, otherwise treat as undefined and return `0.0` (since no data means no learning signal). Then compute `beta = error / (1 - error)`. Edge cases: if `error` is exactly `0.0`, return `0.0` (perfect classifier, weight update shrinks to zero). If `error` is exactly `1.0`, the denominator becomes zero; return a large sentinel value (e.g., `1e9`) to represent an infinite beta, which in practice would indicate that the weak learner is worse than random and should be reversed or discarded. Also, if `total_sum` is zero, return `0.0` to avoid NaN. Time complexity is `O(n)` where `n` is the number of samples; space complexity is `O(1)` auxiliary, excluding the input vectors. The constraints of non-negative weights are assumed; the code does not validate negativity but would naturally handle it if present.
