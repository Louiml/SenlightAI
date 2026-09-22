Write a C++ function named `findMaximumAndIndex` that takes a `std::vector<int>` containing exactly 9 integers (simulating the BOJ 2562 problem) and returns a `std::pair<int, int>` where the first element is the maximum value among the 9 integers and the second element is the 1-based index of the first occurrence of that maximum value. The function must handle duplicate maximum values by returning the smallest index (i.e., the position of the first maximum encountered when scanning from the beginning). The input vector is guaranteed to have exactly 9 elements, each between 1 and 100 (inclusive), so no empty or invalid input cases exist. The function should be `const`-correct and not modify the input vector.
#include <cassert>
#include <vector>
#include <utility>

// Declare the solution function (assumed to be in the same translation unit or included)
std::pair<int, int> findMaximumAndIndex(const std::vector<int>& numbers);

int main() {
    // Basic case: unique maximum
    assert(findMaximumAndIndex({3, 29, 38, 12, 57, 74, 40, 85, 61}) == std::make_pair(85, 8));

    // Maximum at the first position
    assert(findMaximumAndIndex({100, 1, 2, 3, 4, 5, 6, 7, 8}) == std::make_pair(100, 1));

    // Maximum at the last position
    assert(findMaximumAndIndex({1, 2, 3, 4, 5, 6, 7, 8, 99}) == std::make_pair(99, 9));

    // All elements equal: first index is returned
    assert(findMaximumAndIndex({5, 5, 5, 5, 5, 5, 5, 5, 5}) == std::make_pair(5, 1));

    // Duplicate maximum: first occurrence index is returned
    assert(findMaximumAndIndex({10, 20, 20, 15, 20, 30, 30, 1, 2}) == std::make_pair(30, 6));

    // Minimum values (1) and maximum at start
    assert(findMaximumAndIndex({1, 1, 1, 1, 1, 1, 1, 1, 1}) == std::make_pair(1, 1));

    // Maximum appears twice consecutively at the beginning
    assert(findMaximumAndIndex({77, 77, 3, 4, 5, 6, 7, 8, 9}) == std::make_pair(77, 1));

    // Maximum appears twice with other values in between - first occurrence wins
    assert(findMaximumAndIndex({3, 3, 3, 50, 3, 50, 3, 3, 3}) == std::make_pair(50, 4));

    // Random distribution with negative-like values (though constraints say positive, still works)
    assert(findMaximumAndIndex({2, 8, 3, 9, 1, 7, 4, 6, 5}) == std::make_pair(9, 4));

    // Maximum at index 0 and duplicates later
    assert(findMaximumAndIndex({42, 1, 2, 3, 4, 5, 6, 7, 42}) == std::make_pair(42, 1));

    return 0;
}
#include <vector>
#include <utility>
#include <algorithm>

// Returns the maximum value and its 1-based first occurrence index from a vector of exactly 9 integers.
std::pair<int, int> findMaximumAndIndex(const std::vector<int>& numbers) {
    int maxValue = numbers[0];
    int maxIndex = 0; // 0-based index of the first maximum

    for (std::size_t i = 1; i < numbers.size(); ++i) {
        // Use strict '>' to keep the first occurrence when duplicates exist
        if (numbers[i] > maxValue) {
            maxValue = numbers[i];
            maxIndex = static_cast<int>(i);
        }
    }

    // Convert to 1-based index as required by the task
    return std::make_pair(maxValue, maxIndex + 1);
}
// The solution is straightforward: initialize both the maximum value and its index from the first element of the vector (index 0, which corresponds to 1-based index 1). Then iterate through the remaining elements (indices 1 through 8). For each element, compare it with the current maximum. Strictly use `>` (not `>=`) when updating the maximum so that if a duplicate of the current maximum is encountered later, the index is not changed, preserving the first occurrence. After the loop, return `std::make_pair(maxValue, maxIndex + 1)` because the internal index is 0-based but the task requires 1-based. Edge cases: if all elements are equal, the first element's index (1) is returned; if the maximum appears multiple times, the first occurrence's index is returned; the input size is fixed at 9, so no need to check length. Time complexity is O(9) = O(1) since the input size is constant, and space complexity is O(1) for the two tracking variables.
