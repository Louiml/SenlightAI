You are given a sorted (in ascending order) array of integers that may contain duplicates, and a target integer value. Write a C++ function that returns the index of the *first occurrence* of the target in the array, or `-1` if the target is not present. The array is guaranteed to be sorted, but may be empty. For example, given `nums = {1, 2, 2, 2, 3}` and `target = 2`, your function must return `1` (the index of the first `2`), not `2` or `3`. You must implement an efficient binary-search-based algorithm; a linear scan is not acceptable. Handle edge cases such as an empty array, target smaller than all elements, target larger than all elements, and duplicates at the very beginning or end of the array.

// The main challenge is to find the leftmost index of the target in a sorted array. Use a standard binary search, but instead of returning immediately when `nums[mid] == target`, continue searching in the left half (`right = mid - 1`) even after a match, because there might be earlier duplicates. Keep a variable `result` initialized to `-1`; update it to `mid` whenever a match is found, but continue narrowing the search to the left to look for an earlier occurrence. The loop condition is `left <= right` to ensure we check the middle element when the search interval shrinks to one element. Edge cases: for an empty array, `left = 0, right = -1` and the loop never runs, returning `-1`. If the target is smaller than all elements, `right` decreases repeatedly until `left > right`, returning `-1`. If the target is larger than all elements, `left` increases past `right`, returning `-1`. For duplicates, the algorithm correctly finds the first occurrence because every time a match is found, we record `mid` and then restrict the search to `[left, mid-1]`. Time complexity is `O(log n)` for `n` elements, and space complexity is `O(1)` auxiliary (only a few integer variables).

#include <vector>

// Returns the index of the first occurrence of target in the sorted vector nums,
// or -1 if target is not present. Assumes nums is sorted in ascending order.
int firstOccurrence(const std::vector<int>& nums, int target) {
    int left = 0;
    int right = static_cast<int>(nums.size()) - 1;
    int result = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;  // avoid overflow

        if (nums[mid] == target) {
            result = mid;          // record this occurrence
            right = mid - 1;       // look for earlier occurrences on the left
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Empty array
    assert(firstOccurrence({}, 5) == -1);

    // Single element, match and no match
    assert(firstOccurrence({5}, 5) == 0);
    assert(firstOccurrence({5}, 3) == -1);

    // No duplicates, target present / absent
    std::vector<int> nums1 = {1, 3, 5, 7, 9};
    assert(firstOccurrence(nums1, 5) == 2);
    assert(firstOccurrence(nums1, 6) == -1);
    assert(firstOccurrence(nums1, 0) == -1);
    assert(firstOccurrence(nums1, 10) == -1);

    // Duplicates at various positions
    std::vector<int> nums2 = {1, 2, 2, 2, 3};
    assert(firstOccurrence(nums2, 2) == 1);
    assert(firstOccurrence(nums2, 3) == 4);
    assert(firstOccurrence(nums2, 1) == 0);

    // Duplicates at the very beginning
    std::vector<int> nums3 = {0, 0, 0, 1, 2};
    assert(firstOccurrence(nums3, 0) == 0);

    // Duplicates at the very end
    std::vector<int> nums4 = {1, 2, 3, 3, 3};
    assert(firstOccurrence(nums4, 3) == 2);

    // All same elements
    std::vector<int> nums5 = {7, 7, 7, 7};
    assert(firstOccurrence(nums5, 7) == 0);
    assert(firstOccurrence(nums5, 8) == -1);

    // Negative numbers
    std::vector<int> nums6 = {-5, -3, -3, 0, 4};
    assert(firstOccurrence(nums6, -3) == 1);
    assert(firstOccurrence(nums6, -5) == 0);

    return 0;
}
