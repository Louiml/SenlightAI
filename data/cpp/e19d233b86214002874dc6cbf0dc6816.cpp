/*
Write a C++ function named `quickSortDescending` that sorts an array of integers in descending order (largest to smallest) using the quicksort algorithm. The function must take a non-const array and its size as parameters, modify the array in place, and return nothing. You must implement the partition step yourself, using the last element of the current subarray as the pivot. The function should handle edge cases such as empty arrays and arrays with duplicate values correctly.
*/
#include <vector>

// Partition the subarray arr[p..r] such that all elements greater than the pivot
// are on the left, and all elements less than or equal to the pivot are on the right.
// Returns the final index of the pivot.
int partitionDescending(int arr[], int p, int r) {
    int pivot = arr[r];
    int i = p - 1;  // index of the last element greater than pivot
    for (int j = p; j < r; ++j) {
        if (arr[j] > pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[r]);
    return i + 1;
}

// Sorts the subarray arr[p..r] in descending order using quicksort.
void quickSortDescendingHelper(int arr[], int p, int r) {
    if (p < r) {
        int q = partitionDescending(arr, p, r);
        quickSortDescendingHelper(arr, p, q - 1);
        quickSortDescendingHelper(arr, q + 1, r);
    }
}

// Public function: sorts the entire array in descending order.
void quickSortDescending(int arr[], int size) {
    if (size <= 1) {
        return;
    }
    quickSortDescendingHelper(arr, 0, size - 1);
}
#include <cassert>
#include <algorithm>

int main() {
    // Test empty array - should not crash
    int emptyArr[0];
    quickSortDescending(emptyArr, 0);

    // Test single element
    int singleArr[] = {42};
    quickSortDescending(singleArr, 1);
    assert(singleArr[0] == 42);

    // Test already descending order
    int descArr[] = {5, 4, 3, 2, 1};
    quickSortDescending(descArr, 5);
    assert(descArr[0] == 5 && descArr[1] == 4 && descArr[2] == 3 && descArr[3] == 2 && descArr[4] == 1);

    // Test ascending order becomes descending
    int ascArr[] = {1, 2, 3, 4, 5};
    quickSortDescending(ascArr, 5);
    assert(ascArr[0] == 5 && ascArr[1] == 4 && ascArr[2] == 3 && ascArr[3] == 2 && ascArr[4] == 1);

    // Test with duplicates
    int dupArr[] = {3, 1, 3, 2, 1};
    quickSortDescending(dupArr, 5);
    assert(dupArr[0] == 3 && dupArr[1] == 3 && dupArr[2] == 2 && dupArr[3] == 1 && dupArr[4] == 1);

    // Test random order against std::sort with reverse
    int randArr[] = {7, -2, 9, 4, -5, 0, 3};
    int expected[] = {7, -2, 9, 4, -5, 0, 3};
    std::sort(expected, expected + 7, std::greater<int>());
    quickSortDescending(randArr, 7);
    for (int i = 0; i < 7; ++i) {
        assert(randArr[i] == expected[i]);
    }

    // Test negative numbers
    int negArr[] = {-10, -1, -5, -3};
    quickSortDescending(negArr, 4);
    assert(negArr[0] == -1 && negArr[1] == -3 && negArr[2] == -5 && negArr[3] == -10);

    // Test all zeros
    int zeroArr[] = {0, 0, 0};
    quickSortDescending(zeroArr, 3);
    assert(zeroArr[0] == 0 && zeroArr[1] == 0 && zeroArr[2] == 0);

    return 0;
}
// The solution uses the standard quicksort algorithm with a twist: the partition logic is reversed so that elements greater than the pivot are placed on the left side, and smaller elements on the right. In the `partitionDescending` helper, we scan the subarray from the left boundary to just before the pivot, and whenever we encounter an element strictly greater than the pivot, we swap it to the left side. After the scan, the pivot is swapped into the correct position, separating larger left elements from smaller right elements. The recursive `quickSortDescending` function then sorts the left and right subarrays. Edge cases: an empty or single-element array is already sorted, and the recursive base case handles this by checking if `p < r`. Duplicate values are handled because they are placed on the right side (not greater than pivot), preserving a stable partition. Time complexity is average-case O(n log n) and worst-case O(n²) for unbalanced partitions; space complexity is O(log n) for the recursion stack in the average case.
