Write a standalone C++ function `std::string analyzeTicTacToe(const std::vector<std::string>& board)` that takes a 3×3 Tic‑Tac‑Toe board represented as a vector of 9 strings (each either "X", "O", or a space " "), in row-major order (indices 0–8 correspond to positions 1–9). The function must return a single character string indicating the state: `"X"` if X has three in a row (horizontally, vertically, or diagonally), `"O"` if O has three in a row, `"C"` if the board is full with no winner (a draw), and `"E"` otherwise (game still in progress). The input is guaranteed to be valid (exactly 9 elements, each exactly "X", "O", or " "), but there may be multiple winning lines, and the board might be in an impossible state (e.g., both players have three in a row). In such ambiguous cases, prioritize in this order: if any winner exists, return that winner (if both winners exist, return `"X"`); otherwise if full return `"C"`; else return `"E"`. Note that the function should not modify the input vector.
// The main task is to check for winning conditions on a fixed 3×3 grid. The solution should first define all eight possible winning lines: three rows (indices 0–1–2, 3–4–5, 6–7–8), three columns (0–3–6, 1–4–7, 2–5–8), and two diagonals (0–4–8, 2–4–6). For each line, check if all three cells are equal and not a space. If found, record the winner. Because multiple lines could be won by the same player, we only need to track if X has a win and if O has a win. Then apply the priority order: if X wins, return "X"; else if O wins, return "O"; else if board is full (no cell is " "), return "C"; else return "E". Edge cases include a full board with a winner (winner takes precedence), a board with no empty cells and no winner (draw), and a board with both winners (return "X"). Time complexity is O(1) because we check a constant number of lines each with 3 cells, and space is O(1) beyond the input. The function must be const-correct and avoid modifying the board.
#include <string>
#include <vector>

// Analyze a 3x3 Tic-Tac-Toe board given as a 9-element vector in row-major order.
// Return "X" if X wins, "O" if O wins, "C" for a draw, "E" for in progress.
std::string analyzeTicTacToe(const std::vector<std::string>& board) {
    // All winning line indices (triples of positions 0..8)
    const int lines[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8},  // rows
        {0,3,6}, {1,4,7}, {2,5,8},  // columns
        {0,4,8}, {2,4,6}            // diagonals
    };

    bool xWins = false;
    bool oWins = false;

    for (const auto& line : lines) {
        const std::string& a = board[line[0]];
        const std::string& b = board[line[1]];
        const std::string& c = board[line[2]];
        if (a != " " && a == b && b == c) {
            if (a == "X") xWins = true;
            else if (a == "O") oWins = true;
        }
    }

    if (xWins) return "X";
    if (oWins) return "O";

    // Check if board is full
    bool full = true;
    for (const std::string& cell : board) {
        if (cell == " ") {
            full = false;
            break;
        }
    }
    if (full) return "C";
    return "E";
}
#include <cassert>
#include <string>
#include <vector>

// The solution function is assumed to be declared above.
// (For the test, include the solution header or copy the function.)

int main() {
    // X wins on top row
    assert(analyzeTicTacToe({"X","X","X"," ","O"," ","O"," "," "}) == "X");
    // O wins on diagonal
    assert(analyzeTicTacToe({"O","X"," "," ","O","X"," "," ","O"}) == "O");
    // Draw (full board no winner)
    assert(analyzeTicTacToe({"X","O","X","O","X","O","O","X","O"}) == "C");
    // In progress (not full, no winner)
    assert(analyzeTicTacToe({"X"," "," "," ","O"," "," "," "," "}) == "E");
    // Both players have a win; X takes priority
    assert(analyzeTicTacToe({"X","X","X","O","O","O"," "," "," "}) == "X");
    // X wins on a column with extra moves
    assert(analyzeTicTacToe({"X","O"," ","X","O"," ","X"," "," "}) == "X");
    // Full board but impossible state with both winners; X priority
    assert(analyzeTicTacToe({"X","X","X","O","O","O","X","O","X"}) == "X");
    // Empty board is in progress
    assert(analyzeTicTacToe({" "," "," "," "," "," "," "," "," "}) == "E");
    // O wins on bottom row with spaces elsewhere
    assert(analyzeTicTacToe({" "," ","X"," ","X"," ","O","O","O"}) == "O");
    // One move only
    assert(analyzeTicTacToe({"X"," "," "," "," "," "," "," "," "}) == "E");
    return 0;
}
