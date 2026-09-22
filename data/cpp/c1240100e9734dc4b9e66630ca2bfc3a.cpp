/*
Write a C++ function named `canBeatRecord` that takes four `double` values representing the dimensions of a room (length, width, height) and a speed `s` (in meters per second), and returns a `bool` indicating whether a person can traverse the room faster than the current world record of 9.58 seconds. The person must travel a path defined as the product of all three room dimensions (length × width × height), and the time taken is calculated as `100 / (length * width * height * speed)`, rounded to exactly two decimal places (using a custom rounding function that rounds half up, not banker's rounding). If this rounded time is strictly less than 9.58, the function returns `true`; otherwise `false`. The function must not read from or write to any standard input/output, and must work correctly for positive, negative, and zero values, including edge cases where the denominator becomes zero (in which case the result should be `false` unless division by zero is undefined, but for this task treat any non-positive denominator as producing a result that is not less than 9.58).
*/

#include <cmath>

/**
 * Rounds a double to two decimal places using half-up rounding.
 * For example: 3.14159 -> 3.14, 3.145 -> 3.15, -3.145 -> -3.15.
 */
double roundToTwoDecimals(double value) {
    return std::floor(value * 100.0 + 0.5) / 100.0;
}

/**
 * Determines if the calculated time to traverse the room beats the 9.58 seconds record.
 * The time is 100 / (length * width * height * speed), rounded to 2 decimals.
 * Returns false for non-positive denominators (including zero) or if time >= 9.58.
 */
bool canBeatRecord(double length, double width, double height, double speed) {
    const double denominator = length * width * height * speed;
    if (denominator <= 0.0) {
        return false;
    }
    const double rawTime = 100.0 / denominator;
    const double roundedTime = roundToTwoDecimals(rawTime);
    return roundedTime < 9.58;
}

#include <cassert>

int main() {
    // Custom rounding tests
    assert(roundToTwoDecimals(9.5799) == 9.58);
    assert(roundToTwoDecimals(9.575) == 9.58);  // half-up rounding
    assert(roundToTwoDecimals(9.5749) == 9.57);
    assert(roundToTwoDecimals(-9.575) == -9.57); // half-up on negative moves toward +infinity

    // canBeatRecord tests
    // Denominator = 2*2*2*1 = 8, rawTime = 12.5, rounded = 12.5 -> false
    assert(canBeatRecord(2.0, 2.0, 2.0, 1.0) == false);
    // Denominator = 1*1*1*10 = 10, rawTime = 10.0, rounded = 10.0 -> false
    assert(canBeatRecord(1.0, 1.0, 1.0, 10.0) == false);
    // Denominator = 1*1*1*11 = 11, rawTime = 9.0909..., rounded = 9.09 -> true
    assert(canBeatRecord(1.0, 1.0, 1.0, 11.0) == true);
    // Denominator = 2*2*2*1.2 = 9.6, rawTime = 10.4167, rounded = 10.42 -> false
    assert(canBeatRecord(2.0, 2.0, 2.0, 1.2) == false);
    // Denominator = 2*2*2*1.3 = 10.4, rawTime = 9.61538, rounded = 9.62 -> false
    assert(canBeatRecord(2.0, 2.0, 2.0, 1.3) == false);
    // Denominator = 2*2*2*1.5 = 12, rawTime = 8.3333, rounded = 8.33 -> true
    assert(canBeatRecord(2.0, 2.0, 2.0, 1.5) == true);
    // Zero denominator -> false
    assert(canBeatRecord(0.0, 2.0, 2.0, 1.0) == false);
    // Negative denominator (negative dimension) -> false
    assert(canBeatRecord(-1.0, 2.0, 2.0, 1.0) == false);
    // Very small denominator -> rawTime huge, rounded huge -> false
    assert(canBeatRecord(0.0001, 0.0001, 0.0001, 0.0001) == false);
    // Exact boundary: rawTime = 9.575 -> rounds to 9.58, not < 9.58 -> false
    assert(canBeatRecord(1.0, 1.0, 1.0, 100.0 / 9.575) == false);
    return 0;
}

// The core algorithm is straightforward: compute the denominator as `length * width * height * speed`. If the denominator is zero or negative (since division by zero is undefined and negative denominators produce negative times, which are unrealistic), the answer is `false`. Otherwise, compute `rawTime = 100.0 / denominator`. Then round this value to two decimal places using the custom half-up rounding: multiply by 100, add 0.5, cast to `long long` (truncating toward zero), then divide by 100. Important edge cases: when the denominator is extremely small, `rawTime` may overflow to infinity; in that case, the rounded value will also be infinite, so the comparison fails (since infinity is not less than 9.58). Also, when all dimensions and speed are positive, the denominator is positive, and the result depends on whether the denominator is large enough (i.e., the product must be greater than `100 / 9.58 ≈ 10.4384` to beat the record). The time complexity is O(1) and space complexity is O(1).
