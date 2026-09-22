// Write a C++ function `int findInsertPosition(const std::vector<int>& nums, int target)` that returns the index at which `target` should be inserted into an already sorted (ascending) vector `nums` to maintain order. If `target` already exists, return its first occurrence index. The vector may contain duplicate values, negative numbers, and can be empty. The function must use binary search, not linear scan, and must be `const`-correct (i.e., it must not modify the vector).

// The solution uses classic binary search on a sorted array. We maintain two pointers `low` and `high` representing the current search interval. In each iteration, we compute `mid = low + (high - low) / 2` to avoid integer overflow. If `nums[mid] == target`, we return `mid` directly. If `nums[mid] < target`, we move `low = mid + 1` because the target must be to the right. Otherwise, `nums[mid] > target`, we move `high = mid - 1` because the target must be to the left. The loop continues while `low <= high`. When the loop terminates (i.e., `low > high`), `low` is exactly the position where `target` should be inserted to keep the array sorted—this works because all elements before `low` are less than `target` (or equal, but those cases would have been caught earlier), and all elements from `low` onward are greater than `target`. This also handles duplicates correctly because if duplicates exist, we return the first match found by binary search. Edge cases: empty vector → loop never runs, returns `0`; target smaller than all elements → `low` stays `0`; target larger than all → `low` becomes `nums.size()`. Time complexity is O(log n) and space complexity is O(1).

#include <vector>

/*
 * Returns the index where 'target' should be inserted in sorted 'nums'.
 * If 'target' exists, returns the index of its first occurrence.
 * Uses binary search; expects 'nums' to be sorted in ascending order.
 */
int findInsertPosition(const std::vector<int>& nums, int target) {
    int low = 0;
    int high = static_cast<int>(nums.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2; // avoid overflow
        if (nums[mid] == target) {
            return mid;
        } else if (nums[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return low; // insertion position
}

#include <cassert>
#include <vector>

int main() {
    // Empty vector
    std::vector<int> empty;
    assert(findInsertPosition(empty, 5) == 0);

    // Single element
    std::vector<int> single = {3};
    assert(findInsertPosition(single, 3) == 0);
    assert(findInsertPosition(single, 2) == 0);
    assert(findInsertPosition(single, 4) == 1);

    // Standard cases
    std::vector<int> nums = {1, 3, 5, 6};
    assert(findInsertPosition(nums, 5) == 2);
    assert(findInsertPosition(nums, 2) == 1);
    assert(findInsertPosition(nums, 7) == 4);
    assert(findInsertPosition(nums, 0) == 0);

    // With duplicates
    std::vector<int> dup = {1, 2, 2, 3};
    assert(findInsertPosition(dup, 2) == 1);
    assert(findInsertPosition(dup, 4) == 4);

    // With negatives
    std::vector<int> neg = {-5, -1, 0, 4};
    assert(findInsertPosition(neg, -3) == 1);
    assert(findInsertPosition(neg, 6) == 4);

    // Large vector to test binary search correctness
    std::vector<int> large;
    for (int i = 0; i < 1000; ++i) large.push_back(i * 2); // even numbers
    assert(findInsertPosition(large, 500) == 250);
    assert(findInsertPosition(large, 501) == 251);
    assert(findInsertPosition(large, 1999) == 1000);
    return 0;
}
