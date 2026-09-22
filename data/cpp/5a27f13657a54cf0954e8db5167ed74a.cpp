// Write a C++ function named `findExtremeCoordinates` that, given a fixed-size 2D array represented as a `std::array<std::array<float, 4>, 3>` (a 3x4 matrix), returns a `struct` containing the global minimum value (as a `float`), its zero-based row and column indices (as `std::size_t`), and the global maximum value (as a `float`), with its zero-based row and column indices. The function must handle the case where the minimum and maximum values are at the same position (e.g., a matrix with one element), and must break ties by choosing the earliest position in row-major order. The function should be `const`-correct, taking the matrix by `const&`, and should not modify the input.
// The solution requires a single pass over all 12 elements of the 3x4 matrix. Initialize the minimum and maximum values using the first element at position (0,0), and also record their initial coordinates as (0,0). Then iterate through each row and each column. For each element, compare it to the current minimum: if it is strictly less than the current minimum, update the minimum value and its coordinates. Similarly, if it is strictly greater than the current maximum, update the maximum value and its coordinates. Because we use strict comparisons, ties (equal values) will not overwrite the earlier position, preserving row‑major earliest positions for both min and max. Edge case: if the matrix has all equal elements, the minimum and maximum will both be at (0,0), which is correct. Time complexity is O(rows × columns) = O(12) constant, and space complexity is O(1) as only a few variables are used.
#include <array>
#include <cstddef>

// Result struct holding min/max values and their 0‑based positions.
struct ExtremeCoordinates {
    float minValue;
    std::size_t minRow;
    std::size_t minCol;
    float maxValue;
    std::size_t maxRow;
    std::size_t maxCol;
};

// Find global min and max in a fixed 3x4 float matrix, returning coordinates.
// Ties are broken by the earliest position in row‑major order.
ExtremeCoordinates findExtremeCoordinates(const std::array<std::array<float, 4>, 3>& matrix) {
    // Initialize with the first element and its position.
    ExtremeCoordinates result;
    result.minValue = matrix[0][0];
    result.minRow = 0;
    result.minCol = 0;
    result.maxValue = matrix[0][0];
    result.maxRow = 0;
    result.maxCol = 0;

    // Single pass over all rows and columns.
    for (std::size_t row = 0; row < 3; ++row) {
        for (std::size_t col = 0; col < 4; ++col) {
            float current = matrix[row][col];
            // Strict < ensures the earliest position is kept for ties.
            if (current < result.minValue) {
                result.minValue = current;
                result.minRow = row;
                result.minCol = col;
            }
            // Strict > ensures the earliest position is kept for ties.
            if (current > result.maxValue) {
                result.maxValue = current;
                result.maxRow = row;
                result.maxCol = col;
            }
        }
    }

    return result;
}
#include <cassert>
#include <array>

int main() {
    // Case 1: typical matrix with distinct values
    std::array<std::array<float, 4>, 3> m1 = {{
        {{1.0f, 2.0f, 3.0f, 4.0f}},
        {{5.0f, 6.0f, 7.0f, 8.0f}},
        {{9.0f, 10.0f, 11.0f, -1.0f}}
    }};
    ExtremeCoordinates r1 = findExtremeCoordinates(m1);
    assert(r1.minValue == -1.0f && r1.minRow == 2 && r1.minCol == 3);
    assert(r1.maxValue == 11.0f && r1.maxRow == 2 && r1.maxCol == 2);

    // Case 2: all elements equal → both min and max at (0,0)
    std::array<std::array<float, 4>, 3> m2 = {{
        {{7.0f, 7.0f, 7.0f, 7.0f}},
        {{7.0f, 7.0f, 7.0f, 7.0f}},
        {{7.0f, 7.0f, 7.0f, 7.0f}}
    }};
    ExtremeCoordinates r2 = findExtremeCoordinates(m2);
    assert(r2.minValue == 7.0f && r2.minRow == 0 && r2.minCol == 0);
    assert(r2.maxValue == 7.0f && r2.maxRow == 0 && r2.maxCol == 0);

    // Case 3: tie for min at (1,0) and (2,2) → choose (1,0)
    std::array<std::array<float, 4>, 3> m3 = {{
        {{5.0f, 5.0f, 5.0f, 5.0f}},
        {{-3.0f, 5.0f, 5.0f, 5.0f}},
        {{5.0f, 5.0f, -3.0f, 5.0f}}
    }};
    ExtremeCoordinates r3 = findExtremeCoordinates(m3);
    assert(r3.minValue == -3.0f && r3.minRow == 1 && r3.minCol == 0);
    assert(r3.maxValue == 5.0f && r3.maxRow == 0 && r3.maxCol == 0);

    // Case 4: tie for max at (0,3) and (2,0) → choose (0,3)
    std::array<std::array<float, 4>, 3> m4 = {{
        {{1.0f, 2.0f, 3.0f, 9.0f}},
        {{4.0f, -5.0f, 6.0f, 7.0f}},
        {{9.0f, 8.0f, 7.0f, 6.0f}}
    }};
    ExtremeCoordinates r4 = findExtremeCoordinates(m4);
    assert(r4.maxValue == 9.0f && r4.maxRow == 0 && r4.maxCol == 3);
    assert(r4.minValue == -5.0f && r4.minRow == 1 && r4.minCol == 1);

    // Case 5: all negative values
    std::array<std::array<float, 4>, 3> m5 = {{
        {{-1.0f, -2.0f, -3.0f, -4.0f}},
        {{-5.0f, -6.0f, -7.0f, -8.0f}},
        {{-9.0f, -10.0f, -11.0f, -12.0f}}
    }};
    ExtremeCoordinates r5 = findExtremeCoordinates(m5);
    assert(r5.minValue == -12.0f && r5.minRow == 2 && r5.minCol == 3);
    assert(r5.maxValue == -1.0f && r5.maxRow == 0 && r5.maxCol == 0);

    return 0;
}
