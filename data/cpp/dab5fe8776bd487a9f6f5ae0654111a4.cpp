// Given a rectangular grid of characters where each cell is either an uppercase letter ('A'–'Z') or a question mark ('?'), write a C++ function `fillPartialGrid` that takes the grid as a vector of strings and returns a fully filled grid (no '?' remaining) such that for every uppercase letter that appears originally in the grid, the minimal axis-aligned rectangle that contains all original occurrences of that letter (so the bounding box) is completely filled with that same letter. The filling process may replace '?' cells, but must never change an existing letter to a different letter. If multiple valid fillings exist, any one can be returned. It is guaranteed that a valid solution exists. The function should modify the input grid in place and also return it.

// The problem is essentially to propagate each letter to fill its bounding box, and then recursively handle any newly created overlaps or conflicts. Since it's guaranteed valid, a simple greedy approach works: for each letter that appears in the original grid, compute its minimum row, maximum row, minimum column, and maximum column. Then iterate over all cells in that rectangle and set them to that letter. However, doing this once might not be sufficient because after filling a rectangle, a previously unknown '?' might become a letter that now has a larger bounding box (because new occurrences of that letter were introduced). So we need to repeat the process until no more changes occur. The algorithm: repeatedly scan all 26 letters, compute their current bounding boxes (based on current grid content), and fill those boxes. Continue until a pass makes no changes. Since each pass either fills at least one '?' or stops, and the grid is finite, it terminates. Time complexity: each pass is O(rows * cols * 26) and the number of passes is at most rows*cols (each pass fills at least one cell), so worst case O((rows*cols)^2 * 26). For typical small grids (like in the original problem, rows and cols up to maybe 25), this is fine. Space complexity O(1) extra aside from input modification.
//
// Important edge cases: a letter may not appear at all (ignore it), a letter may appear only once (its bounding box is a single cell, no change), or a letter's bounding box may already be fully filled with that letter (no change). Also, after filling, a '?' could become a letter that then triggers further expansion. The validity guarantee ensures we never encounter a conflict where two different letters both claim the same cell; if that happens, the algorithm would overwrite and produce an invalid grid, but the problem guarantees no such input.

#include <vector>
#include <string>
#include <algorithm>

// Fills '?' cells in the grid so that each letter's bounding box is filled.
// Modifies the input grid in place and returns it.
std::vector<std::string>& fillPartialGrid(std::vector<std::string>& grid) {
    if (grid.empty() || grid[0].empty()) return grid;

    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    bool changed = true;
    while (changed) {
        changed = false;
        for (char ch = 'A'; ch <= 'Z'; ++ch) {
            int minR = rows, maxR = -1, minC = cols, maxC = -1;
            // Compute current bounding box of this letter
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < cols; ++c) {
                    if (grid[r][c] == ch) {
                        minR = std::min(minR, r);
                        maxR = std::max(maxR, r);
                        minC = std::min(minC, c);
                        maxC = std::max(maxC, c);
                    }
                }
            }
            if (minR > maxR) continue; // letter not present
            // Fill the bounding box
            for (int r = minR; r <= maxR; ++r) {
                for (int c = minC; c <= maxC; ++c) {
                    if (grid[r][c] != ch) {
                        grid[r][c] = ch;
                        changed = true;
                    }
                }
            }
        }
    }
    return grid;
}

#include <cassert>
#include <vector>
#include <string>

// Include the solution function declaration here (in real test, include the header)
std::vector<std::string>& fillPartialGrid(std::vector<std::string>& grid);

int main() {
    // Test 1: Simple single letter with one '?' inside its bounding box
    {
        std::vector<std::string> grid = {"A?A", "???", "A?A"};
        auto& result = fillPartialGrid(grid);
        assert(result.size() == 3);
        for (auto& row : result) {
            assert(row == "AAA");
        }
    }

    // Test 2: Multiple letters, no overlap conflict
    {
        std::vector<std::string> grid = {"A?B", "???", "C?C"};
        auto& result = fillPartialGrid(grid);
        assert(result[0] == "AAB");
        assert(result[1] == "AAB");
        assert(result[2] == "CCC");
    }

    // Test 3: Already fully filled grid, no '?' 
    {
        std::vector<std::string> grid = {"ABC", "DEF", "GHI"};
        auto& result = fillPartialGrid(grid);
        assert(result == std::vector<std::string>({"ABC", "DEF", "GHI"}));
    }

    // Test 4: Single cell grid
    {
        std::vector<std::string> grid = {"A"};
        auto& result = fillPartialGrid(grid);
        assert(result[0] == "A");
    }

    // Test 5: Letter appears at corners, filling the whole rectangle
    {
        std::vector<std::string> grid = {"X??X", "????", "X??X"};
        auto& result = fillPartialGrid(grid);
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 4; ++c)
                assert(result[r][c] == 'X');
    }

    // Test 6: Letter appears once, no change
    {
        std::vector<std::string> grid = {"?A?", "???", "???"};
        auto& result = fillPartialGrid(grid);
        assert(result[0][1] == 'A');
        // other cells remain '?'
        assert(result[0][0] == '?');
        assert(result[2][2] == '?');
    }

    // Test 7: Chain expansion: filling A expands B's box, etc.
    {
        std::vector<std::string> grid = {"A?B", "???", "A?B"};
        auto& result = fillPartialGrid(grid);
        assert(result[0] == "AAB");
        assert(result[1] == "AAB");
        assert(result[2] == "AAB");
    }

    // Test 8: Non-square grid
    {
        std::vector<std::string> grid = {"A?A", "????"};
        auto& result = fillPartialGrid(grid);
        assert(result[0] == "AAA");
        assert(result[1] == "AAA");
    }

    // Test 9: Input with no uppercase letters (all '?')
    {
        std::vector<std::string> grid = {"???", "???"};
        auto& result = fillPartialGrid(grid);
        // No letters, so nothing changes
        assert(result[0] == "???");
        assert(result[1] == "???");
    }

    // Test 10: Larger grid with multiple letters and overlap guarantee
    {
        std::vector<std::string> grid = {
            "A?C?A",
            "?B?B?",
            "A?C?A"
        };
        auto& result = fillPartialGrid(grid);
        // B's bounding box is at row 1, col 1 and row 1 col 3, so fill row 1 cols 1-3 with B
        assert(result[1][1] == 'B');
        assert(result[1][2] == 'B');
        assert(result[1][3] == 'B');
        // A's bounding box is all rows 0-2, cols 0 and 4 => fill col 0 and 4 fully with A
        assert(result[0][0] == 'A');
        assert(result[2][0] == 'A');
        assert(result[0][4] == 'A');
        assert(result[2][4] == 'A');
        // C's bounding box is row 0 and 2, col 2 => fill col 2 with C
        assert(result[0][2] == 'C');
        assert(result[2][2] == 'C');
    }

    return 0;
}
