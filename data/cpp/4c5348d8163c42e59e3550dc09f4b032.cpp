/*
Write a C++ function `int maxPathLength(const std::vector<std::string>& grid)` that, given a rectangular grid of uppercase letters ('A'–'Z'), returns the length of the longest path starting from the top-left cell, moving only to adjacent cells (up, down, left, right), and never visiting a cell whose letter has already been used earlier in the same path. The starting cell itself counts toward the length. The grid will have at least one row and at least one column, and all rows will have the same length. The function must be efficient for grids up to 20×20.
*/

#include <vector>
#include <string>
#include <algorithm>

// Compute the longest path length starting from (row, col) in the grid.
// The path cannot revisit a letter that has already been encountered.
// Parameters:
//   grid   - constant reference to the grid of uppercase letters
//   row, col - current position
//   used   - boolean array of size 26 indicating which letters are used
// Returns the maximum number of cells reachable from (row, col) along a valid path,
// counting the current cell as part of the length.
int dfs(const std::vector<std::string>& grid, int row, int col, bool used[26]) {
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    int maxLen = 0;
    // directions: down, up, right, left
    const int dr[4] = {1, -1, 0, 0};
    const int dc[4] = {0, 0, 1, -1};
    for (int k = 0; k < 4; ++k) {
        int nr = row + dr[k];
        int nc = col + dc[k];
        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
            int idx = grid[nr][nc] - 'A';
            if (!used[idx]) {
                used[idx] = true;
                int candidate = dfs(grid, nr, nc, used);
                used[idx] = false;
                maxLen = std::max(maxLen, candidate);
            }
        }
    }
    return maxLen + 1; // count current cell
}

// Return the length of the longest path starting from the top-left cell.
int maxPathLength(const std::vector<std::string>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;
    bool used[26] = {false};
    used[grid[0][0] - 'A'] = true;
    return dfs(grid, 0, 0, used);
}

#include <cassert>
#include <vector>
#include <string>

// Solution function declaration (placed here for the test)
int maxPathLength(const std::vector<std::string>& grid);

int main() {
    // Single cell
    assert(maxPathLength({"A"}) == 1);
    // All same letters: cannot move at all
    assert(maxPathLength({"AA", "AA"}) == 1);
    // 2x2 with distinct letters: can visit all four cells (path A->B->C->D)
    assert(maxPathLength({"AB", "DC"}) == 4);
    // 2x2 with a repeated letter blocking full traversal
    // Grid:
    // A B
    // B C  -> starting at A, can visit A->B (top right), then cannot go to B (bottom left) again, but can go to C.
    // Longest path: A->B->C (length 3)
    assert(maxPathLength({"AB", "BC"}) == 3);
    // 3x3 with a Hamiltonian path of length 9 (all letters A..I)
    assert(maxPathLength({"ABC", "DEF", "GHI"}) == 9);
    // 2x3 with a repeated letter in the middle: 
    // A B C
    // D B E  -> possible path A->B->C->E? No, after B (top), cannot reuse B, so can go A->B->C->? Actually C has no new neighbors, then backtrack to B->E? But B used, so from B can go to E (unused), then E->C? C already used. So path A->B->E length 3. Alternative A->D? only A->D valid, D->B (B unused) ->B->C length 4? Wait check: start A, neighbors B and D. If go D, then D neighbors B (unused), B neighbors A (used), C (unused), E (unused). From B can go C or E. If go C, C neighbors B (used) and E (unused) -> but E is unused we can go D? No. Actually path A-D-B-C length 4 valid. So answer should be 4.
    assert(maxPathLength({"ABC", "DBE"}) == 4);
    // Larger grid with repeated letters limiting path length to maximum 26 letters
    std::vector<std::string> big(5, std::string(5, 'A'));
    // All 'A' -> only one cell usable
    assert(maxPathLength(big) == 1);
    // Mixed grid with 4 distinct letters in a cross
    // A B C
    // D E F
    // G H I  -> already tested length 9
    // A grid with 2x2 distinct but one letter repeated in center of 3x3:
    // A B C
    // D E F
    // G H A  -> starting A, cannot revisit A at bottom right. Longest path? Try A(0,0)-B-C-F-I? no I. Let's compute: actually the function will DFS, so we trust it. Let's just assert a known value: A B C / D E F / G H A, longest path length? from A, can go to B,D. Path A-B-C-F-E-D-G-H? H has neighbor bottom A used, so stops at H, length 8? Check: A(0,0), B(0,1), C(0,2), F(1,2), E(1,1), D(1,0), G(2,0), H(2,1) -> 8 cells, then H's neighbors E and A(used), so length 8. So assert == 8.
    assert(maxPathLength({"ABC", "DEF", "GHA"}) == 8);
    // Edge case: 1x3 with repeated letters
    assert(maxPathLength({"ABA"}) == 2); // A->B only
    // 1x2 with distinct letters
    assert(maxPathLength({"AB"}) == 2);
    return 0;
}

// The problem is a classic depth‑first search (DFS) with state tracking of which letters have been used along the current path. Start at cell (0,0), mark its letter as used, then recursively explore all four directions. For each neighboring cell inside bounds, if its letter has not been used, mark it, recurse, and unmark it upon backtracking to try alternatives. The base case is when no valid moves exist; then the path length is the number of cells visited so far, which is accumulated by returning `1 + max(recursive results)`. Since the alphabet has only 26 letters, the state space is small; the maximum path length is bounded by 26 (cannot use a letter twice). The time complexity is O(R×C×4^L) in the worst case, but with L ≤ 26 the branching quickly prunes; in practice for a 20×20 grid it runs fast. Auxiliary space is O(R×C) for the grid plus O(26) for the used-letter boolean array (plus recursion depth up to 26). Edge cases: a 1×1 grid returns 1; a grid where all cells have the same letter returns 1; grids with repeated letters must not reuse them. The solution uses `const` references and a small boolean array for the 26 uppercase letters.
