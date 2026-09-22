// Write a standalone C++ function named `partitionAndSort` that takes a dynamically allocated array of integers along with its size, sorts the array in ascending order using the quicksort algorithm, and returns nothing (modifies the array in place). The function must implement quicksort recursively with an in-place partitioning scheme. You may use any partitioning strategy (e.g., Lomuto or Hoare) but you must not use any standard library sorting functions. The input array may contain duplicate values, negative numbers, and a size of 0 or 1, which should be handled gracefully. After the call, the array must be fully sorted.

The solution implements classic recursive quicksort. The core idea is to select a pivot (here, the first element of the current subarray), partition the subarray so that all elements less than the pivot come before it and all elements greater come after it, placing the pivot in its final sorted position, then recursively sort the left and right partitions. The provided snippet uses a two-phase approach: first, it counts how many elements are smaller than the pivot to find its correct index, swaps the pivot there, then scans the left side for elements greater than the pivot and swaps them with elements on the right side that are smaller. This is correct but less efficient than standard Hoare/Lomuto partitioning. For the reference solution, I will use the Lomuto partition scheme (commonly taught and reliable) because it is simpler and clearer. Edge cases: if `size` is 0 or 1, the function returns immediately. Duplicate values are handled naturally; they may end up on either side of the pivot but are reordered correctly after recursive calls. The time complexity is O(n log n) on average and O(n^2) in the worst case (e.g., already sorted array with a poor pivot choice), but for a teaching task this is acceptable. Space complexity is O(log n) due to the recursion stack, on average.

#include <algorithm>
#include <cstddef>

// Sorts the given array of integers in ascending order using recursive quicksort.
// The array is modified in place. The partition step uses the Lomuto scheme.
void partitionAndSort(int* arr, std::size_t size) {
    // Helper function to recursively sort a subarray [low, high] inclusive.
    auto quickSortRange = [&](auto&& self, int low, int high) -> void {
        if (low >= high) {
            return; // Base case: 0 or 1 element.
        }
        // Lomuto partitioning: choose pivot as the last element.
        int pivot = arr[high];
        int i = low - 1; // Index of the last element less than or equal to pivot.
        for (int j = low; j < high; ++j) {
            if (arr[j] <= pivot) {
                ++i;
                std::swap(arr[i], arr[j]);
            }
        }
        // Place pivot in its correct position.
        std::swap(arr[i + 1], arr[high]);
        int pivotIndex = i + 1;

        // Recursively sort left and right partitions.
        self(self, low, pivotIndex - 1);
        self(self, pivotIndex + 1, high);
    };

    if (size <= 1) {
        return;
    }
    quickSortRange(quickSortRange, 0, static_cast<int>(size) - 1);
}

#include <cassert>

// The solution function is declared here for the test.
void partitionAndSort(int* arr, std::size_t size);

int main() {
    // Test 1: basic array.
    int arr1[] = {1, 6, 4, 5};
    partitionAndSort(arr1, 4);
    assert(arr1[0] == 1 && arr1[1] == 4 && arr1[2] == 5 && arr1[3] == 6);

    // Test 2: already sorted.
    int arr2[] = {1, 2, 3, 4};
    partitionAndSort(arr2, 4);
    assert(arr2[0] == 1 && arr2[1] == 2 && arr2[2] == 3 && arr2[3] == 4);

    // Test 3: reverse sorted.
    int arr3[] = {5, 4, 3, 2, 1};
    partitionAndSort(arr3, 5);
    assert(arr3[0] == 1 && arr3[1] == 2 && arr3[2] == 3 && arr3[3] == 4 && arr3[4] == 5);

    // Test 4: duplicates.
    int arr4[] = {3, 1, 3, 2, 1, 3};
    partitionAndSort(arr4, 6);
    assert(arr4[0] == 1 && arr4[1] == 1 && arr4[2] == 2 && arr4[3] == 3 && arr4[4] == 3 && arr4[5] == 3);

    // Test 5: single element.
    int arr5[] = {42};
    partitionAndSort(arr5, 1);
    assert(arr5[0] == 42);

    // Test 6: empty array (size 0).
    int* arr6 = nullptr;
    partitionAndSort(arr6, 0); // Should do nothing.

    // Test 7: negative numbers and zero.
    int arr7[] = {0, -5, 3, -1, 2, -4};
    partitionAndSort(arr7, 6);
    assert(arr7[0] == -5 && arr7[1] == -4 && arr7[2] == -1 && arr7[3] == 0 && arr7[4] == 2 && arr7[5] == 3);

    // Test 8: large size (100 elements) check sorted property.
    int arr8[100];
    for (int i = 0; i < 100; ++i) {
        arr8[i] = (i * 37) % 101; // Deterministic pseudo-random.
    }
    partitionAndSort(arr8, 100);
    bool isSorted = true;
    for (int i = 1; i < 100; ++i) {
        if (arr8[i] < arr8[i - 1]) {
            isSorted = false;
            break;
        }
    }
    assert(isSorted);

    return 0;
}
