Write a C++ function named `findShortestPath` that implements the A* search algorithm on a simplified grid-based graph. The function takes as input a 2D vector of integers representing a map: `0` indicates a traversable cell and `1` indicates an obstacle. It also takes four integer coordinates: `start_row`, `start_col`, `end_row`, `end_col`, all in the range `[0, rows-1]` and `[0, cols-1]`. The function must return a `std::vector<std::pair<int,int>>` containing the sequence of grid coordinates (row, column pairs) from the start cell to the end cell, inclusive, representing the shortest path under Manhattan distance movement (up, down, left, right; diagonal moves are not allowed). If the start or end is on an obstacle, or no path exists, return an empty vector. If the start equals the end, return a vector containing just that single coordinate. The A* algorithm must use `g` cost as the number of steps taken from the start, `h` cost as the Manhattan distance to the end (i.e., `abs(r - end_row) + abs(c - end_col)`), and `f = g + h` for priority (since all edge costs are 1 and the heuristic is admissible and consistent, this will find the optimal shortest path). The search must expand nodes in increasing `f` order (ties can be broken arbitrarily, e.g., by smaller `h` then smaller `row` then smaller `col`). Track visited cells to avoid revisiting. Use a binary heap or priority queue for efficiency. No external libraries beyond the C++ standard library are allowed.
The solution uses the A* algorithm with a Manhattan heuristic, which is admissible (never overestimates the true cost) and consistent (the heuristic difference between adjacent nodes is at most the step cost of 1), guaranteeing optimality. We maintain a priority queue (min-heap) of candidate nodes keyed by `f = g + h`. Each node stores its coordinates, `g`, and `h`. We also maintain a 2D `visited` array to ensure each cell is processed at most once (since with a consistent heuristic, the first time we pop a node from the priority queue, we have found its optimal `g`). When we pop a node and it is the target, we reconstruct the path by tracing a stored `parent` map (e.g., a 2D vector of pairs initialized to `{-1,-1}`) from the end back to the start, then reverse it. Edge cases: if the start or end is outside the grid (though given as valid per problem statement), or if the start or end is an obstacle, return empty. If the start equals the end, return a single-element vector immediately. When expanding, check all four directions; skip if out of bounds or obstacle or already visited. If a neighbor is not visited, set its parent, compute `g = current.g + 1`, `h = Manhattan`, and push into the priority queue. If no path is found after exhausting the queue, return empty. Time complexity is `O(R * C * log(R * C))` in the worst case because each cell may be pushed into the heap at most once (due to visited check) and each heap operation is logarithmic. Space complexity is `O(R * C)` for visited, parent, and heap storage.
#include <vector>
#include <queue>
#include <cmath>
#include <utility>
#include <algorithm>

// A* shortest path on a grid with obstacles.
// Returns vector of (row, col) pairs from start to end inclusive.
std::vector<std::pair<int,int>> findShortestPath(
    const std::vector<std::vector<int>>& grid,
    int start_row, int start_col,
    int end_row, int end_col) {
    
    int rows = static_cast<int>(grid.size());
    if (rows == 0) return {};
    int cols = static_cast<int>(grid[0].size());
    if (cols == 0) return {};

    // Validate start and end within bounds
    if (start_row < 0 || start_row >= rows || start_col < 0 || start_col >= cols ||
        end_row < 0 || end_row >= rows || end_col < 0 || end_col >= cols) {
        return {};
    }

    // If start or end is obstacle, no path
    if (grid[start_row][start_col] == 1 || grid[end_row][end_col] == 1) {
        return {};
    }

    // If start equals end, return single node
    if (start_row == end_row && start_col == end_col) {
        return {{start_row, start_col}};
    }

    // Manhattan heuristic
    auto heuristic = [&](int r, int c) {
        return std::abs(r - end_row) + std::abs(c - end_col);
    };

    // Priority queue: (f, g, h, row, col)
    // Using a custom comparator to allow tie-breaking (smaller h, then smaller row, then smaller col)
    using Node = std::tuple<int, int, int, int, int>; // f, g, h, row, col
    auto cmp = [](const Node& a, const Node& b) {
        if (std::get<0>(a) != std::get<0>(b)) return std::get<0>(a) > std::get<0>(b); // min f
        if (std::get<2>(a) != std::get<2>(b)) return std::get<2>(a) > std::get<2>(b); // min h
        if (std::get<3>(a) != std::get<3>(b)) return std::get<3>(a) > std::get<3>(b); // min row
        return std::get<4>(a) > std::get<4>(b); // min col
    };
    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> pq(cmp);

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<std::pair<int,int>>> parent(rows, std::vector<std::pair<int,int>>(cols, {-1, -1}));

    int start_g = 0;
    int start_h = heuristic(start_row, start_col);
    pq.emplace(start_g + start_h, start_g, start_h, start_row, start_col);
    visited[start_row][start_col] = true;

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!pq.empty()) {
        auto [f, g, h, r, c] = pq.top();
        pq.pop();

        if (r == end_row && c == end_col) {
            // Reconstruct path
            std::vector<std::pair<int,int>> path;
            int cr = r, cc = c;
            while (!(cr == start_row && cc == start_col)) {
                path.emplace_back(cr, cc);
                auto [pr, pc] = parent[cr][cc];
                cr = pr;
                cc = pc;
            }
            path.emplace_back(start_row, start_col);
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                grid[nr][nc] == 0 && !visited[nr][nc]) {
                visited[nr][nc] = true;
                parent[nr][nc] = {r, c};
                int ng = g + 1;
                int nh = heuristic(nr, nc);
                pq.emplace(ng + nh, ng, nh, nr, nc);
            }
        }
    }

    return {}; // No path found
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be included above.

int main() {
    // Test 1: Simple straight line path
    {
        std::vector<std::vector<int>> grid = {
            {0, 0, 0},
            {1, 1, 0},
            {0, 0, 0}
        };
        auto path = findShortestPath(grid, 0, 0, 2, 2);
        std::vector<std::pair<int,int>> expected = {{0,0}, {0,1}, {0,2}, {1,2}, {2,2}};
        assert(path == expected);
    }

    // Test 2: Start equals end
    {
        std::vector<std::vector<int>> grid = {
            {0, 1},
            {0, 0}
        };
        auto path = findShortestPath(grid, 1, 1, 1, 1);
        assert(path.size() == 1 && path[0] == std::make_pair(1,1));
    }

    // Test 3: No path due to obstacles
    {
        std::vector<std::vector<int>> grid = {
            {0, 1},
            {1, 0}
        };
        auto path = findShortestPath(grid, 0, 0, 1, 1);
        assert(path.empty());
    }

    // Test 4: Start or end on obstacle
    {
        std::vector<std::vector<int>> grid = {
            {0, 1},
            {0, 0}
        };
        auto path = findShortestPath(grid, 0, 1, 1, 1); // start on obstacle
        assert(path.empty());
    }

    // Test 5: Path requires going around an obstacle
    {
        std::vector<std::vector<int>> grid = {
            {0, 0, 1, 0},
            {1, 0, 1, 0},
            {1, 0, 0, 0},
            {1, 1, 1, 0}
        };
        auto path = findShortestPath(grid, 0, 0, 3, 3);
        // Verify path length (7 steps: (0,0)->(1,0)->(1,1)->(2,1)->(2,2)->(2,3)->(3,3))
        assert(path.size() == 7);
        assert(path.front() == std::make_pair(0,0));
        assert(path.back() == std::make_pair(3,3));
    }

    // Test 6: Single cell grid
    {
        std::vector<std::vector<int>> grid = {{0}};
        auto path = findShortestPath(grid, 0, 0, 0, 0);
        assert(path == std::vector<std::pair<int,int>>{{0,0}});
    }

    // Test 7: Larger grid, ensure shortest path (Manhattan distance = 4)
    {
        std::vector<std::vector<int>> grid(5, std::vector<int>(5, 0));
        auto path = findShortestPath(grid, 0, 0, 4, 4);
        assert(path.size() == 9); // 8 steps + 1 = 9 nodes
    }

    // Test 8: Out of bounds coordinates
    {
        std::vector<std::vector<int>> grid = {{0,0},{0,0}};
        auto path = findShortestPath(grid, -1, 0, 1, 1);
        assert(path.empty());
    }

    return 0;
}
