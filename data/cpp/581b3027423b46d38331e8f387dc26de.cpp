/*
Write a C++ function named `findFirstIndex` that, given an array of integers, its size `n`, and a search key, returns the index of the **first occurrence** of the key in the array, or `-1` if the key is not present. The function must operate on a plain C-style array (not `std::vector`) and assume the array is valid with at least one element. The function should be `const`-correct, meaning it must not modify the array contents, and should handle duplicate values correctly by returning the smallest index where the key appears. For example, if the array is `{5, 2, 5, 8}` and the key is `5`, the function must return `0`, not `2`. The solution must be self-contained and not rely on any external libraries beyond the standard headers.
*/
#include <cstddef>

// Returns the index of the first occurrence of key in array of size n, or -1 if not found.
int findFirstIndex(const int array[], int n, int key) {
    for (int i = 0; i < n; ++i) {
        if (array[i] == key) {
            return i;
        }
    }
    return -1;
}
#include <cassert>

int findFirstIndex(const int array[], int n, int key); // Declared from solution

int main() {
    int a1[] = {5, 2, 5, 8};
    assert(findFirstIndex(a1, 4, 5) == 0);   // first occurrence at index 0
    assert(findFirstIndex(a1, 4, 2) == 1);
    assert(findFirstIndex(a1, 4, 8) == 3);
    assert(findFirstIndex(a1, 4, 7) == -1);  // not present

    int a2[] = {10};
    assert(findFirstIndex(a2, 1, 10) == 0);
    assert(findFirstIndex(a2, 1, 99) == -1);

    int a3[] = {-3, -3, -3};
    assert(findFirstIndex(a3, 3, -3) == 0);  // first of duplicates

    int a4[] = {1, 2, 3, 2, 1};
    assert(findFirstIndex(a4, 5, 1) == 0);
    assert(findFirstIndex(a4, 5, 2) == 1);
    assert(findFirstIndex(a4, 5, 3) == 2);

    return 0;
}
// The solution is straightforward linear search: iterate through the array from index `0` to `n-1`, comparing each element with the key. As soon as a match is found, return that index immediately, because iterating from the beginning guarantees the first occurrence. If the loop completes without finding the key, return `-1`. Edge cases include: (1) the key is at index `0`, which is handled because the loop starts at `0`; (2) the key appears multiple times, and we return the smallest index naturally; (3) the key is not present, returning `-1`; (4) the array size `n` is at least `1` per the task, but if `n` were `0`, the loop would not execute and correctly return `-1`. Time complexity is \(O(n)\) in the worst case (key absent or appears last), and \(O(1)\) in the best case (key at the first position). Space complexity is \(O(1)\) auxiliary, as only a few loop variables are used. The function is marked `const int*` parameter to enforce non-modification and accepts the size as an `int`.
