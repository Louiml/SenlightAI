Write a C++ function named `computeStatsWithPointerArithmetic` that takes a non-empty array of integers, its size (as a `size_t`), and returns a `struct` containing three members: the sum of all elements, the minimum element, and the maximum element. The function must use **only pointer arithmetic** (no array subscript notation `arr[i]`) to traverse the array. You must not modify the original array. The traversal can be done forward or backward, but must use pointer increments/decrements or pointer addition/subtraction. The function should handle arrays of any positive size, including single-element arrays where all three statistics are the same value. Return the result by value.

#include <cassert>

// The solution function is declared above.

int main() {
    // Test 1: normal array
    int a1[] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    Stats s1 = computeStatsWithPointerArithmetic(a1, 10);
    assert(s1.sum == 155);
    assert(s1.min == 11);
    assert(s1.max == 20);

    // Test 2: single element
    int a2[] = {42};
    Stats s2 = computeStatsWithPointerArithmetic(a2, 1);
    assert(s2.sum == 42);
    assert(s2.min == 42);
    assert(s2.max == 42);

    // Test 3: negative numbers
    int a3[] = {-5, -1, -10, 0, -3};
    Stats s3 = computeStatsWithPointerArithmetic(a3, 5);
    assert(s3.sum == -19);
    assert(s3.min == -10);
    assert(s3.max == 0);

    // Test 4: duplicates
    int a4[] = {7, 7, 7, 7};
    Stats s4 = computeStatsWithPointerArithmetic(a4, 4);
    assert(s4.sum == 28);
    assert(s4.min == 7);
    assert(s4.max == 7);

    // Test 5: mixed positive and negative, zero
    int a5[] = {0, 100, -100, 50, -50, 25};
    Stats s5 = computeStatsWithPointerArithmetic(a5, 6);
    assert(s5.sum == 25);
    assert(s5.min == -100);
    assert(s5.max == 100);

    return 0;
}

#include <cstddef> // for size_t

// A small struct to hold statistics computed from an integer array.
struct Stats {
    int sum;
    int min;
    int max;
};

/**
 * Computes the sum, minimum, and maximum of an integer array.
 * The array is traversed using pointer arithmetic only.
 *
 * @param arr   Pointer to the first element of the array (non-const qualified in the caller).
 * @param size  Number of elements in the array (must be > 0).
 * @return      A Stats struct containing sum, min, and max.
 */
Stats computeStatsWithPointerArithmetic(const int* arr, size_t size) {
    // Initialize using the first element.
    const int* ptr = arr;
    int sum = *ptr;
    int min = *ptr;
    int max = *ptr;

    // Advance to the next element and iterate.
    ++ptr;
    for (size_t i = 1; i < size; ++i) {
        int value = *ptr;
        sum += value;
        if (value < min) min = value;
        if (value > max) max = value;
        ++ptr; // move to next element
    }

    return Stats{sum, min, max};
}

// The solution uses a single forward pass over the array using a pointer that starts at the first element. Initialize the sum to `0`, and both minimum and maximum to the first element's value. Then iterate through the remaining elements by incrementing the pointer and dereferencing it. For each value, add it to the sum, and update minimum/maximum using comparisons. Because the function must not modify the array, we declare the array parameter as `const int*` (or `const int arr[]`). The time complexity is \(O(n)\) for \(n\) elements, and the auxiliary space is \(O(1)\) since only a few scalar variables are used. No edge cases besides a single-element array (where min, max, and sum are all that value). Negative numbers and duplicates are handled naturally.
