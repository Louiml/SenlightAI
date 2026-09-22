/*
Write a C++ function that takes a sorted integer array and its size as parameters, removes duplicate elements in-place, and returns the new length of the array containing only distinct elements. The function must preserve the original sorted order of the distinct elements. Use a two-pointer or in-place overwriting technique without using extra arrays or containers, and ensure the first `n` elements of the array (where `n` is the returned length) contain the distinct values, while the remaining elements are irrelevant. The input array is assumed to be non-empty and sorted in non-decreasing order. The function should be `const`-correct: it modifies the array contents (since it overwrites duplicates) but does not change the size parameter; mark the size parameter as `const` inside the function where appropriate.
*/
#include <vector>  // Not used directly, but included for completeness if needed in tests

// Removes duplicates from a sorted array in-place.
// Returns the number of distinct elements, which are stored in the first
// returned-length positions of the array. The remaining positions are unchanged.
int removeDuplicates(int arr[], const int n) {
    if (n <= 0) return 0;
    int writeIndex = 0;  // Position of the last kept distinct element.
    for (int readIndex = 1; readIndex < n; ++readIndex) {
        if (arr[readIndex] != arr[writeIndex]) {
            ++writeIndex;
            arr[writeIndex] = arr[readIndex];
        }
    }
    return writeIndex + 1;  // +1 because writeIndex is zero-based.
}
#include <cassert>

int main() {
    // Test 1: Basic duplicates
    int a1[] = {1, 2, 2, 3, 3, 4, 4, 4, 5};
    int n1 = removeDuplicates(a1, 9);
    assert(n1 == 5);
    assert(a1[0] == 1 && a1[1] == 2 && a1[2] == 3 && a1[3] == 4 && a1[4] == 5);

    // Test 2: All identical elements
    int a2[] = {7, 7, 7, 7};
    int n2 = removeDuplicates(a2, 4);
    assert(n2 == 1);
    assert(a2[0] == 7);

    // Test 3: No duplicates (already distinct)
    int a3[] = {1, 2, 3, 4};
    int n3 = removeDuplicates(a3, 4);
    assert(n3 == 4);
    assert(a3[0] == 1 && a3[1] == 2 && a3[2] == 3 && a3[3] == 4);

    // Test 4: Single element
    int a4[] = {42};
    int n4 = removeDuplicates(a4, 1);
    assert(n4 == 1);
    assert(a4[0] == 42);

    // Test 5: Negative numbers and zeros
    int a5[] = {-3, -3, -1, 0, 0, 2, 2};
    int n5 = removeDuplicates(a5, 7);
    assert(n5 == 4);
    assert(a5[0] == -3 && a5[1] == -1 && a5[2] == 0 && a5[3] == 2);

    // Test 6: Large array with many duplicates
    int a6[] = {1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 5, 10, 10, 20};
    int n6 = removeDuplicates(a6, 14);
    assert(n6 == 4);
    assert(a6[0] == 1 && a6[1] == 5 && a6[2] == 10 && a6[3] == 20);

    return 0;
}
// The core idea is to use a single pass with two indices: one index (`writeIndex`) tracks where the next distinct element should be placed, and the other (`readIndex`) scans through the array. Since the array is sorted, all duplicates are adjacent. Start `writeIndex` at 0 (the first element is always distinct). For each subsequent element, compare it with the last written element (`arr[writeIndex]`). If the current element differs, increment `writeIndex` and copy the current element there. If it’s equal, skip it. This overwrites duplicates in-place. Edge cases: single-element array (returns 1), all elements identical (returns 1), already distinct (no overwriting occurs), large arrays (no overflow). Time complexity is O(n) because each element is visited once. Space complexity is O(1) as no extra data structures are used. The function is declared as `int removeDuplicates(int arr[], const int n)` to show const-correctness on the size parameter; the array is mutable because we modify it.
