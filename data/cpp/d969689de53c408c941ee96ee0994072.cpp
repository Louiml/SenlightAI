// Write a standalone C++ function `std::vector<std::vector<std::pair<int,int>>> findAllPaths(const std::vector<std::vector<int>>& maze, int startX, int startY, int endX, int endY)` that takes a rectangular maze represented as a grid of integers where `1` represents a passable cell and `0` represents a wall, along with start and end coordinates (0-indexed), and returns a vector containing every simple path from start to end. Each path is represented as a vector of `std::pair<int,int>` coordinates in order from start to end, where the first pair is always (startX, startY) and the last pair is always (endX, endY). The function must find all distinct paths that do not revisit any cell (simple paths) and may only move up, down, left, or right (not diagonally). The maze is guaranteed to have at least one passable path, and start and end cells are always passable (`1`). If start equals end, the function should return a vector containing one path with a single coordinate pair. The paths can be produced in any order; the function must return an empty vector only if no path exists (though the problem guarantees at least one, handle it defensively).
#include <cassert>
#include <vector>
#include <utility>
#include <algorithm>

// Helper to check if a path is valid given the maze.
bool isValidPath(const std::vector<std::vector<int>>& maze,
                 const std::vector<std::pair<int,int>>& path,
                 int startX, int startY, int endX, int endY) {
    if (path.empty()) return false;
    if (path.front() != std::make_pair(startX, startY)) return false;
    if (path.back() != std::make_pair(endX, endY)) return false;
    for (size_t i = 0; i < path.size(); ++i) {
        auto [x, y] = path[i];
        if (x < 0 || x >= static_cast<int>(maze.size()) ||
            y < 0 || y >= static_cast<int>(maze[0].size())) return false;
        if (maze[x][y] != 1) return false;
        if (i > 0) {
            int dx = std::abs(x - path[i-1].first);
            int dy = std::abs(y - path[i-1].second);
            if (dx + dy != 1) return false; // must be adjacent
        }
        // No repeated cells (simple path)
        for (size_t j = 0; j < i; ++j) {
            if (path[j] == path[i]) return false;
        }
    }
    return true;
}

int main() {
    // Test 1: Simple 1x1 maze, start=end
    std::vector<std::vector<int>> maze1 = {{1}};
    auto paths1 = findAllPaths(maze1, 0,0,0,0);
    assert(paths1.size() == 1);
    assert(paths1[0] == std::vector<std::pair<int,int>>{{0,0}});

    // Test 2: Linear maze 2x1 (two cells)
    std::vector<std::vector<int>> maze2 = {{1}, {1}};
    auto paths2 = findAllPaths(maze2, 0,0,1,0);
    assert(paths2.size() == 1);
    assert(isValidPath(maze2, paths2[0], 0,0,1,0));
    assert(paths2[0].size() == 2);

    // Test 3: 2x2 open grid, start (0,0) to (1,1): exactly 2 paths
    std::vector<std::vector<int>> maze3 = {{1,1}, {1,1}};
    auto paths3 = findAllPaths(maze3, 0,0,1,1);
    assert(paths3.size() == 2);
    for (const auto& p : paths3) assert(isValidPath(maze3, p, 0,0,1,1));
    // Verify unique paths (order may vary)
    std::vector<std::vector<std::pair<int,int>>> expected3 = {
        {{0,0},{0,1},{1,1}},
        {{0,0},{1,0},{1,1}}
    };
    for (const auto& exp : expected3) {
        bool found = false;
        for (const auto& p : paths3) if (p == exp) found = true;
        assert(found);
    }

    // Test 4: 3x3 with a wall in the middle
    std::vector<std::vector<int>> maze4 = {
        {1,0,1},
        {1,1,1},
        {1,0,1}
    };
    // Start (0,0) to (2,2): need to go around center wall
    auto paths4 = findAllPaths(maze4, 0,0,2,2);
    assert(paths4.size() == 2); // two ways around the center wall
    for (const auto& p : paths4) assert(isValidPath(maze4, p, 0,0,2,2));

    // Test 5: 1x2 maze, start (0,0) to (0,1): exactly 1 path
    std::vector<std::vector<int>> maze5 = {{1,1}};
    auto paths5 = findAllPaths(maze5, 0,0,0,1);
    assert(paths5.size() == 1);
    assert(paths5[0] == std::vector<std::pair<int,int>>{{0,0},{0,1}});

    // Test 6: 3x3 grid with a single corridor (no branches)
    std::vector<std::vector<int>> maze6 = {
        {1,1,1},
        {0,0,1},
        {0,0,1}
    };
    auto paths6 = findAllPaths(maze6, 0,0,2,2);
    assert(paths6.size() == 1);
    assert(isValidPath(maze6, paths6[0], 0,0,2,2));
    // The only path must go through (0,1),(0,2),(1,2),(2,2)
    assert(paths6[0] == std::vector<std::pair<int,int>>{{0,0},{0,1},{0,2},{1,2},{2,2}});

    // Test 7: 4x4 open grid, count of simple paths from corner to corner is known to be 6
    std::vector<std::vector<int>> maze7(4, std::vector<int>(4, 1));
    auto paths7 = findAllPaths(maze7, 0,0,3,3);
    assert(paths7.size() == 6);
    for (const auto& p : paths7) assert(isValidPath(maze7, p, 0,0,3,3));

    // Test 8: 2x3 open grid, from (0,1) to (1,1) yields exactly 3 paths
    std::vector<std::vector<int>> maze8 = {{1,1,1}, {1,1,1}};
    auto paths8 = findAllPaths(maze8, 0,1,1,1);
    assert(paths8.size() == 3);
    for (const auto& p : paths8) assert(isValidPath(maze8, p, 0,1,1,1));

    // Test 9: Invalid start (unpassable) returns empty
    std::vector<std::vector<int>> maze9 = {{0,1}, {1,1}};
    auto paths9 = findAllPaths(maze9, 0,0,1,1);
    assert(paths9.empty());

    // Test 10: Start and end in same cell but with walls around, still returns single path
    std::vector<std::vector<int>> maze10 = {{1,0},{0,1}};
    auto paths10 = findAllPaths(maze10, 0,0,0,0);
    assert(paths10.size() == 1);
    assert(paths10[0] == std::vector<std::pair<int,int>>{{0,0}});

    return 0;
}
#include <vector>
#include <utility>
#include <cstddef>

// Recursively explore all simple paths from current position to end.
void dfsFindPaths(const std::vector<std::vector<int>>& maze,
                  int curX, int curY,
                  int endX, int endY,
                  std::vector<std::vector<bool>>& visited,
                  std::vector<std::pair<int,int>>& currentPath,
                  std::vector<std::vector<std::pair<int,int>>>& allPaths) {
    // If we reached the end, store a copy of the current path.
    if (curX == endX && curY == endY) {
        allPaths.push_back(currentPath);
        return;
    }

    const int rows = static_cast<int>(maze.size());
    const int cols = static_cast<int>(maze[0].size());

    // Define the four possible directions: down, up, right, left.
    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    for (int dir = 0; dir < 4; ++dir) {
        int nextX = curX + dx[dir];
        int nextY = curY + dy[dir];

        // Check bounds, passability, and whether already visited.
        if (nextX >= 0 && nextX < rows &&
            nextY >= 0 && nextY < cols &&
            maze[nextX][nextY] == 1 &&
            !visited[nextX][nextY]) {

            visited[nextX][nextY] = true;
            currentPath.push_back({nextX, nextY});

            dfsFindPaths(maze, nextX, nextY, endX, endY, visited, currentPath, allPaths);

            // Backtrack.
            currentPath.pop_back();
            visited[nextX][nextY] = false;
        }
    }
}

// Find all simple paths from (startX,startY) to (endX,endY) in the maze.
// maze[i][j] == 1 means passable, 0 means wall.
std::vector<std::vector<std::pair<int,int>>> findAllPaths(
    const std::vector<std::vector<int>>& maze,
    int startX, int startY, int endX, int endY) {

    std::vector<std::vector<std::pair<int,int>>> allPaths;

    const int rows = static_cast<int>(maze.size());
    if (rows == 0) return allPaths;
    const int cols = static_cast<int>(maze[0].size());
    if (cols == 0) return allPaths;

    // Validate start/end are within bounds and passable.
    if (startX < 0 || startX >= rows || startY < 0 || startY >= cols ||
        endX   < 0 || endX   >= rows || endY   < 0 || endY   >= cols ||
        maze[startX][startY] != 1 || maze[endX][endY] != 1) {
        return allPaths; // invalid input, return empty
    }

    // If start equals end, single-cell path.
    if (startX == endX && startY == endY) {
        allPaths.push_back({{startX, startY}});
        return allPaths;
    }

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::pair<int,int>> currentPath;
    currentPath.push_back({startX, startY});
    visited[startX][startY] = true;

    dfsFindPaths(maze, startX, startY, endX, endY, visited, currentPath, allPaths);

    return allPaths;
}
// The core approach is a depth-first search (DFS) with backtracking that maintains a current path and a visited set. Starting at the start cell, we recursively explore each of the four neighboring cells (up, down, left, right) that are within bounds, passable (value `1`), and not yet visited. When we reach the end cell, we record a copy of the current path in the result vector. After exploring all neighbors from a cell, we backtrack by removing the cell from the current path and marking it as unvisited so other paths can use it. This ensures we find all simple paths. Important edge cases: (1) start equals end → return a single path with one coordinate; (2) the maze may contain walls (`0`), so we must skip them; (3) the maze may have multiple solutions, and our DFS will find all of them; (4) we must use a 2D visited array or an unordered set to avoid revisiting cells in the current path, but we must reset visited flags during backtracking. Time complexity: In the worst case (e.g., an open grid with no walls), the number of simple paths between two points can be exponential; for each path we do work proportional to its length, so the time complexity is \(O(P \cdot L)\), where \(P\) is the number of paths and \(L\) is the average path length, which can be exponential in the grid dimensions. Space complexity is \(O(R \cdot C)\) for the visited array plus the recursion stack depth (up to \(R \cdot C\)) and the storage for all paths, which also can be exponential in the worst case.
