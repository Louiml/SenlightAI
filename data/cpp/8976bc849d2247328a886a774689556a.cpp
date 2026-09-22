// Write a C++ function `vector<double> filterSmallValues(const vector<double>& values, double threshold)` that takes a vector of floating-point numbers and a threshold value, and returns a new vector containing only those elements whose value is less than or equal to the threshold, in their original order. The function must work for any vector size (including empty) and any threshold, and should not modify the input vector. Additionally, write a helper function `string formatVector(const vector<double>& vec)` that converts the filtered result into a string where each element is printed with exactly one digit after the decimal point, in the format `value, value, ...` (comma and space between values), and return that formatted string. The main goal is to produce a vector and its formatted string representation, given an input array of 100 real numbers (the input array is guaranteed to have exactly 100 elements). The threshold is fixed at `10.0`. You only need to implement the two functions; the caller will provide the input vector of 100 doubles.
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Test 1: basic filtering with some values above and below threshold.
    std::vector<double> input1 = {1.5, 12.3, 10.0, -2.7, 11.1, 9.9};
    std::vector<double> filtered1 = filterSmallValues(input1, 10.0);
    assert(filtered1.size() == 4);
    assert(filtered1[0] == 1.5);
    assert(filtered1[1] == 10.0);
    assert(filtered1[2] == -2.7);
    assert(filtered1[3] == 9.9);
    assert(formatVector(filtered1) == "1.5, 10.0, -2.7, 9.9");

    // Test 2: all values larger than threshold -> empty result.
    std::vector<double> input2 = {11.0, 20.5, 100.1};
    std::vector<double> filtered2 = filterSmallValues(input2, 10.0);
    assert(filtered2.empty());
    assert(formatVector(filtered2) == "");

    // Test 3: all values smaller or equal threshold -> same order.
    std::vector<double> input3 = {0.0, -5.5, 10.0, 3.3};
    std::vector<double> filtered3 = filterSmallValues(input3, 10.0);
    assert(filtered3 == input3);
    assert(formatVector(filtered3) == "0.0, -5.5, 10.0, 3.3");

    // Test 4: empty input.
    std::vector<double> input4;
    std::vector<double> filtered4 = filterSmallValues(input4, 10.0);
    assert(filtered4.empty());
    assert(formatVector(filtered4) == "");

    // Test 5: threshold is exactly 10.0 (inclusive) and value is exactly 10.0.
    std::vector<double> input5 = {10.0, 10.01, 9.99};
    std::vector<double> filtered5 = filterSmallValues(input5, 10.0);
    assert(filtered5.size() == 2);
    assert(filtered5[0] == 10.0);
    assert(filtered5[1] == 9.99);

    // Test 6: negative and fractional values formatting.
    std::vector<double> input6 = {-0.04, -1.999, 0.0, 7.76};
    std::vector<double> filtered6 = filterSmallValues(input6, 10.0);
    assert(filtered6.size() == 4);
    assert(formatVector(filtered6) == "-0.0, -2.0, 0.0, 7.8");

    // Test 7: large vector with 100 elements (simulate original problem).
    std::vector<double> input7(100, 5.0); // all 5.0
    input7[0] = 15.0; // one above threshold
    input7[99] = -3.0; // one below
    std::vector<double> filtered7 = filterSmallValues(input7, 10.0);
    assert(filtered7.size() == 99); // 100 minus the 15.0
    assert(filtered7[0] == 5.0);
    assert(filtered7[98] == -3.0);

    return 0;
}
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

// Returns a new vector with only elements <= threshold, preserving original order.
std::vector<double> filterSmallValues(const std::vector<double>& values, double threshold) {
    std::vector<double> result;
    for (double value : values) {
        if (value <= threshold) {
            result.push_back(value);
        }
    }
    return result;
}

// Converts a vector of doubles to a comma-space separated string with one decimal place.
std::string formatVector(const std::vector<double>& vec) {
    std::ostringstream oss;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i > 0) {
            oss << ", ";
        }
        oss << std::fixed << std::setprecision(1) << vec[i];
    }
    return oss.str();
}
// The solution approach is straightforward: iterate through the input vector once, and for each element, check if its value is less than or equal to the threshold. If so, append it to a new result vector. This preserves the original order because we scan from index 0 to size-1. For the formatting function, iterate through the filtered vector, convert each double to a string with one decimal place (using `ostringstream` with fixed and setprecision(1)), and join with comma-space separators. Edge cases: an empty input vector yields an empty filtered vector and an empty formatted string (no trailing comma). If no values pass the threshold, the filtered vector is empty and the formatted string is empty. For `const` correctness, the input vector is passed by `const vector<double>&`. Time complexity is O(n) for filtering and O(k) for formatting, where n is input size (100) and k is the number of filtered elements; total O(n). Space complexity is O(k) for the resulting vector and O(k) for the output string, plus O(1) auxiliary if not counting output. Since the input size is fixed at 100, this is effectively constant, but the general analysis holds.
