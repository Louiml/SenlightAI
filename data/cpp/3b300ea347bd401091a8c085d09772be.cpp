Write a C++ function named `modularExponentiation` that takes three integers `base`, `exponent`, and `modulus` (all non-negative, with `modulus` possibly zero, in which case the result should be `0` per convention) and returns `(base^exponent) mod modulus` as an `int`. Handle negative intermediate results (which can occur in C++ when the base is negative) by converting the final answer to a non-negative value in the range `[0, modulus-1]` if `modulus > 0`. The function must avoid integer overflow by using `long long` for intermediate multiplication. The exponent can be very large (up to `2^31 - 1`), so the algorithm must be efficient. If `exponent == 0`, return `1 % modulus` (which is `0` when `modulus == 1`, `1` when `modulus > 1`, and `0` when `modulus == 0` following the convention). The function must be self-contained and not rely on any external libraries beyond standard headers.
#include <cassert>

int main() {
    // Basic cases
    assert(modularExponentiation(2, 3, 5) == 3);       // 8 % 5 = 3
    assert(modularExponentiation(2, 10, 1000) == 24);  // 1024 % 1000 = 24
    // Exponent zero
    assert(modularExponentiation(5, 0, 7) == 1);
    assert(modularExponentiation(5, 0, 1) == 0);
    // Modulus zero
    assert(modularExponentiation(3, 4, 0) == 0);
    // Negative base
    assert(modularExponentiation(-2, 3, 5) == 2);      // (-8) % 5 = -3, adjusted to 2
    assert(modularExponentiation(-1, 2, 7) == 1);
    // Large exponent to test efficiency
    assert(modularExponentiation(2, 31, 1000000007) == 73741817); // 2^31 % 1e9+7
    // Large base and modulus
    assert(modularExponentiation(123456789, 5, 987654321) == 319233978); // computed externally
    // Modulus 1 always yields 0 (except exponent zero case handled above)
    assert(modularExponentiation(7, 100, 1) == 0);
    // Zero base and positive exponent
    assert(modularExponentiation(0, 5, 7) == 0);
    // One base
    assert(modularExponentiation(1, 1000000, 999) == 1);
    return 0;
}
#include <cstdint>

// Returns (base^exponent) mod modulus, handling negative base and modulus == 0.
// If modulus == 0, returns 0 by convention.
int modularExponentiation(int base, int exponent, int modulus) {
    if (modulus == 0) {
        return 0;
    }
    if (exponent == 0) {
        return 1 % modulus;
    }
    long long ans = 1 % modulus;
    long long currentBase = base % modulus;
    // Ensure base is non-negative for multiplication
    if (currentBase < 0) {
        currentBase += modulus;
    }
    int n = exponent;
    while (n > 0) {
        if (n % 2 == 1) {
            ans = (ans * currentBase) % modulus;
            --n;
        } else {
            currentBase = (currentBase * currentBase) % modulus;
            n /= 2;
        }
    }
    // ans is already in [0, modulus-1] because we used modulus after each step
    // But ensure it's non-negative in case modulus is not positive? modulus > 0 here.
    if (ans < 0) {
        ans += modulus;
    }
    return static_cast<int>(ans);
}
// The problem is to compute modular exponentiation efficiently. The core algorithm is binary exponentiation (also called fast exponentiation or exponentiation by squaring). The idea is to process the exponent bit by bit: while the exponent is positive, if it is odd, multiply the current result by the base and reduce modulo `modulus`, then decrement the exponent; if it is even, square the base and reduce modulo `modulus`, then halve the exponent. This works because `(base^exponent) mod m` can be computed by repeated squaring and multiplication. Important edge cases: (1) if `modulus == 0`, the result is undefined; we return `0` by convention. (2) When `exponent == 0`, the result is `1 % modulus`, which handles the case `modulus == 1` (giving `0`) and `modulus == 0` (giving `1 % 0` is invalid, so we must check `modulus == 0` first and return `0`). (3) If `base` is negative, the multiplication `ans * base` may produce a negative value, so after the loop we adjust the result to be non-negative by adding `modulus` repeatedly if negative, or using `(ans % modulus + modulus) % modulus` which works even for negative `ans`. The time complexity is O(log exponent) because we halve the exponent each iteration, and space complexity is O(1). Use `long long` for intermediate products to avoid overflow since `ans` and `base` can be up to `modulus-1` (which can be near `2^31`), and their product can exceed `2^31`.
