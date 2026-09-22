Write a C++ function that takes a reference to a sorted but possibly rotated vector of distinct integers and a target value, and returns the index of the target if it exists in the vector, or -1 if it does not. A rotated sorted array means that the array was originally sorted in ascending order, then some number of leading elements were moved to the end (e.g., [4,5,6,7,0,1,2] is a rotation of [0,1,2,4,5,6,7]). The function must run in O(log n) time, handle empty vectors, and return the correct index even when the rotation point is at any position, including when the array is not rotated at all. You may not use the standard library `std::find` or linear search.

// The solution uses a modified binary search on the rotated array. At each step, compute the middle index `mid`. If `nums[mid] == target`, return `mid`. Otherwise, determine which half is sorted by comparing `nums[left]` with `nums[mid]`. If `nums[left] <= nums[mid]`, the left half is sorted; then check if `target` lies within that sorted range `[nums[left], nums[mid]]`. If yes, search left by setting `right = mid - 1`; otherwise search right by setting `left = mid + 1`. If the right half is sorted (i.e., `nums[mid] < nums[right]`), check if `target` lies in `[nums[mid], nums[right]]`. If yes, search right; otherwise search left. This works because one half is always strictly sorted (since elements are distinct), and we can reliably eliminate half of the search space. Edge cases: empty vector (return -1), single element (check equality), rotation point exactly at `mid`, and target not present. Time complexity is O(log n) since each iteration halves the search range. Space complexity is O(1) auxiliary, only using a few integer variables.

#include <vector>

// Search for target in a sorted rotated array of distinct integers.
// Returns the index of target, or -1 if not found.
// Assumes the input vector is a rotation of a strictly increasing sequence.
int searchRotated(const std::vector<int>& nums, int target) {
    int left = 0;
    int right = static_cast<int>(nums.size()) - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (nums[mid] == target) {
            return mid;
        }

        // Left half is sorted
        if (nums[left] <= nums[mid]) {
            if (target >= nums[left] && target < nums[mid]) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        // Right half is sorted
        else {
            if (target > nums[mid] && target <= nums[right]) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }

    return -1;
}

#include <cassert>
#include <vector>

// Forward declaration of the function under test.
int searchRotated(const std::vector<int>& nums, int target);

int main() {
    // Non-rotated sorted array
    std::vector<int> arr1 = {1, 2, 3, 4, 5};
    assert(searchRotated(arr1, 3) == 2);
    assert(searchRotated(arr1, 1) == 0);
    assert(searchRotated(arr1, 5) == 4);
    assert(searchRotated(arr1, 0) == -1);

    // Rotated at mid
    std::vector<int> arr2 = {4, 5, 6, 7, 0, 1, 2};
    assert(searchRotated(arr2, 0) == 4);
    assert(searchRotated(arr2, 4) == 0);
    assert(searchRotated(arr2, 2) == 6);
    assert(searchRotated(arr2, 6) == 2);
    assert(searchRotated(arr2, 3) == -1);

    // Rotated with only two elements
    std::vector<int> arr3 = {2, 1};
    assert(searchRotated(arr3, 2) == 0);
    assert(searchRotated(arr3, 1) == 1);
    assert(searchRotated(arr3, 0) == -1);

    // Single element
    std::vector<int> arr4 = {42};
    assert(searchRotated(arr4, 42) == 0);
    assert(searchRotated(arr4, 41) == -1);

    // Empty vector
    std::vector<int> arr5;
    assert(searchRotated(arr5, 1) == -1);

    // Rotated by one from the start
    std::vector<int> arr6 = {5, 1, 2, 3, 4};
    assert(searchRotated(arr6, 1) == 1);
    assert(searchRotated(arr6, 5) == 0);
    assert(searchRotated(arr6, 4) == 4);
    assert(searchRotated(arr6, 0) == -1);

    // Larger rotation, all elements searched
    std::vector<int> arr7 = {3, 4, 5, 6, 7, 8, 9, 10, 1, 2};
    for (int i = 1; i <= 10; ++i) {
        int idx = searchRotated(arr7, i);
        assert(idx != -1);
        assert(arr7[idx] == i);
    }
    assert(searchRotated(arr7, 0) == -1);
    assert(searchRotated(arr7, 11) == -1);
}
