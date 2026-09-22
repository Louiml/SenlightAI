/*
Write a standalone C++ function that takes a 3x3 matrix of doubles (represented as `std::array<std::array<double, 3>, 3>` or a simple struct) and returns a `std::array<double, 3>` containing the minimum value of each column, in order from column 0 to column 2. The function must be `const`-correct, handle arbitrary double values including negatives and zeros, and not modify the input. The task is independent of any external linear algebra library; implement it using only standard C++ constructs.
*/
#include <array>

// Given a 3x3 matrix of doubles, return an array of three doubles
// containing the minimum value of each column (column 0, 1, 2).
std::array<double, 3> columnMins(const std::array<std::array<double, 3>, 3>& matrix) {
    std::array<double, 3> result;
    for (int col = 0; col < 3; ++col) {
        double min_val = matrix[0][col];
        for (int row = 1; row < 3; ++row) {
            if (matrix[row][col] < min_val) {
                min_val = matrix[row][col];
            }
        }
        result[col] = min_val;
    }
    return result;
}
#include <cassert>
#include <array>

std::array<double, 3> columnMins(const std::array<std::array<double, 3>, 3>& matrix);

int main() {
    // Test 1: Simple positive matrix
    std::array<std::array<double, 3>, 3> m1 = {{{1.0, 2.0, 3.0},
                                                 {4.0, 5.0, 6.0},
                                                 {7.0, 8.0, 9.0}}};
    auto r1 = columnMins(m1);
    assert(r1[0] == 1.0 && r1[1] == 2.0 && r1[2] == 3.0);

    // Test 2: Negative values and zeros
    std::array<std::array<double, 3>, 3> m2 = {{{-1.0, 0.0, 3.0},
                                                 {-5.0, -2.0, 1.0},
                                                 {0.0, -10.0, 2.0}}};
    auto r2 = columnMins(m2);
    assert(r2[0] == -5.0 && r2[1] == -10.0 && r2[2] == 1.0);

    // Test 3: All equal values
    std::array<std::array<double, 3>, 3> m3 = {{{7.0, 7.0, 7.0},
                                                 {7.0, 7.0, 7.0},
                                                 {7.0, 7.0, 7.0}}};
    auto r3 = columnMins(m3);
    assert(r3[0] == 7.0 && r3[1] == 7.0 && r3[2] == 7.0);

    // Test 4: Mixed with large and small numbers
    std::array<std::array<double, 3>, 3> m4 = {{{100.0, -0.5, 3.14},
                                                 {-200.0, 2.0, -3.14},
                                                 {0.0, 1.0, 2.718}}};
    auto r4 = columnMins(m4);
    assert(r4[0] == -200.0 && r4[1] == -0.5 && r4[2] == -3.14);

    // Test 5: Duplicates in columns
    std::array<std::array<double, 3>, 3> m5 = {{{2.0, 1.0, 5.0},
                                                 {2.0, 3.0, 5.0},
                                                 {2.0, 0.0, 5.0}}};
    auto r5 = columnMins(m5);
    assert(r5[0] == 2.0 && r5[1] == 0.0 && r5[2] == 5.0);

    return 0;
}
// The problem is straightforward: for each column index `j` (0 to 2), iterate over all row indices `i` (0 to 2) and track the smallest value seen. Initialize each column's minimum to the first element of that column (e.g., `matrix[0][j]`), then compare with the remaining rows. This avoids issues with initialization to a sentinel that might not exist in the data. Edge cases: the matrix is always exactly 3x3, so there are no empty columns or mismatched dimensions. Negative values and duplicates are handled naturally by the comparison. Time complexity is O(9) = O(1) since the matrix size is fixed at 3×3. Space complexity is O(1) auxiliary, since we only store the three minimum values in the output array.
