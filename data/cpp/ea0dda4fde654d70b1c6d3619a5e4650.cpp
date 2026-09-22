// Write a C++ function `bool canStop(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination)` that determines whether a ball can stop at a given destination cell in a rectangular maze. The maze is represented as a 2D grid of integers: `0` represents an empty cell, `1` represents a wall. The ball starts at `start` and can roll in four cardinal directions (up, down, left, right) until it hits a wall or the boundary, at which point it stops at the last empty cell before the obstacle. The ball can only change direction after it has stopped. Return `true` if the ball can stop exactly at `destination` after any sequence of moves; otherwise return `false`. The maze will have at least one row and one column, `start` and `destination` are guaranteed to be empty cells (`0`), and `start` may equal `destination`.
The problem is equivalent to finding whether there is a path from `start` to `destination` in a graph where nodes are "stopping positions" (empty cells reachable after rolling until hitting a wall/boundary). A depth-first search (DFS) or breadth-first search (BFS) works. For each current cell, simulate rolling in each of the four directions: increment row/column in that direction while staying inside the grid and on empty cells. When the loop stops, the cell before the wall/boundary is the new stop position. If that position has not been visited, recurse (or push into queue). If we ever reach the destination, return true. Important edge cases: (1) The start may equal the destination — return true immediately. (2) The ball may stop at the same cell from multiple directions; we must mark visited to avoid infinite loops. (3) The maze boundaries act like walls, so the ball stops at the last valid cell before going out. Complexity: Let \(n\) be rows and \(m\) columns; each cell is visited at most once, and for each visited cell we simulate rolling in four directions, each requiring at most \(O(\max(n,m))\) steps. Thus time is \(O(nm \cdot \max(n,m))\) in the worst case (though typical BFS/DFS on this implicit graph is often considered \(O(nm)\) per direction). Space is \(O(nm)\) for the visited array and the recursion stack / queue.
#include <vector>
#include <queue>

// Returns true if the ball can stop at destination after rolling in the maze.
// maze: 0 = empty, 1 = wall. Start and destination are empty cells.
bool canStop(std::vector<std::vector<int>>& maze, std::vector<int>& start, std::vector<int>& destination) {
    if (maze.empty() || maze[0].empty()) return false;
    int rows = static_cast<int>(maze.size());
    int cols = static_cast<int>(maze[0].size());

    // BFS using queue of stopping positions
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::queue<std::pair<int,int>> q;
    q.push({start[0], start[1]});
    visited[start[0]][start[1]] = true;

    const int dr[4] = {-1, 0, 1, 0};
    const int dc[4] = {0, 1, 0, -1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        if (r == destination[0] && c == destination[1]) {
            return true;
        }

        for (int i = 0; i < 4; ++i) {
            int nr = r;
            int nc = c;
            // Roll until hitting a wall or boundary
            while (nr >= 0 && nr < rows && nc >= 0 && nc < cols && maze[nr][nc] == 0) {
                nr += dr[i];
                nc += dc[i];
            }
            // Step back to the last empty cell
            nr -= dr[i];
            nc -= dc[i];

            if (!visited[nr][nc]) {
                visited[nr][nc] = true;
                q.push({nr, nc});
            }
        }
    }
    return false;
}
#include <cassert>
#include <vector>

// The solution function is declared above.
int main() {
    // Example 1 from LeetCode
    std::vector<std::vector<int>> maze1 = {
        {0,0,1,0,0},
        {0,0,0,0,0},
        {0,0,0,1,0},
        {1,1,0,1,1},
        {0,0,0,0,0}
    };
    std::vector<int> start1 = {0,4};
    std::vector<int> dest1 = {4,4};
    assert(canStop(maze1, start1, dest1) == true);

    // Example 2: destination not reachable because it's in a corner blocked
    std::vector<int> dest2 = {3,2};
    assert(canStop(maze1, start1, dest2) == false);

    // Start equals destination
    std::vector<int> dest3 = {0,4};
    assert(canStop(maze1, start1, dest3) == true);

    // Single-cell maze
    std::vector<std::vector<int>> maze2 = {{0}};
    std::vector<int> s2 = {0,0};
    std::vector<int> d2 = {0,0};
    assert(canStop(maze2, s2, d2) == true);

    // Maze with a wall directly adjacent — ball cannot move
    std::vector<std::vector<int>> maze3 = {{0,1},{0,1}};
    std::vector<int> s3 = {0,0};
    std::vector<int> d3 = {1,0}; // reachable by rolling down
    assert(canStop(maze3, s3, d3) == true);

    // Unreachable because wall blocks passage
    std::vector<std::vector<int>> maze4 = {
        {0,1,0},
        {0,1,0},
        {0,0,0}
    };
    std::vector<int> s4 = {0,0};
    std::vector<int> d4 = {0,2};
    // Ball from (0,0) can only stop at (0,0) or (2,0), cannot reach (0,2)
    assert(canStop(maze4, s4, d4) == false);

    // Path requires a turn: start (0,0) to (2,2) in open 3x3 with walls at edges
    std::vector<std::vector<int>> maze5 = {
        {0,0,0},
        {0,1,0},
        {0,0,0}
    };
    std::vector<int> s5 = {0,0};
    std::vector<int> d5 = {2,2};
    // From (0,0) roll right stops at (0,2), roll down stops at (2,0), etc. Can reach (2,2) via (0,2)->(2,2) or (2,0)->(2,2)
    assert(canStop(maze5, s5, d5) == true);

    // No walls at all in a 2x2: start (0,0) to (1,1) reachable
    std::vector<std::vector<int>> maze6 = {{0,0},{0,0}};
    std::vector<int> s6 = {0,0};
    std::vector<int> d6 = {1,1};
    assert(canStop(maze6, s6, d6) == true);

    return 0;
}
