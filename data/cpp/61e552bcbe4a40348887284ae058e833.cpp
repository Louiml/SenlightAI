// Write a C++ function `positiveAverage` that reads exactly 6 floating-point numbers from a standard input stream (e.g., an `istream` object passed as a parameter) and returns a `std::pair<int, double>` where the first element is the count of positive values among the 6 inputs, and the second element is the arithmetic mean of those positive values, formatted to one decimal place. If no positive values are present, the function should return `{0, 0.0}`. The function must not read from global `cin`; instead it should accept an `std::istream&` parameter so it can be tested with `std::istringstream`. The output should be rounded to exactly one decimal place using standard rounding rules (e.g., 2.25 becomes "2.3", not "2.2").
#include <sstream>
#include <cassert>
#include <cmath>

// Declaration of the function under test (assumed to be in the same translation unit)
std::pair<int, double> positiveAverage(std::istream& in);

int main() {
    // Test 1: Basic case with mixed positives and negatives
    std::istringstream s1("7 -5 0 6 -1 2");
    auto r1 = positiveAverage(s1);
    assert(r1.first == 3);
    assert(std::abs(r1.second - 5.0) < 1e-9);

    // Test 2: All positive
    std::istringstream s2("1.0 2.0 3.0 4.0 5.0 6.0");
    auto r2 = positiveAverage(s2);
    assert(r2.first == 6);
    assert(std::abs(r2.second - 3.5) < 1e-9);

    // Test 3: No positives
    std::istringstream s3("-1 -2 -3 -4 -5 -6");
    auto r3 = positiveAverage(s3);
    assert(r3.first == 0);
    assert(r3.second == 0.0);

    // Test 4: Rounding to one decimal (2.25 → 2.3)
    std::istringstream s4("2.25 -1 2.25 -1 2.25 -1");
    auto r4 = positiveAverage(s4);
    assert(r4.first == 3);
    assert(std::abs(r4.second - 2.3) < 1e-9);

    // Test 5: Zero values are not positive
    std::istringstream s5("0 0 0 1 2 3");
    auto r5 = positiveAverage(s5);
    assert(r5.first == 3);
    assert(std::abs(r5.second - 2.0) < 1e-9);

    // Test 6: Negative and zero only
    std::istringstream s6("0 -0.0 -1 -2 -3 -4");
    auto r6 = positiveAverage(s6);
    assert(r6.first == 0);
    assert(r6.second == 0.0);

    // Test 7: Exact decimal that doesn't need rounding
    std::istringstream s7("1.5 -1 -1 1.5 -1 -1");
    auto r7 = positiveAverage(s7);
    assert(r7.first == 2);
    assert(std::abs(r7.second - 1.5) < 1e-9);

    // Test 8: Mixed with very small values
    std::istringstream s8("0.1 0.2 0.3 -1 -2 -3");
    auto r8 = positiveAverage(s8);
    assert(r8.first == 3);
    assert(std::abs(r8.second - 0.2) < 1e-9);
}
#include <istream>
#include <cmath>
#include <utility>

// Reads exactly 6 doubles from the given stream.
// Returns {count_of_positives, mean_of_positives_rounded_to_1_decimal}.
// If no positive values exist, returns {0, 0.0}.
std::pair<int, double> positiveAverage(std::istream& in) {
    double sum = 0.0;
    int count = 0;

    for (int i = 0; i < 6; ++i) {
        double x;
        in >> x;
        if (x > 0.0) {
            sum += x;
            ++count;
        }
    }

    if (count == 0) {
        return {0, 0.0};
    }

    double mean = sum / count;
    // Round to 1 decimal place using standard rounding (round half away from zero)
    double rounded = std::round(mean * 10.0) / 10.0;
    // Correct potential -0.0 result
    if (rounded == 0.0) rounded = 0.0;
    return {count, rounded};
}
// The approach is straightforward: loop exactly 6 times, reading one double from the provided stream each iteration. For each value, check if it is strictly greater than 0. If so, accumulate it into a running sum and increment a counter. After the loop, if the counter is zero, return `{0, 0.0}` to avoid division by zero. Otherwise, compute the mean as `sum / count`. The return type is a `std::pair<int, double>` — the integer count must be exact, but the double mean must be rounded to one decimal place. C++’s default `double` precision is insufficient for direct rounding, so use `std::round(value * 10.0) / 10.0` or format via a string stream with `std::fixed` and `std::setprecision(1)` and then parse back to double. Edge cases: all values are non-positive (returns zero count and 0.0), negative and zero values are ignored, and the input always contains exactly 6 valid numbers. Time complexity is O(1) (fixed loop), space complexity is O(1) auxiliary.
