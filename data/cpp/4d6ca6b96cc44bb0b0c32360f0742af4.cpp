/*
Write a C++ function named `quickSortDescending` that takes a mutable array of integers and its size, and sorts the array in descending order using a recursive quick sort algorithm. The function must operate in-place, preserve duplicate values, and handle edge cases such as empty arrays, arrays with a single element, and arrays containing negative numbers. Do not use any standard sorting library functions; implement the partitioning and recursion manually.
*/
#include <vector>

// Partition the subarray data[p..r] for descending order using data[p] as pivot.
// Returns the index q such that all elements in data[p..q] are >= data[q+1..r].
int partitionDescending(int data[], int p, int r) {
    int pivot = data[p];
    int i = p;
    int j = r;
    while (true) {
        // Move j left while element is greater than pivot (keeps larger elements on right? Actually for descending, we want larger on left, so adjust logic)
        // Correct descending partition: elements from p..q are >= elements from q+1..r
        while (data[j] < pivot) {  // For descending, we need to move j left while data[j] is smaller than pivot
            j--;
        }
        while (data[i] > pivot) {  // Move i right while data[i] is larger than pivot
            i++;
        }
        if (i < j) {
            std::swap(data[i], data[j]);
        } else {
            return j;
        }
    }
}

// Recursive quick sort for descending order.
void quickSortDescending(int data[], int p, int r) {
    if (p < r) {
        int q = partitionDescending(data, p, r + 1); // r+1 to include index r in the partition range
        quickSortDescending(data, p, q);
        quickSortDescending(data, q + 1, r);
    }
}

// Public function to sort an array of integers in descending order.
void quickSortDescending(int data[], int n) {
    if (n <= 1) return;
    quickSortDescending(data, 0, n - 1);
}
#include <cassert>
#include <algorithm>

// The solution function is declared in the header (or here for testing).
void quickSortDescending(int data[], int n);

int main() {
    // Test 1: Standard unsorted array
    int arr1[] = {3, -1, 4, -5, 2};
    int expected1[] = {4, 3, 2, -1, -5};
    quickSortDescending(arr1, 5);
    for (int i = 0; i < 5; i++) assert(arr1[i] == expected1[i]);

    // Test 2: Already descending
    int arr2[] = {9, 5, 0, -2};
    int expected2[] = {9, 5, 0, -2};
    quickSortDescending(arr2, 4);
    for (int i = 0; i < 4; i++) assert(arr2[i] == expected2[i]);

    // Test 3: All duplicates
    int arr3[] = {7, 7, 7, 7};
    quickSortDescending(arr3, 4);
    for (int i = 0; i < 4; i++) assert(arr3[i] == 7);

    // Test 4: Single element
    int arr4[] = {42};
    quickSortDescending(arr4, 1);
    assert(arr4[0] == 42);

    // Test 5: Two elements out of order
    int arr5[] = {1, 2};
    int expected5[] = {2, 1};
    quickSortDescending(arr5, 2);
    assert(arr5[0] == 2 && arr5[1] == 1);

    // Test 6: Mixed large and small negatives
    int arr6[] = {-1, -100, -5, -20};
    int expected6[] = {-1, -5, -20, -100};
    quickSortDescending(arr6, 4);
    for (int i = 0; i < 4; i++) assert(arr6[i] == expected6[i]);

    // Test 7: Larger array with random pattern
    int arr7[] = {5, -3, 10, 0, 8, -7, 2};
    int expected7[] = {10, 8, 5, 2, 0, -3, -7};
    quickSortDescending(arr7, 7);
    for (int i = 0; i < 7; i++) assert(arr7[i] == expected7[i]);

    return 0;
}
// The solution implements a recursive quick sort with a custom partitioning scheme adapted from the provided snippet. The partitioning function `partition` selects the first element of the current subarray as the pivot. It then uses two indices, `i` starting at the left boundary and `j` starting at the right boundary, moving `j` leftward while the element at `j` is greater than the pivot (for descending order), and moving `i` rightward while the element at `i` is less than the pivot. When both indices stop, if `i` is still less than `j`, the elements at those positions are swapped, and the process repeats. If `i` is not less than `j`, the function returns `j` as the partition index. The recursion then sorts subarrays `[p, q]` and `[q+1, r]`. Edge cases: empty and single-element arrays are trivially sorted and require no action; duplicate values are handled naturally because the comparisons use strict inequalities, leaving equal elements in place until swaps resolve order. The time complexity averages \(O(n \log n)\) and worst-case \(O(n^2)\) when the pivot is consistently the smallest or largest element. Space complexity is \(O(\log n)\) due to recursion stack depth in the average case, and \(O(n)\) in the worst case.
