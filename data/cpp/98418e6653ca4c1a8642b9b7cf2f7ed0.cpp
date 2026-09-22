// Write a C++ function `int countReachableCells(int n, int m, const std::vector<std::pair<int,int>>& blockedCells, int startX, int startY)` that performs a breadth-first search on a grid of size `n` rows by `m` columns, starting from the given cell `(startX, startY)`, where cells are considered blocked if their coordinates appear in the `blockedCells` list (assume blocked cells are unique and within bounds). A cell is reachable if it is within the grid, not blocked, and not previously visited. The function must return the total number of reachable cells (including the start cell) using only four-directional movement (up, down, left, right). The grid coordinates range from `0` to `n-1` for rows and `0` to `m-1` for columns. The start cell is guaranteed to be unblocked. Handle edge cases where the grid may have zero rows or zero columns (return `0`), or the start cell is outside bounds (return `0`). The function must be efficient for grid sizes up to 500×500.

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Basic 3x3 grid with no blocked cells, start at center (1,1)
    assert(countReachableCells(3, 3, {}, 1, 1) == 9);

    // 3x3 grid with all but start blocked → only start
    std::vector<std::pair<int,int>> blockedAllButStart = {{0,0},{0,1},{0,2},{1,0},{1,2},{2,0},{2,1},{2,2}};
    assert(countReachableCells(3, 3, blockedAllButStart, 1, 1) == 1);

    // 4x4 grid with a wall of blocked cells splitting it
    // Block entire second row (row index 1), start at (0,0)
    std::vector<std::pair<int,int>> wall;
    for (int col = 0; col < 4; ++col) wall.push_back({1, col});
    // Top row has 4 cells, bottom rows (2,3) have 8 cells, but they are separated by wall.
    // Start at (0,0) → reachable only top row cells: (0,0),(0,1),(0,2),(0,3) = 4
    assert(countReachableCells(4, 4, wall, 0, 0) == 4);

    // Start out of bounds
    assert(countReachableCells(5, 5, {}, 10, 10) == 0);

    // Zero-sized grid
    assert(countReachableCells(0, 5, {}, 0, 0) == 0);
    assert(countReachableCells(5, 0, {}, 0, 0) == 0);

    // Single cell grid, unblocked
    assert(countReachableCells(1, 1, {}, 0, 0) == 1);

    // 2x2 grid with one blocked cell blocking part of a diagonal path
    // Block (0,1) and (1,0). Start at (0,0) → can go to (0,0),(1,1) via (1,1) but (1,1) is not blocked? Actually start (0,0) → neighbors: (0,1) blocked, (1,0) blocked, so only start.
    std::vector<std::pair<int,int>> diagonalBlock = {{0,1},{1,0}};
    assert(countReachableCells(2, 2, diagonalBlock, 0, 0) == 1);

    // 2x2 grid with no blocks → all 4 cells reachable from any start
    assert(countReachableCells(2, 2, {}, 0, 0) == 4);

    // Larger grid with a central blocked region
    // 5x5, block around center (2,2) making an L shape
    std::vector<std::pair<int,int>> Lblock = {{2,1},{2,2},{1,2}};
    // Start at (0,0) → can reach most cells except the blocked ones and those cut off? Actually BFS will fill everything except blocked cells because no complete containment.
    // Total cells = 25 - 3 = 22
    assert(countReachableCells(5, 5, Lblock, 0, 0) == 22);

    return 0;
}

#include <vector>
#include <queue>
#include <utility>
#include <unordered_set>

// Count reachable cells from a start position in a grid with blocked cells.
// Grid dimensions: n rows (0..n-1), m columns (0..m-1).
// Movement: 4-directional (up, down, left, right). Returns total reachable cells including start.
int countReachableCells(int n, int m, const std::vector<std::pair<int,int>>& blockedCells, int startX, int startY) {
    // Edge cases: empty grid or start out of bounds
    if (n <= 0 || m <= 0) return 0;
    if (startX < 0 || startX >= n || startY < 0 || startY >= m) return 0;

    // Store blocked cells in a hash set for O(1) lookup
    std::unordered_set<long long> blocked;
    for (const auto& cell : blockedCells) {
        // encode pair as a single 64-bit key
        blocked.insert(static_cast<long long>(cell.first) * 1000000LL + cell.second);
    }

    // Visited grid
    std::vector<std::vector<bool>> visited(n, std::vector<bool>(m, false));

    // Direction vectors: up, down, left, right
    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};

    std::queue<std::pair<int,int>> q;
    q.push({startX, startY});
    visited[startX][startY] = true;
    int reachableCount = 1; // start cell is reachable

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();

        for (int dir = 0; dir < 4; ++dir) {
            int nx = cx + dx[dir];
            int ny = cy + dy[dir];

            // Check bounds
            if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
            // Check if previously visited
            if (visited[nx][ny]) continue;
            // Check if blocked
            if (blocked.find(static_cast<long long>(nx) * 1000000LL + ny) != blocked.end()) continue;

            visited[nx][ny] = true;
            q.push({nx, ny});
            ++reachableCount;
        }
    }

    return reachableCount;
}

// The solution uses a standard BFS (breadth-first search) on an implicit grid. We maintain a 2D boolean visited array of size `n × m` (or use a set for blocked cells to avoid allocating a large board if many blocked cells, but the problem says grid size up to 500×500, so a 2D boolean array is fine). Initialization: check if start is within bounds; if not, return 0. Create a queue of pairs, mark the start as visited, and increment a counter. For each popped cell, examine its four neighbors. For each neighbor, check: (1) row index in `[0, n-1]`, (2) column index in `[0, m-1]`, (3) not visited, (4) not in the blocked set. If all pass, mark visited, push to queue, and increment counter. Important edge cases: `n == 0` or `m == 0` → return 0. The start cell being out of bounds → return 0. Also, note that the original code snippet had a bug in its bounds check (`if(nx > n && nx < 0 && ny > m && ny < 0)`); in our solution we correctly check `nx < 0 || nx >= n || ny < 0 || ny >= m`. Time complexity: O(n*m) because each cell is visited at most once, and we do constant work per neighbor. Space complexity: O(n*m) for the visited array, plus O(n*m) in the worst case for the queue.
