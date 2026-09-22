Write a C++ function named `basicFloatMath` that takes two `float` parameters `a` and `b`, and returns a `std::string` containing the results of the four basic arithmetic operations (addition, subtraction, multiplication, and division) formatted as `"sum: <sum>, diff: <diff>, product: <product>, quotient: <quotient>"`. The quotient must be formatted with exactly 3 decimal places (e.g., using `std::fixed` and `std::setprecision`). The function must handle division by zero gracefully: if `b` is exactly `0.0f`, return the string `"division by zero"` instead of performing the division. Use `const` for all parameters and avoid any side effects. The function should be self-contained with all necessary headers included.

// The solution directly performs the four arithmetic operations on the inputs. For addition, subtraction, and multiplication, we simply compute the results and format them. For division, we must check if the divisor `b` is exactly zero to avoid undefined behavior (floating-point division by zero yields `inf` or `NaN` in IEEE 754, but the task requires a graceful return). We use `std::ostringstream` to build the output string, setting the precision for the quotient only via `std::fixed << std::setprecision(3)`. The other values can be formatted using default `std::to_string` or by directly streaming them (streaming gives a default precision of 6 significant digits, which is acceptable). Edge cases: if `b` is `0.0f`, we skip division and return the error message; if `a` or `b` are negative or fractional, normal arithmetic applies. Time complexity is O(1) and space complexity is O(1) for the output string.

#include <string>
#include <sstream>
#include <iomanip>

// Perform basic float arithmetic and return a formatted string.
// If b is zero, division is skipped and an error message is returned.
std::string basicFloatMath(const float a, const float b) {
    const float sum = a + b;
    const float diff = a - b;
    const float product = a * b;

    if (b == 0.0f) {
        return "division by zero";
    }

    const float quotient = a / b;
    std::ostringstream out;
    out << "sum: " << sum;
    out << ", diff: " << diff;
    out << ", product: " << product;
    out << ", quotient: " << std::fixed << std::setprecision(3) << quotient;
    return out.str();
}

#include <cassert>
#include <string>

// Declaration of the solution function (normally would be in a header)
std::string basicFloatMath(const float a, const float b);

int main() {
    // Basic positive numbers
    assert(basicFloatMath(9.8f, 3.14f) == "sum: 12.94, diff: 6.66, product: 30.772, quotient: 3.121");
    // Negative and fractional
    assert(basicFloatMath(-5.5f, 2.0f) == "sum: -3.5, diff: -7.5, product: -11, quotient: -2.750");
    // Division by zero returns error string
    assert(basicFloatMath(1.0f, 0.0f) == "division by zero");
    // Zero numerator, nonzero denominator
    assert(basicFloatMath(0.0f, 5.0f) == "sum: 5, diff: -5, product: 0, quotient: 0.000");
    // Both negative
    assert(basicFloatMath(-2.5f, -0.5f) == "sum: -3, diff: -2, product: 1.25, quotient: 5.000");
    // Integer-like values
    assert(basicFloatMath(10.0f, 4.0f) == "sum: 14, diff: 6, product: 40, quotient: 2.500");
    // Small values
    assert(basicFloatMath(0.1f, 0.2f) == "sum: 0.3, diff: -0.1, product: 0.02, quotient: 0.500");
    // Very small denominator, quotient with rounding
    assert(basicFloatMath(1.0f, 3.0f) == "sum: 4, diff: -2, product: 3, quotient: 0.333");
    // Negative denominator
    assert(basicFloatMath(7.0f, -2.0f) == "sum: 5, diff: 9, product: -14, quotient: -3.500");
    // Both zero, but b zero triggers error
    assert(basicFloatMath(0.0f, 0.0f) == "division by zero");
    return 0;
}
