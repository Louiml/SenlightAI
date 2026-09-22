Write a C++ function named `mergeSortedArrays` that takes a single integer array (via pointer), its length, and two indices `l` and `r` (with `l <= r`), assuming the subarray from `l` to `r` has a valid split point `m` such that the left segment `[l, m]` and the right segment `[m+1, r]` are each already sorted in non-decreasing order. The function must merge these two sorted segments into one sorted segment in-place, modifying the original array so that the entire range `[l, r]` is sorted. The function should return `void` and handle edge cases like empty segments, equal elements, and negative numbers. Do not use any standard sorting library functions.
// The core algorithm is the classic merge step of merge sort. We create two indices: `i` starting at `l` and `j` starting at `m+1`. We also allocate a temporary vector to hold the merged result. While both indices are within their respective subarrays, we compare `arr[i]` and `arr[j]`, copying the smaller value into the temporary vector and advancing the corresponding index. If they are equal, we copy the left element first to maintain stability. After one subarray is exhausted, we copy the remaining elements from the other subarray. Finally, we copy the merged temporary vector back into the original array positions from `l` to `r`. Edge cases include when `l == r` (single element, no work needed), when one subarray is empty (e.g., `m < l` or `r < m+1`), which the algorithm handles naturally because the while loops for the remaining elements will copy everything from the non-empty side. Time complexity is O(r - l + 1) for copying and merging, and space complexity is O(r - l + 1) for the temporary vector.
#include <vector>

// Merge two sorted subarrays arr[l..m] and arr[m+1..r] in-place.
// Assumes both subarrays are sorted in non-decreasing order.
void mergeSortedArrays(int arr[], int l, int m, int r) {
    // If the range is empty or has one element, nothing to merge.
    if (l >= r) return;

    int i = l;          // start of left subarray
    int j = m + 1;      // start of right subarray
    std::vector<int> merged;
    merged.reserve(r - l + 1);

    // Merge while both subarrays have elements.
    while (i <= m && j <= r) {
        if (arr[i] <= arr[j]) {
            merged.push_back(arr[i]);
            ++i;
        } else {
            merged.push_back(arr[j]);
            ++j;
        }
    }

    // Copy remaining elements from the left subarray.
    while (i <= m) {
        merged.push_back(arr[i]);
        ++i;
    }

    // Copy remaining elements from the right subarray.
    while (j <= r) {
        merged.push_back(arr[j]);
        ++j;
    }

    // Write merged results back to the original array.
    for (int k = l; k <= r; ++k) {
        arr[k] = merged[k - l];
    }
}
#include <cassert>
#include <vector>

// Assume the function is declared above (include the solution).

int main() {
    // Test 1: Basic merge with distinct elements
    int arr1[] = {1, 3, 5, 2, 4, 6};
    mergeSortedArrays(arr1, 0, 2, 5);
    assert(arr1[0] == 1 && arr1[1] == 2 && arr1[2] == 3 && arr1[3] == 4 && arr1[4] == 5 && arr1[5] == 6);

    // Test 2: Merge with negative numbers
    int arr2[] = {-7, -2, 0, -5, -1};
    mergeSortedArrays(arr2, 0, 2, 4);
    assert(arr2[0] == -7 && arr2[1] == -5 && arr2[2] == -2 && arr2[3] == -1 && arr2[4] == 0);

    // Test 3: Merge with duplicate elements
    int arr3[] = {2, 2, 2, 1, 2, 2};
    mergeSortedArrays(arr3, 0, 2, 5);
    assert(arr3[0] == 1 && arr3[1] == 2 && arr3[2] == 2 && arr3[3] == 2 && arr3[4] == 2 && arr3[5] == 2);

    // Test 4: Single element (l == r)
    int arr4[] = {42};
    mergeSortedArrays(arr4, 0, 0, 0);
    assert(arr4[0] == 42);

    // Test 5: Left subarray empty (m < l)
    int arr5[] = {5, 6, 7};
    mergeSortedArrays(arr5, 1, 0, 2); // left is empty, right is [6,7]
    // No change expected because l=1, m=0, so left list is empty, right list is arr[1..2]=[6,7] which is already sorted.
    assert(arr5[0] == 5 && arr5[1] == 6 && arr5[2] == 7);

    // Test 6: Right subarray empty (r < m+1)
    int arr6[] = {8, 9, 10};
    mergeSortedArrays(arr6, 0, 2, 1); // left is [8,9], right is empty
    // No change because left is already sorted.
    assert(arr6[0] == 8 && arr6[1] == 9 && arr6[2] == 10);

    // Test 7: Left elements all smaller than right
    int arr7[] = {1, 2, 3, 4, 5, 6};
    mergeSortedArrays(arr7, 0, 2, 5);
    assert(arr7[0] == 1 && arr7[1] == 2 && arr7[2] == 3 && arr7[3] == 4 && arr7[4] == 5 && arr7[5] == 6);

    // Test 8: Right elements all smaller than left
    int arr8[] = {10, 20, 30, 1, 5, 7};
    mergeSortedArrays(arr8, 0, 2, 5);
    assert(arr8[0] == 1 && arr8[1] == 5 && arr8[2] == 7 && arr8[3] == 10 && arr8[4] == 20 && arr8[5] == 30);

    // Test 9: Larger array with mixed values
    int arr9[] = {-3, 0, 4, 7, -10, -2, 1, 9};
    mergeSortedArrays(arr9, 0, 3, 7);
    assert(arr9[0] == -10 && arr9[1] == -3 && arr9[2] == -2 && arr9[3] == 0 && arr9[4] == 1 && arr9[5] == 4 && arr9[6] == 7 && arr9[7] == 9);

    // Test 10: Two elements only
    int arr10[] = {3, 1};
    mergeSortedArrays(arr10, 0, 0, 1);
    assert(arr10[0] == 1 && arr10[1] == 3);

    return 0;
}
