/*
Write a C++ function named `modularExponentiation` that takes three non-negative 64-bit integers `base`, `exponent`, and `modulus` (with `modulus` > 0) and returns `(base^exponent) % modulus` as a 64-bit integer. The function must work correctly even when `exponent` is 0 (returning 1 % modulus), when `base` is 0 and `exponent` is positive (returning 0), and when `modulus` is 1 (returning 0). The computation must avoid overflow by using the square-and-multiply algorithm with modular reduction at each step. The function must be `const`-correct and have no side effects.
*/
#include <cstdint>

// Compute (base^exponent) % modulus using binary exponentiation.
// Preconditions: modulus > 0, base >= 0, exponent >= 0.
// Returns the result as a 64-bit unsigned integer.
std::uint64_t modularExponentiation(std::uint64_t base, std::uint64_t exponent, std::uint64_t modulus) {
    if (modulus == 0) {
        return 0; // Undefined, but return 0 for safety.
    }
    std::uint64_t result = 1 % modulus;
    std::uint64_t currentBase = base % modulus;
    while (exponent > 0) {
        if (exponent & 1) {
            result = (result * currentBase) % modulus;
        }
        currentBase = (currentBase * currentBase) % modulus;
        exponent >>= 1;
    }
    return result;
}
#include <cassert>
#include <cstdint>

// Declaration of the function under test.
std::uint64_t modularExponentiation(std::uint64_t base, std::uint64_t exponent, std::uint64_t modulus);

int main() {
    // Basic cases
    assert(modularExponentiation(2, 10, 1000) == 24);          // 1024 % 1000 = 24
    assert(modularExponentiation(3, 5, 7) == 5);               // 243 % 7 = 5
    assert(modularExponentiation(5, 0, 10) == 1);              // 5^0 = 1
    assert(modularExponentiation(0, 5, 10) == 0);              // 0^5 = 0
    assert(modularExponentiation(7, 3, 1) == 0);               // modulus 1 => 0
    // Large exponent to test loop correctness
    assert(modularExponentiation(2, 63, 1000000007) == 9223372036854775808ULL % 1000000007);
    // Base greater than modulus
    assert(modularExponentiation(10, 3, 7) == 6);              // 1000 % 7 = 6
    // Even exponent and odd exponent
    assert(modularExponentiation(4, 2, 15) == 1);              // 16 % 15 = 1
    assert(modularExponentiation(4, 3, 15) == 4);              // 64 % 15 = 4
    // Large modulus near 64-bit limit
    assert(modularExponentiation(123456789, 987654321, 1000000007) == 309367391);
    // Exponent is 1
    assert(modularExponentiation(9, 1, 100) == 9);
    // Base is modulus
    assert(modularExponentiation(7, 2, 7) == 0);
    return 0;
}
// The solution uses the fast exponentiation method (binary exponentiation or square-and-multiply). The algorithm processes the bits of the exponent from least significant to most significant. We maintain a result accumulator `result` initialized to 1 % modulus (to handle modulus == 1 correctly), and a running base value `currentBase` initialized to `base % modulus`. For each bit of the exponent, if the bit is 1, we multiply the result by the current base and take modulo `modulus`. Then we square the current base and take modulo `modulus`, and shift the exponent right by one bit. This works because the exponent's binary representation decomposes the power into a product of squared bases corresponding to set bits. Edge cases: if modulus is 1, every modulo operation yields 0, so returning 1 % 1 = 0 is correct. If exponent is 0, the loop never runs, and we return 1 % modulus. If base is 0 and exponent > 0, the result becomes 0 after the first multiplication because 0 % modulus = 0, and all subsequent multiplications keep it 0. The time complexity is O(log exponent) because we halve the exponent each iteration, and space complexity is O(1).
