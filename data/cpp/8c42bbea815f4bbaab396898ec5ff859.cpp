// Write a C++ function that takes a non-empty array of `double` values and returns the smallest positive (strictly greater than zero) value in the array. If no positive value exists, the function should return `-1.0`. The function must be `const`-correct (i.e., accept a pointer to const data) and must not modify the input array. The input array size is guaranteed to be at least 1. Handle edge cases such as all negative numbers, zeros, duplicates, and large magnitudes. Do not use any standard library algorithms—implement the logic manually with a loop.

// The solution scans the array linearly, tracking the smallest positive value seen so far. Initialize a candidate variable to `-1.0` (meaning "not found yet"). Iterate over every element: if the current element is greater than 0, then either it’s the first positive found (so set the candidate to it) or it’s smaller than the current candidate (update). This way, duplicates are naturally handled and zeros are ignored (since we use `> 0`, not `>= 0`). If no positive is found, the candidate remains `-1.0`. The main edge case is an all-negative or all-zero array. Another subtle case is when the first positive appears late—each element is checked exactly once. Time complexity is O(n) for n elements; space complexity is O(1) auxiliary, since only a few scalar variables are used.

#include <cstddef> // for size_t

// Returns the smallest positive (>0) value in arr, or -1.0 if none exists.
// The array must be non-empty. Const-correct: does not modify input.
double smallestPositive(const double* arr, std::size_t size) {
    double result = -1.0;
    for (std::size_t i = 0; i < size; ++i) {
        if (arr[i] > 0.0) {
            if (result < 0.0 || arr[i] < result) {
                result = arr[i];
            }
        }
    }
    return result;
}

#include <cassert>
#include <cstddef>

// Declaration of the tested function (assumed to be defined above)
double smallestPositive(const double* arr, std::size_t size);

int main() {
    // Mixed positive and negative
    double a1[] = {3.0, -1.0, 0.5, -2.0, 4.0};
    assert(smallestPositive(a1, 5) == 0.5);

    // All negative
    double a2[] = {-1.0, -2.0, -3.0};
    assert(smallestPositive(a2, 3) == -1.0);

    // All zeros (no positive)
    double a3[] = {0.0, 0.0, 0.0};
    assert(smallestPositive(a3, 3) == -1.0);

    // Single positive value
    double a4[] = {7.5};
    assert(smallestPositive(a4, 1) == 7.5);

    // Single negative value
    double a5[] = {-2.2};
    assert(smallestPositive(a5, 1) == -1.0);

    // Duplicate positives, smallest appears multiple times
    double a6[] = {2.0, 2.0, 1.0, 1.0, 3.0};
    assert(smallestPositive(a6, 5) == 1.0);

    // Zeros and positives mixed
    double a7[] = {0.0, -5.0, 0.0, 0.1, -0.2, 0.2};
    assert(smallestPositive(a7, 6) == 0.1);

    // Large magnitude values
    double a8[] = {1e308, -1e308, 1e-308, 0.0};
    assert(smallestPositive(a8, 4) == 1e-308);

    // All equal positive values
    double a9[] = {4.0, 4.0, 4.0};
    assert(smallestPositive(a9, 3) == 4.0);

    return 0;
}
