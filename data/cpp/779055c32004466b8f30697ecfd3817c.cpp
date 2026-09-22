// Write a C++ function named `temperatureConversion` that accepts an integer number of Celsius degrees and returns a `double` representing the equivalent temperature in Fahrenheit, computed using the formula \( F = 1.6 \times C \). The result must be rounded to exactly one decimal place. The function should be pure (no input/output) and work for any integer input, including negative values and zero. The caller is responsible for formatting the output; your function returns the unformatted `double` value that, when printed with one decimal place, gives the expected rounded result. Ensure the conversion is performed using floating-point arithmetic (not integer division).
The solution is straightforward: multiply the integer Celsius value by the constant factor `1.6` as a `double`. To guarantee correct rounding to one decimal place, perform the multiplication in floating point (e.g., `1.6 * static_cast<double>(celsius)`). Since the input is an integer, no special edge cases arise beyond handling negative numbers and zero, which work naturally with floating-point multiplication. The function returns a `double`; formatting precision is left to the output stream. Time complexity is \(O(1)\) and space complexity is \(O(1)\). The only subtlety is that the multiplication must be done in double precision to avoid truncation or integer rounding.
#include <cmath>

// Convert Celsius to Fahrenheit using F = 1.6 * C.
// Returns a double representing the temperature in Fahrenheit.
double temperatureConversion(int celsius) {
    const double factor = 1.6;
    return factor * static_cast<double>(celsius);
}
#include <cassert>
#include <cmath>

int main() {
    // Test common values with rounding to one decimal place.
    assert(std::abs(temperatureConversion(0) - 0.0) < 1e-9);
    assert(std::abs(temperatureConversion(10) - 16.0) < 1e-9);
    assert(std::abs(temperatureConversion(-5) - (-8.0)) < 1e-9);
    assert(std::abs(temperatureConversion(100) - 160.0) < 1e-9);
    
    // Test a value that produces a fractional result needing rounding.
    assert(std::abs(temperatureConversion(7) - 11.2) < 1e-9);
    assert(std::abs(temperatureConversion(-3) - (-4.8)) < 1e-9);
    
    // Test large integers.
    assert(std::abs(temperatureConversion(1000) - 1600.0) < 1e-9);
    assert(std::abs(temperatureConversion(-1000) - (-1600.0)) < 1e-9);
}
