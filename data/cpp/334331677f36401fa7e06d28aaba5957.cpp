// Write a C++ function that takes a single positive integer `R` representing the radius of a circle, and returns a `std::string` representing the area of a square circumscribed around that circle, formatted with an exact suffix of `.25`. More specifically, the area of the square (which has side length `2R`) is `(2R)^2 = 4R^2`. The function must output the integer part of `4*R*R` followed immediately by `".25"` — this corresponds to the original snippet which effectively computed `4 * R^2` and then appended `.25` (a fixed fractional part, likely because the actual circle area formula involves π but the problem wanted a scaled value). The function must handle very large radii up to 10^9, so use 64-bit integers to avoid overflow; the result string should have no spaces. The function should be named `circleSquareAreaString` and take a `long long radius` as input, returning the formatted string.
The core computation is straightforward: given radius `R`, the integer part is `4 * R * R`. Since `R` can be up to 10^9, `R*R` can be up to 10^18, and multiplying by 4 can exceed 10^18 but stays well within the 64-bit `long long` range (max ~9.2e18). Therefore, using `long long` for the multiplication is safe. The output format requires us to append `".25"` to the integer value. For example, if `R=1`, then `4*1*1=4`, so the output is `"4.25"` (which matches the original code's `r=r*r*4; cout<<...<<".25"`). Edge cases: `R=0` would give `0.25`; the original code didn't restrict positive, but we can handle zero or positive values gracefully. The algorithm runs in O(1) time and O(1) auxiliary space, since it's just arithmetic and string concatenation. One important note: in the original snippet, the input `r` was read as `long long`, and the output was `"Case i: " << r << ".25"` — so we preserve that exact format logic. The solution function should not print anything; it should return the formatted string (without the "Case" prefix, as that is test-specific). The test code will call this function and compare against expected strings.
#include <string>
#include <cstdint>

// Given a circle radius, return a string representing 4 * radius^2 followed by ".25".
// The computed integer part fits in a 64-bit signed integer for radius up to 1e9.
std::string circleSquareAreaString(long long radius) {
    // Compute the integer part as a 64-bit value.
    long long areaIntegerPart = 4LL * radius * radius;
    // Convert to string and append the fixed fractional suffix.
    return std::to_string(areaIntegerPart) + ".25";
}
#include <cassert>
#include <string>

// The solution function is declared above; this main runs assertions.
int main() {
    assert(circleSquareAreaString(1) == "4.25");
    assert(circleSquareAreaString(2) == "16.25");
    assert(circleSquareAreaString(0) == "0.25");
    assert(circleSquareAreaString(10) == "400.25");
    assert(circleSquareAreaString(1000000000) == "4000000000000000000.25");
    assert(circleSquareAreaString(3) == "36.25");
    assert(circleSquareAreaString(5) == "100.25");
    assert(circleSquareAreaString(7) == "196.25");
    return 0;
}
