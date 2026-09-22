Write a C++ function `int findMedianAfterPartition(int arr[], int n)` that takes an array of `n` distinct integers and returns the median value (the element that would be at index `n/2` after sorting) by using exactly **one** call to the provided partition logic. You must not use the full quicksort recursion or sort the entire array; instead, use the partition step repeatedly to narrow down to the median position (similar to quickselect). The function should handle arrays of odd length only (n is odd), and the input array must contain distinct integers. For example, given `{10, 7, 8, 9, 1, 5, 6}`, the median is `7` because after sorting it becomes `{1, 5, 6, 7, 8, 9, 10}` and the middle index is `3` (0-based). Do not modify the original array—operate on a copy to preserve the input.

#include <cassert>
#include <iostream>

int main() {
    // Test with the example from the snippet but odd length
    int arr1[] = {10, 7, 8, 9, 1, 5, 6};
    assert(findMedianAfterPartition(arr1, 7) == 7); // sorted: 1,5,6,7,8,9,10

    // Single element
    int arr2[] = {42};
    assert(findMedianAfterPartition(arr2, 1) == 42);

    // Already sorted odd array
    int arr3[] = {1, 2, 3, 4, 5};
    assert(findMedianAfterPartition(arr3, 5) == 3);

    // Reverse sorted odd array
    int arr4[] = {9, 7, 5, 3, 1};
    assert(findMedianAfterPartition(arr4, 5) == 5);

    // Larger odd array with distinct numbers
    int arr5[] = {100, 20, 50, 80, 10, 30, 90, 40, 60, 70, 110};
    assert(findMedianAfterPartition(arr5, 11) == 60);
    // Verify original array unchanged
    int expected5[] = {100, 20, 50, 80, 10, 30, 90, 40, 60, 70, 110};
    for (int i = 0; i < 11; i++) assert(arr5[i] == expected5[i]);

    // Mixed values with negatives
    int arr6[] = {-5, -1, -10, 0, 3};
    assert(findMedianAfterPartition(arr6, 5) == -1); // sorted: -10,-5,-1,0,3

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <vector>
#include <cstring>

// Utility function to swap two integers (from the snippet)
void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// Partition function as in the snippet: uses last element as pivot
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Finds the median (n/2-th smallest element, 0-based) using quickselect
// Returns the median value. Assumes n is odd and all elements are distinct.
// Does not modify the input array; works on a copy.
int findMedianAfterPartition(int arr[], int n) {
    // Create a copy of the input array
    std::vector<int> copyArr(arr, arr + n);
    int* a = copyArr.data();

    int low = 0;
    int high = n - 1;
    int k = n / 2; // median index for odd n

    while (low <= high) {
        int pi = partition(a, low, high);
        if (pi == k) {
            return a[pi];
        } else if (pi < k) {
            low = pi + 1;
        } else {
            high = pi - 1;
        }
    }
    // Should never reach here for valid input
    return a[k]; // fallback
}

// The solution uses the quickselect algorithm, which is a divide-and-conquer approach that mimics quicksort but only recurses into the side of the partition that contains the desired median index. We copy the input array so we can modify the copy without affecting the caller. We maintain a low index (0) and high index (n-1). In a loop, we call the `partition` function (as given in the snippet) on the copy, which selects the last element as pivot, places the pivot at its correct sorted position, and returns that index `pi`. If `pi` equals the target median index `k = n/2`, we return `arrCopy[pi]`. If `pi < k`, we search the right half by setting `low = pi + 1`; if `pi > k`, we search the left half by setting `high = pi - 1`. Because the array contains distinct integers and n is odd, the median index is unique. Edge cases: n=1 (median is the single element), and we must ensure we copy the array properly using `memcpy` or a loop. Time complexity: average O(n) since each partition step reduces the problem size roughly by half, but worst-case O(n^2) if the pivot is always the smallest or largest element (unlikely with arbitrary inputs). Space complexity: O(n) for the copy, plus O(1) auxiliary for the quickselect loop (no recursion stack, we use iteration).
