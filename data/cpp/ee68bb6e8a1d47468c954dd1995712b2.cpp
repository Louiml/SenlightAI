/*
Write a C++ function `long long countWays(int S)` that, given a positive integer `S` (with `S ≤ 2000`), returns the number of ways to represent `S` as a sum of one or more integers, each of which is at least 3. The order of the summands matters, so `3+4` and `4+3` are considered distinct. The result must be returned modulo `1 000 000 007`. For example, for `S = 7`, the valid representations are `3+4`, `4+3`, `7`, giving 3 ways. For `S = 3`, only `3` is valid (1 way), and for `S < 3`, there are 0 ways.
*/

#include <vector>

// Count the number of ways to represent S as a sum of integers each ≥ 3,
// order matters. Return the count modulo 1'000'000'007.
long long countWays(int S) {
    const long long MOD = 1000000007LL;
    std::vector<long long> dp(S + 1, 0);
    dp[0] = 1;  // empty sum base case

    long long prefixSum = 1;  // sum of dp[0] .. dp[current-3], for the first iteration i=3

    for (int i = 1; i <= S; ++i) {
        if (i >= 3) {
            // At i, the sum of dp[0..i-3] is the value we need.
            // We maintain prefixSum incrementally.
            dp[i] = prefixSum;
            // Update prefixSum for next i: add dp[i-2] (because next i+1 needs dp[0..i-2])
            // We'll handle the update below after deciding when to add.
        }
        // Update prefixSum for the next iteration.
        // For iteration i+1, we need sum of dp[0..(i+1-3)] = dp[0..i-2].
        // So we add dp[i-2] (if i-2 >= 0) after computing dp[i].
        if (i - 2 >= 0) {
            prefixSum = (prefixSum + dp[i - 2]) % MOD;
        }
    }

    return dp[S] % MOD;
}

#include <cassert>

// Forward declaration of the function under test.
long long countWays(int S);

int main() {
    // S < 3 -> 0 ways
    assert(countWays(1) == 0);
    assert(countWays(2) == 0);
    // S = 3 -> only [3]
    assert(countWays(3) == 1);
    // S = 4 -> [4]
    assert(countWays(4) == 1);
    // S = 5 -> [5] only (3+2 invalid)
    assert(countWays(5) == 1);
    // S = 6 -> [6], [3+3] -> 2 ways
    assert(countWays(6) == 2);
    // S = 7 -> [7], [3+4], [4+3] -> 3 ways
    assert(countWays(7) == 3);
    // S = 8 -> [8], [3+5], [5+3], [4+4] -> 4 ways
    assert(countWays(8) == 4);
    // S = 9 -> [9], [3+6], [6+3], [4+5], [5+4], [3+3+3] -> 6 ways
    assert(countWays(9) == 6);
    // Large value sanity: ensure modulo works without overflow (not checking exact value here, but equality to a known small check)
    // For S=2000, just ensure it returns a non-negative number < MOD (we can't easily hand-compute, but we check it completes).
    long long big = countWays(2000);
    assert(big >= 0 && big < 1000000007LL);
    return 0;
}

// The problem is a classic counting problem solvable with dynamic programming. Let `dp[i]` be the number of valid ways to sum to exactly `i` using parts each at least 3. The recurrence is: for `i >= 3`, `dp[i] = sum_{k=3..i} dp[i-k]`, where we conceptually choose the first term `k` (≥3) and then count ways for the remaining sum `i-k`. The base case is `dp[0] = 1` (representing an empty sum, used only as a building block). For `i < 3`, `dp[i] = 0`. To compute efficiently, we can maintain a prefix sum of `dp` up to `i-3`, because `dp[i]` equals the sum of `dp[0]` through `dp[i-3]`. This avoids an inner loop and gives O(S) time for all states. Edge cases: `S = 0` is not in the input (positive integer), but if it were, the answer would be 1 by convention; `S = 1,2` gives 0; `S = 3` gives 1. The modulo operation must be applied at each addition since sums can grow large. Time complexity is O(S), space O(S) for the dp array and O(1) extra for the running prefix sum.
