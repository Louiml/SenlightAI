// Given a two-dimensional grid represented as a `std::vector<std::string>` where each cell is either `'.'` (empty), `'#'` (wall), `'S'` (start), or `'E'` (exit), write a C++ function `int shortestPathLength(const std::vector<std::string>& grid)` that returns the length of the shortest path (number of steps) from the start cell `'S'` to the exit cell `'E'`, moving in four orthogonal directions (up, down, left, right) without stepping onto walls (`'#'`). If no path exists, return `-1`. The grid has at least one `'S'` and exactly one `'E'`. The path may revisit cells, but a BFS-based solution will naturally find the shortest path without revisiting needed. Handle grid sizes up to 1000×1000 efficiently.

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Simple 3x3 grid
    std::vector<std::string> grid1 = {
        "S..",
        ".#.",
        "..E"
    };
    assert(shortestPathLength(grid1) == 4); // S -> (0,1) -> (0,2) -> (1,2) -> (2,2)

    // No path (wall blocks)
    std::vector<std::string> grid2 = {
        "S#E",
        "###",
        "..."
    };
    assert(shortestPathLength(grid2) == -1);

    // Start and exit are adjacent
    std::vector<std::string> grid3 = {
        "SE"
    };
    assert(shortestPathLength(grid3) == 1);

    // Start equals exit (if allowed)
    std::vector<std::string> grid4 = {
        "E"
    };
    // We only have 'E', but assume 'S' is not present; test with start==exit? Instead test normal case.
    grid4[0] = "S";
    assert(shortestPathLength(grid4) == -1); // No 'E' present, but our function expects one. Use a valid case:
    grid4[0] = "S";
    std::vector<std::string> grid4b = {"S"}; // no E → -1
    assert(shortestPathLength(grid4b) == -1);

    // Larger grid with obstacles
    std::vector<std::string> grid5 = {
        "S....",
        "#####",
        "....E"
    };
    assert(shortestPathLength(grid5) == -1); // completely blocked

    std::vector<std::string> grid6 = {
        "S#...",
        ".#...",
        "...#E"
    };
    assert(shortestPathLength(grid6) == 6);

    return 0;
}

#include <vector>
#include <string>
#include <queue>
#include <utility>

// Returns the length of the shortest path from 'S' to 'E' in the grid.
// Moves are orthogonal (up, down, left, right) and cannot go into '#'.
// Returns -1 if no path exists.
int shortestPathLength(const std::vector<std::string>& grid) {
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    int startRow = -1, startCol = -1;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == 'S') {
                startRow = r;
                startCol = c;
                break;
            }
        }
        if (startRow != -1) break;
    }

    // If start is also exit (unlikely but possible), distance is 0.
    if (grid[startRow][startCol] == 'E') return 0;

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::queue<std::pair<int, int>> q;
    q.emplace(startRow, startCol);
    visited[startRow][startCol] = true;

    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};

    int distance = 0;
    while (!q.empty()) {
        int size = static_cast<int>(q.size());
        ++distance;
        for (int i = 0; i < size; ++i) {
            auto [r, c] = q.front();
            q.pop();
            for (int d = 0; d < 4; ++d) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !visited[nr][nc] && grid[nr][nc] != '#') {
                    if (grid[nr][nc] == 'E') {
                        return distance;
                    }
                    visited[nr][nc] = true;
                    q.emplace(nr, nc);
                }
            }
        }
    }
    return -1;
}

// The problem is a classic unweighted shortest-path search on a grid, which can be solved using breadth-first search (BFS). BFS explores all cells at the current distance before moving to the next distance, guaranteeing that the first time we encounter the exit, the distance is minimal. We initialize a queue with the start cell's coordinates and set its distance to 0. We also maintain a visited set (or a 2D boolean array) to avoid re-processing cells, since revisiting cannot yield a shorter path in an unweighted grid. For each cell dequeued, we check its four neighbors; if a neighbor is within bounds, is not a wall, and has not been visited, we mark it visited and enqueue it with distance+1. If the neighbor is the exit, we return its distance immediately. If the queue empties without reaching the exit, we return -1. Edge cases include: start equals exit (should return 0), walls completely blocking the exit, and single-cell grids. Time complexity is O(rows × cols) because every cell is enqueued at most once; space complexity is O(rows × cols) for the visited array plus O(rows × cols) in the worst case for the queue.
