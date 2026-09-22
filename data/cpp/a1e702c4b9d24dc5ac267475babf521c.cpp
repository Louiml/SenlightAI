Write a C++ function `int searchRotated(const std::vector<int>& nums, int target)` that searches for a given `target` value in a circularly sorted array of integers (i.e., a sorted array that has been rotated at some unknown pivot). The array contains distinct integers and is guaranteed to be rotated at least once unless it has size 0 or 1. Return the index of `target` if it exists, otherwise return -1. The solution must run in O(log n) time using binary search, and must handle edge cases like empty arrays, single-element arrays, targets smaller/larger than all elements, and arrays that are not rotated (i.e., already fully sorted – the algorithm must still work correctly).

#include <cassert>
#include <vector>

// Forward declaration of the function being tested
int searchRotated(const std::vector<int>& nums, int target);

int main() {
    // Empty vector
    std::vector<int> empty;
    assert(searchRotated(empty, 5) == -1);
    
    // Single element
    std::vector<int> single = {7};
    assert(searchRotated(single, 7) == 0);
    assert(searchRotated(single, 3) == -1);
    
    // Standard rotated arrays
    std::vector<int> rot1 = {4,5,6,7,0,1,2};
    assert(searchRotated(rot1, 0) == 4);
    assert(searchRotated(rot1, 3) == -1);
    assert(searchRotated(rot1, 4) == 0);
    assert(searchRotated(rot1, 2) == 6);
    
    // Not rotated (fully sorted)
    std::vector<int> sorted = {1,2,3,4,5};
    assert(searchRotated(sorted, 1) == 0);
    assert(searchRotated(sorted, 5) == 4);
    assert(searchRotated(sorted, 6) == -1);
    
    // Rotated at different pivot
    std::vector<int> rot2 = {6,7,8,1,2,3,4,5};
    assert(searchRotated(rot2, 8) == 2);
    assert(searchRotated(rot2, 1) == 3);
    assert(searchRotated(rot2, 5) == 7);
    
    // Two-element rotated
    std::vector<int> two = {2,1};
    assert(searchRotated(two, 2) == 0);
    assert(searchRotated(two, 1) == 1);
    assert(searchRotated(two, 3) == -1);
    
    return 0;
}

#include <vector>

// Search for target in a rotated sorted array with distinct integers.
// Returns the index of target if found, otherwise -1.
// Assumes the input vector is a rotation of a sorted array (or empty/single-element).
int searchRotated(const std::vector<int>& nums, int target) {
    int start = 0;
    int end = static_cast<int>(nums.size()) - 1;
    
    while (start <= end) {
        int mid = start + (end - start) / 2;
        
        if (nums[mid] == target) {
            return mid;
        }
        
        // Left half (start..mid) is sorted if nums[mid] >= nums[start]
        if (nums[mid] >= nums[start]) {
            // target lies within the sorted left half
            if (target >= nums[start] && target < nums[mid]) {
                end = mid - 1;
            } else {
                start = mid + 1;
            }
        } else {
            // Right half (mid..end) is sorted
            // target lies within the sorted right half
            if (target > nums[mid] && target <= nums[end]) {
                start = mid + 1;
            } else {
                end = mid - 1;
            }
        }
    }
    return -1;
}

// The core idea is a modified binary search that accounts for the rotation. Because the array is sorted but rotated, at each step we know that at least one half (from `start` to `mid` or `mid` to `end`) is fully sorted. We can detect which half is sorted by comparing `nums[mid]` with `nums[start]`; since all elements are distinct, if `nums[mid] >= nums[start]`, then the left half is sorted (and monotonic). In that sorted half, we can check if the target lies within that range (inclusive). If yes, we narrow our search to that half; otherwise, the target must be in the other half. Similarly, if the left half is not sorted, then the right half must be sorted, and we apply analogous logic. Edge cases: an empty vector returns -1; a single-element vector returns 0 if it matches target, else -1. For an already sorted (non-rotated) array, the condition `nums[mid] >= nums[start]` is always true, so the left half is always treated as sorted and the algorithm degenerates to a standard binary search. Time complexity is O(log n) because we discard half the search space each iteration. Space complexity is O(1) since we only use a few integer variables.
