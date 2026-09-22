// Write a C++ function `mostFrequentValue` that takes a non-empty array of integers (via a pointer) and its size, and returns the most frequently occurring value. If multiple values tie for the highest frequency, return the smallest such value. The function must not modify the input array, must be `const`-correct where appropriate, and must handle arrays of any size (not just fixed 10 elements). Assume the input array is valid and size is positive.
// The main algorithm is a brute-force frequency count: for each element in the array, count how many times it appears by scanning the entire array. Track the maximum frequency seen so far and the value associated with it. To handle ties correctly (choose the smallest value), when a new count equals the current maximum frequency, update the result only if the current candidate value is smaller than the existing result. This ensures that among all values with the same maximum frequency, the smallest value is selected. Edge cases: a single-element array (the element is the answer), all elements identical (trivial), and multiple values with identical maximum frequency (the smallest must be returned). The algorithm runs in O(n²) time due to the nested loops, and uses O(1) auxiliary space (only a few integer variables). No extra data structures are needed. The function should take `const int*` for the array to guarantee no modification, and return an `int`.
#include <cstddef>  // for size_t

// Returns the most frequent integer in the array.
// If ties exist, returns the smallest among the most frequent values.
// The input array is treated as read-only.
int mostFrequentValue(const int* arr, size_t size) {
    int bestValue = arr[0];
    int bestCount = 0;

    for (size_t i = 0; i < size; ++i) {
        int currentValue = arr[i];
        int currentCount = 0;

        // Count occurrences of arr[i]
        for (size_t j = 0; j < size; ++j) {
            if (arr[j] == currentValue) {
                ++currentCount;
            }
        }

        // Update best if current count is higher, or if equal but value is smaller
        if (currentCount > bestCount ||
            (currentCount == bestCount && currentValue < bestValue)) {
            bestCount = currentCount;
            bestValue = currentValue;
        }
    }

    return bestValue;
}
int main() {
    // Basic case: one clear mode
    int a1[] = {1, 2, 3, 2, 2, 4};
    assert(mostFrequentValue(a1, 6) == 2);

    // All elements unique: each appears once, smallest value is returned
    int a2[] = {5, 1, 3, 2, 4};
    assert(mostFrequentValue(a2, 5) == 1);

    // Tie between multiple values: choose the smallest
    int a3[] = {3, 1, 2, 3, 2};
    assert(mostFrequentValue(a3, 5) == 2);  // both 2 and 3 appear twice, 2 is smaller

    // Single element array
    int a4[] = {7};
    assert(mostFrequentValue(a4, 1) == 7);

    // Negative numbers and ties
    int a5[] = {-2, -1, -2, -1, -3};
    assert(mostFrequentValue(a5, 5) == -2);  // both -1 and -2 appear twice, -2 is smaller

    // Large frequency, with tie on smaller value later
    int a6[] = {4, 4, 4, 1, 1, 1, 2};
    assert(mostFrequentValue(a6, 7) == 1);  // 4 and 1 both appear 3 times, 1 is smaller

    // All identical
    int a7[] = {9, 9, 9, 9};
    assert(mostFrequentValue(a7, 4) == 9);

    // Zero and negative mix
    int a8[] = {0, -1, 0, -1, 0};
    assert(mostFrequentValue(a8, 5) == 0);  // 0 appears 3 times, -1 twice

    return 0;
}
