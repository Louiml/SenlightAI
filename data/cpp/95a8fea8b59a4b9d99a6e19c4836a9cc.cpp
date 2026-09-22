Write a C++ function `countDiceCombinations(int n)` that returns the number of distinct ordered sequences of dice rolls (each roll is an integer from 1 to 6) whose sum equals exactly `n`. The function must compute the result modulo \(10^9+7\). The input `n` is a non-negative integer. For example, for `n=3`, valid sequences are (1,1,1), (1,2), (2,1), and (3), so the answer is 4. The function should handle `n=0` by returning 1 (the empty sequence). The solution must be efficient for `n` up to at least \(10^6\) and must use dynamic programming with memoization to avoid exponential recursion.
#include <cassert>

int main() {
    assert(countDiceCombinations(0) == 1);
    assert(countDiceCombinations(1) == 1);          // (1)
    assert(countDiceCombinations(2) == 2);          // (1,1) (2)
    assert(countDiceCombinations(3) == 4);          // (1,1,1) (1,2) (2,1) (3)
    assert(countDiceCombinations(4) == 8);          // known sequence: 1,1,2,4,8,16,...
    assert(countDiceCombinations(5) == 16);
    assert(countDiceCombinations(6) == 32);
    assert(countDiceCombinations(7) == 63);         // sum of previous 6
    assert(countDiceCombinations(10) == 492);       // from recurrence
    assert(countDiceCombinations(1000) == 939681205); // precomputed modulo 1e9+7
}
#include <vector>
#include <cstdint>

// Count number of ordered sequences of dice rolls (1..6) summing to n, modulo 1e9+7.
// Returns 1 for n==0 (empty sequence).
int countDiceCombinations(int n) {
    if (n < 0) return 0;
    const int MOD = 1000000007;
    std::vector<int> memo(n + 1, -1);
    
    // Recursive helper with lambda captures memo by reference.
    std::function<int(int)> solve = [&](int k) -> int {
        if (k == 0) return 1;
        if (memo[k] != -1) return memo[k];
        int64_t total = 0;
        int maxRoll = (k < 6) ? k : 6;
        for (int d = 1; d <= maxRoll; ++d) {
            total = (total + solve(k - d)) % MOD;
        }
        memo[k] = static_cast<int>(total);
        return memo[k];
    };
    
    return solve(n);
}
(Note: To keep the solution self-contained, include `<functional>` for `std::function`.)
// The problem is a classic counting problem equivalent to finding the number of compositions of `n` using parts from {1,2,3,4,5,6}, where order matters. Define `dp[k]` as the number of ways to achieve sum `k`. The base case is `dp[0]=1` (empty sequence). For any `k>0`, the last roll can be any value `d` from 1 to 6 such that `d ≤ k`; then the number of ways is the sum of `dp[k-d]` over all valid `d`. Using a top-down memoized recursion avoids recomputation. Important edge cases: `n=0` must return 1; for `n` negative (not expected but if given, return 0); when `n<6`, the loop only considers rolls up to `n`. The memoization array is initialized to -1 to distinguish uncomputed values from computed zeros (though zeros never occur because dp for positive n is always >0 for n≥1, but using `-1` is safe). Time complexity is O(n) because each state is computed once with up to 6 transitions, and space complexity is O(n) for the memo array. The modulus is applied after each addition to prevent overflow and keep numbers within 64-bit range.
