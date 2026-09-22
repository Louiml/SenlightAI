/*
Write a C++ function that finds the index of a peak element in a given vector of integers. A peak element is an element that is strictly greater than its neighbors (if they exist). The function must work for arrays of any size, including a single element, and must be efficient for large inputs. It should return the index of any one peak if multiple exist. The algorithm must be based on divide-and-conquer (binary search style) rather than a linear scan, and the function signature should be `int findPeakElement(const std::vector<int>& nums)`. The function must handle edge cases where the peak is at the first or last position, and it must correctly handle arrays with strictly increasing, strictly decreasing, or non-monotonic sequences.
*/

#include <vector>

// Find the index of a peak element in a non-empty vector.
// Returns -1 if the vector is empty (should not happen per task).
int findPeakElement(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    if (n == 0) {
        return -1; // Handle empty input defensively.
    }
    if (n == 1) {
        return 0;
    }

    int left = 0;
    int right = n - 1;

    while (left <= right) {
        const int mid = left + (right - left) / 2;

        // Check if mid is a peak: compare with neighbors (treat boundaries as -infinity)
        const bool leftOk = (mid == 0) || (nums[mid] > nums[mid - 1]);
        const bool rightOk = (mid == n - 1) || (nums[mid] > nums[mid + 1]);

        if (leftOk && rightOk) {
            return mid;
        }

        // If left neighbor is greater, peak must be in left half.
        if (mid > 0 && nums[mid - 1] > nums[mid]) {
            right = mid - 1;
        } else {
            // Otherwise, peak must be in right half (since right neighbor is greater).
            left = mid + 1;
        }
    }

    // Should never reach here for a non-empty array.
    return -1;
}

#include <cassert>
#include <vector>

// The function declaration is assumed to be available from the solution part.
int findPeakElement(const std::vector<int>& nums);

int main() {
    // Single element: the only element is a peak.
    std::vector<int> arr1 = {5};
    assert(findPeakElement(arr1) == 0);

    // Strictly increasing: peak is the last element.
    std::vector<int> arr2 = {1, 2, 3, 4};
    assert(findPeakElement(arr2) == 3);

    // Strictly decreasing: peak is the first element.
    std::vector<int> arr3 = {10, 8, 6, 4};
    assert(findPeakElement(arr3) == 0);

    // Multiple peaks: any valid index is accepted, so check the value at that index.
    std::vector<int> arr4 = {1, 3, 2, 5, 4};
    int idx4 = findPeakElement(arr4);
    assert(idx4 >= 0 && idx4 < 5);
    assert((idx4 == 0 || arr4[idx4] > arr4[idx4-1]) && (idx4 == 4 || arr4[idx4] > arr4[idx4+1]));

    // Peak in the middle.
    std::vector<int> arr5 = {1, 2, 3, 1};
    assert(findPeakElement(arr5) == 2);

    // Duplicate values but no equal neighbors? Actually duplicates may not be strictly greater, but we still handle.
    std::vector<int> arr6 = {2, 2, 2};
    int idx6 = findPeakElement(arr6);
    assert(idx6 >= 0 && idx6 < 3);

    // Larger test: alternating peaks.
    std::vector<int> arr7 = {1, 5, 3, 7, 2, 9, 0};
    int idx7 = findPeakElement(arr7);
    assert(idx7 >= 0 && idx7 < 7);
    assert((idx7 == 0 || arr7[idx7] > arr7[idx7-1]) && (idx7 == 6 || arr7[idx7] > arr7[idx7+1]));

    // Two elements where first is peak.
    std::vector<int> arr8 = {4, 2};
    assert(findPeakElement(arr8) == 0);

    // Two elements where second is peak.
    std::vector<int> arr9 = {2, 4};
    assert(findPeakElement(arr9) == 1);

    return 0;
}

// The core idea is to use binary search to narrow down the region containing a peak. At each step, we examine the middle element and compare it with its neighbors. If the middle element is greater than both neighbors (or has only one neighbor and is greater than that neighbor), it is a peak and we return its index. Otherwise, we move into the side where a neighbor is greater than the middle element—because that side must contain at least one peak (since the array edges are treated as "negative infinity" beyond the boundaries, there is always a peak). Specifically, if the left neighbor is greater than `mid`, then a peak must exist in the left half; if the right neighbor is greater, then a peak must exist in the right half. This works even for arrays of size 1, where the only element is trivially a peak. If the array has size 0 (though problem likely assumes non-empty, we can guard against it), we return -1. The recursion or iterative loop runs in O(log n) time because each step halves the search space. Space complexity is O(log n) for the recursion stack if implemented recursively, or O(1) if implemented iteratively. Edge cases: `mid-1` or `mid+1` may be out of bounds—these are handled by treating out-of-bounds neighbors as "less than" the current element (effectively -infinity).
