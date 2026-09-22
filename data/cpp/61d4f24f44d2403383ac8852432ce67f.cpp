Write a C++ function that performs a fully iterative bottom-up merge sort on a `std::vector<int>`, sorting the elements in non-decreasing order. The function must take a non-const reference to the vector and modify it in place. It should handle vectors of any size, including empty, single-element, already sorted, reverse-sorted, and vectors with duplicate values. The implementation must not use recursion or call the standard library’s `std::sort` (or any other sorting function). The function signature should be `void iterativeMergeSort(std::vector<int>& arr)`. You may write internal helper functions (e.g., for merging two sorted segments) as long as they are also iterative.
// The core idea is to implement merge sort iteratively by simulating the recursion with a bottom-up approach. Instead of dividing the array recursively, we start with subarrays of size 1 (which are trivially sorted) and repeatedly merge adjacent subarrays, doubling the subarray size at each pass until the entire array is merged.
//
// For each pass with subarray size `width`, we iterate over the array in steps of `2*width`. For each pair of adjacent subarrays, we merge them using a helper `merge` function that is identical to the standard merge step from recursive merge sort: it copies the two halves into temporary vectors, then writes them back in sorted order. The merge must handle cases where the right half is shorter (when the array length is not a multiple of `width`) or when the left half is the only remaining subarray (in which case no merging is needed, and we just skip to the next pair).
//
// Edge cases:
// - Empty or size-1 vector: no passes execute, function returns unchanged.
// - Odd-length arrays: the last subarray may be incomplete; the merge range is clamped to `arr.size()-1`.
// - Duplicates: the merge condition uses `<=`, so equal elements remain in stable order (not required but safe).
//
// Time complexity is \(O(n \log n)\) because each merging pass processes all \(n\) elements, and there are \(\log_2 n\) passes. Space complexity is \(O(n)\) due to the temporary vectors allocated in each merge, but these are freed after each merge; an optimized in-place merge is not required.
#include <vector>

// Merge two adjacent sorted segments [l, m] and [m+1, r] of arr.
void mergeSegments(std::vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temporary arrays for left and right halves.
    std::vector<int> left(n1);
    std::vector<int> right(n2);

    for (int i = 0; i < n1; ++i) {
        left[i] = arr[l + i];
    }
    for (int j = 0; j < n2; ++j) {
        right[j] = arr[m + 1 + j];
    }

    // Merge the temporary arrays back into arr[l..r].
    int i = 0; // index into left
    int j = 0; // index into right
    int k = l; // index into arr

    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            ++i;
        } else {
            arr[k] = right[j];
            ++j;
        }
        ++k;
    }

    // Copy remaining elements from left, if any.
    while (i < n1) {
        arr[k] = left[i];
        ++i;
        ++k;
    }

    // Copy remaining elements from right, if any.
    while (j < n2) {
        arr[k] = right[j];
        ++j;
        ++k;
    }
}

// In-place iterative (bottom-up) merge sort for a vector of integers.
void iterativeMergeSort(std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    if (n < 2) {
        return; // Already sorted or empty.
    }

    // Width of the subarray to merge; starts at 1 (single elements).
    for (int width = 1; width < n; width *= 2) {
        // Merge adjacent subarrays of length 'width' (last may be shorter).
        for (int left = 0; left < n - 1; left += 2 * width) {
            int mid = left + width - 1;
            if (mid >= n - 1) {
                break; // Only one subarray left, nothing to merge.
            }
            int right = std::min(left + 2 * width - 1, n - 1);
            mergeSegments(arr, left, mid, right);
        }
    }
}
#include <cassert>
#include <vector>

// Note: Solution code above is included here implicitly for compilation.

int main() {
    // Basic sorting.
    std::vector<int> v1 = {38, 27, 43, 3, 9, 82, 10};
    iterativeMergeSort(v1);
    assert(v1 == std::vector<int>({3, 9, 10, 27, 38, 43, 82}));

    // Empty vector.
    std::vector<int> v2;
    iterativeMergeSort(v2);
    assert(v2.empty());

    // Single element.
    std::vector<int> v3 = {42};
    iterativeMergeSort(v3);
    assert(v3 == std::vector<int>({42}));

    // Already sorted.
    std::vector<int> v4 = {1, 2, 3, 4, 5};
    iterativeMergeSort(v4);
    assert(v4 == std::vector<int>({1, 2, 3, 4, 5}));

    // Reverse sorted.
    std::vector<int> v5 = {5, 4, 3, 2, 1};
    iterativeMergeSort(v5);
    assert(v5 == std::vector<int>({1, 2, 3, 4, 5}));

    // Duplicates.
    std::vector<int> v6 = {7, 7, 2, 9, 2, 7};
    iterativeMergeSort(v6);
    assert(v6 == std::vector<int>({2, 2, 7, 7, 7, 9}));

    // Negative numbers.
    std::vector<int> v7 = {-3, -1, -10, 5, 0};
    iterativeMergeSort(v7);
    assert(v7 == std::vector<int>({-10, -3, -1, 0, 5}));

    // Larger even and odd sizes.
    std::vector<int> v8 = {100, 23, 56, -7, 0, 11, 99, 2, 45, 67};
    iterativeMergeSort(v8);
    assert(v8 == std::vector<int>({-7, 0, 2, 11, 23, 45, 56, 67, 99, 100}));

    // All same values.
    std::vector<int> v9 = {5, 5, 5, 5};
    iterativeMergeSort(v9);
    assert(v9 == std::vector<int>({5, 5, 5, 5}));

    return 0;
}
