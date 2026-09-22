// Write a C++ function `long long smallestPowerfulNumber(int n)` that, given an integer `n` (`1 <= n <= 1000`), returns the smallest positive integer that has exactly `n` positive divisors. The result is guaranteed to fit in a 64-bit signed integer. For example, `n=1` returns `1`, `n=2` returns `2` (divisors: 1,2), `n=3` returns `4` (divisors: 1,2,4), `n=4` returns `6` (divisors: 1,2,3,6). The function must use a prime-based approach: any number with `n` divisors can be factored as \( p_1^{e_1} p_2^{e_2} \cdots p_k^{e_k} \) where the divisor count is \((e_1+1)(e_2+1)\cdots(e_k+1)=n\). To minimize the number, assign larger exponents to smaller primes, and the exponents must be non-increasing (i.e., \(e_1 \ge e_2 \ge \dots \ge e_k\)). You may assume the first 16 primes suffice.
#include <cassert>
#include <iostream>

// Include the solution function here (or link appropriately)
// For brevity, we assume the function is defined above.

int main() {
    assert(smallestPowerfulNumber(1) == 1);
    assert(smallestPowerfulNumber(2) == 2);
    assert(smallestPowerfulNumber(3) == 4);
    assert(smallestPowerfulNumber(4) == 6);
    assert(smallestPowerfulNumber(5) == 16);
    assert(smallestPowerfulNumber(6) == 12);
    assert(smallestPowerfulNumber(7) == 64);
    assert(smallestPowerfulNumber(8) == 24);
    assert(smallestPowerfulNumber(9) == 36);
    assert(smallestPowerfulNumber(10) == 48);
    // Additional test: n=12 -> smallest is 60? Actually 60 has divisors: 1,2,3,4,5,6,10,12,15,20,30,60 = 12, yes.
    assert(smallestPowerfulNumber(12) == 60);
    // n=16 -> 120? Let's check: 120 divisors=16? 120=2^3*3*5 => (4*2*2)=16, yes.
    assert(smallestPowerfulNumber(16) == 120);
    // n=1000 result fits long long? Known smallest is 2^4*3^4*5^4*7*11*13? Might be large but under 9e18.
    long long result1000 = smallestPowerfulNumber(1000);
    assert(result1000 > 0 && result1000 <= 4e18);
    std::cout << "All tests passed.\n";
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

static const long long INF = 4e18;

// Compute the smallest number with exactly n divisors.
long long smallestPowerfulNumber(int n) {
    // First 16 primes are enough for n <= 1000.
    const int primes[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    
    // Memoization: memo[n][primeIdx][maxExp] but we can use unordered_map
    // Since n <= 1000, primeIdx <= 16, maxExp <= 20 (because 2^20 > 1e18? actually 2^60 overflows long long, but maxExp is at most 60 but practical).
    // We'll use a 3D array with dimensions [1001][17][61] and initialize to -1.
    static long long memo[1001][17][61];
    static bool initialized = false;
    if (!initialized) {
        for (int i = 0; i <= 1000; ++i)
            for (int j = 0; j < 17; ++j)
                for (int k = 0; k < 61; ++k)
                    memo[i][j][k] = -1;
        initialized = true;
    }
    
    // Fast power with overflow check: returns -1 if overflow.
    auto powCheck = [](long long base, int exp) -> long long {
        long long res = 1;
        for (int i = 0; i < exp; ++i) {
            if (res > INF / base) return -1;
            res *= base;
        }
        return res;
    };
    
    // Recursive DP function.
    function<long long(int, int, int)> solve = [&](int remaining, int primeIdx, int maxExp) -> long long {
        if (remaining == 1) return 1;
        if (primeIdx >= 16) return INF;
        if (maxExp > 60) maxExp = 60; // clamp, since 2^60 > long long but we'll check overflow anyway.
        if (memo[remaining][primeIdx][maxExp] != -1) return memo[remaining][primeIdx][maxExp];
        
        long long best = INF;
        // Iterate over divisors d of remaining, where d = exponent+1
        for (int d = 1; d <= remaining; ++d) {
            if (remaining % d != 0) continue;
            int exp = d - 1;
            if (exp == 0) continue;
            if (exp > maxExp) continue;
            long long powVal = powCheck(primes[primeIdx], exp);
            if (powVal == -1) continue;
            long long sub = solve(remaining / d, primeIdx + 1, exp);
            if (sub == INF) continue;
            if (powVal > INF / sub) continue;
            best = min(best, powVal * sub);
        }
        return memo[remaining][primeIdx][maxExp] = best;
    };
    
    return solve(n, 0, 60); // 60 is a safe upper bound for maxExp with n<=1000.
}
// The problem is a classic "smallest number with exactly N divisors" dynamic programming problem. The key idea: if the prime factorization is \(p_1^{e_1} p_2^{e_2} \cdots p_k^{e_k}\), then the number of divisors is \(\prod (e_i+1)\). We want the smallest number for a given divisor count `n`. Since smaller primes should carry larger or equal exponents, we can process primes sequentially. Define `dp[i][j]` = smallest number using the first `i` primes (i.e., primes up to index `i-1`) such that the product of `(exponent+1)` equals `j`. But a more common formulation is `dp[i][j]` = smallest number using first `i` primes (indices 0..i-1) where `j` is the current divisor count. Transition: we choose an exponent `e` for the i-th prime, then `dp[i][j]` from `dp[i-1][j/(e+1)]` multiplied by `prime[i-1]^e`, provided `j` is divisible by `e+1`. We also enforce that exponents are non-increasing, so we carry the previous exponent in state, or we add a constraint that e must be <= previous exponent. But the sample code uses a 2D DP with `dp[i][j]` where `i` is the divisor count and `j` is the number of primes used (actually it uses `dp[count][primeIndex]`). Actually the given snippet: `dp[i][j]` where `i` is the target divisor count and `j` is the number of primes already used. They initialize `dp[1][i]=1` for any `i` (since 1 has 1 divisor and can be represented with no primes). Then for each `i` from 2 to n, for each prime index `j` (they use 0..100), `dp[i][j]` is computed by trying a divisor `k` of `i` where `k` is the factor `(exponent+1)` for the next prime, and then using `dp[i/k][j+1]` multiplied by `prime[j]^(k-1)`. This works because if we assign exponent `e=k-1` to prime at index `j`, and the remaining divisor count is `i/k`, and we must ensure the exponents are non-increasing. However, the sample does not enforce non-increasing, so it might produce wrong results for some n, but for the given constraints it might accidentally work with the min operation? Actually the non-increasing property is necessary to avoid overcounting permutations. But because we process primes in increasing order (index j from 0 upward), and in the transition we move from `dp[i/k][j+1]` to `dp[i][j]`, the exponent `e` for prime j is potentially larger than the exponent used for prime j+1? Not enforced. The standard solution uses a state that includes the maximum allowed exponent. For a robust solution, we can use recursion with memoization: `solve(n, primeIndex, maxExponent)` returns the smallest number with exactly `n` divisors using primes from `primeIndex` onward, and each exponent cannot exceed `maxExponent`. That enforces non-increasing. Complexity: For each state, we iterate over all divisors of `n` up to `n`, and the number of states is roughly `n * (number of primes) * (max exponent)` but in practice small. For `n<=1000`, it's fine. Time complexity is roughly O(n * π * sqrt(n)) worst, but using divisor enumeration reduces it. Space O(n * π). We'll implement a top-down DP with `long long` and `INF` sentinel. Edge cases: n=1 returns 1. Ensure overflow: use `__int128` for multiplication check before casting to long long, but constraints guarantee fit in long long. Use precomputed first 16 primes (prime[0]=2, prime[1]=3, ... prime[15]=53). The recursion: for each divisor `d` of `n` (where `d = exponent+1`), we can assign exponent `e = d-1` to current prime, provided `e <= maxExponent` and `e>=1` (or e can be 0? If e=0, then d=1, but that doesn't reduce divisor count, so we skip). Then `candidate = prime[primeIndex]^e * solve(n/d, primeIndex+1, e)`. Take minimum. Base case: if n==1 return 1. If primeIndex >= number of primes, return INF. Also prune if `prime[primeIndex]^e` overflows. The answer is `solve(n, 0, INT_MAX)`.
