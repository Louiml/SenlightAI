// Write a C++ function named `floodFillRegion` that takes a vector of strings representing a grid (each string may have a different length), a marker character (e.g., `'*'`, `'#'`, or any non-space, non-underscore character that outlines a region), and a character `fillChar` (the original character inside the region to be replaced). The function must perform a 4-directional flood fill from the first non-space, non-marker, non-underscore character encountered (scanning row-major from top-left), replacing all connected cells containing that original character (including diagonally? No—only orthogonal neighbors) with the fill character, but **stopping** at the marker boundaries and not crossing spaces or underscores. It must return the modified grid as a vector of strings. The input grid is terminated by a line consisting of a single underscore `'_'` followed by newline, but the function should ignore that terminator and process only the lines before it. If no fillable region exists (i.e., no character other than spaces, underscores, or the marker), return the grid unchanged. Preserve the original line lengths; do not pad. All characters are printable ASCII except spaces, underscores, and markers are considered boundaries. The marker is the first non-space, non-underscore character seen in the grid (it may appear multiple times). The flood fill should only fill cells that contain the same character as the starting cell and are not blocked by the marker, spaces, or underscores. Assume at least one line before the underscore.

#include <cassert>
#include <vector>
#include <string>

// Include the solution function here (in practice, it would be in a header).

int main() {
    // Test 1: Simple box
    std::vector<std::string> grid1 = {
        "*****",
        "*aaa*",
        "*aba*",
        "*****"
    };
    auto res1 = floodFillRegion(grid1, 'x');
    assert(res1[1][1] == 'x');
    assert(res1[1][2] == 'x');
    assert(res1[2][1] == 'x');
    assert(res1[2][2] == 'a'); // isolated 'a' not connected to start
    assert(res1[0][0] == '*');

    // Test 2: No region
    std::vector<std::string> grid2 = {
        "####",
        "#  #",
        "####"
    };
    auto res2 = floodFillRegion(grid2, 'z');
    assert(res2 == grid2);

    // Test 3: Region with spaces as barriers
    std::vector<std::string> grid3 = {
        "+++ ++",
        "+ab+ a",
        "+++ ++"
    };
    auto res3 = floodFillRegion(grid3, 'y');
    assert(res3[1][1] == 'y');
    assert(res3[1][2] == 'y');
    assert(res3[1][5] == 'a'); // separated by space, not filled

    // Test 4: Multiple rows with varying lengths
    std::vector<std::string> grid4 = {
        "###",
        "#ab",
        "###c"
    };
    auto res4 = floodFillRegion(grid4, 'q');
    // marker is '#', start at (1,1) 'a', fill 'a' and 'b' (neighbor orthogonally)
    assert(res4[1][1] == 'q');
    assert(res4[1][2] == 'q');
    assert(res4[2][0] == '#');
    assert(res4[2][3] == 'c');

    // Test 5: Only one cell region
    std::vector<std::string> grid5 = {
        "X"
    };
    auto res5 = floodFillRegion(grid5, 'o');
    assert(res5[0][0] == 'o');

    return 0;
}

#include <vector>
#include <string>
#include <queue>
#include <utility>

// Perform flood fill on a grid. The marker is the first non-space, non-underscore character.
// Replace all connected cells (orthogonal) that have the same character as the first fillable cell
// with the given fillChar. Return the modified grid.
std::vector<std::string> floodFillRegion(const std::vector<std::string>& grid, char fillChar) {
    int rows = static_cast<int>(grid.size());
    if (rows == 0) return grid;

    // Determine marker: first non-space, non-underscore character.
    char marker = 0;
    for (int i = 0; i < rows && marker == 0; ++i) {
        for (int j = 0; j < static_cast<int>(grid[i].size()); ++j) {
            if (grid[i][j] != ' ' && grid[i][j] != '_') {
                marker = grid[i][j];
                break;
            }
        }
    }
    if (marker == 0) return grid; // no marker found

    // Find starting cell of region to fill.
    int startRow = -1, startCol = -1;
    char startChar = 0;
    for (int i = 0; i < rows && startRow == -1; ++i) {
        for (int j = 0; j < static_cast<int>(grid[i].size()); ++j) {
            char c = grid[i][j];
            if (c != ' ' && c != '_' && c != marker) {
                startRow = i;
                startCol = j;
                startChar = c;
                break;
            }
        }
    }
    if (startRow == -1) return grid; // no fillable region

    // Make a mutable copy
    std::vector<std::string> result = grid;

    // Visited matrix (rows x max width)
    std::vector<std::vector<bool>> visited(rows);
    int maxWidth = 0;
    for (const auto& s : grid) {
        if (static_cast<int>(s.size()) > maxWidth) maxWidth = static_cast<int>(s.size());
    }
    for (int i = 0; i < rows; ++i) {
        visited[i].assign(maxWidth, false);
    }

    // BFS
    std::queue<std::pair<int,int>> q;
    q.push({startRow, startCol});
    visited[startRow][startCol] = true;
    result[startRow][startCol] = fillChar;

    const int dr[] = {1, -1, 0, 0};
    const int dc[] = {0, 0, 1, -1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < static_cast<int>(grid[nr].size())) {
                if (!visited[nr][nc] && grid[nr][nc] == startChar) {
                    visited[nr][nc] = true;
                    result[nr][nc] = fillChar;
                    q.push({nr, nc});
                }
            }
        }
    }

    return result;
}

// The solution must first parse the input lines (stopping at a line whose first character is `'_'`). Determine the marker by scanning the grid row-major, skipping spaces and underscores; the first non-space, non-underscore character is the marker (it appears as a boundary outline). Then find the first cell that is not visited, not space, not underscore, and not equal to the marker; that cell’s character becomes the fill target. If such a cell exists, start a BFS (or DFS) from it using a queue; for each cell, examine its four orthogonal neighbors. A neighbor is valid if it is within bounds, not visited, and its character is the same as the starting cell's original character (not the marker, not space, not underscore). When valid, update the grid cell to the new fill character (which is the same as the starting character, but the task requires returning the grid after replacing that character with the given `fillChar`? Wait—re-read: The function should replace all connected cells containing that original character with the fill character. The given code snippet uses the original character as the fill (it sets `grid[rr][cc] = line`, where `line` is the character at the start). The task is to write a function that fills that region with a provided `fillChar`. So the function signature: `vector<string> floodFillRegion(const vector<string>& input, char marker, char fillChar)`? But the marker is not provided; it is detected from input. So signature: `vector<string> floodFillRegion(const vector<string>& grid, char fillChar)`? Let's design: The function receives the grid (only lines before the underscore, so the caller has stripped the terminator) and a `fillChar`. It internally detects the marker and the region to fill. It returns a new grid where the region's characters are replaced by `fillChar`. Edge cases: grid may have lines of varying lengths; ensure bounds checking uses `grid[i].size()` per row. If no region exists, return original grid. Use a 2D boolean visited array sized to max rows and max width (but we can use a set of pairs or vector<vector<bool>>). BFS ensures linear time. Time complexity O(R*C) where R is number of rows, C is maximum width (or sum of widths). Space complexity O(R*C) for visited.
