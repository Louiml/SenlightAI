You are given an array of gem positions on a 1D line, where each position is an integer between 1 and 30000 (inclusive). You start at position `d` (1 ≤ d ≤ 30000) and on your first move you jump exactly `d` units to the right. After that, if your previous jump length was `j`, your next jump must be of length `j-1`, `j`, or `j+1`, provided the new position stays within the line (i.e., position ≤ 30000). When you land on a position, you collect all gems there (multiple gems can be at the same position). Write a C++ function `int maxGemsCollected(const std::vector<int>& gemPositions, int d)` that returns the maximum number of gems you can collect along a valid path. You may stop at any time; you do not need to use all possible jumps. The number of gems `n` is at least 1, and each gem position is within [1, 30000]. The jump length can never go below 1 and cannot exceed 30000, but in practice the maximum useful jump length is bounded by about 250 because the total line length is small (more on that in analysis). The initial jump length must be exactly `d`.
// This is a dynamic programming problem on a DAG (directed acyclic graph) because all jumps move strictly to the right (positive jump lengths). The state is `(position, lastJumpLength)`. From a state, you can transition to three possible next states: `(position + (lastJump-1), lastJump-1)` if lastJump>1, `(position + lastJump, lastJump)`, and `(position + (lastJump+1), lastJump+1)`. Since positions are bounded by 30000, we can memoize. However, the jump length can be large if `d` is large, but note that the maximum reachable position is 30000, and the number of possible distinct jump lengths that actually matter is small: if the jump length ever exceeds about 250, you’d need to sum 250+251+... which quickly exceeds 30000 when starting from position 1. In fact, the maximum useful jump length is at most about 250 for any starting position (because the total sum of a sequence of increasing jump lengths from d must fit within 30000). Therefore we offset the jump length dimension to handle negative indices in DP table. We use a 2D table `dp[pos][jump+off]` where `off` is a constant offset (e.g., 250) to store the maximum gems from that state. The recursion: `dp[pos][j] = g[pos] + max( (if valid) dp[pos+j-1][j-1], dp[pos+j][j], dp[pos+j+1][j+1] )`, where `g[pos]` is the number of gems at that position. If no transition is valid, the value is just `g[pos]`. The answer is `dp[d][d]`. Since we only move forward, there are no cycles. Time complexity: O(30000 * MAXJUMP) where MAXJUMP is about 500 (to accommodate offset range), but in practice the reachable states are far fewer. Space complexity is O(30000 * 507) which is about 15 million integers, acceptable. Edge case: when `d` is very large (e.g., 30000), the only move you can take is to jump 30000, landing at 60000 which is out of bounds, so you collect only the gems at the starting position. The function handles that by only considering valid transitions.
#include <vector>
#include <cstring>
#include <algorithm>

const int MAX_POS = 30000;
const int OFFSET = 250; // because maximum useful jump length is ~250

// Return the maximum number of gems collectible starting at position startPos
// with first jump length startJump. gemCount[pos] = number of gems at position pos.
int maxGemsCollected(const std::vector<int>& gemPositions, int d) {
    // Build gem count array
    std::vector<int> gemCount(MAX_POS + 1, 0);
    for (int pos : gemPositions) {
        ++gemCount[pos];
    }

    // DP table: dp[pos][jump + OFFSET] = max gems from state (pos, jump)
    // Initialize with -1 to indicate uncomputed.
    static int dp[MAX_POS + 1][2 * OFFSET + 3];
    memset(dp, -1, sizeof(dp));

    // Recursive lambda with memoization
    std::function<int(int, int)> solve = [&](int pos, int jump) -> int {
        // Out of valid range? Should not be called with invalid pos, but guard.
        if (pos < 1 || pos > MAX_POS) return 0;
        // Use a reference to the memo cell
        int &memo = dp[pos][jump + OFFSET];
        if (memo != -1) return memo;

        // Start with gems at current position
        int best = gemCount[pos];

        // Try jump-1
        if (jump > 1 && pos + (jump-1) <= MAX_POS) {
            best = std::max(best, gemCount[pos] + solve(pos + (jump-1), jump-1));
        }
        // Try jump
        if (pos + jump <= MAX_POS) {
            best = std::max(best, gemCount[pos] + solve(pos + jump, jump));
        }
        // Try jump+1
        if (pos + (jump+1) <= MAX_POS) {
            best = std::max(best, gemCount[pos] + solve(pos + (jump+1), jump+1));
        }

        memo = best;
        return memo;
    };

    return solve(d, d);
}
#include <cassert>
#include <vector>
#include <iostream>

int maxGemsCollected(const std::vector<int>& gemPositions, int d);

int main() {
    // Test 1: Single gem at start
    assert(maxGemsCollected({5}, 5) == 1);
    // Test 2: Gems at positions reachable by constant jumps
    assert(maxGemsCollected({2, 4, 6}, 2) == 3);
    // Test 3: Gems requiring variable jumps
    // d=1, path: 1 -> 2 (j+1=2) -> 4 (j+2? no, j=2 so jump+1=3 lands at 5) Actually careful.
    // Let's design: d=2, gem at 2 (start), 5 (jump 3), 9 (jump 4) => total 3
    assert(maxGemsCollected({2, 5, 9}, 2) == 3);
    // Test 4: Duplicate gems at same position
    assert(maxGemsCollected({3, 3, 3, 4}, 3) == 4); // collect 3 at 3, then jump 1 to 4 collect 1
    // Test 5: Large jump overshoots board -> only start gems
    assert(maxGemsCollected({30000}, 30000) == 1);
    // Test 6: Best path might not take all gems
    // d=1, gems at 1 and 3. Path 1->2 (jump1) collect 1, then stop. Could also go 1->3 (jump2) collect 1. Either way max=2.
    assert(maxGemsCollected({1, 3}, 1) == 2);
    // Test 7: No possible move after start, only start gem
    assert(maxGemsCollected({10}, 10) == 1);
    // Test 8: Gems along a decreasing jump sequence
    // d=2, gems at 2, 4 (jump2), 5 (jump1 from 4) => total 3
    assert(maxGemsCollected({2, 4, 5}, 2) == 3);
    // Test 9: Many gems but best path avoids some
    // d=1, gems at 1,2,3,4,5. Can collect all by jumps 1,1,1... => 5
    assert(maxGemsCollected({1,2,3,4,5}, 1) == 5);
    std::cout << "All tests passed.\n";
    return 0;
}
