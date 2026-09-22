/*
Write a C++ function named `stableCountingSort` that takes a non-empty array of integers, its size, and an output array of the same size, and fills the output array with the elements of the input array sorted in non-decreasing order using the counting-by-comparison method (for each element, count how many elements are smaller, and place it at that index). The relative order of equal elements must be preserved (stable sort): if two elements are equal, the one that appears earlier in the input must appear earlier in the output. The input array must not be modified, and the function should return `void`. Note that the array may contain negative numbers and duplicate values, and the size `n` is at least 1. Do not use standard sorting algorithms or additional container types like `std::vector` or `std::map` in the implementation.
*/

#include <cstddef>

// Fill output array b with a stable non-decreasing sort of input array a.
// a: input array (read-only), b: output array (same size), n: number of elements.
void stableCountingSort(const int a[], int b[], std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) {
        int position = 0;
        // Count elements strictly smaller than a[i].
        for (std::size_t j = 0; j < n; ++j) {
            if (a[j] < a[i]) {
                ++position;
            }
        }
        // Count equal elements that appear earlier in the input (for stability).
        for (std::size_t j = 0; j < i; ++j) {
            if (a[j] == a[i]) {
                ++position;
            }
        }
        b[position] = a[i];
    }
}

#include <cassert>
#include <cstddef>

// Function declaration (as per solution).
void stableCountingSort(const int a[], int b[], std::size_t n);

int main() {
    // Test 1: basic unsorted array with positives.
    int a1[6] = {3, 5, 7, 2, 4, 9};
    int b1[6];
    stableCountingSort(a1, b1, 6);
    assert(b1[0] == 2 && b1[1] == 3 && b1[2] == 4 && b1[3] == 5 && b1[4] == 7 && b1[5] == 9);

    // Test 2: negative numbers.
    int a2[4] = {-3, -1, -7, -2};
    int b2[4];
    stableCountingSort(a2, b2, 4);
    assert(b2[0] == -7 && b2[1] == -3 && b2[2] == -2 && b2[3] == -1);

    // Test 3: duplicates with stability (input: 5,3,5,1,3 -> sorted: 1,3,3,5,5 and order of 3s/5s preserved).
    int a3[5] = {5, 3, 5, 1, 3};
    int b3[5];
    stableCountingSort(a3, b3, 5);
    assert(b3[0] == 1 && b3[1] == 3 && b3[2] == 3 && b3[3] == 5 && b3[4] == 5);
    // Verify stability: the first '3' in input (index 1) must be b3[1], second '3' (index 4) b3[2].
    // Already implied by the values, but we can check by comparing pointers? Not needed for values.

    // Test 4: single element.
    int a4[1] = {42};
    int b4[1];
    stableCountingSort(a4, b4, 1);
    assert(b4[0] == 42);

    // Test 5: already sorted array (with duplicates).
    int a5[4] = {1, 2, 2, 3};
    int b5[4];
    stableCountingSort(a5, b5, 4);
    assert(b5[0] == 1 && b5[1] == 2 && b5[2] == 2 && b5[3] == 3);

    // Test 6: all equal elements.
    int a6[3] = {7, 7, 7};
    int b6[3];
    stableCountingSort(a6, b6, 3);
    assert(b6[0] == 7 && b6[1] == 7 && b6[2] == 7);

    // Test 7: reverse sorted.
    int a7[5] = {9, 6, 4, 1, 0};
    int b7[5];
    stableCountingSort(a7, b7, 5);
    assert(b7[0] == 0 && b7[1] == 1 && b7[2] == 4 && b7[3] == 6 && b7[4] == 9);

    // Test 8: mix of negative, zero, and positive.
    int a8[6] = {-2, 0, 3, -5, 1, 0};
    int b8[6];
    stableCountingSort(a8, b8, 6);
    assert(b8[0] == -5 && b8[1] == -2 && b8[2] == 0 && b8[3] == 0 && b8[4] == 1 && b8[5] == 3);

    return 0;
}

// The main algorithm follows the provided snippet: for each element `a[i]`, iterate over the entire array and count how many elements are strictly smaller than `a[i]`. That count gives the starting position for that value in the sorted output. However, to make the sort stable (preserve the relative order of equal elements), a simple count of strictly smaller elements is insufficient because equal elements would all be assigned the same index. To achieve stability, we augment the count: for each element, we count the number of elements that are strictly smaller, plus the number of elements that are equal but appear earlier in the input (i.e., for index `i`, count occurrences of `a[j] == a[i]` where `j < i`). This ensures that equal elements occupy consecutive positions in the same order as they appear in the input. The algorithm uses two nested loops, so the time complexity is O(n²) in the worst case, and it uses O(1) auxiliary space (only the output array, which is required by the interface). Edge cases include negative numbers (comparison works fine), duplicates (handled by the stable placement), and arrays of size 1 (the single element is placed at index 0). The input array is read-only, so we pass it as `const int*` or `const int[]` to enforce const correctness.
