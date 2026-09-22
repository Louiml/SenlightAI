// Write a C++ function named `integerSquareRoot` that takes a non-negative 32-bit unsigned integer and returns its square root rounded to the nearest integer using standard round-half-away-from-zero semantics (i.e., values exactly halfway between two integers round up). The function must compute the result using a floating-point calculation and cast the rounded value back to a 16-bit unsigned integer. You may assume the input is such that the true square root fits within the range of `uint16_t` (i.e., input ≤ 4,294,836,225). Do not use any external libraries beyond the standard C++ headers.
The core algorithm is straightforward: convert the input to a `double`, compute its square root via `std::sqrt`, then round the result to the nearest integer using `std::round`, and finally cast to `uint16_t`. The key edge case involves values like 0 (square root 0) and values where the square root is exactly a half-integer (e.g., input = 2.25 gives 1.5, which rounds to 2). `std::round` correctly handles these. Another edge case is large inputs near the maximum (e.g., 4,294,836,225), where the square root is 65535.999… which rounds to 65536? Actually 65535² = 4,294,836,225, so the exact square root is 65535.0, but for inputs slightly above that up to the limit, the true sqrt is < 65536, and rounding gives either 65535 or 65536 depending on the value. The problem assumes the input is such that the true square root fits in `uint16_t`, so the result of rounding will always be ≤ 65535, but the cast is safe anyway. The time complexity is O(1) because it’s a single floating-point operation, and the space complexity is O(1).
#include <cstdint>
#include <cmath>

// Compute the nearest integer to the square root of a non-negative 32-bit unsigned integer.
// The input must be such that the rounded result fits in uint16_t.
uint16_t integerSquareRoot(uint32_t input) {
    // Compute square root in double precision.
    double sqrtValue = std::sqrt(static_cast<double>(input));
    // Round to nearest integer (half away from zero).
    double rounded = std::round(sqrtValue);
    // Cast to uint16_t (safe per problem constraints).
    return static_cast<uint16_t>(rounded);
}
#include <cassert>
#include <cstdint>

// Declaration of the solution function (would be included via header in practice).
uint16_t integerSquareRoot(uint32_t input);

int main() {
    // Perfect squares
    assert(integerSquareRoot(0) == 0);
    assert(integerSquareRoot(1) == 1);
    assert(integerSquareRoot(4) == 2);
    assert(integerSquareRoot(9) == 3);
    assert(integerSquareRoot(65535 * 65535) == 65535); // Maximum perfect square fitting in uint32_t

    // Non-perfect squares
    assert(integerSquareRoot(2) == 1);   // sqrt(2) ≈ 1.414 → 1
    assert(integerSquareRoot(3) == 2);   // sqrt(3) ≈ 1.732 → 2
    assert(integerSquareRoot(10) == 3);  // sqrt(10) ≈ 3.162 → 3
    assert(integerSquareRoot(15) == 4);  // sqrt(15) ≈ 3.873 → 4

    // Half-integer rounding cases
    assert(integerSquareRoot(2) == 1);     // Not half, but check
    assert(integerSquareRoot(6) == 2);     // sqrt(6) ≈ 2.449 → 2
    assert(integerSquareRoot(7) == 3);     // sqrt(7) ≈ 2.646 → 3
    assert(integerSquareRoot(12) == 3);    // sqrt(12) ≈ 3.464 → 3
    assert(integerSquareRoot(20) == 4);    // sqrt(20) ≈ 4.472 → 4

    // Edge case: large value just above a perfect square
    assert(integerSquareRoot(65535 * 65535 + 1) == 65535); // sqrt ≈ 65535.0000076 → 65535

    return 0;
}
