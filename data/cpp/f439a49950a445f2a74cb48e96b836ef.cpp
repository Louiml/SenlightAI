Write a C++ function `double probabilityOfAtLeastTarget(const std::vector<double>& prob, int target)` that computes the probability that, after tossing each coin exactly once, the number of heads is at least `target`. The input `prob` is a vector of doubles where each element `prob[i]` (0 ≤ `prob[i]` ≤ 1) is the probability that the i-th coin lands heads. The function should handle any non-empty vector and any `target` from 0 to the size of the vector (inclusive). If `target` is 0, the result must be 1.0. If `target` is greater than the number of coins, the result must be 0.0. Use dynamic programming with a 1D array (optimized from 2D) to compute the probabilities. The result must be accurate within a tolerance of 1e-9 for floating-point comparisons.

// The core idea is a standard dynamic programming over coin tosses. Let `dp[j]` represent the probability of getting exactly `j` heads after processing some prefix of the coins. Initially, before any coin, `dp[0] = 1.0`. For each coin with head probability `p`, we update the DP array from high `j` down to 0 (to avoid reusing the same coin multiple times): `new_dp[j] = dp[j] * (1 - p) + (j > 0 ? dp[j-1] * p : 0)`. The first term is the case where this coin is tails, and the second where it is heads. After processing all coins, we sum `dp[j]` for all `j >= target` to get the probability of at least `target` heads. Edge cases: if `target <= 0`, return 1.0 because it's certain to have at least 0 heads. If `target > n`, return 0.0. If `target == 0`, also return 1.0. For `target` between 1 and `n`, the DP works. Time complexity is O(n * target) because we only need to track up to `target` heads (any more heads beyond target doesn't affect the final sum, but we can simplify by tracking up to `target` and capping at `target` for the sum). However, to keep it simple and correct, we can track up to `n` but it's more efficient to cap at `target` because when computing `dp[j]` for `j > target`, we don't need them; we can just accumulate probabilities of exceeding target into a separate variable. But the straightforward 1D DP updating from `min(i, target)` down to 0 works. Space complexity is O(target+1). We must be careful with floating-point precision; using `double` is fine for this problem.

#include <vector>

// Compute the probability that the number of heads is at least `target`
// after tossing each coin exactly once. prob[i] is the probability of heads for coin i.
double probabilityOfAtLeastTarget(const std::vector<double>& prob, int target) {
    const int n = static_cast<int>(prob.size());
    if (target <= 0) return 1.0;
    if (target > n) return 0.0;

    // dp[j] for j < target stores probability of exactly j heads,
    // dp[target] stores probability of at least target heads.
    std::vector<double> dp(target + 1, 0.0);
    dp[0] = 1.0; // zero coins processed, zero heads certain

    for (double p : prob) {
        // Update from high to low so each coin is used only once.
        for (int j = target; j >= 0; --j) {
            if (j < target) {
                // Exactly j heads: tails on this coin from previous j, heads from previous j-1.
                double tails = dp[j] * (1.0 - p);
                double heads = (j > 0) ? dp[j - 1] * p : 0.0;
                dp[j] = tails + heads;
            } else {
                // j == target: at least target heads.
                // Whether this coin is heads or tails, if we already had at least target,
                // we still have at least target. Also, from exactly target-1 heads plus this coin head.
                double already_at_least = dp[j]; // old value
                double from_target_minus_1 = (j > 0) ? dp[j - 1] * p : 0.0;
                dp[j] = already_at_least * 1.0 + from_target_minus_1;
            }
        }
    }
    return dp[target];
}

#include <cassert>
#include <cmath>
#include <vector>

// The solution function declared here (include the header or define above).
double probabilityOfAtLeastTarget(const std::vector<double>& prob, int target);

int main() {
    // Test 1: Single fair coin, target 1 -> 0.5
    assert(std::abs(probabilityOfAtLeastTarget({0.5}, 1) - 0.5) < 1e-9);

    // Test 2: Single fair coin, target 0 -> 1.0
    assert(std::abs(probabilityOfAtLeastTarget({0.5}, 0) - 1.0) < 1e-9);

    // Test 3: Single fair coin, target 2 (impossible) -> 0.0
    assert(std::abs(probabilityOfAtLeastTarget({0.5}, 2) - 0.0) < 1e-9);

    // Test 4: Two fair coins, target 1 -> 0.75
    assert(std::abs(probabilityOfAtLeastTarget({0.5, 0.5}, 1) - 0.75) < 1e-9);

    // Test 5: Two fair coins, target 2 -> 0.25
    assert(std::abs(probabilityOfAtLeastTarget({0.5, 0.5}, 2) - 0.25) < 1e-9);

    // Test 6: Two coins with probabilities 0.3, 0.6, target 2 -> 0.3*0.6 = 0.18
    assert(std::abs(probabilityOfAtLeastTarget({0.3, 0.6}, 2) - 0.18) < 1e-9);

    // Test 7: Same coins, target 1 -> 1 - (0.7*0.4) = 1 - 0.28 = 0.72
    assert(std::abs(probabilityOfAtLeastTarget({0.3, 0.6}, 1) - 0.72) < 1e-9);

    // Test 8: All tails probability 0, target 0 -> 1.0
    assert(std::abs(probabilityOfAtLeastTarget({0.0, 0.0}, 0) - 1.0) < 1e-9);

    // Test 9: Three coins with probabilities 1.0, 1.0, 1.0, target 3 -> 1.0
    assert(std::abs(probabilityOfAtLeastTarget({1.0, 1.0, 1.0}, 3) - 1.0) < 1e-9);

    // Test 10: Edge case: empty vector not allowed, but with target 0 returns 1.0
    // We won't test empty per task (non-empty), but check target > n for n=1
    assert(std::abs(probabilityOfAtLeastTarget({0.9}, 5) - 0.0) < 1e-9);

    return 0;
}
