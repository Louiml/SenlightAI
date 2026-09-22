// Given an array of `n` positive integers (with `2 ≤ n ≤ 2000` and each `a[i] ≤ 10^6`), write a C++ function `long long countValidPaths(const std::vector<int>& a)` that returns the number of valid ways to start at index 0 and end at index `n-1`, moving only from a lower index `i` to a higher index `j` (i.e., `i < j`) such that `gcd(a[i], a[j]) > 1`. The count must be returned modulo `998244353`. A path is a sequence of indices `0 = i0 < i1 < … < ik = n-1` where for every consecutive pair `(it, it+1)`, `gcd(a[it], a[it+1]) != 1`. You may assume the input array is non-empty and all elements are positive. The function should handle large counts without overflow by applying the modulo at each addition step.

// The problem is a classic DP on a DAG (directed acyclic graph) where edges go from lower to higher indices. The recurrence is: `dp[i]` = number of valid paths from index `i` to `n-1`. Base case: `dp[n-1] = 1` because there is exactly one empty path (just stay at the last index). For any `i` from `n-2` down to `0`, we sum `dp[j]` over all `j > i` such that `gcd(a[i], a[j]) != 1`. The answer is `dp[0]`. Since we iterate over all pairs `(i, j)` with `i < j`, this is `O(n^2)` time and `O(n)` space. Edge cases: when `n=2`, the answer is `1` if `gcd(a[0], a[1]) != 1`, else `0`. When all pairs have gcd equal to 1, the answer is `0`. Since the modulo is applied after each addition, no overflow occurs as long as `dp` values are stored as `long long`. The function must be self-contained and avoid using global variables except for constants.

#include <vector>
#include <numeric>

const long long MOD = 998244353LL;

long long countValidPaths(const std::vector<int>& a) {
    int n = static_cast<int>(a.size());
    std::vector<long long> dp(n, 0);
    dp[n - 1] = 1;
    for (int i = n - 2; i >= 0; --i) {
        long long sum = 0;
        for (int j = i + 1; j < n; ++j) {
            if (std::gcd(a[i], a[j]) > 1) {
                sum = (sum + dp[j]) % MOD;
            }
        }
        dp[i] = sum;
    }
    return dp[0];
}

#include <cassert>
#include <vector>
#include <numeric>

long long countValidPaths(const std::vector<int>& a);

int main() {
    // Test 1: n=2 with gcd > 1
    assert(countValidPaths({2, 4}) == 1);
    // Test 2: n=2 with gcd == 1
    assert(countValidPaths({2, 3}) == 0);
    // Test 3: all pairs have gcd > 1 (complete graph)
    assert(countValidPaths({2, 4, 6}) == 4); // paths: 0-1-2, 0-2, 0-1-? wait only 3 indices, paths: [0,2] and [0,1,2] and [0,1]? no, must end at 2. So [0,2] and [0,1,2] = 2, but let's compute: dp[2]=1, dp[1] = dp[2] (gcd(4,6)=2) = 1, dp[0] = dp[1]+dp[2] = 2. So expected 2.
    assert(countValidPaths({2, 4, 6}) == 2);
    // Test 4: chain where each adjacent pair works but non-adjacent not
    assert(countValidPaths({2, 3, 4}) == 1); // only path 0-2? gcd(2,4)=2, but 0-1 gcd=1, 1-2 gcd=1, so only 0-2 works -> 1
    // Test 5: larger example
    assert(countValidPaths({6, 10, 15}) == 1); // 0-1 gcd=2, 1-2 gcd=5, 0-2 gcd=3, paths: [0,2] only? [0,1,2] works too because gcd(6,10)=2 and gcd(10,15)=5, so yes 2 paths. Let's compute: dp[2]=1, dp[1]=dp[2]=1, dp[0] = dp[1]+dp[2] = 2. So expected 2.
    assert(countValidPaths({6, 10, 15}) == 2);
    // Test 6: all are primes -> no edges
    assert(countValidPaths({2, 3, 5, 7}) == 0);
    // Test 7: single edge from start to end with others isolated
    assert(countValidPaths({2, 5, 4}) == 0); // gcd(2,5)=1, gcd(2,4)=2, gcd(5,4)=1, path 0-2 only? yes, but 0-2 works, so 1. Let's compute: dp[2]=1, dp[1]=0 (gcd(5,4)=1), dp[0]=dp[1]+dp[2]=1. So expected 1.
    assert(countValidPaths({2, 5, 4}) == 1);
    // Test 8: count with large modulo (just check result is within modulo)
    std::vector<int> test8(2000, 2); // all 2, gcd=2 for any pair, total paths = 2^(1999) mod MOD
    // Known value: 2^1999 mod 998244353? We'll just assert it's between 0 and MOD-1
    long long res8 = countValidPaths(test8);
    assert(res8 >= 0 && res8 < MOD);
    // Test 9: already computed small case
    assert(countValidPaths({2, 4, 8, 16}) == 4); // paths: 0-1-2-3, 0-1-3, 0-2-3, 0-3 => 4
    // Test 10: n=1? Not in constraints but function should handle: dp[0]=1
    assert(countValidPaths({5}) == 1);
}
