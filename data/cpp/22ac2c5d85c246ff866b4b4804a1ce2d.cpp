Write a C++ function `int bestScoreInLine(const std::string& board, char symbol, int n)` that, given a flat string representation of an `n x n` tic-tac-toe board (row-major order, where each cell is either `'X'`, `'O'`, or `'.'` for empty) and a character `symbol` representing one of the players, returns the maximum number of symbols that player has consecutively aligned in any single row, column, or diagonal (both main diagonals). The function should consider only straight lines of length at least 3 and should not wrap around the board. If no line of length ≥3 exists, return 0. Assume `n ≥ 3` and the input string length is exactly `n*n` and contains only the characters `'X'`, `'O'`, and `'.'`.
// The solution must scan all possible contiguous segments along rows, columns, and both diagonal directions. For each direction (right, down, down-right, down-left), traverse every starting cell that can accommodate a segment of the required minimum length 3 without going out of bounds. For each segment, count how many consecutive cells from the start match the given symbol. Keep the maximum count seen. Because the board is small (n is typically ≤ 15 in competitive settings), this approach is efficient enough. Edge cases: the board may have no alignments at all, or the maximum alignment length could be exactly 3, or a line might be longer than 3 — the algorithm must still find the longest possible consecutive run in any direction. Also, a segment might contain the symbol in a non-consecutive way (e.g., `X.X`), but since we require consecutive alignment, we must break the count when a non-matching cell is encountered. The algorithm uses four nested loops (one per direction) and, for each starting cell, iterates at most `n` steps, so the total time complexity is `O(n^3)` per direction, overall `O(n^3)` for all directions, which is acceptable for `n ≤ 15`. Space complexity is `O(1)` extra memory.
#include <string>
#include <algorithm>

// Given a flat n*n board string (row-major), return the longest contiguous
// line of 'symbol' in any row, column, or main diagonal.
int bestScoreInLine(const std::string& board, char symbol, int n) {
    if (n < 3) return 0;
    int best = 0;
    
    // Helper lambda to count a contiguous run starting at (r,c) with given step.
    auto countRun = [&](int r, int c, int dr, int dc) {
        int count = 0;
        int rr = r, cc = c;
        while (rr >= 0 && rr < n && cc >= 0 && cc < n && board[rr * n + cc] == symbol) {
            ++count;
            rr += dr;
            cc += dc;
        }
        return count;
    };
    
    // Check rows (direction right: dr=0, dc=1)
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            best = std::max(best, countRun(r, c, 0, 1));
        }
    }
    
    // Check columns (direction down: dr=1, dc=0)
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            best = std::max(best, countRun(r, c, 1, 0));
        }
    }
    
    // Check main diagonal (down-right: dr=1, dc=1)
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            best = std::max(best, countRun(r, c, 1, 1));
        }
    }
    
    // Check anti-diagonal (down-left: dr=1, dc=-1)
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            best = std::max(best, countRun(r, c, 1, -1));
        }
    }
    
    // Only consider lines of length >=3
    return (best >= 3) ? best : 0;
}
#include <cassert>
#include <string>

int bestScoreInLine(const std::string& board, char symbol, int n);

int main() {
    // 3x3 board: row 0 has XXX -> best = 3
    std::string b1 = "XXX.O.O..";
    assert(bestScoreInLine(b1, 'X', 3) == 3);
    
    // 3x3 board: diagonal X.. .X. ..X -> best = 3
    std::string b2 = "X..OX.O.X";
    assert(bestScoreInLine(b2, 'X', 3) == 3);
    
    // 4x4 board: column 1 has O at positions (0,1), (1,1), (2,1) -> best = 3
    std::string b3 = ".O..XO..XO..O...";
    assert(bestScoreInLine(b3, 'O', 4) == 3);
    
    // 5x5 board: row 2 has X at columns 1,2,3,4 -> best = 4
    std::string b4 = "....." 
                     "....."
                     ".XXXX"
                     "....."
                     ".....";
    assert(bestScoreInLine(b4, 'X', 5) == 4);
    
    // No line of length >=3 -> return 0
    std::string b5 = "X.O.X.O.X";
    assert(bestScoreInLine(b5, 'X', 3) == 0);
    
    // Anti-diagonal in 4x4: positions (0,3), (1,2), (2,1) -> best = 3
    std::string b6 = "..OX.XO..X...O...";
    assert(bestScoreInLine(b6, 'X', 4) == 3);
    
    // Mixed symbols: 'O' has only 2 in a row
    std::string b7 = "OO.X.X..";
    assert(bestScoreInLine(b7, 'O', 3) == 0);
    
    // All symbols same on entire board -> best = n
    std::string b8 = "XXXXX" "XXXXX" "XXXXX" "XXXXX" "XXXXX";
    assert(bestScoreInLine(b8, 'X', 5) == 5);
    
    // Single symbol in the middle of 3x3 -> no line
    std::string b9 = "...X....";
    assert(bestScoreInLine(b9, 'X', 3) == 0);
    
    // n=4, row 0 has X at all 4, but n=4 so best=4
    std::string b10 = "XXXX....";
    assert(bestScoreInLine(b10, 'X', 4) == 4);
    
    return 0;
}
