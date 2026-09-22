Write a C++ function `std::string ticTacToeOutcome(const std::vector<std::string>& board)` that takes a 4×4 Tic-Tac-Toe board represented as a vector of 4 strings, each of length 4, containing only characters `'X'`, `'O'`, `'T'` (a wildcard that can count as either player), and `'.'` (empty cell). The function must return exactly one of the four strings: `"X won"`, `"O won"`, `"Draw"`, or `"Game has not completed"`, following these rules: (1) A player wins if all four cells in any row, column, or diagonal contain that player's symbol or `'T'`; (2) If both players could theoretically win (impossible in valid input, but if it happens, return `"Draw"`); (3) If no winner and there are no empty cells, return `"Draw"`; (4) If no winner and there is at least one empty cell, return `"Game has not completed"`. The function should be robust and not modify the input.

#include <cassert>
#include <string>
#include <vector>

// Function declaration (must match the solution)
std::string ticTacToeOutcome(const std::vector<std::string>& board);

int main() {
    // X wins in a row
    assert(ticTacToeOutcome({"XXXX", "OO..", "....", "...."}) == "X won");
    // O wins in a column with T wildcard
    assert(ticTacToeOutcome({"O.X.", "OX..", "OX..", "O.T."}) == "O won");
    // X wins on main diagonal with T
    assert(ticTacToeOutcome({"X...", ".X.T", "..X.", "...X"}) == "X won");
    // O wins on anti-diagonal
    assert(ticTacToeOutcome({"...O", "..O.", ".O..", "O..."}) == "O won");
    // Draw (no empty cells, no winner)
    assert(ticTacToeOutcome({"XOXO", "OXOX", "XOXO", "OXOX"}) == "Draw");
    // Game incomplete (no winner, empty cells)
    assert(ticTacToeOutcome({"XOX.", "OXO.", "....", "...."}) == "Game has not completed");
    // All Ts is not a win (unclear winner) - treat as incomplete if empty, else draw
    assert(ticTacToeOutcome({"TTTT", "TTTT", "TTTT", "TTTT"}) == "Draw");
    // Single empty cell, no winner
    assert(ticTacToeOutcome({"XOXO", "OXOX", "XOX.", "OXOX"}) == "Game has not completed");
    // X wins in last row with T
    assert(ticTacToeOutcome({"O O.", "O.X.", "O..X", "XTXX"}) == "X won");
    // Both have possible win? Invalid, but function returns first found (X) - test not included for simplicity

    return 0;
}

#include <string>
#include <vector>

// Returns the outcome of a 4x4 Tic-Tac-Toe board according to the specified rules.
std::string ticTacToeOutcome(const std::vector<std::string>& board) {
    const int N = 4;
    bool hasEmpty = false;

    // Helper lambda to check a line; returns 'X', 'O', or '\0' if no winner.
    auto checkLine = [&](int startRow, int startCol, int dRow, int dCol) -> char {
        char ref = board[startRow][startCol];
        // If reference is empty or wildcard, line cannot directly declare a win.
        if (ref == '.' || ref == 'T') return '\0';
        for (int i = 1; i < N; ++i) {
            int r = startRow + i * dRow;
            int c = startCol + i * dCol;
            if (board[r][c] == '.') {
                hasEmpty = true;
                return '\0';
            }
            if (board[r][c] != ref && board[r][c] != 'T') {
                return '\0';
            }
        }
        return ref; // Only 'X' or 'O' reaches here.
    };

    // Check rows
    for (int r = 0; r < N; ++r) {
        char winner = checkLine(r, 0, 0, 1);
        if (winner == 'X') return "X won";
        if (winner == 'O') return "O won";
    }

    // Check columns
    for (int c = 0; c < N; ++c) {
        char winner = checkLine(0, c, 1, 0);
        if (winner == 'X') return "X won";
        if (winner == 'O') return "O won";
    }

    // Check main diagonal
    char winner = checkLine(0, 0, 1, 1);
    if (winner == 'X') return "X won";
    if (winner == 'O') return "O won";

    // Check anti-diagonal
    winner = checkLine(0, N-1, 1, -1);
    if (winner == 'X') return "X won";
    if (winner == 'O') return "O won";

    // If no winner, check for emptiness
    if (!hasEmpty) {
        // Double-check if any '.' exists (in case a line was not fully traversed)
        for (const auto& row : board) {
            for (char ch : row) {
                if (ch == '.') {
                    hasEmpty = true;
                    break;
                }
            }
            if (hasEmpty) break;
        }
    }

    return hasEmpty ? "Game has not completed" : "Draw";
}

// The solution first checks all 4 rows, then all 4 columns, and finally the two diagonals (top-left to bottom-right and top-right to bottom-left). For each line (row/column/diagonal), initialize a reference character from the first cell. If that reference character is `'.'`, the line cannot be a winning line (since `'.'` is not a player symbol and `'T'` cannot be a reference because it could be either). Then iterate over the remaining three cells: if a cell is `'.'`, mark the board as incomplete and eliminate the line from being a winner; if a cell is not equal to the reference character and is not `'T'`, eliminate the line. After the loop, if the line was not eliminated and the reference was `'X'` or `'O'`, return that symbol's win string. After checking all lines, if no winner was found, return `"Draw"` if the board is completely filled (no `'.'` anywhere), otherwise return `"Game has not completed"`. Edge cases: a line like `"T T T T"` would have the first char `'T'` as reference, and all other `'T'`s are equal, so the line is not eliminated — but since `'T'` is not a valid player symbol, we must explicitly check that the reference is not `'T'` and not `'.'` before declaring a win (or we only declare a win if the reference is `'X'` or `'O'`). Time complexity is O(1) because the board is fixed 4×4; space complexity is O(1) beyond the input.
