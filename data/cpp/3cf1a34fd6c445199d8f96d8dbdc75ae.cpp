// Write a C++ function `twoSumIndices` that takes a reference to a vector of integers sorted in non-decreasing order and a target sum, and returns a `std::pair<size_t, size_t>` containing the indices of the two numbers that add up exactly to the target. The function must assume that exactly one valid pair exists in the input. Handle edge cases such as duplicate values (where multiple pairs could exist, but you must return the first pair found while scanning from the outside inward) and ensure the function does not modify the input vector and uses constant extra space.
// The solution uses the classic two-pointer technique on the sorted array. Initialize two indices: `left = 0` and `right = size - 1`. Compute the sum of the elements at these indices. If the sum equals the target, return the pair. If the sum is greater than the target, decrement `right` because the array is sorted, so moving left would only increase the sum further. If the sum is less than the target, increment `left` because we need a larger sum. Repeat until the pointers meet. This works because exactly one solution is guaranteed, and the sorted order lets us narrow the search in linear time. Edge cases include an array with two elements only (the function must return indices 0 and 1) and arrays with negative numbers (the logic still holds). Time complexity is O(n) with n being the number of elements, and space complexity is O(1) auxiliary.
#include <vector>
#include <utility>

// Returns the indices of two numbers in the sorted vector that sum to target.
// Assumes the input is sorted in non-decreasing order and exactly one solution exists.
std::pair<size_t, size_t> twoSumIndices(const std::vector<int>& nums, int target) {
    size_t left = 0;
    size_t right = nums.size() - 1;
    while (left < right) {
        int sum = nums[left] + nums[right];
        if (sum == target) {
            return {left, right};
        } else if (sum < target) {
            ++left;
        } else {
            --right;
        }
    }
    // This point should never be reached because a solution is guaranteed.
    return {0, 0};
}
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Basic example from the prompt (corrected vector declaration)
    std::vector<int> v1 = {1, 2, 3, 4, 6};
    auto r1 = twoSumIndices(v1, 6);
    assert(r1.first == 1 && r1.second == 3); // 2 + 4 = 6

    // Two-element vector
    std::vector<int> v2 = {5, 9};
    auto r2 = twoSumIndices(v2, 14);
    assert(r2.first == 0 && r2.second == 1);

    // Duplicate values that also satisfy the target
    std::vector<int> v3 = {2, 2, 3, 4};
    auto r3 = twoSumIndices(v3, 4);
    assert(r3.first == 0 && r3.second == 1); // first pair found

    // Negative numbers
    std::vector<int> v4 = {-3, 0, 2, 5};
    auto r4 = twoSumIndices(v4, 2);
    assert(r4.first == 0 && r4.second == 3); // -3 + 5 = 2

    // Target achieved by the two largest elements
    std::vector<int> v5 = {1, 3, 7, 8};
    auto r5 = twoSumIndices(v5, 15);
    assert(r5.first == 2 && r5.second == 3); // 7 + 8 = 15

    // Larger vector
    std::vector<int> v6 = {1, 3, 4, 6, 8, 10, 12};
    auto r6 = twoSumIndices(v6, 14);
    assert(r6.first == 1 && r6.second == 6); // 3 + 12 = 14

    return 0;
}
