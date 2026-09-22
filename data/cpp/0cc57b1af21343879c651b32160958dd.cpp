// Write a C++ function named `quickSortArray` that sorts an integer array in ascending order using the classic quicksort algorithm with the first element of the current subarray as the pivot. The function should take three parameters: a pointer to the first element of the array, the starting index (low), and the ending index (high) of the subarray to be sorted. The function must modify the array in place and return `void`. Your implementation must handle arrays of any size (including empty arrays where `high < low`), and must correctly sort duplicate values. Do not use any standard library sorting functions such as `std::sort`; you must implement the partitioning and recursion yourself. The function should be robust regardless of initial array order (sorted, reversed, random, or all identical).
#include <cassert>
#include <algorithm>

// The solution functions are assumed to be defined above this point.

int main() {
    // Test case 1: Normal random order
    int arr1[] = {3, 1, 6, 5, 9, 2, 8};
    quickSortArray(arr1, 0, 6);
    assert(std::is_sorted(arr1, arr1 + 7));

    // Test case 2: Already sorted ascending
    int arr2[] = {1, 2, 3, 4, 5};
    quickSortArray(arr2, 0, 4);
    assert(std::is_sorted(arr2, arr2 + 5));

    // Test case 3: Reverse sorted descending
    int arr3[] = {9, 8, 7, 6, 1};
    quickSortArray(arr3, 0, 4);
    assert(std::is_sorted(arr3, arr3 + 5));

    // Test case 4: All elements identical
    int arr4[] = {7, 7, 7, 7};
    quickSortArray(arr4, 0, 3);
    assert(std::is_sorted(arr4, arr4 + 4));

    // Test case 5: Single element
    int arr5[] = {42};
    quickSortArray(arr5, 0, 0);
    assert(arr5[0] == 42);

    // Test case 6: Two elements in descending order
    int arr6[] = {5, 1};
    quickSortArray(arr6, 0, 1);
    assert(arr6[0] == 1 && arr6[1] == 5);

    // Test case 7: Duplicates mixed with unique values
    int arr7[] = {3, 1, 3, 2, 1, 4, 3};
    quickSortArray(arr7, 0, 6);
    assert(std::is_sorted(arr7, arr7 + 7));

    // Test case 8: Larger array with negatives
    int arr8[] = {-5, 0, -2, 10, -1, 7};
    quickSortArray(arr8, 0, 5);
    assert(std::is_sorted(arr8, arr8 + 6));

    // Test case 9: Empty subarray call (should not crash, no effect)
    int arr9[] = {2, 1};
    quickSortArray(arr9, 0, -1);  // low > high, does nothing
    assert(arr9[0] == 2 && arr9[1] == 1);

    // Test case 10: Performance sanity check with 100 elements random
    int arr10[100];
    for (int i = 0; i < 100; ++i) arr10[i] = (i * 37) % 101; // deterministic pseudo-random
    quickSortArray(arr10, 0, 99);
    assert(std::is_sorted(arr10, arr10 + 100));

    return 0;
}
#include <utility>   // for std::swap
#include <cstddef>   // for size_t (optional, but good practice)

// Partition the subarray arr[low..high] using arr[low] as pivot.
// Returns the final index of the pivot after partitioning.
int partition(int arr[], int low, int high) {
    int pivot = arr[low];
    int i = low + 1;
    int j = high;

    while (true) {
        // Move i right while element is <= pivot and within bounds.
        while (i <= high && arr[i] <= pivot) {
            ++i;
        }
        // Move j left while element is >= pivot and within bounds.
        while (j >= low + 1 && arr[j] >= pivot) {
            --j;
        }
        // If they crossed, j is the correct spot for the pivot.
        if (i >= j) {
            break;
        }
        // Swap out-of-place elements.
        std::swap(arr[i], arr[j]);
    }
    // Place pivot in its final position.
    std::swap(arr[low], arr[j]);
    return j;
}

// Recursively sort the subarray arr[low..high] using quicksort.
void quickSortArray(int arr[], int low, int high) {
    if (low < high) {
        int partitionIndex = partition(arr, low, high);
        quickSortArray(arr, low, partitionIndex - 1);
        quickSortArray(arr, partitionIndex + 1, high);
    }
}
// The solution uses the standard quicksort divide-and-conquer approach. The core is a partition function that selects the first element as the pivot (`arr[low]`). Two indices, `i` (starting at `low+1`) and `j` (starting at `high`), move toward each other: `i` moves right while it points to an element ≤ pivot (but must stop when exceeding `high`), and `j` moves left while it points to an element ≥ pivot (but must stop when reaching `low`). When `i` and `j` cross (i.e., `i >= j`), `j` is the correct final position for the pivot, so swap `arr[j]` with `arr[low]` and return `j`. Otherwise, swap the elements at `i` and `j` to place smaller-than-pivot values on the left and larger-than-pivot values on the right, then continue. Edge case: when the pivot is the smallest or largest element, `i` or `j` may run off the subarray bounds; the inner while loops guard with `i<=high` and `j>=low+1`. For arrays of length 0 or 1 (low>=high), recursion stops. After partitioning, recursively sort the left subarray `[low, partition-1]` and right subarray `[partition+1, high]`. Time complexity averages `O(n log n)` and worst-case `O(n^2)` (e.g., when the array is already sorted and pivot is the first element). Space complexity is `O(log n)` average for the recursion stack, but `O(n)` worst-case; however, the in-place algorithm uses no extra array storage.
