// Write a C++ function `int findMaxAbsValue(const int* arr, size_t size)` that takes an array of integers and its size, and returns the integer from the array whose absolute value is the largest. If two elements have the same absolute value (e.g., `-6` and `6`), return the positive one. If the array is empty, return `0`. The function must not modify the input array. You do not need to handle reading input or printing output; only implement the function. Your solution will be tested with arrays of varying lengths, including negative numbers, zeros, and duplicates.
The algorithm iterates through the array once, tracking the element with the maximum absolute value seen so far. Initialize the result to the first element if the array is non-empty; otherwise return `0` early. For each subsequent element, compare `abs(candidate)` with `abs(currentBest)`. If the candidate’s absolute value is strictly greater, update the best. If it is equal, prefer the positive version: if the candidate is positive and the current best is negative (or vice versa), update to the positive value. Edge cases: empty array (return 0), all negative values (pick the one with smallest magnitude because that has the largest absolute value, but actually largest absolute value among negatives is the most negative number, e.g., `-10` has abs 10), and duplicates with equal magnitude but opposite signs (choose positive). Time complexity is O(n) with O(1) auxiliary space.
#include <cstddef>
#include <cstdlib>

// Return the element with the largest absolute value; prefer positive on ties.
int findMaxAbsValue(const int* arr, size_t size) {
    if (size == 0) {
        return 0;
    }

    int best = arr[0];
    for (size_t i = 1; i < size; ++i) {
        const int current = arr[i];
        const int absCurrent = std::abs(current);
        const int absBest = std::abs(best);

        if (absCurrent > absBest) {
            best = current;
        } else if (absCurrent == absBest && current > best) {
            best = current; // prefer positive on equal absolute values
        }
    }
    return best;
}
#include <cassert>

// Assume findMaxAbsValue is defined above (or included here).
int main() {
    // Single element
    int a1[] = {42};
    assert(findMaxAbsValue(a1, 1) == 42);

    // All negative – largest absolute value is -10
    int a2[] = {-3, -7, -10, -1};
    assert(findMaxAbsValue(a2, 4) == -10);

    // Mixed positive/negative with same magnitude – prefer positive
    int a3[] = {-5, 5, -2, 2};
    assert(findMaxAbsValue(a3, 4) == 5);

    // Typical case with distinct absolute values
    int a4[] = {1, -9, 4, -2, 8};
    assert(findMaxAbsValue(a4, 5) == -9);

    // Zeros and duplicates
    int a5[] = {0, 0, -1, 1, -3};
    assert(findMaxAbsValue(a5, 5) == -3);

    // Empty array returns 0
    assert(findMaxAbsValue(nullptr, 0) == 0);

    // All equal values
    int a7[] = {-4, -4, -4};
    assert(findMaxAbsValue(a7, 3) == -4);

    // Positive maximum absolute
    int a8[] = {-100, 99, 100};
    assert(findMaxAbsValue(a8, 3) == 100);

    // Single negative
    int a9[] = {-7};
    assert(findMaxAbsValue(a9, 1) == -7);

    return 0;
}
