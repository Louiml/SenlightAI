Given an array of integers stored in a structure `Array` (with fields `A[20]` for elements, `size` for capacity, and `length` for current element count), write a C++ function `void Rearrange(struct Array *arr)` that rearranges the elements in-place so that all negative numbers appear before all non-negative numbers, preserving the relative order of elements within each group (negative group and non-negative group). The function must work on any valid array with positive length and handle edge cases like all negative, all non-negative, or mixed values. Do not use any extra array or dynamic allocation; modify the original array directly. The function signature must match exactly, and the function should be declared `const`-correct where applicable. For example, given `{2, -3, 5, -10, -15, 17}`, the result should be `{-3, -10, -15, 2, 5, 17}`.

// The core algorithm is a stable partition: we iterate through the array with an index `i` that tracks the position where the next negative number should be placed. For each element, if it is negative, we shift all elements from `i` to the current index one position to the right and then place the negative element at position `i`, incrementing `i`. This preserves relative order of negatives and non-negatives because we process left-to-right and always insert negatives in sequence. For non-negative elements, we simply move on. Edge cases: an array with no negatives or no non-negatives will remain unchanged; an array with a single element is trivially correct. The time complexity is \(O(n^2)\) in the worst case due to shifting, but it uses \(O(1)\) extra space. Alternatively, a stable partition with a temporary buffer would be \(O(n)\) time but violates the no-extra-allocation requirement. The in-place shifting method meets the specification, though not optimal in time, it is correct and clear for small fixed-size arrays (max 20 elements).

#include <cstddef>  // for size_t, though not strictly needed

// Rearranges the array so that all negative numbers appear before non-negative numbers,
// preserving relative order within each group. Works in-place with O(1) auxiliary space.
void Rearrange(struct Array *arr) {
    if (arr == nullptr || arr->length <= 1) return;

    int insertPos = 0;  // index where the next negative should be placed

    for (int i = 0; i < arr->length; ++i) {
        if (arr->A[i] < 0) {
            // Save the negative value
            int temp = arr->A[i];

            // Shift elements from insertPos to i-1 to the right by one
            for (int j = i; j > insertPos; --j) {
                arr->A[j] = arr->A[j - 1];
            }

            // Place the negative value at its correct position
            arr->A[insertPos] = temp;

            // Update insertion position for the next negative
            ++insertPos;
        }
    }
    // Non-negative elements are already in correct relative order
}

#include <cassert>
#include <iostream>

struct Array {
    int A[20];
    int size;
    int length;
};

// Function declaration (must be visible in test file)
void Rearrange(struct Array *arr);

int main() {
    // Test 1: Mixed array
    Array arr1 = {{2, -3, 5, -10, -15, 17}, 10, 6};
    Rearrange(&arr1);
    int expected1[] = {-3, -10, -15, 2, 5, 17};
    for (int i = 0; i < 6; ++i) assert(arr1.A[i] == expected1[i]);

    // Test 2: All negatives
    Array arr2 = {{-1, -2, -3}, 10, 3};
    Rearrange(&arr2);
    int expected2[] = {-1, -2, -3};
    for (int i = 0; i < 3; ++i) assert(arr2.A[i] == expected2[i]);

    // Test 3: All non-negatives (including zero)
    Array arr3 = {{0, 1, 2, 3}, 10, 4};
    Rearrange(&arr3);
    int expected3[] = {0, 1, 2, 3};
    for (int i = 0; i < 4; ++i) assert(arr3.A[i] == expected3[i]);

    // Test 4: Single element negative
    Array arr4 = {{-5}, 10, 1};
    Rearrange(&arr4);
    assert(arr4.A[0] == -5);

    // Test 5: Single element non-negative
    Array arr5 = {{7}, 10, 1};
    Rearrange(&arr5);
    assert(arr5.A[0] == 7);

    // Test 6: Already partitioned
    Array arr6 = {{-1, -2, 3, 4}, 10, 4};
    Rearrange(&arr6);
    int expected6[] = {-1, -2, 3, 4};
    for (int i = 0; i < 4; ++i) assert(arr6.A[i] == expected6[i]);

    // Test 7: Mixed with repeated values
    Array arr7 = {{1, -1, 2, -2, 3, -3}, 10, 6};
    Rearrange(&arr7);
    int expected7[] = {-1, -2, -3, 1, 2, 3};
    for (int i = 0; i < 6; ++i) assert(arr7.A[i] == expected7[i]);

    // Test 8: Large negative at the end
    Array arr8 = {{1, 2, -3}, 10, 3};
    Rearrange(&arr8);
    int expected8[] = {-3, 1, 2};
    for (int i = 0; i < 3; ++i) assert(arr8.A[i] == expected8[i]);

    return 0;
}
