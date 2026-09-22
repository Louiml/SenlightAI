Write a C++ function named `sumBatch` that takes an array of integers and its size as parameters, and returns the sum of all elements in the array as a `long long` (to safely handle large sums). The function must be `const`-correct: it should accept the array as a pointer to `const int` (or `const std::vector<int>&` if you prefer, but the task specifies a raw array), and it should not modify the input. The function should handle edge cases such as an empty array (returning 0), negative numbers, and very large positive/negative values that could overflow a 32-bit `int`. You do not need to read input or print output; the function will be called directly by test code.
The solution is straightforward: iterate through the array from index 0 to `n-1`, accumulating each element into a `long long` accumulator. Initialize the accumulator to 0 so that an empty array (size 0) correctly returns 0. Use `long long` instead of `int` to avoid overflow when summing many large values (e.g., 100,000 elements each near 2^31-1 would overflow 32-bit int but fit in 64-bit long long). The parameter should be declared as `const int* nums` (or `const int nums[]`) to emphasize that the function does not modify the input. Time complexity is O(n) — one pass over the array. Space complexity is O(1) — only a few local variables. The main edge cases are: empty array (returns 0), single element (returns that element), all negative numbers (sum will be negative), and large magnitude sums that require 64-bit storage.
#include <cstddef> // for size_t (optional, but good practice)

// Returns the sum of all integers in the array 'nums' of length 'n'.
// Uses long long to avoid overflow when summing many or large integers.
// The array is treated as read-only (const) and never modified.
long long sumBatch(const int* nums, size_t n) {
    long long total = 0;
    for (size_t i = 0; i < n; ++i) {
        total += nums[i];
    }
    return total;
}
#include <cassert>

int main() {
    // Empty array
    int empty[] = {};
    assert(sumBatch(empty, 0) == 0);

    // Single positive element
    int single[] = {42};
    assert(sumBatch(single, 1) == 42);

    // Single negative element
    int singleNeg[] = {-7};
    assert(sumBatch(singleNeg, 1) == -7);

    // All positive numbers
    int pos[] = {1, 2, 3, 4, 5};
    assert(sumBatch(pos, 5) == 15);

    // Mixed positive and negative
    int mixed[] = {10, -3, 5, -8, 2};
    assert(sumBatch(mixed, 5) == 6);

    // All negative numbers
    int neg[] = {-1, -2, -3, -4};
    assert(sumBatch(neg, 4) == -10);

    // Duplicate large values (sum would overflow int but fits in long long)
    int large[10];
    for (int i = 0; i < 10; ++i) large[i] = 2000000000; // 2 billion each
    assert(sumBatch(large, 10) == 20000000000LL); // 20 billion

    // Zero and negatives
    int zeroMix[] = {0, 0, -5, 0, 5};
    assert(sumBatch(zeroMix, 5) == 0);

    // Size 1 with zero
    int zeroSingle[] = {0};
    assert(sumBatch(zeroSingle, 1) == 0);

    // Larger array with alternating pattern
    int alt[100];
    for (int i = 0; i < 100; ++i) alt[i] = (i % 2 == 0) ? 1 : -1;
    assert(sumBatch(alt, 100) == 0);

    return 0;
}
