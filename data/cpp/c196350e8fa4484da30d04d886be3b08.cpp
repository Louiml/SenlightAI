// Write a C++ function named `insertionSortDescending` that takes a non-const array of integers and its length as parameters, and sorts the array in **descending** order using the insertion sort algorithm (the largest element first). The function must modify the array in place, return `void`, and handle arrays of any length (including length 0 and 1). The algorithm must shift larger elements toward the beginning, so each element from index 1 onward is compared with the elements before it and inserted into the correct descending position. Do not use any standard library sorting functions.

// The insertion sort algorithm works by building a sorted portion of the array incrementally. For a descending sort, we iterate from index `1` to `length-1`. For each element at index `i`, we store it in a `keyValue` variable, then move a pointer `j` starting at `i-1` backward. While `j >= 0` and `array[j] < keyValue` (since we want descending order, the element before must be greater), we shift `array[j]` one position to the right (into `array[j+1]`) and decrement `j`. After the while loop, we place `keyValue` at index `j+1`. This ensures that all elements before `j+1` are greater than `keyValue` and all after are smaller. Edge cases: if length is 0 or 1, the loop never executes and the function does nothing. Duplicate elements are handled naturally—if an element is equal to `keyValue`, the while condition uses strict `<`, so it stops and the equal element stays to the left, preserving stable order (not required but fine). Time complexity is `O(n^2)` in the worst case (reverse-sorted input) and `O(n)` in the best case (already sorted descending), with `O(1)` auxiliary space.

#include <cstddef>

// Sorts an array of integers in descending order using insertion sort.
void insertionSortDescending(int array[], std::size_t length) {
    if (length < 2) {
        return;
    }

    for (std::size_t i = 1; i < length; ++i) {
        int keyValue = array[i];
        std::size_t j = i;

        // Shift smaller elements to the right to make room for keyValue.
        while (j > 0 && array[j - 1] < keyValue) {
            array[j] = array[j - 1];
            --j;
        }

        array[j] = keyValue;
    }
}

#include <cassert>
#include <cstddef>

void insertionSortDescending(int array[], std::size_t length);

int main() {
    // Test empty array (length 0)
    int empty[] = {};
    insertionSortDescending(empty, 0);

    // Test single element
    int single[] = {42};
    insertionSortDescending(single, 1);
    assert(single[0] == 42);

    // Test already sorted descending
    int sortedDesc[] = {10, 8, 5, 3, 1};
    insertionSortDescending(sortedDesc, 5);
    assert(sortedDesc[0] == 10 && sortedDesc[1] == 8 && sortedDesc[2] == 5 && sortedDesc[3] == 3 && sortedDesc[4] == 1);

    // Test reverse order (ascending input -> descending output)
    int reverseAsc[] = {1, 3, 5, 8, 10};
    insertionSortDescending(reverseAsc, 5);
    assert(reverseAsc[0] == 10 && reverseAsc[1] == 8 && reverseAsc[2] == 5 && reverseAsc[3] == 3 && reverseAsc[4] == 1);

    // Test duplicates
    int dup[] = {4, 2, 4, 2, 4};
    insertionSortDescending(dup, 5);
    assert(dup[0] == 4 && dup[1] == 4 && dup[2] == 4 && dup[3] == 2 && dup[4] == 2);

    // Test negative numbers
    int neg[] = {-3, -1, -10, -5};
    insertionSortDescending(neg, 4);
    assert(neg[0] == -1 && neg[1] == -3 && neg[2] == -5 && neg[3] == -10);

    // Test mixed large array
    int mixed[] = {5, -2, 8, 0, -9, 3, 7, 1};
    insertionSortDescending(mixed, 8);
    assert(mixed[0] == 8 && mixed[1] == 7 && mixed[2] == 5 && mixed[3] == 3 && mixed[4] == 1 && mixed[5] == 0 && mixed[6] == -2 && mixed[7] == -9);

    // Test same elements all identical
    int allSame[] = {7, 7, 7, 7};
    insertionSortDescending(allSame, 4);
    assert(allSame[0] == 7 && allSame[1] == 7 && allSame[2] == 7 && allSame[3] == 7);

    return 0;
}
