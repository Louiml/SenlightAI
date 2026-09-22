Write a C++ function `bool hasPerfectPlus(int h, int w, const vector<string>& grid)` that determines whether a given `h x w` grid of `'*'` and `'.'` characters contains exactly one perfect plus sign. A perfect plus sign of any size consists of a central `'*'` with contiguous vertical and horizontal arms of `'*'` extending at least one cell in all four directions (up, down, left, right). The vertical arm is a straight column of `'*'` through the center, and the horizontal arm is a straight row of `'*'` through the center. The grid may contain other `'*'` cells outside this plus sign, but only one plus sign may exist. The function should return `true` if exactly one perfect plus sign exists, is fully contained within the grid, and every `'*'` in the grid is part of that plus sign; otherwise return `false`. The grid dimensions satisfy `1 ≤ h, w ≤ 1000`, and the grid contains only `'*'` and `'.'` characters.

The algorithm scans every cell that could be the center of a plus sign: a `'*'` with at least one `'*'` directly above, below, left, and right. For each such candidate center, we mark the entire vertical and horizontal arm as "covered" by extending from the center in all four directions while the adjacent cell is `'*'`. We must track whether more than one center candidate exists; if a second candidate is found, we immediately reject. After processing all cells, we verify that every `'*'` in the grid has been marked as covered. If exactly one center was found, no conflicts occurred, and all `'*'` are covered, we return `true`; otherwise `false`. Edge cases include grids with no plus sign, grids with multiple plus signs, grids with `'*'` cells not belonging to the plus sign, and grids where the arms extend to the border. Time complexity is O(h·w) because each cell is visited a constant number of times (once for scanning, and each arm extension traverses cells at most once). Space complexity is O(h·w) for two boolean matrices (original and covered) or we can mark covered in a separate vector of booleans. For large dimensions we should use `vector<vector<char>>` or `vector<string>` to avoid stack overflow, and avoid copying large grids.

#include <vector>
#include <string>

using namespace std;

// Determine if the grid contains exactly one perfect plus sign covering all '*' cells.
bool hasPerfectPlus(int h, int w, const vector<string>& grid) {
    // Mark which cells are part of the found plus sign.
    vector<vector<bool>> covered(h, vector<bool>(w, false));
    int plusCount = 0;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            // A potential center must have at least one '*' in all four directions.
            if (i == 0 || i == h - 1 || j == 0 || j == w - 1) continue;
            if (grid[i][j] == '*' &&
                grid[i - 1][j] == '*' &&
                grid[i + 1][j] == '*' &&
                grid[i][j - 1] == '*' &&
                grid[i][j + 1] == '*') {
                // Found a center; reject if we already have one.
                if (plusCount >= 1) return false;
                plusCount++;

                // Mark the center.
                covered[i][j] = true;

                // Extend upward.
                for (int r = i - 1; r >= 0 && grid[r][j] == '*'; --r) {
                    covered[r][j] = true;
                }
                // Extend downward.
                for (int r = i + 1; r < h && grid[r][j] == '*'; ++r) {
                    covered[r][j] = true;
                }
                // Extend left.
                for (int c = j - 1; c >= 0 && grid[i][c] == '*'; --c) {
                    covered[i][c] = true;
                }
                // Extend right.
                for (int c = j + 1; c < w && grid[i][c] == '*'; ++c) {
                    covered[i][c] = true;
                }
            }
        }
    }

    // If we didn't find exactly one center, reject.
    if (plusCount != 1) return false;

    // Every '*' in the grid must be covered by the plus sign.
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (grid[i][j] == '*' && !covered[i][j]) {
                return false;
            }
        }
    }

    return true;
}

#include <cassert>
#include <vector>
#include <string>

// Include the solution function here (or include the header if separated).
bool hasPerfectPlus(int h, int w, const vector<string>& grid);

int main() {
    // Single plus sign, all cells covered.
    vector<string> grid1 = {
        "....*....",
        "....*....",
        "*********",
        "....*....",
        "....*...."
    };
    assert(hasPerfectPlus(5, 9, grid1) == true);

    // No plus sign (no vertical arm).
    vector<string> grid2 = {
        ".*.",
        "***",
        ".*."
    };
    // Actually this is a plus sign; let's test a non-plus pattern.
    vector<string> grid3 = {
        ".*.",
        ".*.",
        "***"
    };
    assert(hasPerfectPlus(3, 3, grid3) == false);

    // Extra '*' not part of the plus.
    vector<string> grid4 = {
        ".*.*",
        "***.",
        ".*.."
    };
    assert(hasPerfectPlus(3, 4, grid4) == false);

    // Two plus signs.
    vector<string> grid5 = {
        ".*.*.",
        "*****",
        ".*.*.",
        "..*.."
    };
    assert(hasPerfectPlus(4, 5, grid5) == false);

    // Single cell plus (cross shape).
    vector<string> grid6 = {
        ".*.",
        "***",
        ".*."
    };
    assert(hasPerfectPlus(3, 3, grid6) == true);

    // Grid with no stars.
    vector<string> grid7 = {
        "...",
        "...",
        "..."
    };
    assert(hasPerfectPlus(3, 3, grid7) == false);

    // Plus at edge is invalid because center must have all four directions.
    vector<string> grid8 = {
        "****",
        "****",
        "****"
    };
    assert(hasPerfectPlus(3, 4, grid8) == false);

    // Vertical line and horizontal line crossing but arms not all extending.
    vector<string> grid9 = {
        ".*.",
        "***",
        ".*."
    };
    assert(hasPerfectPlus(3, 3, grid9) == true);

    // Single star only.
    vector<string> grid10 = {
        "*"
    };
    assert(hasPerfectPlus(1, 1, grid10) == false);

    return 0;
}
