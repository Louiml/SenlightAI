// Write a C++ function that takes a 4x4 matrix of integers (represented as a `std::array<std::array<int,4>,4>` or a simple 2D C-style array via a reference) and returns a new 4x4 matrix where the top-right 2x2 block has been set to zero, leaving all other elements unchanged. The function should not modify the input matrix; it should return a copy with the zeroed block. The input matrix may contain any integer values, including negatives and zeros. The solution must work for any 4x4 integer matrix.
The main task is to copy the input matrix and then set the elements in the top-right 2x2 block to zero. The top-right 2x2 block corresponds to rows 0 and 1 (0-indexed) and columns 2 and 3 (0-indexed). The algorithm is straightforward: create a copy of the input matrix, then loop over rows 0–1 and columns 2–3 and assign 0 to each. Since the matrix size is fixed at 4x4, the time complexity is O(16) = O(1) for both copying and zeroing, and the space complexity is O(1) auxiliary (the returned matrix itself is the output). Edge cases include when the matrix already contains zeros in that block (no issue), or when it contains very large or negative values (still fine; only the top-right block is overwritten). No other elements are touched.
#include <array>
#include <cstddef>

// Returns a copy of the 4x4 input matrix with its top-right 2x2 block set to zero.
// The input matrix is not modified.
std::array<std::array<int, 4>, 4> zeroTopRightBlock(
    const std::array<std::array<int, 4>, 4>& matrix) {
    // Make a copy of the input matrix.
    auto result = matrix;
    // Zero the top-right 2x2 block: rows 0-1, columns 2-3.
    for (std::size_t row = 0; row < 2; ++row) {
        for (std::size_t col = 2; col < 4; ++col) {
            result[row][col] = 0;
        }
    }
    return result;
}
#include <cassert>
#include <array>

int main() {
    // Test 1: Basic case with a random-like matrix.
    std::array<std::array<int, 4>, 4> m1 = {{
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    }};
    auto r1 = zeroTopRightBlock(m1);
    // Check that the top-right 2x2 block is zero.
    assert(r1[0][2] == 0 && r1[0][3] == 0);
    assert(r1[1][2] == 0 && r1[1][3] == 0);
    // Check that other elements are unchanged.
    assert(r1[0][0] == 1 && r1[0][1] == 2);
    assert(r1[1][0] == 5 && r1[1][1] == 6);
    assert(r1[2][0] == 9 && r1[2][1] == 10 && r1[2][2] == 11 && r1[2][3] == 12);
    assert(r1[3][0] == 13 && r1[3][1] == 14 && r1[3][2] == 15 && r1[3][3] == 16);
    // Original matrix must be unchanged.
    assert(m1[0][2] == 3 && m1[1][3] == 8);

    // Test 2: All zeros input (top-right block already zero).
    std::array<std::array<int, 4>, 4> m2 = {{0}};
    auto r2 = zeroTopRightBlock(m2);
    for (auto& row : r2) for (int val : row) assert(val == 0);

    // Test 3: Negative values in the top-right block.
    std::array<std::array<int, 4>, 4> m3 = {{
        {-1, -2, -3, -4},
        {-5, -6, -7, -8},
        {1, 2, 3, 4},
        {5, 6, 7, 8}
    }};
    auto r3 = zeroTopRightBlock(m3);
    assert(r3[0][2] == 0 && r3[0][3] == 0);
    assert(r3[1][2] == 0 && r3[1][3] == 0);
    assert(r3[0][0] == -1 && r3[1][1] == -6);
    assert(r3[2][0] == 1 && r3[3][3] == 8);

    // Test 4: Check that the function does not modify input.
    std::array<std::array<int, 4>, 4> m4 = {{
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    }};
    auto before = m4;
    auto r4 = zeroTopRightBlock(m4);
    assert(m4 == before); // input unchanged

    return 0;
}
