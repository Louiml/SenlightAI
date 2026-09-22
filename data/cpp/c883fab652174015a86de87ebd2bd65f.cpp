Write a C++ function `findShortestPath` that takes a rectangular grid of characters (a vector of strings) containing exactly one cell `'A'` (start) and one cell `'B'` (target), with `'.'` as open cells and `'#'` as walls. The function must return a string representing the shortest path from `'A'` to `'B'` as a sequence of moves `'U'` (up), `'D'` (down), `'L'` (left), `'R'` (right). If no path exists, return the string `"NO"`. The grid dimensions are given as `n` (rows) and `m` (columns). Movement is allowed only in the four cardinal directions, and you may not step onto walls or leave the grid. The function should return either the list of moves (e.g., `"DRUR"`) or `"NO"`. Assume `n, m >= 1` and that `'A'` and `'B'` are always present exactly once.

// The solution uses a standard breadth-first search (BFS) because it guarantees the shortest path in an unweighted grid. We maintain a visited matrix initialized to `'.'` on every cell. We start BFS from the position of `'A'`. For each cell popped from the queue, we examine its four neighbors (up, down, left, right). If a neighbor is within bounds, is not a wall, and has not been visited yet, we mark it visited and record the move direction that led into that neighbor in the visited matrix (using characters `'U'`, `'D'`, `'L'`, `'R'`). We push the neighbor into the queue. BFS stops when we pop the `'B'` cell or the queue becomes empty. After BFS, if `'B'` was never visited, there is no path, and we return `"NO"`. Otherwise, we reconstruct the path by starting at `'B'` and tracing backwards: each step, we read the move direction stored at the current cell, append it to the answer string, and move back to the previous cell by reversing the move (e.g., if the stored move is `'U'`, go down). We continue until we reach the `'A'` cell. Finally, reverse the collected string to get the correct forward path. Edge cases include `'A'` and `'B'` being adjacent (path length 1), the grid being a single cell (impossible since both `'A'` and `'B'` exist, so at least two open cells), and cases where all paths are blocked. Time complexity is O(n*m) because each cell is enqueued at most once. Space complexity is O(n*m) for the visited matrix and the queue.

#include <string>
#include <vector>
#include <queue>
#include <utility>

// Returns the shortest path from 'A' to 'B' as a string of moves,
// or "NO" if no path exists.
std::string findShortestPath(const std::vector<std::string>& grid) {
    int n = grid.size();
    int m = grid[0].size();

    std::pair<int, int> start, end;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == 'A') start = {i, j};
            if (grid[i][j] == 'B') end = {i, j};
        }
    }

    // visited stores '.' for unvisited, otherwise the move direction
    // used to reach that cell.
    std::vector<std::string> visited(n, std::string(m, '.'));

    const int dx[4] = {1, 0, -1, 0};
    const int dy[4] = {0, -1, 0, 1};
    const char dirChar[4] = {'D', 'L', 'U', 'R'};

    std::queue<std::pair<int, int>> q;
    q.push(start);
    visited[start.first][start.second] = 'A';

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        if (x == end.first && y == end.second) break;

        for (int k = 0; k < 4; ++k) {
            int nx = x + dx[k];
            int ny = y + dy[k];
            if (nx < 0 || ny < 0 || nx >= n || ny >= m) continue;
            if (visited[nx][ny] != '.') continue;
            if (grid[nx][ny] == '#') continue;
            visited[nx][ny] = dirChar[k];
            q.push({nx, ny});
        }
    }

    if (visited[end.first][end.second] == '.') {
        return "NO";
    }

    std::string path;
    int x = end.first;
    int y = end.second;
    while (grid[x][y] != 'A') {
        char c = visited[x][y];
        path += c;
        if (c == 'U') ++x;          // came from up, so go down to previous
        else if (c == 'D') --x;     // came from down, so go up
        else if (c == 'L') ++y;     // came from left, so go right
        else --y;                   // came from right, so go left
    }

    std::reverse(path.begin(), path.end());
    return path;
}

#include <cassert>
#include <string>
#include <vector>

// The function from the solution block is assumed to be included above.

int main() {
    // Simple 3x3 grid with direct path
    std::vector<std::string> grid1 = {
        "A.B",
        "...",
        "###"
    };
    assert(findShortestPath(grid1) == "R");

    // Larger grid with orthogonal turns
    std::vector<std::string> grid2 = {
        "A..",
        ".#.",
        "..B"
    };
    // Path: D D R R or R R D D (both length 4)
    std::string result2 = findShortestPath(grid2);
    assert(result2.size() == 4);
    assert(result2 == "DDRR" || result2 == "RRDD");

    // No path due to wall separating
    std::vector<std::string> grid3 = {
        "A#B",
        ".#.",
        "..."
    };
    assert(findShortestPath(grid3) == "NO");

    // A and B adjacent vertically
    std::vector<std::string> grid4 = {
        "A.",
        "B.",
        ".."
    };
    assert(findShortestPath(grid4) == "D");

    // Path that requires going around a wall
    std::vector<std::string> grid5 = {
        "A.#",
        ".#.",
        "#.B"
    };
    // Only path: R D L D R (from start) or similar
    std::string result5 = findShortestPath(grid5);
    assert(result5 == "RDLDR" || result5 == "DRDLR" || result5 == "DDRR" || result5 == "RRDD");
    // Actually the only valid shortest is "RDLDR" or "DRDLR" but both length 5.
    assert(result5.size() == 5);

    // Single open corridor with turn
    std::vector<std::string> grid6 = {
        "A#..",
        ".#.#",
        "...B"
    };
    std::string result6 = findShortestPath(grid6);
    assert(result6.size() >= 1);
    assert(result6 != "NO");

    // B is immediately reachable after one step left
    std::vector<std::string> grid7 = {
        "BA",
        ".."
    };
    assert(findShortestPath(grid7) == "L");

    // Grid with only start and target, no walls
    std::vector<std::string> grid8 = {
        "AB"
    };
    assert(findShortestPath(grid8) == "R");

    // Fully blocked except for start cell
    std::vector<std::string> grid9 = {
        "A#",
        "##"
    };
    // End B must exist, but impossible here; so not valid input. Instead test a valid no-path.
    std::vector<std::string> grid10 = {
        "A#",
        "#B"
    };
    assert(findShortestPath(grid10) == "NO");

    return 0;
}
