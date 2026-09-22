/*
Given a non-empty vector of integers, write a C++ function that returns a new vector containing the running sum of the original vector. The running sum at index `i` is defined as the sum of all elements from index 0 through index `i` (inclusive) of the original vector. The function must not modify the input vector, must handle vectors of any length from 1 upward, and must work correctly with negative numbers and zeros. Return the resulting vector by value.
*/

#include <vector>

// Compute the running sum of the input vector.
// The returned vector's i-th element equals the sum of nums[0..i].
std::vector<int> computeRunningSum(const std::vector<int>& nums) {
    std::vector<int> result(nums);
    for (std::size_t i = 1; i < nums.size(); ++i) {
        result[i] += result[i - 1];
    }
    return result;
}

#include <cassert>
#include <vector>

// Function under test (declared here for completeness; in practice include the header)
std::vector<int> computeRunningSum(const std::vector<int>& nums);

int main() {
    // Basic case
    std::vector<int> nums1 = {1, 2, 3, 4};
    std::vector<int> expected1 = {1, 3, 6, 10};
    assert(computeRunningSum(nums1) == expected1);

    // Single element
    std::vector<int> nums2 = {7};
    assert(computeRunningSum(nums2) == std::vector<int>({7}));

    // All negative numbers
    std::vector<int> nums3 = {-1, -2, -3};
    assert(computeRunningSum(nums3) == std::vector<int>({-1, -3, -6}));

    // Mixed positive, negative, zero
    std::vector<int> nums4 = {3, -1, 0, 5};
    assert(computeRunningSum(nums4) == std::vector<int>({3, 2, 2, 7}));

    // Larger vector with duplicates
    std::vector<int> nums5 = {2, 2, 2, 2, 2};
    assert(computeRunningSum(nums5) == std::vector<int>({2, 4, 6, 8, 10}));

    // Input not modified
    std::vector<int> original = {1, 2, 3};
    computeRunningSum(original);
    assert(original == std::vector<int>({1, 2, 3}));

    return 0;
}

// The solution is straightforward: create a result vector that is a copy of the input vector, then iterate from the second element (index 1) to the end, adding the previous accumulated value to the current element. This works because after processing index `i-1`, `res[i-1]` already holds the sum of elements from 0 to `i-1`. Adding the original `nums[i]` (which is preserved in `res[i]` before modification) yields the running sum up to `i`. Edge cases: an empty vector is not allowed per the task specification, but if it were, the loop simply wouldn't execute and an empty vector would be returned; a single-element vector requires no loop and returns the element itself. Negative numbers and zeros are handled naturally by addition. Time complexity is O(n) where n is the number of elements, and space complexity is O(n) for the returned vector (the input is not modified, and no extra auxiliary space beyond the result is used).
