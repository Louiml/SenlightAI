// Write a C++ function named `transposeSquareInPlace` that takes a 1D array of `int` values representing a square matrix stored in row-major order, along with the matrix's width (number of rows/columns), and transposes the matrix in place. The function should swap the element at `(x, y)` with the element at `(y, x)` for all `x < y`, following the exact logic of the provided `SquareTranspose` template. The matrix contains only `int` values, and you may assume the width is positive. The function must modify the input array directly and return `void`. Handle the case where width is 1 (no swaps needed) and width is 2 or larger. The time complexity should be O(width²), and space complexity O(1) — no auxiliary arrays or temporary storage beyond a few scalars.

// The core operation is a classic in-place matrix transpose for square matrices. Given a row-major layout where index `idx = x + width * y` maps to row `y` and column `x`, the transpose swaps `data[x + width*y]` with `data[y + width*x]` for every pair `(x, y)` with `x < y`. This guarantees each off-diagonal pair is swapped exactly once, leaving the diagonal elements (where `x == y`) unchanged. The approach uses two nested loops: the outer loop iterates `x` from `0` to `width-1`, and the inner loop iterates `y` from `x+1` to `width-1`. For each such pair, compute the two 1D indices and swap the integer values using `std::swap`. Edge cases: width = 0 or width = 1 — the inner loop condition `y = x+1` never executes, so no swaps occur. The algorithm processes exactly `width*(width-1)/2` swaps, taking O(width²) time. The space complexity is O(1) because only loop counters and a temporary for swapping are used. No special handling for negative values or duplicates is needed; the swap is purely positional.

#include <algorithm>  // for std::swap

// Transpose a square matrix stored in row-major order in a 1D array.
// width is the number of rows and columns.
void transposeSquareInPlace(int* data, int width) {
    for (int x = 0; x < width; ++x) {
        for (int y = x + 1; y < width; ++y) {
            const int idx0 = x + width * y;
            const int idx1 = y + width * x;
            std::swap(data[idx0], data[idx1]);
        }
    }
}

#include <cassert>

// Helper to compare two arrays element-by-element
bool arraysEqual(const int* a, const int* b, int size) {
    for (int i = 0; i < size; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

int main() {
    // Test 1: 2x2 matrix
    int m1[4] = {1, 2, 3, 4};
    int expected1[4] = {1, 3, 2, 4};
    transposeSquareInPlace(m1, 2);
    assert(arraysEqual(m1, expected1, 4));

    // Test 2: 3x3 matrix
    int m2[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int expected2[9] = {1, 4, 7, 2, 5, 8, 3, 6, 9};
    transposeSquareInPlace(m2, 3);
    assert(arraysEqual(m2, expected2, 9));

    // Test 3: width = 1, no change
    int m3[1] = {42};
    transposeSquareInPlace(m3, 1);
    assert(m3[0] == 42);

    // Test 4: 4x4 matrix with negative values and zeros
    int m4[16] = {0, -1, 2, -3, 4, -5, 6, -7, 8, -9, 10, -11, 12, -13, 14, -15};
    int expected4[16] = {0, 4, 8, 12, -1, -5, -9, -13, 2, 6, 10, 14, -3, -7, -11, -15};
    transposeSquareInPlace(m4, 4);
    assert(arraysEqual(m4, expected4, 16));

    // Test 5: 5x5 matrix with repeated values
    int m5[25] = {1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5};
    int expected5[25] = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
    transposeSquareInPlace(m5, 5);
    assert(arraysEqual(m5, expected5, 25));

    // Test 6: Identifying matrix transposed is itself
    int m6[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    transposeSquareInPlace(m6, 3);
    assert(m6[0] == 1 && m6[4] == 1 && m6[8] == 1);
    assert(m6[1] == 0 && m6[2] == 0 && m6[3] == 0 && m6[5] == 0 && m6[6] == 0 && m6[7] == 0);

    return 0;
}
