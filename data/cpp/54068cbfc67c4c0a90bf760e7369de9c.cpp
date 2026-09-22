// Write a C++ function `selectionSortDescending(int arr[], int n)` that sorts an array of integers in descending order (largest to smallest) using the selection sort algorithm. The function should modify the input array in place and return `void`. The array may contain duplicate values, negative numbers, and zero. The function must work correctly for arrays of any size `n >= 0` (including empty arrays) without causing undefined behavior.
The solution is a direct adaptation of the classic selection sort algorithm, but modified to select the maximum element in each pass instead of the minimum. The algorithm works by repeatedly finding the largest element in the unsorted portion of the array (from index `i` to `n-1`) and swapping it with the element at position `i`. After each outer loop iteration, the sorted portion grows from the left with the largest remaining element placed correctly. Key edge cases: (1) For `n == 0` or `n == 1`, no swaps are performed and the array is returned as-is. (2) Duplicate values are handled naturally because comparisons use `<` or `>`; if multiple maxima exist, the first encountered is selected, and the sort remains stable in terms of value placement (though selection sort is not stable in the traditional sense, this is irrelevant for sorting integers). (3) Negative numbers and zero are handled by standard integer comparison. Time complexity is \(O(n^2)\) for both average and worst cases due to the nested loops (the inner loop runs `n-i-1` times, summing to about `n^2/2` comparisons). Space complexity is \(O(1)\) extra space, as only a few temporary variables (loop counters and a maximum index) are used, and the swap is done in place. Since the function modifies the array directly, it should accept a non-const pointer or reference; using `int arr[]` decays to `int*` which allows modification. For const correctness, the parameter is `int arr[]` (not `const int arr[]`), as modification is required.
#include <utility>  // for std::swap

// Sorts the given array of integers in descending order using selection sort.
// The function modifies the array in place and returns void.
void selectionSortDescending(int arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        int maxIndex = i;  // index of the maximum element in unsorted part

        // Find the index of the largest element from i to n-1
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] > arr[maxIndex]) {
                maxIndex = j;
            }
        }

        // Swap the found maximum with the element at position i
        if (maxIndex != i) {
            std::swap(arr[maxIndex], arr[i]);
        }
    }
}
#include <cassert>

int main() {
    // Test empty array (n=0) – should not crash
    int emptyArr[] = {};
    selectionSortDescending(emptyArr, 0);

    // Test single-element array
    int singleArr[] = {42};
    selectionSortDescending(singleArr, 1);
    assert(singleArr[0] == 42);

    // Test basic descending order
    int arr1[] = {6, 2, 8, 4, 10};
    selectionSortDescending(arr1, 5);
    assert(arr1[0] == 10 && arr1[1] == 8 && arr1[2] == 6 && arr1[3] == 4 && arr1[4] == 2);

    // Test with duplicates
    int arr2[] = {3, 3, 3, 1, 3};
    selectionSortDescending(arr2, 5);
    assert(arr2[0] == 3 && arr2[1] == 3 && arr2[2] == 3 && arr2[3] == 3 && arr2[4] == 1);

    // Test with negatives and zero
    int arr3[] = {-5, 0, -1, -10};
    selectionSortDescending(arr3, 4);
    assert(arr3[0] == 0 && arr3[1] == -1 && arr3[2] == -5 && arr3[3] == -10);

    // Test already sorted ascending array
    int arr4[] = {1, 2, 3, 4};
    selectionSortDescending(arr4, 4);
    assert(arr4[0] == 4 && arr4[1] == 3 && arr4[2] == 2 && arr4[3] == 1);

    // Test already sorted descending array
    int arr5[] = {5, 4, 3};
    selectionSortDescending(arr5, 3);
    assert(arr5[0] == 5 && arr5[1] == 4 && arr5[2] == 3);

    return 0;
}
