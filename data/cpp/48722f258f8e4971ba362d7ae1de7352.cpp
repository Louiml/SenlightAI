// You are given a rectangular board represented as a vector of strings, where each character is either `'#'` (a blocked cell) or `'.'` (an open cell). Write a C++ function that takes the board plus a starting cell (given by row and column indices) and a target cell, and returns the minimum Manhattan distance (not path length) from the start to the target **only if** there exists a contiguous path of open cells between them using 4-directional moves (up, down, left, right) that does not enter blocked cells. If no such path exists, return `-1`. If the start or target is blocked or out of bounds, also return `-1`. The Manhattan distance is computed as `abs(startRow - targetRow) + abs(startCol - targetCol)` and is independent of obstacles; it is only reported when a valid open-cell path exists. The board dimensions are at least 1×1 and both indices are zero-based.
#include <cassert>
#include <string>
#include <vector>

// The solution function is assumed to be declared above
// (in a real compilation, this would be in a header)

int main() {
    // Test 1: Simple open 3x3 grid, start (0,0) to target (2,2) - reachable, Manhattan=4
    std::vector<std::string> b1 = {"...", "...", "..."};
    assert(pathManhattanDistance(b1, 0, 0, 2, 2) == 4);

    // Test 2: Target blocked
    std::vector<std::string> b2 = {"...", ".#.", "..."};
    assert(pathManhattanDistance(b2, 0, 0, 2, 2) == -1);

    // Test 3: Start blocked
    std::vector<std::string> b3 = {"#..", "...", "..."};
    assert(pathManhattanDistance(b3, 0, 0, 2, 2) == -1);

    // Test 4: Obstacles block path, but Manhattan is small
    std::vector<std::string> b4 = {"..#", "..#", "..#"};
    assert(pathManhattanDistance(b4, 0, 0, 2, 0) == -1);

    // Test 5: Start equals target (open cell)
    std::vector<std::string> b5 = {".#.", "#.#", ".#."};
    assert(pathManhattanDistance(b5, 1, 1, 1, 1) == 0);

    // Test 6: Edge case - out of bounds start
    std::vector<std::string> b6 = {"."};
    assert(pathManhattanDistance(b6, -1, 0, 0, 0) == -1);

    // Test 7: Edge case - single cell blocked
    std::vector<std::string> b7 = {"#"};
    assert(pathManhattanDistance(b7, 0, 0, 0, 0) == -1);

    // Test 8: Path exists around obstacles, Manhattan distance returned correctly
    std::vector<std::string> b8 = {"....", ".##.", "...."};
    // Start (0,0) to (1,3): There is a path around, Manhattan = 4
    assert(pathManhattanDistance(b8, 0, 0, 1, 3) == 4);

    // Test 9: No path due to wall separating
    std::vector<std::string> b9 = {"...", "###", "..."};
    assert(pathManhattanDistance(b9, 0, 0, 2, 2) == -1);

    // Test 10: Larger board, path exists, Manhattan correct
    std::vector<std::string> b10 = {"....#", "##..#", "..#..", "#...."};
    // Start (0,0) to (3,4): Manhattan = 7, path exists via right and bottom
    assert(pathManhattanDistance(b10, 0, 0, 3, 4) == 7);
}
#include <vector>
#include <queue>
#include <cstdlib> // for abs

int pathManhattanDistance(const std::vector<std::string>& board, int startRow, int startCol, int targetRow, int targetCol) {
    int rows = board.size();
    int cols = board[0].size();
    
    // Validate bounds and blocked cells
    if (startRow < 0 || startRow >= rows || startCol < 0 || startCol >= cols ||
        targetRow < 0 || targetRow >= rows || targetCol < 0 || targetCol >= cols ||
        board[startRow][startCol] == '#' || board[targetRow][targetCol] == '#') {
        return -1;
    }
    
    // If start equals target, Manhattan distance is 0 and path exists trivially
    if (startRow == targetRow && startCol == targetCol) {
        return 0;
    }
    
    // BFS setup
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::queue<std::pair<int,int>> q;
    q.push({startRow, startCol});
    visited[startRow][startCol] = true;
    
    // Direction vectors: up, down, left, right
    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};
    
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        
        // Explore neighbors
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                !visited[nr][nc] && board[nr][nc] == '.') {
                if (nr == targetRow && nc == targetCol) {
                    return std::abs(startRow - targetRow) + std::abs(startCol - targetCol);
                }
                visited[nr][nc] = true;
                q.push({nr, nc});
            }
        }
    }
    
    return -1; // target unreachable
}
// The solution approach uses a breadth-first search (BFS) starting from the given start cell to determine reachability of the target cell through open cells. First, validate that both start and target are within bounds and are not blocked (`'#'`); if either fails, immediately return `-1`. Then perform a standard BFS using a queue of cell coordinates, marking visited cells in a 2D boolean vector to avoid cycles. If the target is reached during BFS, compute and return the Manhattan distance between the start and target (which is constant, not the BFS path length). If BFS exhausts without reaching the target, return `-1`. Edge cases include boards with only one cell where start equals target (if open, returns `0`; if blocked, returns `-1`), and boards with obstacles blocking all possible paths. Time complexity is O(R×C) for BFS traversal, where R and C are board dimensions. Space complexity is O(R×C) for the visited array and the queue in the worst case.
