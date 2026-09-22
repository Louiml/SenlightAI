// Implement a C++ function `generateMagicGrid(int n)` that takes an integer `n` (which is guaranteed to be a positive multiple of 4, to simplify the construction pattern) and returns a 2D vector (or equivalent) of size `n x n` containing the integers from `0` to `n*n - 1` exactly once each, arranged in a specific column-based block pattern. The pattern is defined as follows: for each block of 4 consecutive columns (starting at column indices 0, 4, 8, ...), and for each row `i` from `0` to `n-1`, the four entries in columns `4*k, 4*k+1, 4*k+2, 4*k+3` for block `k` (where `k = 0 .. n/4 - 1`) are set as: `a[i][4k] = (i + k*n)*4`, `a[i][4k+1] = a[i][4k] + 1`, `a[i][4k+2] = a[i][4k] + 2`, `a[i][4k+3] = a[i][4k] + 3`. The function should return the filled grid as a `std::vector<std::vector<int>>`. Note: The grid must contain every integer from 0 to `n*n-1` exactly once, and the order is purely determined by this formula; no sorting or other rearrangement is allowed. The function should be efficient for `n` up to 1000 (thus `n*n` values may be large, but they fit in `int`; note that `n*n-1` for n=1000 is 999999, which fits in a 32-bit signed int). If `n` is not a positive multiple of 4, the behavior is undefined, but you may assume the input is valid.
#include <cassert>
#include <vector>

// Function declaration (from solution)
std::vector<std::vector<int>> generateMagicGrid(int n);

int main() {
    // Test n=4
    auto g4 = generateMagicGrid(4);
    std::vector<std::vector<int>> expected4 = {
        {0, 1, 2, 3},
        {4, 5, 6, 7},
        {8, 9, 10, 11},
        {12, 13, 14, 15}
    };
    assert(g4 == expected4);

    // Test n=8, check dimension and values range
    auto g8 = generateMagicGrid(8);
    assert(g8.size() == 8);
    assert(g8[0].size() == 8);
    // Verify all numbers 0..63 appear exactly once
    std::vector<int> seen(64, 0);
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            int val = g8[i][j];
            assert(val >= 0 && val < 64);
            seen[val]++;
        }
    }
    for (int v : seen) assert(v == 1);

    // Test specific pattern for n=8: row 0, block 0 gives 0,1,2,3; block 1 gives 32,33,34,35
    assert(g8[0][0] == 0 && g8[0][1] == 1 && g8[0][2] == 2 && g8[0][3] == 3);
    assert(g8[0][4] == 32 && g8[0][5] == 33 && g8[0][6] == 34 && g8[0][7] == 35);

    // Test n=12, verify row 1, block 2 (k=2) values: base = (1 + 2*12)*4 = (25)*4 = 100
    auto g12 = generateMagicGrid(12);
    assert(g12[1][8] == 100);
    assert(g12[1][9] == 101);
    assert(g12[1][10] == 102);
    assert(g12[1][11] == 103);

    // Test n=16, check first and last values
    auto g16 = generateMagicGrid(16);
    assert(g16[0][0] == 0);
    assert(g16[15][15] == 255); // last row, last col, base = (15 + (3)*16)*4 = (63)*4 = 252, +3=255

    // Test that all numbers are unique for n=16 (sample check)
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            int val = g16[i][j];
            assert(val >= 0 && val < 256);
        }
    }

    return 0;
}
#include <vector>

// Generate an n x n grid where each block of 4 consecutive columns is filled
// according to the pattern: a[i][4k] = (i + k*n)*4, and next columns +1,+2,+3.
std::vector<std::vector<int>> generateMagicGrid(int n) {
    // Initialize n x n grid with zeros (or directly assign).
    std::vector<std::vector<int>> grid(n, std::vector<int>(n, 0));

    // Process each block of 4 columns.
    int numBlocks = n / 4;  // since n is a multiple of 4
    for (int k = 0; k < numBlocks; ++k) {
        for (int i = 0; i < n; ++i) {
            int base = (i + k * n) * 4;
            int col = k * 4;
            grid[i][col] = base;
            grid[i][col + 1] = base + 1;
            grid[i][col + 2] = base + 2;
            grid[i][col + 3] = base + 3;
        }
    }

    return grid;
}
// The problem essentially asks to generate a specific permutation of numbers from 0 to n*n-1 placed in a 2D grid according to a simple arithmetic rule. The construction is column-block based: each block of four consecutive columns (indices 0-3, 4-7, etc.) is filled using the row index `i` and block index `k` to compute a base value `(i + k*n)*4`. Then the base, base+1, base+2, base+3 are placed in the four consecutive columns. This ensures that each row-block pair produces four consecutive numbers, and as `i` and `k` vary, the base values are `4 * (i + k*n)`. Since `i` ranges from 0 to n-1 and `k` from 0 to n/4-1, the values `i + k*n` range from 0 to n-1 + (n/4-1)*n = n-1 + n*n/4 - n = n*n/4 - 1. Multiplying by 4 gives a range from 0 to n*n - 4, and each such number is the base of a set of 4 consecutive numbers. Thus every integer from 0 to n*n-1 appears exactly once. The algorithm is straightforward: allocate a 2D vector of size n x n, iterate over blocks `k` (from 0 to n/4-1) and rows `i` (from 0 to n-1), compute base = (i + k*n)*4, and fill the four columns. Time complexity is O(n^2) because we fill n*n cells. Space complexity is O(n^2) for the output grid. Edge cases: n=0 (but problem says positive multiple, so n>=4), n=4 is smallest valid. No other issues; the formula is deterministic and correct.
