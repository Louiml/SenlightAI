/*
Write a C++ function that accepts a non-empty array of real numbers (as a `std::vector<double>`) and returns a new vector containing only those numbers that are less than or equal to 10, preserving the original order and each value with exactly one digit after the decimal point. The function must be const-correct (i.e., it should not modify the input), handle negative values and zeros, and return an empty vector if no elements satisfy the condition. For example, given `{1.5, 11.0, -3.25, 10.0, 20.0}`, the result should be `{1.5, -3.2, 10.0}` (note rounding to one decimal). The function must be named `filterSmallValues` and take a `const std::vector<double>&` argument.
*/
#include <vector>
#include <cmath>

// Return a vector containing only values <= 10, rounded to one decimal place.
std::vector<double> filterSmallValues(const std::vector<double>& input) {
    std::vector<double> result;
    for (double value : input) {
        if (value <= 10.0) {
            // Round to one decimal place.
            double rounded = std::round(value * 10.0) / 10.0;
            result.push_back(rounded);
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Basic case with mix of values.
    std::vector<double> input1 = {1.5, 11.0, -3.25, 10.0, 20.0};
    std::vector<double> expected1 = {1.5, -3.2, 10.0};
    std::vector<double> result1 = filterSmallValues(input1);
    assert(result1.size() == expected1.size());
    for (size_t i = 0; i < expected1.size(); ++i) {
        assert(std::fabs(result1[i] - expected1[i]) < 1e-6);
    }

    // All values are <= 10.
    std::vector<double> input2 = {0.0, -5.0, 9.99, 10.0};
    std::vector<double> result2 = filterSmallValues(input2);
    assert(result2.size() == 4);
    assert(result2[0] == 0.0);
    assert(result2[1] == -5.0);
    assert(std::fabs(result2[2] - 10.0) < 1e-6); // 9.99 rounds to 10.0
    assert(result2[3] == 10.0);

    // No values satisfy condition.
    std::vector<double> input3 = {10.1, 100.0, -0.0};
    std::vector<double> result3 = filterSmallValues(input3);
    assert(result3.empty());

    // Negative zero and rounding edge.
    std::vector<double> input4 = {-0.04, 5.05, 7.99};
    std::vector<double> result4 = filterSmallValues(input4);
    assert(result4.size() == 3);
    assert(std::fabs(result4[0] - (-0.0)) < 1e-6);
    assert(std::fabs(result4[1] - 5.1) < 1e-6); // 5.05 rounds to 5.1 (banker's rounding may vary, but we use standard round)
    assert(std::fabs(result4[2] - 8.0) < 1e-6);

    // Single element exactly 10.
    std::vector<double> input5 = {10.0};
    std::vector<double> result5 = filterSmallValues(input5);
    assert(result5.size() == 1);
    assert(result5[0] == 10.0);
}
// The solution iterates through the input vector and, for each element, checks if it is less than or equal to 10.0 using a simple comparison (`value <= 10.0`). If true, the value is appended to a result vector. To satisfy the one‑decimal‑place requirement, the original precision is not preserved; instead, we must ensure the output values are rounded to one decimal place. This can be done by applying `std::round(value * 10.0) / 10.0` to each qualifying element before storing it in the result. This rounding is essential because floating‑point numbers like `-3.25` would otherwise remain as‑is; the task requires exactly one digit after the decimal point in the output, which is achieved by the rounding step. Edge cases include: (1) no qualifying elements → return an empty vector; (2) values exactly equal to 10 → included; (3) negative values → included if ≤ 10; (4) values like `-0.0` → rounding yields `-0.0` which is acceptable. Time complexity is O(n) where n is the number of elements, because we process each element once. Space complexity is O(m) where m is the number of qualifying elements (worst case O(n)) for the result vector; no other significant extra storage is used.
