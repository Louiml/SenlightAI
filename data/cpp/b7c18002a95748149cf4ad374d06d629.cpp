/*
Write a C++ function `int countOilDeposits(const std::vector<std::string>& grid)` that counts the number of connected oil deposits in a 2D grid of characters, where `'@'` represents oil and `'*'` represents empty space. Two cells are considered connected if they are adjacent horizontally, vertically, or diagonally (8 directions). The function should take a rectangular grid (all strings of equal length) and return the total number of distinct connected groups of `'@'` cells. The grid is at most 100×100. The input grid should not be modified; the function must work on a copy or use recursion without altering the original. Ensure the function handles edge cases like empty grid, grids with no oil, and grids where all cells are oil.
*/
#include <vector>
#include <string>
#include <functional>

// Counts the number of connected oil deposits ('@') in a rectangular grid.
// Cells are connected if adjacent in any of the 8 directions.
int countOilDeposits(const std::vector<std::string>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;

    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    // Make a mutable copy to mark visited cells without modifying input.
    std::vector<std::string> mutableGrid = grid;

    // Direction offsets for 8 neighbors (row, col): NW, N, NE, W, E, SW, S, SE
    const int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // Iterative DFS using a stack to avoid recursion depth issues.
    std::function<void(int, int)> floodFill = [&](int r, int c) {
        // Use an explicit stack for DFS (safe for large grids).
        std::vector<std::pair<int, int>> stack;
        stack.push_back({r, c});
        mutableGrid[r][c] = '*'; // mark as visited

        while (!stack.empty()) {
            auto [cr, cc] = stack.back();
            stack.pop_back();

            for (int d = 0; d < 8; ++d) {
                int nr = cr + dr[d];
                int nc = cc + dc[d];
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && mutableGrid[nr][nc] == '@') {
                    mutableGrid[nr][nc] = '*'; // mark before pushing to avoid duplicates
                    stack.push_back({nr, nc});
                }
            }
        }
    };

    int count = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (mutableGrid[i][j] == '@') {
                ++count;
                floodFill(i, j);
            }
        }
    }
    return count;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be declared above.
// (Here we include it for completeness, but in a real task it would be linked.)

int main() {
    // Empty grid
    assert(countOilDeposits({}) == 0);

    // No oil
    assert(countOilDeposits({"***", "***"}) == 0);

    // Single deposit
    assert(countOilDeposits({"@*", "**"}) == 1);

    // All oil
    assert(countOilDeposits({"@@", "@@"}) == 1);

    // Two separate deposits
    assert(countOilDeposits({"@*@", "***", "@*@"}) == 3);

    // Diagonal connectivity
    assert(countOilDeposits({"@*", "*@"}) == 1);

    // Classic UVa 572 example
    std::vector<std::string> grid1 = {"*@*@*", "*@*@*", "*@*@*"};
    assert(countOilDeposits(grid1) == 3);

    std::vector<std::string> grid2 = {"@@*@", "@@*@", "*@*@"};
    assert(countOilDeposits(grid2) == 4);

    // Larger test with one big blob
    std::vector<std::string> grid3 = {"@@@", "@@@", "@@@"};
    assert(countOilDeposits(grid3) == 1);

    // Check input not modified
    std::vector<std::string> original = {"@*", "*@"};
    countOilDeposits(original);
    assert(original == std::vector<std::string>({"@*", "*@"}));

    return 0;
}
// The problem is a classic flood-fill / connected components counting problem on an implicit grid with 8-neighbor connectivity. The main algorithm: iterate over every cell in the grid. When an unvisited `'@'` cell is found, increment the counter and perform a depth-first search (DFS) that marks all reachable adjacent `'@'` cells as visited (e.g., by changing them to `'*'` or using a separate visited array). Since the input grid must not be modified, we can either make a mutable copy of the grid inside the function or use a separate `visited` boolean matrix. The DFS explores all 8 directions using two offset arrays for row and column changes. Edge cases: empty grid (return 0), grid with no `'@'` (return 0), grid with all `'@'` (return 1), and handling boundary checks to avoid out-of-bounds access. Time complexity is O(R*C) because each cell is visited at most once; space complexity is O(R*C) for the recursive call stack in the worst case (if the whole grid is one large oil deposit) and O(R*C) for the visited array or grid copy.
