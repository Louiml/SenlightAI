// Write a C++ function that takes a 4x4 matrix of doubles (represented as a `std::vector<std::vector<double>>`) and returns a new 4x4 matrix where the 2x2 block in the lower-left corner (rows 2-3, columns 0-1, using zero-based indexing) is replaced by the 2x2 block from the lower-right corner (rows 2-3, columns 2-3). All other elements remain unchanged. The function should not modify the input matrix; it should return a copy with the block copied. You may assume the input matrix always has exactly 4 rows and 4 columns, but you should still validate this and throw `std::invalid_argument` if the dimensions are not 4x4.

The solution constructs a result matrix by copying the input matrix, then extracts the lower-right and lower-left 2x2 blocks using nested loops. The lower-right block starts at row 2 and column 2; the lower-left block starts at row 2 and column 0. For each of the 2 rows and 2 columns, copy the value from `input[2 + i][2 + j]` into `result[2 + i][0 + j]`. Edge cases: must verify the matrix has exactly 4 rows and each row has exactly 4 columns; otherwise throw an exception. The algorithm runs in O(1) time since the matrix size is fixed (4x4), but generalizing to copying a block of size k from an n×n matrix takes O(k²) time, and copying the entire matrix takes O(n²) auxiliary space. The function is `const`-correct as it takes a `const` reference and returns a new vector.

#include <vector>
#include <stdexcept>

// Replace the lower-left 2x2 block of a 4x4 matrix with the lower-right 2x2 block.
// Returns a new matrix; input remains unchanged.
std::vector<std::vector<double>> replaceLowerLeftBlock(const std::vector<std::vector<double>>& matrix) {
    // Validate dimensions: exactly 4 rows and each row exactly 4 columns.
    if (matrix.size() != 4) {
        throw std::invalid_argument("Matrix must have exactly 4 rows.");
    }
    for (const auto& row : matrix) {
        if (row.size() != 4) {
            throw std::invalid_argument("Each row must have exactly 4 columns.");
        }
    }

    // Create a copy to modify.
    std::vector<std::vector<double>> result = matrix;

    // Copy from lower-right block (rows 2-3, cols 2-3) to lower-left (rows 2-3, cols 0-1).
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            result[2 + i][0 + j] = result[2 + i][2 + j];
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <stdexcept>

int main() {
    // Test 1: Basic replacement with known values.
    std::vector<std::vector<double>> m1 = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };
    std::vector<std::vector<double>> r1 = replaceLowerLeftBlock(m1);
    std::vector<std::vector<double>> expected1 = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {11, 12, 11, 12},
        {15, 16, 15, 16}
    };
    assert(r1 == expected1);
    // Input not modified.
    assert(m1[2][0] == 9 && m1[2][1] == 10);

    // Test 2: All zeros, result remains all zeros.
    std::vector<std::vector<double>> m2(4, std::vector<double>(4, 0.0));
    std::vector<std::vector<double>> r2 = replaceLowerLeftBlock(m2);
    assert(r2 == m2);

    // Test 3: Check that upper blocks remain unchanged.
    std::vector<std::vector<double>> m3 = {
        {1,0,0,0},
        {0,1,0,0},
        {0,0,2,3},
        {0,0,4,5}
    };
    std::vector<std::vector<double>> r3 = replaceLowerLeftBlock(m3);
    assert(r3[0][0] == 1 && r3[1][1] == 1);
    assert(r3[2][0] == 2 && r3[2][1] == 3);
    assert(r3[3][0] == 4 && r3[3][1] == 5);

    // Test 4: Invalid dimensions throw exception.
    bool threw = false;
    try {
        std::vector<std::vector<double>> bad = {{1,2,3}}; // 1 row, 3 cols
        replaceLowerLeftBlock(bad);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 5: Invalid column count in a row throws exception.
    threw = false;
    try {
        std::vector<std::vector<double>> bad = {
            {1,2,3,4},
            {1,2,3},
            {1,2,3,4},
            {1,2,3,4}
        };
        replaceLowerLeftBlock(bad);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
}
