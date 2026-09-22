Write a C++ function `int shortestPathDistance(int R, int C, int startY, int startX, int goalY, int goalX, const std::vector<std::vector<char>>& grid)` that, given a grid of characters where `.` represent open cells and `#` represent walls, returns the minimum number of steps needed to move from the start cell to the goal cell using 4-directional movement (up, down, left, right). The start and goal coordinates are given in 0-based indices. If the goal is unreachable, return `-1`. The function must treat cells outside the grid as invalid, must not step into walls, and must handle cases where the start equals the goal (return 0). The grid dimensions are at least 1×1. The function should not modify the input grid.

The problem is a classic unweighted shortest-path problem on a grid, solved efficiently using Breadth-First Search (BFS). Since all moves have the same cost (1 step), BFS guarantees that the first time we reach the goal, the distance is minimal.  
We maintain a queue of cells to visit, initialized with the start cell. We also maintain a distance matrix (`dist`) initialized to -1 (meaning unvisited). We set `dist[startY][startX] = 0`. While the queue is not empty, we pop the front cell, check if it's the goal (if so, return its distance), and for each of the four neighbors (up, down, left, right), we check if the neighbor is inside the grid, is an open cell (`.`), and is unvisited. If so, we set its distance to current distance + 1 and push it onto the queue.  
Edge cases:  
- If start is equal to goal, BFS will pop the start and immediately return 0.  
- If the goal is surrounded by walls or unreachable, the queue will empty without ever reaching the goal, so we return -1.  
- Coordinates are assumed valid (inside grid) for start and goal; but the function should still check bounds for neighbors.  
Time complexity: O(R * C) because each cell is processed at most once. Space complexity: O(R * C) for the distance matrix and the queue (which may hold up to O(R*C) cells).

#include <vector>
#include <deque>
#include <utility>

// Returns the shortest path distance from start to goal in a grid where '.' is open and '#' is wall.
// Coordinates are 0-based. Returns -1 if unreachable.
int shortestPathDistance(int R, int C, int startY, int startX, int goalY, int goalX,
                         const std::vector<std::vector<char>>& grid) {
    // Distance matrix initialized to -1 (unvisited)
    std::vector<std::vector<int>> dist(R, std::vector<int>(C, -1));

    // Direction vectors for up, down, left, right
    const int dy[4] = {-1, 1, 0, 0};
    const int dx[4] = {0, 0, -1, 1};

    // BFS queue of (y, x) pairs
    std::deque<std::pair<int, int>> q;
    q.push_back({startY, startX});
    dist[startY][startX] = 0;

    while (!q.empty()) {
        auto [y, x] = q.front();
        q.pop_front();

        // If we reached the goal, return its distance
        if (y == goalY && x == goalX) {
            return dist[y][x];
        }

        // Explore all 4 neighbors
        for (int i = 0; i < 4; ++i) {
            int ny = y + dy[i];
            int nx = x + dx[i];

            // Check bounds, open cell, and not visited
            if (ny >= 0 && ny < R && nx >= 0 && nx < C &&
                grid[ny][nx] == '.' && dist[ny][nx] == -1) {
                dist[ny][nx] = dist[y][x] + 1;
                q.push_back({ny, nx});
            }
        }
    }

    // Goal not reached
    return -1;
}

#include <cassert>
#include <vector>

// Declare the function (provided in solution)
int shortestPathDistance(int R, int C, int startY, int startX, int goalY, int goalX,
                         const std::vector<std::vector<char>>& grid);

int main() {
    // Test 1: Simple open grid 3x3, start (0,0), goal (2,2)
    {
        std::vector<std::vector<char>> grid = {
            {'.', '.', '.'},
            {'.', '.', '.'},
            {'.', '.', '.'}
        };
        assert(shortestPathDistance(3, 3, 0, 0, 2, 2, grid) == 4);
    }

    // Test 2: Start equals goal
    {
        std::vector<std::vector<char>> grid = {
            {'.', '#'},
            {'#', '.'}
        };
        assert(shortestPathDistance(2, 2, 0, 0, 0, 0, grid) == 0);
    }

    // Test 3: Goal blocked by walls, unreachable
    {
        std::vector<std::vector<char>> grid = {
            {'.', '#', '.'},
            {'.', '#', '.'},
            {'.', '#', '.'}
        };
        assert(shortestPathDistance(3, 3, 0, 0, 0, 2, grid) == -1);
    }

    // Test 4: Obstacle forces longer path
    {
        std::vector<std::vector<char>> grid = {
            {'.', '#', '.'},
            {'.', '#', '.'},
            {'.', '.', '.'}
        };
        // Path: (0,0)->(1,0)->(2,0)->(2,1)->(2,2)->(1,2)->(0,2) = 6 steps
        assert(shortestPathDistance(3, 3, 0, 0, 0, 2, grid) == 6);
    }

    // Test 5: 1x1 grid with start=goal
    {
        std::vector<std::vector<char>> grid = {{'.'}};
        assert(shortestPathDistance(1, 1, 0, 0, 0, 0, grid) == 0);
    }

    // Test 6: Grid with a single column, multiple rows
    {
        std::vector<std::vector<char>> grid = {
            {'.'},
            {'.'},
            {'.'}
        };
        assert(shortestPathDistance(3, 1, 0, 0, 2, 0, grid) == 2);
    }

    // Test 7: Larger grid with a wall maze
    {
        std::vector<std::vector<char>> grid = {
            {'.', '.', '#', '.', '.'},
            {'.', '#', '.', '#', '.'},
            {'.', '.', '.', '.', '.'},
            {'#', '#', '#', '#', '.'},
            {'.', '.', '.', '.', '.'}
        };
        // Shortest path from (0,0) to (4,4) goes around, distance 8
        assert(shortestPathDistance(5, 5, 0, 0, 4, 4, grid) == 8);
    }

    // Test 8: All walls except start and goal, no path
    {
        std::vector<std::vector<char>> grid = {
            {'.', '#', '#'},
            {'#', '#', '#'},
            {'#', '#', '.'}
        };
        assert(shortestPathDistance(3, 3, 0, 0, 2, 2, grid) == -1);
    }

    // Test 9: Direct adjacency
    {
        std::vector<std::vector<char>> grid = {
            {'.', '.'},
            {'.', '.'}
        };
        assert(shortestPathDistance(2, 2, 0, 0, 0, 1, grid) == 1);
        assert(shortestPathDistance(2, 2, 0, 0, 1, 1, grid) == 2);
    }

    // Test 10: Goal corner with walls near start
    {
        std::vector<std::vector<char>> grid = {
            {'.', '#', '.'},
            {'.', '#', '.'},
            {'.', '.', '.'}
        };
        // Start (0,0), goal (0,2), must go down and around: distance 6
        assert(shortestPathDistance(3, 3, 0, 0, 0, 2, grid) == 6);
    }

    return 0;
}
