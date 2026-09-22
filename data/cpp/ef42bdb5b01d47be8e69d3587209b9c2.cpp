Write a C++ function that processes an array of integers. The function must be named `sumBetweenFirstAndLastZero`, take a `const int[]` array and its size as parameters, and return an `int`. It should compute and return the sum of all elements located strictly between the first occurrence of zero and the last occurrence of zero in the array. If the array contains fewer than two zero elements (i.e., zero or only one zero), the function must return 0. The function must handle arrays with negative values, duplicates, and zeros at any positions. The array size is guaranteed to be a positive integer (≥1), but the function must still work correctly for any valid array size.
// The solution requires scanning the input array once to locate the indices of the first and last zero elements. Initialize two integer variables: `firstZeroIndex = -1` and `lastZeroIndex = -1`. Iterate through each element; if an element equals zero, set `firstZeroIndex` to the current index if it has not been set yet (i.e., if it’s still -1), and always update `lastZeroIndex` to the current index. After the loop, if either `firstZeroIndex` or `lastZeroIndex` remains -1, or if both indices are equal (meaning there is only one zero), return 0. Otherwise, iterate from `firstZeroIndex + 1` up to (but not including) `lastZeroIndex`, accumulating the sum and return it. Edge cases include: no zeros → return 0; exactly one zero → return 0; zeros at the very first and last positions → sum of all elements in between (possibly zero if adjacent zeros); negative values do not affect the logic. Time complexity is O(n) because the array is traversed twice at most (once for finding indices, once for summing), and space complexity is O(1) since only a few scalar variables are used.
#include <cstddef> // for size_t if needed, but using int for simplicity
#include <iostream> // for std::ostream, not required for function

// Computes the sum of elements between the first and last zero in an array.
// Returns 0 if there are fewer than two zeros.
int sumBetweenFirstAndLastZero(const int arr[], int size) {
    int firstZeroIndex = -1;
    int lastZeroIndex = -1;

    // Find first and last zero indices
    for (int i = 0; i < size; ++i) {
        if (arr[i] == 0) {
            if (firstZeroIndex == -1) {
                firstZeroIndex = i;
            }
            lastZeroIndex = i;
        }
    }

    // If fewer than two zeros or only one zero, return 0
    if (firstZeroIndex == -1 || lastZeroIndex == -1 || firstZeroIndex == lastZeroIndex) {
        return 0;
    }

    // Sum elements strictly between the two zeros
    int sum = 0;
    for (int i = firstZeroIndex + 1; i < lastZeroIndex; ++i) {
        sum += arr[i];
    }

    return sum;
}
#include <cassert>

int main() {
    // Multiple zeros with elements between
    int arr1[] = {1, 0, 2, 3, 0, 4};
    assert(sumBetweenFirstAndLastZero(arr1, 6) == 5); // 2+3

    // No zeros
    int arr2[] = {1, 2, 3};
    assert(sumBetweenFirstAndLastZero(arr2, 3) == 0);

    // One zero only
    int arr3[] = {0, 5, 6};
    assert(sumBetweenFirstAndLastZero(arr3, 3) == 0);

    // Zeros at edges, sum of middle
    int arr4[] = {0, 7, 8, 0};
    assert(sumBetweenFirstAndLastZero(arr4, 4) == 15); // 7+8

    // Adjacent zeros (no elements between)
    int arr5[] = {0, 0, 9};
    assert(sumBetweenFirstAndLastZero(arr5, 3) == 0);

    // Negative values
    int arr6[] = {-5, 0, -2, -3, 0, 10};
    assert(sumBetweenFirstAndLastZero(arr6, 6) == -5); // -2 + -3

    // Single element array (no zeros)
    int arr7[] = {42};
    assert(sumBetweenFirstAndLastZero(arr7, 1) == 0);

    // Single zero
    int arr8[] = {0};
    assert(sumBetweenFirstAndLastZero(arr8, 1) == 0);

    // Three zeros: first and last include middle zero
    int arr9[] = {0, 1, 0, 2, 0};
    assert(sumBetweenFirstAndLastZero(arr9, 5) == 3); // 1+0+2

    // Zeros at same position (only one zero) with other elements
    int arr10[] = {4, 0, 5};
    assert(sumBetweenFirstAndLastZero(arr10, 3) == 0);

    return 0;
}
