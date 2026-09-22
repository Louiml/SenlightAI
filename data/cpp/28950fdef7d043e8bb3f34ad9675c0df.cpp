// Write a C++ function `searchInRotatedSortedArray` that takes a pointer to a sorted array of integers that has been rotated at some unknown pivot, the array size, and a target key. The function must return the index of the key if present, otherwise return -1. The array contains distinct integers. Design the algorithm to work in logarithmic time without using any additional memory. The function should be `const`-correct where appropriate.
The array is sorted and then rotated, meaning it consists of two sorted subarrays meeting at a pivot. Binary search can still be applied by determining which half is sorted at each step. At each midpoint, compare `arr[low]` with `arr[mid]`. If `arr[low] <= arr[mid]`, the left half from `low` to `mid` is sorted. Then check if the key lies within that sorted range; if so, search there, otherwise search the right half. If `arr[low] > arr[mid]`, the right half from `mid` to `high` is sorted. Similarly, check if the key lies in that range. This approach correctly narrows the search space by half each iteration. Edge cases include empty array (return -1), single element, key equal to pivot, and key not present. Time complexity is O(log n) with O(1) auxiliary space.
#include <cstddef> // for size_t

// Search for key in a rotated sorted array of distinct integers.
// Returns the index of key if found, otherwise -1.
int searchInRotatedSortedArray(const int* arr, int n, int key) {
    if (n <= 0 || arr == nullptr) {
        return -1;
    }
    int low = 0;
    int high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == key) {
            return mid;
        }
        // Left half is sorted
        if (arr[low] <= arr[mid]) {
            if (key >= arr[low] && key < arr[mid]) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        // Right half is sorted
        else {
            if (key > arr[mid] && key <= arr[high]) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }
    return -1;
}
#include <cassert>

int main() {
    int arr1[] = {4, 5, 6, 7, 0, 1, 2};
    assert(searchInRotatedSortedArray(arr1, 7, 0) == 4);
    assert(searchInRotatedSortedArray(arr1, 7, 3) == -1);
    assert(searchInRotatedSortedArray(arr1, 7, 4) == 0);
    assert(searchInRotatedSortedArray(arr1, 7, 2) == 6);

    int arr2[] = {1};
    assert(searchInRotatedSortedArray(arr2, 1, 1) == 0);
    assert(searchInRotatedSortedArray(arr2, 1, 2) == -1);

    int arr3[] = {5, 1, 2, 3, 4};
    assert(searchInRotatedSortedArray(arr3, 5, 5) == 0);
    assert(searchInRotatedSortedArray(arr3, 5, 1) == 1);
    assert(searchInRotatedSortedArray(arr3, 5, 4) == 4);

    int arr4[] = {2, 3, 4, 5, 6, 7, 8, 1};
    assert(searchInRotatedSortedArray(arr4, 8, 8) == 6);
    assert(searchInRotatedSortedArray(arr4, 8, 1) == 7);

    // Edge case: empty array and null pointer
    assert(searchInRotatedSortedArray(nullptr, 0, 5) == -1);
    int arr5[] = {10, 20, 30, 40, 50};
    assert(searchInRotatedSortedArray(arr5, 5, 30) == 2);
    assert(searchInRotatedSortedArray(arr5, 5, 10) == 0);
    assert(searchInRotatedSortedArray(arr5, 5, 50) == 4);
    return 0;
}
