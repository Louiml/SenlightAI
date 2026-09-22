// Write a C++ function `int insertPosition(const std::vector<int>& nums, int target)` that, given a sorted vector of distinct integers in non-decreasing order and a target value, returns the index where the target is found if it exists, or the index where it would be inserted to maintain the sorted order. The vector may be empty, and the target may be smaller than all elements or larger than all elements. Your function must not modify the input vector and should be efficient. Do not use standard library binary search functions like `std::lower_bound`; implement the logic yourself.
The cleanest approach is a binary search on the sorted array. Maintain two pointers: `left = 0` and `right = nums.size()` (where `right` is one past the last index). While `left < right`, compute `mid = left + (right - left) / 2` to avoid overflow. If `nums[mid] == target`, return `mid` immediately. If `nums[mid] < target`, then the target must be to the right, so set `left = mid + 1`. Otherwise, set `right = mid`. After the loop, `left` is the first position where `nums[left] >= target`, which is exactly the insertion index or the found index (if found, we would have returned earlier). Edge cases: empty vector — loop doesn't run, returns 0. Target smaller than all — `left` stays 0. Target larger than all — `left` becomes `nums.size()`. Time complexity is O(log n) for n elements, space O(1). This avoids the O(n) linear scans from the original snippet.
#include <vector>

// Returns the index of target if present, otherwise the insertion index
// to maintain sorted order. The input vector is assumed sorted in ascending order.
int insertPosition(const std::vector<int>& nums, int target) {
    int left = 0;
    int right = static_cast<int>(nums.size()); // one past the last valid index

    while (left < right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] == target) {
            return mid;
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}
#include <cassert>
#include <vector>

int main() {
    // Basic case from the original snippet
    std::vector<int> nums1 = {1, 3, 5, 6};
    assert(insertPosition(nums1, 5) == 2); // found
    assert(insertPosition(nums1, 2) == 1); // insert between 1 and 3
    assert(insertPosition(nums1, 7) == 4); // insert at end
    assert(insertPosition(nums1, 0) == 0); // insert at beginning

    // Empty vector
    std::vector<int> empty;
    assert(insertPosition(empty, 42) == 0);

    // Single element
    std::vector<int> single = {10};
    assert(insertPosition(single, 10) == 0);
    assert(insertPosition(single, 5) == 0);
    assert(insertPosition(single, 15) == 1);

    // Larger sorted vector with distinct values
    std::vector<int> nums2 = {3, 6, 7, 8, 10};
    assert(insertPosition(nums2, 7) == 2);
    assert(insertPosition(nums2, 6) == 1);
    assert(insertPosition(nums2, 9) == 4);
    assert(insertPosition(nums2, 1) == 0);
    assert(insertPosition(nums2, 11) == 5);

    // All elements less than target, and all greater
    std::vector<int> nums3 = {2, 4, 6, 8};
    assert(insertPosition(nums3, 9) == 4);
    assert(insertPosition(nums3, 1) == 0);

    return 0;
}
