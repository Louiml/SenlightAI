// Write a C++ function named `sumMatrixAndArray` that takes a 3x3 integer matrix (represented as a `std::array<std::array<int,3>,3>` or a C-style 2D array) and a 1D integer array of exactly 9 elements, adds the matrix elements (in row-major order) to the corresponding array elements, and returns the resulting 1D `std::array<int,9>` (or pointer to an array). The function must handle negative numbers and ensure no overflow for typical `int` values (since the problem is pedagogical, assume inputs fit in `int`). The function should be `const`-correct with respect to input parameters and should not modify the original matrix or array. The output order is strictly row-major (element (0,0) added to index 0, element (0,1) added to index 1, ..., element (2,2) added to index 8).

// The solution iterates over the 9 positions in row-major order: for each index `k` from 0 to 8, compute `row = k / 3` and `col = k % 3`, then set `result[k] = matrix[row][col] + array[k]`. This works for both C-style arrays and `std::array`. Edge cases: empty inputs are not applicable since sizes are fixed; negative numbers are handled naturally by integer addition; potential overflow is ignored per the task. Time complexity is O(9) = O(1) constant, space complexity is O(1) auxiliary (ignoring the output container). If using a C-style 2D array parameter, it decays to a pointer to array of 3 ints, so the function signature must correctly declare that. For clarity, we use `std::array` for both inputs and output, which avoids pointer decay issues and is more modern C++.

#include <array>

// Add a 3x3 matrix (row-major) to a 9-element array, returning the summed array.
std::array<int, 9> sumMatrixAndArray(
    const std::array<std::array<int, 3>, 3>& matrix,
    const std::array<int, 9>& arr) {
    std::array<int, 9> result;
    for (int k = 0; k < 9; ++k) {
        int row = k / 3;
        int col = k % 3;
        result[k] = matrix[row][col] + arr[k];
    }
    return result;
}

#include <cassert>
#include <array>

// Function under test
std::array<int, 9> sumMatrixAndArray(
    const std::array<std::array<int, 3>, 3>& matrix,
    const std::array<int, 9>& arr);

int main() {
    // Test 1: Basic positive numbers
    std::array<std::array<int, 3>, 3> m1 = {{{1,2,3},{4,5,6},{7,8,9}}};
    std::array<int, 9> a1 = {9,8,7,6,5,4,3,2,1};
    std::array<int, 9> expected1 = {10,10,10,10,10,10,10,10,10};
    assert(sumMatrixAndArray(m1, a1) == expected1);

    // Test 2: Mixed signs and zeros
    std::array<std::array<int, 3>, 3> m2 = {{{-1,-2,-3},{4,0,-6},{7,8,-9}}};
    std::array<int, 9> a2 = {1,2,3,4,5,6,7,8,9};
    std::array<int, 9> expected2 = {0,0,0,8,5,0,14,16,0};
    assert(sumMatrixAndArray(m2, a2) == expected2);

    // Test 3: Matrix all zeros
    std::array<std::array<int, 3>, 3> m3 = {{{0,0,0},{0,0,0},{0,0,0}}};
    std::array<int, 9> a3 = {1,2,3,4,5,6,7,8,9};
    assert(sumMatrixAndArray(m3, a3) == a3);

    // Test 4: Array all zeros
    std::array<std::array<int, 3>, 3> m4 = {{{1,2,3},{4,5,6},{7,8,9}}};
    std::array<int, 9> a4 = {0,0,0,0,0,0,0,0,0};
    std::array<int, 9> expected4 = {1,2,3,4,5,6,7,8,9};
    assert(sumMatrixAndArray(m4, a4) == expected4);

    // Test 5: Negative array values
    std::array<std::array<int, 3>, 3> m5 = {{{5,6,7},{8,9,10},{11,12,13}}};
    std::array<int, 9> a5 = {-5,-6,-7,-8,-9,-10,-11,-12,-13};
    std::array<int, 9> expected5 = {0,0,0,0,0,0,0,0,0};
    assert(sumMatrixAndArray(m5, a5) == expected5);

    // Test 6: Large values within int range (60000)
    std::array<std::array<int, 3>, 3> m6 = {{{60000,1,2},{3,4,5},{6,7,8}}};
    std::array<int, 9> a6 = {1,2,3,4,5,6,7,8,9};
    std::array<int, 9> expected6 = {60001,3,5,7,9,11,13,15,17};
    assert(sumMatrixAndArray(m6, a6) == expected6);

    return 0;
}
