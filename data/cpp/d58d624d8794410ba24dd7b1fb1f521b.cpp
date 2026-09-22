/*
You are given a series of queries, each consisting of two non-negative integers \(n\) and \(k\) with \(0 \le k \le n < 4000001\). For each query, compute \( \binom{n}{k} \mod 1000000007 \), where \(\binom{n}{k} = \frac{n!}{k!(n-k)!}\). Write a C++ free function `long long nCrMod(int n, int k)` that returns the binomial coefficient modulo the prime \(1000000007\). The function may be called many times with increasing values of \(n\) across queries, so optimize by precomputing factorials up to the maximum \(n\) encountered. Use modular inverse via the extended Euclidean algorithm (not Fermat's little theorem) to handle division under the modulus. The function must be self-contained except for standard library includes, and should not use global mutable state that requires manual initialization; instead, use a static local cache that lazily expands as larger \(n\) are requested.
*/
#include <vector>

// Returns (n! mod MOD) using a lazily growing cache.
// Precomputes factorials up to the maximum n seen so far.
long long nCrMod(int n, int k) {
    const long long MOD = 1000000007LL;
    static std::vector<long long> fac = {1LL, 1LL}; // fac[0]=1, fac[1]=1
    
    // Extend factorial cache up to n, if needed.
    if (static_cast<int>(fac.size()) <= n) {
        const int old_size = static_cast<int>(fac.size());
        fac.resize(n + 1);
        for (int i = old_size; i <= n; ++i) {
            fac[i] = (fac[i-1] * static_cast<long long>(i)) % MOD;
        }
    }
    
    // Compute denominator = k! * (n-k)! mod MOD
    const long long den = (fac[k] * fac[n - k]) % MOD;
    
    // Extended Euclidean algorithm to find inverse of den modulo MOD.
    long long r1 = MOD, r2 = den;
    long long t1 = 0, t2 = 1;
    while (r2 != 0) {
        long long q = r1 / r2;
        long long tmp_r = r1 - q * r2;
        r1 = r2;
        r2 = tmp_r;
        long long tmp_t = t1 - q * t2;
        t1 = t2;
        t2 = tmp_t;
    }
    // r1 should be 1 (since MOD is prime and den is not multiple of MOD).
    // t1 is the inverse, but may be negative; convert to positive.
    if (t1 < 0) t1 += MOD;
    
    return (fac[n] * t1) % MOD;
}
#include <cassert>

// Declaration of the function (assume it's in the same translation unit).
long long nCrMod(int n, int k);

int main() {
    // C(5,2) = 10
    assert(nCrMod(5, 2) == 10);
    // C(5,0) = 1 and C(5,5) = 1
    assert(nCrMod(5, 0) == 1);
    assert(nCrMod(5, 5) == 1);
    // C(10,3) = 120
    assert(nCrMod(10, 3) == 120);
    // C(100,1) = 100
    assert(nCrMod(100, 1) == 100);
    // C(100,99) = 100
    assert(nCrMod(100, 99) == 100);
    // Large n to test overflow and modular arithmetic: C(1000,500) mod 1e9+7
    // Since 500! * 500! inverse mod p, we just verify a known small prime result
    // C(7,3) = 35
    assert(nCrMod(7, 3) == 35);
    // C(4,2) = 6
    assert(nCrMod(4, 2) == 6);
    // C(2,1) = 2
    assert(nCrMod(2, 1) == 2);
    // C(1,0) = 1
    assert(nCrMod(1, 0) == 1);
    // C(1,1) = 1
    assert(nCrMod(1, 1) == 1);
    
    return 0;
}
// The core idea is to precompute factorials \(fac[i] = i! \mod MOD\) for all \(i\) up to the largest \(n\) requested. Since \(MOD = 1000000007\) is prime and \(n < MOD\), the modular inverse of \(k!\) and \((n-k)!\) exists. We compute the inverse of the product \(den = fac[k] \cdot fac[n-k] \mod MOD\) using the extended Euclidean algorithm to find \(x\) such that \(den \cdot x \equiv 1 \pmod{MOD}\). Then the answer is \(fac[n] \cdot x \mod MOD\). The extended Euclidean algorithm is applied to \((r_1, r_2) = (MOD, den)\) and tracks coefficients \((t_1, t_2)\) so that after the loop, \(t_1\) is the inverse of \(den\) modulo \(MOD\). Because \(MOD\) is prime, the inverse always exists for \(den \neq 0\); since \(n<MOD\), \(den\) cannot be a multiple of \(MOD\), so it is never 0 modulo \(MOD\). Edge cases: \(k=0\) or \(k=n\) yields \(den=1\), and the inverse is 1. The lazy factorial cache uses a static vector initialized with `{1, 1}` (since 0! and 1! are 1). Each time a query arrives with a larger \(n\), the cache extends. The time complexity per query is \(O(n_{\max})\) amortized for factorial precomputation (if n grows), plus \(O(\log MOD)\) for the extended Euclidean algorithm. Space complexity is \(O(n_{\max})\) for the factorial cache.
