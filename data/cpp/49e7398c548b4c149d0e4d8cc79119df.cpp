/*
Write a C++ function `int removeDuplicatesAtMostTwo(std::vector<int>& nums)` that takes a non-empty vector of integers sorted in non-decreasing order and modifies it in-place so that each unique value appears at most twice, keeping the relative order unchanged. The function must place the resulting valid elements in the first portion of the vector (indices `0` through `k-1`) and return the new length `k`. The values beyond index `k` in the vector are irrelevant and can be left as-is. The function must run in O(1) extra space (excluding the input vector itself) and must not rely on any additional data structures. For example, given `nums = [1,1,1,2,2,3]`, the function should return `5` and leave `nums` with the first five elements being `[1,1,2,2,3]` (the sixth element can be anything). This problem is inspired by the classic "Remove Duplicates from Sorted Array II" from LeetCode.
*/
#include <vector>

// Removes duplicates in-place so that each element appears at most twice.
// Returns the new length of the valid prefix. O(1) extra space.
int removeDuplicatesAtMostTwo(std::vector<int>& nums) {
    if (nums.size() <= 2) {
        return static_cast<int>(nums.size());
    }

    int write_index = 2; // first two elements are always kept
    for (int read_index = 2; read_index < static_cast<int>(nums.size()); ++read_index) {
        // If current element differs from the element two positions back in the result,
        // it's safe to include it (ensures at most two copies).
        if (nums[read_index] != nums[write_index - 2]) {
            nums[write_index] = nums[read_index];
            ++write_index;
        }
    }

    return write_index;
}
#include <cassert>
#include <vector>

// Declaration of the function under test (assumed defined above)
int removeDuplicatesAtMostTwo(std::vector<int>& nums);

int main() {
    // Example 1
    std::vector<int> nums1 = {1, 1, 1, 2, 2, 3};
    int k1 = removeDuplicatesAtMostTwo(nums1);
    assert(k1 == 5);
    std::vector<int> expected1 = {1, 1, 2, 2, 3};
    for (int i = 0; i < k1; ++i) assert(nums1[i] == expected1[i]);

    // Example 2
    std::vector<int> nums2 = {0, 0, 1, 1, 1, 1, 2, 3, 3};
    int k2 = removeDuplicatesAtMostTwo(nums2);
    assert(k2 == 7);
    std::vector<int> expected2 = {0, 0, 1, 1, 2, 3, 3};
    for (int i = 0; i < k2; ++i) assert(nums2[i] == expected2[i]);

    // All duplicates
    std::vector<int> nums3 = {7, 7, 7, 7};
    int k3 = removeDuplicatesAtMostTwo(nums3);
    assert(k3 == 2);
    assert(nums3[0] == 7 && nums3[1] == 7);

    // Already at most two
    std::vector<int> nums4 = {1, 1, 2, 2, 3, 3};
    int k4 = removeDuplicatesAtMostTwo(nums4);
    assert(k4 == 6);
    std::vector<int> expected4 = {1, 1, 2, 2, 3, 3};
    for (int i = 0; i < k4; ++i) assert(nums4[i] == expected4[i]);

    // Single element
    std::vector<int> nums5 = {42};
    int k5 = removeDuplicatesAtMostTwo(nums5);
    assert(k5 == 1 && nums5[0] == 42);

    // Two identical elements
    std::vector<int> nums6 = {5, 5};
    int k6 = removeDuplicatesAtMostTwo(nums6);
    assert(k6 == 2 && nums6[0] == 5 && nums6[1] == 5);

    // Negative numbers
    std::vector<int> nums7 = {-3, -3, -2, -2, -2, -1};
    int k7 = removeDuplicatesAtMostTwo(nums7);
    assert(k7 == 5);
    std::vector<int> expected7 = {-3, -3, -2, -2, -1};
    for (int i = 0; i < k7; ++i) assert(nums7[i] == expected7[i]);

    // Single duplicate pair then many others
    std::vector<int> nums8 = {1, 1, 1, 2, 3, 3, 3, 4, 4, 4, 4};
    int k8 = removeDuplicatesAtMostTwo(nums8);
    assert(k8 == 8);
    std::vector<int> expected8 = {1, 1, 2, 3, 3, 4, 4, 4}; // wait, at most twice → 4 should appear twice
    // Correct expected: {1,1,2,3,3,4,4} length 7? Let's recalc: 1 appears twice, 2 once, 3 twice, 4 twice → total 7
    // Actually let me correct: 4 appears four times, so only two copies → {1,1,2,3,3,4,4} = 7
    expected8 = {1, 1, 2, 3, 3, 4, 4};
    assert(k8 == static_cast<int>(expected8.size()));
    for (int i = 0; i < k8; ++i) assert(nums8[i] == expected8[i]);

    return 0;
}
// The core idea uses a two-pointer technique adapted for allowing up to two copies. Since the array is sorted, any valid element at position `index` (where `index` is the current write position) must be compared to the element two positions before the write position. If `nums[i]` (the current read element) is different from `nums[index-2]`, then placing it at `nums[index]` is safe because it would not create more than two consecutive copies—if it were equal to `nums[index-2]`, then there would already be two copies of that value before it (since the array is sorted, and we only allow two). This comparison works because after writing, the element at `index-2` is the first of the two possible copies, and if the current element equals it, then adding it would give three copies. Therefore, we only copy when they differ. Initialization: if the vector has fewer than 3 elements, all elements are already valid, so return the original size. Otherwise, start both pointers at index 2 (the third position) because the first two positions are always kept. Iterate `i` from 2 to the end, and for each `i`, if `nums[i] != nums[index-2]`, copy `nums[i]` to `nums[index]` and increment `index`. Edge cases: empty vector is not allowed per constraints (length >= 1), but the code could handle it by returning 0. Vectors of length 1 or 2 require no changes. Duplicate-heavy arrays (e.g., all equal values) will result in only two copies at the front, and the algorithm correctly skips all subsequent duplicates because they will equal `nums[index-2]` after the second copy is placed. Time complexity is O(n) with a single pass; space complexity is O(1) auxiliary.
