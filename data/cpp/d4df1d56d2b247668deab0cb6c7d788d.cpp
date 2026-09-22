Write a C++ function named `mergeSortedArrays` that takes two sorted integer arrays, `arr1` of size `k` and `arr2` of size `m`, along with a pre-allocated output array `result` of size `k + m`, and merges them into a single sorted array in non-decreasing order. The function should return nothing (void) and must assume both input arrays are already sorted in ascending order. The output array will be provided by the caller and is guaranteed to have enough capacity. The function must handle duplicate values correctly and must not use any temporary containers.
// The merge operation is a classic two-pointer technique. Start with an index `i = 0` for `arr1`, `j = 0` for `arr2`, and `k = 0` for the output array. Repeatedly compare the current elements `arr1[i]` and `arr2[j]`. Place the smaller into `result[k]`, advance the pointer of the array from which the element was taken, and increment `k`. If equal, choose either (e.g., from `arr1`). Continue until one of the arrays is fully processed. Then copy any remaining elements from the other array directly into the output. Edge cases include: one or both input arrays being empty, duplicate values (handled naturally since the comparison uses `<`, so equal elements from `arr2` are chosen when `arr1[i] >= arr2[j]`), and arrays of different lengths. The algorithm runs in O(k + m) time, as each element is processed exactly once, and uses O(1) extra space, only a few index variables.
#include <cstddef>  // for size_t

void mergeSortedArrays(const int arr1[], std::size_t k,
                       const int arr2[], std::size_t m,
                       int result[]) {
    std::size_t i = 0, j = 0, o = 0;  // indexes for arr1, arr2, result

    // Merge while both arrays have elements left
    while (i < k && j < m) {
        if (arr1[i] <= arr2[j]) {
            result[o++] = arr1[i++];
        } else {
            result[o++] = arr2[j++];
        }
    }

    // Copy remaining elements from arr1 (if any)
    while (i < k) {
        result[o++] = arr1[i++];
    }

    // Copy remaining elements from arr2 (if any)
    while (j < m) {
        result[o++] = arr2[j++];
    }
}
#include <cassert>
#include <cstddef>

int main() {
    int arr1[] = {1, 3, 5, 7, 9};
    int arr2[] = {2, 4, 6};
    int result[8];
    mergeSortedArrays(arr1, 5, arr2, 3, result);
    int expected1[] = {1, 2, 3, 4, 5, 6, 7, 9};
    for (std::size_t i = 0; i < 8; ++i) {
        assert(result[i] == expected1[i]);
    }

    int a2[] = {};
    int b2[] = {10, 20, 30};
    int res2[3];
    mergeSortedArrays(a2, 0, b2, 3, res2);
    int expected2[] = {10, 20, 30};
    for (std::size_t i = 0; i < 3; ++i) {
        assert(res2[i] == expected2[i]);
    }

    int a3[] = {5, 5, 5};
    int b3[] = {5, 5};
    int res3[5];
    mergeSortedArrays(a3, 3, b3, 2, res3);
    int expected3[] = {5, 5, 5, 5, 5};
    for (std::size_t i = 0; i < 5; ++i) {
        assert(res3[i] == expected3[i]);
    }

    int a4[] = {1, 2, 3};
    int b4[] = {4, 5};
    int res4[5];
    mergeSortedArrays(a4, 3, b4, 2, res4);
    int expected4[] = {1, 2, 3, 4, 5};
    for (std::size_t i = 0; i < 5; ++i) {
        assert(res4[i] == expected4[i]);
    }

    int a5[] = {7, 8};
    int b5[] = {1, 2, 9};
    int res5[5];
    mergeSortedArrays(a5, 2, b5, 3, res5);
    int expected5[] = {1, 2, 7, 8, 9};
    for (std::size_t i = 0; i < 5; ++i) {
        assert(res5[i] == expected5[i]);
    }
    return 0;
}
