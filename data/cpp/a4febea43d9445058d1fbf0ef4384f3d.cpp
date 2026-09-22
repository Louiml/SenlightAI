Write a C++ function that takes an `n x m` grid of characters `'0'` and `'1'` representing initially lit cells, and returns a new grid of the same dimensions where every cell that shares a diagonal (either main diagonal direction, i.e., top-left to bottom-right, or anti-diagonal direction, i.e., top-right to bottom-left) with at least one initially lit cell is set to `'1'`; all other cells remain `'0'`. The function signature should be `std::vector<std::string> illuminateDiagonals(const std::vector<std::string>& grid)`. The grid is guaranteed to be non-empty, with dimensions `1 <= n, m <= 1000`, and each string has length exactly `m`. The output must preserve the original row order and each row as a string of `'0'` and `'1'`. Do not modify the input. For a cell, a diagonal match means there exists some initially lit cell `(r, c)` such that either `(i - j) == (r - c)` (same main diagonal) or `(i + j) == (r + c)` (same anti-diagonal). The original lit cells themselves will naturally be included in the output because they trivially share both diagonals with themselves.
The key is to avoid checking every pair of cells (which would be O(n²·m²)). Instead, precompute which diagonals are "used" by any initially lit cell. For each lit cell `(i, j)`, mark:
- The main diagonal index `i - j + (m - 1)` (adding offset to make it non-negative; possible range is from `-(m-1)` to `n-1`, so after offset it's `0` to `n+m-2`).
- The anti-diagonal index `i + j` (range `0` to `n+m-2`).

Both arrays can be of size `n + m` (since max index is `n+m-2`). Then, for each cell `(i, j)`, check if either its main diagonal index or its anti-diagonal index is marked; if so, set that cell to `'1'`, else to `'0'`. This runs in O(n·m) time and uses O(n + m) auxiliary space. Edge cases: when the grid has only one row or one column, diagonals still work correctly; empty input is not expected per constraints, but if it were, the function could return an empty vector. Also careful with integer offset calculation to avoid negative indices.
#include <vector>
#include <string>

// Given a grid of '0' and '1', returns a new grid where every cell that shares
// a main diagonal or an anti-diagonal with at least one initially lit cell is '1'.
std::vector<std::string> illuminateDiagonals(const std::vector<std::string>& grid) {
    if (grid.empty() || grid[0].empty()) {
        return {};
    }

    const int n = static_cast<int>(grid.size());
    const int m = static_cast<int>(grid[0].size());

    // Diagonal index arrays: main diagonal = i - j + (m - 1), anti = i + j
    const int diagSize = n + m; // max allowed index is n+m-2, so size n+m is safe
    std::vector<bool> mainDiagUsed(diagSize, false);
    std::vector<bool> antiDiagUsed(diagSize, false);

    // Mark diagonals from originally lit cells
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == '1') {
                mainDiagUsed[i - j + (m - 1)] = true;
                antiDiagUsed[i + j] = true;
            }
        }
    }

    // Build result grid
    std::vector<std::string> result(n, std::string(m, '0'));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (mainDiagUsed[i - j + (m - 1)] || antiDiagUsed[i + j]) {
                result[i][j] = '1';
            }
        }
    }

    return result;
}
#include <cassert>
#include <vector>
#include <string>

// Function under test is declared above; this is a self-contained test.
int main() {
    // Test 1: Simple 2x2 with one lit cell at (0,0) -> lights whole main diagonal and anti-diagonal
    std::vector<std::string> grid1 = {"10", "00"};
    std::vector<std::string> expected1 = {"11", "11"};
    assert(illuminateDiagonals(grid1) == expected1);

    // Test 2: 3x3 with a lit center -> lights all cells that share center diagonals
    std::vector<std::string> grid2 = {"000", "010", "000"};
    std::vector<std::string> expected2 = {"101", "111", "101"};
    assert(illuminateDiagonals(grid2) == expected2);

    // Test 3: All zeros -> output all zeros
    std::vector<std::string> grid3 = {"000", "000"};
    std::vector<std::string> expected3 = {"000", "000"};
    assert(illuminateDiagonals(grid3) == expected3);

    // Test 4: All ones -> every cell already lit, so output remains all ones
    std::vector<std::string> grid4 = {"111", "111"};
    std::vector<std::string> expected4 = {"111", "111"};
    assert(illuminateDiagonals(grid4) == expected4);

    // Test 5: Single row, multiple columns
    std::vector<std::string> grid5 = {"101"};
    std::vector<std::string> expected5 = {"111"}; // both '1's at ends cover all diagonals in a row
    assert(illuminateDiagonals(grid5) == expected5);

    // Test 6: Single column
    std::vector<std::string> grid6 = {"1", "0", "1"};
    std::vector<std::string> expected6 = {"1", "1", "1"};
    assert(illuminateDiagonals(grid6) == expected6);

    // Test 7: 1x1 with '0' -> remains '0'
    std::vector<std::string> grid7 = {"0"};
    std::vector<std::string> expected7 = {"0"};
    assert(illuminateDiagonals(grid7) == expected7);

    // Test 8: 1x1 with '1' -> remains '1'
    std::vector<std::string> grid8 = {"1"};
    std::vector<std::string> expected8 = {"1"};
    assert(illuminateDiagonals(grid8) == expected8);

    // Test 9: Non-square 3x4, lit at top-left and bottom-right
    std::vector<std::string> grid9 = {"1000", "0000", "0001"};
    // Main diagonal for (0,0): index (0-0+3)=3; for (2,3): (2-3+3)=2 -> these lit cells mark different mains.
    // Anti for (0,0): 0; for (2,3): 5. Let's compute expected systematically:
    // Cells that share main 2 or 3: main2 -> (i-j+3=2 => i-j=-1) cells: (0,1),(1,2),(2,3) ; main3 -> (i-j+3=3 => i-j=0) cells: (0,0),(1,1),(2,2)
    // Cells that share anti 0 or 5: anti0 -> (i+j=0) only (0,0); anti5 -> (i+j=5) only (2,3)
    // Union: (0,0),(0,1),(1,1),(1,2),(2,2),(2,3) => row0: 1100, row1: 0110, row2: 0001? Wait check (2,0): i+j=2, i-j=2 → main2? i-j=2 not in {2,3}? Actually main2 means i-j=-1, main3 i-j=0. (2,0): i-j=2 not in set. anti: i+j=2 not 0 or5. So 0. 
    // So expected: 
    // row0: i=0, j=0 lit, j=1 lit, j=2? main3? i-j= -2 not, anti=2 not -> 0, j=3? i-j=-3 not, anti=3 not -> 0 => "1100"
    // row1: i=1, j=0 main4? no, anti1? no ->0, j=1 main3 (i-j=0) yes, anti2? no ->1, j=2 main2 (i-j=-1) yes, anti3? no ->1, j=3 main1? no, anti4? no ->0 => "0110"
    // row2: i=2, j=0 main5? no, anti2? no ->0, j=1 main4? no, anti3? no ->0, j=2 main3 (i-j=0) yes, anti4? no ->1, j=3 main2 (i-j=-1) yes, anti5? yes ->1 => "0011"
    std::vector<std::string> expected9 = {"1100", "0110", "0011"};
    assert(illuminateDiagonals(grid9) == expected9);

    // Test 10: Empty grid edge (though constraints say non-empty, function should handle gracefully)
    std::vector<std::string> grid10 = {};
    std::vector<std::string> expected10 = {};
    assert(illuminateDiagonals(grid10) == expected10);

    return 0;
}
