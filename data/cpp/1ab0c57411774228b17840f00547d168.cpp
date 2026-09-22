Write a C++ function that, given a sorted array of distinct integers and a target value, returns the index where the target is found, or the index where it would be inserted to maintain the sorted order. The input vector may be empty, and the target may be smaller than all elements, larger than all elements, or equal to an existing element. Your function should operate in O(log n) time.
// The solution uses binary search to find the target or its insertion point. We maintain two pointers, `left` and `right`, representing the current search range. The loop continues while `left < right`. At each step, we compute the midpoint without overflow using `left + (right - left) / 2`. If the target equals `nums[mid]`, we return `mid` immediately. If `nums[mid]` is greater than the target, we move the `right` pointer to `mid` (not `mid - 1`) because `mid` could be the insertion point if the target is smaller than all elements from `mid` onward. Otherwise, we move `left` to `mid + 1`. After the loop, the `left` pointer marks the first position where `nums[left] >= target`, so if `nums[left] < target`, we return `left + 1`; otherwise we return `left`. This handles empty arrays (loop skipped, but we must guard with a size check) and all edge cases. Time complexity is O(log n), space complexity is O(1). The main subtlety is the `right = mid` assignment, which ensures the insertion point is correctly found when the target is not present. For an empty vector, the function should return 0.
#include <vector>

// Return the insertion index of target in sorted distinct `nums`.
int searchInsertPosition(const std::vector<int>& nums, int target) {
    int left = 0;
    int right = static_cast<int>(nums.size()) - 1;

    // Handle empty vector
    if (nums.empty()) return 0;

    while (left < right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] == target) return mid;
        if (nums[mid] > target) {
            right = mid; // mid might be insertion point
        } else {
            left = mid + 1;
        }
    }

    // left == right; check if target is greater than the last element
    if (nums[left] < target) return left + 1;
    return left;
}
#include <cassert>
#include <vector>

int main() {
    std::vector<int> nums1 = {1, 3, 5, 6};
    assert(searchInsertPosition(nums1, 5) == 2);
    assert(searchInsertPosition(nums1, 2) == 1);
    assert(searchInsertPosition(nums1, 7) == 4);
    assert(searchInsertPosition(nums1, 0) == 0);

    std::vector<int> nums2 = {1};
    assert(searchInsertPosition(nums2, 0) == 0);
    assert(searchInsertPosition(nums2, 2) == 1);
    assert(searchInsertPosition(nums2, 1) == 0);

    std::vector<int> nums3 = {};
    assert(searchInsertPosition(nums3, 5) == 0);

    std::vector<int> nums4 = {1, 4, 9, 10, 15};
    assert(searchInsertPosition(nums4, 10) == 3);
    assert(searchInsertPosition(nums4, 12) == 4);
    assert(searchInsertPosition(nums4, 4) == 1);
    assert(searchInsertPosition(nums4, 16) == 5);
    assert(searchInsertPosition(nums4, 0) == 0);
}
