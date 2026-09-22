Write a C++ function `int findInsertionPoint(const double* sortedArray, int size, double key, int direction)` that performs a binary search on a strictly sorted array of `double` values to find the index where `key` would be inserted to maintain the sort order. The `direction` parameter is non-negative for ascending order and negative for descending order. For ascending order, the function must return the index of the first element greater than `key`; if all elements are ≤ `key`, return `size`. For descending order, the function must return the index of the first element less than `key`; if all elements are ≥ `key`, return `size` as well. Use an iterative binary search that operates on conceptual 1-based indices internally, and ensure correct handling of edge cases such as empty arrays, keys smaller than all elements, keys larger than all elements, and duplicate values.

The solution uses an iterative binary search adapted to find the insertion point. The key idea is to maintain a search interval `[iMin, iMax)` in conceptual 1-based indexing (where `iMin = 0` and `iMax = size` in 0-based terms), representing the range of candidate insertion positions. For ascending order, we compare `key` with the element at the midpoint (`mid = (iMin + iMax) / 2`). If `key < array[mid]`, we narrow the search to the left half (set `iMax = mid`); otherwise, we move to the right half (set `iMin = mid + 1`). This yields the first index where `array[index] > key`. For descending order, the comparison is reversed (`key > array[mid]`), yielding the first index where `array[index] < key`. Edge cases are naturally handled: empty arrays return 0; keys smaller than all elements return 0; keys larger than all elements return `size`. Duplicate values work because we only stop when the strict inequality holds, and otherwise move right. The time complexity is O(log n) and space complexity is O(1). Must be careful with integer overflow when computing mid: use `iMin + (iMax - iMin) / 2`. Also note that the function should not modify the input array, so it takes a `const double*`.

#include <cstddef>

// Returns the insertion index for 'key' in a sorted array of 'size' doubles.
// direction >= 0: ascending order; direction < 0: descending order.
// For ascending, returns first index i such that array[i] > key, else size.
// For descending, returns first index i such that array[i] < key, else size.
int findInsertionPoint(const double* array, int size, double key, int direction) {
    int low = 0;
    int high = size;  // exclusive upper bound

    if (direction >= 0) {
        // Ascending: find first index where array[index] > key
        while (low < high) {
            int mid = low + (high - low) / 2;
            if (key < array[mid]) {
                high = mid;
            } else {
                low = mid + 1;
            }
        }
    } else {
        // Descending: find first index where array[index] < key
        while (low < high) {
            int mid = low + (high - low) / 2;
            if (key > array[mid]) {
                high = mid;
            } else {
                low = mid + 1;
            }
        }
    }
    return low;
}

#include <cassert>

int main() {
    // Ascending order tests
    double asc[] = {1.0, 3.0, 5.0, 7.0, 9.0};
    assert(findInsertionPoint(asc, 5, 0.0, 1) == 0);      // smaller than all
    assert(findInsertionPoint(asc, 5, 1.0, 1) == 1);      // exactly first element
    assert(findInsertionPoint(asc, 5, 2.0, 1) == 1);      // between 1 and 3
    assert(findInsertionPoint(asc, 5, 4.0, 1) == 2);      // between 3 and 5
    assert(findInsertionPoint(asc, 5, 9.0, 1) == 5);      // exactly last element
    assert(findInsertionPoint(asc, 5, 10.0, 1) == 5);     // larger than all
    assert(findInsertionPoint(asc, 5, 7.0, 1) == 4);      // duplicate value

    // Descending order tests
    double desc[] = {9.0, 7.0, 5.0, 3.0, 1.0};
    assert(findInsertionPoint(desc, 5, 0.0, -1) == 5);    // smaller than all
    assert(findInsertionPoint(desc, 5, 10.0, -1) == 0);   // larger than all
    assert(findInsertionPoint(desc, 5, 8.0, -1) == 1);    // between 9 and 7
    assert(findInsertionPoint(desc, 5, 4.0, -1) == 3);    // between 5 and 3
    assert(findInsertionPoint(desc, 5, 1.0, -1) == 5);    // exactly last element
    assert(findInsertionPoint(desc, 5, 3.0, -1) == 4);    // duplicate value

    // Edge cases
    double empty[] = {};
    assert(findInsertionPoint(empty, 0, 5.0, 1) == 0);
    assert(findInsertionPoint(empty, 0, 5.0, -1) == 0);

    double single[] = {42.0};
    assert(findInsertionPoint(single, 1, 41.0, 1) == 0);
    assert(findInsertionPoint(single, 1, 42.0, 1) == 1);
    assert(findInsertionPoint(single, 1, 43.0, 1) == 1);
    assert(findInsertionPoint(single, 1, 41.0, -1) == 1);
    assert(findInsertionPoint(single, 1, 42.0, -1) == 1);
    assert(findInsertionPoint(single, 1, 43.0, -1) == 0);

    return 0;
}
