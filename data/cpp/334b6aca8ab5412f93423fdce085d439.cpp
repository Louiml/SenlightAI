// Write a C++ function that takes a fixed-size `std::array<int, 10>` by reference, along with a valid insertion index (between 0 and 9) and an integer value. The function should shift all elements from the given index to the end of the array one position to the right (the last element is overwritten), then place the new integer at the specified index. The function must modify the array in place and return nothing. Your solution must correctly handle insertion at index 0 and at index 9. The provided test code and main function are not part of your task—only the function is required. You may assume the index is always valid; no error checks are needed.

The algorithm is straightforward: to insert an element at a valid index `idx`, start from the last valid position of the array (index 9) and iterate backward. For each position `i` from 9 down to `idx + 1`, copy the value from position `i - 1` to position `i`. This shifts the block `[idx, 8]` one step to the right. After the loop finishes, assign the new element to `a[idx]`. The loop must go backward, otherwise if we iterate forward we would overwrite values before copying them. Edge cases: if `idx == 9`, the loop condition `i > 9` fails immediately, so no shifting occurs, and the new value is simply placed at the last position (effectively overwriting the old last element). If `idx == 0`, the entire array shifts right, and the old last element is lost. Time complexity: \(O(n)\) where \(n = 10\) is constant, but in general it is \(O(n)\). Space complexity: \(O(1)\) as only a single local variable (the loop index) is used, plus the function parameter reference.

#include <array>

// Insert `element` at position `index` in the fixed-size array `a`.
// Existing elements from `index` onward are shifted one position to the right.
// The last element of the array is overwritten. `index` must be in [0, 9].
void insertIntoArray(std::array<int, 10>& a, int index, int element) {
    // Shift elements from the end down to index+1 one step to the right.
    for (int i = static_cast<int>(a.size()) - 1; i > index; --i) {
        a[i] = a[i - 1];
    }
    // Place the new element at the desired index.
    a[index] = element;
}

#include <array>
#include <cassert>

// Declare the function under test (should match the solution exactly).
void insertIntoArray(std::array<int, 10>& a, int index, int element);

int main() {
    // Test 1: Insert at index 3 in a typical array.
    std::array<int, 10> a1 = {5, 2, 7, 4, 9, 1, 3, 6, 8, 0};
    insertIntoArray(a1, 3, 10);
    std::array<int, 10> expected1 = {5, 2, 7, 10, 4, 9, 1, 3, 6, 8};
    assert(a1 == expected1);

    // Test 2: Insert at index 0 (shift whole array right).
    std::array<int, 10> a2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    insertIntoArray(a2, 0, 99);
    std::array<int, 10> expected2 = {99, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    assert(a2 == expected2);

    // Test 3: Insert at index 9 (overwrites last element without shifting).
    std::array<int, 10> a3 = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    insertIntoArray(a3, 9, 42);
    std::array<int, 10> expected3 = {10, 20, 30, 40, 50, 60, 70, 80, 90, 42};
    assert(a3 == expected3);

    // Test 4: Insert at index 4, all duplicate values.
    std::array<int, 10> a4 = {7, 7, 7, 7, 7, 7, 7, 7, 7, 7};
    insertIntoArray(a4, 4, 1);
    std::array<int, 10> expected4 = {7, 7, 7, 7, 1, 7, 7, 7, 7, 7};
    assert(a4 == expected4);

    // Test 5: Insert at index 1, array with negative and zero values.
    std::array<int, 10> a5 = {-3, -1, 0, 2, 5, 8, -7, 4, 9, 1};
    insertIntoArray(a5, 1, -10);
    std::array<int, 10> expected5 = {-3, -10, -1, 0, 2, 5, 8, -7, 4, 9};
    assert(a5 == expected5);

    return 0;
}
