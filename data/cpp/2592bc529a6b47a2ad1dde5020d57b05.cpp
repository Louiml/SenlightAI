/*
Write a C++ function `long modpower(long base, long exp, long mod)` that computes `(base^exp) % mod` efficiently using exponentiation by squaring, while handling negative exponents by computing the modular inverse of the base (using Fermat's Little Theorem since `mod` is prime). The inputs satisfy `0 <= base < mod`, `mod` is a prime number, and `exp` can be any `long` (including negative). The result must always be in the range `[0, mod-1]`. The function should be self-contained, not relying on external libraries for modular arithmetic.
*/
#include <cstddef>

// Computes (base^exp) % mod using exponentiation by squaring.
// Preconditions: mod is prime, 0 <= base < mod, exp is any long.
// For negative exp, base must be nonzero modulo mod.
long modpower(long base, long exp, long mod) {
    // Handle base modulo mod (already in range but ensure positive)
    base %= mod;
    if (base < 0) base += mod;

    // Handle exponent zero
    if (exp == 0) return 1 % mod;

    // For negative exponent, replace base with its modular inverse using Fermat's Little Theorem
    if (exp < 0) {
        // Compute base^(mod-2) % mod as the inverse
        long inverse = 1;
        long power = base;
        long e = mod - 2;
        while (e > 0) {
            if (e & 1) inverse = (inverse * power) % mod;
            power = (power * power) % mod;
            e >>= 1;
        }
        base = inverse;
        exp = -exp;
    }

    // Binary exponentiation for positive exponent
    long result = 1;
    long power = base;
    while (exp > 0) {
        if (exp & 1) result = (result * power) % mod;
        power = (power * power) % mod;
        exp >>= 1;
    }
    return result;
}
int main() {
    // Basic positive exponent
    assert(modpower(2, 10, 1000000007) == 1024);
    // Zero exponent returns 1
    assert(modpower(5, 0, 1000000007) == 1);
    // Base 0 with positive exponent
    assert(modpower(0, 5, 7) == 0);
    // Negative exponent: inverse of 2 mod 5 is 3, so 2^-1 = 3, 2^-2 = 3^2=9%5=4
    assert(modpower(2, -1, 5) == 3);
    assert(modpower(2, -2, 5) == 4);
    // Larger prime and exponent
    assert(modpower(3, 20, 17) == 3*3*3*3*3*3*3*3*3*3*3*3*3*3*3*3*3*3*3*3 % 17);
    // Ensure repetition matches iterative squaring
    long p = 1000000007;
    long result = 1;
    for (int i = 0; i < 10; i++) result = (result * 123) % p;
    assert(modpower(123, 10, p) == result);
    // Negative exponent with base 1
    assert(modpower(1, -100, 13) == 1);
    // Edge case: exponent is very large negative
    assert(modpower(7, -3, 11) == modpower(7, 8, 11)); // since 7^10 = 1 mod 11, 7^-3 = 7^7
}
// The algorithm uses binary exponentiation: for a positive exponent `e`, we process its bits from least significant to most significant, maintaining a running result and a power of the base squared at each step. If the current bit is set, multiply the result by the current power modulo `mod`. For negative exponents, first compute the modular inverse of the base using Fermat's Little Theorem: `base^(mod-2) % mod` (valid because `mod` is prime and `base` is nonzero modulo `mod`). Then raise this inverse to the absolute value of the exponent using the same binary exponentiation. Handle the edge case where `exp == 0` by returning `1 % mod` (which is `1` since `mod > 1`). Also handle the case `base == 0` with a negative exponent: since `0` has no modular inverse, assume the input is valid (i.e., `base != 0` when `exp < 0`). The time complexity is `O(log |exp|)` multiplications, each taking constant time on fixed-width integers, and space complexity is `O(1)`.
