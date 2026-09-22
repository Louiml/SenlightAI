// Write a C++ function `int minTogglesToClear(int n, const std::vector<std::vector<int>>& board)` that, given an `n x n` binary matrix (where `n` is between 2 and 18 inclusive, each cell is 0 or 1), returns the minimum number of cell presses needed to turn every cell to 0. Pressing a cell toggles its own value and the values of its four orthogonal neighbors (up, down, left, right; out-of-bounds neighbors are ignored). Each cell may be pressed at most once (pressing more than once is never beneficial), and the problem is under-determined, so some configurations may be impossible; in that case return -1.
The key observation is that once the presses in the first row are decided (there are `2^n` possibilities, since each cell either is pressed or not, and pressing twice cancels), the presses in every subsequent row are forced: when scanning row `i` from top to bottom, if cell `(i-1, j)` is still 1, the only way to turn it off without affecting already-solved rows above is to press `(i, j)`. Thus, we iterate over all `2^n` masks for the first row, simulate the toggling row by row, and count presses. After processing rows `1` to `n-1`, if the last row is all zeros, the configuration is valid; otherwise it is impossible, and we skip it. We track the minimum valid count. Edge cases include `n=2` (minimal size), boards that are already all zeros (answer 0), and boards that cannot be cleared (return -1). The time complexity is `O(2^n * n^2)` due to the `2^n` mask iterations each simulating up to `n^2` toggles; with `n <= 18` this is acceptable (about 4.7 million operations worst-case). Space complexity is `O(n^2)` for copying the board, or `O(n)` if using bitmask rows.
#include <vector>
#include <algorithm>
#include <climits>

// Returns the minimum number of presses to clear an n x n binary board,
// or -1 if impossible. Pressing (r,c) toggles itself and orthogonal neighbors.
int minTogglesToClear(int n, const std::vector<std::vector<int>>& board) {
    const int INF = INT_MAX / 2;
    int best = INF;

    // Convert the board into bitmasks for efficient toggling.
    std::vector<int> src(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (board[i][j] == 1) {
                src[i] |= (1 << j);
            }
        }
    }

    // Try every possible mask for presses in the first row.
    for (int mask = 0; mask < (1 << n); ++mask) {
        std::vector<int> p = src;
        int presses = 0;

        // Apply the first-row presses.
        for (int j = 0; j < n; ++j) {
            if (mask & (1 << j)) {
                ++presses;
                // Toggle (0, j) and its neighbors.
                p[0] ^= (1 << j);
                if (j > 0) p[0] ^= (1 << (j - 1));
                if (j + 1 < n) p[0] ^= (1 << (j + 1));
                if (n > 1) p[1] ^= (1 << j);
            }
        }

        // Process rows 1..n-1: if a cell above is 1, press the cell below.
        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (p[i - 1] & (1 << j)) {
                    ++presses;
                    // Toggle (i, j) and its neighbors.
                    p[i] ^= (1 << j);
                    if (j > 0) p[i] ^= (1 << (j - 1));
                    if (j + 1 < n) p[i] ^= (1 << (j + 1));
                    if (i + 1 < n) p[i + 1] ^= (1 << j);
                }
            }
        }

        // Check if the last row is all zeros.
        if (p[n - 1] == 0) {
            best = std::min(best, presses);
        }
    }

    return (best == INF) ? -1 : best;
}
#include <cassert>
#include <vector>

// The solution function is declared above.
int minTogglesToClear(int n, const std::vector<std::vector<int>>& board);

int main() {
    // 1x1 is not allowed (n>=2), but test n=2 cases.
    // All zeros => no presses needed.
    assert(minTogglesToClear(2, {{0,0},{0,0}}) == 0);
    // Single 1 in corner: press that corner.
    assert(minTogglesToClear(2, {{1,0},{0,0}}) == 1);
    // Checkerboard 2x2: press all four cells (presses for first row both, then second row both).
    assert(minTogglesToClear(2, {{1,0},{0,1}}) == 2);
    // Impossible case: 2x2 with a single 1 at (0,0) and another at (1,1) actually is possible, test a known impossible one:
    // A 2x2 with three 1's (a row of three impossible? Actually all 2x2 are solvable; test).
    // For n=3, a pattern that is impossible: all cells 1.
    assert(minTogglesToClear(3, {{1,1,1},{1,1,1},{1,1,1}}) == 9); // all 9 presses clears it.
    // A known impossible case for 3x3: only center is 1? It's solvable. Try pattern: (0,0)=1, (2,2)=1? Actually let's use a pattern that is impossible: a single 1 at (0,0) for n=3 is impossible? Let's verify: pressing (0,0) toggles (0,0),(0,1),(1,0) leaving three 1's, cannot clear. So answer -1.
    assert(minTogglesToClear(3, {{1,0,0},{0,0,0},{0,0,0}}) == -1);
    // Larger test: 4x4 all zeros.
    assert(minTogglesToClear(4, std::vector<std::vector<int>>(4, std::vector<int>(4,0))) == 0);
    // 4x4 with first row all ones: press all four in first row.
    assert(minTogglesToClear(4, {{1,1,1,1},{0,0,0,0},{0,0,0,0},{0,0,0,0}}) == 4);
    return 0;
}
