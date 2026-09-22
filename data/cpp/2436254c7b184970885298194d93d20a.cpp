Write a C++ function that takes a vector of integers and returns a vector of indices (in ascending order) of all positions where the value is equal to either the largest or the second largest distinct value in the array. The array may contain duplicate values, and if there are fewer than two distinct values, then treat the largest value as both "largest" and "second largest" (i.e., output indices where the value equals the maximum). For example, for input `[5, 1, 5, 3]`, the largest distinct is 5, second largest distinct is 3, so output indices `{0, 2, 3}` (since 5 appears at index 0 and 2, and 3 at index 3). For input `[7, 7, 7]`, output indices `{0, 1, 2}` because only one distinct value exists, so both "largest" and "second largest" are 7. The function should handle negative numbers, large arrays, and duplicates correctly.
#include <cassert>
#include <vector>

int main() {
    // Basic case with duplicates.
    std::vector<int> v1 = {5, 1, 5, 3};
    assert(indicesOfLargestAndSecondLargest(v1) == std::vector<int>({0, 2, 3}));

    // All identical.
    std::vector<int> v2 = {7, 7, 7};
    assert(indicesOfLargestAndSecondLargest(v2) == std::vector<int>({0, 1, 2}));

    // Single element.
    std::vector<int> v3 = {42};
    assert(indicesOfLargestAndSecondLargest(v3) == std::vector<int>({0}));

    // Negative numbers and two distinct values.
    std::vector<int> v4 = {-3, -1, -3, -2, -1};
    // largest = -1, second largest = -2 → indices: -1 at 1,4; -2 at 3
    assert(indicesOfLargestAndSecondLargest(v4) == std::vector<int>({1, 3, 4}));

    // Empty input.
    std::vector<int> v5;
    assert(indicesOfLargestAndSecondLargest(v5).empty());

    // Large array with many duplicates.
    std::vector<int> v6 = {10, 9, 10, 8, 9, 10};
    // largest = 10, second = 9 → indices: 0,2,5 (10) and 1,4 (9)
    assert(indicesOfLargestAndSecondLargest(v6) == std::vector<int>({0, 1, 2, 4, 5}));

    // Only two distinct values, both appear many times.
    std::vector<int> v7 = {3, 1, 3, 1, 3};
    // largest = 3, second = 1 → indices: 0,2,4 (3) and 1,3 (1)
    assert(indicesOfLargestAndSecondLargest(v7) == std::vector<int>({0, 1, 2, 3, 4}));

    return 0;
}
#include <vector>
#include <algorithm>
#include <limits>

// Return indices of elements equal to the largest or second largest distinct value.
std::vector<int> indicesOfLargestAndSecondLargest(const std::vector<int>& arr) {
    std::vector<int> result;
    if (arr.empty()) return result;

    // Find the largest value.
    int largest = *std::max_element(arr.begin(), arr.end());

    // Find the second largest distinct value (strictly less than largest).
    int secondLargest = std::numeric_limits<int>::min();
    bool foundSecond = false;
    for (int x : arr) {
        if (x < largest && x > secondLargest) {
            secondLargest = x;
            foundSecond = true;
        }
    }
    if (!foundSecond) {
        secondLargest = largest; // Only one distinct value.
    }

    // Collect indices where value matches either target.
    for (int i = 0; i < static_cast<int>(arr.size()); ++i) {
        if (arr[i] == largest || arr[i] == secondLargest) {
            result.push_back(i);
        }
    }
    return result;
}
// The main algorithm first identifies the largest and second largest distinct values in the array. To do this without sorting the entire array (which would disrupt index order), we can scan the array twice: first find the maximum value, then find the maximum value that is strictly less than that maximum. If no such value exists (i.e., all elements are equal), then the second largest is set equal to the maximum. After determining these two target values, we iterate through the array once more and collect all indices where the element equals either target value, appending them in ascending index order (which naturally occurs by scanning from index 0 upward). Duplicate values cause multiple indices to be included. Edge cases include: an array with one element (that element is both largest and second largest), an array with all identical values (same as previous), and arrays with negative numbers. Time complexity is O(n) for the three linear passes (or two passes if we combine), which is optimal since we must examine every element. Space complexity is O(k) where k is the number of returned indices (worst case O(n)), plus O(1) auxiliary space for the two target values.
