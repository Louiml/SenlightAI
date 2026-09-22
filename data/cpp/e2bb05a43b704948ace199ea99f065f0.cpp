// Write a C++ function that removes duplicates from a sorted integer array in-place, allowing at most two occurrences of any value, and returns the new length of the array. The function must modify the input array so that the first `k` elements contain the valid result (where `k` is the returned length), and all other elements are ignored. You may assume the input array is sorted in non-decreasing order. The function should handle an empty array correctly, and for any distinct value that appears more than twice, only the first two occurrences should be kept. The relative order of the remaining elements must be preserved.

The algorithm uses a single pass with three variables: `currentValue` to track the value being examined, `writeIndex` to mark where the next valid element should be placed in the array, and `remainingCopies` to count how many more copies of the current value we are still allowed to emit. Initially, if the array is empty, return 0. Set `currentValue = A[0]`, `writeIndex = 0`, and `remainingCopies = 2` because each distinct value can appear at most twice. Iterate from index 1 to n-1. If `A[i]` equals `currentValue`, decrement `remainingCopies` and skip this element (we do not write it now because we might write it later when the run changes). If `A[i]` differs, then we have reached the end of the current run; flush the run: write `currentValue` once (always), and if `remainingCopies` is not 2 (meaning we saw more than one copy), write it a second time. Then set `currentValue = A[i]` and reset `remainingCopies = 2`. After the loop, flush the final run similarly. The number of written elements is `writeIndex`, which we return. Edge cases include an empty array, a single element, all elements identical, and arrays where each element appears exactly once or exactly twice. Time complexity is O(n) with O(1) auxiliary space.

#include <vector>

// Removes duplicates from a sorted array, keeping at most two copies of each value.
// Returns the new length of the valid prefix in the array.
// The input array is modified in-place. It is assumed to be sorted.
int removeDuplicatesAtMostTwo(int A[], int n) {
    if (n == 0) {
        return 0;
    }

    int currentValue = A[0];
    int writeIndex = 0;
    int remainingCopies = 2; // We can keep at most 2 copies.

    for (int i = 1; i < n; ++i) {
        if (A[i] == currentValue) {
            --remainingCopies; // One more copy of the same value seen; we skip it for now.
            continue;
        } else {
            // End of the current run. Flush it.
            A[writeIndex++] = currentValue;
            if (remainingCopies != 2) {
                A[writeIndex++] = currentValue; // We saw at least two copies, emit second one.
            }
            currentValue = A[i];
            remainingCopies = 2;
        }
    }

    // Flush the last run.
    A[writeIndex++] = currentValue;
    if (remainingCopies != 2) {
        A[writeIndex++] = currentValue;
    }

    return writeIndex;
}

#include <cassert>
#include <algorithm>

// Declaration of the function being tested.
int removeDuplicatesAtMostTwo(int A[], int n);

int main() {
    // Test 1: Empty array.
    int empty[] = {};
    assert(removeDuplicatesAtMostTwo(empty, 0) == 0);

    // Test 2: All distinct.
    int distinct[] = {1, 2, 3, 4};
    int len1 = removeDuplicatesAtMostTwo(distinct, 4);
    assert(len1 == 4);
    assert(distinct[0] == 1 && distinct[1] == 2 && distinct[2] == 3 && distinct[3] == 4);

    // Test 3: All identical.
    int allSame[] = {5, 5, 5, 5};
    int len2 = removeDuplicatesAtMostTwo(allSame, 4);
    assert(len2 == 2);
    assert(allSame[0] == 5 && allSame[1] == 5);

    // Test 4: Mixed with duplicates exceeding two.
    int mixed[] = {1, 1, 1, 2, 2, 3, 3, 3};
    int len3 = removeDuplicatesAtMostTwo(mixed, 8);
    assert(len3 == 6);
    assert(mixed[0] == 1 && mixed[1] == 1 && mixed[2] == 2 && mixed[3] == 2 &&
           mixed[4] == 3 && mixed[5] == 3);

    // Test 5: Single element.
    int single[] = {42};
    int len4 = removeDuplicatesAtMostTwo(single, 1);
    assert(len4 == 1);
    assert(single[0] == 42);

    // Test 6: Exactly two copies of one value.
    int twoCopies[] = {7, 7, 8, 8};
    int len5 = removeDuplicatesAtMostTwo(twoCopies, 4);
    assert(len5 == 4);
    assert(twoCopies[0] == 7 && twoCopies[1] == 7 && twoCopies[2] == 8 && twoCopies[3] == 8);

    return 0;
}
