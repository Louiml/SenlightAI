// Write a C++ function that takes an integer array and its size as parameters, and returns the number of distinct values in the array. The array is not guaranteed to be sorted, and may contain negative numbers, duplicates, and any valid integer range. The function must use only constant extra space (do not use sorting, hash sets, or any dynamic data structures) and must not modify the original array. The function signature should be `int countDistinct(const int* arr, int size)`. Assume the input size is non-negative and that the array is valid (non-null if size > 0).

// The challenge is to count distinct values without extra space or sorting. Since we cannot use extra storage, we can use a brute-force comparison approach: for each element at index `i`, check if the same value appears anywhere before index `i`. If it does not appear earlier, then this is the first occurrence of that value, so increment the count. This ensures each distinct value is counted exactly once. Edge cases include an empty array (return 0), an array where all elements are the same (return 1), and negative numbers (which are handled naturally by equality comparison). Time complexity is O(n²) in the worst case because for each element we scan backward through all previous elements. Space complexity is O(1) since we use only a few integer variables. The algorithm does not modify the original array and works for any integer values.

#include <cstddef> // for std::size_t

// Count the number of distinct values in an integer array.
// Uses constant extra space and does not modify the input.
// Time: O(n^2), Space: O(1)
int countDistinct(const int* arr, int size) {
    int distinct = 0;
    for (int i = 0; i < size; ++i) {
        bool seenBefore = false;
        for (int j = 0; j < i; ++j) {
            if (arr[j] == arr[i]) {
                seenBefore = true;
                break;
            }
        }
        if (!seenBefore) {
            ++distinct;
        }
    }
    return distinct;
}

#include <cassert>

int main() {
    // Empty array
    int empty[] = {};
    assert(countDistinct(empty, 0) == 0);

    // Single element
    int single[] = {42};
    assert(countDistinct(single, 1) == 1);

    // All same values
    int same[] = {7, 7, 7, 7};
    assert(countDistinct(same, 4) == 1);

    // All distinct
    int distinct[] = {1, 2, 3, 4, 5};
    assert(countDistinct(distinct, 5) == 5);

    // Mixed with negatives and duplicates
    int mixed[] = {-3, 5, -3, 5, 0, 0, 2};
    assert(countDistinct(mixed, 7) == 4); // {-3, 5, 0, 2}

    // Unsorted with duplicates at ends
    int unsorted[] = {9, 1, 9, 2, 1, 3};
    assert(countDistinct(unsorted, 6) == 4); // {9, 1, 2, 3}

    // Large repetition pattern
    int repeated[] = {0, 1, 0, 1, 0, 1};
    assert(countDistinct(repeated, 6) == 2);

    // All zeroes (including negative zero is same)
    int zeros[] = {0, -0, 0};
    assert(countDistinct(zeros, 3) == 1);
}
