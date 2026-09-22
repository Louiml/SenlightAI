// Write a C++ function named `mountainPeak` that takes a constant reference to a `std::vector<int>` representing a valid mountain array (an array that strictly increases up to a peak index, then strictly decreases), and returns the index of the peak element. The array is guaranteed to have at least 3 elements and satisfy the mountain property. Your function must run in O(log n) time complexity and must not modify the input. Return the index as an integer. If the input were invalid (e.g., not a mountain), you may return -1, but for valid inputs always return the correct peak index.
// The mountain array has a single peak where the value is greater than both its neighbors. We can use binary search to find this peak efficiently. At each step, compute the middle index `mid`. If `arr[mid] > arr[mid+1]` and `arr[mid] > arr[mid-1]`, then `mid` is the peak and return it. Otherwise, compare `arr[mid]` with `arr[mid+1]`: if `arr[mid] > arr[mid+1]`, the peak lies to the left (including `mid`), so set `high = mid-1`. If `arr[mid] < arr[mid+1]`, the peak lies to the right, so set `low = mid+1`. Since the array is a valid mountain, this binary search will always find the peak. Edge cases: the peak could be at index 1 or `size()-2`, but those are handled naturally because we always check neighbors that exist (the array size is ≥ 3). Time complexity is O(log n) and space complexity is O(1).
#include <vector>

/**
 * @brief Returns the index of the peak element in a mountain array.
 *
 * A mountain array strictly increases up to a peak index and then strictly decreases.
 * @param arr A constant reference to the input vector (valid mountain array).
 * @return int The index of the peak element.
 */
int mountainPeak(const std::vector<int>& arr) {
    if (arr.size() < 3) return -1; // Invalid mountain array

    int low = 0;
    int high = arr.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        // Check if mid is the peak (consider boundaries safely)
        if ((mid == 0 || arr[mid] > arr[mid - 1]) &&
            (mid == static_cast<int>(arr.size()) - 1 || arr[mid] > arr[mid + 1])) {
            // But for a valid mountain, the first and last cannot be peak unless size==1
            // Since size>=3, we can safely check neighbors:
            return mid;
        }

        // If mid is not the peak, decide which side to go
        if (arr[mid] < arr[mid + 1]) {
            // Peak is to the right
            low = mid + 1;
        } else {
            // arr[mid] > arr[mid+1] or arr[mid] < arr[mid-1] (but not both greater)
            // Peak is to the left (including mid? Actually go left)
            high = mid - 1;
        }
    }

    return -1; // Should never reach here for valid input
}
#include <cassert>
#include <vector>

int mountainPeak(const std::vector<int>& arr);

int main() {
    std::vector<int> arr1 = {0, 2, 1, 0};
    assert(mountainPeak(arr1) == 1);

    std::vector<int> arr2 = {0, 1, 2, 3, 4, 3, 2, 1};
    assert(mountainPeak(arr2) == 4);

    std::vector<int> arr3 = {1, 3, 2};
    assert(mountainPeak(arr3) == 1);

    std::vector<int> arr4 = {0, 10, 5, 2};
    assert(mountainPeak(arr4) == 1);

    std::vector<int> arr5 = {2, 4, 6, 8, 7, 5, 3};
    assert(mountainPeak(arr5) == 3);

    std::vector<int> arr6 = {3, 4, 5, 4};
    assert(mountainPeak(arr6) == 2);

    std::vector<int> arr7 = {1, 2, 3, 2, 1};
    assert(mountainPeak(arr7) == 2);

    std::vector<int> arr8 = {0, 1, 0};
    assert(mountainPeak(arr8) == 1);

    return 0;
}
