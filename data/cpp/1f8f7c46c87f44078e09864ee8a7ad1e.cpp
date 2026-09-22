Write a C++ function `int shortestPathOnGrid(const std::vector<std::string>& grid)` that, given a rectangular grid as a vector of strings, each character representing a cell, finds the length of the shortest path from the start cell marked `'S'` to the finish cell marked `'F'`. The grid uses `'.'` for open cells and `'#'` for blocked cells. You may move up, down, left, or right by one cell at a time, and you cannot move outside the grid or into blocked cells. The function should return the number of steps in the shortest path (i.e., the number of moves from S to F), or `-1` if no such path exists. The grid always contains exactly one `'S'` and exactly one `'F'`. Note: the start cell itself counts as step 0, so if `S` and `F` are adjacent, the answer is 1. If they are the same cell (not possible in this task), the answer would be 0, but that case will not occur.
#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be defined above.

int main() {
    // Simple direct path
    std::vector<std::string> grid1 = {
        "S...",
        "....",
        "....",
        "...F"
    };
    assert(shortestPathOnGrid(grid1) == 6);

    // Blocked wall requires detour
    std::vector<std::string> grid2 = {
        "S#..",
        ".#..",
        ".#..",
        "..F."
    };
    assert(shortestPathOnGrid(grid2) == 6);

    // No path exists
    std::vector<std::string> grid3 = {
        "S##",
        "###",
        "##F"
    };
    assert(shortestPathOnGrid(grid3) == -1);

    // Adjacent start and finish -> 1 step
    std::vector<std::string> grid4 = {
        "SF"
    };
    assert(shortestPathOnGrid(grid4) == 1);

    // Single row with open cells
    std::vector<std::string> grid5 = {
        "S...F"
    };
    assert(shortestPathOnGrid(grid5) == 4);

    // Single column with open cells
    std::vector<std::string> grid6 = {
        "S",
        ".",
        ".",
        "F"
    };
    assert(shortestPathOnGrid(grid6) == 3);

    // Start surrounded by walls except finish far away
    std::vector<std::string> grid7 = {
        "S.#",
        ".#.",
        "..F"
    };
    assert(shortestPathOnGrid(grid7) == 4);

    // All open but finish at far corner of 5x5
    std::vector<std::string> grid8(5, std::string(5, '.'));
    grid8[0][0] = 'S';
    grid8[4][4] = 'F';
    assert(shortestPathOnGrid(grid8) == 8);

    return 0;
}
#include <vector>
#include <string>
#include <queue>
#include <utility>

// Returns the minimum number of steps from 'S' to 'F' on a grid, or -1 if impossible.
int shortestPathOnGrid(const std::vector<std::string>& grid) {
    const int rows = static_cast<int>(grid.size());
    if (rows == 0) return -1;
    const int cols = static_cast<int>(grid[0].size());

    int startRow = -1, startCol = -1;
    int finishRow = -1, finishCol = -1;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == 'S') {
                startRow = r;
                startCol = c;
            } else if (grid[r][c] == 'F') {
                finishRow = r;
                finishCol = c;
            }
        }
    }

    // If start or finish missing, return -1 (though problem guarantees presence)
    if (startRow == -1 || finishRow == -1) return -1;

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<int>> distance(rows, std::vector<int>(cols, 0));

    std::queue<std::pair<int, int>> q;
    q.push({startRow, startCol});
    visited[startRow][startCol] = true;

    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        if (r == finishRow && c == finishCol) {
            return distance[r][c];
        }

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                !visited[nr][nc] && grid[nr][nc] != '#') {
                visited[nr][nc] = true;
                distance[nr][nc] = distance[r][c] + 1;
                q.push({nr, nc});
            }
        }
    }

    return -1;
}
// The solution uses Breadth‑First Search (BFS) on an unweighted grid, which guarantees the first time we reach the finish cell, we have taken the minimum number of steps. We maintain a queue of cell coordinates. We also have a boolean visited grid to avoid reprocessing cells and an integer distance grid to store the number of steps from the start. Initially, the start cell is marked visited with distance 0. Then we pop cells from the queue, explore the four orthogonal neighbors. For each neighbor that is inside the grid, not blocked, and not visited, we mark it visited, set its distance to current distance + 1, and push it onto the queue. When we pop a cell that equals the finish, we return its distance. If the queue empties without reaching `F`, we return `-1`. Edge cases: the start or finish may be at the border; there may be no path; the grid may be a single row or column; all cells may be blocked except `S` and `F`, in which case the path may be impossible if they are not adjacent. The time complexity is \(O(R \cdot C)\) where \(R\) is the number of rows and \(C\) is the number of columns, since each cell is visited at most once. The space complexity is also \(O(R \cdot C)\) for the visited and distance grids.
