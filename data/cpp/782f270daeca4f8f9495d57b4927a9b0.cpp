/*
Write a C++ function `candyCrush` that simulates the classic "Candy Crush" elimination process on a 2D board of non-negative integers. The function takes a mutable reference to a `vector<vector<int>>` board and repeatedly applies the following rules until no changes occur: (1) Identify all horizontal or vertical runs of 3 or more identical non-zero values; (2) Mark all cells in those runs as "crushed" (to be removed); (3) After marking all such cells in the entire board, remove all crushed cells by shifting remaining non-zero values in each column downward (toward larger row indices) to fill the empty spaces, and fill the vacated top cells with zeros. The process repeats until a full pass finds no crushed cells. The board dimensions are at least 1×1. Return the final stable board by modifying the input reference in place. The function should be a standalone free function named `candyCrush` that takes `std::vector<std::vector<int>>& board` and returns `void` (or returns the board by reference, but the task expects modification in place). Ensure the solution is efficient for boards up to 50×50.
*/
#include <vector>

void candyCrush(std::vector<std::vector<int>>& board) {
    if (board.empty() || board[0].empty()) return;

    int n = (int)board.size();
    int m = (int)board[0].size();

    while (true) {
        // Mark cells that should be crushed
        std::vector<std::vector<bool>> crash(n, std::vector<bool>(m, false));
        bool anyCrush = false;

        // Horizontal runs
        for (int i = 0; i < n; ++i) {
            int start = 0;
            for (int j = 1; j <= m; ++j) {
                if (j < m && board[i][j] == board[i][start]) {
                    // continue run
                } else {
                    int length = j - start;
                    if (length >= 3 && board[i][start] != 0) {
                        for (int k = start; k < j; ++k) crash[i][k] = true;
                        anyCrush = true;
                    }
                    start = j;
                }
            }
        }

        // Vertical runs
        for (int j = 0; j < m; ++j) {
            int start = 0;
            for (int i = 1; i <= n; ++i) {
                if (i < n && board[i][j] == board[start][j]) {
                    // continue run
                } else {
                    int length = i - start;
                    if (length >= 3 && board[start][j] != 0) {
                        for (int k = start; k < i; ++k) crash[k][j] = true;
                        anyCrush = true;
                    }
                    start = i;
                }
            }
        }

        if (!anyCrush) break;

        // Compact each column downward
        for (int j = 0; j < m; ++j) {
            std::vector<int> kept;
            kept.reserve(n);
            for (int i = n - 1; i >= 0; --i) {
                if (!crash[i][j]) {
                    kept.push_back(board[i][j]);
                }
            }
            // Fill from bottom with kept values, rest with zeros
            int keptSize = (int)kept.size();
            for (int k = 0; k < keptSize; ++k) {
                board[n - 1 - k][j] = kept[k];
            }
            for (int k = keptSize; k < n; ++k) {
                board[n - 1 - k][j] = 0;
            }
        }
    }
}
#include <cassert>
#include <vector>

// Include the solution function here (omitted for brevity in test snippet)

int main() {
    // Basic horizontal crush
    {
        std::vector<std::vector<int>> board = {{1,1,1}};
        std::vector<std::vector<int>> expected = {{0,0,0}};
        candyCrush(board);
        assert(board == expected);
    }

    // Basic vertical crush
    {
        std::vector<std::vector<int>> board = {{1},{1},{1}};
        std::vector<std::vector<int>> expected = {{0},{0},{0}};
        candyCrush(board);
        assert(board == expected);
    }

    // Multiple crushes with gravity
    {
        std::vector<std::vector<int>> board = {
            {1,2,3},
            {1,2,3},
            {1,2,3}
        };
        std::vector<std::vector<int>> expected = {
            {0,0,0},
            {0,0,0},
            {0,0,0}
        };
        candyCrush(board);
        assert(board == expected);
    }

    // No crush (stable)
    {
        std::vector<std::vector<int>> board = {
            {1,2,3},
            {4,5,6},
            {7,8,9}
        };
        std::vector<std::vector<int>> expected = board;
        candyCrush(board);
        assert(board == expected);
    }

    // Cascade effect
    {
        std::vector<std::vector<int>> board = {
            {2,2,2,1},
            {1,1,1,1},
            {3,3,3,2}
        };
        // First pass crushes row0: [2,2,2], row1: [1,1,1,1], col3: [1,1,2]? no.
        // After gravity, new values fall and may cause more
        std::vector<std::vector<int>> expected = {
            {0,0,0,0},
            {0,0,0,0},
            {0,0,0,0}
        };
        candyCrush(board);
        assert(board == expected);
    }

    // Mixed with zeros already
    {
        std::vector<std::vector<int>> board = {
            {0,1,1,1},
            {2,2,2,0},
            {0,0,0,0}
        };
        std::vector<std::vector<int>> expected = {
            {0,0,0,0},
            {0,0,0,0},
            {0,0,0,0}
        };
        candyCrush(board);
        assert(board == expected);
    }

    // Single element
    {
        std::vector<std::vector<int>> board = {{5}};
        std::vector<std::vector<int>> expected = {{5}};
        candyCrush(board);
        assert(board == expected);
    }

    // Two consecutive same values (not crushed)
    {
        std::vector<std::vector<int>> board = {{1,1}};
        std::vector<std::vector<int>> expected = {{1,1}};
        candyCrush(board);
        assert(board == expected);
    }

    // Larger board with cascade after gravity
    {
        std::vector<std::vector<int>> board = {
            {3,3,3,3,3},
            {2,1,1,1,2},
            {1,2,2,2,1},
            {0,1,2,1,0}
        };
        std::vector<std::vector<int>> expected = {
            {0,0,0,0,0},
            {0,0,0,0,0},
            {1,2,1,2,1},
            {1,1,0,1,0}
        };
        candyCrush(board);
        assert(board == expected);
    }

    return 0;
}
// The algorithm mimics the provided snippet but is refactored into a single, self-contained function. The core approach uses a helper that performs one pass: first, scan each row to find horizontal runs of equal non-zero values of length ≥3, mark those cells in a boolean "crash" matrix. Then scan each column to find vertical runs of length ≥3, similarly marking cells. After both scans, if no cell was marked, the board is stable and we return. Otherwise, we compact each column: iterate from bottom to top, collecting non-crushed values into a new column vector; then fill the column from the bottom with those values and the top with zeros. This process is repeated in a `while` loop until a pass with no marks occurs. Key edge cases: boards with all zeros (stable immediately), single-row or single-column boards, runs that overlap between horizontal and vertical (they are all marked, but compaction only uses the mark matrix), and ensuring we do not delete values that are part of a run of length exactly 2. Time complexity: each pass is O(n*m) for scanning and compacting; in the worst case, each pass removes at least one cell, so with V cells total, at most O(V) passes, giving O(n*m * V) which for small boards is fine, but practically O(n*m) per pass and typically few passes. Space complexity: O(n*m) for the boolean crash matrix.
