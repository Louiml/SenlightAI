/*
Write a C++ function that inserts a given `key` into a sorted integer array `arr` of current length `n` and capacity `capacity`. The array is already sorted in non-decreasing order, with valid elements stored in the first `n` positions. If the array already has `n == capacity` elements, the function should return `n` without modifying the array. Otherwise, insert `key` into its correct sorted position by shifting larger elements to the right, update the array in place, and return the new length `n + 1`. Assume `0 <= n <= capacity`, and that the array has enough physical storage for at least `capacity` elements. Do not use any standard library sorting functions; perform the insertion explicitly.
*/

#include <vector>

// Inserts key into a sorted array (non-decreasing order) of length n,
// given a total capacity. Returns the new length after insertion, or n if full.
int insertIntoSorted(int arr[], int n, int key, int capacity) {
    if (n >= capacity) {
        return n;
    }
    int i = n - 1;
    while (i >= 0 && arr[i] > key) {
        arr[i + 1] = arr[i];
        --i;
    }
    arr[i + 1] = key;
    return n + 1;
}

#include <cassert>

int main() {
    // Case 1: insert into middle
    int a1[10] = {1, 3, 5, 7};
    int n1 = insertIntoSorted(a1, 4, 4, 10);
    assert(n1 == 5);
    for (int i = 0; i < n1; ++i) assert(a1[i] == i * 1 + 1 + (i > 2 ? 1 : 0)); // {1,3,4,5,7}

    // Case 2: insert smaller than all elements
    int a2[10] = {10, 20, 30};
    int n2 = insertIntoSorted(a2, 3, 5, 10);
    assert(n2 == 4);
    assert(a2[0] == 5 && a2[1] == 10 && a2[2] == 20 && a2[3] == 30);

    // Case 3: insert larger than all elements
    int a3[10] = {2, 4, 6};
    int n3 = insertIntoSorted(a3, 3, 8, 10);
    assert(n3 == 4);
    assert(a3[0] == 2 && a3[1] == 4 && a3[2] == 6 && a3[3] == 8);

    // Case 4: array full, no insertion
    int a4[3] = {1, 2, 3};
    int n4 = insertIntoSorted(a4, 3, 0, 3);
    assert(n4 == 3 && a4[0] == 1 && a4[1] == 2 && a4[2] == 3);

    // Case 5: empty array (n = 0)
    int a5[5] = {};
    int n5 = insertIntoSorted(a5, 0, 42, 5);
    assert(n5 == 1 && a5[0] == 42);

    // Case 6: duplicates, key equals an existing element
    int a6[5] = {2, 2, 2, 2};
    int n6 = insertIntoSorted(a6, 4, 2, 5);
    assert(n6 == 5);
    assert(a6[0] == 2 && a6[1] == 2 && a6[2] == 2 && a6[3] == 2 && a6[4] == 2);
}

// The insertion is performed by scanning from the end of the currently valid portion of the array (index `n-1`) toward the beginning. For each element `arr[i]` that is greater than `key`, shift it one position to the right (`arr[i+1] = arr[i]`). The loop stops when either we reach the front (`i < 0`) or find an element `<= key`. At that point, the correct insertion index is `i+1`, and we place `key` there. This works correctly because the array is sorted and shifting preserves the sorted order of the existing elements. The edge cases include: (1) `n == capacity` — the function returns immediately without modification; (2) `key` is smaller than all existing elements, so the loop shifts all elements and inserts at index 0; (3) `key` is larger than all elements, so the loop body never executes and the key is appended at index `n`. The algorithm runs in O(n) worst-case time (when all elements are greater than `key`), and O(1) auxiliary space since the shift is done in-place. No memory allocation or additional containers are used.
