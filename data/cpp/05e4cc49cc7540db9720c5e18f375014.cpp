Write a standalone C++ function that uses a uniform-cost grid search (similar to Dijkstra's algorithm) to find the shortest path between two integer grid coordinates while avoiding obstacles. The function should accept a 2D boolean grid where `true` represents a traversable cell and `false` represents an obstacle, along with start and end coordinates. Movement is allowed only in the four cardinal directions (up, down, left, right). If a path exists, return a `std::vector<std::pair<int,int>>` containing the coordinates of the path from start to end (inclusive). If no path exists, return an empty vector. The path should minimize the total number of steps taken, and among paths with equal length, any valid shortest path is acceptable. The grid dimensions are given as `(rows, cols)`, and coordinates are provided as `(row, col)` pairs where `0 <= row < rows` and `0 <= col < cols`. The start and end are guaranteed to be within bounds and traversable. The function must be const-correct and handle edge cases such as start equal to end (returning a single-element vector) and grids where obstacles completely block the path. Your implementation should use a priority queue for efficiency and should not rely on any external libraries beyond the C++ standard library.

The solution uses a uniform-cost search (Dijkstra's algorithm) on an unweighted grid, which reduces to breadth-first search (BFS) in terms of optimality but can be implemented with a priority queue as specified. The algorithm initializes a distance map (or 2D vector) where all distances are set to infinity except the start cell, which is set to zero. A priority queue stores `(distance, row, col)` tuples, keyed by distance. The main loop pops the cell with the smallest distance. If this cell is the end, we reconstruct the path by tracing back through predecessor pointers stored in a separate 2D vector. For each of the four cardinal neighbors, if the neighbor is within bounds, traversable, and the new distance (current distance + 1) is less than the stored distance, we update the distance, set the predecessor, and push the neighbor into the queue. This ensures each cell is processed at most once in its final optimal state. After the search completes without finding the end, we return an empty vector. Edge cases include: start equal to end (the loop immediately returns a single-element path), obstacles blocking the path (queue empties without finding the end), and unreachable regions (implicitly handled by returning empty). Time complexity is \(O(R \cdot C \log(R \cdot C))\) due to the priority queue, where \(R\) is rows and \(C\) is columns. Space complexity is \(O(R \cdot C)\) for the distance, predecessor, and queue storage. Since the grid is unweighted, a simple BFS with a queue would achieve \(O(R \cdot C)\) time, but the task specifies a priority queue approach to mirror the provided snippet's style.

#include <vector>
#include <queue>
#include <utility>
#include <limits>
#include <algorithm>

// Find the shortest path on a grid from start to end (row, col) pairs.
// grid[r][c] is true if traversable, false if obstacle.
// Returns a vector of coordinates from start to end inclusive, or empty if no path.
std::vector<std::pair<int, int>> findShortestPath(
    const std::vector<std::vector<bool>>& grid,
    const std::pair<int, int>& start,
    const std::pair<int, int>& end)
{
    const int rows = static_cast<int>(grid.size());
    if (rows == 0) return {};
    const int cols = static_cast<int>(grid[0].size());

    // Distance matrix, initialize to infinity
    std::vector<std::vector<int>> dist(rows, std::vector<int>(cols, std::numeric_limits<int>::max()));
    // Predecessor matrix for path reconstruction: stores (prev_row, prev_col) or (-1,-1) for none
    std::vector<std::vector<std::pair<int, int>>> prev(rows, std::vector<std::pair<int, int>>(cols, {-1, -1}));

    // Priority queue: (distance, row, col). Using greater to get min-heap
    using QueueItem = std::tuple<int, int, int>;
    std::priority_queue<QueueItem, std::vector<QueueItem>, std::greater<QueueItem>> pq;

    const int startRow = start.first;
    const int startCol = start.second;
    const int endRow = end.first;
    const int endCol = end.second;

    dist[startRow][startCol] = 0;
    pq.emplace(0, startRow, startCol);

    // Direction vectors: up, down, left, right
    constexpr int dr[4] = {-1, 1, 0, 0};
    constexpr int dc[4] = {0, 0, -1, 1};

    while (!pq.empty()) {
        auto [currentDist, r, c] = pq.top();
        pq.pop();

        // Skip if this entry is outdated (larger distance than the known best)
        if (currentDist > dist[r][c]) continue;

        // Check if we reached the end
        if (r == endRow && c == endCol) {
            // Reconstruct path
            std::vector<std::pair<int, int>> path;
            int curR = endRow;
            int curC = endCol;
            while (!(curR == startRow && curC == startCol)) {
                path.emplace_back(curR, curC);
                auto [prevR, prevC] = prev[curR][curC];
                curR = prevR;
                curC = prevC;
            }
            path.emplace_back(startRow, startCol);
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Explore neighbors
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            if (!grid[nr][nc]) continue; // obstacle

            int newDist = currentDist + 1;
            if (newDist < dist[nr][nc]) {
                dist[nr][nc] = newDist;
                prev[nr][nc] = {r, c};
                pq.emplace(newDist, nr, nc);
            }
        }
    }

    // No path found
    return {};
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is provided above; tests below assume it is included.

int main() {
    using Point = std::pair<int, int>;

    // Test 1: Simple open grid
    {
        std::vector<std::vector<bool>> grid = {
            {true, true, true},
            {true, true, true},
            {true, true, true}
        };
        auto path = findShortestPath(grid, {0,0}, {2,2});
        assert(path.size() == 5); // (0,0)->(0,1)->(0,2)->(1,2)->(2,2)
        assert(path.front() == Point(0,0));
        assert(path.back() == Point(2,2));
        // Verify path is valid and each step is adjacent
        for (size_t i = 1; i < path.size(); ++i) {
            int dr = std::abs(path[i].first - path[i-1].first);
            int dc = std::abs(path[i].second - path[i-1].second);
            assert(dr + dc == 1);
        }
    }

    // Test 2: Start equals end
    {
        std::vector<std::vector<bool>> grid = {
            {true, false},
            {false, true}
        };
        auto path = findShortestPath(grid, {1,1}, {1,1});
        assert(path.size() == 1);
        assert(path[0] == Point(1,1));
    }

    // Test 3: Obstacle blocking path
    {
        std::vector<std::vector<bool>> grid = {
            {true, false, true},
            {true, false, true},
            {true, false, true}
        };
        auto path = findShortestPath(grid, {0,0}, {0,2});
        assert(path.empty());
    }

    // Test 4: Path around obstacle
    {
        std::vector<std::vector<bool>> grid = {
            {true, true, true},
            {true, false, true},
            {true, true, true}
        };
        auto path = findShortestPath(grid, {0,0}, {2,2});
        assert(!path.empty());
        assert(path.front() == Point(0,0));
        assert(path.back() == Point(2,2));
        // The shortest path should be length 5 (e.g., (0,0)->(1,0)->(2,0)->(2,1)->(2,2))
        assert(path.size() == 5);
    }

    // Test 5: Single row grid
    {
        std::vector<std::vector<bool>> grid = {
            {true, false, true, true}
        };
        auto path = findShortestPath(grid, {0,0}, {0,3});
        assert(path.empty()); // blocked by obstacle at (0,1)
        
        std::vector<std::vector<bool>> grid2 = {
            {true, true, true, true}
        };
        auto path2 = findShortestPath(grid2, {0,0}, {0,3});
        assert(path2.size() == 4);
        assert(path2.front() == Point(0,0));
        assert(path2.back() == Point(0,3));
    }

    // Test 6: Larger grid with winding path
    {
        std::vector<std::vector<bool>> grid = {
            {true, true, true, false, true},
            {false, false, true, false, true},
            {true, true, true, true, true},
            {true, false, false, false, true},
            {true, true, true, true, true}
        };
        auto path = findShortestPath(grid, {0,0}, {4,4});
        assert(!path.empty());
        assert(path.front() == Point(0,0));
        assert(path.back() == Point(4,4));
        // Verify path length is optimal: known from manual BFS it should be 9 steps (10 cells)
        assert(path.size() == 10);
    }

    // Test 7: Grid with all obstacles except start/end
    {
        std::vector<std::vector<bool>> grid = {
            {true, false, false},
            {false, false, false},
            {false, false, true}
        };
        auto path = findShortestPath(grid, {0,0}, {2,2});
        assert(path.empty());
    }

    return 0;
}
