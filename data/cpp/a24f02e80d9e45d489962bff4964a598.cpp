Write a C++ function `powerModulo` that takes two positive integers `base` and `exponent` (up to 10^18) and a modulus `mod` (a positive integer ≤ 10^9) and returns `(base^exponent) % mod` using binary exponentiation (fast modular exponentiation). The function must handle large exponents efficiently without overflow, and it should work correctly even when `base` is 0. Do not use the built-in `pow` function or any external big-integer library.
The algorithm is binary exponentiation: while the exponent is greater than zero, if it is odd, multiply the result accumulator by the current base and take modulo; then square the base and take modulo, and right-shift the exponent by one bit. Edge cases: exponent = 0 returns 1 % mod (even if base = 0, 0^0 is defined as 1 for this task); base = 0 with positive exponent returns 0; any result must be reduced modulo `mod` to avoid overflow. Use `long long` for intermediate multiplications because the product of two numbers up to 10^9 can reach 10^18, which fits in 64-bit but not 32-bit. Time complexity is O(log exponent) multiplications, space complexity is O(1).
#include <cstdint>

// Compute (base^exponent) % mod using binary exponentiation.
// All parameters are positive integers. Returns the result as a 64-bit integer.
int64_t powerModulo(int64_t base, int64_t exponent, int64_t mod) {
    if (mod == 1) return 0; // any number modulo 1 is 0
    int64_t result = 1 % mod;
    base %= mod;
    while (exponent > 0) {
        if (exponent & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exponent >>= 1;
    }
    return result;
}
#include <cassert>
#include <cstdint>

int main() {
    // Basic cases
    assert(powerModulo(2, 10, 1000) == 24);      // 2^10 = 1024 % 1000 = 24
    assert(powerModulo(3, 5, 100) == 43);        // 243 % 100 = 43
    assert(powerModulo(7, 0, 100) == 1);         // anything^0 = 1
    // Modulus = 1
    assert(powerModulo(123456789, 987654321, 1) == 0);
    // Base = 0
    assert(powerModulo(0, 0, 10) == 1);          // 0^0 defined as 1
    assert(powerModulo(0, 5, 10) == 0);          // 0^5 = 0
    // Large exponent
    assert(powerModulo(2, 200, 1000000007) == 498199261); // known modular result
    // Large base and exponent with small mod
    assert(powerModulo(1000000000, 1000000000000000000LL, 1000000007) == 140625001);
    // Verify against a known property: (base^a)*(base^b) = base^(a+b)
    int64_t a = powerModulo(5, 100, 1000000007);
    int64_t b = powerModulo(5, 200, 1000000007);
    int64_t c = powerModulo(5, 300, 1000000007);
    assert((a * b) % 1000000007 == c);
    return 0;
}
