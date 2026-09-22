// Write a C++ function that takes a 4x4 integer matrix (represented as `std::array<std::array<int,4>,4>` or a similar container) and returns a new 4x4 matrix where the 2x2 block in the top-right corner is zeroed out, while all other elements remain unchanged. The function must accept a const reference to the input matrix and return a new matrix by value. Also write a helper function to print the matrix for debugging if needed, but the main solution function must be pure (no side effects).
The solution approach is straightforward: copy the input matrix into a new local matrix, then set all elements in the top-right 2x2 block (rows 0 and 1, columns 2 and 3) to zero. Because the input is taken by const reference, we must make a copy first before modifying. The block indices are fixed: for a 4x4 matrix, `topRightCorner(2,2)` corresponds to rows `0..1` and columns `2..3`. We iterate over those row and column pairs and assign `0`. Edge cases: no special ones since the size is fixed to 4x4. Time complexity: \(O(16)\) = \(O(1)\) because the matrix size is constant, but conceptually copying and zeroing 4 elements per row takes constant time. Space complexity: \(O(1)\) extra beyond the returned matrix copy (which is \(O(16)\)).
#include <array>
#include <algorithm>

using Matrix4i = std::array<std::array<int, 4>, 4>;

// Returns a copy of the input matrix with the top-right 2x2 block zeroed.
Matrix4i zeroTopRightCorner(const Matrix4i& input) {
    Matrix4i result = input;  // make a copy
    constexpr int block_size = 2;
    // The top-right block occupies rows 0..1 and columns 2..3.
    for (int row = 0; row < block_size; ++row) {
        for (int col = 4 - block_size; col < 4; ++col) {
            result[row][col] = 0;
        }
    }
    return result;
}
#include <cassert>

int main() {
    // Helper to build a matrix filled with sequential values 1..16 for validation.
    auto makeMatrix = [](int start = 1) {
        Matrix4i m;
        int val = start;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                m[i][j] = val++;
        return m;
    };

    // Test 1: Identity-like pattern with values 1..16.
    Matrix4i m1 = makeMatrix(1);
    Matrix4i r1 = zeroTopRightCorner(m1);
    // Expected: top-right 2x2 zeroed => positions (0,2),(0,3),(1,2),(1,3) become 0.
    // Build expected matrix manually.
    Matrix4i expected1 = makeMatrix(1);
    expected1[0][2] = 0; expected1[0][3] = 0;
    expected1[1][2] = 0; expected1[1][3] = 0;
    assert(r1 == expected1);

    // Test 2: Ensure original matrix unchanged (const correctness).
    Matrix4i original = makeMatrix(5);
    Matrix4i copy = original;
    Matrix4i r2 = zeroTopRightCorner(original);
    assert(original == copy); // original unchanged
    // Check that top-right block in result is zero.
    for (int i = 0; i < 2; ++i)
        for (int j = 2; j < 4; ++j)
            assert(r2[i][j] == 0);
    // Check that other elements are same as original.
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (!(i < 2 && j >= 2))
                assert(r2[i][j] == original[i][j]);

    // Test 3: All zeros input remains all zeros.
    Matrix4i allZero{};
    Matrix4i r3 = zeroTopRightCorner(allZero);
    assert(r3 == allZero);

    // Test 4: All ones input becomes only top-right 2x2 zeros.
    Matrix4i allOnes;
    for (auto& row : allOnes) row.fill(1);
    Matrix4i r4 = zeroTopRightCorner(allOnes);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            if (i < 2 && j >= 2) assert(r4[i][j] == 0);
            else assert(r4[i][j] == 1);
        }

    // Test 5: Random-ish matrix pattern.
    Matrix4i m5 = makeMatrix(10);
    Matrix4i r5 = zeroTopRightCorner(m5);
    // Verify only the 4 specified cells are zero.
    int zero_count = 0;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            if (r5[i][j] == 0) zero_count++;
        }
    assert(zero_count == 4);
    // Verify those zeros are exactly at the expected positions.
    assert(r5[0][2] == 0 && r5[0][3] == 0 && r5[1][2] == 0 && r5[1][3] == 0);

    return 0;
}
