// Implement a C++ function named `sortIntegers` that takes a dynamically allocated array of integers (`int* arr`) and its size (`int size`) as parameters, sorts the array in ascending order, and returns `void`. The function must implement the **insertion sort** algorithm exactly as described below, and it must handle arrays with duplicate values and arrays where elements are already sorted. The function should not use any external sorting functions like `std::sort`. You are free to add a small helper function for swapping, but the main sorting logic must be in `sortIntegers`. The function must be correct for all integer arrays of any size ≥ 0. For size 0 or 1, the function should do nothing. The solution must be compiled with a standard C++17 compiler, and you must include any necessary headers (e.g., `<algorithm>` for `std::swap` is allowed, but you can also implement your own swap). You are encouraged to use `const` correctness where appropriate (e.g., the size parameter should be `const int size`), but the array itself is mutable. Your output should be a single, self-contained function definition without a `main` function.
// The task is to implement insertion sort on an array of integers. The algorithm works by building the sorted portion of the array one element at a time. For each index `i` from 1 to `size-1`, we take the element at `arr[i]` and insert it into the correct position in the sorted subarray `arr[0..i-1]`. This is done by shifting larger elements to the right. The standard implementation uses a temporary variable to store the current element, then a `while` loop that shifts elements while they are greater than the current element. Important edge cases: (1) If the array is empty (size=0) or has one element (size=1), the function does nothing. (2) Duplicate values are handled naturally because the algorithm uses `>` (not `>=`) when comparing, so equal values are not swapped across each other, preserving stability, though that is not strictly required. (3) The algorithm is efficient on nearly-sorted data because the inner loop terminates quickly; worst-case complexity is O(n²) for reverse-sorted arrays, best-case is O(n) for already-sorted. Space complexity is O(1) auxiliary because we only use a few local variables.
#include <cstddef> // for size_t

// Sorts an integer array in ascending order using insertion sort.
// arr: pointer to the first element of the array (mutable)
// size: number of elements in the array (non-negative)
void sortIntegers(int* arr, const int size) {
    if (size <= 1) {
        return; // Nothing to sort for empty or single-element arrays
    }

    // Start from the second element (index 1)
    for (int i = 1; i < size; ++i) {
        // Store the current element to insert
        int key = arr[i];
        int j = i - 1;

        // Shift elements of arr[0..i-1] that are greater than key
        // to one position to their right
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }

        // Place the key in its correct position
        arr[j + 1] = key;
    }
}
#include <cassert>

// Forward declaration of the function under test
void sortIntegers(int* arr, const int size);

int main() {
    // Test empty array
    int emptyArr[] = {};
    sortIntegers(emptyArr, 0); // Should not crash

    // Test single element
    int singleArr[] = {42};
    sortIntegers(singleArr, 1);
    assert(singleArr[0] == 42);

    // Test already sorted array
    int sortedArr[] = {1, 2, 3, 4, 5};
    sortIntegers(sortedArr, 5);
    assert(sortedArr[0] == 1 && sortedArr[4] == 5);

    // Test reverse sorted array
    int reverseArr[] = {9, 8, 7, 6, 5};
    sortIntegers(reverseArr, 5);
    assert(reverseArr[0] == 5 && reverseArr[4] == 9);

    // Test duplicate values
    int dupArr[] = {3, 1, 2, 1, 3};
    sortIntegers(dupArr, 5);
    assert(dupArr[0] == 1 && dupArr[1] == 1 && dupArr[4] == 3);

    // Test negative numbers and zeros
    int mixedArr[] = {0, -5, 10, -2, 3, -5};
    sortIntegers(mixedArr, 6);
    assert(mixedArr[0] == -5 && mixedArr[1] == -5 && mixedArr[5] == 10);

    // Test large array (basic check with a few known values)
    int largeArr[] = {100, 1, 50, 2, 99, 3, 98, 4, 97, 5};
    sortIntegers(largeArr, 10);
    assert(largeArr[0] == 1 && largeArr[9] == 100);

    // Test all zeros
    int zerosArr[] = {0, 0, 0, 0};
    sortIntegers(zerosArr, 4);
    assert(zerosArr[0] == 0 && zerosArr[3] == 0);

    // Test alternating high and low values
    int altArr[] = {10, -10, 20, -20, 30, -30};
    sortIntegers(altArr, 6);
    assert(altArr[0] == -30 && altArr[5] == 30);

    // Test array with size 2
    int twoArr[] = {7, 3};
    sortIntegers(twoArr, 2);
    assert(twoArr[0] == 3 && twoArr[1] == 7);

    return 0;
}
