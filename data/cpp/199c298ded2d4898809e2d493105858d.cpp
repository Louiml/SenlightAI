// Write a C++ function named `findPeakIndex` that takes a `const std::vector<int>&` representing a mountain array (an array that strictly increases to a peak element and then strictly decreases) and returns the index of the peak element. The function must use a binary search approach to achieve O(log n) time complexity. The input is guaranteed to be a valid mountain array with at least 3 elements, all integers can be positive, negative, or zero, and the peak is never at the first or last index. The function should return the index (as an `int`) of the element that is greater than its immediate neighbors.

#include <cassert>
#include <vector>

int main() {
    // Basic mountain arrays
    std::vector<int> arr1 = {0, 1, 0};
    assert(findPeakIndex(arr1) == 1);

    std::vector<int> arr2 = {0, 2, 1, 0};
    assert(findPeakIndex(arr2) == 1);

    std::vector<int> arr3 = {0, 10, 5, 2};
    assert(findPeakIndex(arr3) == 1);

    // Negative numbers and larger peak
    std::vector<int> arr4 = {-10, -5, 3, -1, -20};
    assert(findPeakIndex(arr4) == 2);

    // Peak near the end but not at the last index
    std::vector<int> arr5 = {1, 2, 3, 4, 5, 6, 7, 6, 5};
    assert(findPeakIndex(arr5) == 6);

    // Peak near the start but not at the first index
    std::vector<int> arr6 = {1, 100, 99, 98, 97};
    assert(findPeakIndex(arr6) == 1);

    // All decreasing after a single increase (peak is index 1)
    std::vector<int> arr7 = {5, 10, 9, 8};
    assert(findPeakIndex(arr7) == 1);

    // Long array with a single peak
    std::vector<int> arr8 = {1, 3, 5, 7, 9, 8, 6, 4, 2, 0, -1};
    assert(findPeakIndex(arr8) == 4);

    // Peak with even length
    std::vector<int> arr9 = {2, 4, 6, 8, 7, 5, 3};
    assert(findPeakIndex(arr9) == 3);

    // Duplicate-like values? Not possible per spec, but test a case with all increasing then a drop
    std::vector<int> arr10 = {0, 2, 4, 6};
    // This is not a valid mountain array because it has no strict decrease after peak,
    // but if we call it anyway, the function will return the last index (bug).
    // So we skip this test. Instead test a valid one with 0 and negatives.
    std::vector<int> arr11 = {-3, -2, -1, -2};
    assert(findPeakIndex(arr11) == 2);

    return 0;
}

#include <vector>

// Given a mountain array (strictly increases then strictly decreases),
// return the index of the peak element using binary search.
// The peak element is greater than its neighbors.
int findPeakIndex(const std::vector<int>& arr) {
    int left = 0;
    int right = arr.size() - 1;

    while (left < right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] < arr[mid + 1]) {
            // On the increasing slope, move right
            left = mid + 1;
        } else {
            // On the decreasing slope or at the peak, move left boundary to mid
            right = mid;
        }
    }
    // left and right converge to the peak index
    return left;
}

// The solution uses binary search on the index space. We maintain two pointers `left` (initially 0) and `right` (initially `arr.size()-1`). In each iteration, we compute the mid index and compare `arr[mid]` with `arr[mid+1]`. If `arr[mid] < arr[mid+1]`, this means we are on the increasing slope, so the peak must be to the right, hence we move `left = mid + 1`. Otherwise, if `arr[mid] >= arr[mid+1]`, we are on a decreasing slope or at the peak, so we move `right = mid`. The loop continues while `left < right`; when they converge, `left` (or `right`) is the peak index. Important edge cases: the peak cannot be at the boundaries since the array strictly increases then decreases, so the binary search will always converge correctly. The algorithm runs in O(log n) time and uses O(1) auxiliary space. No special handling for duplicate values is needed because the mountain array is strictly increasing/decreasing, so adjacent elements are never equal.
