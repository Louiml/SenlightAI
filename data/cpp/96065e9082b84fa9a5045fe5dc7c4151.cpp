/*
Write a C++ function `countGamePlans` that simulates a simplified two-player turn-based game on a 1D board of `n` cells, where each cell contains a non-negative integer. A move consists of selecting one cell, reducing its value by exactly 1 (if it is positive), and then moving a token one step to the right (if possible) to the next cell; if the token is at the last cell, it wraps around to the first. The game starts with the token at cell 0 and alternates turns between players. The game ends when no cell has a positive value (i.e., all cells are zero). The function takes the initial board as a `std::vector<int>` and returns the total number of distinct move sequences that lead to the game ending, where a sequence is defined by the exact order of moves (which cell is decremented at each turn). The game is deterministic: at each turn, the player must make a legal move (decrement a positive cell); if multiple cells are positive, any can be chosen. The function should count all possible sequences of moves from the initial state until all cells are zero, regardless of whose turn it is (i.e., we are only counting the number of legal move sequences, not considering win/loss). The board size `n` can be at most 5, and each cell's initial value is at most 3, so the total number of moves is at most 15. Use recursion with memoization on the board state and current token position. The function signature is `std::uint64_t countGamePlans(const std::vector<int>& board);`. Note that the token position does not affect the legality of moves (only which cells are positive), but it does affect the state because the token wraps around; however, in this simplified version, the token position does not influence the moves at all—it is irrelevant to the game logic. Therefore, for this task, ignore the token: a move is simply decrementing any positive cell by 1. The game ends when all cells are zero. Count the number of sequences of decrements (order matters) that reduce the board to all zeros.
*/

#include <cstdint>
#include <vector>
#include <map>

// Count the number of distinct move sequences that reduce the board to all zeros.
// A move decrements any positive cell by 1. The order of moves matters.
std::uint64_t countGamePlans(const std::vector<int>& board) {
    // Memoization map: state vector -> number of sequences
    static std::map<std::vector<int>, std::uint64_t> memo;

    // If all cells are zero, exactly one sequence (empty) is possible.
    bool all_zero = true;
    for (int value : board) {
        if (value != 0) {
            all_zero = false;
            break;
        }
    }
    if (all_zero) {
        return 1;
    }

    // Check memo
    auto it = memo.find(board);
    if (it != memo.end()) {
        return it->second;
    }

    std::uint64_t total = 0;
    for (std::size_t i = 0; i < board.size(); ++i) {
        if (board[i] > 0) {
            // Make a copy and decrement this cell
            std::vector<int> next = board;
            --next[i];
            total += countGamePlans(next);
        }
    }

    memo[board] = total;
    return total;
}

#include <cassert>
#include <vector>
#include <cstdint>

int main() {
    // Empty board: no moves, one sequence
    assert(countGamePlans({}) == 1);
    // All zeros: one sequence
    assert(countGamePlans({0, 0, 0}) == 1);
    // Single cell with value 1: one sequence (decrement once)
    assert(countGamePlans({1}) == 1);
    // Single cell with value 3: only one sequence (three identical decrements)
    assert(countGamePlans({3}) == 1);
    // Two cells [1,1]: two sequences (cell0 first or cell1 first)
    assert(countGamePlans({1, 1}) == 2);
    // [2,1]: 3!/(2!*1!) = 3
    assert(countGamePlans({2, 1}) == 3);
    // [2,2]: 4!/(2!*2!) = 6
    assert(countGamePlans({2, 2}) == 6);
    // [1,2,1]: 4!/(1!*2!*1!) = 12
    assert(countGamePlans({1, 2, 1}) == 12);
    // [3,0,1]: 4!/(3!*0!*1!) = 4
    assert(countGamePlans({3, 0, 1}) == 4);
    // Maximum small case: [3,3,3,3,3] -> 15!/(3!^5) = 168168000? Let's compute approximate: 15! = 1307674368000, 3!^5 = 7776, quotient = 168168000. Check exact.
    assert(countGamePlans({3, 3, 3, 3, 3}) == 168168000ULL);
    return 0;
}

// The problem reduces to counting the number of permutations of the multiset of decrements, where each decrement corresponds to reducing a specific cell by 1. For a board with values `v_i`, the total number of moves is `M = sum(v_i)`. Each distinct sequence of moves is a sequence of length `M` where cell `i` appears exactly `v_i` times. The number of distinct sequences is the multinomial coefficient: `M! / (v_0! * v_1! * ... * v_{n-1}!)`. For example, if the board is `[2,1]`, then M=3, and sequences are: decrement cell0, cell0, cell1 (order among identical operations matters? Actually, since operations on the same cell are identical, the distinct sequences are determined by positions of cell0 vs cell1, which is `3!/(2!*1!)=3`. So the answer is the multinomial coefficient. However, the problem statement mentions recursion and memoization, but due to the token being irrelevant, a direct combinatorial formula suffices. But to follow the spirit, we can implement a recursive function that counts sequences: at each step, choose a positive cell, decrement, recurse. Without memoization, this would be exponential in the number of moves, but with small constraints (max 15 moves), it is feasible. However, the task asks for a function that returns a `uint64_t`, and the reference solution can use memoization on the state vector to avoid redundant computations (e.g., `[1,2]` reached via different orders). Memoization key can be the vector itself (since the token is ignored). The state space is small: each cell value between 0 and 3, n≤5, so at most `4^5=1024` states. The base case is all zeros → return 1 (one way to do nothing). For each positive cell, recursively count for decremented state, sum. This gives the multinomial coefficient. Time complexity: O(number of states * n) = O(1024*5) = ~5k operations, but without memoization it would be O(M! / product(v_i!)) which can be large (like 15! / (3!^5) ≈ 168 million? Actually 15! is 1.3e12, divided by 6^5≈7776 → ~1.68e8, still large but feasible? But with memoization it is trivial). We'll use memoization. Edge cases: empty board (n=0) → return 1 (no moves). Board with zeros only → return 1. Values up to 3, so 64-bit is safe (max multinomial for 15 items with 5 groups of 3 is ~168 million, fits in uint64_t).
