Given a grid of size \(m \times n\) where each cell is either `'.'` (empty) or `'#'` (occupied), write a C++ function `std::string checkPath(const std::vector<std::string>& grid)` that starts at the row containing `'#'` in the first column (exactly one such row is guaranteed). From that starting position, you may move from column `i` to column `i+1` only if the destination cell at row either `now-1`, `now`, or `now+1` (within bounds) in the next column contains `'#'`. If you can reach the last column (column \(n-1\)) by a sequence of such valid moves, return the string `"KENA"`; otherwise, return `"TIDAK KENA"`. The input grid will have at least one `'#'` in the first column, and all indices are 0-based. Handle the case where the grid has only one column (i.e., \(n=1\))—in that case, you are already at the last column, so the answer is always `"KENA"` because the start is valid. Use standard C++ vectors and strings, and do not modify the input grid.
// The problem is a simple path simulation on a small neighbor window. We first locate the starting row `now` by scanning the first column for the `'#'` character. Then, for each column index from `0` to `n-2`, we check whether any of the three candidate rows (`now-1`, `now`, `now+1`) is within `[0, m-1]` and contains `'#'` in the next column. We choose the first valid candidate in the order `now-1`, `now`, `now+1` and update `now` to that row. If no candidate is valid, we return `"TIDAK KENA"`. If we successfully process all transitions, we return `"KENA"`. Edge cases include: single-column grids (immediate success), starting row at the top or bottom boundary (candidate rows out of range must be skipped), and grids where the path is blocked. The algorithm runs in \(O(m \cdot n)\) worst-case time if we consider scanning the first column separately, but the main loop is \(O(n)\) since each iteration checks at most 3 cells; space complexity is \(O(1)\) additional beyond the input. The guarantee that exactly one `'#'` exists in the first column simplifies initialization.
#include <string>
#include <vector>

// Determines whether a path exists from the first column to the last column
// following the movement rule: from row 'now' at column i, can move to row
// 'now-1', 'now', or 'now+1' at column i+1 if that cell is '#'.
// Returns "KENA" if reachable, otherwise "TIDAK KENA".
std::string checkPath(const std::vector<std::string>& grid) {
    if (grid.empty()) {
        return "TIDAK KENA";
    }

    const int m = static_cast<int>(grid.size());      // number of rows
    const int n = static_cast<int>(grid[0].size());   // number of columns

    // Find the starting row (the row with '#' in the first column)
    int now = -1;
    for (int i = 0; i < m; ++i) {
        if (grid[i][0] == '#') {
            now = i;
            break;
        }
    }
    // By problem guarantee, now is never -1
    if (now < 0) {
        return "TIDAK KENA";
    }

    // If only one column, already at the end
    if (n == 1) {
        return "KENA";
    }

    // Simulate moving column by column
    for (int col = 0; col < n - 1; ++col) {
        bool found = false;
        for (int d = -1; d <= 1; ++d) {
            int row = now + d;
            if (row >= 0 && row < m && grid[row][col + 1] == '#') {
                now = row;
                found = true;
                break;
            }
        }
        if (!found) {
            return "TIDAK KENA";
        }
    }
    return "KENA";
}
#include <cassert>
#include <string>
#include <vector>

// Declare the function (it is defined elsewhere; here we prototype it)
std::string checkPath(const std::vector<std::string>& grid);

int main() {
    // Basic successful path
    assert(checkPath({".#.", "##.", "..."}) == "KENA");
    // Basic blocked path
    assert(checkPath({"#..", ".#.", "..#"}) == "TIDAK KENA");
    // Single column
    assert(checkPath({"#", ".", "#"}) == "KENA");
    // Path requiring downward move at top row
    assert(checkPath({"#..", ".#.", "..#"}) == "KENA");
    // Path requiring upward move at bottom row
    assert(checkPath({"..#", ".#.", "#.."}) == "KENA");
    // Direct horizontal path
    assert(checkPath({".##.", "##.#", "....", ".#.#"}) == "TIDAK KENA");
    // Long path with zigzag
    assert(checkPath({"#...", ".##.", "..#.", "...#"}) == "KENA");
    // Path blocked after multiple moves
    assert(checkPath({".#.", ".#.", "...", ".#."}) == "TIDAK KENA");
    // Wide grid, one step per column
    assert(checkPath({"#....", ".#...", "..#..", "...#.", "....#"}) == "KENA");
    // All rows empty except first column start
    assert(checkPath({"#", ".", ".", "."}) == "KENA");
    return 0;
}
