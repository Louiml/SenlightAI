Write a C++ function `bool canKeepAllOnes(const std::vector<std::string>& grid)` that takes a rectangular binary grid (each character is `'0'` or `'1'`) and returns `true` if there is no `2x2` sub-square in the grid that contains exactly three `'1'` cells and one `'0'` cell, and `false` otherwise. The grid may have any positive number of rows and columns, and you must handle edge cases where the grid has only one row or one column (in which case no `2x2` sub-square exists, so the answer is always `true`). The function must be `const`-correct and not modify the input.
#include <cassert>
#include <string>
#include <vector>

// (Include the solution function here)

int main() {
    // Basic valid cases
    assert(canKeepAllOnes({"01", "10"}) == true);      // sum=2
    assert(canKeepAllOnes({"11", "11"}) == true);      // sum=4
    assert(canKeepAllOnes({"00", "00"}) == true);      // sum=0

    // Single row or column
    assert(canKeepAllOnes({"1"}) == true);
    assert(canKeepAllOnes({"0", "1"}) == true);
    assert(canKeepAllOnes({"101", "010"}) == true);    // n=2,m=3 -> each 2x2 sum=2

    // Exactly three ones in a 2x2 block -> fail
    assert(canKeepAllOnes({"100", "110", "000"}) == false);  // top-left 2x2 has ones at (0,0),(1,0),(1,1) sum=3
    assert(canKeepAllOnes({"101", "111", "001"}) == false);  // multiple blocks, one fails

    // Larger valid grid
    assert(canKeepAllOnes({"1110", "1101", "1011", "0111"}) == false); // has a block with sum=3

    // Larger truly valid grid (no sum=3 anywhere)
    assert(canKeepAllOnes({"1110", "1100", "1001", "0011"}) == true);

    // Edge case with all zeros and one one in a corner, 3x3
    assert(canKeepAllOnes({"100", "000", "000"}) == true);
    assert(canKeepAllOnes({"100", "100", "000"}) == true); // vertical pair, no 2x2 with 3 ones

    return 0;
}
#include <string>
#include <vector>

// Returns true iff no 2x2 sub-square contains exactly three '1' cells.
bool canKeepAllOnes(const std::vector<std::string>& grid) {
    const int n = static_cast<int>(grid.size());
    if (n < 2) return true;
    const int m = static_cast<int>(grid[0].size());
    if (m < 2) return true;

    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < m - 1; ++j) {
            int sum = (grid[i][j] - '0') +
                      (grid[i + 1][j] - '0') +
                      (grid[i][j + 1] - '0') +
                      (grid[i + 1][j + 1] - '0');
            if (sum == 3) {
                return false;
            }
        }
    }
    return true;
}
// The key observation is that a `2x2` block with exactly three `'1'` cells (and hence one `'0'`) creates an impossible pattern in the context of this problem (which is typically derived from a "2D prefix OR" or "uniqueness" problem). The solution iterates through all possible top-left corners of `2x2` sub-squares (i.e., rows `0` to `n-2`, columns `0` to `m-2`). For each, it sums the four character values (converted from `'0'`/`'1'` to integer `0`/`1`). If any such sum equals `3`, we immediately return `false`. If no such block is found, return `true`. Edge cases: if `n < 2` or `m < 2`, the loop simply doesn't execute and the function returns `true`. Time complexity is `O(n*m)` because each cell is examined in at most four `2x2` blocks, but the loop visits `(n-1)(m-1)` blocks and each does constant work, so overall `O(n*m)`. Space complexity is `O(1)` auxiliary (not counting the input grid).
