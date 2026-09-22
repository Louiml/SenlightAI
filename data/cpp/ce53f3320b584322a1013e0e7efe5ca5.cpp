/*
You are given `n` distinct stone-pile sizes (each between 1 and 100) and a target number of stones `k` (between 1 and 100,000). Two players play a game where they alternately remove exactly one of the given pile sizes from the current total; the player who cannot make a legal move (i.e., the current total is smaller than every available pile size) loses. Assuming both play optimally and the first player starts with `k` stones, write a C++ function `std::string gameWinner(int n, int k, const std::vector<int>& moves)` that returns `"First"` if the first player has a winning strategy, or `"Second"` otherwise. The pile sizes may not be sorted, but the function must handle them correctly regardless of order.
*/

#include <vector>
#include <string>
#include <algorithm>

std::string gameWinner(int n, int k, const std::vector<int>& moves) {
    // Create a copy to sort without modifying the input.
    std::vector<int> sortedMoves(moves.begin(), moves.begin() + n);
    std::sort(sortedMoves.begin(), sortedMoves.end());
    
    // If the smallest move exceeds k, first player has no legal move.
    if (sortedMoves[0] > k) {
        return "Second";
    }
    
    // dp[i] is true if the player to move with i stones wins.
    std::vector<bool> dp(k + 1, false);
    
    // Fill states from the smallest possible move upward.
    for (int i = sortedMoves[0]; i <= k; ++i) {
        bool canWin = false;
        for (int m : sortedMoves) {
            if (m > i) break;  // moves are sorted, so stop early.
            if (!dp[i - m]) {
                canWin = true;
                break;
            }
        }
        dp[i] = canWin;
    }
    
    return dp[k] ? "First" : "Second";
}

#include <cassert>
#include <vector>
#include <string>

// Declaration of the solution function (assume it is defined above).
std::string gameWinner(int n, int k, const std::vector<int>& moves);

int main() {
    // Example from the snippet: n=2, moves {1,3}, k=5 → First
    assert(gameWinner(2, 5, {1, 3}) == "First");
    
    // n=2, moves {1,3}, k=4 → Second (both moves lead to winning states for opponent)
    assert(gameWinner(2, 4, {1, 3}) == "Second");
    
    // Unsorted moves, k=2, moves {2,1} → First (remove 2 directly)
    assert(gameWinner(2, 2, {2, 1}) == "First");
    
    // Single move of 5, k=4 → Second (no legal move)
    assert(gameWinner(1, 4, {5}) == "Second");
    
    // Single move of 5, k=5 → First
    assert(gameWinner(1, 5, {5}) == "First");
    
    // Moves {2,4}, k=1 → Second (no move possible)
    assert(gameWinner(2, 1, {2, 4}) == "Second");
    
    // Moves {2,4}, k=3 → Second (only 2 possible → leads to dp[1]=false, so opponent wins? Actually check: remove 2 → opponent at 1 cannot move → opponent loses, so first wins)
    // Wait: At 3, only legal move is 2 (since 4>3). Then dp[3-2]=dp[1]=false, so dp[3]=true → First.
    assert(gameWinner(2, 3, {2, 4}) == "First");
    
    // Moves {2,4}, k=6 → First (remove 4 → dp[2]=true? need careful: dp[2] win? At 2, remove 2 → dp[0]=false, so dp[2]=true. So from 6, remove 4 → opponent at 2 wins? That's bad. Remove 2 → opponent at 4: from 4, remove 2 → dp[2]=true, remove 4→dp[0]=false so dp[4]=true. So both moves lead to opponent winning → dp[6]=false → Second.)
    assert(gameWinner(2, 6, {2, 4}) == "Second");
    
    // Larger random check: k=10, moves {1,2} → always First because can leave odd/even? Actually with 1 and 2, the first player can always win by leaving 0, so always First.
    assert(gameWinner(2, 10, {1, 2}) == "First");
    
    // k=0 not allowed per constraints, but just in case: smallest move >0, so dp[0]=false → Second
    // test function with k=0? Not needed, but could add:
    // assert(gameWinner(2, 0, {1,2}) == "Second");
    
    return 0;
}

// This is a classic impartial combinatorial game solvable via dynamic programming on the remaining stone count. Define `dp[i]` as `true` if the player whose turn it is when `i` stones remain has a winning strategy, and `false` otherwise. Base case: for any `i < min(moves)`, no move is possible, so that player loses → `dp[i] = false`. For each `i` from the smallest move up to `k`, we try every legal move `m` such that `m ≤ i`. If there exists a move that leads to a losing state for the opponent (i.e., `dp[i - m] == false`), then the current player can force a win, so `dp[i] = true`; if no such move exists, `dp[i] = false`. The answer is `dp[k]`. Important edge cases: ensure we handle unsorted input, duplicate values (though problem says distinct, but we can tolerate them), and the case where the smallest move is greater than `k` — then the first player loses immediately, so return `"Second"`. Time complexity is `O(k * n)` because for each of `k+1` states we iterate over at most `n` moves (filtering those exceeding `i`). Space complexity is `O(k)` for the DP array. The algorithm is correct because the game is finite and acyclic (total stones strictly decrease), so dynamic programming works.
