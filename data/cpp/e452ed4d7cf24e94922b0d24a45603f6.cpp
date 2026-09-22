// Write a C++ function that takes two non-zero integers `a` and `b` as parameters and returns a string containing the integer quotient, the integer remainder (using truncation toward zero, as C++ does for integers), and the floating-point result of `a / b` with exactly 6 decimal places, all separated by single spaces. The function must handle negative numbers correctly (for example, `-7 / 2` gives quotient `-3` and remainder `-1`), and must not modify its inputs. Assume `b` is never zero.

// The solution computes the integer quotient using `a / b` and the remainder using `a % b`, relying on C++'s built-in truncation-toward-zero semantics for integers, which is important when either operand is negative. To obtain the floating-point result with exactly 6 decimal places, convert both integers to `double` and perform division, then format the result using `std::ostringstream` with `std::fixed` and `std::setprecision(6)`. Edge cases: if `a` divides exactly by `b`, the remainder is zero but must still be printed (e.g., `0`). Negative divisors are handled naturally by C++'s operator semantics. The time complexity is O(1) and auxiliary space is O(1) (ignoring the returned string). No special handling for zero divisor is needed per the specification.

#include <string>
#include <sstream>
#include <iomanip>

// Given two non-zero integers a and b, return a string with:
// integer quotient, integer remainder, and floating result (6 decimals), space-separated.
std::string divisionDetails(int a, int b) {
    const int quotient = a / b;
    const int remainder = a % b;

    const double d_a = static_cast<double>(a);
    const double d_b = static_cast<double>(b);
    const double division = d_a / d_b;

    std::ostringstream out;
    out << quotient << ' ' << remainder << ' ';
    out << std::fixed << std::setprecision(6) << division;
    return out.str();
}

#include <cassert>
#include <string>

// Forward declaration for testing purposes (the real function is above).
std::string divisionDetails(int a, int b);

int main() {
    // Positive division
    assert(divisionDetails(7, 2) == "3 1 3.500000");
    // Negative dividend
    assert(divisionDetails(-7, 2) == "-3 -1 -3.500000");
    // Negative divisor
    assert(divisionDetails(7, -2) == "-3 1 -3.500000");
    // Both negative
    assert(divisionDetails(-7, -2) == "3 -1 3.500000");
    // Exact division
    assert(divisionDetails(10, 5) == "2 0 2.000000");
    // Larger numbers with negative
    assert(divisionDetails(100, -3) == "-33 1 -33.333333");
    // Zero dividend
    assert(divisionDetails(0, 3) == "0 0 0.000000");
    // One divided by a negative
    assert(divisionDetails(1, -4) == "0 1 -0.250000");
    return 0;
}
