// Write a C++ function named `stableMergeSort` that takes a `std::vector<int>` by reference and sorts it in ascending order using the merge sort algorithm. The function must be stable, meaning that equal elements preserve their original relative order. The sorting must be performed in-place on the input vector (no returning a new vector). The function should handle vectors of any size, including empty vectors and vectors with duplicate values, without crashing or producing undefined behavior. Do not use `std::sort` or any other standard library sorting algorithm; implement the merge and divide steps manually.
#include <cassert>
#include <vector>

// The solution function is expected to be declared above this main.

int main() {
    // Basic sort
    std::vector<int> v1 = {5, 2, 9, 1, 5, 6};
    stableMergeSort(v1);
    assert((v1 == std::vector<int>{1, 2, 5, 5, 6, 9}));

    // Empty vector
    std::vector<int> v2;
    stableMergeSort(v2);
    assert(v2.empty());

    // Single element
    std::vector<int> v3 = {42};
    stableMergeSort(v3);
    assert((v3 == std::vector<int>{42}));

    // Already sorted
    std::vector<int> v4 = {1, 2, 3, 4, 5};
    stableMergeSort(v4);
    assert((v4 == std::vector<int>{1, 2, 3, 4, 5}));

    // Reverse sorted
    std::vector<int> v5 = {9, 8, 7, 6, 5};
    stableMergeSort(v5);
    assert((v5 == std::vector<int>{5, 6, 7, 8, 9}));

    // Duplicate values (stability test using a pair structure would be needed for full stability,
    // but here we at least check duplicates are sorted correctly)
    std::vector<int> v6 = {3, 1, 2, 3, 3, 0};
    stableMergeSort(v6);
    assert((v6 == std::vector<int>{0, 1, 2, 3, 3, 3}));

    // Large vector with many duplicates
    std::vector<int> v7 = {5, 5, 5, 5, 5};
    stableMergeSort(v7);
    assert((v7 == std::vector<int>{5, 5, 5, 5, 5}));

    // Negative numbers
    std::vector<int> v8 = {-3, -1, -7, 0, 2};
    stableMergeSort(v8);
    assert((v8 == std::vector<int>{-7, -3, -1, 0, 2}));

    return 0;
}
#include <vector>

// Merge two sorted halves of arr[low..high] into one sorted segment
void mergeHalves(std::vector<int>& arr, int low, int mid, int high) {
    int leftSize = mid - low + 1;
    int rightSize = high - mid;

    // Temporary vectors to hold the two halves
    std::vector<int> leftHalf(leftSize);
    std::vector<int> rightHalf(rightSize);

    // Copy data into temporary vectors
    for (int i = 0; i < leftSize; ++i)
        leftHalf[i] = arr[low + i];
    for (int j = 0; j < rightSize; ++j)
        rightHalf[j] = arr[mid + 1 + j];

    // Merge the temporary vectors back into arr[low..high]
    int i = 0;   // index in leftHalf
    int j = 0;   // index in rightHalf
    int k = low; // index in original arr

    while (i < leftSize && j < rightSize) {
        // Use <= to maintain stability (equal elements come from left first)
        if (leftHalf[i] <= rightHalf[j]) {
            arr[k] = leftHalf[i];
            ++i;
        } else {
            arr[k] = rightHalf[j];
            ++j;
        }
        ++k;
    }

    // Copy any remaining elements from leftHalf
    while (i < leftSize) {
        arr[k] = leftHalf[i];
        ++i;
        ++k;
    }

    // Copy any remaining elements from rightHalf
    while (j < rightSize) {
        arr[k] = rightHalf[j];
        ++j;
        ++k;
    }
}

// Recursive merge sort function
void mergeSortRecursive(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int mid = low + (high - low) / 2;
        mergeSortRecursive(arr, low, mid);
        mergeSortRecursive(arr, mid + 1, high);
        mergeHalves(arr, low, mid, high);
    }
}

// Public function to sort a vector in-place using merge sort
void stableMergeSort(std::vector<int>& arr) {
    if (arr.size() <= 1) return; // already sorted
    mergeSortRecursive(arr, 0, static_cast<int>(arr.size()) - 1);
}
// The solution uses the classic divide-and-conquer merge sort approach. The main function recursively splits the vector into two halves until each subarray has at most one element (base case). Then, during the "merge" step, two sorted subarrays are merged back into the parent array in sorted order. To maintain stability, when comparing elements from the left and right halves, if they are equal, the element from the left half is chosen first. The merge step uses temporary vectors to hold copies of the left and right halves, then writes the merged result back into the original vector. Edge cases include an empty vector (immediately returns), a vector with one element (already sorted), and vectors with duplicate values (stable order preserved). Time complexity is O(n log n) for all cases, and space complexity is O(n) due to the temporary arrays created during merge operations.
