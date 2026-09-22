// Write a C++ function that takes an array of integers and its length, and returns the maximum value in the array. The array is guaranteed to be non-empty (length ≥ 1). The function must not modify the input array, must handle negative values and duplicates correctly, and should have a descriptive name. The solution should be self-contained with appropriate headers and const correctness.
// The algorithm is a simple linear scan. Initialize a variable `maxVal` with the first element of the array. Then iterate from index 1 to n-1, comparing each element with `maxVal`. If an element is larger, update `maxVal`. Since the array is guaranteed non-empty, accessing `a[0]` is safe. Duplicate values and negative numbers require no special handling because we compare with `>`, so equal values do not overwrite. Time complexity is O(n) for n elements, and space complexity is O(1) auxiliary (no extra storage beyond a single variable). Edge cases: length 1 returns that single element; all negative numbers returns the largest (closest to zero) negative; all equal values returns that value.
#include <vector> // not required but included for completeness
#include <algorithm> // for std::max, but we implement manually for clarity

// Returns the maximum value in a non-empty integer array.
// The input array is not modified; we use const correctness.
int findMaximum(const int* const arr, const int length) {
    // Precondition: length >= 1
    int maxVal = arr[0];
    for (int i = 1; i < length; ++i) {
        if (arr[i] > maxVal) {
            maxVal = arr[i];
        }
    }
    return maxVal;
}
#include <cassert>

int main() {
    // Test 1: normal positive numbers
    int arr1[] = {3, 7, 2, 9, 1};
    assert(findMaximum(arr1, 5) == 9);

    // Test 2: all negative numbers
    int arr2[] = {-5, -1, -10};
    assert(findMaximum(arr2, 3) == -1);

    // Test 3: single element
    int arr3[] = {42};
    assert(findMaximum(arr3, 1) == 42);

    // Test 4: duplicates
    int arr4[] = {4, 4, 4};
    assert(findMaximum(arr4, 3) == 4);

    // Test 5: mixed with negatives and positives
    int arr5[] = {-3, 0, -7, 5, -1};
    assert(findMaximum(arr5, 5) == 5);

    // Test 6: largest at the beginning
    int arr6[] = {100, 1, 2, 3};
    assert(findMaximum(arr6, 4) == 100);

    // Test 7: largest at the end
    int arr7[] = {1, 2, 3, 100};
    assert(findMaximum(arr7, 4) == 100);

    // Test 8: all equal negative
    int arr8[] = {-2, -2, -2};
    assert(findMaximum(arr8, 3) == -2);

    // Test 9: mixed with zero
    int arr9[] = {0, -1, -5};
    assert(findMaximum(arr9, 3) == 0);

    // Test 10: large length with alternating values
    int arr10[] = {1, 9, 2, 8, 3, 7, 4, 6, 5};
    assert(findMaximum(arr10, 9) == 9);

    return 0;
}
