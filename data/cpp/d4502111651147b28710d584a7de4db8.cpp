Write a C++ function `heapSortDescending(int arr[], int n)` that sorts an array of `n` integers in descending order using an in-place heap sort algorithm. The function should treat index 0 as the first element of the array (0-based indexing), but internally you may use a temporary 1-based indexing scheme if needed. The function must return the number of swaps performed during the entire sorting process (including those in the build-heap phase). The input array `arr` must be modified in-place, and the function should work correctly for arrays of length 0 or 1 (no swaps, no modifications). All comparisons must be based on integer values, and duplicates should be handled stably (order among equal elements may be arbitrary but allowed). The function must not use any additional dynamic memory except for constant-size variables. You may use helper functions inside the same file (static or in an unnamed namespace) to keep the solution clean.
#include <cassert>
#include <cstring>

int heapSortDescending(int* arr, int n); // declaration

int main() {
    // Test 1: empty array
    int a0[] = {};
    assert(heapSortDescending(a0, 0) == 0);

    // Test 2: single element
    int a1[] = {5};
    assert(heapSortDescending(a1, 1) == 0);
    assert(a1[0] == 5);

    // Test 3: already descending
    int a2[] = {5, 4, 3, 2, 1};
    int swaps2 = heapSortDescending(a2, 5);
    assert(swaps2 >= 0); // exact count depends on implementation but should be >=0
    assert(a2[0] == 5 && a2[1] == 4 && a2[2] == 3 && a2[3] == 2 && a2[4] == 1);

    // Test 4: ascending input becomes descending
    int a3[] = {1, 2, 3, 4, 5};
    heapSortDescending(a3, 5);
    assert(a3[0] == 5 && a3[1] == 4 && a3[2] == 3 && a3[3] == 2 && a3[4] == 1);

    // Test 5: duplicates
    int a4[] = {3, 1, 3, 2, 1};
    heapSortDescending(a4, 5);
    assert(a4[0] == 3 && a4[1] == 3 && a4[2] == 2 && a4[3] == 1 && a4[4] == 1);

    // Test 6: negative numbers
    int a5[] = {-3, -1, -2, -5};
    heapSortDescending(a5, 4);
    assert(a5[0] == -1 && a5[1] == -2 && a5[2] == -3 && a5[3] == -5);

    // Test 7: large array (100 elements descending order check)
    const int N = 100;
    int a6[N];
    for (int i = 0; i < N; ++i) a6[i] = i; // 0..99
    heapSortDescending(a6, N);
    for (int i = 0; i < N; ++i) {
        assert(a6[i] == N - 1 - i);
    }

    // Test 8: swap count correctness for a small known case
    int a7[] = {4, 3, 2, 1};
    // Expected swaps: build + extraction. We just verify it returns a positive number and array sorted.
    int swaps7 = heapSortDescending(a7, 4);
    assert(swaps7 > 0);
    assert(a7[0] == 4 && a7[1] == 3 && a7[2] == 2 && a7[3] == 1);

    return 0;
}
#include <cstddef>

// Helper: swap two integers and increment a swap counter.
static void swapCount(int& a, int& b, int& count) {
    int tmp = a;
    a = b;
    b = tmp;
    ++count;
}

// Helper: sift down the element at given index in a min-heap of given size.
static void siftDown(int* arr, int idx, int heapSize, int& swaps) {
    while (true) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        if (left < heapSize && arr[left] < arr[smallest]) smallest = left;
        if (right < heapSize && arr[right] < arr[smallest]) smallest = right;
        if (smallest == idx) break;
        swapCount(arr[idx], arr[smallest], swaps);
        idx = smallest;
    }
}

// Sorts arr in descending order in-place and returns number of swaps performed.
int heapSortDescending(int* arr, int n) {
    if (n <= 1) return 0;
    int swaps = 0;

    // Build min-heap (in-place, iterative from last parent to root).
    for (int i = (n / 2) - 1; i >= 0; --i) {
        siftDown(arr, i, n, swaps);
    }

    // Extract elements one by one from heap.
    for (int end = n - 1; end > 0; --end) {
        // Move current root (minimum) to the end (descending placement).
        swapCount(arr[0], arr[end], swaps);
        // Reduce heap size and repair heap.
        siftDown(arr, 0, end, swaps);
    }

    return swaps;
}
// The solution follows the classic heap sort algorithm but adapted for descending order. First, we build a min-heap (because descending order requires extracting the minimum repeatedly) from the given array. However, because the original snippet uses 1-based indexing, we can map the user's 0-based array into a 1-based logical view by using a local pointer that points to `arr[-1]`? That is dangerous. Instead, we use a different approach: we work directly on the 0-based array but implement a min-heap using 0-based children formulas: left child = 2*i+1, right child = 2*i+2, parent = (i-1)/2 for i>0. Build the heap by heapifying from the last parent down to root. Then repeatedly swap the root (minimum) with the last unsorted element, reduce the heap size by 1, and heapify down. This yields descending order because the extracted minima go to the end with descending values. Count every swap performed in both build and extraction phases. Edge cases: n=0 or 1 → no swaps, return 0. Duplicates are handled naturally. Time complexity: O(n log n) for both build (O(n)) and extraction (O(n log n)). Space complexity: O(1) auxiliary (only constant variables and recursion stack if using recursion; better to implement iteratively to avoid extra stack, but recursion depth is O(log n) so acceptable). The function should return the swap count as an integer, potentially large but within `int` range for typical inputs.
