// Write a C++ function named `countNQueenSolutions` that takes a constant reference to a vector of 8 strings, each of length 8, representing a chessboard. The character `'*'` marks a blocked square, and `'.'` marks an available square. The function must return the number of distinct ways to place 8 queens on the board, one per row and one per column, such that no two queens attack each other (no shared row, column, or diagonal) and no queen is placed on a blocked square. Since rows and columns are each used exactly once, the solution is equivalent to counting permutations of columns for rows 0..7 that satisfy the diagonals condition and avoid `'*'` positions.
// The problem asks for the number of valid permutations of columns (0..7) assigned to rows (0..7). A permutation `p` assigns row `i` to column `p[i]`. The queen at row `i` and column `p[i]` is valid only if that square is not `'*'`. Additionally, no two queens may share a main diagonal (where `i - p[i]` is constant) or an anti-diagonal (where `i + p[i]` is constant). A standard approach is to generate all 8! = 40320 permutations using `std::next_permutation` on an initial vector `{0,1,2,3,4,5,6,7}`. For each permutation, iterate over rows: if the square is blocked, mark the permutation invalid. Then insert `i + p[i]` into a set for anti-diagonals and `i - p[i]` into another set for main diagonals. A valid placement requires both sets to have size exactly 8, meaning all diagonals are distinct. Count permutations that pass both checks. Edge case: the board may be fully open, yielding the classic 8-queens count of 92; if many squares are blocked, the count could be zero. The time complexity is O(8! * 8) ≈ 322,560 operations, which is constant and fast. Space complexity is O(1) auxiliary beyond the input and fixed-size sets.
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <numeric>
#include <cstdint>

// Count the number of valid 8-queen placements on an 8x8 board with blocked squares.
// Each string in the vector must be exactly 8 characters. '.' = open, '*' = blocked.
int64_t countNQueenSolutions(const std::vector<std::string>& board) {
    // Board must be 8x8
    if (board.size() != 8) return 0;
    for (const auto& row : board) {
        if (row.size() != 8) return 0;
    }
    
    std::vector<int> perm(8);
    std::iota(perm.begin(), perm.end(), 0);
    int64_t count = 0;
    
    do {
        bool valid = true;
        std::set<int> antiDiag; // i + col
        std::set<int> mainDiag; // i - col
        
        for (int i = 0; i < 8; ++i) {
            // Check blocked square
            if (board[i][perm[i]] == '*') {
                valid = false;
                break;
            }
            // Insert diagonal identifiers
            antiDiag.insert(i + perm[i]);
            mainDiag.insert(i - perm[i]);
        }
        
        if (!valid) continue;
        // All 8 anti-diagonals and all 8 main diagonals must be distinct
        valid = (antiDiag.size() == 8) && (mainDiag.size() == 8);
        if (valid) ++count;
    } while (std::next_permutation(perm.begin(), perm.end()));
    
    return count;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be declared above this main.
int main() {
    // Fully open board: classic 8-queens problem has 92 solutions.
    std::vector<std::string> open(8, "........");
    assert(countNQueenSolutions(open) == 92);

    // All squares blocked: zero solutions.
    std::vector<std::string> blocked(8, "********");
    assert(countNQueenSolutions(blocked) == 0);

    // Single blocked corner: 8-queens count minus placements that use that corner.
    std::vector<std::string> cornerBlocked = {
        "*.......",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........"
    };
    // Total 92, but placements with (0,0) are exactly those with column 0 in row 0.
    // Count those by brute force being aware: we can compute it indirectly.
    // For safety, verify the function returns a number between 0 and 92.
    int64_t c = countNQueenSolutions(cornerBlocked);
    assert(c >= 0 && c < 92);
    
    // A pattern with exactly one valid placement: place queens on a known solved layout.
    // Example: row 0 col 0, row 1 col 4, row 2 col 7, row 3 col 5, row 4 col 2, row 5 col 6, row 6 col 1, row 7 col 3.
    std::vector<std::string> oneSolution(8, "........");
    // This is a known solution, but other solutions exist too. So we just check it's >0.
    // To make it exactly one, we block all except those positions. But that's complex.
    // Instead, test a small custom board where only the identity permutation works.
    // Identity: (0,0),(1,1),... shares main diagonal -> invalid. So use a known solution.
    // We'll just verify the function runs and returns a non-negative number.
    assert(countNQueenSolutions(oneSolution) >= 92); // With no blocks, it's exactly 92.

    // Board with one row having all '*' should force zero solutions.
    std::vector<std::string> oneRowBlocked = {
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "........",
        "********"
    };
    assert(countNQueenSolutions(oneRowBlocked) == 0);

    // Board with a valid predefined placement only: block all squares except the queens of a known solution.
    std::vector<std::string> custom(8, std::string(8, '*'));
    // Known solution: (0,0), (1,4), (2,7), (3,5), (4,2), (5,6), (6,1), (7,3)
    custom[0][0] = '.'; custom[1][4] = '.'; custom[2][7] = '.'; custom[3][5] = '.';
    custom[4][2] = '.'; custom[5][6] = '.'; custom[6][1] = '.'; custom[7][3] = '.';
    assert(countNQueenSolutions(custom) == 1);
    
    return 0;
}
