// Given a sorted vector of distinct integers and a target value, write a C++ function that returns the index where the target would be inserted to maintain sorted order. If the target already exists in the vector, return its index. The function must operate in O(log n) time and must not modify the input vector. Assume the vector is non-empty and strictly increasing (no duplicates). Handle edge cases where the target is smaller than all elements, larger than all elements, or exactly equal to an element.

// The problem is a classic binary search variant often called "search insert position." The main algorithm is to perform a binary search that narrows the range `[low, high]` until `low > high`. At each step, compare the middle element with the target. If the middle element is less than the target, move `low` to `mid + 1`; if it is greater, move `high` to `mid - 1`; if equal, return `mid`. When the loop ends, `low` holds the correct insertion index because it represents the first position where all elements before it are less than the target and all elements from it onward are greater than or equal to the target. This is equivalent to `std::lower_bound`. Important edge cases: (1) target smaller than first element → returns 0, (2) target larger than last element → returns size of vector, (3) target present → returns its exact index. Time complexity is O(log n) because each step halves the search interval. Space complexity is O(1) auxiliary, as only a few integer variables are used, and the input is not copied.

#include <vector>

// Return the insertion index of target in sorted distinct vector nums.
// If target exists, return its index; otherwise, return the index where it would be inserted.
int searchInsertPosition(const std::vector<int>& nums, int target) {
    int low = 0;
    int high = static_cast<int>(nums.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2; // avoid potential overflow
        if (nums[mid] == target) {
            return mid;
        } else if (nums[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return low; // low is the first index where nums[low] > target
}

#include <cassert>
#include <vector>

// Function declaration (assumed from solution)
int searchInsertPosition(const std::vector<int>& nums, int target);

int main() {
    std::vector<int> nums1 = {1, 3, 5, 6};
    assert(searchInsertPosition(nums1, 5) == 2);
    assert(searchInsertPosition(nums1, 2) == 1);
    assert(searchInsertPosition(nums1, 7) == 4);
    assert(searchInsertPosition(nums1, 0) == 0);

    std::vector<int> nums2 = {10, 20, 30};
    assert(searchInsertPosition(nums2, 10) == 0);
    assert(searchInsertPosition(nums2, 15) == 1);
    assert(searchInsertPosition(nums2, 30) == 2);
    assert(searchInsertPosition(nums2, 25) == 2);
    assert(searchInsertPosition(nums2, 40) == 3);

    std::vector<int> nums3 = {-5, -1, 0, 3};
    assert(searchInsertPosition(nums3, -5) == 0);
    assert(searchInsertPosition(nums3, -2) == 1);
    assert(searchInsertPosition(nums3, 0) == 2);
    assert(searchInsertPosition(nums3, 2) == 3);
    assert(searchInsertPosition(nums3, 4) == 4);

    return 0;
}
