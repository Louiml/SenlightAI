Write a C++ function `mergeSortAndGetSortedArray` that takes a vector of integers by reference, sorts it in ascending order using the merge sort algorithm, and returns nothing (modifying the input vector in place). The function must be robust for empty vectors, single-element vectors, vectors with duplicate values, and vectors containing negative numbers. The signature should be `void mergeSortAndGetSortedArray(std::vector<int>& arr)`. You are not allowed to use any standard library sorting functions; you must implement both the merge and the recursive merge sort logic manually. The solution must be self-contained with only necessary headers, and must not include a `main` function.
The solution implements the classic divide-and-conquer merge sort algorithm. The function takes a vector by reference and sorts it in place. The main approach: if the vector has zero or one element, it is already sorted so we return immediately. Otherwise, we recursively sort the left half and right half of the vector, then merge the two sorted halves into a temporary array and copy the result back into the original vector. The merge step uses two indices to traverse the left and right subarrays, picking the smaller element at each step and appending it to the original position. Edge cases include: empty vector (return immediately), single element (return immediately), duplicates (handled by using `<=` in the comparison so it is stable), and negative numbers (comparison works the same). Time complexity is O(n log n) for all cases, and space complexity is O(n) due to the temporary arrays used during merging.
#include <vector>

// Merge two sorted subarrays [left, mid] and [mid+1, right] into the original array.
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    std::vector<int> leftArr(leftSize);
    std::vector<int> rightArr(rightSize);

    for (int i = 0; i < leftSize; ++i) {
        leftArr[i] = arr[left + i];
    }
    for (int i = 0; i < rightSize; ++i) {
        rightArr[i] = arr[mid + 1 + i];
    }

    int i = 0, j = 0, k = left;
    while (i < leftSize && j < rightSize) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }

    while (i < leftSize) {
        arr[k++] = leftArr[i++];
    }
    while (j < rightSize) {
        arr[k++] = rightArr[j++];
    }
}

// Recursive merge sort on the subarray defined by left and right indices.
void mergeSortHelper(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(arr, left, mid);
        mergeSortHelper(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Sorts the given vector in place using merge sort.
void mergeSortAndGetSortedArray(std::vector<int>& arr) {
    if (arr.size() <= 1) {
        return;
    }
    mergeSortHelper(arr, 0, static_cast<int>(arr.size()) - 1);
}
#include <cassert>
#include <vector>

// Solution function declarations
void merge(std::vector<int>& arr, int left, int mid, int right);
void mergeSortHelper(std::vector<int>& arr, int left, int right);
void mergeSortAndGetSortedArray(std::vector<int>& arr);

int main() {
    // Test 1: Basic case
    std::vector<int> a1 = {5, 2, 9, 1, 5, 6};
    mergeSortAndGetSortedArray(a1);
    assert((a1 == std::vector<int>{1, 2, 5, 5, 6, 9}));

    // Test 2: Already sorted
    std::vector<int> a2 = {1, 2, 3, 4, 5};
    mergeSortAndGetSortedArray(a2);
    assert((a2 == std::vector<int>{1, 2, 3, 4, 5}));

    // Test 3: Reverse sorted
    std::vector<int> a3 = {9, 7, 5, 3, 1};
    mergeSortAndGetSortedArray(a3);
    assert((a3 == std::vector<int>{1, 3, 5, 7, 9}));

    // Test 4: Empty vector
    std::vector<int> a4;
    mergeSortAndGetSortedArray(a4);
    assert(a4.empty());

    // Test 5: Single element
    std::vector<int> a5 = {42};
    mergeSortAndGetSortedArray(a5);
    assert((a5 == std::vector<int>{42}));

    // Test 6: Duplicates and negatives
    std::vector<int> a6 = {-3, -1, -1, 0, 2, -5};
    mergeSortAndGetSortedArray(a6);
    assert((a6 == std::vector<int>{-5, -3, -1, -1, 0, 2}));

    // Test 7: All identical elements
    std::vector<int> a7 = {7, 7, 7, 7};
    mergeSortAndGetSortedArray(a7);
    assert((a7 == std::vector<int>{7, 7, 7, 7}));

    // Test 8: Two elements unsorted
    std::vector<int> a8 = {10, -1};
    mergeSortAndGetSortedArray(a8);
    assert((a8 == std::vector<int>{-1, 10}));

    // Test 9: Large vector with random values
    std::vector<int> a9 = {100, 0, -100, 50, -50, 25, -25};
    mergeSortAndGetSortedArray(a9);
    assert((a9 == std::vector<int>{-100, -50, -25, 0, 25, 50, 100}));

    // Test 10: Large size to ensure it works for bigger inputs
    std::vector<int> a10 = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    mergeSortAndGetSortedArray(a10);
    assert((a10 == std::vector<int>{1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9}));

    return 0;
}
