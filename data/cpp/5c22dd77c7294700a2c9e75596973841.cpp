// Write a C++ function named `findFirstOccurrence` that takes a target integer, an array of integers, and the array’s size (as a `size_t`), and returns the index of the first occurrence of the target in the array. If the target is not present, the function must return `-1`. The function must use linear search and must not modify the input array. Also write a separate test harness that calls this function with at least five different cases, including: target found at the beginning, target found in the middle, target found at the end, target not found, and an empty array (size `0`), verifying each result with `assert`.
The main algorithm is a simple linear scan: iterate through the array from index `0` to `size - 1`, comparing each element to the target. The moment a match is found, return that index immediately (since we need the first occurrence). If the loop completes without a match, return `-1`. Edge cases include an empty array (the loop never runs, so return `-1`), duplicate values (stop at the first match), and negative indices (not possible because we return `-1` only for “not found”, and array indices are non-negative). Time complexity is \(O(n)\) in the worst case (target absent or at the last position) and \(O(1)\) in the best case (target at the first position). Space complexity is \(O(1)\) because we only use a loop counter and a return value. The function should take the array as a `const` pointer to emphasize that it does not modify the input, and the size should be `size_t` to match standard array conventions.
#include <cstddef>  // for size_t

// Returns the index of the first occurrence of target in arr,
// or -1 if target is not found. Uses linear search.
int findFirstOccurrence(int target, const int arr[], size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (arr[i] == target) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
#include <cassert>

int main() {
    int arr1[] = {5, 3, 8, 3, 9};
    assert(findFirstOccurrence(5, arr1, 5) == 0);   // found at beginning
    assert(findFirstOccurrence(8, arr1, 5) == 2);   // found in middle
    assert(findFirstOccurrence(9, arr1, 5) == 4);   // found at end
    assert(findFirstOccurrence(7, arr1, 5) == -1);  // not found

    int arr2[] = {2, 2, 2};
    assert(findFirstOccurrence(2, arr2, 3) == 0);   // duplicates: first index

    int arr3[] = {};  // empty array, size 0
    assert(findFirstOccurrence(1, arr3, 0) == -1);  // empty array case

    return 0;
}
