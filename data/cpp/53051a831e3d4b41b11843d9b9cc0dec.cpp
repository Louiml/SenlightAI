// Write a C++ function `zeroOutFirstTwo(int* arr, size_t n)` that takes a pointer to an array of integers and its size, and modifies the array in place so that the first two elements are set to zero. If the array has fewer than two elements, the function should do nothing. Additionally, write a helper function `int getSecondElement(const int* arr, size_t n)` that returns the second element (index 1) of the array, but if the array has fewer than two elements, return `-1` to indicate an error. The main challenge is to correctly handle edge cases with small arrays and to demonstrate safe pointer arithmetic and bounds checking. You are not allowed to use any standard library containers (like `std::vector`), only raw arrays and pointers. The functions must be `const`-correct where appropriate (the helper function must take a pointer-to-const).
#include <cassert>

int main() {
    // Test with a normal array
    int a[5] = {10, 20, 30, 40, 50};
    assert(getSecondElement(a, 5) == 20);
    zeroOutFirstTwo(a, 5);
    assert(a[0] == 0 && a[1] == 0 && a[2] == 30);

    // Test with exactly two elements
    int b[2] = {7, 8};
    assert(getSecondElement(b, 2) == 8);
    zeroOutFirstTwo(b, 2);
    assert(b[0] == 0 && b[1] == 0);

    // Test with one element: should not modify and helper returns -1
    int c[1] = {99};
    assert(getSecondElement(c, 1) == -1);
    zeroOutFirstTwo(c, 1);
    assert(c[0] == 99);

    // Test with empty array (size 0)
    int* d = nullptr;
    assert(getSecondElement(d, 0) == -1);
    zeroOutFirstTwo(d, 0); // should do nothing, no crash

    // Test with array of size 2 but negative values
    int e[2] = {-5, -10};
    assert(getSecondElement(e, 2) == -10);
    zeroOutFirstTwo(e, 2);
    assert(e[0] == 0 && e[1] == 0);

    return 0;
}
#include <cstddef> // for size_t

// Sets the first two elements of the array to zero if the array has at least two elements.
// If n < 2, the function does nothing (no memory access outside bounds).
void zeroOutFirstTwo(int* arr, size_t n) {
    if (n >= 2) {
        arr[0] = 0;
        arr[1] = 0;
    }
}

// Returns the second element (index 1) of the array if it exists.
// If n < 2, returns -1 to indicate an error.
// The input pointer is const to promise not to modify the array.
int getSecondElement(const int* arr, size_t n) {
    if (n >= 2) {
        return arr[1];
    }
    return -1;
}
// The solution involves two functions. For `zeroOutFirstTwo`, we simply check if `n >= 2`; if so, we set `arr[0] = 0` and `arr[1] = 0`. This modifies the original array. The main edge case is when `n < 2`, in which case we must not write to memory that is not part of the array (to avoid undefined behavior). For `getSecondElement`, we first check if `n >= 2`; if yes, return `arr[1]`, otherwise return `-1`. The function takes a pointer-to-const to promise not to modify the array. Time complexity for both is O(1) because only constant operations are performed (bounds checks and possibly assignments/reads). Space complexity is O(1) as no extra storage is used. The pointer arithmetic and dereferencing are straightforward, but care must be taken to ensure the size is checked before any access.
