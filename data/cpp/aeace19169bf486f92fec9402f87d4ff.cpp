/*
Write a standalone C++ function that takes a non-negative integer \( N \) as input and returns the value of the expression \( 2^{N+1} - 2 \), computed modulo \( 998244353 \). The function must handle large values of \( N \) (up to \( 10^{18} \)) efficiently using fast exponentiation, and must return the result as a `long long`. Note that the expression is equivalent to \( 2 \times (2^N - 1) \), and the computation must be performed using modular arithmetic to avoid overflow.
*/

#include <cstdint>

constexpr long long MOD = 998244353LL;

// Compute (base^exp) % mod using fast exponentiation.
static long long mod_pow(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

// Return (2^(N+1) - 2) % MOD, for N >= 0.
long long compute_two_power_minus_two(long long N) {
    long long pow_val = mod_pow(2, N + 1, MOD);
    long long ans = (pow_val - 2) % MOD;
    if (ans < 0) ans += MOD;
    return ans;
}

#include <cassert>

int main() {
    assert(compute_two_power_minus_two(0) == 0);
    assert(compute_two_power_minus_two(1) == 2);    // 2^2 - 2 = 2
    assert(compute_two_power_minus_two(2) == 6);    // 2^3 - 2 = 6
    assert(compute_two_power_minus_two(3) == 14);   // 2^4 - 2 = 14
    assert(compute_two_power_minus_two(10) == 2046);
    // Large N: verify with modular arithmetic
    assert(compute_two_power_minus_two(1000000000000000000LL) == 178194985);
    return 0;
}

// The core problem is to compute \( 2^{N+1} - 2 \pmod{M} \) where \( M = 998244353 \). Since \( N \) can be as large as \( 10^{18} \), direct multiplication or iteration is infeasible. We use fast exponentiation (binary exponentiation) to compute \( 2^{N+1} \mod M \) in \( O(\log N) \) time. The modular arithmetic ensures all intermediate values remain within the 64-bit range. Edge cases: when \( N = 0 \), the result is \( 2^{1} - 2 = 0 \); when \( N \) is extremely large, the modulus handles wrapping. The formula can also be written as \( (2^{N+1} - 2) \mod M \), but to avoid negative results, we add \( M \) before taking the modulus if needed. Time complexity is \( O(\log N) \), space complexity is \( O(1) \).
