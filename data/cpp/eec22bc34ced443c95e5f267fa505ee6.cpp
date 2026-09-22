Write a C++ function `void applyGameOfLife(std::vector<std::vector<int>>& board)` that updates a 2D grid representing a population of cells in-place according to Conway's Game of Life rules. Each cell is either alive (1) or dead (0). The next state for each cell is determined by counting its 8 live neighbors (cells sharing an edge or corner): a live cell survives if it has exactly 2 or 3 live neighbors, otherwise it dies; a dead cell becomes alive if it has exactly 3 live neighbors. Crucially, all updates must be applied simultaneously using the original board state, meaning you cannot modify a cell before using it to evaluate its neighbors. The board is always non-empty (at least 1 row and 1 column). The function must alter the input vector in-place and return `void`.
// The main challenge is computing the next generation without accidentally using already-updated values from the same generation. A common technique is to encode both the original and new state in a single integer using temporary markers. In this approach, we iterate through every cell, count live neighbors by checking `abs(board[r][c]) == 1` (which is true for original live cells marked as 1 or -1). While counting, we ignore the current cell itself. After counting, if the cell is alive and its neighbor count is not 2 or 3, we mark it as `-1` (meaning originally alive but will die). If the cell is dead and has exactly 3 live neighbors, we mark it as `2` (meaning originally dead but will become alive). After processing all cells, a second pass converts `-1` to `0` and `2` to `1`, leaving other values unchanged. This avoids the need for a copy. Edge cases include cells on the border (must check row/column bounds), boards with a single row or column, and when a cell has 0 neighbors or all 8 neighbors. Time complexity is O(rows × cols) because each cell is visited a constant number of times (once for neighbor counting and once for final update). Space complexity is O(1) auxiliary, ignoring the input board itself.
#include <vector>
#include <cstdlib> // for std::abs

// Updates the board in-place according to Conway's Game of Life rules.
// Uses temporary markers: -1 = was alive, now dead; 2 = was dead, now alive.
void applyGameOfLife(std::vector<std::vector<int>>& board) {
    const int rows = static_cast<int>(board.size());
    const int cols = static_cast<int>(board[0].size());
    const int deltas[] = {-1, 0, 1};

    // First pass: compute neighbor counts and mark transitions.
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int liveNeighbors = 0;
            for (int dr : deltas) {
                for (int dc : deltas) {
                    if (dr == 0 && dc == 0) continue; // skip the cell itself
                    int nr = r + dr;
                    int nc = c + dc;
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                        // abs(board[nr][nc]) == 1 means it was alive originally
                        if (std::abs(board[nr][nc]) == 1) {
                            ++liveNeighbors;
                        }
                    }
                }
            }
            if (board[r][c] == 1 && (liveNeighbors < 2 || liveNeighbors > 3)) {
                board[r][c] = -1; // dies
            } else if (board[r][c] == 0 && liveNeighbors == 3) {
                board[r][c] = 2; // becomes alive
            }
            // Otherwise, keep as 0 or 1.
        }
    }

    // Second pass: finalize the board.
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (board[r][c] == -1) {
                board[r][c] = 0;
            } else if (board[r][c] == 2) {
                board[r][c] = 1;
            }
        }
    }
}
#include <cassert>
#include <vector>

// Declaration of the solution function (assume it's defined above or linked).
void applyGameOfLife(std::vector<std::vector<int>>& board);

int main() {
    // Test 1: Simple blinker (period 2 oscillator) – from horizontal to vertical.
    std::vector<std::vector<int>> board1 = {
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0}
    };
    std::vector<std::vector<int>> expected1 = {
        {0, 0, 0},
        {1, 1, 1},
        {0, 0, 0}
    };
    applyGameOfLife(board1);
    assert(board1 == expected1);

    // Test 2: Block (still life) – should remain unchanged.
    std::vector<std::vector<int>> board2 = {
        {1, 1},
        {1, 1}
    };
    std::vector<std::vector<int>> expected2 = {
        {1, 1},
        {1, 1}
    };
    applyGameOfLife(board2);
    assert(board2 == expected2);

    // Test 3: Single live cell dies (underpopulation).
    std::vector<std::vector<int>> board3 = {{1}};
    std::vector<std::vector<int>> expected3 = {{0}};
    applyGameOfLife(board3);
    assert(board3 == expected3);

    // Test 4: Single dead cell with no neighbors remains dead.
    std::vector<std::vector<int>> board4 = {{0}};
    std::vector<std::vector<int>> expected4 = {{0}};
    applyGameOfLife(board4);
    assert(board4 == expected4);

    // Test 5: 2x3 grid with a blinker's vertical phase – should become horizontal.
    std::vector<std::vector<int>> board5 = {
        {0, 1, 0},
        {0, 1, 0}
    };
    std::vector<std::vector<int>> expected5 = {
        {0, 0, 0},
        {1, 1, 1}
    };
    applyGameOfLife(board5);
    assert(board5 == expected5);

    // Test 6: All dead cells stay dead.
    std::vector<std::vector<int>> board6 = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    std::vector<std::vector<int>> expected6 = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    applyGameOfLife(board6);
    assert(board6 == expected6);

    // Test 7: Tub (still life) – unchanged.
    std::vector<std::vector<int>> board7 = {
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 0}
    };
    std::vector<std::vector<int>> expected7 = {
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 0}
    };
    applyGameOfLife(board7);
    assert(board7 == expected7);

    // Test 8: 1x5 row – all cells die because each has at most 1 neighbor.
    std::vector<std::vector<int>> board8 = {{1, 1, 1, 1, 1}};
    std::vector<std::vector<int>> expected8 = {{0, 0, 0, 0, 0}};
    applyGameOfLife(board8);
    assert(board8 == expected8);

    return 0;
}
