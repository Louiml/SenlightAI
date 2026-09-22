// Given a non-empty 3x3 matrix of doubles represented as a `std::array<std::array<double, 3>, 3>` (row-major), write a C++ function named `columnWiseMax` that returns a `std::array<double, 3>` where the i-th element is the maximum value in the i-th column of the input matrix. The function must be `const`-correct, take the matrix by constant reference, and must not modify the input. The input matrix will always have exactly 3 rows and 3 columns, may contain negative numbers, zeros, and duplicate values. If a column has all equal values, that value is the maximum for that column.
The task is a straightforward column-wise reduction. Since the matrix is fixed at 3x3, we can iterate over each column index `col` from 0 to 2, and for each column, iterate over each row `row` from 0 to 2, updating a running maximum initialized to a very small value (e.g., `-std::numeric_limits<double>::infinity()` or simply the first element of that column). The main algorithm is: for each column, start with `maxVal = matrix[0][col]`, then for rows 1 and 2 compare and update. Edge cases: duplicate values (handled naturally because `>` comparison will keep the same max), negative numbers (initialized correctly), and zeros (no issue). Time complexity: since the matrix dimensions are constant (3x3), the time is O(9) = O(1) in asymptotic terms, but conceptually it's O(rows * cols) for a general matrix. Space complexity is O(1) for the output array and no additional dynamic memory. The solution uses `.at()` or direct indexing; we use direct indexing for efficiency but note that bounds are guaranteed.
#include <array>
#include <algorithm>
#include <limits>

// Return an array where element i is the maximum value in column i of the input 3x3 matrix.
// The matrix is given in row-major order: matrix[row][col].
std::array<double, 3> columnWiseMax(const std::array<std::array<double, 3>, 3>& matrix) {
    std::array<double, 3> result;
    for (std::size_t col = 0; col < 3; ++col) {
        // Initialize with the first element of this column.
        double maxVal = matrix[0][col];
        // Compare with the remaining two rows.
        for (std::size_t row = 1; row < 3; ++row) {
            if (matrix[row][col] > maxVal) {
                maxVal = matrix[row][col];
            }
        }
        result[col] = maxVal;
    }
    return result;
}
#include <cassert>
#include <array>
#include <cmath>

// The solution function is declared above (or included via header in a real project).
// For completeness, we duplicate a minimal declaration here for compilation:
std::array<double, 3> columnWiseMax(const std::array<std::array<double, 3>, 3>& matrix);

int main() {
    // Test 1: Simple positive numbers
    std::array<std::array<double, 3>, 3> m1 = {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}};
    auto r1 = columnWiseMax(m1);
    assert(r1 == std::array<double, 3>({7, 8, 9}));

    // Test 2: Negative numbers
    std::array<std::array<double, 3>, 3> m2 = {{{-1, -2, -3}, {-4, -5, -6}, {-7, -8, -9}}};
    assert(columnWiseMax(m2) == std::array<double, 3>({-1, -2, -3}));

    // Test 3: Mixed values and zeros
    std::array<std::array<double, 3>, 3> m3 = {{{0, -5, 2}, {3, 0, 1}, {-1, 4, -2}}};
    assert(columnWiseMax(m3) == std::array<double, 3>({3, 4, 2}));

    // Test 4: All equal in one column
    std::array<std::array<double, 3>, 3> m4 = {{{5, 1, 9}, {5, 3, 8}, {5, 2, 7}}};
    assert(columnWiseMax(m4) == std::array<double, 3>({5, 3, 9}));

    // Test 5: Duplicate maximum values
    std::array<std::array<double, 3>, 3> m5 = {{{2, 4, 4}, {4, 1, 3}, {4, 4, 2}}};
    assert(columnWiseMax(m5) == std::array<double, 3>({4, 4, 4}));

    // Test 6: Non-integer doubles
    std::array<std::array<double, 3>, 3> m6 = {{{1.5, 0.1, -2.2}, {0.9, 0.5, -1.1}, {2.3, 2.0, -0.5}}};
    auto r6 = columnWiseMax(m6);
    assert(std::abs(r6[0] - 2.3) < 1e-9);
    assert(std::abs(r6[1] - 2.0) < 1e-9);
    assert(std::abs(r6[2] - (-0.5)) < 1e-9);

    return 0;
}
