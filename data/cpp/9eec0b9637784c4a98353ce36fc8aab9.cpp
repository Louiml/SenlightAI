// Write a C++ function named `replaceWithGreatestOnRight` that takes a non-empty vector of integers `arr` and returns a new vector of the same size where each element at index `i` is replaced by the greatest element among the elements to its right in the original array. The last element must be replaced with `-1`. The original vector must not be modified. The function should work efficiently for any size, including a single-element vector, and handle duplicate values and negative numbers correctly.
// The solution processes the input array from right to left, maintaining a running maximum of all elements seen so far to the right of the current index. Initialize a variable `maxSeen` to `-1` (since the rightmost element's replacement is always `-1`). Iterate from the second-to-last element down to the first. For each index `i`, update `maxSeen` to the maximum of its current value and `arr[i+1]`, then assign `maxSeen` to the result at index `i`. The last element of the result is directly set to `-1` without needing computation. Edge cases: if the array has one element, the result is simply `[-1]`; negative numbers are handled naturally by `std::max`. Time complexity is O(n) with a single pass, and space complexity is O(n) for the output vector, plus O(1) auxiliary space for the tracking variable.
#include <vector>
#include <algorithm>

// Replace each element with the greatest element to its right; last element becomes -1.
std::vector<int> replaceWithGreatestOnRight(const std::vector<int>& arr) {
    if (arr.empty()) {
        return {};
    }
    
    std::vector<int> result(arr.size());
    int maxSeen = -1;
    result[arr.size() - 1] = -1;
    
    for (int i = static_cast<int>(arr.size()) - 2; i >= 0; --i) {
        maxSeen = std::max(maxSeen, arr[i + 1]);
        result[i] = maxSeen;
    }
    
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Basic case
    std::vector<int> arr1 = {17, 18, 5, 4, 6, 1};
    std::vector<int> expected1 = {18, 6, 6, 6, 1, -1};
    assert(replaceWithGreatestOnRight(arr1) == expected1);

    // Single element
    std::vector<int> arr2 = {5};
    std::vector<int> expected2 = {-1};
    assert(replaceWithGreatestOnRight(arr2) == expected2);

    // Already sorted ascending
    std::vector<int> arr3 = {1, 2, 3, 4};
    std::vector<int> expected3 = {4, 4, 4, -1};
    assert(replaceWithGreatestOnRight(arr3) == expected3);

    // All equal elements
    std::vector<int> arr4 = {7, 7, 7};
    std::vector<int> expected4 = {7, 7, -1};
    assert(replaceWithGreatestOnRight(arr4) == expected4);

    // Negative numbers and duplicates
    std::vector<int> arr5 = {-5, -1, -3, -1};
    std::vector<int> expected5 = {-1, -1, -1, -1};
    assert(replaceWithGreatestOnRight(arr5) == expected5);

    // Strictly decreasing
    std::vector<int> arr6 = {9, 8, 7, 6};
    std::vector<int> expected6 = {8, 7, 6, -1};
    assert(replaceWithGreatestOnRight(arr6) == expected6);
    
    return 0;
}
