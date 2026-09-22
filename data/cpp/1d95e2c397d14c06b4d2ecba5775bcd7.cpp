Write a C++ function `findAndMarkShortestPath` that, given a rectangular maze represented as a vector of strings (each character `'0'` for a passable cell, `'1'` for a wall, `'e'` for entry, and `'x'` for exit), returns a new vector of strings where the cells along the shortest path (using BFS) from entry to exit are marked with `'.'`, while preserving all other characters. If no path exists, return the original maze unchanged. The function must not modify the input maze. Cells are 4-directionally adjacent (up, down, left, right). The maze is guaranteed to contain exactly one `'e'` and one `'x'`.

#include <cassert>
#include <vector>
#include <string>

// Assume the solution function is declared above.

int main() {
    // Single path
    {
        std::vector<std::string> maze = {
            "e0x",
            "000",
            "111"
        };
        auto result = findAndMarkShortestPath(maze);
        assert(result[0] == ".0x");
        assert(result[1] == "...");
        assert(result[2] == "111");
    }

    // Multiple paths, shortest is marked
    {
        std::vector<std::string> maze = {
            "e000",
            "0100",
            "000x"
        };
        auto result = findAndMarkShortestPath(maze);
        // Shortest path length is 4 (e->(0,1)->(0,2)->(0,3)->(1,3)->(2,3)? Actually let's count:
        // e(0,0)->(0,1)->(0,2)->(1,2)? No, 1 is wall. Better path: (0,0)->(1,0)->(2,0)->(2,1)->(2,2)->(2,3) length 5? Let's just check result marks some path.
        assert(result[0][0] == '.');
        assert(result[2][3] == '.');
    }

    // No path
    {
        std::vector<std::string> maze = {
            "e1x",
            "111",
            "000"
        };
        auto result = findAndMarkShortestPath(maze);
        assert(result == maze);
    }

    // Entry equals exit
    {
        std::vector<std::string> maze = {"ex"};
        auto result = findAndMarkShortestPath(maze);
        assert(result[0] == "..");
    }

    // 1x1 entry equals exit
    {
        std::vector<std::string> maze = {"e"};
        // No 'x'? But guaranteed one exit, so skip this test.
    }

    // Larger maze, ensure entry and exit unchanged
    {
        std::vector<std::string> maze = {
            "e000000",
            "1111110",
            "000000x"
        };
        auto result = findAndMarkShortestPath(maze);
        assert(result[0][0] == '.');
        assert(result[2][6] == '.');
        // Check each path cell is '.'
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 7; ++j) {
                if (result[i][j] == '.') {
                    assert(maze[i][j] == '0' || maze[i][j] == 'e' || maze[i][j] == 'x');
                } else if (result[i][j] == '1') {
                    assert(maze[i][j] == '1');
                }
            }
        }
    }

    // All open grid, verify BFS marks a valid path
    {
        std::vector<std::string> maze = {
            "e00x",
            "0000",
            "0000"
        };
        auto result = findAndMarkShortestPath(maze);
        // Entry and exit marked
        assert(result[0][0] == '.');
        assert(result[0][3] == '.');
        // Count '.' cells should equal distance+1 = 3+1 = 4? Actually distance from (0,0) to (0,3) is 3 steps, so path length 4 cells.
        int count = 0;
        for (auto& row : result) for (char ch : row) if (ch == '.') count++;
        assert(count == 4);
    }

    return 0;
}

#include <vector>
#include <string>
#include <queue>
#include <utility>

// Find shortest path from 'e' to 'x' in a maze, mark with '.', return new maze.
// If no path, return original maze unchanged.
std::vector<std::string> findAndMarkShortestPath(const std::vector<std::string>& maze) {
    int rows = maze.size();
    if (rows == 0) return maze;
    int cols = maze[0].size();

    int start_r = -1, start_c = -1, end_r = -1, end_c = -1;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (maze[i][j] == 'e') { start_r = i; start_c = j; }
            if (maze[i][j] == 'x') { end_r = i; end_c = j; }
        }
    }

    // BFS
    std::vector<std::vector<int>> dist(rows, std::vector<int>(cols, -1));
    std::queue<std::pair<int,int>> q;
    dist[start_r][start_c] = 0;
    q.push({start_r, start_c});

    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        if (r == end_r && c == end_c) break;
        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k];
            int nc = c + dc[k];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && dist[nr][nc] == -1) {
                char ch = maze[nr][nc];
                if (ch == '0' || ch == 'x') {
                    dist[nr][nc] = dist[r][c] + 1;
                    q.push({nr, nc});
                }
            }
        }
    }

    if (dist[end_r][end_c] == -1) return maze; // no path

    // Reconstruct path
    std::vector<std::string> result = maze;
    int r = end_r, c = end_c;
    while (!(r == start_r && c == start_c)) {
        result[r][c] = '.';
        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k];
            int nc = c + dc[k];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && dist[nr][nc] == dist[r][c] - 1) {
                r = nr;
                c = nc;
                break;
            }
        }
    }
    result[start_r][start_c] = '.';
    return result;
}

// Use Breadth-First Search (BFS) from the entry cell to explore the maze level by level, guaranteeing the shortest path in terms of number of steps because all edges have equal weight. Maintain a queue of positions and a 2D visited array (or distance array) initialized to false. While dequeuing a cell, check if it is the exit; if so, reconstruct the path by backtracking from the exit to the entry using a parent pointer or distance values. Mark each cell on the reconstructed path with `'.'`. If the queue becomes empty without reaching the exit, no path exists, so return a copy of the original maze. Important edge cases: entry equal to exit (path length 0, mark only that cell), walls blocking all routes, and maze dimensions up to moderate sizes. Time complexity is O(R*C) for BFS plus O(path length) for reconstruction, which is at most O(R*C); space complexity is O(R*C) for visited/parent storage and the queue.
