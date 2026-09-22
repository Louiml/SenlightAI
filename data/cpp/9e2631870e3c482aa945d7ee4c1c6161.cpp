// Write a C++ function named `moveZerosToEnd` that takes a reference to a `std::vector<int>` and modifies it in-place so that all zero elements are moved to the end of the vector while preserving the relative order of all non-zero elements. The function should not return anything, should work for vectors of any size (including empty and single-element vectors), and must not use any extra array or container for temporary storage. The input vector may contain negative numbers, positive numbers, and multiple zeros. After the function returns, all non-zero elements must appear in their original relative order at the front, followed by all the zeros (also in their original relative order, which is trivially maintained since zeros are identical).
// The optimal solution uses a two-pointer technique with a single forward scan. Maintain an index `j` that represents the position where the next non-zero element should be placed; initialize `j` to 0. Iterate through the vector with index `i` from 0 to `n-1`. When `arr[i]` is non-zero, swap `arr[i]` with `arr[j]` and increment `j`. This effectively "pushes" zeros to the right as non-zero elements are placed in their correct front positions. Because non-zero elements are processed in order and always placed at the next available front slot, their relative order is preserved. Edge cases handled: empty vector (loop does nothing), all zeros (no swaps occur, `j` stays 0), no zeros (every element swaps with itself, `j` increments to `n`), negative numbers (treated as non-zero just like positives). The commented alternative approach in the snippet (using two pointers `i` and `j` from ends) is incorrect because it can disturb relative order—so the forward-scan swap approach is the correct one. Time complexity is O(n) since each element is examined once and each swap is O(1). Space complexity is O(1) auxiliary, as only a few integer counters are used.
#include <vector>
#include <utility>  // for std::swap

/**
 * Moves all zeros in the vector to the end, preserving the relative
 * order of non-zero elements. Works in-place with O(1) extra space.
 */
void moveZerosToEnd(std::vector<int>& arr) {
    int j = 0;  // next position to place a non-zero element
    for (int i = 0; i < static_cast<int>(arr.size()); ++i) {
        if (arr[i] != 0) {
            std::swap(arr[i], arr[j]);
            ++j;
        }
    }
}
#include <vector>
#include <cassert>

int main() {
    // Empty vector
    std::vector<int> v1 = {};
    moveZerosToEnd(v1);
    assert(v1.empty());

    // No zeros
    std::vector<int> v2 = {1, -2, 3};
    moveZerosToEnd(v2);
    assert((v2 == std::vector<int>{1, -2, 3}));

    // All zeros
    std::vector<int> v3 = {0, 0, 0};
    moveZerosToEnd(v3);
    assert((v3 == std::vector<int>{0, 0, 0}));

    // Mixed with single zero
    std::vector<int> v4 = {1, 0, 2};
    moveZerosToEnd(v4);
    assert((v4 == std::vector<int>{1, 2, 0}));

    // Multiple zeros interleaved, negative numbers included
    std::vector<int> v5 = {0, 5, 0, -3, 0, 8};
    moveZerosToEnd(v5);
    assert((v5 == std::vector<int>{5, -3, 8, 0, 0, 0}));

    // Leading zeros
    std::vector<int> v6 = {0, 0, 7, 9};
    moveZerosToEnd(v6);
    assert((v6 == std::vector<int>{7, 9, 0, 0}));

    // Zeros at the end already
    std::vector<int> v7 = {4, 0, 0};
    moveZerosToEnd(v7);
    assert((v7 == std::vector<int>{4, 0, 0}));

    // Single non-zero
    std::vector<int> v8 = {42};
    moveZerosToEnd(v8);
    assert((v8 == std::vector<int>{42}));

    return 0;
}
