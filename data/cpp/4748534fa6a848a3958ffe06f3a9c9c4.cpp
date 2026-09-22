Given an integer array and its logical size, write a C++ function that sorts the array in ascending order using the Lomuto or Hoare partition-based quicksort algorithm. The function should take the array, the starting index, and the ending index as parameters, and modify the array in place. Your implementation must correctly handle duplicate values, single-element subarrays, and the case where the pivot is the smallest or largest element. Ensure the pivot selection is deterministic (e.g., the first element of each subarray). Provide a free function named `quickSort` that performs the sort recursively.
The core idea is to use a recursive divide-and-conquer approach. Choose the first element of the current subarray as the pivot. Use two pointers: one scanning from the left (`start`) and one from the right (`end`). Move `start` rightward while the element at `start` is less than or equal to the pivot, and move `end` leftward while the element at `end` is greater than the pivot. When both pointers stop and `start < end`, swap the elements. Continue until `start >= end`. Then swap the pivot (at the original left boundary) with the element at `end`, placing the pivot in its final sorted position. Recursively sort the left subarray (before the pivot) and the right subarray (after the pivot). Edge cases: if the subarray has one element or is empty, do nothing. When duplicates exist, using `<=` on the left scan and `>` on the right scan prevents infinite loops and keeps equal elements on both sides of the pivot. Time complexity is O(n log n) on average and O(n²) in the worst case (e.g., already sorted input with this pivot choice); space complexity is O(log n) due to recursion stack.
#include <utility> // for std::swap

// Partition function using first element as pivot.
// Returns the final index of the pivot.
int partition(int arr[], int low, int high) {
    int pivot = arr[low];
    int start = low;
    int end = high;

    while (start < end) {
        // Move start right while element <= pivot (handles duplicates safely)
        while (start < end && arr[start] <= pivot) {
            ++start;
        }
        // Move end left while element > pivot
        while (arr[end] > pivot) {
            --end;
        }
        if (start < end) {
            std::swap(arr[start], arr[end]);
        }
    }
    // Place pivot at its correct position
    std::swap(arr[low], arr[end]);
    return end;
}

// Recursive quicksort function.
// Sorts the subarray from index low to high (inclusive).
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pivotIdx = partition(arr, low, high);
        quickSort(arr, low, pivotIdx - 1);
        quickSort(arr, pivotIdx + 1, high);
    }
}
#include <cassert>

// Test cases for quickSort
int main() {
    // Basic case with distinct elements
    int arr1[] = {5, 2, 9, 1, 7};
    quickSort(arr1, 0, 4);
    assert(arr1[0] == 1 && arr1[1] == 2 && arr1[2] == 5 && arr1[3] == 7 && arr1[4] == 9);

    // Case with duplicates
    int arr2[] = {3, 1, 3, 3, 2};
    quickSort(arr2, 0, 4);
    assert(arr2[0] == 1 && arr2[1] == 2 && arr2[2] == 3 && arr2[3] == 3 && arr2[4] == 3);

    // Already sorted array
    int arr3[] = {1, 2, 3, 4, 5};
    quickSort(arr3, 0, 4);
    assert(arr3[0] == 1 && arr3[1] == 2 && arr3[2] == 3 && arr3[3] == 4 && arr3[4] == 5);

    // Reverse sorted array
    int arr4[] = {9, 7, 5, 3, 1};
    quickSort(arr4, 0, 4);
    assert(arr4[0] == 1 && arr4[1] == 3 && arr4[2] == 5 && arr4[3] == 7 && arr4[4] == 9);

    // Single element
    int arr5[] = {42};
    quickSort(arr5, 0, 0);
    assert(arr5[0] == 42);

    // Two elements (swap needed)
    int arr6[] = {2, 1};
    quickSort(arr6, 0, 1);
    assert(arr6[0] == 1 && arr6[1] == 2);

    // All identical elements
    int arr7[] = {7, 7, 7, 7};
    quickSort(arr7, 0, 3);
    assert(arr7[0] == 7 && arr7[1] == 7 && arr7[2] == 7 && arr7[3] == 7);

    // Negative numbers
    int arr8[] = {-3, -1, -2, 0};
    quickSort(arr8, 0, 3);
    assert(arr8[0] == -3 && arr8[1] == -2 && arr8[2] == -1 && arr8[3] == 0);

    // Mixed with many duplicates
    int arr9[] = {2, 1, 2, 1, 2, 1};
    quickSort(arr9, 0, 5);
    assert(arr9[0] == 1 && arr9[1] == 1 && arr9[2] == 1 && arr9[3] == 2 && arr9[4] == 2 && arr9[5] == 2);

    return 0;
}
