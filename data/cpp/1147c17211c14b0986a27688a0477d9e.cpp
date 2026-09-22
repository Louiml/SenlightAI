Write a C++ function named `removeDuplicatesWithinTwo` that takes a non-empty `std::vector<int>& nums` (sorted in non-decreasing order) and removes duplicates in-place such that each element appears at most twice relative to its original value, keeping the relative order of the remaining elements unchanged. The function must return the new length of the array after removal. Do not allocate extra space for another vector; you must modify the input vector in-place using `O(1)` extra memory. The function should handle edge cases like empty vectors, vectors with all identical elements, vectors with no duplicates, and vectors of length 1 or 2.
The core idea is to use a two-pointer technique: one pointer `i` iterates through the original array, and another pointer `len` tracks the position where the next valid element should be placed. We maintain a counter `count` that tracks how many times the current value has been seen consecutively. For each element at index `i`, if the array is empty (only relevant for the first iteration) or the current element equals the last placed element (`nums[len-1]`), we increment `count`; otherwise, we reset `count` to 1. If `count <= 2`, we copy `nums[i]` to `nums[len]` and increment `len`. This ensures that each distinct value appears at most twice. Since the array is sorted, equal values are always adjacent, so the consecutive counting is accurate. Edge cases: an empty vector returns 0; a vector of length 1 or 2 already satisfies the condition, so we return its size; a vector with all identical values (e.g., `[1,1,1,1]`) becomes `[1,1]` and returns 2; a vector with no duplicates (e.g., `[1,2,3]`) returns 3 unchanged. The algorithm runs in `O(n)` time with `O(1)` auxiliary space, performing a single pass over the input.
#include <vector>

/**
 * Removes duplicates in-place from a sorted vector so each element appears at most twice.
 * Returns the new length of the vector after removal.
 * The first `return` bytes of the vector contain the valid result; the rest is unspecified.
 */
int removeDuplicatesWithinTwo(std::vector<int>& nums) {
    if (nums.empty()) {
        return 0;
    }

    int len = 0;          // length of the valid prefix
    int count = 0;        // consecutive occurrences of the current value

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        if (i == 0 || nums[i] != nums[len - 1]) {
            count = 1;
        } else {
            ++count;
        }

        if (count <= 2) {
            nums[len] = nums[i];
            ++len;
        }
    }

    return len;
}
#include <cassert>
#include <vector>

// Forward declaration of the function under test
int removeDuplicatesWithinTwo(std::vector<int>& nums);

int main() {
    // Empty vector
    std::vector<int> empty;
    assert(removeDuplicatesWithinTwo(empty) == 0);

    // Single element
    std::vector<int> single = {5};
    assert(removeDuplicatesWithinTwo(single) == 1);
    assert(single[0] == 5);

    // Two identical elements (already valid)
    std::vector<int> twoSame = {7, 7};
    assert(removeDuplicatesWithinTwo(twoSame) == 2);
    assert(twoSame[0] == 7 && twoSame[1] == 7);

    // Three identical elements -> should keep two
    std::vector<int> threeSame = {3, 3, 3};
    assert(removeDuplicatesWithinTwo(threeSame) == 2);
    assert(threeSame[0] == 3 && threeSame[1] == 3);

    // No duplicates
    std::vector<int> noDup = {1, 2, 3};
    assert(removeDuplicatesWithinTwo(noDup) == 3);
    assert(noDup[0] == 1 && noDup[1] == 2 && noDup[2] == 3);

    // Mixed with some duplicates exceeding two
    std::vector<int> mixed = {0, 0, 0, 1, 1, 2, 2, 2, 3};
    assert(removeDuplicatesWithinTwo(mixed) == 7);
    assert(mixed[0] == 0 && mixed[1] == 0 && mixed[2] == 1 && mixed[3] == 1 &&
           mixed[4] == 2 && mixed[5] == 2 && mixed[6] == 3);

    // All identical with many copies
    std::vector<int> allSame = {4, 4, 4, 4, 4};
    assert(removeDuplicatesWithinTwo(allSame) == 2);
    assert(allSame[0] == 4 && allSame[1] == 4);

    // Large vector with runs of length 3 and 4
    std::vector<int> large = {1, 1, 1, 2, 2, 3, 3, 3, 3, 4, 5, 5, 5};
    assert(removeDuplicatesWithinTwo(large) == 9);
    assert(large[0] == 1 && large[1] == 1 && large[2] == 2 && large[3] == 2 &&
           large[4] == 3 && large[5] == 3 && large[6] == 4 && large[7] == 5 &&
           large[8] == 5);

    return 0;
}
