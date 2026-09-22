// Write a C++ function that reverses the elements of an integer array in place. The function must accept an array and its size as parameters, and it must not create any additional array or container for storage. After reversal, the function should modify the original array so that the first element becomes the last, the second becomes the second-to-last, and so on. The function must handle arrays of any non-negative size, including empty arrays (size 0) and arrays with a single element, which require no changes. You may assume the input array is valid and the size matches the actual number of elements.
#include <cassert>

int main() {
    // Test 1: normal odd-length array
    int a1[] = {5, 2, 1, 3, 8};
    reverseArray(a1, 5);
    assert(a1[0] == 8 && a1[1] == 3 && a1[2] == 1 && a1[3] == 2 && a1[4] == 5);

    // Test 2: even-length array
    int a2[] = {1, 2, 3, 4};
    reverseArray(a2, 4);
    assert(a2[0] == 4 && a2[1] == 3 && a2[2] == 2 && a2[3] == 1);

    // Test 3: single element (no change)
    int a3[] = {42};
    reverseArray(a3, 1);
    assert(a3[0] == 42);

    // Test 4: empty array (size 0) - should not crash or change anything
    int a4[] = {};  // zero-length array is legal in C++ for this test
    reverseArray(a4, 0);

    // Test 5: array with duplicates
    int a5[] = {7, -3, 0, 7, -3};
    reverseArray(a5, 5);
    int expected5[] = {-3, 7, 0, -3, 7};
    for (int i = 0; i < 5; ++i) {
        assert(a5[i] == expected5[i]);
    }
}
#include <cstddef>  // for size_t

// Reverses the elements of array `arr` with `n` elements in place.
void reverseArray(int* arr, size_t n) {
    size_t start = 0;
    size_t end = n > 0 ? n - 1 : 0;

    while (start < end) {
        // Swap the elements at the two pointers.
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;

        // Move pointers toward the center.
        ++start;
        --end;
    }
}
// The solution uses a two-pointer technique: initialize a `start` index at 0 and an `end` index at `n-1`. While `start < end`, swap the elements at those positions, then increment `start` and decrement `end`. This works because each swap correctly places the elements in their reversed positions, and the loop stops when the pointers meet or cross, which is correct for both even and odd sized arrays. For an empty array (`n == 0`) or a single-element array (`n == 1`), the loop condition is immediately false, so the function simply returns without any changes. The algorithm runs in O(n) time (performing at most n/2 swaps) and uses O(1) extra space since everything is done in place. Edge cases include negative numbers (which are just swapped like any other int), duplicates, and large arrays. The function should be marked `const`-correct by taking the array as a non-const pointer (since it modifies the data) but not modifying any other external state.
