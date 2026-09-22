// Given a 6x7 Connect Four board represented as a `std::vector<std::vector<char>>` where each cell contains `' '` (empty), `'X'`, or `'O'`, write a C++ function that simulates a move for a given player and validates it, returning a new board with the disc placed in the lowest available row of the specified column, or the original board unchanged if the column is invalid or full. The function must not modify the input board and must handle out-of-range columns, full columns (where the top cell is occupied), and ensure the disc falls to the bottom-most empty position. The function should use a constant board size of 6 rows and 7 columns defined within the function or as compile-time constants.

#include <cassert>
#include <vector>

int main() {
    // Setup an empty board
    std::vector<std::vector<char>> emptyBoard(6, std::vector<char>(7, ' '));

    // Test 1: Place X in column 0 on empty board
    auto newBoard1 = makeMove(emptyBoard, 0, 'X');
    assert(newBoard1[5][0] == 'X');
    assert(newBoard1[0][0] == ' ');

    // Test 2: Original board unchanged
    assert(emptyBoard[5][0] == ' ');

    // Test 3: Place O in column 3 on empty board
    auto newBoard2 = makeMove(emptyBoard, 3, 'O');
    assert(newBoard2[5][3] == 'O');

    // Test 4: Invalid column (negative) returns original board
    auto invalidNeg = makeMove(emptyBoard, -1, 'X');
    assert(invalidNeg == emptyBoard);

    // Test 5: Invalid column (too large) returns original board
    auto invalidLarge = makeMove(emptyBoard, 7, 'X');
    assert(invalidLarge == emptyBoard);

    // Test 6: Full column (top cell occupied) returns original board
    std::vector<std::vector<char>> fullColumnBoard(6, std::vector<char>(7, ' '));
    for (int row = 0; row < 6; ++row) {
        fullColumnBoard[row][2] = 'X';  // fill column 2 completely
    }
    auto fullResult = makeMove(fullColumnBoard, 2, 'O');
    assert(fullResult == fullColumnBoard);

    // Test 7: Partially filled column places at correct row
    std::vector<std::vector<char>> partialBoard(6, std::vector<char>(7, ' '));
    partialBoard[5][4] = 'X';  // bottom row filled in column 4
    auto partialResult = makeMove(partialBoard, 4, 'O');
    assert(partialResult[4][4] == 'O');  // disc lands above
    assert(partialResult[5][4] == 'X');  // original disc preserved

    // Test 8: Multiple moves stack correctly
    auto board = makeMove(emptyBoard, 1, 'X');  // X at row 5
    board = makeMove(board, 1, 'O');            // O at row 4
    board = makeMove(board, 1, 'X');            // X at row 3
    assert(board[5][1] == 'X');
    assert(board[4][1] == 'O');
    assert(board[3][1] == 'X');
    assert(board[2][1] == ' ');

    // Test 9: Returns new object, not a reference to input
    auto boardA = emptyBoard;
    auto boardB = makeMove(boardA, 5, 'O');
    assert(boardA[5][5] == ' ');
    assert(boardB[5][5] == 'O');

    // Test 10: Valid columns range from 0 to 6
    auto testAllCols = emptyBoard;
    for (int c = 0; c < 7; ++c) {
        auto res = makeMove(testAllCols, c, 'X');
        assert(res[5][c] == 'X');
    }

    return 0;
}

#include <vector>

// Return a new board after placing a disc for 'player' in column 'col'.
// If col is invalid or the column is full, return the original board unchanged.
std::vector<std::vector<char>> makeMove(const std::vector<std::vector<char>>& board, int col, char player) {
    const int ROWS = 6;
    const int COLS = 7;

    // Validate column index
    if (col < 0 || col >= COLS) {
        return board;
    }

    // Check if column is full (top cell occupied)
    if (board[0][col] != ' ') {
        return board;
    }

    // Create a copy of the board to modify
    std::vector<std::vector<char>> newBoard = board;

    // Place disc at the lowest available empty row
    for (int row = ROWS - 1; row >= 0; --row) {
        if (newBoard[row][col] == ' ') {
            newBoard[row][col] = player;
            return newBoard;
        }
    }

    // Should never reach here if validation passed, but return original for safety
    return board;
}

// The solution requires a pure function that returns a new board state without mutating the input. The key steps are: first, validate the column index (must be between 0 and 6 inclusive) and check whether the top cell of that column is empty (if not, the column is full). Then, iterate from the bottom row (index 5) upward to find the first empty row in that column; place the player's disc there. If no empty cell is found (shouldn't happen given the top check but included for safety), return the original board. Edge cases include negative or too-large column indices, a full column (top cell not empty), and ensuring the copy is independent of the original. Time complexity is O(6) for the column scan, effectively O(1) constant since board dimensions are fixed; space complexity is O(1) for the new board copy, which is also constant. The function is `const`-correct and does not use global state.
