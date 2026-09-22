// Write a C++ function that accepts a 4x4 integer matrix (passed as a 2D array or `std::array`/`std::vector`) and returns the sum of all elements located on or below the main diagonal. The main diagonal runs from the top-left to the bottom-right. For a 4x4 matrix with rows and columns indexed from 0 to 3, the entries to include are those where the row index is greater than or equal to the column index (i.e., `i >= j`). The function should handle matrices with negative numbers, zeros, and any integer values. The matrix size is fixed at 4x4, so no dynamic resizing is needed. Your solution must be a standalone function that can be called directly, and you should include appropriate `const` correctness.

// The solution iterates through every cell of the 4x4 matrix. For each cell at row `i` and column `j`, we check if `i >= j` (which includes the main diagonal and everything below it). If true, we add the value to a running total. Alternatively, we can loop over rows and for each row only iterate columns from 0 to `i` inclusive, which directly targets the required triangular region without needing an `if` condition. Both approaches are equivalent; the second is slightly more efficient because it avoids checking all 16 cells, but the difference is negligible for a fixed 4x4 size. Edge cases: all zeros yields a sum of 0; negative numbers are handled naturally by integer addition; the main diagonal is always included. Time complexity is O(16) = O(1) since the matrix size is constant; space complexity is O(1) excluding input storage.

#include <array>

// Compute the sum of elements on or below the main diagonal of a 4x4 matrix.
int lowerTriangularSum(const std::array<std::array<int, 4>, 4>& matrix) {
    int total = 0;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col <= row; ++col) {
            total += matrix[row][col];
        }
    }
    return total;
}

#include <cassert>
#include <array>

// Assume the solution function is declared above.

int main() {
    // Test 1: Simple positive numbers
    std::array<std::array<int, 4>, 4> m1 = {{
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    }};
    // Lower triangle sum: 1 + 5+6 + 9+10+11 + 13+14+15+16 = 100
    assert(lowerTriangularSum(m1) == 100);

    // Test 2: All zeros
    std::array<std::array<int, 4>, 4> m2 = {{
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }};
    assert(lowerTriangularSum(m2) == 0);

    // Test 3: Negative values
    std::array<std::array<int, 4>, 4> m3 = {{
        {-1, 2, 3, 4},
        {-5, -6, 7, 8},
        {9, 10, -11, 12},
        {13, -14, 15, 16}
    }};
    // Sum: -1 + (-5-6) + (9+10-11) + (13-14+15+16) = -1 -11 +8 +30 = 26
    assert(lowerTriangularSum(m3) == 26);

    // Test 4: Only the diagonal has values
    std::array<std::array<int, 4>, 4> m4 = {{
        {7, 0, 0, 0},
        {0, 8, 0, 0},
        {0, 0, 9, 0},
        {0, 0, 0, 10}
    }};
    assert(lowerTriangularSum(m4) == 34);

    // Test 5: Mixed large numbers
    std::array<std::array<int, 4>, 4> m5 = {{
        {100, -200, 300, -400},
        {500, -600, 700, -800},
        {900, 1000, -1100, 1200},
        {-1300, 1400, 1500, 1600}
    }};
    // Sum: 100 + (500-600) + (900+1000-1100) + (-1300+1400+1500+1600)
    // = 100 -100 +800 +3200 = 4000
    assert(lowerTriangularSum(m5) == 4000);

    return 0;
}
