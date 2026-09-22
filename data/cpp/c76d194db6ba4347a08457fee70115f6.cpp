/*
Write a C++ function `broadcastAddToColumns` that takes a 2D floating-point matrix (represented as a vector of vectors, e.g., `std::vector<std::vector<float>>`) and a 1D vector of column offsets (with length equal to the matrix's row count). The function must modify the matrix in place such that each column `j` has the offset vector added element-wise to its rows. The function should validate that the offset vector length matches the number of rows; if not, throw a `std::invalid_argument`. The function must also handle the case of an empty matrix (zero rows and/or zero columns) gracefully by doing nothing. Use `const` references for inputs that are not modified. Include necessary headers and avoid using Eigen or any external library—implement with standard C++ containers only.
*/
#include <vector>
#include <stdexcept>

// Adds the given offset vector to each column of the matrix in place.
// The offset vector must have the same length as the number of rows in the matrix.
// Throws std::invalid_argument if sizes mismatch.
void broadcastAddToColumns(std::vector<std::vector<float>>& matrix,
                           const std::vector<float>& offset) {
    if (matrix.empty()) {
        // If matrix has no rows, offset must be empty; otherwise it's an error.
        if (!offset.empty()) {
            throw std::invalid_argument("Offset size must match matrix row count (0).");
        }
        return;
    }

    size_t rows = matrix.size();
    if (offset.size() != rows) {
        throw std::invalid_argument("Offset size must match matrix row count.");
    }

    size_t cols = matrix[0].size();
    for (size_t j = 0; j < cols; ++j) {
        for (size_t i = 0; i < rows; ++i) {
            matrix[i][j] += offset[i];
        }
    }
}
#include <cassert>
#include <vector>
#include <stdexcept>

int main() {
    // Test 1: Basic 2x4 matrix from the snippet (but with float).
    std::vector<std::vector<float>> mat = {
        {1, 2, 6, 9},
        {3, 1, 7, 2}
    };
    std::vector<float> offset = {0, 1};
    broadcastAddToColumns(mat, offset);
    assert(mat[0][0] == 1 && mat[0][1] == 2 && mat[0][2] == 6 && mat[0][3] == 9);
    assert(mat[1][0] == 4 && mat[1][1] == 2 && mat[1][2] == 8 && mat[1][3] == 3);

    // Test 2: Different offset values.
    std::vector<std::vector<float>> mat2 = {{1.5f, -2.0f}, {3.0f, 4.0f}};
    std::vector<float> offset2 = {0.5f, -1.0f};
    broadcastAddToColumns(mat2, offset2);
    assert(mat2[0][0] == 2.0f && mat2[0][1] == -1.5f);
    assert(mat2[1][0] == 2.0f && mat2[1][1] == 3.0f);

    // Test 3: Single row, single column.
    std::vector<std::vector<float>> mat3 = {{5.0f}};
    std::vector<float> offset3 = {2.0f};
    broadcastAddToColumns(mat3, offset3);
    assert(mat3[0][0] == 7.0f);

    // Test 4: Zero columns (rows exist, columns=0).
    std::vector<std::vector<float>> mat4 = {{}, {}};
    std::vector<float> offset4 = {1.0f, 2.0f};
    broadcastAddToColumns(mat4, offset4);
    assert(mat4[0].empty() && mat4[1].empty());

    // Test 5: Empty matrix (no rows) with empty offset does nothing.
    std::vector<std::vector<float>> mat5;
    std::vector<float> offset5;
    broadcastAddToColumns(mat5, offset5);
    assert(mat5.empty());

    // Test 6: Mismatch throws.
    std::vector<std::vector<float>> mat6 = {{1.0f}};
    std::vector<float> offset6 = {1.0f, 2.0f};
    bool threw = false;
    try {
        broadcastAddToColumns(mat6, offset6);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
}
// The solution iterates over each column of the matrix (indexed by `j` from 0 to number of columns - 1) and each row (indexed by `i` from 0 to number of rows - 1). For each element `matrix[i][j]`, add the corresponding offset `offset[i]` to it. Since the offset vector’s length must equal the number of rows, we first check that condition and throw `std::invalid_argument` if violated. Edge cases: if the matrix has zero columns, the loop does nothing; if it has zero rows, the offset vector must be empty (length 0) to pass validation, and we do nothing. The algorithm runs in O(R*C) time where R is rows and C is columns, and uses O(1) extra space beyond the input (no temporary storage). The function modifies the matrix in place, so no copy is needed.
