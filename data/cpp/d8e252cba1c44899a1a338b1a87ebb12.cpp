Write a C++ function that, given a non-negative integer `mode`, returns a string representing the result of a series of conditional mathematical computations. If `mode == 1`, the function must compute and return the value of `(8 * pow(z, 2) + 1) / (y + pow(t, 2))`, where `z = 1`, `t = 2`, and `y = t + z`. If `mode == 2`, use the constants `x = 1.82`, `y = 1.25`, compute `z = sqrt(15 * y) / (y + cos(x)/sin(x))`, and then return `(y - z / y - x) / (cos(x) / pow(x - y, 2))`. If `mode` is any other value, return the string `"Invalid mode"`. All results must be formatted as a decimal number with at least 3 digits after the decimal point. The function must not read any input or print anything to the console.
// The task requires implementing a conditional mathematical computation based on an integer input. For each mode, we must carefully compute intermediate values using the specified formulas, taking care to use `double` for all floating-point operations to avoid integer division issues. For mode 1, the constants are simple integers, but the expression involves division, so we must cast or use floating-point literals. For mode 2, we need to handle the trigonometric functions `cos` and `sin`, which require radians (though the input `x=1.82` is already in radians). The expression `ctgx` is defined as `cos(x)/sin(x)` (cotangent), and we must compute it before using `z`. Edge cases: when `mode` is not 1 or 2, we return the error string. Since `sin(x)` for `x=1.82` is not zero, no division by zero occurs in mode 2. For mode 1, `y + t^2` equals `3 + 4 = 7`, non-zero, so safe. Formatting is done with `std::ostringstream` and `std::fixed << std::setprecision(3)` to ensure at least 3 decimal places. Time complexity is O(1) since only a few arithmetic operations are performed, and space complexity is O(1) aside from the returned string.
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>

// Computes a mathematical expression based on the mode.
// mode 1: returns (8*pow(z,2)+1)/(y+pow(t,2)) with z=1, t=2, y=t+z
// mode 2: returns formula with x=1.82, y=1.25, z=sqrt(15*y)/(y+cos(x)/sin(x))
// other: returns "Invalid mode"
std::string computeByMode(int mode) {
    if (mode == 1) {
        const double z = 1.0;
        const double t = 2.0;
        const double y = t + z;
        const double result = (8.0 * std::pow(z, 2) + 1.0) / (y + std::pow(t, 2));
        std::ostringstream out;
        out << std::fixed << std::setprecision(3) << result;
        return out.str();
    }
    if (mode == 2) {
        const double x = 1.82;
        const double y = 1.25;
        const double ctgx = std::cos(x) / std::sin(x);
        const double z = std::sqrt(15.0 * y) / (y + ctgx);
        const double result = (y - z / y - x) / (std::cos(x) / std::pow(x - y, 2));
        std::ostringstream out;
        out << std::fixed << std::setprecision(3) << result;
        return out.str();
    }
    return "Invalid mode";
}
#include <cassert>
#include <string>

// Solution function declaration (include in test file or link separately)
std::string computeByMode(int mode);

int main() {
    // Known values computed independently
    // Mode 1: z=1, t=2, y=3 -> (8*1+1)/(3+4) = 9/7 ≈ 1.285714 -> "1.286"
    assert(computeByMode(1) == "1.286");
    // Mode 2: verify manually with a calculator for x=1.82, y=1.25
    // ctgx = cos(1.82)/sin(1.82) ≈ -0.2463 / 0.9692 ≈ -0.254
    // z = sqrt(18.75)/(1.25-0.254) ≈ 4.330 / 0.996 ≈ 4.348
    // numerator = (1.25 - 4.348/1.25 - 1.82) = (1.25 - 3.478 - 1.82) = -4.048
    // denominator = cos(1.82)/pow(0.57,2) = -0.2463/0.3249 ≈ -0.758
    // result ≈ -4.048 / -0.758 ≈ 5.341 -> "5.341" (approximate)
    assert(computeByMode(2) == "5.341");
    // Invalid modes
    assert(computeByMode(0) == "Invalid mode");
    assert(computeByMode(3) == "Invalid mode");
    assert(computeByMode(-1) == "Invalid mode");
    return 0;
}
