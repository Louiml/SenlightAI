Write a C++ function that performs a single complete pass of bubble sort on an integer array (comparing and swapping adjacent elements from left to right up to the last element) and returns the number of swaps performed during that pass, without modifying the original array. The function should accept a constant reference-to-array or a raw pointer with a length parameter, and it must work for arrays of any size, including empty arrays (size 0) and arrays of size 1. For an empty or single-element array, the function must return 0. The function should not print anything; it should only count and return the swaps. Use `const` correctly to ensure the input array is not modified.
#include <cassert>

int main() {
    int empty[] = {};
    int single[] = {42};
    int sorted[] = {1, 2, 3, 4, 5};
    int reverse[] = {5, 4, 3, 2, 1};
    int duplicates[] = {3, 3, 3};
    int mix[] = {2, 5, 1, 4, 3};

    assert(countSwapsInOnePass(empty, 0) == 0);
    assert(countSwapsInOnePass(single, 1) == 0);
    assert(countSwapsInOnePass(sorted, 5) == 0);
    assert(countSwapsInOnePass(reverse, 5) == 4); // (5,4), (4,3), (3,2), (2,1)
    assert(countSwapsInOnePass(duplicates, 3) == 0);
    assert(countSwapsInOnePass(mix, 5) == 3); // (5,1), (5,4), (4,3)

    // Verify the original arrays remain unchanged
    assert(reverse[0] == 5 && reverse[1] == 4 && reverse[2] == 3 && reverse[3] == 2 && reverse[4] == 1);
    assert(mix[0] == 2 && mix[1] == 5 && mix[2] == 1 && mix[3] == 4 && mix[4] == 3);

    return 0;
}
#include <cstddef> // for size_t

// Perform one complete pass of bubble sort (comparisons only) on a constant array.
// Returns the number of swaps that would have been performed.
// The array is not modified.
int countSwapsInOnePass(const int A[], size_t n) {
    int swaps = 0;
    for (size_t i = 0; i + 1 < n; ++i) {
        if (A[i] > A[i + 1]) {
            ++swaps;
        }
    }
    return swaps;
}
// The core algorithm is a straightforward single iteration of bubble sort: iterate from index 0 to n-2 (inclusive), and for each pair (i, i+1), if the current element is greater than the next, increment a swap counter. Since the task requires not modifying the original array, we only compare values and do not call `swap`; we just check the condition `A[i] > A[i+1]` and increment the counter. Edge cases: when `n` is 0 or 1, the loop body never executes, so the function returns 0 immediately. The time complexity is O(n) because we make exactly `n-1` comparisons (or fewer if we break early, but we don't need to break). Space complexity is O(1) as we only use a counter variable. The function must be `const`-correct: pass the array as `const int*` or `const int A[]` to prevent modification.
