Write a C++ function `long long countInversions(int* arr, int size)` that counts the number of inversions in an array of integers. An inversion is a pair of indices `(i, j)` such that `i < j` and `arr[i] > arr[j]`. The function should return the total count of such pairs. The input array may contain negative numbers, duplicates, and up to 10^5 elements. The function should not modify the original array (make a copy if needed). For example, in `[5, 3, 2]` there are three inversions: `(5,3)`, `(5,2)`, `(3,2)`. In `[2, 2, 1]`, there are two inversions: `(2,1)` for each occurrence of 2. Provide a standalone function that can be called from a test harness.

The classic approach is a modified merge sort that counts inversions while merging. The idea: split the array into halves, recursively count inversions in each half, then count the "cross" inversions between the two halves during the merge step. When merging two sorted halves (left part `L` and right part `R`), if an element in `L` is greater than an element in `R`, then that `L` element is greater than *all* remaining elements in `R` (since `R` is sorted ascending). So if we merge in reverse order (from largest to smallest), whenever we pick from the left side, we know it is larger than all remaining right-side elements, and we add the count of remaining right elements to the inversion counter. To avoid modifying the original array, we copy it into a temporary vector inside the function. Edge cases: empty array (returns 0), single element (returns 0), all equal elements (returns 0), already sorted ascending (returns 0), sorted descending (returns n*(n-1)/2). Time complexity is O(n log n) due to merge sort; space complexity is O(n) for the temporary array used during merge.

#include <vector>
#include <cstddef>

// Count inversions in an array using merge sort.
long long countInversions(const int* arr, int size) {
    if (size <= 1) return 0;
    
    // Work on a copy to avoid modifying the input.
    std::vector<int> temp(size);
    std::vector<int> sorted(arr, arr + size);
    
    long long inversions = 0;
    
    // Recursive lambda for merge sort with inversion counting.
    // We count inversions by merging from largest to smallest.
    std::function<void(int, int)> mergeSort = [&](int left, int right) {
        if (left >= right) return;
        int mid = left + (right - left) / 2;
        
        mergeSort(left, mid);
        mergeSort(mid + 1, right);
        
        // Merge two sorted halves [left..mid] and [mid+1..right] in descending order.
        int i = mid;        // last index of left half
        int j = right;      // last index of right half
        int k = right;      // write from end of the merged segment
        int remainingRight = right - mid; // number of unused elements in right half
        
        while (i >= left && j > mid) {
            if (sorted[i] > sorted[j]) {
                // sorted[i] is greater than all remaining in right half (since right is sorted ascending)
                inversions += remainingRight;
                temp[k--] = sorted[i--];
            } else {
                // sorted[j] is >= sorted[i], so no inversion for this j
                temp[k--] = sorted[j--];
                remainingRight--;
            }
        }
        // Copy remaining elements (if any) from left half
        while (i >= left) {
            temp[k--] = sorted[i--];
        }
        // Copy remaining elements (if any) from right half
        while (j > mid) {
            temp[k--] = sorted[j--];
        }
        // Write back the merged segment into sorted array
        for (int idx = left; idx <= right; ++idx) {
            sorted[idx] = temp[idx];
        }
    };
    
    mergeSort(0, size - 1);
    return inversions;
}

#include <cassert>
#include <iostream>

// Include the solution function (assume it's above in the same file)
extern long long countInversions(const int* arr, int size);

int main() {
    // Single element
    int a1[] = {1};
    assert(countInversions(a1, 1) == 0);
    
    // Already sorted ascending
    int a2[] = {1, 2, 3, 4};
    assert(countInversions(a2, 4) == 0);
    
    // Reverse sorted
    int a3[] = {4, 3, 2, 1};
    assert(countInversions(a3, 4) == 6);
    
    // Duplicates
    int a4[] = {2, 2, 1};
    assert(countInversions(a4, 3) == 2);
    
    // Mixed with negatives
    int a5[] = {-3, 5, -1, 2};
    // Inversions: (-3,-1)? no because -3 < -1. (-3,2)? no. (5,-1) yes, (5,2) yes. (-1,2)? no. => 2
    assert(countInversions(a5, 4) == 2);
    
    // Larger test with known count: 5,3,2,1 => inversions: (5,3),(5,2),(5,1),(3,2),(3,1),(2,1) => 6
    int a6[] = {5, 3, 2, 1};
    assert(countInversions(a6, 4) == 6);
    
    // Empty array (size 0) – should work (though size_t, but we pass int)
    int a7[] = {};
    assert(countInversions(a7, 0) == 0);
    
    // All equal
    int a8[] = {7, 7, 7};
    assert(countInversions(a8, 3) == 0);
    
    // Large random-ish check: 10,9,...1 => 45 inversions
    int a9[10];
    for (int i = 0; i < 10; ++i) a9[i] = 10 - i;
    assert(countInversions(a9, 10) == 45);
    
    std::cout << "All tests passed." << std::endl;
    return 0;
}
