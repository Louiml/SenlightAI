Write a C++ function `vector<vector<int>> queensAttackTheKing(vector<vector<int>>& queens, vector<int>& king)` that, given a list of queen positions and the king's position on an 8×8 chessboard, returns a vector of all queen positions that can attack the king in a single move. A queen attacks along rows, columns, and diagonals in all 8 directions, but is blocked by any other piece (including other queens) in its path. The input positions are 0-indexed coordinates `[row, col]`. The board is always 8×8, and the king's position is distinct from all queen positions. Edge cases include queens that are on the same line but blocked by another queen, and cases where no queen can attack. The function must not modify the input vectors and should handle duplicate queen positions gracefully (though the problem guarantees unique queen positions).

#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Test 1: Basic case - one queen in each direction
    {
        std::vector<std::vector<int>> queens = {{0,0},{0,7},{7,0},{7,7},{0,3},{3,0},{3,7},{7,3}};
        std::vector<int> king = {4,4};
        auto result = queensAttackTheKing(queens, king);
        std::vector<std::vector<int>> expected = {{0,0},{0,7},{7,0},{7,7},{0,3},{3,0},{3,7},{7,3}};
        // Sort both to compare regardless of order
        std::sort(result.begin(), result.end());
        std::sort(expected.begin(), expected.end());
        assert(result == expected);
    }

    // Test 2: Queen blocked by another queen on same line
    {
        std::vector<std::vector<int>> queens = {{1,0},{2,0}};
        std::vector<int> king = {4,0};
        auto result = queensAttackTheKing(queens, king);
        // Only the closest one (1,0) attacks (same column, distance 3) – (2,0) is blocked
        std::vector<std::vector<int>> expected = {{1,0}};
        assert(result == expected);
    }

    // Test 3: No queen can attack
    {
        std::vector<std::vector<int>> queens = {{0,0}};
        std::vector<int> king = {5,5};
        auto result = queensAttackTheKing(queens, king);
        assert(result.empty());
    }

    // Test 4: Queen adjacent to king
    {
        std::vector<std::vector<int>> queens = {{4,5}};
        std::vector<int> king = {4,4};
        auto result = queensAttackTheKing(queens, king);
        std::vector<std::vector<int>> expected = {{4,5}};
        assert(result == expected);
    }

    // Test 5: Multiple directions with multiple queens, only nearest per direction
    {
        std::vector<std::vector<int>> queens = {{0,0},{1,1},{2,2},{0,3},{5,3}};
        std::vector<int> king = {4,4};
        // (2,2) is on diagonal up-left (distance 2) but (1,1) is closer (dist 3?) Actually (1,1) dist max(|3|,|3|)=3, (2,2) dist 2 – so (2,2) blocks others? No, on that diagonal, closest to king is (2,2) as it's nearer than (1,1) and (0,0). So only (2,2) from that diagonal.
        // (0,3) same row? dr=4, dc=-1 – not same row/col/diag. (5,3) dr=1, dc=-1 – diagonal down-left? dr=1, dc=-1 => abs equal 1 – yes. So direction down-left. Only (5,3) there.
        auto result = queensAttackTheKing(queens, king);
        std::vector<std::vector<int>> expected = {{2,2},{5,3}};
        std::sort(result.begin(), result.end());
        std::sort(expected.begin(), expected.end());
        assert(result == expected);
    }

    // Test 6: Edge of board
    {
        std::vector<std::vector<int>> queens = {{0,0}};
        std::vector<int> king = {7,7};
        auto result = queensAttackTheKing(queens, king);
        std::vector<std::vector<int>> expected = {{0,0}};
        assert(result == expected);
    }

    return 0;
}

#include <vector>
#include <cmath>
#include <limits>

// Determine which queens can attack the king in a single move on an 8x8 board.
// Queens attack along rows, columns, and diagonals, but are blocked by any other piece in between.
std::vector<std::vector<int>> queensAttackTheKing(const std::vector<std::vector<int>>& queens,
                                                  const std::vector<int>& king) {
    const int N = 8;
    // nearest[dir] = index of closest queen in that direction, or -1 if none.
    // Directions: 0=up,1=down,2=left,3=right,4=up-left,5=up-right,6=down-left,7=down-right
    std::vector<int> nearest(8, -1);
    // Precompute distance to compare: use Chebyshev distance (max of row/col differences)
    // For a given direction, we just need to compare max(|dr|,|dc|)

    for (int idx = 0; idx < (int)queens.size(); ++idx) {
        int qr = queens[idx][0];
        int qc = queens[idx][1];
        int kr = king[0];
        int kc = king[1];

        int dr = qr - kr;
        int dc = qc - kc;

        // Skip if not on a line through the king (same row, col, or diagonal slope ±1)
        if (dr == 0 && dc == 0) continue; // queen can't be on king per problem, but safe
        if (dr == 0 || dc == 0 || std::abs(dr) == std::abs(dc)) {
            // Determine direction index
            int dir;
            if (dr == 0 && dc > 0) dir = 2;       // left
            else if (dr == 0 && dc < 0) dir = 3;  // right (since col increases to the right)
            else if (dc == 0 && dr > 0) dir = 1;  // down (row increases downward)
            else if (dc == 0 && dr < 0) dir = 0;  // up
            else if (dr < 0 && dc < 0) dir = 4;   // up-left
            else if (dr < 0 && dc > 0) dir = 5;   // up-right
            else if (dr > 0 && dc < 0) dir = 6;   // down-left
            else dir = 7;                         // down-right

            // Distance from king (Chebyshev)
            int dist = std::max(std::abs(dr), std::abs(dc));

            // If no queen in this direction yet, or this one is closer
            if (nearest[dir] == -1) {
                nearest[dir] = idx;
            } else {
                int prevIdx = nearest[dir];
                int prevDist = std::max(std::abs(queens[prevIdx][0] - kr), std::abs(queens[prevIdx][1] - kc));
                if (dist < prevDist) {
                    nearest[dir] = idx;
                }
            }
        }
    }

    std::vector<std::vector<int>> result;
    for (int idx : nearest) {
        if (idx != -1) {
            result.push_back(queens[idx]);
        }
    }
    return result;
}

// The straightforward approach is to simulate the board and perform a directional search from the king. For each of the 8 possible directions, move step-by-step from the king until either: (1) we go off the board, (2) we hit a queen (then that queen is the first one in that direction and can attack), or (3) we hit another piece (but since positions only contain queens and the king, we only stop at queens or off-board). However, a cleaner method: for each queen, check if it lies on one of the 8 lines passing through the king (same row, same column, or same diagonal with slope ±1). If it does, then it can attack only if no other queen lies between it and the king on that same line. Thus, for each of the 8 directions from the king, collect all queens lying on that ray, and keep the closest one (smallest Manhattan or Chebyshev distance). Any queen that is the closest in its direction attacks the king. This avoids explicit path traversal. Complexity: Let Q be the number of queens. Checking each queen for direction and distance takes O(Q) time; storing the nearest per direction takes O(1) extra space (8 slots). Thus total time O(Q), space O(1) besides input and output. Edge cases: queen exactly adjacent to king (distance 1) will be the closest; multiple queens on same line – only the nearest one attacks; queen on same row but blocked by another queen closer to king – the closer one attacks, the farther one does not.
