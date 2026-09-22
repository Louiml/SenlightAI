Write a C++ function `int countSurjectiveFunctions(int n, int x, long long mod)` that computes, modulo `mod`, the number of surjective (onto) functions from a domain of size `n` to a codomain of size `x`, where `1 <= n <= 500`, `1 <= x <= 500`, and `mod = 998244353`. The function must precompute factorials, powers, and binomial coefficients up to 500 internally and use dynamic programming with the recurrence: for `i >= 2`, if `j < i` then `f(i,j) = j^i`; if `j >= i`, then `f(i,j) = (i-1)^i + sum_{k=2..i} f(k, j-i+1) * (i-1)^(i-k) * C(i,k)`. The result is `f(n, x)`. The solution must handle edge cases like `n=1` (return `1^x = 1` if `x=1` else `0`) and small values properly, and be efficient for the given limits.

The problem asks to count surjective functions where every element in the codomain (size `x`) is hit by at least one element from the domain (size `n`). The core recurrence is derived by considering the inverse image of one specific element in the codomain: choose a subset `k` of the domain to map to that element, and the remaining `n-k` elements must map to the other `x-1` codomain elements surjectively. However, the provided snippet uses a different but equivalent DP that precomputes for all `i` and `j` up to 500. The recurrence given in the task: for `i >= 2`, if `j < i`, then `f(i,j) = j^i` (there are `j^i` total functions, and since `j < i`, all must be surjective? Actually, if `j < i`, it's impossible to be surjective, but the snippet uses `expo[j][i]` which is `j^i`, not zero. That's because the recurrence is counting something else—likely the number of functions with a certain property that simplifies to `j^i` when `j < i`. For counting surjective functions, the correct answer for `j < i` is `0`, but the given recurrence is a known formula for the number of ways to color a graph or a combinatorics problem involving surjective-like mappings. For this task, we must follow the exact recurrence provided. So we precompute `pow[i][j] = i^j mod mod` for `i` and `j` from 0 to 500, `fact[k]` and `invFact[k]` for binomials, and `C[i][k]`. Then initialize `f[1][j] = 1` for all `j? Actually `function[1][j]` is not used in the recurrence because loop starts at `i=2`. For `i=2`, `f[2][j]=j`. Then for `i>2`, use the recurrence. Time complexity is O(500^3) due to triple loops (for each `i`, `j`, and inner `k` up to `i`), but with `n,x <= 500`, the actual computation is about 500*500*500/2 ≈ 62.5M operations, which is fine. Space is O(500^2) for DP and precomputed tables. Edge cases: `n=1`, `x=1` should return 1 (since the only function maps the single domain element to the single codomain element). For `n=1`, `x>1` should return 0 because not surjective. For `n=2`, `x=1` should return 1? Using recurrence: `function[2][1]` since `j=1 < i=2`, we use `expo[1][2] = 1^2 = 1`, so returns 1, which is correct (only one function from 2 elements to 1 element is surjective). For `n=2`, `x=2` returns `function[2][2] = j = 2`? That would be 2, but actually surjective functions from 2 to 2 are exactly 2 (bijections). So it works for these. For `n=3, x=2`, recurrence gives `function[3][2]` since `j=2 < i=3`, use `expo[2][3]=8`, but true surjective functions from 3 to 2 is 2^3 - 2 = 6. So this recurrence is not counting surjective functions in general. The task is to implement exactly the recurrence from the snippet, not a general surjective count. So we must follow the given formula. Therefore, the solution will implement the exact DP from the snippet.

#include <vector>
#include <cstdint>

using int64 = long long;

// Computes the value defined by the recurrence from the snippet:
// f(n, x) = (x^n if x < n) else ( (n-1)^n + sum_{k=2..n} f(k, x-n+1) * (n-1)^(n-k) * C(n,k) ) mod mod.
int countSurjectiveFunctions(int n, int x, long long mod) {
    const int MAX = 500;
    
    // Precompute powers: powers[i][j] = i^j % mod
    std::vector<std::vector<long long>> powers(MAX+1, std::vector<long long>(MAX+1, 0));
    for (int base = 0; base <= MAX; ++base) {
        powers[base][0] = 1;
        for (int exp = 1; exp <= MAX; ++exp) {
            powers[base][exp] = (powers[base][exp-1] * base) % mod;
        }
    }
    
    // Precompute factorials and inverse factorials for binomial coefficients
    std::vector<long long> fact(MAX+1, 1);
    std::vector<long long> invFact(MAX+1, 1);
    auto modPow = [](long long a, long long b, long long m) {
        long long res = 1;
        while (b > 0) {
            if (b & 1) res = (res * a) % m;
            a = (a * a) % m;
            b >>= 1;
        }
        return res;
    };
    for (int i = 1; i <= MAX; ++i) fact[i] = (fact[i-1] * i) % mod;
    invFact[MAX] = modPow(fact[MAX], mod-2, mod);
    for (int i = MAX; i > 0; --i) invFact[i-1] = (invFact[i] * i) % mod;
    
    auto binom = [&](int a, int b) -> long long {
        if (b < 0 || b > a) return 0;
        return fact[a] * invFact[b] % mod * invFact[a-b] % mod;
    };
    
    // Build DP table
    std::vector<std::vector<long long>> dp(MAX+1, std::vector<long long>(MAX+1, 0));
    
    // Base case from snippet: for i=2, dp[2][j] = j for all j
    for (int j = 1; j <= MAX; ++j) {
        dp[2][j] = j % mod;
    }
    
    for (int i = 3; i <= MAX; ++i) {
        for (int j = 1; j <= MAX; ++j) {
            if (j < i) {
                dp[i][j] = powers[j][i];
            } else {
                long long sum = powers[i-1][i];
                for (int k = 2; k <= i; ++k) {
                    long long term = dp[k][j-i+1];
                    term = (term * powers[i-1][i-k]) % mod;
                    term = (term * binom(i, k)) % mod;
                    sum = (sum + term) % mod;
                }
                dp[i][j] = sum;
            }
        }
    }
    
    // Handle n=1 separately
    if (n == 1) {
        return (x == 1) ? 1 : 0;
    }
    
    return static_cast<int>(dp[n][x]);
}

#include <cassert>

int main() {
    const long long mod = 998244353;
    
    // n=1, x=1 -> 1 (only one function)
    assert(countSurjectiveFunctions(1, 1, mod) == 1);
    // n=1, x=2 -> 0 (can't be surjective)
    assert(countSurjectiveFunctions(1, 2, mod) == 0);
    // n=2, x=1 -> 1 (1^2 = 1)
    assert(countSurjectiveFunctions(2, 1, mod) == 1);
    // n=2, x=2 -> 2 (j=2)
    assert(countSurjectiveFunctions(2, 2, mod) == 2);
    // n=3, x=1 -> 1 (1^3 = 1)
    assert(countSurjectiveFunctions(3, 1, mod) == 1);
    // n=3, x=3 -> 3 (from recurrence: since j=3 >= i=3, sum starts with (2)^3=8, k=2 term: dp[2][3-3+1=1] * 2^(1) * C(3,2) = 1*2*3=6, k=3 term: dp[3][1]*2^0*C(3,3)=1*1*1=1, total 8+6+1=15? But wait, dp[2][1]=1, dp[3][1] is j<i so 1^3=1, so sum=8+1*2*3+1*1*1=8+6+1=15. Check: 15 is the correct number of surjective functions from 3 to 3? Actually surjective from 3 to 3 is 3! = 6. This recurrence gives 15, so it's not standard surjective. But we test the recurrence as given.)
    assert(countSurjectiveFunctions(3, 3, mod) == 15);
    // n=3, x=2 -> since j=2 < i=3, returns 2^3=8
    assert(countSurjectiveFunctions(3, 2, mod) == 8);
    // n=4, x=3 -> j=3 < i=4, returns 3^4=81
    assert(countSurjectiveFunctions(4, 3, mod) == 81);
    // n=5, x=6 -> j=6 >= i=5, we can compute a small value manually? Let's trust the DP and just check non-modular small case: for n=5, x=6, recurrence sum = (4)^5=1024, k=2: dp[2][6-5+1=2]=2 * 4^(3)=64 * C(5,2)=10 => 2*64*10=1280; k=3: dp[3][2]=8 * 4^(2)=16 * C(5,3)=10 => 8*16*10=1280; k=4: dp[4][2]=2^4=16 * 4^(1)=4 * C(5,4)=5 => 16*4*5=320; k=5: dp[5][2]=2^5=32 * 4^(0)=1 * C(5,5)=1 => 32*1*1=32; sum=1024+1280+1280+320+32=3936. So assert dp[5][6]==3936.
    assert(countSurjectiveFunctions(5, 6, mod) == 3936);
    // n=2, x=3 -> j=3 >= i=2, dp[2][3]=3
    assert(countSurjectiveFunctions(2, 3, mod) == 3);
    // n=10, x=5 -> j < i, so returns 5^10 % mod = 9765625 % 998244353 = 9765625
    assert(countSurjectiveFunctions(10, 5, mod) == 9765625);
}
