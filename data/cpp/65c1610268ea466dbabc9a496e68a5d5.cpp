// Write a C++ function that takes a square matrix represented as a `std::vector<std::vector<double>>` and returns a `std::vector<double>` where each element is the maximum absolute value among the elements in that row of the matrix. The input is guaranteed to be a non-empty square matrix (same number of rows and columns, at least 1×1). The function should handle negative values correctly by using their absolute values, and should not modify the input matrix. The returned vector must have the same length as the number of rows, with the i-th element corresponding to the row-wise maximum of `abs(matrix[i][j])` for all valid column indices `j`.
#include <cassert>
#include <vector>

int main() {
    // Test 1: simple 2x2 with positives and negatives
    std::vector<std::vector<double>> m1 = {{1.0, -4.0}, {3.0, 2.0}};
    std::vector<double> r1 = rowMaxAbs(m1);
    assert(r1.size() == 2);
    assert(r1[0] == 4.0);
    assert(r1[1] == 3.0);

    // Test 2: 1x1 matrix
    std::vector<std::vector<double>> m2 = {{-7.5}};
    std::vector<double> r2 = rowMaxAbs(m2);
    assert(r2.size() == 1);
    assert(r2[0] == 7.5);

    // Test 3: all zeros
    std::vector<std::vector<double>> m3 = {{0.0, 0.0}, {0.0, 0.0}};
    std::vector<double> r3 = rowMaxAbs(m3);
    assert(r3[0] == 0.0 && r3[1] == 0.0);

    // Test 4: 3x3 with duplicates and mixed signs
    std::vector<std::vector<double>> m4 = {{-2.0, 2.0, -2.0}, {0.5, -0.5, 0.5}, {100.0, -1.0, 50.0}};
    std::vector<double> r4 = rowMaxAbs(m4);
    assert(r4[0] == 2.0);
    assert(r4[1] == 0.5);
    assert(r4[2] == 100.0);

    // Test 5: ensure input not modified
    std::vector<std::vector<double>> m5 = {{1.0, -3.0}, {2.0, -4.0}};
    std::vector<std::vector<double>> original = m5;
    rowMaxAbs(m5);
    assert(m5 == original);
}
#include <vector>
#include <cmath>
#include <algorithm>

// Returns a vector where element i is the maximum absolute value in row i of the input square matrix.
std::vector<double> rowMaxAbs(const std::vector<std::vector<double>>& matrix) {
    const std::size_t rows = matrix.size();
    std::vector<double> result(rows);
    for (std::size_t i = 0; i < rows; ++i) {
        const auto& row = matrix[i];
        double max_val = std::abs(row[0]);
        for (std::size_t j = 1; j < row.size(); ++j) {
            max_val = std::max(max_val, std::abs(row[j]));
        }
        result[i] = max_val;
    }
    return result;
}
// The solution iterates over each row of the matrix. For each row, it initializes a running maximum with the absolute value of the first element in that row, then scans the remaining elements (if any) and updates the maximum whenever the absolute value of a later element is larger. Because the matrix is square and non-empty, every row has at least one element, so no empty-row edge case exists. The function must use `std::abs` from `<cmath>` to handle possibly negative doubles. The input matrix is passed by `const` reference to avoid copying and to enforce non-modification. Time complexity is O(n²) where n is the number of rows (or columns), since we visit each of the n² elements exactly once. Space complexity is O(n) for the output vector, plus O(1) auxiliary space for the running maximum and loop indices. Edge cases include a 1×1 matrix (returns its absolute value) and rows containing only zeros (maximum is 0). Floating-point comparisons are exact for absolute values as computed directly, so no epsilon tolerance is needed for the specific test cases provided.
