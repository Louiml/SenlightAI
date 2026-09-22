/*
Write a C++ function that, given a `std::vector<int>` (which is guaranteed to be non-empty), returns a `std::pair<int, int>` where the first element is the smallest value in the vector and the second element is the largest value in the vector. The function must not modify the input vector, must work correctly with negative numbers, duplicates, and a single-element vector, and must not rely on any standard library min/max algorithms (i.e., implement the comparison manually using loops). The solution should be robust, efficient, and suitable for large inputs.
*/
#include <vector>
#include <utility>

// Return the minimum and maximum elements of a non-empty vector as a pair.
std::pair<int, int> findMinMax(const std::vector<int>& data) {
    int minimum = data[0];
    int maximum = data[0];

    for (std::size_t i = 1; i < data.size(); ++i) {
        if (data[i] < minimum) {
            minimum = data[i];
        }
        if (data[i] > maximum) {
            maximum = data[i];
        }
    }

    return std::make_pair(minimum, maximum);
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function (assumed to be defined above)
std::pair<int, int> findMinMax(const std::vector<int>& data);

int main() {
    // Basic case
    std::vector<int> v1 = {5, 2, 9, 1, 7};
    assert(findMinMax(v1) == std::make_pair(1, 9));

    // Single element
    std::vector<int> v2 = {42};
    assert(findMinMax(v2) == std::make_pair(42, 42));

    // All duplicates
    std::vector<int> v3 = {7, 7, 7, 7};
    assert(findMinMax(v3) == std::make_pair(7, 7));

    // Negative numbers
    std::vector<int> v4 = {-3, -8, -1, -10};
    assert(findMinMax(v4) == std::make_pair(-10, -1));

    // Mixed positive and negative
    std::vector<int> v5 = {-5, 0, 5, -2, 2};
    assert(findMinMax(v5) == std::make_pair(-5, 5));

    // Already sorted ascending
    std::vector<int> v6 = {1, 2, 3, 4};
    assert(findMinMax(v6) == std::make_pair(1, 4));

    // Already sorted descending
    std::vector<int> v7 = {10, 8, 6, 4, 2};
    assert(findMinMax(v7) == std::make_pair(2, 10));

    return 0;
}
// The solution involves iterating through the vector exactly once, tracking the current minimum and maximum values. Initialize both `minimum` and `maximum` to the first element of the vector (this handles the case where the vector has exactly one element). Then, for each subsequent element, compare it with the current `minimum` and `maximum` and update them accordingly. Edge cases include: a vector with only one element (both min and max are the same), all elements equal (no updates but correct result), negative numbers (comparison works naturally with integers), and very large vectors (linear time with constant extra space). Since the function takes a `const std::vector<int>&`, no copying occurs, and the original data is preserved. Time complexity is O(n) where n is the number of elements; space complexity is O(1) auxiliary space (besides the input vector storage and the pair itself).
