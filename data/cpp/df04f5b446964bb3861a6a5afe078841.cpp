Write a C++ function `int countRemovedDolls(const std::vector<std::vector<int>>& board, const std::vector<int>& moves)` that simulates a crane game. The `board` is a square matrix (N×N) where each cell contains a positive integer representing a doll type, or `0` for an empty slot. The crane starts at the top of each column specified in `moves` (1-indexed) and picks the first non-zero doll from top to bottom, moving it to a temporary basket (a stack). If the top doll in the basket has the same type as the newly picked doll, both are removed from the basket (i.e., "exploded") and the count of removed dolls increases by 2. If different, the new doll is placed on top of the stack. If the column is empty (all zeros), the crane does nothing. The function should return the total number of dolls removed after processing all moves. You may assume the board is non-empty, square, and all moves are valid column indices.

#include <cassert>
#include <vector>

// Assuming countRemovedDolls is defined above.

int main() {
    // Example from the problem statement
    std::vector<std::vector<int>> board1 = {
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 3},
        {0, 2, 5, 0, 1},
        {4, 2, 4, 4, 2},
        {3, 5, 1, 3, 1}
    };
    std::vector<int> moves1 = {1, 5, 3, 5, 1, 2, 1, 4};
    assert(countRemovedDolls(board1, moves1) == 4);

    // Empty column: no removal
    std::vector<std::vector<int>> board2 = {{0, 0}, {0, 0}};
    std::vector<int> moves2 = {1, 2, 1, 2};
    assert(countRemovedDolls(board2, moves2) == 0);

    // Single row, matching dolls
    std::vector<std::vector<int>> board3 = {{1, 1}};
    std::vector<int> moves3 = {1, 2};
    assert(countRemovedDolls(board3, moves3) == 2);

    // All same doll stacked, no match after first
    std::vector<std::vector<int>> board4 = {{1, 0}, {1, 0}};
    std::vector<int> moves4 = {1, 1, 1};
    assert(countRemovedDolls(board4, moves4) == 0);

    // Multiple removals sequence
    std::vector<std::vector<int>> board5 = {
        {0, 2},
        {2, 0}
    };
    std::vector<int> moves5 = {2, 1, 1, 2};
    assert(countRemovedDolls(board5, moves5) == 4);

    // Move with invalid column is not allowed per assumptions,
    // but no test needed.

    return 0;
}

#include <vector>
#include <stack>

// Simulate a crane game; returns number of removed dolls.
int countRemovedDolls(const std::vector<std::vector<int>>& board, const std::vector<int>& moves) {
    // Make a mutable copy because we need to clear picked cells.
    auto grid = board;
    std::stack<int> basket;
    int removed = 0;
    const int N = static_cast<int>(grid.size());

    for (int move : moves) {
        int col = move - 1;  // convert to 0-indexed
        // Scan top to bottom in the given column
        for (int row = 0; row < N; ++row) {
            int doll = grid[row][col];
            if (doll != 0) {
                if (!basket.empty() && basket.top() == doll) {
                    basket.pop();
                    removed += 2;
                } else {
                    basket.push(doll);
                }
                grid[row][col] = 0;  // pick the doll up
                break;  // only one doll per move
            }
        }
    }
    return removed;
}

// The solution uses a stack to represent the basket. For each move, convert the 1-indexed column to a 0-indexed index, then scan rows from top (index 0) to bottom until a non-zero value is found. If found, compare it with the current top of the stack: if equal, pop the stack and add 2 to the result; otherwise push the doll value onto the stack. After processing, set that board cell to 0 to mark it as empty. If the entire column is zero, skip. The main edge case is when the stack is empty: just push. Also, since the board is passed by `const` reference, we need to copy it locally to modify cells. Time complexity: O(M × N) where M is number of moves and N is board dimension (since each move scans at most N rows). Space complexity: O(N²) for the copy plus O(N) for the stack in the worst case.
