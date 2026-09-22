// Write a C++ function `bool isConsistentGrid(const std::array<std::array<int, 3>, 3>& grid)` that determines whether a 3×3 grid of integers satisfies the condition: for every 2×2 sub-square (there are exactly four such sub-squares, formed by rows 1-2 or 2-3 and columns 1-2 or 2-3), the sum of the main diagonal equals the sum of the anti-diagonal. Equivalently, for each sub-square with top-left at (r, c), check that `grid[r][c] + grid[r+1][c+1] == grid[r][c+1] + grid[r+1][c]`. Return `true` if all four sub-squares satisfy this, otherwise `false`. The grid indices are 0-based (0..2). The input integers can be any valid C++ `int` values, including negatives and duplicates. The function must be `const`-correct and should not modify the input.
#include <array>
#include <cassert>

bool isConsistentGrid(const std::array<std::array<int, 3>, 3>& grid);

int main() {
    // All zeros: trivially consistent.
    std::array<std::array<int, 3>, 3> g1 = {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}};
    assert(isConsistentGrid(g1) == true);

    // Example from original snippet: c[1][1]=1, c[1][2]=2, c[1][3]=3,
    // c[2][1]=2, c[2][2]=4, c[2][3]=6, c[3][1]=3, c[3][2]=5, c[3][3]=9
    // This satisfies the original snippet's condition.
    std::array<std::array<int, 3>, 3> g2 = {{{1, 2, 3}, {2, 4, 6}, {3, 5, 9}}};
    assert(isConsistentGrid(g2) == true);

    // Single cell difference breaks one sub-square: change bottom-right to 8.
    std::array<std::array<int, 3>, 3> g3 = {{{1, 2, 3}, {2, 4, 6}, {3, 5, 8}}};
    assert(isConsistentGrid(g3) == false);

    // Negative values and duplicates.
    std::array<std::array<int, 3>, 3> g4 = {{{-1, -2, -3}, {2, 1, 0}, {-1, -2, -3}}};
    // Check manually: sub-square top-left (0,0): -1+1 = 0, -2+2 = 0 -> ok.
    // top-left (0,1): -2+0 = -2, -3+1 = -2 -> ok.
    // top-left (1,0): 2+(-3) = -1, 1+(-1) = 0 -> fails.
    assert(isConsistentGrid(g4) == false);

    // Larger values.
    std::array<std::array<int, 3>, 3> g5 = {{{100, 200, 300}, {200, 400, 600}, {300, 500, 900}}};
    assert(isConsistentGrid(g5) == true);

    // Only one sub-square broken at top-left.
    std::array<std::array<int, 3>, 3> g6 = {{{1, 2, 3}, {2, 5, 6}, {3, 5, 9}}};
    assert(isConsistentGrid(g6) == false);

    // All cells equal.
    std::array<std::array<int, 3>, 3> g7 = {{{7, 7, 7}, {7, 7, 7}, {7, 7, 7}}};
    assert(isConsistentGrid(g7) == true);

    // Mixed positives/negatives, consistent.
    std::array<std::array<int, 3>, 3> g8 = {{{0, 2, 4}, {2, 0, 2}, {4, 2, 0}}};
    // sub-square (0,0): 0+0 = 0, 2+2 = 4 -> fails.
    assert(isConsistentGrid(g8) == false);
}
#include <array>

// Checks if a 3x3 grid satisfies the condition for all four 2x2 sub-squares:
// for each 2x2 block, the sum of the main diagonal equals the sum of the anti-diagonal.
bool isConsistentGrid(const std::array<std::array<int, 3>, 3>& grid) {
    // Iterate over the top-left corners of all 2x2 sub-squares.
    for (int r = 0; r < 2; ++r) {
        for (int c = 0; c < 2; ++c) {
            const int mainDiagSum = grid[r][c] + grid[r + 1][c + 1];
            const int antiDiagSum = grid[r][c + 1] + grid[r + 1][c];
            if (mainDiagSum != antiDiagSum) {
                return false;
            }
        }
    }
    return true;
}
// The problem reduces to checking four independent 2×2 sub-squares within a 3×3 grid. A 2×2 sub-square with top-left at row `r` and column `c` (where `r` can be 0 or 1, and `c` can be 0 or 1) has entries `grid[r][c]`, `grid[r][c+1]`, `grid[r+1][c]`, and `grid[r+1][c+1]`. The condition for consistency is that the sum of the main diagonal (`grid[r][c] + grid[r+1][c+1]`) equals the sum of the anti-diagonal (`grid[r][c+1] + grid[r+1][c]`). We simply iterate over the four possible `(r,c)` pairs and check equality; if any fails, return `false`; otherwise return `true`. Edge cases: since the grid is always exactly 3×3, there are exactly four sub-squares, no more and no less. Negative values and duplicates are naturally handled by integer equality checks. Time complexity is O(1) because the grid size is constant (9 elements), and space complexity is O(1) as we only use a few local variables.
