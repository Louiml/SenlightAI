/*
Write a C++ function named `minMaxPair` that accepts a `std::vector<int>` and returns a `std::pair<int,int>` where the first element is the smallest value in the vector and the second element is the largest value. The input vector will contain at least one element. The function must be `const`-correct (i.e., it should not modify the input vector), and you must handle edge cases such as a single-element vector, all-equal values, and negative numbers. You may assume the vector is not empty.
*/
#include <vector>
#include <utility>
#include <algorithm>

// Returns a pair containing the minimum and maximum values from the input vector.
// The input vector must be non-empty.
std::pair<int, int> minMaxPair(const std::vector<int>& values) {
    int minVal = values.front();
    int maxVal = values.front();

    // Iterate from the second element onward
    for (size_t i = 1; i < values.size(); ++i) {
        minVal = std::min(minVal, values[i]);
        maxVal = std::max(maxVal, values[i]);
    }

    return std::make_pair(minVal, maxVal);
}
#include <cassert>
#include <vector>
#include <utility>

// Forward declaration of the solution function
std::pair<int, int> minMaxPair(const std::vector<int>& values);

int main() {
    // Single element
    std::vector<int> v1 = {5};
    assert(minMaxPair(v1) == std::make_pair(5, 5));

    // All equal
    std::vector<int> v2 = {3, 3, 3, 3};
    assert(minMaxPair(v2) == std::make_pair(3, 3));

    // Negative and positive numbers
    std::vector<int> v3 = {-10, 2, -1, 7, 0};
    assert(minMaxPair(v3) == std::make_pair(-10, 7));

    // Already sorted
    std::vector<int> v4 = {1, 2, 3, 4, 5};
    assert(minMaxPair(v4) == std::make_pair(1, 5));

    // Reverse sorted
    std::vector<int> v5 = {9, 8, 7, 6};
    assert(minMaxPair(v5) == std::make_pair(6, 9));

    // Large vector with extremes at the end
    std::vector<int> v6 = {0, 1000, -500, 2};
    assert(minMaxPair(v6) == std::make_pair(-500, 1000));

    return 0;
}
// The solution iterates through the vector once, tracking the current minimum and maximum. Initialize both from the first element (using `front()`), then for each subsequent element, update the min and max using `std::min` and `std::max` or manual comparisons. Since the vector has at least one element, dereferencing `front()` is safe. The time complexity is \(O(n)\) where \(n\) is the size of the vector, and the space complexity is \(O(1)\) beyond the input vector and the returned pair. Edge cases: single-element vector (min == max), negative numbers (handled naturally by comparisons), and duplicates (no special handling needed since comparisons are not strict). The function should be marked `const` to reflect it does not modify the input, and it should be declared as `std::pair<int,int>`.
