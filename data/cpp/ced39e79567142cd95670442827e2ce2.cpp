Write a C++ function that takes a non-empty vector of positive integers and returns the maximum possible value of the last element after repeatedly applying the following operations in any order: (1) decrement any element by 1 as many times as desired (but not below 1), and (2) rearrange the elements arbitrarily. The goal is to make the final array satisfy the condition that the absolute difference between every pair of adjacent elements is at most 1, starting with the first element equal to 1. Return the largest possible value that the maximum element can achieve under these constraints.
// The key insight is that to maximize the final maximum element, we want to sort the array in ascending order and then greedily adjust the first element to 1 (which is the minimum possible starting value). After sorting, we iterate from left to right. For each pair `(arr[i], arr[i+1])`, if the gap between them is greater than 1, we can only raise `arr[i+1]` by reducing it to `arr[i] + 1` (because we cannot increase elements, only decrease them, and we want the next value as high as possible while satisfying the adjacent difference constraint). We never need to change `arr[i]` because it already satisfies the constraint with its predecessor. After processing all pairs, the last element holds the maximum possible value. Important edge cases: a single element array—set it to 1; duplicates—no adjustment needed because difference is 0; already valid arrays—no changes except ensuring first element is 1 (if it was larger, we reduce it to 1, which may cascade adjustments). Time complexity is O(n log n) due to sorting, and space complexity is O(1) auxiliary (sorting may use O(log n) stack space in typical implementations).
#include <vector>
#include <algorithm>
#include <cstdlib>

// Given a vector of positive integers, return the maximum possible value
// of the largest element after rearranging and decrementing (but not below 1)
// so that the first element is 1 and every adjacent pair differs by at most 1.
int maximumElementAfterDecrementingAndRearranging(std::vector<int>& arr) {
    if (arr.empty()) return 0;
    std::sort(arr.begin(), arr.end());
    arr[0] = 1; // minimal possible first element
    for (size_t i = 0; i + 1 < arr.size(); ++i) {
        if (std::abs(arr[i] - arr[i + 1]) > 1) {
            arr[i + 1] = arr[i] + 1;
        }
    }
    return arr.back();
}
#include <cassert>
#include <vector>

int main() {
    std::vector<int> v1 = {2, 2, 1};
    assert(maximumElementAfterDecrementingAndRearranging(v1) == 2);

    std::vector<int> v2 = {1, 2, 3, 4, 5};
    assert(maximumElementAfterDecrementingAndRearranging(v2) == 5);

    std::vector<int> v3 = {100, 1, 1000};
    assert(maximumElementAfterDecrementingAndRearranging(v3) == 2);

    std::vector<int> v4 = {1};
    assert(maximumElementAfterDecrementingAndRearranging(v4) == 1);

    std::vector<int> v5 = {1, 1, 1};
    assert(maximumElementAfterDecrementingAndRearranging(v5) == 1);

    std::vector<int> v6 = {3, 3, 3, 3};
    assert(maximumElementAfterDecrementingAndRearranging(v6) == 2);

    std::vector<int> v7 = {5, 4, 3, 2, 1};
    assert(maximumElementAfterDecrementingAndRearranging(v7) == 5);

    std::vector<int> v8 = {10, 10, 10, 1};
    assert(maximumElementAfterDecrementingAndRearranging(v8) == 3);

    return 0;
}
