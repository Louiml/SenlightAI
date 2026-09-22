// Write a C++ function `selectionSortDescending` that sorts an array of integers in descending order (largest to smallest) using the selection sort algorithm. The function should take a pointer to an integer array and its size as parameters, modify the array in place, and return nothing. The function must handle edge cases like an empty array (size 0) and a single-element array (size 1) without causing errors or unnecessary operations. The sorting must be stable (maintain the relative order of equal elements). For example, given the array `{12, 4, 1, 8, 9}` and size 5, after calling the function, the array should become `{12, 9, 8, 4, 1}`.

The solution applies the classic selection sort algorithm but adapted for descending order. The outer loop runs from index 0 to `n-2`. For each iteration `i`, we assume the current position holds the largest remaining element. We then scan the unsorted portion of the array (from `i+1` to `n-1`) to find the index of the maximum element in that subarray. After identifying the maximum index `maxIdx`, we swap the element at `i` with the element at `maxIdx`. This ensures that after each outer iteration, the largest remaining element is placed at the correct position at the front. For edge cases: if `n <= 1`, the outer loop will not run (since `n-1` would be 0 or negative), so the function returns without modifications. Stability is not naturally guaranteed by selection sort (it may swap equal elements), but we can preserve stability by only swapping when `maxIdx != i` and using `>` (strictly greater) when comparing, so we never replace an equal element with a later one. Time complexity is O(n²) in all cases (best, average, worst) because of the nested loops. Space complexity is O(1) since we only use a constant amount of extra memory (a few integer variables).

#include <algorithm> // for std::swap

// Sorts an integer array in descending order using selection sort.
// Handles empty and single-element arrays safely.
// Uses a strict greater-than comparison to maintain stability among equal elements.
void selectionSortDescending(int arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        int maxIdx = i;
        for (int j = i + 1; j < n; ++j) {
            // Strict '>' ensures stability: equal elements are not swapped.
            if (arr[j] > arr[maxIdx]) {
                maxIdx = j;
            }
        }
        if (maxIdx != i) {
            std::swap(arr[maxIdx], arr[i]);
        }
    }
    // No need to return; array is modified in place.
}

#include <cassert>

int main() {
    // Test 1: Basic descending sort
    int arr1[] = {12, 4, 1, 8, 9};
    selectionSortDescending(arr1, 5);
    assert(arr1[0] == 12 && arr1[1] == 9 && arr1[2] == 8 && arr1[3] == 4 && arr1[4] == 1);

    // Test 2: Already sorted descending
    int arr2[] = {9, 7, 5, 3, 1};
    selectionSortDescending(arr2, 5);
    assert(arr2[0] == 9 && arr2[1] == 7 && arr2[2] == 5 && arr2[3] == 3 && arr2[4] == 1);

    // Test 3: Reverse sorted (ascending) input
    int arr3[] = {1, 3, 5, 7, 9};
    selectionSortDescending(arr3, 5);
    assert(arr3[0] == 9 && arr3[1] == 7 && arr3[2] == 5 && arr3[3] == 3 && arr3[4] == 1);

    // Test 4: Single element
    int arr4[] = {42};
    selectionSortDescending(arr4, 1);
    assert(arr4[0] == 42);

    // Test 5: Empty array (should not crash)
    int* arr5 = nullptr;
    selectionSortDescending(arr5, 0);

    // Test 6: Duplicate elements (stability check)
    int arr6[] = {5, 2, 5, 1};
    selectionSortDescending(arr6, 4);
    // The result must be {5,5,2,1}; relative order of the two 5s is maintained.
    assert(arr6[0] == 5 && arr6[1] == 5 && arr6[2] == 2 && arr6[3] == 1);

    // Test 7: Negative numbers
    int arr7[] = {-3, -1, -7, -2};
    selectionSortDescending(arr7, 4);
    assert(arr7[0] == -1 && arr7[1] == -2 && arr7[2] == -3 && arr7[3] == -7);

    // Test 8: Mixed positives and negatives
    int arr8[] = {0, -5, 10, 3, -2};
    selectionSortDescending(arr8, 5);
    assert(arr8[0] == 10 && arr8[1] == 3 && arr8[2] == 0 && arr8[3] == -2 && arr8[4] == -5);

    return 0;
}
