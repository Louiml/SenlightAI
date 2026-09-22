// Write a C++ function that takes a 4×4 integer matrix (represented as a 2D array or a fixed-size array of arrays, e.g., `int[4][4]`) and returns a new 4×4 matrix where the bottom two rows are set to zero, while the top two rows remain unchanged. The function should accept the input matrix by `const` reference, return a new matrix by value, and preserve the original input. Use a row-major ordering and assume all values are initialized. The function must work for any integer values, including negative numbers and zeros, without modifying the input.
// The solution approach is straightforward: create a copy of the input matrix, then overwrite the last two rows (indices 2 and 3) with zeros. The independent function should return the new matrix, leaving the original intact because the input is passed by `const` reference and a new local copy is made. Edge cases: the matrix is always 4×4, so no dynamic sizing is needed; however, ensure we handle all 8 cells in the bottom rows correctly. Time complexity is O(1) since the matrix size is fixed (16 cells total, 8 to zero). Space complexity is O(1) for the returned matrix itself (16 integers), plus a temporary copy during return (which the compiler may optimize via copy elision or move semantics). The function should be `const`-correct: take a `const` reference and return a non-`const` value.
#include <array>
#include <cstddef>

// Zero out the bottom two rows of a 4x4 integer matrix, returning the result.
std::array<std::array<int, 4>, 4> zeroBottomRows(
    const std::array<std::array<int, 4>, 4>& matrix) {
    
    auto result = matrix; // Copy all rows.
    
    // Set bottom two rows (indices 2 and 3) to zero.
    for (std::size_t row = 2; row < 4; ++row) {
        for (std::size_t col = 0; col < 4; ++col) {
            result[row][col] = 0;
        }
    }
    
    return result;
}
#include <cassert>
#include <array>

// The solution function is defined above; here is the test harness.
int main() {
    // Test 1: Basic case with distinct values.
    std::array<std::array<int, 4>, 4> a = {{
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    }};
    auto b = zeroBottomRows(a);
    assert((b == std::array<std::array<int, 4>, 4>{{
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }}));

    // Test 2: Original input remains unchanged.
    assert((a[2][0] == 9 && a[3][3] == 16));

    // Test 3: All zeros input.
    std::array<std::array<int, 4>, 4> zeros = {{
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }};
    auto zerosOut = zeroBottomRows(zeros);
    assert(zerosOut == zeros);

    // Test 4: Negative numbers and mixed values.
    std::array<std::array<int, 4>, 4> mixed = {{
        {-1, 2, -3, 4},
        {5, -6, 7, -8},
        {9, -10, 11, -12},
        {-13, 14, -15, 16}
    }};
    auto mixedOut = zeroBottomRows(mixed);
    assert(mixedOut[2][0] == 0 && mixedOut[3][2] == 0);
    assert(mixedOut[0][0] == -1 && mixedOut[1][3] == -8);
}
