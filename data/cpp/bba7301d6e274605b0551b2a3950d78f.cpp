/*
Write a C++ function named `searchInsertPosition` that takes a sorted vector of integers (`std::vector<int>`) and a target integer, and returns the index where the target would be inserted to maintain sorted order. If the target already exists in the vector, return its index. The function must handle duplicates correctly (returning the first occurrence's index), must work for an empty vector, and must not use any standard library algorithms like `lower_bound` — you must implement the binary search logic manually. The function should be `const`‑correct (accept a `const` reference to the vector) and must not modify the input.
*/

#include <vector>

// Returns the insert position of `target` in the sorted vector `nums`.
// If target exists, returns the index of its first occurrence.
// The input vector must be sorted in ascending order.
int searchInsertPosition(const std::vector<int>& nums, int target) {
    int left = 0;
    int right = static_cast<int>(nums.size()); // exclusive upper bound

    while (left < right) {
        int mid = left + (right - left) / 2; // avoid overflow
        if (nums[mid] < target) {
            left = mid + 1; // target is to the right
        } else {
            right = mid; // target is at mid or to the left
        }
    }
    return left; // left == right == insertion index
}

#include <cassert>
#include <vector>

// Declaration of the function under test
int searchInsertPosition(const std::vector<int>& nums, int target);

int main() {
    // Basic cases
    assert(searchInsertPosition({1, 3, 5, 6}, 5) == 2);
    assert(searchInsertPosition({1, 3, 5, 6}, 2) == 1);
    assert(searchInsertPosition({1, 3, 5, 6}, 7) == 4);
    assert(searchInsertPosition({1, 3, 5, 6}, 0) == 0);

    // Duplicate handling: returns first occurrence
    assert(searchInsertPosition({1, 3, 3, 3, 5}, 3) == 1);

    // Empty vector
    assert(searchInsertPosition({}, 10) == 0);

    // Single element
    assert(searchInsertPosition({7}, 7) == 0);
    assert(searchInsertPosition({7}, 5) == 0);
    assert(searchInsertPosition({7}, 9) == 1);

    // Negative numbers and mixed values
    assert(searchInsertPosition({-5, -2, 0, 3, 8}, -2) == 1);
    assert(searchInsertPosition({-5, -2, 0, 3, 8}, 4) == 4);
    assert(searchInsertPosition({-5, -2, 0, 3, 8}, -10) == 0);
    assert(searchInsertPosition({-5, -2, 0, 3, 8}, 100) == 5);

    return 0;
}

// The problem is a classic binary search variant. We maintain two pointers: `left = 0` and `right = size` (exclusive upper bound). While `left < right`, compute `mid = left + (right - left) / 2`. If `nums[mid] < target`, then the target (or its insertion point) must be to the right, so set `left = mid + 1`. Otherwise (i.e., `nums[mid] >= target`), set `right = mid`. This loop continues until `left == right`, which is the first position where `nums[pos] >= target` (or `pos == size` if all elements are smaller). This position is exactly the insertion index for a target not present, and for a present target it is the first occurrence index because we move `right` leftward even on equality. Edge cases: empty vector → returns 0; target smaller than all elements → returns 0; target larger than all elements → returns `size`. Time complexity is O(log n) for `n` elements; space complexity is O(1) auxiliary.
