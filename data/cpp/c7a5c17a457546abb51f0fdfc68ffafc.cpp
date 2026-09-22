// Write a C++ function `void captureSurroundedRegions(std::vector<std::vector<char>>& board)` that takes an `m x n` matrix of characters `'X'` and `'O'` and modifies it in place by flipping all `'O'` cells that are fully surrounded by `'X'` (i.e., not connected to any `'O'` on the border via horizontal or vertical adjacency) into `'X'`. Cells on the border or connected to a border `'O'` must remain `'O'`. The input matrix dimensions are at least 1×1. Your solution must handle edge cases such as empty board (0×0), single-cell boards, all-border `'O'` cells, and boards with no `'O'` cells. Do not modify cells outside the matrix. The function must not use recursion (to avoid stack overflow on large inputs); use an iterative approach with an explicit stack or queue.
#include <cassert>
#include <vector>

// Note: The solution function is declared above; includes already in solution block.
int main() {
    // Test 1: Standard example from problem.
    {
        std::vector<std::vector<char>> board = {
            {'X','X','X','X'},
            {'X','O','O','X'},
            {'X','X','O','X'},
            {'X','O','X','X'}
        };
        captureSurroundedRegions(board);
        std::vector<std::vector<char>> expected = {
            {'X','X','X','X'},
            {'X','X','X','X'},
            {'X','X','X','X'},
            {'X','O','X','X'}
        };
        assert(board == expected);
    }
    
    // Test 2: All O's, but small board (2x2) - all touch border, none flipped.
    {
        std::vector<std::vector<char>> board = {
            {'O','O'},
            {'O','O'}
        };
        captureSurroundedRegions(board);
        std::vector<std::vector<char>> expected = {
            {'O','O'},
            {'O','O'}
        };
        assert(board == expected);
    }
    
    // Test 3: All X's, nothing to change.
    {
        std::vector<std::vector<char>> board = {
            {'X','X'},
            {'X','X'}
        };
        captureSurroundedRegions(board);
        std::vector<std::vector<char>> expected = {
            {'X','X'},
            {'X','X'}
        };
        assert(board == expected);
    }
    
    // Test 4: Single row (1x5) - no surrounded region.
    {
        std::vector<std::vector<char>> board = {
            {'X','O','X','O','X'}
        };
        captureSurroundedRegions(board);
        std::vector<std::vector<char>> expected = {
            {'X','O','X','O','X'}
        };
        assert(board == expected);
    }
    
    // Test 5: Single column (5x1) - no surrounded region.
    {
        std::vector<std::vector<char>> board = {
            {'X'}, {'O'}, {'X'}, {'O'}, {'X'}
        };
        captureSurroundedRegions(board);
        std::vector<std::vector<char>> expected = {
            {'X'}, {'O'}, {'X'}, {'O'}, {'X'}
        };
        assert(board == expected);
    }
    
    // Test 6: Empty board.
    {
        std::vector<std::vector<char>> board;
        captureSurroundedRegions(board);
        assert(board.empty());
    }
    
    // Test 7: Large connected border O region reaching inside.
    {
        std::vector<std::vector<char>> board = {
            {'O','X','X','X'},
            {'O','O','X','X'},
            {'X','O','O','X'},
            {'X','X','O','O'}
        };
        captureSurroundedRegions(board);
        // All O's connected to the border, so none flipped.
        std::vector<std::vector<char>> expected = {
            {'O','X','X','X'},
            {'O','O','X','X'},
            {'X','O','O','X'},
            {'X','X','O','O'}
        };
        assert(board == expected);
    }
    
    // Test 8: Fully surrounded island in center.
    {
        std::vector<std::vector<char>> board = {
            {'X','X','X'},
            {'X','O','X'},
            {'X','X','X'}
        };
        captureSurroundedRegions(board);
        std::vector<std::vector<char>> expected = {
            {'X','X','X'},
            {'X','X','X'},
            {'X','X','X'}
        };
        assert(board == expected);
    }
    
    // Test 9: Multiple surrounded islands.
    {
        std::vector<std::vector<char>> board = {
            {'X','X','X','X','X'},
            {'X','O','X','O','X'},
            {'X','X','X','X','X'},
            {'X','O','X','O','X'},
            {'X','X','X','X','X'}
        };
        captureSurroundedRegions(board);
        std::vector<std::vector<char>> expected = {
            {'X','X','X','X','X'},
            {'X','X','X','X','X'},
            {'X','X','X','X','X'},
            {'X','X','X','X','X'},
            {'X','X','X','X','X'}
        };
        assert(board == expected);
    }
    
    // Test 10: Border O connected to interior O that is not fully enclosed.
    {
        std::vector<std::vector<char>> board = {
            {'X','X','X','O'},
            {'X','O','X','O'},
            {'X','O','O','O'},
            {'X','X','X','X'}
        };
        captureSurroundedRegions(board);
        std::vector<std::vector<char>> expected = {
            {'X','X','X','O'},
            {'X','O','X','O'},
            {'X','O','O','O'},
            {'X','X','X','X'}
        };
        assert(board == expected);
    }
    
    return 0;
}
#include <vector>
#include <stack>
#include <utility>

// Captures all 'O' regions fully surrounded by 'X' in place.
void captureSurroundedRegions(std::vector<std::vector<char>>& board) {
    if (board.empty() || board[0].empty()) return; // empty board
    
    const int rows = static_cast<int>(board.size());
    const int cols = static_cast<int>(board[0].size());
    
    // If board is too small, no cell can be surrounded (all touch border).
    if (rows < 3 || cols < 3) return;
    
    // Direction vectors for up, down, left, right.
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    
    auto isInside = [&](int r, int c) {
        return r >= 0 && r < rows && c >= 0 && c < cols;
    };
    
    // Flood-fill from border 'O's using an explicit stack (no recursion).
    std::stack<std::pair<int, int>> st;
    
    // Helper lambda to start traversal from a cell if it is 'O'.
    auto tryStart = [&](int r, int c) {
        if (board[r][c] == 'O') {
            board[r][c] = 'S'; // mark as safe (connected to border)
            st.push({r, c});
        }
    };
    
    // Add all border 'O' cells to the stack.
    for (int i = 0; i < rows; ++i) {
        tryStart(i, 0);
        tryStart(i, cols - 1);
    }
    for (int j = 0; j < cols; ++j) {
        tryStart(0, j);
        tryStart(rows - 1, j);
    }
    
    // Process stack: mark all connected 'O' as 'S'.
    while (!st.empty()) {
        auto [r, c] = st.top();
        st.pop();
        for (int dir = 0; dir < 4; ++dir) {
            int nr = r + dr[dir];
            int nc = c + dc[dir];
            if (isInside(nr, nc) && board[nr][nc] == 'O') {
                board[nr][nc] = 'S';
                st.push({nr, nc});
            }
        }
    }
    
    // Final conversion: 'O' becomes 'X', 'S' reverts to 'O'.
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (board[r][c] == 'O') {
                board[r][c] = 'X';
            } else if (board[r][c] == 'S') {
                board[r][c] = 'O';
            }
        }
    }
}
// The algorithm works in three phases. First, identify all `'O'` cells that are connected to any border `'O'` via a flood-fill traversal (BFS or DFS using a stack/queue). Mark these "safe" cells with a temporary sentinel value (e.g., `'S'`) to distinguish them from surrounded `'O'` cells. Start the traversal from every `'O'` on the top, bottom, left, and right borders. For each such cell, push it onto a stack and process neighbors in all four directions, marking any unvisited `'O'` as `'S'`. Second, after the traversal completes, iterate over all cells: any cell still `'O'` is not connected to the border, so flip it to `'X'`; any cell marked `'S'` is restored back to `'O'`. Edge cases: if the board has fewer than 3 rows or columns, no `'O'` can be fully surrounded (since every cell touches a border), so the function returns immediately. An empty board is handled by checking row count first. Time complexity is O(m·n) because each cell is visited at most once during the flood fill and once during the final pass. Space complexity is O(m·n) in the worst case for the stack (e.g., a border of `'O'` cells that expands inward), but since we mark cells immediately upon pushing, the stack never holds duplicates.
