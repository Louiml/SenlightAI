Given a sorted array of distinct integers in ascending order and a target value, write a C++ function `int searchInsert(const std::vector<int>& nums, int target)` that returns the index where the target is found, or the index where it would be inserted to maintain the sorted order. The function must run in O(log n) time and O(1) space. The array is guaranteed non-empty (length ≥ 1), values and target are within [-10^4, 10^4]. The function must correctly handle cases where the target is smaller than all elements (return 0), larger than all elements (return array length), or equal to an existing element (return its exact index). You may not use built-in binary search functions like `std::lower_bound` – implement the binary search logic yourself.

// The core algorithm is a classic binary search that finds the leftmost position where the target either exists or should be inserted. Maintain two pointers `left = 0` and `right = nums.size() - 1`. While `left <= right`:, compute `mid = left + (right - left) / 2` to avoid overflow. Compare `nums[mid]` with the target:
// - If `nums[mid] == target`: return `mid` immediately because elements are distinct, so no duplicates.
// - If `nums[mid] < target`: the target must be to the right, so set `left = mid + 1`.
// - If `nums[mid] > target`: the target must be to the left, so set `right = mid - 1`.
// When the loop terminates, `left` is the insertion position because it points to the first index where `nums[left] >= target`, which is exactly what is needed. Edge cases: target smaller than all elements – loop ends with `left = 0`; target larger than all elements – loop ends with `left = nums.size()`. Since the array is non-empty, no empty-array special casing is needed. Time complexity O(log n) because each iteration halves the search space. Space complexity O(1) – only two integer pointers.

#include <vector>

// Returns the index of target if present, otherwise the index where it should be inserted
// to maintain ascending order. Assumes nums is a non-empty sorted vector of distinct integers.
int searchInsert(const std::vector<int>& nums, int target) {
    int left = 0;
    int right = static_cast<int>(nums.size()) - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2; // avoids potential overflow
        if (nums[mid] == target) {
            return mid; // found exact match, distinct elements so first occurrence is unique
        } else if (nums[mid] < target) {
            left = mid + 1; // target lies to the right
        } else {
            right = mid - 1; // target lies to the left
        }
    }
    
    // Loop exits when left > right; left is the correct insertion position.
    return left;
}

#include <cassert>
#include <vector>

// The solution function is declared elsewhere; include it here for completeness.
// In a real setup, you would include the header or copy the implementation above.

int main() {
    // Basic cases from the problem statement
    std::vector<int> nums1 = {1, 3, 5, 6};
    assert(searchInsert(nums1, 5) == 2);
    assert(searchInsert(nums1, 2) == 1);
    assert(searchInsert(nums1, 7) == 4);
    assert(searchInsert(nums1, 0) == 0);
    
    // Single-element array
    std::vector<int> nums2 = {42};
    assert(searchInsert(nums2, 42) == 0);
    assert(searchInsert(nums2, 10) == 0);
    assert(searchInsert(nums2, 100) == 1);
    
    // Larger arrays, boundaries
    std::vector<int> nums3 = {-10, -5, 0, 3, 8, 12};
    assert(searchInsert(nums3, -10) == 0);
    assert(searchInsert(nums3, 12) == 5);
    assert(searchInsert(nums3, -7) == 1);
    assert(searchInsert(nums3, 1) == 3);
    assert(searchInsert(nums3, 20) == 6);
    
    // Negative targets and values
    std::vector<int> nums4 = {-100, -50, 0, 50, 100};
    assert(searchInsert(nums4, -200) == 0);
    assert(searchInsert(nums4, 200) == 5);
    assert(searchInsert(nums4, -50) == 1);
    assert(searchInsert(nums4, -1) == 2);
    
    // All equal check (distinct assumption, but test exact target)
    std::vector<int> nums5 = {1, 2, 3, 4, 5};
    assert(searchInsert(nums5, 3) == 2);
    assert(searchInsert(nums5, 6) == 5);
    
    return 0;
}
