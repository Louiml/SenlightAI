// Write a C++ function named `rowwiseAbsMax` that takes a 3x3 matrix of floating-point numbers (represented as a `std::array<std::array<double, 3>, 3>` or a 2D array) and returns a `std::array<double, 3>` containing the maximum absolute value of each row. For each row, compute the absolute value of each element and find the largest among them. The function must be `const`‑correct (i.e., it should not modify the input matrix), and it should work for any 3x3 matrix, including those with negative values, zeros, and values with large magnitudes. The input matrix is guaranteed to be exactly 3x3. Do not use any external libraries beyond the standard C++ library.
#include <cassert>
#include <cmath>

int main() {
    // Test 1: simple positive values
    std::array<std::array<double, 3>, 3> m1 = {{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}}};
    auto r1 = rowwiseAbsMax(m1);
    assert(r1[0] == 3.0 && r1[1] == 6.0 && r1[2] == 9.0);

    // Test 2: mixed signs
    std::array<std::array<double, 3>, 3> m2 = {{{-1.5, 2.0, -3.5}, {4.0, -5.0, 6.0}, {-7.0, 8.0, -9.0}}};
    auto r2 = rowwiseAbsMax(m2);
    assert(r2[0] == 3.5 && r2[1] == 6.0 && r2[2] == 9.0);

    // Test 3: all zeros
    std::array<std::array<double, 3>, 3> m3 = {{{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}};
    auto r3 = rowwiseAbsMax(m3);
    assert(r3[0] == 0.0 && r3[1] == 0.0 && r3[2] == 0.0);

    // Test 4: large negative numbers
    std::array<std::array<double, 3>, 3> m4 = {{{-100.0, -1.0, -0.5}, {-200.0, -2.0, -3.0}, {-1e6, -1e5, -1e4}}};
    auto r4 = rowwiseAbsMax(m4);
    assert(r4[0] == 100.0 && r4[1] == 200.0 && r4[2] == 1e6);

    // Test 5: row with identical values
    std::array<std::array<double, 3>, 3> m5 = {{{2.0, 2.0, 2.0}, {-3.0, -3.0, -3.0}, {0.5, 0.5, 0.5}}};
    auto r5 = rowwiseAbsMax(m5);
    assert(r5[0] == 2.0 && r5[1] == 3.0 && r5[2] == 0.5);

    // Test 6: values with fractional parts
    std::array<std::array<double, 3>, 3> m6 = {{{0.1, -0.2, 0.3}, {-0.4, 0.5, -0.6}, {0.7, -0.8, 0.9}}};
    auto r6 = rowwiseAbsMax(m6);
    assert(std::abs(r6[0] - 0.3) < 1e-9);
    assert(std::abs(r6[1] - 0.6) < 1e-9);
    assert(std::abs(r6[2] - 0.9) < 1e-9);

    return 0;
}
#include <array>
#include <cmath>
#include <algorithm>

// Compute the maximum absolute value for each row of a 3x3 matrix.
// Returns a std::array<double, 3> where result[r] is the max of abs(matrix[r][c]).
std::array<double, 3> rowwiseAbsMax(const std::array<std::array<double, 3>, 3>& matrix) {
    std::array<double, 3> result{};
    for (int r = 0; r < 3; ++r) {
        double row_max = std::abs(matrix[r][0]);
        for (int c = 1; c < 3; ++c) {
            row_max = std::max(row_max, std::abs(matrix[r][c]));
        }
        result[r] = row_max;
    }
    return result;
}
// The solution iterates over each row (index `r` from 0 to 2) and each column (index `c` from 0 to 2). For each row, initialize a local variable `row_max` to the absolute value of the first element in that row (or to `-INFINITY` and then update). Then for each element in the row, compute `std::abs(matrix[r][c])` and update `row_max` if it is larger. After finishing the row, store `row_max` in the output array. Complexity is \(O(3 \times 3) = O(1)\) time and \(O(1)\) auxiliary space (only the output array and a temporary variable). Edge cases: all elements are zero (output should be zeros), all elements are negative (absolute values turn them positive), or values are very large (still handled by `std::abs` for `double`). Since the size is fixed, no dynamic allocation is needed.
