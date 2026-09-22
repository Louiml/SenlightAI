// Implement a C++ function that evaluates the current state of an 8x8 Othello (Reversi) board from the perspective of a given player. The board is represented as a 2D array of integers where 255 represents white discs, 0 represents black discs, and 16581375 represents empty cells. The function must compute a heuristic score that combines multiple evaluation criteria: disc count, positional weight (using a provided weight matrix), corner control, and mobility (number of valid moves). Given the board state, the player (−1 for black, 1 for white), and the selected heuristic mode (an integer from 1 to 5), return a double value that estimates the board's favorability for that player. Higher positive scores should indicate better positions for the specified player.
// The solution requires implementing a heuristic evaluation function that considers up to four distinct factors, depending on the mode parameter `k`:
// 1. **Mode 1 (Disc count)**: Count the number of discs for each player and return the difference multiplied by the player's perspective.
// 2. **Mode 2 (Positional stability)**: For each occupied cell, add the corresponding weight from the provided 8x8 weight matrix to the owning player's total, then return the weighted difference.
// 3. **Mode 3 (Corner control)**: Count discs in the four corners (0,0), (0,7), (7,0), (7,7) and give each corner a bonus of 10 points, then add to the disc count difference.
// 4. **Mode 4 (Mobility)**: Count valid moves for each player and give the opponent's mobility a penalty (positive for fewer opponent moves). Specifically, sum 2 times the number of valid moves for the opponent and subtract from the disc count difference (i.e., `sum1 += 2 * validmoves(grid, -1).size()` for player 1).
// 5. **Mode 5 (Composite)**: Combine disc count, weighted position, corner bonuses (30 per corner), and mobility (5 per valid move).
//
// The core helper function `validmoves` must determine all legal moves for a given player on the current board. A legal move is an empty cell that flips at least one opponent disc in any of the eight directions. The function iterates through all empty cells, checks each direction by scanning contiguous opponent discs until hitting either a friendly disc (valid move) or a boundary/empty cell (invalid), and collects valid positions.
//
// For the scoring function itself, the approach is:
// - Initialize `sum1` and `sum2` to 0.
// - Iterate through all 8x8 cells. For mode 1, simply count discs. For mode 2, add the weight from the global `weight` matrix. For modes 3–5, incorporate additional bonuses as described.
// - For mode 4, compute mobility by calling `validmoves` for the opponent and multiplying by 2.
// - For mode 5, compute both disc count and weight contributions, add corner bonuses of 30 points per corner, and add 5 times the number of valid moves for both players.
// - Finally, subtract `sum2` from `sum1` and multiply by the player's perspective (`play` is passed in as 1 or -1) to make the score relative to that player.
//
// Edge cases:
// - Empty board or no discs: The function should still return 0 (since sums remain equal).
// - All cells filled: Mobility is zero; the function accurately reflects the final disc count.
// - Invalid moves list empty: `validmoves` returns an empty vector; this is handled gracefully.
//
// Time complexity: The scoring function calls `validmoves` for mobility modes, which itself is O(8 × 64 × 8) = O(4096) in the worst case (checking all 8 directions for each cell). For a single score calculation, this is acceptable. Space complexity is O(1) additional space for the score computation, aside from the temporary vector returned by `validmoves` (O(64) in the worst case).
#include <vector>
#include <utility>

// The global weight matrix (as provided in the original code)
const int weight[8][8] = {
    {9,2,7,7,7,7,2,9},
    {2,1,4,4,4,4,1,2},
    {7,4,6,5,5,6,4,7},
    {7,4,5,6,6,5,4,7},
    {7,4,5,6,6,5,4,7},
    {7,4,6,5,5,6,4,7},
    {2,1,4,4,4,4,1,2},
    {9,2,7,7,7,7,2,9}
};

// Check if a point exists in the list of valid moves
bool containsPoint(const std::vector<std::pair<int,int>>& moves, std::pair<int,int> p) {
    for (const auto& m : moves) {
        if (m.first == p.first && m.second == p.second) return true;
    }
    return false;
}

// Return all valid moves for a given player on the board
// player = 1 for white (255), player = -1 for black (0)
std::vector<std::pair<int,int>> validmoves(const int grid[8][8], int player) {
    std::vector<std::pair<int,int>> res;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (grid[i][j] != 16581375) continue;  // Only consider empty cells
            
            int ownColor = (player == 1) ? 255 : 0;
            int oppColor = (player == 1) ? 0 : 255;
            bool valid = false;
            
            // Check all eight directions
            const int dirs[8][2] = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
            for (int d = 0; d < 8; ++d) {
                int x = i + dirs[d][0];
                int y = j + dirs[d][1];
                bool foundOpp = false;
                while (x >= 0 && x < 8 && y >= 0 && y < 8 && grid[x][y] == oppColor) {
                    foundOpp = true;
                    x += dirs[d][0];
                    y += dirs[d][1];
                }
                // If we found opponent discs and then hit our own disc, it's a valid move
                if (foundOpp && x >= 0 && x < 8 && y >= 0 && y < 8 && grid[x][y] == ownColor) {
                    valid = true;
                    break;
                }
            }
            if (valid) {
                res.push_back(std::make_pair(i, j));
            }
        }
    }
    return res;
}

// Compute heuristic score for the board from the perspective of 'play' (1 or -1)
// mode k: 1=disc count, 2=positional weight, 3=corners bonus, 4=mobility, 5=composite
double evaluateBoard(const int grid[8][8], int play, int k) {
    int sum1 = 0, sum2 = 0;  // aggregated scores for white and black
    
    // Determine corners for bonus calculations
    const int corners[4][2] = {{0,0},{0,7},{7,0},{7,7}};
    
    // Pass 1: accumulate disc counts and positional weights
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (grid[i][j] == 255) {
                sum1++;
                if (k == 2 || k == 5) sum1 += weight[i][j] - 1;  // For positional mode, add weight (minus 1 to avoid double counting disc count in composite)
            } else if (grid[i][j] == 0) {
                sum2++;
                if (k == 2 || k == 5) sum2 += weight[i][j] - 1;
            }
        }
    }
    
    // For mode 3: add corner bonuses (10 per corner owned)
    if (k == 3) {
        for (int c = 0; c < 4; ++c) {
            int val = grid[corners[c][0]][corners[c][1]];
            if (val == 255) sum1 += 10;
            if (val == 0) sum2 += 10;
        }
    }
    
    // For mode 4: add mobility bonus (2 per opponent move removed)
    if (k == 4) {
        sum1 += 2 * static_cast<int>(validmoves(grid, -1).size());
        sum2 += 2 * static_cast<int>(validmoves(grid, 1).size());
    }
    
    // For mode 5: add corner bonuses (30 each) and mobility (5 per move)
    if (k == 5) {
        for (int c = 0; c < 4; ++c) {
            int val = grid[corners[c][0]][corners[c][1]];
            if (val == 255) sum1 += 30;
            if (val == 0) sum2 += 30;
        }
        sum1 += 5 * static_cast<int>(validmoves(grid, 1).size());
        sum2 += 5 * static_cast<int>(validmoves(grid, -1).size());
    }
    
    double res = static_cast<double>(sum1 - sum2) * play;
    return res;
}
#include <cassert>
#include <vector>
#include <utility>

// Declare the function from the solution (already included above)

int main() {
    // Initial board setup
    int board1[8][8];
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            board1[i][j] = 16581375;
    board1[3][3] = 255; board1[3][4] = 0;
    board1[4][4] = 255; board1[4][3] = 0;
    
    // Mode 1: disc count difference (white has 2, black has 2 initially)
    assert(evaluateBoard(board1, 1, 1) == 0.0);
    assert(evaluateBoard(board1, -1, 1) == 0.0);
    
    // Mode 2: positional weight difference (same initial)
    assert(evaluateBoard(board1, 1, 2) == 0.0);
    
    // Mode 3: no corners yet, so same as disc count
    assert(evaluateBoard(board1, 1, 3) == 0.0);
    
    // Mode 4: initial mobility (both players have 4 moves), so still 0
    assert(evaluateBoard(board1, 1, 4) == 0.0);
    assert(validateMoves(board1, 1).size() == 4);  // Check helper works
    
    // Test with a board where white has more pieces
    int board2[8][8];
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            board2[i][j] = 16581375;
    board2[3][3] = 255; board2[3][4] = 255; board2[4][4] = 255; board2[4][3] = 0;
    
    // Mode 1: white has 3, black has 1 → white perspective gives +2
    assert(evaluateBoard(board2, 1, 1) == 2.0);
    assert(evaluateBoard(board2, -1, 1) == -2.0);
    
    // Mode 2: white total weight = 6 (center) +? Actually compute manually:
    // positions: (3,3)=6, (3,4)=5, (4,4)=6; black at (4,3)=5
    // white: 6+5+6=17, black: 5 → difference = 12
    assert(evaluateBoard(board2, 1, 2) == 12.0);
    
    // Mode 3: corner control test
    int board3[8][8];
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            board3[i][j] = 16581375;
    board3[0][0] = 255; board3[0][7] = 0;
    board3[3][3] = 255; board3[3][4] = 0;
    // White has corners: (0,0), plus 2 discs → disc count 3, black has 2
    // Mode 3: white sum = 3 + 10 (corner) = 13, black = 2 → diff = 11
    assert(evaluateBoard(board3, 1, 3) == 11.0);
    
    // Mode 5: composite (disc count + weight + corners + mobility)
    // For a simple case, we can just sanity check it returns a plausible value
    double composite = evaluateBoard(board2, 1, 5);
    assert(composite > 0);  // White ahead should give positive score
    
    std::vector<std::pair<int,int>> moves = validmoves(board1, 1);
    assert(moves.size() == 4);
    
    return 0;
}
