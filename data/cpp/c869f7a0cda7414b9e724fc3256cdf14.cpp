Write a C++ function named `reverseArrayInPlace` that takes a non-empty array of integers (passed as a pointer) and its size, and reverses the order of its elements in place without using any additional array or container. The function should modify the original array, not return a new one. After the function returns, the first element of the array should be the last original element, and so on. The function must work correctly for arrays of any size, including size 1 (where the array remains unchanged). You must apply `const` correctness appropriately: the size parameter should be treated as read-only, but the array elements themselves are mutable.

// The main algorithm is a two-pointer swap approach. We start with two indices: `left = 0` and `right = cap - 1`. We swap the elements at these positions, then increment `left` and decrement `right`, continuing until `left >= right`. This works because each swap moves the outer elements to their correct mirrored positions. For an array of even length, the loop runs exactly `cap/2` times; for odd length, the middle element is left untouched. Edge cases: an empty array is not allowed per the task (non-empty), but if it were, the loop would do nothing since `cap/2` is 0. Size 1 requires no swaps, and the loop correctly exits after the first check because `0 < 0` is false. Time complexity is O(n) where n is the array size, since each element is involved in at most one swap. Space complexity is O(1) because we only use a few integer variables, no extra storage.

#include <utility>  // for std::swap

// Reverses the order of elements in the array pointed to by arr,
// given its size. The array is modified in place.
void reverseArrayInPlace(int* arr, const int size) {
    // Use two pointers: one from the start, one from the end.
    // Continue until the pointers meet or cross.
    for (int left = 0, right = size - 1; left < right; ++left, --right) {
        std::swap(arr[left], arr[right]);
    }
}

#include <cassert>

void reverseArrayInPlace(int* arr, const int size);

int main() {
    // Test 1: Typical array
    int a1[] = {1, 2, 3, 4, 5};
    reverseArrayInPlace(a1, 5);
    assert(a1[0] == 5 && a1[1] == 4 && a1[2] == 3 && a1[3] == 2 && a1[4] == 1);

    // Test 2: Even length
    int a2[] = {10, 20, 30, 40};
    reverseArrayInPlace(a2, 4);
    assert(a2[0] == 40 && a2[1] == 30 && a2[2] == 20 && a2[3] == 10);

    // Test 3: Single element (should remain unchanged)
    int a3[] = {42};
    reverseArrayInPlace(a3, 1);
    assert(a3[0] == 42);

    // Test 4: Two elements
    int a4[] = {7, -3};
    reverseArrayInPlace(a4, 2);
    assert(a4[0] == -3 && a4[1] == 7);

    // Test 5: Negative numbers and zeros
    int a5[] = {-1, 0, 5, -2, 3};
    reverseArrayInPlace(a5, 5);
    assert(a5[0] == 3 && a5[1] == -2 && a5[2] == 5 && a5[3] == 0 && a5[4] == -1);

    // Test 6: Duplicate values
    int a6[] = {8, 8, 8};
    reverseArrayInPlace(a6, 3);
    assert(a6[0] == 8 && a6[1] == 8 && a6[2] == 8);

    return 0;
}
