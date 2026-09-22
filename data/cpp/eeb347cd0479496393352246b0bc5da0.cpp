/*
Write a C++ function named `addCppIntWithDouble` that takes a `cpp_int` (from `nil::crypto3::multiprecision`) and a `double`, and returns a `cpp_int` representing the sum of the two values, rounded to the nearest integer (with halves rounding away from zero). The function must handle negative doubles, fractional parts, and large integer values without overflow. The input `cpp_int` may be zero, positive, or negative. The function must not modify its inputs and must be `const`-correct.
*/

#include <nil/crypto3/multiprecision/cpp_int.hpp>
#include <string>
#include <cmath>

using namespace nil::crypto3::multiprecision;

// Add a double (rounded to nearest integer, halves away from zero) to a cpp_int.
cpp_int addCppIntWithDouble(const cpp_int& base, double value) {
    // Split double into integral and fractional parts
    double integral_part;
    double fractional_part = std::modf(value, &integral_part);

    // Round to nearest, halves away from zero
    double rounded = integral_part;
    double abs_frac = std::fabs(fractional_part);
    if (abs_frac >= 0.5) {
        if (value >= 0) {
            rounded = integral_part + 1.0;
        } else {
            rounded = integral_part - 1.0;
        }
    }

    // Convert rounded double to string (safe for cpp_int construction)
    std::string rounded_str = std::to_string(rounded);
    // Remove trailing ".0" if present
    if (rounded_str.find(".0") != std::string::npos) {
        rounded_str = rounded_str.substr(0, rounded_str.find(".0"));
    }

    cpp_int rounded_int(rounded_str);
    return base + rounded_int;
}

#include <cassert>
#include <nil/crypto3/multiprecision/cpp_int.hpp>

using namespace nil::crypto3::multiprecision;

// Declaration of the function under test (as defined in solution)
cpp_int addCppIntWithDouble(const cpp_int& base, double value);

int main() {
    // Basic positive addition
    assert(addCppIntWithDouble(cpp_int(3), 3.3) == cpp_int(6));  // 3.3 rounds to 3

    // Fractional part >= 0.5 rounds up
    assert(addCppIntWithDouble(cpp_int(0), 2.5) == cpp_int(3));

    // Negative value with fraction >= 0.5 rounds away from zero
    assert(addCppIntWithDouble(cpp_int(10), -2.5) == cpp_int(7)); // -2.5 rounds to -3

    // Exact integer double
    assert(addCppIntWithDouble(cpp_int(100), 7.0) == cpp_int(107));

    // Negative base and negative double
    assert(addCppIntWithDouble(cpp_int(-5), -1.6) == cpp_int(-7)); // -1.6 rounds to -2

    // Zero double with positive base
    assert(addCppIntWithDouble(cpp_int(42), 0.0) == cpp_int(42));

    // Large value: add 1.4 (rounds to 1) to large integer
    cpp_int large("123456789012345678901234567890");
    assert(addCppIntWithDouble(large, 1.4) == cpp_int("123456789012345678901234567891"));

    // Fraction exactly 0.5 with negative sign and negative base
    assert(addCppIntWithDouble(cpp_int(-1), -0.5) == cpp_int(-2)); // -0.5 rounds to -1

    // Fraction exactly 0.5 with positive sign and zero base
    assert(addCppIntWithDouble(cpp_int(0), 0.5) == cpp_int(1));

    return 0;
}

// The task involves adding an arbitrary-precision integer (`cpp_int`) to a floating-point double. Since `cpp_int` supports arbitrary size, the double must be converted to a `cpp_int` representation after rounding. The main algorithm: first, extract the integral and fractional parts of the double using `std::floor` and `std::modf`. For rounding to nearest with halves away from zero, check the absolute fractional part: if it is >= 0.5, add 1 to the absolute integral part with the correct sign. Then convert the rounded integral double to a `cpp_int` using a string or direct conversion (careful: `cpp_int` does not have a direct double constructor that rounds correctly). Use `std::to_string` on the rounded integral double (which yields a string like "-3" or "5") and construct `cpp_int` from that string. Then add this to the input `cpp_int`. Edge cases: negative doubles, exactly .5 fractions, zero, and very large doubles that may lose precision (but within double range, it's fine). Time complexity: O(n) where n is the number of digits in the integer part of the double (due to string conversion), and O(n) auxiliary space for the string and result. The `cpp_int` addition is O(m) where m is the number of limbs, but we can state overall O(n) for typical cases.
