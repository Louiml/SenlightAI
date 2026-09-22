/*
Write a C++ function `reverseAndModify` that takes an array of integers and its size, reverses the array in place, and additionally transforms each element so that every element at an even index (0-based) gets multiplied by 2, and every element at an odd index gets decremented by 1. The function should work for both even and odd array sizes, including arrays of length 1. Do not allocate any extra array; perform all operations in-place. The function should return `void` and modify the original array.
*/

#include <utility>   // for std::swap

// Reverse the array in place, then apply index-based transformations:
// even index -> multiply by 2, odd index -> subtract 1.
void reverseAndModify(int arr[], int n) {
    if (n <= 0) return;

    // Step 1: Reverse the array in place.
    int start = 0;
    int end = n - 1;
    while (start < end) {
        std::swap(arr[start], arr[end]);
        ++start;
        --end;
    }

    // Step 2: Transform elements based on their index in the reversed array.
    for (int i = 0; i < n; ++i) {
        if (i % 2 == 0) {
            arr[i] *= 2;   // even index multiply by 2
        } else {
            arr[i] -= 1;   // odd index subtract 1
        }
    }
}

#include <cassert>

// Solution function declaration is assumed to be available (include the header).
void reverseAndModify(int arr[], int n);

int main() {
    // Test 1: even-sized array
    int arr1[] = {1, 2, 3, 4};
    reverseAndModify(arr1, 4);
    int expected1[] = {8, 2, 4, 0}; // reversed: {4,3,2,1} -> transform: {8,2,4,0}
    for (int i = 0; i < 4; ++i) assert(arr1[i] == expected1[i]);

    // Test 2: odd-sized array
    int arr2[] = {10, 20, 30};
    reverseAndModify(arr2, 3);
    int expected2[] = {60, 19, 20}; // reversed: {30,20,10} -> transform: {60,19,20}
    for (int i = 0; i < 3; ++i) assert(arr2[i] == expected2[i]);

    // Test 3: single-element array
    int arr3[] = {5};
    reverseAndModify(arr3, 1);
    assert(arr3[0] == 10); // reversed: {5} -> transform: {10}

    // Test 4: empty array (no operation, no crash)
    int arr4[] = {};
    reverseAndModify(arr4, 0); // should do nothing

    // Test 5: array with all identical elements
    int arr5[] = {7, 7, 7, 7};
    reverseAndModify(arr5, 4);
    int expected5[] = {14, 6, 14, 6};
    for (int i = 0; i < 4; ++i) assert(arr5[i] == expected5[i]);

    // Test 6: negative numbers
    int arr6[] = {-1, -2, -3};
    reverseAndModify(arr6, 3);
    int expected6[] = {-6, -3, -2}; // reversed: {-3,-2,-1} -> transform: {-6,-3,-2}
    for (int i = 0; i < 3; ++i) assert(arr6[i] == expected6[i]);

    return 0;
}

// The solution requires two passes over the array. First, reverse the array using two pointers (`start` at index 0, `end` at `n-1`), swapping elements and moving pointers toward each other until they meet or cross. This is the classic in-place reversal and works for both even and odd lengths. After reversal, iterate through the array once more and apply the transformation based on index: for even indices, multiply by 2; for odd indices, subtract 1. Edge cases include an empty array (size 0) where no operation is needed, and a single-element array where the element is at index 0 and gets doubled after reversal (which is a no-op reversal). The time complexity is O(n) for the reversal plus O(n) for the transformation, so O(n) overall. Space complexity is O(1) extra space since all operations are in-place.
