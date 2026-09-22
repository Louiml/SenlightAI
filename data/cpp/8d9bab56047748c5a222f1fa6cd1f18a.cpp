// Write a C++ function `int biggestOnePiece(const std::vector<std::vector<int>>& grid)` that, given a square grid of size N (1 ≤ N ≤ 1000) containing only 0s and 1s, returns the size (count of 1s) of the largest connected component of 1s, where two 1s are considered connected if they share an edge (horizontally or vertically, not diagonally). The grid is guaranteed to be square, and values are only 0 or 1. The function should not modify the input grid and must handle edge cases such as a grid with no 1s (return 0) and grids where the largest piece is the entire grid. The function must be efficient enough for N up to 1000 within a 1-second time limit, so avoid redundant traversals and use iterative or recursive DFS/BFS with proper visited tracking.
// The problem reduces to finding the size of the largest connected component in a binary grid using 4-directional adjacency. The standard approach is to iterate over every cell; when a cell contains 1 and has not yet been visited, we perform a depth-first search (DFS) or breadth-first search (BFS) starting from that cell, counting all reachable 1s. Mark each visited 1 in a separate boolean visited grid to avoid re-counting and infinite loops. Track the maximum component size encountered. Since each cell is visited at most once (when it is a 1 and unvisited), the time complexity is O(N²) and space complexity is O(N²) for the visited array (plus recursion stack depth up to O(N²) in the worst case if using recursive DFS, which may be acceptable for N=1000 if the stack is large, but iterative BFS is safer to avoid stack overflow). Edge cases: grid of all 0s returns 0; grid of all 1s returns N²; single-cell grid returns 1 if it's a 1 else 0; diagonal 1s are not connected. The implementation should use `const` references for the input grid and avoid any global mutable state.
#include <vector>
#include <queue>
#include <algorithm>

// Returns the size of the largest connected component of 1s in a square grid.
// Connectivity is defined by sharing an edge (up, down, left, right).
int biggestOnePiece(const std::vector<std::vector<int>>& grid) {
    int n = grid.size();
    if (n == 0) return 0;
    
    std::vector<std::vector<bool>> visited(n, std::vector<bool>(n, false));
    int max_piece = 0;
    
    // Direction vectors for 4 neighbours
    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] == 1 && !visited[i][j]) {
                // BFS to count connected component
                std::queue<std::pair<int,int>> q;
                q.push({i, j});
                visited[i][j] = true;
                int count = 0;
                
                while (!q.empty()) {
                    auto [x, y] = q.front();
                    q.pop();
                    ++count;
                    
                    for (int dir = 0; dir < 4; ++dir) {
                        int nx = x + dx[dir];
                        int ny = y + dy[dir];
                        if (nx >= 0 && nx < n && ny >= 0 && ny < n &&
                            grid[nx][ny] == 1 && !visited[nx][ny]) {
                            visited[nx][ny] = true;
                            q.push({nx, ny});
                        }
                    }
                }
                max_piece = std::max(max_piece, count);
            }
        }
    }
    return max_piece;
}
#include <cassert>
#include <vector>

// The function to test (already defined above)
int biggestOnePiece(const std::vector<std::vector<int>>& grid);

int main() {
    // Test 1: Sample from problem
    std::vector<std::vector<int>> grid1 = {{1,1},{0,1}};
    assert(biggestOnePiece(grid1) == 3);

    // Test 2: All zeros
    std::vector<std::vector<int>> grid2 = {{0,0,0},{0,0,0},{0,0,0}};
    assert(biggestOnePiece(grid2) == 0);

    // Test 3: All ones
    std::vector<std::vector<int>> grid3 = {{1,1,1},{1,1,1},{1,1,1}};
    assert(biggestOnePiece(grid3) == 9);

    // Test 4: Single cell
    std::vector<std::vector<int>> grid4 = {{0}};
    assert(biggestOnePiece(grid4) == 0);
    std::vector<std::vector<int>> grid5 = {{1}};
    assert(biggestOnePiece(grid5) == 1);

    // Test 5: Diagonal ones are not connected
    std::vector<std::vector<int>> grid6 = {{1,0},{0,1}};
    assert(biggestOnePiece(grid6) == 1);

    // Test 6: Multiple disconnected components
    std::vector<std::vector<int>> grid7 = {{1,1,0,0},{1,1,0,0},{0,0,1,1},{0,0,1,1}};
    assert(biggestOnePiece(grid7) == 4);

    // Test 7: Large component with a single isolated cell
    std::vector<std::vector<int>> grid8 = {{1,1,1,1},{1,0,1,1},{1,1,1,1},{0,0,0,1}};
    assert(biggestOnePiece(grid8) == 12);

    // Test 8: Spiral shape leading to a long chain
    std::vector<std::vector<int>> grid9 = {
        {1,1,1,0},
        {0,0,1,0},
        {1,1,1,0},
        {1,0,0,0}
    };
    // Largest piece: top row (3) + middle (1) + bottom-left vertical (3) + left-middle (1) = 8? Actually let's count:
    // Row0: (0,0),(0,1),(0,2) = 3; Row1: (1,2) = 1; Row2: (2,0),(2,1),(2,2) = 3; Row3: (3,0) = 1; total = 8. The isolated (0,3) is 0, so largest is 8.
    assert(biggestOnePiece(grid9) == 8);

    // Test 9: Constraint lower bound already tested; upper bound with moderate N (e.g., 10x10)
    int N = 10;
    std::vector<std::vector<int>> grid10(N, std::vector<int>(N, 1));
    assert(biggestOnePiece(grid10) == N*N);

    return 0;
}
