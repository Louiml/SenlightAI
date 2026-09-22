// Write a C++ function `temperatureAdjustment(double initialCelsius, double changeInFahrenheit)` that, given a starting temperature in Celsius and a change in Fahrenheit, returns the final temperature in Celsius after applying the change. The change represents how many Fahrenheit degrees to add to the starting temperature (it can be negative, zero, or positive). Use the exact conversion: 1 Celsius degree equals 9/5 Fahrenheit degrees, so adding `F` Fahrenheit degrees corresponds to adding `5*F/9` Celsius degrees. However, in the original snippet, the formula was `cel + abs((5 * fer) - 160) / 9.0` — this is a simplified version that works only when converting a Celsius value to Fahrenheit then adding `fer` Fahrenheit degrees and converting back. For the task, you must implement the mathematically correct conversion: `finalCelsius = initialCelsius + (changeInFahrenheit * 5.0 / 9.0)`. Round the result to two decimal places and return it as a `double`. Do not use absolute value — handle negative changes correctly. Ensure your function is `const`-correct and does not modify its inputs.

The core algorithm is straightforward: apply the linear conversion factor between Fahrenheit and Celsius. Since a change of 1°F equals `5/9` °C, we multiply the input change by `5.0/9.0` and add it to the initial Celsius value. The main edge case is that the change can be negative, zero, or positive — no special handling is needed because the linear formula naturally handles all cases. Another edge case is floating-point precision: we round the result to two decimal places using `std::round(value * 100.0) / 100.0` to match the expected output format of two decimals. For a constant-time operation, time complexity is O(1) and space complexity is O(1).

#include <cmath>

// Given a starting temperature in Celsius and a change in Fahrenheit,
// return the final temperature in Celsius rounded to two decimal places.
double temperatureAdjustment(double initialCelsius, double changeInFahrenheit) {
    const double factor = 5.0 / 9.0; // 1°F = 5/9 °C
    const double finalCelsius = initialCelsius + changeInFahrenheit * factor;
    // Round to two decimal places
    return std::round(finalCelsius * 100.0) / 100.0;
}

#include <cassert>
#include <cmath>

// The solution function is declared above, but for the test we include it here.
double temperatureAdjustment(double initialCelsius, double changeInFahrenheit) {
    const double factor = 5.0 / 9.0;
    const double finalCelsius = initialCelsius + changeInFahrenheit * factor;
    return std::round(finalCelsius * 100.0) / 100.0;
}

int main() {
    // Basic positive change: 0°C + 9°F change = 0 + 9*(5/9) = 5°C
    assert(std::fabs(temperatureAdjustment(0.0, 9.0) - 5.0) < 1e-9);

    // Negative change: 20°C + (-9°F) = 20 - 5 = 15°C
    assert(std::fabs(temperatureAdjustment(20.0, -9.0) - 15.0) < 1e-9);

    // Zero change
    assert(std::fabs(temperatureAdjustment(37.0, 0.0) - 37.0) < 1e-9);

    // Fractional change: 10°C + 1°F = 10 + 5/9 ≈ 10.5556 -> rounds to 10.56
    assert(std::fabs(temperatureAdjustment(10.0, 1.0) - 10.56) < 1e-9);

    // Larger change and rounding: 100°C + 18°F = 100 + 10 = 110°C exactly
    assert(std::fabs(temperatureAdjustment(100.0, 18.0) - 110.0) < 1e-9);
}
