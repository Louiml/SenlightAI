// Given a rotated sorted array of distinct integers and a target value, write a C++ function `int searchRotatedSorted(const std::vector<int>& nums, int target)` that returns the index of the target if it exists, or -1 if it does not. The array was originally sorted in ascending order and then rotated at some unknown pivot (e.g., [4,5,6,7,0,1,2]). The function must use a modified binary search algorithm, handling both the left and right halves that may be unsorted, and must run in O(log n) time. Guarentee the input contains at least one element, and all integers are distinct.
// The key insight is that even though the array is rotated, at least one half of the array (left or right relative to the middle) is always sorted in ascending order. We perform a standard binary search but with an extra check: after computing `middle`, we determine which half is sorted. If the left half (`nums[left] <= nums[middle]`) is sorted, we check if the target lies within that sorted range; if so, we narrow to the left half, otherwise we go right. If the right half (`nums[middle] <= nums[right]`) is sorted, we similarly check if the target is in that range and decide which side to search. This works because in a rotated sorted array without duplicates, at least one side must be sorted. Edge cases include the target being at the pivot, a single-element array, and the target being the smallest or largest element. Time complexity is O(log n) since we discard half the search space each iteration, and space complexity is O(1) auxiliary.
#include <vector>

// Search for target in a rotated sorted array of distinct integers.
// Returns the index of target if found, otherwise -1.
int searchRotatedSorted(const std::vector<int>& nums, int target) {
    int left = 0;
    int right = static_cast<int>(nums.size()) - 1;

    while (left <= right) {
        int middle = left + (right - left) / 2;

        if (nums[middle] == target) {
            return middle;
        }

        // Check if the left half is sorted.
        if (nums[left] <= nums[middle]) {
            // Target is in the sorted left half.
            if (target >= nums[left] && target < nums[middle]) {
                right = middle - 1;
            } else {
                left = middle + 1;
            }
        } 
        // Otherwise, the right half is sorted.
        else {
            // Target is in the sorted right half.
            if (target > nums[middle] && target <= nums[right]) {
                left = middle + 1;
            } else {
                right = middle - 1;
            }
        }
    }

    return -1;
}
#include <cassert>
#include <vector>
#include "solution.h" // assuming the solution is in a header or included above

int main() {
    // Standard rotated array from the snippet
    std::vector<int> arr1 = {4,5,6,7,0,1,2};
    assert(searchRotatedSorted(arr1, 0) == 4);
    assert(searchRotatedSorted(arr1, 5) == 1);
    assert(searchRotatedSorted(arr1, 3) == -1);
    assert(searchRotatedSorted(arr1, 7) == 3);
    assert(searchRotatedSorted(arr1, 2) == 6);

    // Single element
    std::vector<int> arr2 = {3};
    assert(searchRotatedSorted(arr2, 3) == 0);
    assert(searchRotatedSorted(arr2, 1) == -1);

    // Already sorted (not rotated)
    std::vector<int> arr3 = {1,2,3,4,5};
    assert(searchRotatedSorted(arr3, 1) == 0);
    assert(searchRotatedSorted(arr3, 5) == 4);
    assert(searchRotatedSorted(arr3, 0) == -1);

    // Rotated at the very end (pivot at beginning)
    std::vector<int> arr4 = {2,3,4,5,1};
    assert(searchRotatedSorted(arr4, 1) == 4);
    assert(searchRotatedSorted(arr4, 2) == 0);

    // Larger rotated array
    std::vector<int> arr5 = {6,7,8,9,10,1,2,3,4,5};
    assert(searchRotatedSorted(arr5, 10) == 4);
    assert(searchRotatedSorted(arr5, 1) == 5);
    assert(searchRotatedSorted(arr5, 5) == 9);
    assert(searchRotatedSorted(arr5, 11) == -1);

    return 0;
}
