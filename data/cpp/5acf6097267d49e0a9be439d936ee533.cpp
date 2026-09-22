Write a C++ function named `findStablePattern` that takes a 2D integer vector representing a grid of colored balls (values 0 meaning empty, positive integers meaning colors) and returns a vector of pairs representing the coordinates (row, column) of all balls that form part of at least one contiguous horizontal or vertical segment of three or more balls of the same color. The grid dimensions are at least 3×3. Coordinates are zero-based. The function must return the positions in row-major order (sorted by row, then column) without duplicates. If no such balls exist, return an empty vector. The function must not modify the input grid. Example: for grid `{{1,1,1,2},{0,3,3,3},{2,2,2,2}}`, the balls at (0,0),(0,1),(0,2) form a horizontal segment; (1,1),(1,2),(1,3) form another; and all four (2,0),(2,1),(2,2),(2,3) form a horizontal segment (len>=3). The result would include those 9 unique positions sorted by row-major: (0,0),(0,1),(0,2),(1,1),(1,2),(1,3),(2,0),(2,1),(2,2),(2,3).

// The solution uses a two‑pass approach: first, scan each row to detect horizontal runs of three or more equal colors; mark those positions. Then scan each column to detect vertical runs of three or more equal colors; mark those positions. The marking uses a boolean visited grid of same size to avoid duplicates. After both passes, collect all marked positions into a vector of pairs and sort by row, then column (row-major order). Edge cases: when a ball belongs to both a horizontal and vertical segment, it is added once. When a run is longer than three, all positions in the run are marked. The function assumes that the grid is non‑empty rectangular with at least 3 rows and columns, but it can handle smaller sizes gracefully by just returning an empty vector. Time complexity is O(R*C) because each cell is visited a constant number of times per pass (scanning rows and columns), plus O(K log K) for sorting the result, where K is the number of marked cells (K ≤ R*C). Auxiliary space is O(R*C) for the visited grid and the result vector.

#include <vector>
#include <utility>
#include <algorithm>

// Returns the row-major sorted list of (row, col) coordinates of balls that are part
// of a horizontal or vertical contiguous segment of at least 3 equal values.
std::vector<std::pair<int,int>> findStablePattern(const std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) return {};
    int R = static_cast<int>(grid.size());
    int C = static_cast<int>(grid[0].size());
    if (R < 3 && C < 3) return {}; // no possible segment of length 3

    std::vector<std::vector<bool>> marked(R, std::vector<bool>(C, false));

    // Horizontal runs
    for (int i = 0; i < R; ++i) {
        int j = 0;
        while (j < C) {
            int start = j;
            int color = grid[i][j];
            ++j;
            while (j < C && grid[i][j] == color) {
                ++j;
            }
            int len = j - start;
            if (len >= 3) {
                for (int k = start; k < j; ++k) {
                    marked[i][k] = true;
                }
            }
        }
    }

    // Vertical runs
    for (int j = 0; j < C; ++j) {
        int i = 0;
        while (i < R) {
            int start = i;
            int color = grid[i][j];
            ++i;
            while (i < R && grid[i][j] == color) {
                ++i;
            }
            int len = i - start;
            if (len >= 3) {
                for (int k = start; k < i; ++k) {
                    marked[k][j] = true;
                }
            }
        }
    }

    std::vector<std::pair<int,int>> result;
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            if (marked[i][j]) {
                result.emplace_back(i, j);
            }
        }
    }
    // Already in row-major order due to the loop order, but sort for safety.
    std::sort(result.begin(), result.end());
    return result;
}

#include <cassert>
#include <vector>
#include <utility>

// (the solution function is assumed to be included above)

int main() {
    // Basic horizontal and vertical
    std::vector<std::vector<int>> g1 = {{1,1,1,2},{0,3,3,3},{2,2,2,2}};
    auto r1 = findStablePattern(g1);
    std::vector<std::pair<int,int>> e1 = {{0,0},{0,1},{0,2},{1,1},{1,2},{1,3},{2,0},{2,1},{2,2},{2,3}};
    assert(r1 == e1);

    // Single vertical run of length 5
    std::vector<std::vector<int>> g2 = {{5},{5},{5},{5},{5}}; // but grid must be at least 3 columns? actual use: make 5x5 diagonal? Let's do 1 row? Not valid. Use 5x1 grid? spec says at least 3×3 but we still handle.
    std::vector<std::vector<int>> g2b = {{5,0,0},{5,1,1},{5,2,2},{5,3,3},{5,4,4}};
    auto r2 = findStablePattern(g2b);
    std::vector<std::pair<int,int>> e2 = {{0,0},{1,0},{2,0},{3,0},{4,0}};
    assert(r2 == e2);

    // Cross pattern (both horizontal and vertical share a common center)
    std::vector<std::vector<int>> g3 = {{0,1,0},{1,1,1},{0,1,0}};
    auto r3 = findStablePattern(g3);
    std::vector<std::pair<int,int>> e3 = {{0,1},{1,0},{1,1},{1,2},{2,1}};
    assert(r3 == e3);

    // No segments (all different)
    std::vector<std::vector<int>> g4 = {{1,2,3},{4,5,6},{7,8,9}};
    assert(findStablePattern(g4).empty());

    // Length exactly 3 at boundary
    std::vector<std::vector<int>> g5 = {{7,7,7},{0,0,0},{0,0,0}};
    auto r5 = findStablePattern(g5);
    std::vector<std::pair<int,int>> e5 = {{0,0},{0,1},{0,2}};
    assert(r5 == e5);

    // Length 4 horizontal, also vertical in same run but overlapping
    std::vector<std::vector<int>> g6 = {{2,2,2,2},{2,1,1,1},{2,1,1,1}};
    auto r6 = findStablePattern(g6);
    // row0: all 4; col0 rows0-2 -> 3; col1 rows1-2 -> len2 (no); etc.
    std::vector<std::pair<int,int>> e6 = {{0,0},{0,1},{0,2},{0,3},{1,0},{2,0}};
    assert(r6 == e6);

    // Empty grid
    std::vector<std::vector<int>> g7;
    assert(findStablePattern(g7).empty());

    // Single row with three
    std::vector<std::vector<int>> g8 = {{3,3,3}};
    auto r8 = findStablePattern(g8);
    std::vector<std::pair<int,int>> e8 = {{0,0},{0,1},{0,2}};
    assert(r8 == e8);

    return 0;
}
