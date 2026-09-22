// Write a C++ function that takes a 4x4 matrix of integers (represented as a fixed-size 2D array or a custom `struct`/`class` with a 2D array member) and modifies it in-place so that the 2x2 sub-block in the top-right corner is entirely set to zero. The function must also return a boolean indicating whether any of the original values in that top-right 2x2 block were non-zero before being zeroed. The input matrix is provided as a `std::array<std::array<int, 4>, 4>` (or equivalent). You must implement the function without using any external matrix libraries, and must not copy the whole matrix; only the top‑right 2×2 block may be modified. All other elements (the 12 remaining entries) must remain unchanged. The function should be `const`‑correct for read‑only operations and handle both positive and negative integers.
The core operation is straightforward: for a 4x4 matrix, the top-right 2x2 block consists of rows 0 and 1, and columns 2 and 3. We need to iterate over these four positions, note if any value is non-zero (by checking `!= 0`), and then set each to zero. Since we modify in-place, we must not accidentally skip positions. The order of checking and zeroing can be done in the same loop: for each of the four positions, read the value, compare to zero, then assign zero. The return value is the logical OR of all non-zero flags; if any position had a non‑zero value, return `true`; otherwise return `false`. Edge cases: all zeros in the block → return `false`; any non‑zero (positive or negative) → return `true`. The matrix is fixed size 4x4, so time complexity is O(1) (constant number of operations, specifically 4 checks and 4 assignments). Space complexity is O(1) – only a local boolean variable for the result. No dynamic allocation or extra data structures are needed.
#include <array>
#include <cstddef>

// Modifies the top-right 2x2 sub-block of a 4x4 matrix in-place.
// Returns true if any value in that block was non-zero before zeroing.
bool zeroTopRight2x2(std::array<std::array<int, 4>, 4>& matrix) {
    bool any_non_zero = false;
    for (std::size_t row = 0; row < 2; ++row) {
        for (std::size_t col = 2; col < 4; ++col) {
            if (matrix[row][col] != 0) {
                any_non_zero = true;
            }
            matrix[row][col] = 0;
        }
    }
    return any_non_zero;
}
#include <cassert>

int main() {
    // Test 1: Non-zero values in the block -> returns true, block zeroed.
    std::array<std::array<int, 4>, 4> m1 = {{
        {{1, 2, 3, 4}},
        {{5, 6, 7, 8}},
        {{9, 10, 11, 12}},
        {{13, 14, 15, 16}}
    }};
    assert(zeroTopRight2x2(m1) == true);
    assert(m1[0][2] == 0 && m1[0][3] == 0);
    assert(m1[1][2] == 0 && m1[1][3] == 0);
    // Other elements unchanged.
    assert(m1[0][0] == 1 && m1[0][1] == 2);
    assert(m1[1][0] == 5 && m1[1][1] == 6);
    assert(m1[2][0] == 9 && m1[2][1] == 10 && m1[2][2] == 11 && m1[2][3] == 12);
    assert(m1[3][0] == 13 && m1[3][1] == 14 && m1[3][2] == 15 && m1[3][3] == 16);

    // Test 2: All zeros in the block -> returns false, nothing changes.
    std::array<std::array<int, 4>, 4> m2 = {{
        {{0, 0, 0, 0}},
        {{0, 0, 0, 0}},
        {{1, 2, 3, 4}},
        {{5, 6, 7, 8}}
    }};
    assert(zeroTopRight2x2(m2) == false);
    // All entries remain the same.
    for (auto& row : m2) {
        for (int v : row) {
            assert(v == 0 || v >= 1); // but we can just check key positions
        }
    }
    assert(m2[0][2] == 0 && m2[0][3] == 0 && m2[1][2] == 0 && m2[1][3] == 0);

    // Test 3: Negative numbers in the block -> returns true.
    std::array<std::array<int, 4>, 4> m3 = {{
        {{-1, -2, -3, -4}},
        {{-5, -6, -7, -8}},
        {{0, 0, 0, 0}},
        {{0, 0, 0, 0}}
    }};
    assert(zeroTopRight2x2(m3) == true);
    assert(m3[0][2] == 0 && m3[0][3] == 0);
    assert(m3[1][2] == 0 && m3[1][3] == 0);
    assert(m3[0][0] == -1 && m3[1][1] == -6);

    // Test 4: Only one non-zero in the block -> returns true.
    std::array<std::array<int, 4>, 4> m4 = {{
        {{0, 0, 42, 0}},
        {{0, 0, 0, 0}},
        {{1, 2, 3, 4}},
        {{5, 6, 7, 8}}
    }};
    assert(zeroTopRight2x2(m4) == true);
    assert(m4[0][2] == 0 && m4[0][3] == 0);
    assert(m4[1][2] == 0 && m4[1][3] == 0);

    // Test 5: Matrix with random values, verify those not in block are untouched.
    std::array<std::array<int, 4>, 4> m5 = {{
        {{10, 20, 30, 40}},
        {{50, 60, 70, 80}},
        {{90, 100, 110, 120}},
        {{130, 140, 150, 160}}
    }};
    auto original = m5; // copy for comparison
    assert(zeroTopRight2x2(m5) == true);
    // Check block zeroed.
    for (int r = 0; r < 2; ++r) {
        for (int c = 2; c < 4; ++c) {
            assert(m5[r][c] == 0);
        }
    }
    // Check others unchanged.
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            if (r < 2 && c >= 2) continue; // skip block
            assert(m5[r][c] == original[r][c]);
        }
    }

    return 0;
}
