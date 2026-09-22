/*
Write a C++ function that takes a vector of double-precision floating-point numbers and returns a new vector containing only those elements whose value is less than or equal to 10.0. The function must preserve the original order of the qualifying elements and should not modify the input vector. The input vector may be empty, may contain values exactly equal to 10.0, negative values, and values with many decimal places. The function should be declared with `const` correctness for the parameter and return a `std::vector<double>` by value.
*/
#include <vector>

// Return a new vector containing only elements from `input` that are <= 10.0.
std::vector<double> filterValuesAtMostTen(const std::vector<double>& input) {
    std::vector<double> result;
    result.reserve(input.size()); // optional optimization to avoid reallocations
    for (const double value : input) {
        if (value <= 10.0) {
            result.push_back(value);
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// Function under test (declared here for clarity; in a single-file test, include the solution above).
std::vector<double> filterValuesAtMostTen(const std::vector<double>& input);

int main() {
    // Basic filtering with mixed values
    std::vector<double> v1 = {1.5, 10.0, 20.0, -0.5, 10.0001};
    std::vector<double> r1 = filterValuesAtMostTen(v1);
    assert((r1 == std::vector<double>{1.5, 10.0, -0.5}));

    // Empty input
    std::vector<double> v2;
    assert(filterValuesAtMostTen(v2).empty());

    // All values greater than 10
    std::vector<double> v3 = {10.1, 11.0, 100.0};
    assert(filterValuesAtMostTen(v3).empty());

    // All values less than or equal to 10 (including exact 10)
    std::vector<double> v4 = {10.0, 0, -3.25, 9.999};
    std::vector<double> r4 = filterValuesAtMostTen(v4);
    assert(r4.size() == 4);
    assert((r4 == v4)); // unchanged and same order

    // Original vector remains unmodified
    std::vector<double> v5 = {5.0, 15.0};
    filterValuesAtMostTen(v5);
    assert((v5 == std::vector<double>{5.0, 15.0}));
}
// The solution iterates through the input vector using a range-based for loop or an index loop, checking each element if it satisfies `value <= 10.0`. If true, the element is appended to a result vector using `push_back` or `emplace_back`. Edge cases include an empty input (returns empty result), values exactly equal to 10.0 (should be included since condition is ≤), and all values greater than 10.0 (returns empty). No special handling for floating-point precision is needed because the comparison is straightforward. The time complexity is O(n) where n is the number of elements in the input vector, as each element is visited exactly once. The space complexity is O(m) where m is the number of qualifying elements (the size of the returned vector), plus O(1) auxiliary space for the loop variable and the result vector's internal storage.
