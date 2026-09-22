// Write a C++ function named `mergeSortArray` that takes an array of integers, its starting index `left`, and its ending index `right`, and sorts the subarray `arr[left..right]` in non-decreasing order using the classic recursive merge sort algorithm. The function must be stable (preserving relative order of equal elements), must not use any external libraries beyond the standard C++ headers (`<vector>`, `<algorithm>`, etc.), and must operate in-place on the given array in the sense that the original array is modified directly, though it may use temporary auxiliary arrays inside the merge step. The sorting must be deterministic and correct for arrays with zero, one, or many elements, including negative values and duplicates. The function should have `void` return type and be declared with `const`-correct parameters (the array itself is mutable, but index parameters are `const int` or passed by value). You may assume that `left <= right` and that both indices are valid for the array. Provide only the function implementation without a `main` function; the solution will be tested by calling this function on various arrays.

#include <cassert>
#include <iostream>
#include <vector>

// Declaration of the function to test
void mergeSortArray(int arr[], const int left, const int right);

int main() {
    // Test 1: Already sorted array
    int arr1[] = {1, 2, 3, 4, 5};
    mergeSortArray(arr1, 0, 4);
    for (int i = 0; i < 5; ++i) assert(arr1[i] == i + 1);

    // Test 2: Reverse sorted array
    int arr2[] = {5, 4, 3, 2, 1};
    mergeSortArray(arr2, 0, 4);
    for (int i = 0; i < 5; ++i) assert(arr2[i] == i + 1);

    // Test 3: Array with duplicates
    int arr3[] = {3, 1, 2, 3, 1};
    mergeSortArray(arr3, 0, 4);
    int expected3[] = {1, 1, 2, 3, 3};
    for (int i = 0; i < 5; ++i) assert(arr3[i] == expected3[i]);

    // Test 4: Single element array
    int arr4[] = {42};
    mergeSortArray(arr4, 0, 0);
    assert(arr4[0] == 42);

    // Test 5: Array with negative numbers
    int arr5[] = {-3, -1, -7, -2};
    mergeSortArray(arr5, 0, 3);
    int expected5[] = {-7, -3, -2, -1};
    for (int i = 0; i < 4; ++i) assert(arr5[i] == expected5[i]);

    // Test 6: Large array with random values (simple check)
    int arr6[] = {100, -50, 0, 75, -100, 25};
    mergeSortArray(arr6, 0, 5);
    int expected6[] = {-100, -50, 0, 25, 75, 100};
    for (int i = 0; i < 6; ++i) assert(arr6[i] == expected6[i]);

    // Test 7: Even number of elements with all equal
    int arr7[] = {7, 7, 7, 7};
    mergeSortArray(arr7, 0, 3);
    for (int i = 0; i < 4; ++i) assert(arr7[i] == 7);

    // Test 8: Empty subarray (should not be called per spec, but test with left > right)
    // Not applicable because function assumes left <= right, so skip.

    // Test 9: Two elements unsorted
    int arr9[] = {2, 1};
    mergeSortArray(arr9, 0, 1);
    assert(arr9[0] == 1 && arr9[1] == 2);

    // Test 10: Mixed positive and negative large numbers
    int arr10[] = {1000, -1000, 500, -500, 0};
    mergeSortArray(arr10, 0, 4);
    int expected10[] = {-1000, -500, 0, 500, 1000};
    for (int i = 0; i < 5; ++i) assert(arr10[i] == expected10[i]);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <vector>
#include <algorithm>

// Merge two sorted halves of arr[left..mid] and arr[mid+1..right] into arr[left..right]
void mergeHalves(int arr[], int left, int mid, int right) {
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    // Temporary arrays to hold the halves
    std::vector<int> leftArr(leftSize);
    std::vector<int> rightArr(rightSize);

    for (int i = 0; i < leftSize; ++i) {
        leftArr[i] = arr[left + i];
    }
    for (int j = 0; j < rightSize; ++j) {
        rightArr[j] = arr[mid + 1 + j];
    }

    int i = 0; // index for leftArr
    int j = 0; // index for rightArr
    int k = left; // index for original array

    // Merge the two halves back into arr
    while (i < leftSize && j < rightSize) {
        // Use <= for stability: equal elements from left are placed first
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            ++i;
        } else {
            arr[k] = rightArr[j];
            ++j;
        }
        ++k;
    }

    // Copy any remaining elements from leftArr
    while (i < leftSize) {
        arr[k] = leftArr[i];
        ++i;
        ++k;
    }

    // Copy any remaining elements from rightArr
    while (j < rightSize) {
        arr[k] = rightArr[j];
        ++j;
        ++k;
    }
}

// Recursively sort the subarray arr[left..right] using merge sort
void mergeSortArray(int arr[], const int left, const int right) {
    if (left >= right) {
        return; // Base case: zero or one element already sorted
    }

    int mid = left + (right - left) / 2; // Avoid potential overflow

    // Sort left and right halves
    mergeSortArray(arr, left, mid);
    mergeSortArray(arr, mid + 1, right);

    // Merge the sorted halves
    mergeHalves(arr, left, mid, right);
}

// The merge sort algorithm follows a divide-and-conquer strategy. The main approach is: if the subarray has more than one element (`left < right`), compute the midpoint `mid = (left + right) / 2`, recursively sort the left half `[left..mid]` and the right half `[mid+1..right]`, then merge the two sorted halves into a single sorted sequence. For the merge step, create two temporary arrays that copy the elements from the left and right halves. Then iterate through both temporary arrays, comparing their first elements and placing the smaller (or equal, to ensure stability) element back into the original array at the appropriate position. After one array is exhausted, copy the remaining elements from the other array. Edge cases include empty or single-element subarrays, where no action is taken (base case). Duplicate values are handled by using `<=` in the comparison so that elements from the left half are placed before equal elements from the right half, preserving stability. Time complexity is \(O(n \log n)\) for all cases (best, average, worst), and space complexity is \(O(n)\) due to the temporary arrays used during the merge (auxiliary space), not counting the recursion stack depth of \(O(\log n)\).
