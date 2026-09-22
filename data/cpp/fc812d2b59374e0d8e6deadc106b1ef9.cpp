Write a C++ function named `shiftAlternatePairs` that takes an array of integers and its length `n` as input and modifies the array in-place by swapping each element at index `i` with the element at index `i+2` for all `i` from 0 to n-3 (i.e., for every valid two-step-apart pair). The function should return `void` and must work for any integer array, including those with an odd or even number of elements. For arrays with fewer than 3 elements, no changes should be made. The function should be efficient, use constant extra space, and respect the original relative order of elements except for the specified swaps.
The core algorithm is a simple loop that iterates `i` from `0` to `n-3` (inclusive). For each iteration, it swaps `arr[i]` and `arr[i+2]` using a standard `std::swap`. This effectively moves elements two positions forward or backward, creating an alternating pattern. For example, with `n=5`, after the operation, the element originally at index 0 moves to index 2, index 2 moves to index 4, index 1 moves to index 3, and index 3 moves to index 1 (via the swap at i=1). Index 4 remains unchanged because there is no index 6. Edge cases: if `n < 3`, the loop condition `i < n-2` is false, so the function does nothing. For `n = 3`, it swaps indices 0 and 2. For even `n`, the last index (n-1) is never swapped because the maximum `i` is `n-3`, so `i+2` becomes `n-1` at the last iteration, meaning `arr[n-1]` swaps with `arr[n-3]` but `arr[n-2]` is also swapped with `arr[n-4]`, etc. The algorithm runs in exactly `O(n)` time (since it does about `n/2` swaps on average, but the loop runs `n-2` times, each O(1)) and uses `O(1)` extra space (only a loop counter and temporary storage for the swap). No special handling for duplicates or negative numbers is needed.
#include <algorithm> // for std::swap

// Swaps every element at index i with the element at index i+2 for i from 0 to n-3.
// If n < 3, the function does nothing.
void shiftAlternatePairs(int arr[], int n) {
    for (int i = 0; i < n - 2; ++i) {
        std::swap(arr[i], arr[i + 2]);
    }
}
#include <cassert>
#include <vector>

// The solution function is declared above; here we test it.
void shiftAlternatePairs(int arr[], int n); // forward declaration

int main() {
    // Test 1: Basic case with even length
    int arr1[] = {1, 2, 3, 4};
    shiftAlternatePairs(arr1, 4);
    assert(arr1[0] == 3 && arr1[1] == 4 && arr1[2] == 1 && arr1[3] == 2);

    // Test 2: Odd length
    int arr2[] = {10, 20, 30, 40, 50};
    shiftAlternatePairs(arr2, 5);
    assert(arr2[0] == 30 && arr2[1] == 40 && arr2[2] == 50 && arr2[3] == 20 && arr2[4] == 10);

    // Test 3: Length 3
    int arr3[] = {5, 6, 7};
    shiftAlternatePairs(arr3, 3);
    assert(arr3[0] == 7 && arr3[1] == 6 && arr3[2] == 5);

    // Test 4: Length 2 (no changes)
    int arr4[] = {8, 9};
    shiftAlternatePairs(arr4, 2);
    assert(arr4[0] == 8 && arr4[1] == 9);

    // Test 5: Length 1 (no changes)
    int arr5[] = {42};
    shiftAlternatePairs(arr5, 1);
    assert(arr5[0] == 42);

    // Test 6: Length 0 (no changes)
    int arr6[] = {};
    shiftAlternatePairs(arr6, 0);
    // No assertion needed, just ensure no crash

    // Test 7: Negative numbers and zeros
    int arr7[] = {-1, 0, -2, 0, -3};
    shiftAlternatePairs(arr7, 5);
    assert(arr7[0] == -2 && arr7[1] == 0 && arr7[2] == -3 && arr7[3] == 0 && arr7[4] == -1);

    // Test 8: Large array with even length, verify pattern
    int arr8[] = {1, 2, 3, 4, 5, 6};
    shiftAlternatePairs(arr8, 6);
    assert(arr8[0] == 3 && arr8[1] == 4 && arr8[2] == 5 && arr8[3] == 6 && arr8[4] == 1 && arr8[5] == 2);

    // Test 9: Duplicate values
    int arr9[] = {7, 7, 7, 7};
    shiftAlternatePairs(arr9, 4);
    assert(arr9[0] == 7 && arr9[1] == 7 && arr9[2] == 7 && arr9[3] == 7);

    return 0;
}
