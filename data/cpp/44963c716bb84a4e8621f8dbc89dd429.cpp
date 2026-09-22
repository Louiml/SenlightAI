/*
Given a vector of integers that is guaranteed to contain at least one element, write a C++ function that returns a `pair<int, int>` where the first element is the smallest value and the second is the largest value. The function must handle positive, negative, and duplicate values correctly. The solution should avoid modifying the input vector and must be implemented without using any standard library algorithms such as `min_element` or `max_element`. The function should be named `findMinMax` and must accept a `const std::vector<int>&` as its only parameter. The return type must be `std::pair<int, int>`. The function must be entirely self-contained, with all necessary headers included.
*/

#include <utility>
#include <vector>

// Return the smallest and largest values from a non-empty vector of integers.
// The input vector is not modified.
std::pair<int, int> findMinMax(const std::vector<int>& values) {
    // The vector is guaranteed to be non-empty, so we safely access the first element.
    int minValue = values[0];
    int maxValue = values[0];

    // Iterate over the remaining elements.
    for (size_t i = 1; i < values.size(); ++i) {
        // Update the minimum if a smaller value is found.
        if (values[i] < minValue) {
            minValue = values[i];
        }
        // Update the maximum if a larger value is found.
        if (values[i] > maxValue) {
            maxValue = values[i];
        }
    }

    // Return the pair (min, max).
    return {minValue, maxValue};
}

#include <cassert>
#include <utility>
#include <vector>

// Solution function declaration (usually placed in a header, but here inline for testing).
std::pair<int, int> findMinMax(const std::vector<int>& values);

int main() {
    // Test with a typical mix of positive and negative numbers.
    std::vector<int> v1 = {3, -1, 7, 0, -5, 2};
    assert(findMinMax(v1) == std::make_pair(-5, 7));

    // Test with a single element; both min and max are that element.
    std::vector<int> v2 = {42};
    assert(findMinMax(v2) == std::make_pair(42, 42));

    // Test with all duplicate values.
    std::vector<int> v3 = {-3, -3, -3};
    assert(findMinMax(v3) == std::make_pair(-3, -3));

    // Test with extreme values including zero and negatives.
    std::vector<int> v4 = {-1000000, 1000000, 0};
    assert(findMinMax(v4) == std::make_pair(-1000000, 1000000));

    // Test with a large vector to ensure no overflow or performance issues.
    std::vector<int> v5 = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1, -2};
    assert(findMinMax(v5) == std::make_pair(-2, 9));

    // Test where the minimum appears at the end.
    std::vector<int> v6 = {1, 2, 3, -100};
    assert(findMinMax(v6) == std::make_pair(-100, 3));

    // Test where the maximum appears at the beginning.
    std::vector<int> v7 = {100, -50, 0, 25};
    assert(findMinMax(v7) == std::make_pair(-50, 100));

    // Test with all positive numbers.
    std::vector<int> v8 = {5, 1, 3, 2, 4};
    assert(findMinMax(v8) == std::make_pair(1, 5));

    // Test with all negative numbers.
    std::vector<int> v9 = {-10, -2, -7, -5};
    assert(findMinMax(v9) == std::make_pair(-10, -2));

    // Test with two elements where the min and max are swapped.
    std::vector<int> v10 = {5, -5};
    assert(findMinMax(v10) == std::make_pair(-5, 5));

    return 0;
}

// The problem asks for the minimum and maximum values in a non-empty vector of integers. The straightforward approach is to iterate through the vector once, tracking both values. Initialize `minValue` and `maxValue` from the first element, since the vector is guaranteed to be non-empty. Then, for each subsequent element, update `minValue` if the current element is smaller, and update `maxValue` if the current element is larger. Because the vector might contain negative numbers, duplicates, or only one element, we must not assume positive values or uniqueness. After the loop, return a `std::pair` with these two values. The algorithm runs in exactly one pass over the vector, giving an O(n) time complexity. The additional memory used is only two integers plus the pair itself, so the space complexity is O(1). Edge cases include a vector of size one (both min and max are that single element), a vector with all equal values, and vectors containing both positive and negative extremes.
