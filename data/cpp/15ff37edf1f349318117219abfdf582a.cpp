Write a C++ function `bool canSolvePuzzle(const std::vector<std::vector<int>>& start, const std::vector<std::vector<int>>& goal)` that determines whether an `n x n` sliding puzzle (where `n >= 1` and the board contains exactly one `0` representing the blank tile, with all other tiles being distinct integers from `1` to `n*n - 1`) can be solved from the given `start` configuration to reach the `goal` configuration by sliding tiles into the blank. The board is represented as a vector of vectors of ints. The function should return `true` if a sequence of legal moves (up/down/left/right swaps with the blank) exists, and `false` otherwise. Do not attempt to find the sequence, just decide solvability. The input boards are assumed to be valid (same dimensions, same set of tiles exactly once, single zero). Use the parity-based invariant: for an `n x n` board, the puzzle is solvable if and only if the parity of the number of inversions (counting pairs of tiles where a higher-numbered tile appears before a lower-numbered tile, ignoring the blank) plus the row number of the blank (counting from the bottom, 1-indexed) is even when `n` is even, or if the number of inversions is even when `n` is odd. Your function should be `const`-correct and work for any `n`.
The solution relies on the well-known invariant of the sliding puzzle: certain configurations are unreachable from each other. We compare the parity of the starting and goal configurations. For any board, define the inversion count as the number of pairs (i,j) with i<j such that the tile at position i (in row-major order, ignoring the zero) is greater than the tile at position j. Then, for an `n x n` board:
- If `n` is odd, the parity of the inversion count is invariant under any legal move.
- If `n` is even, the parity of `(inversion count + row of blank from bottom, 1-indexed)` is invariant.
Two configurations are mutually reachable if and only if they have the same invariant parity. Therefore, we compute the inversion count for both `start` and `goal` (ignoring the zero), find the blank's row from the bottom for each, and then compare their parities. Edge cases: `n=1` has no tiles other than the zero, so inversion count is zero and it's trivially solvable; boards may have any dimension, but the function assumes valid input. The inversion count can be computed in O(n^2) time by flattening all non-zero tiles into a vector and counting inversions using a nested loop (or using O(n log n) with merge sort, but n is typically small, so O(n^2) is acceptable). Space complexity is O(n^2) for the flattened vector. The time complexity is O(n^4) in the worst case for a naive O(n^2) inversion count on O(n^2) tiles, but for typical puzzle sizes (3x3, 4x4) it's trivial. We can also use a Fenwick tree for O(n^2 log n) but the simple approach is clearer for a teaching task.
#include <vector>
#include <algorithm>

// Determine if a sliding puzzle is solvable based on parity invariants.
bool canSolvePuzzle(const std::vector<std::vector<int>>& start,
                    const std::vector<std::vector<int>>& goal) {
    const int n = static_cast<int>(start.size());
    if (n == 0) return true; // edge case, but assume valid

    // Helper lambda to compute inversion count (ignoring zero) and blank row from bottom.
    auto computeInvAndBlankRow = [n](const std::vector<std::vector<int>>& board) {
        std::vector<int> tiles;
        int blankRowFromBottom = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (board[i][j] == 0) {
                    blankRowFromBottom = n - i; // row index from bottom, 1-indexed
                } else {
                    tiles.push_back(board[i][j]);
                }
            }
        }
        int inv = 0;
        for (size_t i = 0; i < tiles.size(); ++i) {
            for (size_t j = i + 1; j < tiles.size(); ++j) {
                if (tiles[i] > tiles[j]) ++inv;
            }
        }
        return std::make_pair(inv, blankRowFromBottom);
    };

    auto startInfo = computeInvAndBlankRow(start);
    auto goalInfo = computeInvAndBlankRow(goal);

    // Determine parity based on n.
    bool startParity;
    bool goalParity;
    if (n % 2 == 1) {
        startParity = (startInfo.first % 2 == 0);
        goalParity = (goalInfo.first % 2 == 0);
    } else {
        startParity = ((startInfo.first + startInfo.second) % 2 == 0);
        goalParity = ((goalInfo.first + goalInfo.second) % 2 == 0);
    }
    return startParity == goalParity;
}
#include <cassert>
#include <vector>

// Assume the solution function is defined above.

int main() {
    // Example 3x3 solvable: the classic 8-puzzle from the snippet.
    std::vector<std::vector<int>> start1 = {{1,5,3},{2,4,0},{7,8,6}};
    std::vector<std::vector<int>> goal1 = {{1,2,3},{4,5,6},{7,8,0}};
    assert(canSolvePuzzle(start1, goal1) == true);

    // Same start but goal is a swapped pair (should be unsolvable for odd n).
    std::vector<std::vector<int>> goal2 = {{1,2,3},{4,5,6},{8,7,0}};
    assert(canSolvePuzzle(start1, goal2) == false);

    // 2x2 example: n even, check parity condition.
    std::vector<std::vector<int>> start2 = {{0,1},{3,2}};
    std::vector<std::vector<int>> goal3 = {{1,2},{3,0}};
    // For start2: tiles [1,3,2], inversions: (3,2) -> 1; blank at row 2 (from bottom) -> total 3 (odd) -> unsolvable parity?
    // Actually compute: n=2 even, condition: (inv + blankRow)%2 must match.
    // start2: inv=1, blankRow=2 -> sum=3 odd. goal3: tiles [1,2,3], inv=0, blankRow=1 -> sum=1 odd -> should be solvable? 
    // Let's just test that it matches the function's logic; but we assert true because invariants match.
    assert(canSolvePuzzle(start2, goal3) == true);

    // 2x2 unsolvable: swap two adjacent tiles.
    std::vector<std::vector<int>> start3 = {{0,1},{2,3}};
    std::vector<std::vector<int>> goal4 = {{0,2},{1,3}};
    // start3: inv=0, blankRow=2 -> sum=2 even. goal4: tiles [2,1,3], inv=1, blankRow=2 -> sum=3 odd -> different parity.
    assert(canSolvePuzzle(start3, goal4) == false);

    // 1x1 trivial always solvable.
    std::vector<std::vector<int>> start4 = {{0}};
    std::vector<std::vector<int>> goal5 = {{0}};
    assert(canSolvePuzzle(start4, goal5) == true);

    // 4x4 example: even n, use a known solvable pair.
    std::vector<std::vector<int>> start5 = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,0}};
    std::vector<std::vector<int>> goal6 = start5; // obviously solvable
    assert(canSolvePuzzle(start5, goal6) == true);

    // Same but with a single tile swap (unsolvable in 4x4 if parity changes? Let's just test the logic).
    std::vector<std::vector<int>> goal7 = start5;
    std::swap(goal7[0][0], goal7[0][1]); // swap 1 and 2
    // For start5: inv=0 (already sorted), blankRow=1 -> sum=1 odd. goal7: tiles order: 2,1,3,4,... -> one inversion -> inv=1, blankRow=1 -> sum=2 even -> different parity.
    assert(canSolvePuzzle(start5, goal7) == false);

    return 0;
}
