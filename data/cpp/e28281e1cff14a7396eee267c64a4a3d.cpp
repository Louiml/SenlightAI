Write a C++ function named `transformArray` that takes a dynamically allocated integer array and its size `N` as parameters. The function should modify the array in place by iterating through its elements (using pointer arithmetic, not array indexing) and applying the following transformation: for each position `i` (0-based), if `(i + 1)` is even (i.e., the element is at an even position when counting from 1), set the element to `0`; otherwise (odd position), multiply the element by `2`. The function should not return anything, must work for any non-negative integer `N` (including `0`), and should properly handle `N = 0` (do nothing). After transformation, the function should also output the modified array to the standard output, with elements separated by a single space and followed by a newline. Assume the input array already contains valid integer values. Do not allocate, free, or reallocate any memory inside the function; the caller is responsible for memory management. Use `const` only where appropriate (note: since the array is modified, it cannot be const for the array parameter itself, but the pointer can be considered non-const).
#include <cassert>
#include <sstream>
#include <iostream>

// Declare the function to test (usually from the solution header)
void transformArray(int* arr, int N);

// Helper to capture output for testing
std::string captureOutput(int* arr, int N) {
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    transformArray(arr, N);
    std::cout.rdbuf(old);
    return oss.str();
}

int main() {
    // Test 1: Basic 5-element array
    int arr1[] = {3, 7, 2, 9, 5};
    transformArray(arr1, 5);
    assert(arr1[0] == 6);   // odd (1) -> double
    assert(arr1[1] == 0);   // even (2) -> zero
    assert(arr1[2] == 4);   // odd (3) -> double
    assert(arr1[3] == 0);   // even (4) -> zero
    assert(arr1[4] == 10);  // odd (5) -> double

    // Test 2: N = 0 (nothing to do, output just newline)
    int* empty = nullptr;
    assert(captureOutput(empty, 0) == "\n");

    // Test 3: N = 1 (single element, odd position -> double)
    int arr3[] = {5};
    assert(captureOutput(arr3, 1) == "10\n");
    assert(arr3[0] == 10);

    // Test 4: Even-sized array
    int arr4[] = {1, 2, 3, 4};
    transformArray(arr4, 4);
    assert(arr4[0] == 2); // odd -> double
    assert(arr4[1] == 0); // even -> zero
    assert(arr4[2] == 6); // odd -> double
    assert(arr4[3] == 0); // even -> zero

    // Test 5: All zeros input
    int arr5[] = {0, 0, 0};
    transformArray(arr5, 3);
    assert(arr5[0] == 0);
    assert(arr5[1] == 0);
    assert(arr5[2] == 0);

    // Test 6: Negative numbers
    int arr6[] = {-4, -3, -2};
    transformArray(arr6, 3);
    assert(arr6[0] == -8); // odd -> double
    assert(arr6[1] == 0);  // even -> zero
    assert(arr6[2] == -4); // odd -> double

    // Test 7: Output formatting check (single space separator, newline at end)
    int arr7[] = {1, 2, 3};
    assert(captureOutput(arr7, 3) == "2 0 6\n");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <iostream>

// Transform an array in place: set even positions (1-based) to 0, odd positions to double.
// Uses pointer arithmetic and outputs the resulting array followed by a newline.
void transformArray(int* arr, int N) {
    for (int i = 0; i < N; ++i) {
        if ((i + 1) % 2 == 0) {
            *(arr + i) = 0;          // even position (1-based) -> set to zero
        } else {
            *(arr + i) *= 2;         // odd position (1-based) -> double
        }
    }
    for (int i = 0; i < N; ++i) {
        std::cout << *(arr + i);
        if (i < N - 1) std::cout << ' ';
    }
    std::cout << '\n';
}
// The solution iterates over the array using pointer arithmetic: `*(arr + i)` gives the element at index `i`. For each index `i`, we check if `(i + 1) % 2 == 0` — if true, set the current element to `0` (which is the same as multiplying by zero); otherwise, multiply it by `2`. The transformation is in-place; no extra array is needed. For `N = 0`, the loop does not execute, and the output should print only a newline (the function should handle this gracefully). Edge case: `rand()` values are not relevant here because the function operates on already existing array contents. The time complexity is O(N) because we visit each element exactly once. The space complexity is O(1) auxiliary, as we only use a loop variable and no additional storage. Pointer arithmetic is used consistently, avoiding array indexing syntax. The function outputs the modified array after processing.
