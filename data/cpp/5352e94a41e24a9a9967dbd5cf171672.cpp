You are given an `N x N` board represented as a vector of vectors of integers, where `0` denotes an empty cell and any positive integer denotes a filled cell. A "block" is a rectangular region of size either `2 x 3` or `3 x 2`. A block is removable if, after filling its empty cells with the single distinct positive integer that appears anywhere in the block, the filled cells can be "dropped" vertically (i.e., every empty cell in the block has no non-zero cell directly above it in the same column, considering the entire board, not just the block), and the block contains at most two empty cells. When a block is removed, all cells in that rectangle become `0`. Write a C++ function `int removableBlocks(vector<vector<int>> board)` that returns the maximum number of blocks that can be removed sequentially, where after each removal, the board is updated, and the process repeats until no further removals are possible. The board size `N` is between 3 and 50 inclusive. You must remove blocks one at a time, and the order may affect the total count (but the given process greedily scans top-left to bottom-right repeatedly). For your solution, implement exactly the algorithm described: repeatedly scan all possible top-left positions in row-major order, and if a block (either orientation) is removable at that position, remove it and increment the count; continue until a full scan yields no removals. Return the total number of removals.

#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple 3x3 board with one 2x2 block? No, blocks are 2x3 or 3x2. 
    // A 3x3 board can fit only 2x3 or 3x2, so make a removable 2x3 block.
    std::vector<std::vector<int>> board1 = {
        {1, 1, 1},
        {1, 0, 0},
        {0, 0, 0}
    };
    // 2x3 block at (0,0): cells (0,0),(0,1),(0,2),(1,0),(1,1),(1,2) -> has empty cells at (1,1),(1,2) but above them in column 1 and 2 row 0 are non-zero? Actually row 0 col 1 is 1, so those empty cells have non-zero above -> cannot remove. Try other positions. 
    // Let's use a board where a 2x3 block is fully filled and no empty cells.
    std::vector<std::vector<int>> board2 = {
        {2, 2, 2},
        {2, 2, 2},
        {0, 0, 0}
    };
    assert(removableBlocks(board2) == 1); // remove 2x3 at (0,0) first pass

    // Test 2: Two separate removable blocks in a 4x4 board.
    std::vector<std::vector<int>> board3 = {
        {3, 3, 3, 0},
        {3, 3, 3, 0},
        {0, 0, 0, 0},
        {4, 4, 0, 4} // this block is invalid because multiple values
    };
    // Only the top-left 2x3 block is removable. After removal, no more.
    assert(removableBlocks(board3) == 1);

    // Test 3: A 3x2 block.
    std::vector<std::vector<int>> board4 = {
        {5, 5},
        {5, 5},
        {5, 5}
    };
    // N=3, board is 3x2? Actually vector has 3 rows, 2 columns -> N=3? No, N is board.size() = 3, but columns are 2, which is not square. The problem expects N x N, so ensure square.
    // Let's make a 3x3 board with a 3x2 block.
    std::vector<std::vector<int>> board5 = {
        {6, 6, 0},
        {6, 6, 0},
        {6, 6, 0}
    };
    // 3x2 block at (0,0) has no empty cells -> removable.
    assert(removableBlocks(board5) == 1);

    // Test 4: Block with two empty cells and above clear.
    std::vector<std::vector<int>> board6 = {
        {7, 7, 7},
        {0, 0, 7},
        {0, 0, 0}
    };
    // 2x3 at (0,0): cells row0 all 7, row1: (1,0)=0, (1,1)=0, (1,2)=7. Empty cells have no non-zero above (row0 col0 and col1 are 7? Actually row0 col0=7, so above is non-zero! So cannot. Try 3x2 at (0,0): row0 col0=7,row0 col1=7; row1 col0=0,row1 col1=0 (above row0 col0/1 are 7 -> blocked); so no.
    // Instead, make empty cells in top row? Impossible because above none. Let's make a block with empty cells in bottom row and above clear.
    std::vector<std::vector<int>> board7 = {
        {8, 8, 8},
        {8, 8, 8},
        {0, 0, 0}
    };
    // 3x2 at (0,0): row0 and row1 filled, row2 col0 and col1 empty, but above row2 col0 has row1 col0=8? Actually row2 col0 is empty, above row1 col0=8, so blocked. So no. 
    // Easier: a 2x3 block with all filled.
    std::vector<std::vector<int>> board8 = {
        {9, 9, 9},
        {9, 9, 9},
        {9, 9, 9}
    };
    // This board has overlapping blocks. The first pass will remove the 2x3 at (0,0), then the rest? After removal, board becomes all zeros? Then no more. So answer 1.
    assert(removableBlocks(board8) == 1);

    // Test 5: Larger board with multiple sequential removals.
    std::vector<std::vector<int>> board9 = {
        {1, 1, 1, 0, 0},
        {1, 1, 1, 0, 0},
        {0, 0, 0, 2, 2},
        {0, 0, 0, 2, 2},
        {0, 0, 0, 2, 2}
    };
    // First pass: at (0,0) 2x3 block is all 1 -> remove. Then board becomes all zeros in top-left 3x2? Actually after removal, rows 0-1 cols0-2 become 0. Then at (2,3) we have a 3x2 block of 2's, but now after first removal, those cells are still 2, and above them (rows 0-1, cols3-4) are 0, so they are removable. So total 2.
    assert(removableBlocks(board9) == 2);

    // Test 6: Block with two empty cells and no above blocked.
    std::vector<std::vector<int>> board10 = {
        {0, 3, 3},
        {0, 3, 3},
        {0, 3, 3}
    };
    // 3x2 at (0,1): cells rows0-2 col1-2, all 3, no empty -> removable.
    // Also 2x3 at (0,1)? Actually r=0,c=1, 2x3 would need cols1-3 but N=3, so not.
    // So answer 1.
    assert(removableBlocks(board10) == 1);

    return 0;
}

#include <vector>

// Helper function to check if a block starting at (row, col) with given height and width is removable.
// It modifies the board only if removable.
bool canRemoveBlock(std::vector<std::vector<int>>& board, int row, int col, int h, int w) {
    int emptyCount = 0;
    int lastValue = -1;

    // First pass: check conditions without modifying
    for (int r = row; r < row + h; ++r) {
        for (int c = col; c < col + w; ++c) {
            const int value = board[r][c];
            if (value == 0) {
                // If there is any non-zero cell directly above in the same column,
                // the empty cell cannot be filled.
                for (int above = 0; above < r; ++above) {
                    if (board[above][c] != 0) {
                        return false;
                    }
                }
                ++emptyCount;
                if (emptyCount > 2) {
                    return false;
                }
            } else {
                // All non-zero cells in the block must have the same value.
                if (lastValue != -1 && lastValue != value) {
                    return false;
                }
                lastValue = value;
            }
        }
    }

    // All conditions satisfied: remove the block by setting its cells to zero.
    for (int r = row; r < row + h; ++r) {
        for (int c = col; c < col + w; ++c) {
            board[r][c] = 0;
        }
    }
    return true;
}

// Main solution function: returns the number of blocks removed by the greedy process.
int removableBlocks(std::vector<std::vector<int>> board) {
    const int N = static_cast<int>(board.size());
    int answer = 0;

    bool changed;
    do {
        changed = false;
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                // Try 2x3 block
                if (r <= N - 2 && c <= N - 3 && canRemoveBlock(board, r, c, 2, 3)) {
                    ++answer;
                    changed = true;
                }
                // Try 3x2 block
                else if (r <= N - 3 && c <= N - 2 && canRemoveBlock(board, r, c, 3, 2)) {
                    ++answer;
                    changed = true;
                }
            }
        }
    } while (changed);

    return answer;
}

// The algorithm simulates the removal process greedily. For each iteration, it scans the entire board row by row, column by column. For each top-left cell `(r, c)`, it first tries to fit a `2x3` block (if `r <= N-2` and `c <= N-3`), and if that block is removable, it removes it and increments the count for this iteration. If not, it tries a `3x2` block (if `r <= N-3` and `c <= N-2`). The removable check for a block of height `h` and width `w` involves:  
// - For each cell in the block, if it is empty (`0`), we check whether there is any non-zero cell directly above it in the same column on the board (from row 0 to `r-1`). If yes, the block cannot be filled and removed because the empty cell is blocked from above.  
// - Also, the total number of empty cells in the block must be at most 2.  
// - For non-empty cells, they must all have the same value (since the block represents a single piece).  
// If all conditions hold, we set all cells in the block to `0` and return true. The outer loop repeats this scanning until no block is removed in a full pass. The greedy approach is deterministic and matches the problem statement. The time complexity is O(K * N^2 * (h*w)) where K is the number of removal rounds; in the worst case, K is O(N^2) and each block check is O(1) (since h*w is at most 6). Thus worst-case time is O(N^4), but with N≤50 that is at most ~6.25 million operations, which is fine. Space complexity is O(N^2) for the board copy; we make a copy because the input is passed by value, so we modify the local copy.
