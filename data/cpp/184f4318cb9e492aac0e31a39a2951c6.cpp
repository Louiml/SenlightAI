Write a C++ function `int removeBlocks(int m, int n, std::vector<std::string> board)` that simulates a block-removal game on an `m x n` grid where each cell contains an uppercase letter. In each round, any 2×2 square whose four cells all contain the same letter is marked for removal; all marked cells are then cleared to a sentinel character `'1'`. After clearing, blocks fall downward (gravity): non-`'1'` characters sink to the bottom of their column, preserving their relative order. This process repeats until no 2×2 square of identical letters remains. The function must return the total number of cells cleared over all rounds. The input board is non-empty with `m, n ≤ 30`. Handle edge cases where the board has fewer than 2 rows or columns (no removals possible), and ensure that after a round, newly formed 2×2 squares from falling are considered in the next iteration. Do not modify the input board; operate on a copy.
The algorithm follows an iterative simulation. Maintain a mutable copy of the board. In each iteration, scan every cell `(i, j)` that is not `'1'` and check whether the three neighbors forming a 2×2 square with it (specifically `(i, j+1)`, `(i+1, j)`, `(i+1, j+1)`) exist within bounds and match its character. If they all match, mark all four cells in a separate boolean `boom` grid. After the scan, if no marks were made, terminate. Otherwise, iterate over all cells and for each marked cell, set it to `'1'` and increment the answer counter. Then apply gravity column by column: for each column, move non-`'1'` characters downward using a two-pointer or repeated swapping method so that all `'1'`s float to the top. Repeat the loop. Edge cases: if `m < 2` or `n < 2`, no 2×2 squares can exist, and the function immediately returns 0. Also ensure the `boom` grid is reset each round (since the code uses a global array, in the standalone function use a local `std::vector<std::vector<bool>>`). Time complexity: each round is O(m·n) for scanning and O(m²·n) worst-case for gravity (due to swapping), and the number of rounds is at most the number of clearable cells, so worst-case O(m³·n²) but practically bounded; space is O(m·n) for the board copy and boom grid.
#include <vector>
#include <string>

// Simulates removal of 2x2 same-letter blocks with gravity, returning total removed cells.
int removeBlocks(int m, int n, std::vector<std::string> board) {
    if (m < 2 || n < 2) return 0;

    int removed = 0;
    bool anyRemoved = true;

    while (anyRemoved) {
        anyRemoved = false;
        std::vector<std::vector<bool>> boom(m, std::vector<bool>(n, false));

        // Mark all 2x2 squares with identical letters.
        for (int i = 0; i < m - 1; ++i) {
            for (int j = 0; j < n - 1; ++j) {
                char c = board[i][j];
                if (c == '1') continue;
                if (board[i][j+1] == c && board[i+1][j] == c && board[i+1][j+1] == c) {
                    boom[i][j] = boom[i][j+1] = boom[i+1][j] = boom[i+1][j+1] = true;
                    anyRemoved = true;
                }
            }
        }

        if (!anyRemoved) break;

        // Clear marked cells and count them.
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (boom[i][j]) {
                    board[i][j] = '1';
                    ++removed;
                }
            }
        }

        // Apply gravity per column: push non-'1' down.
        for (int j = 0; j < n; ++j) {
            int writeRow = m - 1;
            for (int i = m - 1; i >= 0; --i) {
                if (board[i][j] != '1') {
                    board[writeRow][j] = board[i][j];
                    if (writeRow != i) board[i][j] = '1';
                    --writeRow;
                }
            }
        }
    }

    return removed;
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Example: standard 4x4 case.
    std::vector<std::string> board1 = {"AAAB", "AABB", "ABBB", "CCCC"};
    assert(removeBlocks(4, 4, board1) == 8);

    // No possible removals (different letters).
    std::vector<std::string> board2 = {"AB", "CD"};
    assert(removeBlocks(2, 2, board2) == 0);

    // Single row or column -> no 2x2 possible.
    std::vector<std::string> board3 = {"AAA"};
    assert(removeBlocks(1, 3, board3) == 0);
    std::vector<std::string> board4 = {"A", "A", "A"};
    assert(removeBlocks(3, 1, board4) == 0);

    // All same letters in 2x2 -> remove all 4.
    std::vector<std::string> board5 = {"AA", "AA"};
    assert(removeBlocks(2, 2, board5) == 4);

    // After removal, new squares form and are cleared in subsequent rounds.
    // Example: 
    // "AA"
    // "AA"
    // "AB" -> first round removes top 2x2, then fall, no more removals -> 4 removed.
    std::vector<std::string> board6 = {"AA", "AA", "AB"};
    assert(removeBlocks(3, 2, board6) == 4);

    // More complex cascade:
    // "AAA"
    // "AAA"
    // "BAA" 
    // Round1: remove top-left 2x2 (positions 0,0 to 1,1) and top-right? Actually 2x2 at (0,1) too. Let's compute expected carefully.
    // Board:
    // row0: A A A
    // row1: A A A
    // row2: B A A
    // 2x2 at (0,0): AAA? (0,0)(0,1)(1,0)(1,1) all A -> mark.
    // (0,1): (0,1)(0,2)(1,1)(1,2) all A -> mark.
    // (1,0): (1,0)(1,1)(2,0)(2,1) = A A B A -> not all same.
    // (1,1): (1,1)(1,2)(2,1)(2,2) = A A A A -> all A -> mark.
    // So mark rows 0-1 all three columns? Actually (0,2) and (1,2) are marked from second and third squares. So first round removes 6 cells (all of row0 and row1). Then fall: row2 has B, A, A and they stay at bottom. New board:
    // "111"
    // "BAA" -> still 2x2? No, only one row of non-'1', so no more removals. Total removed = 6.
    std::vector<std::string> board7 = {"AAA", "AAA", "BAA"};
    assert(removeBlocks(3, 3, board7) == 6);

    // Test that input board is not modified (copy).
    std::vector<std::string> original = {"AA", "AA"};
    std::vector<std::string> inputCopy = original;
    removeBlocks(2, 2, inputCopy);
    assert(inputCopy == original);

    return 0;
}
