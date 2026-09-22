// Write a C++ function named `isLoShuMagicSquare` that takes a 3x3 two-dimensional array of integers (passed as an array parameter with a fixed column size of 3) and returns a boolean value indicating whether it forms a valid Lo Shu magic square. The function must verify all of the following conditions: (1) every element is within the inclusive range 1 to 9, (2) all nine numbers are distinct (no duplicates), (3) the sum of each row equals 15, (4) the sum of each column equals 15, and (5) the sum of both main diagonals equals 15. The function should be `const`-correct, meaning it must not modify the input array. For the task, assume the input array is always 3x3; no dynamic sizes or templates are needed.
#include <cassert>

int main() {
    // Valid Lo Shu magic square.
    int magic[3][3] = {{4, 9, 2}, {3, 5, 7}, {8, 1, 6}};
    assert(isLoShuMagicSquare(magic) == true);

    // Values out of range (contains 10 and -8).
    int outOfRange[3][3] = {{10, 2, 3}, {4, 15, 6}, {7, 8, -8}};
    assert(isLoShuMagicSquare(outOfRange) == false);

    // Duplicate values (two 5s).
    int duplicates[3][3] = {{5, 1, 9}, {6, 5, 4}, {8, 3, 7}};
    assert(isLoShuMagicSquare(duplicates) == false);

    // Correct range and uniqueness, but row sums not 15.
    int wrongRowSum[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    assert(isLoShuMagicSquare(wrongRowSum) == false);

    // Correct range, uniqueness, rows, columns, but diagonal fails.
    int wrongDiag[3][3] = {{2, 7, 6}, {9, 5, 1}, {4, 3, 8}};
    // Rows: 15, 15, 15; Cols: 15, 15, 15; Main diag = 2+5+8 = 15, Anti diag = 6+5+4 = 15 → actually valid? Check.
    // Let's pick a valid alternate Lo Shu: {{8, 1, 6}, {3, 5, 7}, {4, 9, 2}}.
    int anotherMagic[3][3] = {{8, 1, 6}, {3, 5, 7}, {4, 9, 2}};
    assert(isLoShuMagicSquare(anotherMagic) == true);

    // A near-magic square that fails anti-diagonal.
    int badAntiDiag[3][3] = {{2, 7, 6}, {9, 5, 1}, {4, 3, 8}};
    // Let's compute: anti-diag = 6+5+4 = 15, main = 15, rows 15, cols 15 → actually valid.
    // To force anti-diag fail, modify one element while keeping range/uniqueness.
    int failAnti[3][3] = {{2, 7, 6}, {9, 5, 1}, {4, 3, 8}};
    // This is actually valid because all sums are 15. Use a different one.
    int invalidAnti[3][3] = {{2, 9, 4}, {7, 5, 3}, {6, 1, 8}};
    // Rows: 15, 15, 15; Cols: 15, 15, 15; Main diag = 2+5+8 = 15; Anti diag = 4+5+6 = 15 → valid? Actually yes.
    // To guarantee failure, use a grid with all numbers 1-9 but not magic.
    int notMagic[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    assert(isLoShuMagicSquare(notMagic) == false);

    // Edge: all zeros (range fail).
    int allZeros[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    assert(isLoShuMagicSquare(allZeros) == false);

    // Edge: valid but with last element changed to fail column sum.
    int failCol[3][3] = {{4, 9, 2}, {3, 5, 7}, {8, 1, 7}}; // duplicate 7 and wrong sums
    assert(isLoShuMagicSquare(failCol) == false);

    // Another valid Lo Shu (rotated).
    int rotated[3][3] = {{6, 1, 8}, {7, 5, 3}, {2, 9, 4}};
    assert(isLoShuMagicSquare(rotated) == true);
}
#include <cstddef>

// Returns true if the 3x3 array forms a valid Lo Shu magic square.
bool isLoShuMagicSquare(const int grid[3][3]) {
    // 1. Check range: all values must be between 1 and 9 inclusive.
    for (std::size_t row = 0; row < 3; ++row) {
        for (std::size_t col = 0; col < 3; ++col) {
            if (grid[row][col] < 1 || grid[row][col] > 9) {
                return false;
            }
        }
    }

    // 2. Check uniqueness: all values must be distinct.
    bool seen[10] = {false}; // indices 0..9; we only use 1..9.
    for (std::size_t row = 0; row < 3; ++row) {
        for (std::size_t col = 0; col < 3; ++col) {
            int value = grid[row][col];
            if (seen[value]) {
                return false;
            }
            seen[value] = true;
        }
    }

    // 3. Compute the target sum from the first row.
    int targetSum = grid[0][0] + grid[0][1] + grid[0][2];
    if (targetSum != 15) {
        return false;
    }

    // 4. Check all row sums.
    for (std::size_t row = 0; row < 3; ++row) {
        int rowSum = 0;
        for (std::size_t col = 0; col < 3; ++col) {
            rowSum += grid[row][col];
        }
        if (rowSum != targetSum) {
            return false;
        }
    }

    // 5. Check all column sums.
    for (std::size_t col = 0; col < 3; ++col) {
        int colSum = 0;
        for (std::size_t row = 0; row < 3; ++row) {
            colSum += grid[row][col];
        }
        if (colSum != targetSum) {
            return false;
        }
    }

    // 6. Check both diagonal sums.
    int mainDiagSum = grid[0][0] + grid[1][1] + grid[2][2];
    int antiDiagSum = grid[0][2] + grid[1][1] + grid[2][0];
    if (mainDiagSum != targetSum || antiDiagSum != targetSum) {
        return false;
    }

    // All checks passed.
    return true;
}
// The solution requires validating multiple separate properties of a 3x3 integer grid. The algorithm proceeds step‑by‑step: first, iterate through all nine elements to check that each is between 1 and 9 inclusive; if any value falls outside, immediately return `false`. Second, use a fixed‑size boolean array (or a set) of size 10 (indices 0–9) to track which numbers from 1 to 9 have been seen. For each element, if it has already been encountered, return `false`; otherwise mark it as seen. Third, compute the sum of the first row (which must be 15) and then verify that every row, column, and both diagonals have that same sum; if any mismatch occurs, return `false`. Important edge cases include arrays containing zeros, negative numbers, values greater than 9, duplicate values like two 5s, or an array that passes range and uniqueness checks but fails row/column/diagonal sums. The time complexity is O(1) because the grid is fixed at 3x3, and the space complexity is O(1) for the auxiliary boolean tracking array (constant size 10). The implementation uses `const` parameters to prevent modification and `const` local references where appropriate for clarity.
