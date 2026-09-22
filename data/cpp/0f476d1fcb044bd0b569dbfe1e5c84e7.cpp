/*
Write a C++ function named `recursiveInsertionSort` that takes a pointer to an array of integers and its size `n`, and sorts the array in ascending order using recursion in the style of insertion sort. The recursive structure must follow this pattern: for `n == 1`, the function returns immediately (base case); for `n > 1`, it recursively sorts the first `n-1` elements so that they are in non-decreasing order, then inserts the last element (`a[n-1]`) into its correct position within the already‑sorted prefix by shifting larger elements to the right. The function must modify the array in‑place, return `void`, and handle edge cases such as `n = 0` (do nothing) and `n = 1`. Do not use any standard sorting library functions (e.g., `std::sort`, `std::stable_sort`). The function signature must be `void recursiveInsertionSort(int* arr, int size);`. Provide a complete implementation with necessary headers and comments, but do not include a `main` function in the solution section.
*/

#include <cstddef> // for size_t if needed, but we use int size

// Recursive insertion sort: sorts arr[0..size-1] in ascending order.
void recursiveInsertionSort(int* arr, int size) {
    // Base case: empty or single-element array is already sorted.
    if (size <= 1) {
        return;
    }

    // Hypothesis: sort the first size-1 elements.
    recursiveInsertionSort(arr, size - 1);

    // Inductive step: insert the last element into the sorted prefix.
    int key = arr[size - 1];
    int i = size - 2;
    // Shift elements greater than key to the right.
    while (i >= 0 && arr[i] > key) {
        arr[i + 1] = arr[i];
        --i;
    }
    // Place the key in its correct position.
    arr[i + 1] = key;
}

#include <cassert>

// Solution function declaration (from the solution section).
void recursiveInsertionSort(int* arr, int size);

int main() {
    // Test 1: Basic unsorted array.
    int a1[] = {3, 1, 5, 9, 7, 6};
    recursiveInsertionSort(a1, 6);
    assert(a1[0] == 1 && a1[1] == 3 && a1[2] == 5 && a1[3] == 6 && a1[4] == 7 && a1[5] == 9);

    // Test 2: Already sorted array.
    int a2[] = {1, 2, 3, 4};
    recursiveInsertionSort(a2, 4);
    assert(a2[0] == 1 && a2[1] == 2 && a2[2] == 3 && a2[3] == 4);

    // Test 3: Reverse-sorted array (worst case).
    int a3[] = {9, 7, 5, 3, 1};
    recursiveInsertionSort(a3, 5);
    assert(a3[0] == 1 && a3[1] == 3 && a3[2] == 5 && a3[3] == 7 && a3[4] == 9);

    // Test 4: Duplicate values.
    int a4[] = {4, 2, 4, 1, 2};
    recursiveInsertionSort(a4, 5);
    assert(a4[0] == 1 && a4[1] == 2 && a4[2] == 2 && a4[3] == 4 && a4[4] == 4);

    // Test 5: Single element.
    int a5[] = {42};
    recursiveInsertionSort(a5, 1);
    assert(a5[0] == 42);

    // Test 6: Two elements unsorted.
    int a6[] = {10, -3};
    recursiveInsertionSort(a6, 2);
    assert(a6[0] == -3 && a6[1] == 10);

    // Test 7: All zeros.
    int a7[] = {0, 0, 0};
    recursiveInsertionSort(a7, 3);
    assert(a7[0] == 0 && a7[1] == 0 && a7[2] == 0);

    // Test 8: Negative numbers mixed.
    int a8[] = {-5, 0, -1, 2, -3};
    recursiveInsertionSort(a8, 5);
    assert(a8[0] == -5 && a8[1] == -3 && a8[2] == -1 && a8[3] == 0 && a8[4] == 2);

    return 0;
}

// The algorithm is a recursive implementation of insertion sort. In the base case, when `size` is `0` or `1`, no sorting is needed, so we return. For `size > 1`, we first recursively call `recursiveInsertionSort(arr, size-1)`, which ensures that the first `size-1` elements are sorted in ascending order. Then, we take the value at index `size-1` and compare it to elements from right to left, starting from index `size-2`. As long as the current element is greater than our key and we haven't reached the left end, we shift that element one position to the right. When the correct position is found (either at index `-1` or at an element ≤ the key), we place the key there. This mirrors the iterative insertion sort but uses recursion to sort the prefix instead of a loop. Edge cases include `size = 0` (return immediately), and arrays with duplicate values—these are handled correctly because we stop shifting when the element is not greater than the key. Time complexity is \(O(n^2)\) in the worst case (e.g., reverse-sorted input) and \(O(n)\) in the best case (already sorted input, because the inner while loop never shifts). Space complexity is \(O(n)\) due to the recursion stack depth, which is not constant like the iterative version. The in‑place nature means no extra array storage is used, but the call stack grows with `n`.
