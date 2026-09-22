/*
Given a positive integer `n`, write a C++ function `int countVowelPermutation(int n)` that returns the number of strings of length `n` that can be formed using only the vowels `'a'`, `'e'`, `'i'`, `'o'`, and `'u'`, subject to the following rules: each vowel `'a'` may only be followed by `'e'`; each `'e'` may only be followed by `'a'` or `'i'`; each `'i'` may not be followed by another `'i'`, but may be followed by any other vowel; each `'o'` may only be followed by `'i'` or `'u'`; and each `'u'` may only be followed by `'a'`. The answer must be returned modulo \(10^9 + 7\). For example, for `n = 1` there are exactly 5 valid strings (each single vowel), and for `n = 2` the valid strings are "ae", "ea", "ei", "ia", "ie", "io", "iu", "oi", "ou", "ua", totaling 10.
*/
#include <vector>

// Count the number of valid vowel strings of length n modulo 1e9+7.
int countVowelPermutation(int n) {
    const int MOD = 1000000007;
    // Indices: 0='a', 1='e', 2='i', 3='o', 4='u'
    // dp[c] = count of strings of current length ending with vowel c
    std::vector<long long> dp(5, 1); // length 1: each vowel alone
    if (n == 1) return 5;

    for (int len = 2; len <= n; ++len) {
        std::vector<long long> next(5, 0);
        // 'a' can be preceded by 'u' only
        next[0] = dp[4];
        // 'e' can be preceded by 'a' or 'i'
        next[1] = (dp[0] + dp[2]) % MOD;
        // 'i' can be preceded by any vowel except itself
        next[2] = (dp[0] + dp[1] + dp[3] + dp[4]) % MOD;
        // 'o' can be preceded by 'i' or 'u'
        next[3] = (dp[2] + dp[4]) % MOD;
        // 'u' can be preceded by 'a' only
        next[4] = dp[0];
        dp = next;
    }

    long long result = 0;
    for (int c = 0; c < 5; ++c) {
        result = (result + dp[c]) % MOD;
    }
    return static_cast<int>(result);
}
#include <cassert>

int main() {
    // Base cases
    assert(countVowelPermutation(1) == 5);
    assert(countVowelPermutation(2) == 10);

    // From LeetCode examples
    assert(countVowelPermutation(3) == 19);
    assert(countVowelPermutation(5) == 68);

    // Larger n to check modulo handling (should not overflow or produce negatives)
    assert(countVowelPermutation(10) == 10821);
    assert(countVowelPermutation(50) == 964239966);
    assert(countVowelPermutation(100) == 617292330);
    assert(countVowelPermutation(2000) == 670034862);

    // Edge: maximum n (problem constraints usually up to 20000)
    assert(countVowelPermutation(20000) == 615581824);
}
// This problem is a classic dynamic programming counting problem. We can define a state `dp[i][c]` as the number of valid strings of length `i+1` (where `i` is the zero‑based index from 0 to `n-1`) that end with vowel `c`. Because the rules only constrain which vowel may appear immediately before a given vowel, we can build the counts from the last position backward or forward. 
//
// A forward DP is more intuitive: initialize `dp[0][c] = 1` for all five vowels (strings of length 1). Then for each next position, for each vowel `c`, sum the counts of all vowels that are allowed to precede `c`. For example, if `c = 'a'`, then the only allowed predecessor is `'u'` (since `'u'` may be followed by `'a'`); if `c = 'e'`, predecessors are `'a'` and `'i'`; if `c = 'i'`, predecessors are `'a'`, `'e'`, `'o'`, `'u'`; if `c = 'o'`, predecessors are `'i'` and `'u'`; if `c = 'u'`, predecessor is `'a'`. 
//
// The answer for length `n` is the sum of `dp[n-1][c]` over all five vowels, modulo \(10^9+7\). Edge case: for `n = 1`, the answer is 5. The time complexity is \(O(n \cdot 5) = O(n)\), and the space complexity can be reduced to \(O(1)\) by only keeping the previous row. For clarity, a 2D array of size `(n+1) × 5` is acceptable for moderate `n`, but we can implement an iterative 1D array approach.
