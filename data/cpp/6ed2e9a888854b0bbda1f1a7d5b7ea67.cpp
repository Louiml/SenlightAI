Write a C++ function that takes a 4×4 matrix of integers (represented as a nested `std::array<std::array<int,4>,4>` or a custom `Matrix4i` type) and modifies it so that the bottom two rows are set to zero, and then returns the modified matrix. The function should not mutate the input matrix; it should return a new matrix with the bottom two rows zeroed and the top two rows unchanged. The function must work correctly for matrices containing arbitrary integers, including negatives and zeros, and must preserve the order of elements in the top rows.
// The solution must copy the input matrix into a new 4×4 matrix. Then, for rows indexed 2 and 3 (0-based), set every element to 0. The top rows (indices 0 and 1) remain as in the input. The main algorithm is straightforward: iterate over the two bottom rows and assign zero to each of the four columns. Edge cases: the input is always exactly 4×4, so no size checks are needed; values are integers, so zero assignment is trivial. Time complexity is O(1) because the matrix size is constant (4×4 = 16 elements, but only 8 are modified). Space complexity is O(1) for the new matrix (also constant). The function should be `const`-correct: the input parameter is a `const` reference, and the function returns a new matrix by value.
#include <array>
#include <cstddef>

// Define a convenient alias for a 4x4 integer matrix.
using Matrix4i = std::array<std::array<int, 4>, 4>;

// Return a new matrix where the bottom two rows are all zeros,
// and the top two rows are copied from the input.
Matrix4i zeroBottomRows(const Matrix4i& input) {
    Matrix4i result = input;  // Copy the whole matrix.
    for (std::size_t row = 2; row < 4; ++row) {
        for (std::size_t col = 0; col < 4; ++col) {
            result[row][col] = 0;
        }
    }
    return result;
}
#include <cassert>

int main() {
    // Construct a test matrix with distinct values.
    Matrix4i a = {{
        {{1, 2, 3, 4}},
        {{5, 6, 7, 8}},
        {{9, 10, 11, 12}},
        {{13, 14, 15, 16}}
    }};

    Matrix4i expected = {{
        {{1, 2, 3, 4}},
        {{5, 6, 7, 8}},
        {{0, 0, 0, 0}},
        {{0, 0, 0, 0}}
    }};
    assert(zeroBottomRows(a) == expected);

    // Test with negative and zero values in the top rows.
    Matrix4i b = {{
        {{-1, 0, 3, -4}},
        {{5, -6, 7, 8}},
        {{9, 10, 11, 12}},
        {{13, 14, 15, 16}}
    }};
    Matrix4i expected_b = {{
        {{-1, 0, 3, -4}},
        {{5, -6, 7, 8}},
        {{0, 0, 0, 0}},
        {{0, 0, 0, 0}}
    }};
    assert(zeroBottomRows(b) == expected_b);

    // Test that the original matrix is not modified.
    Matrix4i c = a;
    zeroBottomRows(c);
    assert(c == a);  // Original remains unchanged.

    // Test with all zeros already in the bottom rows.
    Matrix4i d = {{
        {{1, 2, 3, 4}},
        {{5, 6, 7, 8}},
        {{0, 0, 0, 0}},
        {{0, 0, 0, 0}}
    }};
    assert(zeroBottomRows(d) == d);

    // Test with all zeros everywhere.
    Matrix4i e = {{
        {{0, 0, 0, 0}},
        {{0, 0, 0, 0}},
        {{0, 0, 0, 0}},
        {{0, 0, 0, 0}}
    }};
    assert(zeroBottomRows(e) == e);

    return 0;
}
