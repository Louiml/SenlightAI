Write a C++ function `computeBMI(double weightKg, double heightM)` that calculates and returns the Body Mass Index (BMI) given a person's weight in kilograms and height in meters. Use the formula `BMI = weight / (height * height)`. The function must validate inputs: if height is less than or equal to 0 or weight is negative, return a special sentinel value `-1.0` to indicate invalid input (since a real BMI is always positive). Normalize the result to two decimal places (e.g., using `std::round` and `std::setprecision` logic or by returning a value rounded to nearest hundredth). The function must be `const`-correct and not throw exceptions. The task requires handling only `double` inputs, and you must implement the logic yourself without using `pow` from `<cmath>` for the square calculation (use multiplication for clarity and performance).

// The solution computes `BMI = weight / (height * height)` directly. First, validate inputs: `height <= 0.0` or `weight < 0.0` are invalid, so return `-1.0`. Otherwise, compute the raw BMI. Then round to two decimal places using `std::round(rawBMI * 100.0) / 100.0`. This ensures consistent output for floating-point comparisons. Edge cases: height exactly 0 triggers invalid; very large or small values may overflow, but we assume reasonable input. Time complexity is `O(1)` (constant operations), space complexity `O(1)`. The function uses only `<cmath>` for `std::round` and `<limits>` if needed, but for simplicity we just use `#include <cmath>`. The function is a free function with no side effects and marked `const`? Actually functions cannot be `const` unless they are member functions; here we apply `const` to local variables where appropriate. We'll ensure the implementation is self-contained.

#include <cmath>  // for std::round

// Compute BMI given weight in kg and height in meters.
// Returns -1.0 for invalid inputs (height <= 0 or weight < 0).
// Rounds result to two decimal places.
double computeBMI(double weightKg, double heightM) {
    // Validate inputs
    if (heightM <= 0.0 || weightKg < 0.0) {
        return -1.0;
    }
    // Calculate raw BMI: weight / height^2
    double heightSquared = heightM * heightM;
    double rawBmi = weightKg / heightSquared;
    // Round to two decimal places
    double roundedBmi = std::round(rawBmi * 100.0) / 100.0;
    return roundedBmi;
}

#include <cassert>
#include <cmath>

// The solution function is declared here (or included from header)
double computeBMI(double weightKg, double heightM);

int main() {
    // Normal cases
    assert(std::abs(computeBMI(70.0, 1.75) - 22.86) < 0.001);
    assert(std::abs(computeBMI(50.0, 1.60) - 19.53) < 0.001);
    assert(std::abs(computeBMI(90.0, 1.80) - 27.78) < 0.001);
    // Boundary: zero weight is valid (BMI = 0)
    assert(computeBMI(0.0, 1.70) == 0.0);
    // Invalid inputs
    assert(computeBMI(70.0, 0.0) == -1.0);
    assert(computeBMI(-5.0, 1.75) == -1.0);
    assert(computeBMI(70.0, -1.2) == -1.0);
    // Rounding behavior: exact two decimal
    assert(std::abs(computeBMI(100.0, 2.0) - 25.0) < 0.001);
    assert(std::abs(computeBMI(60.0, 1.50) - 26.67) < 0.001);
    return 0;
}
