// Write a standalone C++ function `moveZeroesToEnd` that takes a mutable array of integers and its length, and rearranges the elements in-place so that all non-zero elements appear first in their original relative order, followed by all zeros. The function must not use any additional array or container for storage beyond a few scalar variables. The input array may contain negative numbers, duplicates, and zeros anywhere; the output must preserve the relative order of non-zero numbers, and every zero must be moved to the end of the array. The function should handle arrays of length 0 or 1 without error, and must be efficient for large arrays.
The solution uses a two-pointer technique with a single pass. Maintain a pointer `nextNonZero` that marks the position where the next non-zero element should be placed. Iterate through the array with another pointer `current`. Whenever `arr[current]` is non-zero, swap it with `arr[nextNonZero]` and increment `nextNonZero`. This effectively shifts non-zero elements left, preserving their original order, while zeros are pushed to the right as a side effect of the swaps. If a non-zero element is already at its target position (`current == nextNonZero`), swapping with itself is harmless but can be optimized by only swapping when different. Edge cases include all zeros (no swaps, all zeros remain), no zeros (every element is swapped with itself, but no change), and negative numbers (treated as non-zero). The algorithm runs in `O(n)` time and uses `O(1)` extra space. The relative order of non-zero elements is preserved because the algorithm always swaps the current non-zero element with the first available position from the left, thus placing earlier non-zero elements before later ones.
#include <utility>  // for std::swap

// Move all zeros to the end of the array while preserving the relative order of non-zero elements.
// The rearrangement is done in-place.
void moveZeroesToEnd(int arr[], int n) {
    int nextNonZero = 0;  // position where the next non-zero element should be placed

    for (int current = 0; current < n; ++current) {
        if (arr[current] != 0) {
            // Swap only if the current element is not already at the target position.
            if (current != nextNonZero) {
                std::swap(arr[nextNonZero], arr[current]);
            }
            ++nextNonZero;  // advance the target position for the next non-zero element
        }
    }
    // No need to explicitly set zeros; they end up at the tail after all swaps.
}
#include <cassert>
#include <iostream>

// The function under test is declared here for clarity.
void moveZeroesToEnd(int arr[], int n);

int main() {
    // Test 1: Basic case with zeros interspersed.
    int a1[] = {0, 1, 0, 3, 12};
    moveZeroesToEnd(a1, 5);
    assert(a1[0] == 1 && a1[1] == 3 && a1[2] == 12 && a1[3] == 0 && a1[4] == 0);

    // Test 2: All zeros.
    int a2[] = {0, 0, 0};
    moveZeroesToEnd(a2, 3);
    assert(a2[0] == 0 && a2[1] == 0 && a2[2] == 0);

    // Test 3: No zeros.
    int a3[] = {1, 2, 3};
    moveZeroesToEnd(a3, 3);
    assert(a3[0] == 1 && a3[1] == 2 && a3[2] == 3);

    // Test 4: Negative numbers and zeros.
    int a4[] = {-1, 0, -2, 0, -3};
    moveZeroesToEnd(a4, 5);
    assert(a4[0] == -1 && a4[1] == -2 && a4[2] == -3 && a4[3] == 0 && a4[4] == 0);

    // Test 5: Single element zero.
    int a5[] = {0};
    moveZeroesToEnd(a5, 1);
    assert(a5[0] == 0);

    // Test 6: Single element non-zero.
    int a6[] = {42};
    moveZeroesToEnd(a6, 1);
    assert(a6[0] == 42);

    // Test 7: Empty array (n=0) - should not crash.
    int* a7 = nullptr;
    moveZeroesToEnd(a7, 0);

    // Test 8: Zeros at the beginning and end.
    int a8[] = {0, 5, 6, 0};
    moveZeroesToEnd(a8, 4);
    assert(a8[0] == 5 && a8[1] == 6 && a8[2] == 0 && a8[3] == 0);

    // Test 9: Already sorted with zeros at end.
    int a9[] = {1, 2, 0, 0};
    moveZeroesToEnd(a9, 4);
    assert(a9[0] == 1 && a9[1] == 2 && a9[2] == 0 && a9[3] == 0);

    // Test 10: Large values and duplicates.
    int a10[] = {1000000, -1000000, 0, 1000000, 0, -5};
    moveZeroesToEnd(a10, 6);
    assert(a10[0] == 1000000 && a10[1] == -1000000 && a10[2] == 1000000 && a10[3] == -5 && a10[4] == 0 && a10[5] == 0);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
