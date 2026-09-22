// Write a C++ function named `isFiniteValue` that takes a `double` argument and returns a `bool` indicating whether the value is finite. A value is considered finite if it is not infinity (positive or negative) and not NaN (Not-a-Number). The function must operate by inspecting the raw 64-bit IEEE 754 representation of the `double` (e.g., using a `uint64_t` and bit masks) rather than relying on standard library functions like `std::isfinite`. It must correctly handle all categories: finite normal numbers, subnormal numbers, positive/negative zero, infinities, and NaNs. Assume the platform uses IEEE 754 double-precision format (64 bits). The solution must not include a `main` function; only the function definition and any necessary helper types or constants (placed inside a namespace if desired).

// The core idea is to understand the IEEE 754 binary64 layout: 1 sign bit, 11 exponent bits, and 52 fraction (mantissa) bits. The exponent bits, when interpreted as an unsigned integer (call it `e`), determine the category:
// - If `e == 0x7FF` (all exponent bits set) and the fraction bits are all zero: the value is ±infinity → not finite.
// - If `e == 0x7FF` and the fraction bits are non-zero: the value is NaN (either quiet or signaling) → not finite.
// - Any other exponent value (including `e == 0` for zeros and subnormals) indicates a finite value.
// Thus, the algorithm copies the `double`'s bits into a `uint64_t` (using `memcpy` or a union/bit-cast), extracts the exponent bits, and checks if they equal `0x7FF`. If so, return `false`; otherwise return `true`. This correctly treats both +0.0 and -0.0 as finite (they have exponent 0). Edge cases include subnormal numbers (exponent 0, fraction non-zero) which are finite, and negative NaN values which are still non-finite. The time complexity is O(1) and space complexity is O(1). Using `memcpy` is the safest portable way to alias raw bits without violating strict aliasing rules.

#include <cstdint>
#include <cstring>

// Return true if the double is finite (not ±Inf and not NaN), false otherwise.
bool isFiniteValue(double value) {
    std::uint64_t bits = 0;
    std::memcpy(&bits, &value, sizeof(bits));
    constexpr std::uint64_t exponentMask = 0x7FF0000000000000ULL;
    const std::uint64_t exponent = (bits & exponentMask) >> 52;
    return exponent != 0x7FF;
}

#include <cassert>
#include <cmath>

int main() {
    // Finite normal numbers
    assert(isFiniteValue(0.0) == true);
    assert(isFiniteValue(-0.0) == true);
    assert(isFiniteValue(1.0) == true);
    assert(isFiniteValue(-123.456) == true);
    assert(isFiniteValue(1.7976931348623157e308) == true);  // DBL_MAX

    // Subnormal numbers (smallest positive and negative)
    assert(isFiniteValue(4.9406564584124654e-324) == true);
    assert(isFiniteValue(-4.9406564584124654e-324) == true);

    // Infinities
    assert(isFiniteValue(INFINITY) == false);
    assert(isFiniteValue(-INFINITY) == false);

    // NaN (quiet and signaling)
    assert(isFiniteValue(NAN) == false);
    assert(isFiniteValue(std::nan("1")) == false);

    return 0;
}
