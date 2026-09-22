Write a C++ function `int countWays(int n)` that computes the number of distinct ways to reach a total sum `n` by repeatedly rolling a fair six-sided die (faces 1 through 6), where the order of rolls matters. For example, to reach sum 3, the valid sequences are [1+1+1], [1+2], [2+1], and [3], so the answer is 4. The result must be returned modulo \(10^9 + 7\). The function must handle `n = 0` (return 1, representing the empty sequence) and `n < 0` (return 0, as no positive die rolls can decrease the sum). The implementation must be iterative using dynamic programming with a 1D array, and must not use recursion. The function should work for any integer `n` up to 10^6 without overflow or excessive runtime.
The problem is a classic "Combination Sum" with the set of numbers {1,2,3,4,5,6}. Let `dp[i]` be the number of ways to reach sum `i`. The base case is `dp[0] = 1` (empty sequence). For each `i` from 1 to `n`, we consider the last die roll: it could be 1, 2, ..., 6 (but not larger than `i`). Thus `dp[i] = sum_{j=1..6, j<=i} dp[i-j]`. This is a bottom-up dynamic programming approach. Important edge cases: if `n` is negative, return 0 because no positive rolls can reach a negative sum; if `n` is 0, return 1. The iterative solution avoids recursion stack overflow and runs in O(n*6) = O(n) time, using O(n) space for the DP array. Since n can be large (up to 10^6), the time complexity is acceptable, and the modulus prevents overflow. The space can be optimized to O(1) (only last 6 values) but a full array is simpler and still within memory limits for n up to 10^6 (about 4 MB for int). For very large n, an O(log n) matrix exponentiation or fast doubling approach exists, but not required here.
#include <vector>

// Count the number of distinct sequences of die rolls (1..6) summing to n, modulo 1e9+7.
// Returns 1 for n == 0 (empty sequence), 0 for n < 0.
int countWays(int n) {
    const int MOD = 1000000007;
    if (n < 0) return 0;
    if (n == 0) return 1;

    std::vector<int> dp(n + 1, 0);
    dp[0] = 1; // one way to reach sum 0 (no rolls)

    for (int i = 1; i <= n; ++i) {
        long long ways = 0;
        for (int j = 1; j <= 6 && j <= i; ++j) {
            ways += dp[i - j];
            if (ways >= MOD) ways -= MOD; // keep within int range
        }
        dp[i] = static_cast<int>(ways);
    }
    return dp[n];
}
#include <cassert>

int main() {
    // Base cases
    assert(countWays(0) == 1);
    assert(countWays(-5) == 0);
    // Small values (manually verified)
    assert(countWays(1) == 1); // [1]
    assert(countWays(2) == 2); // [1+1], [2]
    assert(countWays(3) == 4); // [1+1+1], [1+2], [2+1], [3]
    assert(countWays(4) == 8); // all sequences: 1111,112,121,211,22,13,31,4
    assert(countWays(5) == 16);
    assert(countWays(6) == 32);
    // Check that n=7 is sum of previous 6
    assert(countWays(7) == (countWays(6) + countWays(5) + countWays(4) + countWays(3) + countWays(2) + countWays(1)) % 1000000007);
    // Large value to ensure modulo works and no overflow
    assert(countWays(1000000) >= 0); // just ensure it runs without error (value is arbitrary)
    assert(countWays(1000000) < 1000000007);
    return 0;
}
