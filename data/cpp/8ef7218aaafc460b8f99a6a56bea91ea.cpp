// Given a sorted array of distinct integers and a target value, write a C++ function that returns the index of the target if it exists, or the index where it would be inserted to maintain sorted order. The function must take a `const std::vector<int>&` and an `int` target, returning `size_t`. For example, given `[1, 3, 5, 6]` and target `5`, return `2`; target `2` returns `1`; target `7` returns `4`; target `0` returns `0`. This is the classic "search insert position" problem, but must be implemented using binary search for efficiency, not linear scan.

// The solution uses a standard binary search algorithm for a sorted array. We maintain two pointers: `left` starting at `0` and `right` starting at `nums.size()`. The invariant is that `nums[left-1] < target` (or `left == 0`) and `nums[right] >= target` (or `right == size`), so the answer is always between `left` and `right`. We repeatedly compute `mid = left + (right - left) / 2` to avoid overflow. If `nums[mid] < target`, then the target must be to the right, so we set `left = mid + 1`; otherwise (including equality), the target is at `mid` or to the left, so we set `right = mid`. When `left` and `right` converge, `left` is the insert position. This handles edge cases: an empty array returns `0`; target smaller than all elements returns `0`; target larger than all returns `size`; duplicates are not present per the problem statement. Time complexity is \(O(\log n)\) and space complexity is \(O(1)\).

#include <vector>
#include <cstddef>

// Search for the target in a sorted array, or return the insertion index.
// Precondition: nums is sorted in ascending order with distinct integers.
size_t searchInsertPosition(const std::vector<int>& nums, int target) {
    size_t left = 0;
    size_t right = nums.size();

    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

#include <cassert>
#include <vector>

// The solution function is assumed to be available from the Solution section.
// Copy the implementation above for the test to compile.

int main() {
    std::vector<int> nums = {1, 3, 5, 6};

    assert(searchInsertPosition(nums, 5) == 2);
    assert(searchInsertPosition(nums, 2) == 1);
    assert(searchInsertPosition(nums, 7) == 4);
    assert(searchInsertPosition(nums, 0) == 0);

    std::vector<int> empty;
    assert(searchInsertPosition(empty, 10) == 0);

    std::vector<int> single = {42};
    assert(searchInsertPosition(single, 42) == 0);
    assert(searchInsertPosition(single, 41) == 0);
    assert(searchInsertPosition(single, 43) == 1);

    std::vector<int> neg = {-10, -5, 0, 3};
    assert(searchInsertPosition(neg, -7) == 1);
    assert(searchInsertPosition(neg, -10) == 0);
    assert(searchInsertPosition(neg, 3) == 3);
    assert(searchInsertPosition(neg, 4) == 4);

    return 0;
}
