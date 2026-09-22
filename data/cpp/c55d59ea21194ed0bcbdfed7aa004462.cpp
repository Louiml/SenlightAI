Write a C++ function `std::vector<double> zigzagScan(const std::vector<std::vector<double>>& matrix)` that takes an 8x8 matrix of doubles and returns a vector of 64 doubles in zigzag (JPEG-like) scan order. The zigzag order starts at the top-left corner (0,0), then moves to (0,1), (1,0), (2,0), (1,1), (0,2), and continues in alternating diagonal directions, covering all 64 elements. You may assume the input is always exactly 8x8. The function must be `const`-correct, meaning it should not modify the input matrix. Your implementation must handle both diagonals that go up-right (starting from the top row or left column) and diagonals that go down-left (starting from the bottom row or right column), matching the exact order shown in the reference code. Do not use any external libraries beyond the standard C++ headers.

The zigzag scan traverses all elements in a square grid by moving along diagonals. The pattern alternates between two directions: for diagonals that start at the top row or left column (the "first half" of the scan), the direction alternates: the first diagonal (length 1) goes down-right, the second (length 2) goes up-left, the third (length 3) goes down-right, and so on. For diagonals that start at the right column or bottom row (the "second half"), the direction pattern continues: the diagonal starting at (0,7) goes down-left, the one starting at (1,7) goes up-right, etc., until the last single-element diagonal at (7,7). A simple approach is to iterate over all diagonals identified by a starting coordinate. For the first half, diagonals start at (i,0) for i from 0 to 7, and for the second half, diagonals start at (7,j) for j from 1 to 7. For each diagonal of length L, if the diagonal index (i + starting column) is even, traverse from the start point down-right; if odd, traverse from the end point up-left. The time complexity is O(64) since we visit each element exactly once, and space complexity is O(1) extra (excluding the output vector, which is O(64) as required). Edge cases include the very first element (0,0) and the very last (7,7), which are both single-element diagonals; they are handled naturally by the diagonal-length logic.

#include <vector>
#include <cstddef>

// Returns a vector of 64 doubles in zigzag (JPEG) scan order from an 8x8 matrix.
// Assumes the input matrix is exactly 8x8.
std::vector<double> zigzagScan(const std::vector<std::vector<double>>& matrix) {
    const std::size_t N = 8;
    std::vector<double> result;
    result.reserve(N * N);

    // Process all diagonals.
    // First half: diagonals starting at (i, 0) for i = 0 to 7
    // Second half: diagonals starting at (7, j) for j = 1 to 7
    for (std::size_t start = 0; start < 2 * N - 1; ++start) {
        // Determine the starting row and column of the diagonal.
        std::size_t row = (start < N) ? start : N - 1;
        std::size_t col = (start < N) ? 0 : start - (N - 1);

        // Length of the current diagonal.
        std::size_t length = (start < N) ? start + 1 : 2 * N - 1 - start;

        // If the diagonal index (start) is even, traverse from (row, col) going down-right.
        // If odd, traverse from the opposite end going up-left.
        if (start % 2 == 0) {
            for (std::size_t k = 0; k < length; ++k) {
                result.push_back(matrix[row - k][col + k]);
            }
        } else {
            for (std::size_t k = 0; k < length; ++k) {
                result.push_back(matrix[row - (length - 1 - k)][col + (length - 1 - k)]);
            }
        }
    }

    return result;
}

#include <cassert>
#include <vector>
#include <cmath>

// The solution function (provided above) is assumed to be included here.
// For brevity, we replicate it in the test file directly or include the header.

int main() {
    // Test 1: Simple sequential matrix 0..63
    std::vector<std::vector<double>> m1(8, std::vector<double>(8));
    double val = 0;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            m1[i][j] = val++;
    
    std::vector<double> z1 = zigzagScan(m1);
    // Expected first few: 0, 1, 8, 16, 9, 2, 3, 10, 17, 24, ...
    assert(z1[0] == 0);
    assert(z1[1] == 1);
    assert(z1[2] == 8);
    assert(z1[3] == 16);
    assert(z1[4] == 9);
    assert(z1[5] == 2);
    assert(z1[6] == 3);
    assert(z1[7] == 10);
    assert(z1[8] == 17);
    assert(z1[9] == 24);
    assert(z1[10] == 32);
    assert(z1.size() == 64);
    // Last element should be 63
    assert(z1[63] == 63);

    // Test 2: All ones
    std::vector<std::vector<double>> m2(8, std::vector<double>(8, 1.0));
    std::vector<double> z2 = zigzagScan(m2);
    for (double x : z2) assert(x == 1.0);

    // Test 3: Zero matrix
    std::vector<std::vector<double>> m3(8, std::vector<double>(8, 0.0));
    std::vector<double> z3 = zigzagScan(m3);
    for (double x : z3) assert(x == 0.0);

    // Test 4: Check that scanning and reconstructing with a known inverse returns original
    // (Here we just verify the order by checking unique values from a pattern)
    std::vector<std::vector<double>> m4(8, std::vector<double>(8, 0.0));
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            m4[i][j] = i * 100 + j;
    std::vector<double> z4 = zigzagScan(m4);
    assert(z4[0] == 0.0);          // (0,0)
    assert(z4[1] == 1.0);          // (0,1)
    assert(z4[2] == 100.0);        // (1,0)
    assert(z4[3] == 200.0);        // (2,0)
    assert(z4[4] == 101.0);        // (1,1)
    assert(z4[63] == 707.0);       // (7,7)

    // Test 5: Negative values
    std::vector<std::vector<double>> m5(8, std::vector<double>(8, -1.0));
    m5[0][0] = -5.5;
    m5[7][7] = 3.14;
    std::vector<double> z5 = zigzagScan(m5);
    assert(z5[0] == -5.5);
    assert(z5[63] == 3.14);
    assert(z5[1] == -1.0);

    return 0;
}
