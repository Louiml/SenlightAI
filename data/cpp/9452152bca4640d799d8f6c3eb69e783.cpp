Write a C++ function `std::vector<std::vector<char>> surroundRegions(std::vector<std::vector<char>> board)` that takes a 2D rectangular grid of characters, where each cell is either `'O'` (open) or `'X'` (blocked), and returns a new board where every `'O'` that is not connected to the border (via a path of adjacent `'O'` cells in the four cardinal directions) is replaced with `'X'`. Cells connected to the border remain `'O'`. The function must not modify the input board (pass by value or make a copy) and must handle empty boards, single-row/column boards, and all cells being `'X'` or `'O'`. The grid is guaranteed to be rectangular (all rows have the same length). The solution should be efficient for large grids.

// The standard approach is to identify all `'O'` cells that are reachable from the border, because those must remain `'O'`. We can perform a multi-source Breadth-First Search (BFS) starting from every border cell that contains `'O'`. We temporarily mark these cells as `'V'` (visited) to avoid revisiting them. Then we traverse neighbors in four directions; whenever we find an unvisited `'O'`, we mark it as `'V'` and push it into the queue. After BFS completes, we iterate through the entire grid: any cell that is `'V'` is safe and becomes `'O'`, any other cell (including original `'X'` and unvisited `'O'`) becomes `'X'`. This correctly captures that interior `'O'` regions not connected to the border are surrounded and should be flipped. Edge cases include an empty board (return immediately), a board with only one row or column (all `'O'`s are on the border and remain), and a board that is entirely `'O'` (all remain), or entirely `'X'` (nothing changes). Time complexity is O(R*C) where R is rows and C is columns because each cell is visited at most once during BFS, plus a final scan. Space complexity is O(R*C) in the worst case for the queue (e.g., entire border is `'O'` and floods the whole grid), plus a constant for directions.

#include <vector>
#include <queue>
#include <utility>

// Returns a new board where O's not connected to the border are flipped to X.
std::vector<std::vector<char>> surroundRegions(std::vector<std::vector<char>> board) {
    if (board.empty() || board[0].empty()) {
        return board;
    }

    const int rows = static_cast<int>(board.size());
    const int cols = static_cast<int>(board[0].size());

    // Queue for BFS starting from border 'O's.
    std::queue<std::pair<int, int>> q;

    // Mark all border 'O's as visited ('V') and enqueue them.
    for (int i = 0; i < rows; ++i) {
        if (board[i][0] == 'O') {
            board[i][0] = 'V';
            q.emplace(i, 0);
        }
        if (board[i][cols - 1] == 'O') {
            board[i][cols - 1] = 'V';
            q.emplace(i, cols - 1);
        }
    }
    for (int j = 0; j < cols; ++j) {
        if (board[0][j] == 'O') {
            board[0][j] = 'V';
            q.emplace(0, j);
        }
        if (board[rows - 1][j] == 'O') {
            board[rows - 1][j] = 'V';
            q.emplace(rows - 1, j);
        }
    }

    // Four cardinal directions.
    const std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    // BFS to find all 'O's connected to the border.
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        for (const auto& d : directions) {
            const int nr = r + d.first;
            const int nc = c + d.second;
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && board[nr][nc] == 'O') {
                board[nr][nc] = 'V';
                q.emplace(nr, nc);
            }
        }
    }

    // Final pass: 'V' -> 'O', everything else -> 'X'.
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (board[i][j] == 'V') {
                board[i][j] = 'O';
            } else {
                board[i][j] = 'X';
            }
        }
    }

    return board;
}

#include <cassert>
#include <vector>

int main() {
    // Empty board
    std::vector<std::vector<char>> empty;
    assert(surroundRegions(empty) == empty);

    // Single row, all border O's stay
    std::vector<std::vector<char>> single_row = {{'O', 'X', 'O'}};
    assert(surroundRegions(single_row) == single_row);

    // Single column, all border O's stay
    std::vector<std::vector<char>> single_col = {{'O'}, {'X'}, {'O'}};
    assert(surroundRegions(single_col) == single_col);

    // Basic case with interior O's flipped
    std::vector<std::vector<char>> board1 = {
        {'X', 'X', 'X', 'X'},
        {'X', 'O', 'O', 'X'},
        {'X', 'X', 'O', 'X'},
        {'X', 'O', 'X', 'X'}
    };
    std::vector<std::vector<char>> expected1 = {
        {'X', 'X', 'X', 'X'},
        {'X', 'X', 'X', 'X'},
        {'X', 'X', 'X', 'X'},
        {'X', 'O', 'X', 'X'}
    };
    assert(surroundRegions(board1) == expected1);

    // All O's on border and connected, none flipped
    std::vector<std::vector<char>> board2 = {
        {'O', 'O', 'O'},
        {'O', 'X', 'O'},
        {'O', 'O', 'O'}
    };
    assert(surroundRegions(board2) == board2);

    // All X's unchanged
    std::vector<std::vector<char>> board3 = {{'X', 'X'}, {'X', 'X'}};
    assert(surroundRegions(board3) == board3);

    // Large connected region from border, no flip
    std::vector<std::vector<char>> board4 = {
        {'O', 'O', 'X'},
        {'O', 'X', 'O'},
        {'X', 'O', 'O'}
    };
    assert(surroundRegions(board4) == board4);

    // Border O connected to interior, all remain
    std::vector<std::vector<char>> board5 = {
        {'X', 'O', 'X'},
        {'O', 'O', 'X'},
        {'X', 'X', 'X'}
    };
    assert(surroundRegions(board5) == board5);
}
