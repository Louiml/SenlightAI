// Write a C++ function `maxScore(int n, int m, int k, const std::vector<int>& turns)` that simulates a board game with `k` tokens (numbered 1 through `k`), each initially positioned at square 1. There are `n` turns, where turn `i` (0-indexed) uses the value `turns[i]` to move one token forward by that many squares. A token’s position is capped at `m` (if it would exceed `m`, it stays at `m`). After all `n` turns are processed, the score is the number of tokens located exactly at square `m`. The game allows choosing, for each turn independently, which of the `k` tokens to move. The function must return the maximum possible score achievable over all sequences of token choices (one choice per turn). The input vector `turns` has length `n`, and all values are positive integers. The function should not modify the input vector. Assume `n >= 1`, `k >= 1`, and `m >= 1`.
// The problem is a classic exhaustive search problem where we need to try all possible assignments of moves to tokens. Since for each turn there are `k` choices, the total number of combinations is `k^n`. For small constrain (e.g., `n ≤ 10`, `k ≤ 3`), brute-force recursion works fine. The solution uses a recursive function `dfs` that takes the current turn index and a reference to the current positions vector (or use a copy to avoid mutation issues). At each step, for each token index, we tentatively move that token by `turns[turnIndex]`, cap the position at `m`, recurse to the next turn, then undo the move (backtracking). At the base case (`turnIndex == n`), we count how many tokens are at exactly `m` and update the maximum. Edge cases: if `m` is already 1 and tokens start at 1, but turns are positive so they move away, so no token starts at `m` unless `m == 1`; if `m == 1`, then after first move all tokens leave 1 and might never return, so score could be 0 unless a turn moves a token to exactly 1 (impossible since positive), so score is 0. Also, if a token reaches `m` early, moving it again might keep it at `m` (if positive move beyond), so it stays at `m` and counts toward final score – that’s fine. The recursion explores all `k^n` paths. Time complexity: O(k^n * n) (each leaf does O(k) counting, internal nodes O(k) branching). Space complexity: O(n) for recursion stack plus O(k) for positions. For larger constraints, dynamic programming or pruning could be needed, but for a typical exercise, brute-force is acceptable.
#include <vector>
#include <algorithm>
#include <functional>

int maxScore(int n, int m, int k, const std::vector<int>& turns) {
    std::vector<int> pos(k, 1);
    int best = 0;

    std::function<void(int)> dfs = [&](int turnIdx) {
        if (turnIdx == n) {
            int score = 0;
            for (int p : pos) {
                if (p == m) ++score;
            }
            best = std::max(best, score);
            return;
        }

        int moveVal = turns[turnIdx];
        for (int token = 0; token < k; ++token) {
            int prev = pos[token];
            pos[token] = std::min(m, pos[token] + moveVal);
            dfs(turnIdx + 1);
            pos[token] = prev;
        }
    };

    dfs(0);
    return best;
}
#include <cassert>
#include <vector>

int maxScore(int n, int m, int k, const std::vector<int>& turns);

int main() {
    // Example 1: Simple case
    assert(maxScore(2, 5, 2, {2, 3}) == 1);

    // Example 2: All moves can reach the goal with one token
    assert(maxScore(3, 10, 1, {5, 3, 2}) == 1);

    // Example 3: Multiple tokens can all reach goal
    assert(maxScore(2, 3, 3, {2, 1}) == 3);

    // Example 4: No token can reach goal
    assert(maxScore(2, 10, 2, {1, 1}) == 0);

    // Example 5: Goal is 1, tokens start at 1 but moves are positive, so never stay at 1
    assert(maxScore(1, 1, 2, {1}) == 0);

    // Example 6: Cap at m; moving beyond still counts
    assert(maxScore(1, 5, 1, {10}) == 1);

    // Example 7: Larger k and n, with one token reaching goal
    assert(maxScore(4, 5, 3, {2, 2, 2, 2}) == 1);

    // Example 8: All tokens can be pushed to goal in different turns
    assert(maxScore(4, 4, 2, {3, 1, 3, 1}) == 2);

    // Example 9: Mixed, only some tokens can reach
    assert(maxScore(3, 6, 3, {2, 2, 2}) == 3);

    // Example 10: Edge case n=1, k=1, m large
    assert(maxScore(1, 100, 1, {99}) == 0);
    return 0;
}
