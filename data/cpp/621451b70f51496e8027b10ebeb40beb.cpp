Write a C++ function that processes a 2D grid of integers using a stack-based approach. The function should take as input the grid dimensions (rows and columns) followed by the grid values, and return a vector of integers representing the "mountain path" traversal. The traversal starts at the top-left cell and repeatedly moves to the adjacent cell (up, down, left, right) with the maximum value among unvisited neighbors, pushing that chosen value onto a stack. After visiting all cells, the function returns the stack contents from bottom to top. The grid is guaranteed to be non-empty (at least one cell), and all values are positive integers. If multiple neighbors have the same maximum, choose the one with the smallest row index, and if tied, the smallest column index.
// The algorithm uses a stack to record the order of visited values. It maintains a visited matrix to track which cells have been processed. Starting at (0,0), push its value onto the stack and mark it visited. Then repeatedly look at the four cardinal neighbors of the current cell. Filter out neighbors that are out of bounds or already visited. If no unvisited neighbor exists, stop (this should only happen after all cells are visited). Among valid neighbors, choose the one with the maximum value; if there is a tie, break by smallest row then smallest column. Move to that cell, push its value, mark it visited, and repeat. The function returns a vector built by popping all elements from the stack into a temporary vector, then reversing it (since stack pops from top, we need bottom-to-top order). Edge cases: a single-cell grid returns just that value; grids where the path gets stuck early (which cannot happen because the grid is finite and we always move to an unvisited neighbor until all are visited—assuming reachability; but the problem guarantees that from any cell, there exists at least one unvisited neighbor until all cells are visited? Actually, the traversal may get stuck if the current cell has no unvisited neighbors, but that only occurs when all cells are visited. Since we stop only when no move is possible, and we always move to an unvisited cell, the process must end after exactly rows*columns steps because each move marks one new cell. So the stack size will equal the number of cells. Time complexity is O(rows*columns*4) = O(R*C) because each cell is processed once and we check its four neighbors. Space complexity is O(R*C) for the visited matrix and the stack.
#include <vector>
#include <stack>
#include <cstddef>

// Returns the values visited in order from start to end using stack-based mountain path traversal.
std::vector<int> mountainPath(int rows, int cols, const std::vector<int>& grid) {
    // Convert flat grid to 2D for easier access
    std::vector<std::vector<int>> mat(rows, std::vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            mat[i][j] = grid[i * cols + j];
        }
    }

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::stack<int> data;

    // Start at (0,0)
    int curR = 0, curC = 0;
    visited[curR][curC] = true;
    data.push(mat[curR][curC]);

    // Direction vectors: up, down, left, right
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (true) {
        int bestVal = -1;
        int bestR = -1, bestC = -1;

        // Find the best unvisited neighbor
        for (int d = 0; d < 4; ++d) {
            int nr = curR + dr[d];
            int nc = curC + dc[d];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !visited[nr][nc]) {
                // If better value, or same value but smaller row, or same row smaller col
                if (mat[nr][nc] > bestVal ||
                    (mat[nr][nc] == bestVal && (nr < bestR || (nr == bestR && nc < bestC)))) {
                    bestVal = mat[nr][nc];
                    bestR = nr;
                    bestC = nc;
                }
            }
        }

        if (bestR == -1) {
            // No unvisited neighbor; should only happen when all visited
            break;
        }

        // Move to best neighbor
        curR = bestR;
        curC = bestC;
        visited[curR][curC] = true;
        data.push(mat[curR][curC]);
    }

    // Extract from stack bottom to top
    std::vector<int> result;
    result.reserve(data.size());
    while (!data.empty()) {
        result.push_back(data.top());
        data.pop();
    }
    std::reverse(result.begin(), result.end());
    return result;
}
#include <cassert>
#include <vector>
#include <iostream>

// The solution function is declared here (as if included from solution)
// Assume the function definition above is present.

int main() {
    // Test 1: Single cell
    std::vector<int> g1 = {5};
    assert(mountainPath(1, 1, g1) == std::vector<int>({5}));

    // Test 2: 1x2 grid, path must go to the next cell (only one neighbor)
    std::vector<int> g2 = {3, 8};
    assert(mountainPath(1, 2, g2) == std::vector<int>({3, 8}));

    // Test 3: 2x2 grid with clear maximum path
    // Start (0,0)=10. Neighbors: (0,1)=20, (1,0)=5 -> choose 20
    // Then (0,1)=20. Neighbors: (0,0) visited, (1,1)=15, (0,2) invalid, (-1) invalid -> choose 15
    // Then (1,1)=15. Neighbors: (1,0)=5 -> choose 5.
    std::vector<int> g3 = {10, 20, 5, 15};
    assert(mountainPath(2, 2, g3) == std::vector<int>({10, 20, 15, 5}));

    // Test 4: 2x2 with tie-breaking (same max neighbor values)
    // Start (0,0)=1. Neighbors: (0,1)=9 and (1,0)=9. Tie: smaller row (1) vs (0)? Actually (1,0) has row 1 > row 0, so choose (0,1). Then from (0,1) go to (1,1).
    std::vector<int> g4 = {1, 9, 9, 7};
    assert(mountainPath(2, 2, g4) == std::vector<int>({1, 9, 7, 9}));

    // Test 5: 3x3 grid, ensure all cells visited (stack size equals total)
    std::vector<int> g5 = {2, 1, 3, 4, 9, 8, 7, 6, 5};
    auto res5 = mountainPath(3, 3, g5);
    assert(res5.size() == 9);
    assert(res5[0] == 2); // start value
    // Check that all values appear exactly once (simple set check)
    std::vector<int> sorted_res = res5;
    std::sort(sorted_res.begin(), sorted_res.end());
    std::vector<int> expected_sorted = {1,2,3,4,5,6,7,8,9};
    assert(sorted_res == expected_sorted);

    // Test 6: 2x3 grid, larger, test path correctness manually
    // Start (0,0)=1. Neighbors: (0,1)=5, (1,0)=8 -> choose 8 (since 8>5)
    // Then (1,0)=8. Neighbors: (0,0) visited, (1,1)=6, (2,0) invalid -> choose 6
    // Then (1,1)=6. Neighbors: (0,1)=5, (1,2)=7, (2,1) invalid, (2,0) invalid -> choose 7 (7>5)
    // Then (1,2)=7. Neighbors: (0,2)=3, (1,1) visited -> choose 3
    // Then (0,2)=3. Neighbors: (0,1)=5 -> choose 5
    // Then (0,1)=5. All visited.
    std::vector<int> g6 = {1,5,3, 8,6,7};
    assert(mountainPath(2, 3, g6) == std::vector<int>({1,8,6,7,3,5}));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
