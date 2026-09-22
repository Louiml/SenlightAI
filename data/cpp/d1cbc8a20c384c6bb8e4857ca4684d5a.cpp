// Write a C++ function named `findMinMaxPositions` that takes a `std::vector<int>` as input (by const reference) and returns a `std::pair<int, int>` where the first element is the index of the smallest value and the second element is the index of the largest value. If multiple occurrences of the smallest or largest value exist, return the index of the first occurrence for each. The input vector is guaranteed to be non-empty. The function should handle both positive and negative integers, and duplicate values are allowed. The result should be based on the original 0-based indices.
// The solution initializes both the minimum and maximum values and their corresponding indices using the first element of the vector (index 0). Then it iterates through the remaining elements once. For each element, if it is strictly less than the current minimum, update the minimum value and its index. Similarly, if it is strictly greater than the current maximum, update the maximum value and its index. Using strict comparisons (`<` and `>`) ensures that the first occurrence is kept when duplicates exist, because later equal values do not overwrite the earlier index. The algorithm runs in a single pass, so the time complexity is O(n) where n is the size of the vector, and the auxiliary space complexity is O(1) because only a constant number of variables are used. Edge cases include a single-element vector (both min and max are that element at index 0) and vectors with all equal elements (both indices remain 0). The function should be const-correct by accepting the vector by const reference to avoid copying.
#include <vector>
#include <utility>
#include <cstddef>

// Returns a pair of indices: first is the index of the smallest value,
// second is the index of the largest value. For duplicates, returns the
// first occurrence of each extreme.
std::pair<std::size_t, std::size_t> findMinMaxPositions(const std::vector<int>& values) {
    std::size_t min_index = 0;
    std::size_t max_index = 0;
    int min_value = values[0];
    int max_value = values[0];

    for (std::size_t i = 1; i < values.size(); ++i) {
        if (values[i] < min_value) {
            min_value = values[i];
            min_index = i;
        }
        if (values[i] > max_value) {
            max_value = values[i];
            max_index = i;
        }
    }

    return {min_index, max_index};
}
#include <cassert>
#include <vector>
#include <utility>

// Function declaration from the solution
std::pair<std::size_t, std::size_t> findMinMaxPositions(const std::vector<int>& values);

int main() {
    // Basic case with distinct values
    std::vector<int> v1 = {3, 1, 4, 1, 5, 9, 2, 6};
    auto result1 = findMinMaxPositions(v1);
    assert(result1.first == 1 && result1.second == 5);

    // Single element
    std::vector<int> v2 = {42};
    auto result2 = findMinMaxPositions(v2);
    assert(result2.first == 0 && result2.second == 0);

    // All equal values
    std::vector<int> v3 = {7, 7, 7, 7};
    auto result3 = findMinMaxPositions(v3);
    assert(result3.first == 0 && result3.second == 0);

    // Duplicates with extremes occurring multiple times
    std::vector<int> v4 = {-5, 10, -5, 10, 0};
    auto result4 = findMinMaxPositions(v4);
    assert(result4.first == 0 && result4.second == 1);

    // Negative numbers and large vector
    std::vector<int> v5 = {-100, 0, -200, 50, -200, 100, -100};
    auto result5 = findMinMaxPositions(v5);
    assert(result5.first == 2 && result5.second == 5);

    // Strictly increasing
    std::vector<int> v6 = {1, 2, 3, 4, 5};
    auto result6 = findMinMaxPositions(v6);
    assert(result6.first == 0 && result6.second == 4);

    // Strictly decreasing
    std::vector<int> v7 = {9, 8, 7, 6};
    auto result7 = findMinMaxPositions(v7);
    assert(result7.first == 3 && result7.second == 0);

    // Extremes at ends
    std::vector<int> v8 = {-1, 0, 0, 1};
    auto result8 = findMinMaxPositions(v8);
    assert(result8.first == 0 && result8.second == 3);

    // Duplicate max first occurrence at end
    std::vector<int> v9 = {2, 2, 2, 9, 9};
    auto result9 = findMinMaxPositions(v9);
    assert(result9.first == 0 && result9.second == 3);

    // Mixed with zeros
    std::vector<int> v10 = {0, -1, 0, 1, 0};
    auto result10 = findMinMaxPositions(v10);
    assert(result10.first == 1 && result10.second == 3);

    return 0;
}
