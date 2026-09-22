// Write a C++ function `std::vector<std::string> colorBoard(int n, int m, const std::vector<std::string>& grid)` that, given a grid with `n` rows and `m` columns, where each cell is either `'R'`, `'W'`, or `'.'`, determines whether it is possible to assign colors (`'R'` for red, `'W'` for white) to all cells (including those originally `'.'`) such that every adjacent pair of cells (sharing an edge) has different colors, and the colors of the existing `'R'` and `'W'` cells remain unchanged. The function should return a vector of strings representing a valid fully-colored board, or an empty vector if no valid coloring exists. All rows have exactly `m` characters.

The problem is a classical bipartite coloring on a grid graph. Since a grid is bipartite, a valid coloring exists if and only if no conflict arises among the pre‑colored cells. First, assign each cell `(i,j)` (0‑based) a parity `parity = (i+j) % 2`. For a valid checkerboard pattern, all cells with the same parity must share the same base color (say `'R'` for parity 0), and the opposite parity gets the opposite color (`'W'`). For each pre‑colored cell, compute its required base color: if the cell is `'R'` and its parity is 0, then base must be red; if `'R'` and parity 1, base must be white; similarly for `'W'` (parity 0 → white, parity 1 → red). If any two conflicting base colors appear, it’s impossible. Otherwise, pick the unique base color if no pre‑colored cells exist (we can arbitrarily choose red). Then fill the board: for parity 0, use base color; for parity 1, use the opposite color. Edge cases: empty grid (n=0) returns empty (though not expected), single cell with a pre‑colored color is always valid, and all‑`.` grids can be colored in two ways (we choose one). Time complexity is O(n·m) because we scan each cell once, and space complexity is O(n·m) for the output, plus O(1) auxiliary (besides input storage).

#include <string>
#include <vector>
#include <algorithm>

// Returns a fully colored board consistent with the given pre-colored cells,
// or an empty vector if no valid checkerboard coloring exists.
std::vector<std::string> colorBoard(int n, int m, const std::vector<std::string>& grid) {
    // No possible base color yet; 0 = red base, 1 = white base, 2 = none assigned
    int baseColor = 2;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            char cell = grid[i][j];
            if (cell == '.') continue;
            int parity = (i + j) % 2;
            int requiredBase;
            if (cell == 'R') {
                // For parity 0, need base red; for parity 1, base white
                requiredBase = parity; // 0 → red, 1 → white
            } else { // cell == 'W'
                requiredBase = 1 - parity; // parity 0 → white, 1 → red
            }
            if (baseColor == 2) {
                baseColor = requiredBase;
            } else if (baseColor != requiredBase) {
                return {};
            }
        }
    }

    // If no pre-colored cells, choose arbitrary base (red).
    if (baseColor == 2) baseColor = 0;

    std::vector<std::string> result(n, std::string(m, '.'));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int parity = (i + j) % 2;
            int colorIndex = (parity ^ baseColor); // xor: parity flips base
            result[i][j] = (colorIndex == 0) ? 'R' : 'W';
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be declared above.
// Free function colorBoard is provided in the solution section.

int main() {
    // Simple empty grid (n=0) returns empty
    assert(colorBoard(0, 0, {}) == std::vector<std::string>{});

    // Single pre-colored cell
    {
        auto res = colorBoard(1, 1, {"R"});
        assert(res.size() == 1 && res[0] == "R");
    }

    // All '.' board: returns valid checkerboard (base red chosen)
    {
        auto res = colorBoard(2, 2, {"..", ".."});
        assert(res.size() == 2);
        assert((res[0] == "RW" && res[1] == "WR") || (res[0] == "WR" && res[1] == "RW"));
    }

    // Valid pre-colored board 2x2
    {
        auto res = colorBoard(2, 2, {"R.", ".W"});
        // Expected: R W / W R (since R at (0,0) parity 0 → base red)
        assert(res.size() == 2);
        assert(res[0] == "RW" && res[1] == "WR");
    }

    // Conflict: two same parity cells with different colors
    {
        auto res = colorBoard(2, 2, {"R.", "W."});
        // (0,0) and (1,0) both parity 0, but require different bases → empty
        assert(res.empty());
    }

    // Conflict on parity 1: (0,1) W and (1,0) R (both parity 1)
    {
        auto res = colorBoard(2, 2, {".W", "R."});
        assert(res.empty());
    }

    // Larger valid board 3x3 with some constraints
    {
        std::vector<std::string> grid = {"R..", "...", "..W"};
        auto res = colorBoard(3, 3, grid);
        // (0,0) parity 0 → base red; (2,2) parity 0 (since 2+2=4) → should be red. It is 'W' → conflict!
        // Actually check: (2,2) 'W' requires base white (parity 0 → white). Conflict with base red.
        assert(res.empty());
    }

    // Another valid 3x3: pre-colored diagonal R/W pattern
    {
        std::vector<std::string> grid = {"R..", ".W.", "..R"};
        auto res = colorBoard(3, 3, grid);
        // (0,0) R parity0 → base red; (1,1) W parity0 (1+1=2 even) → base white → conflict? No: (1,1) is parity 0, W needs base white, R needs base red → conflict → empty
        assert(res.empty());
    }

    // Single row with alternating constraints
    {
        auto res = colorBoard(1, 3, {"R.W"});
        // (0,0) R parity0 → base red; (0,2) W parity0 → base white → conflict → empty
        assert(res.empty());
    }

    // Single row valid
    {
        auto res = colorBoard(1, 3, {"R.W"});
        // Actually conflict as above, so let's test a valid one:
        std::vector<std::string> validRow = {"R.W"}; // conflict, so we replace with a valid one
        // Better: {"R.W"} is conflict, so use {"R.."} → base red, then output "RWR"
        auto res2 = colorBoard(1, 3, {"R.."});
        assert(res2.size() == 1 && res2[0] == "RWR");
    }

    return 0;
}
