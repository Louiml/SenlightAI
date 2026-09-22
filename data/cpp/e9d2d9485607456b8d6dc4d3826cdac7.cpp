/*
Write a C++ function `void reverseArrayRecursive(int arr[], int start, int end)` that reverses the elements of an integer array in place using only recursion and without using any loop, extra array, or built-in reversal function. The function should accept the array, the starting index (0), and the ending index (n-1) where n is the size of the array. The function must swap the first and last elements, then recursively process the inner subarray. Test the function with an array of size 8 containing integers 0 through 7, and verify the output is `76543210` after reversal.
*/

#include <vector>
#include <cstddef>

// Recursively reverse the elements of an integer array in place.
// The range to reverse is from index 'start' to index 'end' inclusive.
// Base case: when start >= end, no more swapping is needed.
void reverseArrayRecursive(int arr[], int start, int end) {
    // Base case: if there are zero or one elements left, do nothing.
    if (start >= end) {
        return;
    }
    // Swap the first and last elements of the current subarray.
    int temp = arr[start];
    arr[start] = arr[end];
    arr[end] = temp;
    // Recursively reverse the inner subarray.
    reverseArrayRecursive(arr, start + 1, end - 1);
}

#include <cassert>

int main() {
    // Test case: array of size 8 with values 0..7
    int arr1[] = {0, 1, 2, 3, 4, 5, 6, 7};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    reverseArrayRecursive(arr1, 0, size1 - 1);
    int expected1[] = {7, 6, 5, 4, 3, 2, 1, 0};
    for (int i = 0; i < size1; ++i) {
        assert(arr1[i] == expected1[i]);
    }

    // Test case: even larger array
    int arr2[] = {10, 20, 30, 40};
    int size2 = sizeof(arr2) / sizeof(arr2[0]);
    reverseArrayRecursive(arr2, 0, size2 - 1);
    assert(arr2[0] == 40 && arr2[1] == 30 && arr2[2] == 20 && arr2[3] == 10);

    // Test case: single element (no change)
    int arr3[] = {99};
    reverseArrayRecursive(arr3, 0, 0);
    assert(arr3[0] == 99);

    // Test case: empty range (start > end) should not crash
    int arr4[] = {1, 2, 3};
    reverseArrayRecursive(arr4, 2, 1); // start > end → no operation
    assert(arr4[0] == 1 && arr4[1] == 2 && arr4[2] == 3);

    // Test case: already reversed array (should un-reverse to original)
    int arr5[] = {5, 4, 3, 2, 1};
    reverseArrayRecursive(arr5, 0, 4);
    int expected5[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; ++i) {
        assert(arr5[i] == expected5[i]);
    }
    return 0;
}

// The solution uses a recursive divide-and-conquer approach. At each recursive call, we swap the element at the `start` index with the element at the `end` index, then recurse with `start+1` and `end-1`. The base case occurs when `start >= end` (or `start == end` for odd-length arrays), at which point the subarray has zero or one element and requires no further swapping. This works because the recursion systematically moves the pointers inward until they meet or cross, ensuring every element is swapped exactly once. Edge cases include empty arrays (where `start > end` should be handled gracefully by checking `start >= end`) and arrays with a single element (where the swap does nothing but the function still terminates correctly). The `swap` operation can be done via a temporary variable or using XOR/arithmetic tricks, but a temporary is safest and clearest. Time complexity is O(n) since each element is visited once, and space complexity is O(n) due to the recursion call stack depth (for an array of size n). For large arrays, this recursion depth could cause stack overflow, but for the given test size it is safe.
