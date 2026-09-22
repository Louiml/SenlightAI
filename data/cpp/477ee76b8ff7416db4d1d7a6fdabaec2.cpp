/*
Given an array of integers, write a C++ function `findSecondLargest` that takes an array and its size as parameters and returns the value of the second largest distinct element in the array. If the array contains fewer than two distinct values, return -1. The function must handle arrays with negative numbers, duplicates, and unsorted order. For example, for input `{1, 2, 3, 4, 5}` it returns `4`, for `{5, 5, 5}` it returns `-1`, and for `{-3, -1, -2}` it returns `-2`. The function should not modify the input array and must work for any non-empty array.
*/

#include <climits>
#include <cstddef>

// Returns the second largest distinct value in arr, or -1 if fewer than two distinct values exist.
int findSecondLargest(const int* arr, std::size_t n) {
    if (n < 2) {
        return -1;
    }

    int largest = arr[0];
    bool hasSecond = false;
    int secondLargest = 0; // only valid when hasSecond is true

    for (std::size_t i = 1; i < n; ++i) {
        int current = arr[i];
        if (current > largest) {
            secondLargest = largest;
            hasSecond = true;
            largest = current;
        } else if (current < largest) {
            if (!hasSecond || current > secondLargest) {
                secondLargest = current;
                hasSecond = true;
            }
        }
        // If current == largest, ignore duplicate.
    }

    return hasSecond ? secondLargest : -1;
}

#include <cassert>

int main() {
    // Basic sorted ascending
    int arr1[] = {1, 2, 3, 4, 5};
    assert(findSecondLargest(arr1, 5) == 4);

    // Sorted descending
    int arr2[] = {9, 7, 5, 3, 1};
    assert(findSecondLargest(arr2, 5) == 7);

    // All duplicates
    int arr3[] = {7, 7, 7, 7};
    assert(findSecondLargest(arr3, 4) == -1);

    // Duplicates but with distinct values
    int arr4[] = {4, 4, 3, 3, 2, 2};
    assert(findSecondLargest(arr4, 6) == 3);

    // Negative numbers
    int arr5[] = {-3, -1, -2};
    assert(findSecondLargest(arr5, 3) == -2);

    // Single element
    int arr6[] = {42};
    assert(findSecondLargest(arr6, 1) == -1);

    // Two distinct elements, largest first
    int arr7[] = {10, 5};
    assert(findSecondLargest(arr7, 2) == 5);

    // Two distinct elements, second largest is negative
    int arr8[] = {0, -4};
    assert(findSecondLargest(arr8, 2) == -4);

    // Unsorted with largest at the end
    int arr9[] = {3, 8, 1, 9, 2};
    assert(findSecondLargest(arr9, 5) == 8);

    // Array with one large, rest equal smaller
    int arr10[] = {100, 1, 1, 1};
    assert(findSecondLargest(arr10, 4) == 1);

    return 0;
}

// The solution uses a single pass to track the largest and second largest distinct values. Initialize `largest` to the first element and `secondLargest` to a sentinel value like `INT_MIN` (or a boolean flag to indicate "not found"). Iterate through the array starting from index 1. For each element:
// - If it is greater than `largest`, update `secondLargest` to the old `largest`, then set `largest` to the new value.
// - Else if it is strictly between `secondLargest` and `largest` (i.e., greater than `secondLargest` and not equal to `largest`), update `secondLargest`.
// - Ignore values equal to `largest` because duplicates don't create a new distinct second largest.
//
// Edge cases:  
// - If all elements are equal, `secondLargest` remains as the sentinel/flag, and we return -1.  
// - If the array has exactly one element, also return -1 because fewer than two distinct values exist.  
// - Negative numbers are handled naturally because comparisons work fine with negatives.  
// - The sentinel must be chosen carefully if using `INT_MIN`, but using a boolean flag is safer and avoids reliance on extreme values.
//
// Time complexity: O(n) where n is the array size (single traversal). Space complexity: O(1) auxiliary space (only two variables and a flag). The function is `const`-correct by taking the array as `const int*` to guarantee no modification.
