// Write a C++ function named `reverseArrayInPlace` that takes a non-empty array of integers and its size as parameters, reverses the order of the elements in the array in-place (without allocating a second array), and returns nothing (void). The function must handle arrays of any positive size, including size 1 where no changes are needed. The function should use pointer arithmetic or index swapping with two pointers moving toward each other. The solution must not use any standard library algorithms like `std::reverse` — implement the reversal manually. After the function call, the original array must contain the elements in reversed order. Provide a free function (not a member of a class) with proper `const` correctness — meaning you must pass the array as a non-const pointer to int so that modifications affect the caller's data.
#include <cassert>

// The solution function is declared above (in the Solution section).
int main() {
    // Test 1: standard reversal
    int a1[] = {1, 2, 3, 4, 5};
    reverseArrayInPlace(a1, 5);
    assert(a1[0] == 5 && a1[1] == 4 && a1[2] == 3 && a1[3] == 2 && a1[4] == 1);

    // Test 2: even number of elements
    int a2[] = {10, 20, 30, 40};
    reverseArrayInPlace(a2, 4);
    assert(a2[0] == 40 && a2[1] == 30 && a2[2] == 20 && a2[3] == 10);

    // Test 3: single element (no change)
    int a3[] = {42};
    reverseArrayInPlace(a3, 1);
    assert(a3[0] == 42);

    // Test 4: two elements
    int a4[] = {7, 8};
    reverseArrayInPlace(a4, 2);
    assert(a4[0] == 8 && a4[1] == 7);

    // Test 5: negative numbers and zeros
    int a5[] = {-1, 0, 5, -3};
    reverseArrayInPlace(a5, 4);
    assert(a5[0] == -3 && a5[1] == 5 && a5[2] == 0 && a5[3] == -1);

    // Test 6: repeated values
    int a6[] = {3, 3, 3};
    reverseArrayInPlace(a6, 3);
    assert(a6[0] == 3 && a6[1] == 3 && a6[2] == 3);

    // Edge case: size 0 is not required but code should not crash
    int* a7 = nullptr;
    reverseArrayInPlace(a7, 0); // Should be safe if we guard size <= 1

    return 0;
}
#include <utility> // for std::swap (optional, but allowed)

// Reverse the elements of an integer array in-place.
// The function does not return a value; it modifies the array passed in.
void reverseArrayInPlace(int* arr, int size) {
    if (size <= 1) {
        return; // nothing to reverse
    }
    int left = 0;
    int right = size - 1;
    while (left < right) {
        // Swap elements manually (no std::swap required, but can use it)
        int tmp = arr[left];
        arr[left] = arr[right];
        arr[right] = tmp;
        ++left;
        --right;
    }
}
// The main algorithm uses two indices: one starting at the beginning (`left = 0`) and one at the end (`right = size - 1`). In each iteration, swap the elements at these indices, then increment `left` and decrement `right`. Continue while `left < right`. If the array has an even number of elements, all pairs are swapped; if odd, the middle element stays in place because the loop stops when left equals right. Edge cases: size 0 (but task specifies non-empty, so no need to handle, but still safe to guard), size 1 (loop does nothing, array unchanged). Time complexity is O(n/2) which simplifies to O(n) because each element is involved in at most one swap. Space complexity is O(1) since only a temporary variable for swapping is used, and no extra array or data structure is allocated.
