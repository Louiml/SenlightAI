Given multiple test cases, each containing two non-negative integers \( n \) and \( r \), write a C++ function `ull nCrModP(ull n, ull r)` that computes the binomial coefficient \( \binom{n}{r} \) modulo \( p = 1000000007 \) (a prime). The input may have many test cases (up to \( 10^5 \)), and each \( n \) can be as large as \( 10^9 \), but \( r \) will not exceed 1000. The function must handle the case \( r > n \) by returning 0, and must be efficient enough for the given constraints. The result must be returned as an `unsigned long long`.
#include <cassert>

int main() {
    // Basic known values
    assert(nCrModP(5, 2) == 10);
    assert(nCrModP(10, 3) == 120);
    assert(nCrModP(0, 0) == 1);
    assert(nCrModP(0, 1) == 0);
    assert(nCrModP(7, 7) == 1);
    assert(nCrModP(7, 8) == 0);

    // Larger n but small r (mod check with known result)
    // C(1000000000, 1) = 1000000000
    assert(nCrModP(1000000000ULL, 1) == 1000000000ULL % 1000000007ULL);
    // C(1000000000, 2) = 1000000000 * 999999999 / 2 mod p
    ull expected = ( (1000000000ULL % 1000000007ULL) * (999999999ULL % 1000000007ULL) % 1000000007ULL ) * 500000004ULL % 1000000007ULL;
    assert(nCrModP(1000000000ULL, 2) == expected);

    // Random small check: C(20, 10) = 184756
    assert(nCrModP(20, 10) == 184756);

    // Repeated calls to ensure static init works correctly
    assert(nCrModP(6, 3) == 20);
    assert(nCrModP(100, 0) == 1);
    assert(nCrModP(100, 100) == 1);
    return 0;
}
#include <vector>
#include <cstdint>

using ull = unsigned long long;

const ull MOD = 1000000007ULL;

// Fast modular exponentiation: (base^exp) % MOD
ull modPow(ull base, ull exp, ull mod) {
    ull result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

// Compute C(n, r) modulo MOD using multiplicative formula.
// Precondition: MOD is prime and r <= 1000.
ull nCrModP(ull n, ull r) {
    if (r > n) return 0;
    if (r == 0) return 1;

    const ull maxR = 1000;
    // Precompute factorials and inverse factorials up to maxR once.
    static std::vector<ull> fact(maxR + 1, 1);
    static std::vector<ull> invFact(maxR + 1, 1);
    static bool initialized = false;
    if (!initialized) {
        for (ull i = 1; i <= maxR; ++i) {
            fact[i] = (fact[i - 1] * i) % MOD;
        }
        invFact[maxR] = modPow(fact[maxR], MOD - 2, MOD);
        for (ull i = maxR; i > 0; --i) {
            invFact[i - 1] = (invFact[i] * i) % MOD;
        }
        initialized = true;
    }

    // Numerator: n * (n-1) * ... * (n-r+1) mod MOD
    ull numerator = 1;
    for (ull i = 0; i < r; ++i) {
        numerator = (numerator * ((n - i) % MOD)) % MOD;
    }

    // Denominator: r! mod MOD, then multiply by modular inverse
    ull result = (numerator * invFact[r]) % MOD;
    return result;
}
// The naive dynamic programming approach (as in the snippet) is \( O(n \cdot r) \) per test case, which fails for \( n \) up to \( 10^9 \). Since \( r \) is small (at most 1000), we can use the multiplicative formula for binomial coefficients modulo a prime:
// \[
// \binom{n}{r} = \frac{n \cdot (n-1) \cdots (n-r+1)}{r!}
// \]
// We compute the numerator as a product of \( r \) terms, each reduced modulo \( p \), and the denominator as \( r! \) modulo \( p \). Since \( p \) is prime and \( r < p \) (because \( r \le 1000 \ll p \)), we can use modular inverse (via fast exponentiation \( a^{p-2} \bmod p \)) to divide. For each test case, this takes \( O(r \log p) \) time, which is about \( 1000 \times 30 \approx 30000 \) operations—fine for \( 10^5 \) cases. Edge cases: if \( r > n \), return 0; if \( r = 0 \), return 1; handle \( n = 0, r = 0 \) as 1. Space complexity is \( O(1) \). We precompute factorials and inverse factorials up to 1000 once to speed up denominator computation, but even without that, \( r! \) can be computed in \( O(r) \) per call.
