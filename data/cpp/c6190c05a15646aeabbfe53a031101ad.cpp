// Write a C++ function `long long maxCoinsAfterFlips(int n, const std::vector<long long>& coinValues, const std::vector<std::pair<int, long long>>& bonuses)` that simulates a game where you have `n` coins in a row, each with a value given in `coinValues` (1-indexed for convenience). You must pick a non-empty contiguous subsequence of coins (i.e., you flip exactly one contiguous segment of coins) and collect their total value. Additionally, there are `m` special counters (given in `bonuses` as pairs `(counter_index, bonus_value)`). If, after picking a segment, the number of coins you have picked is exactly `c` (where `c` matches one of the special counter indices), then you add the corresponding `bonus_value` to your score. More precisely, the total score for picking a contiguous segment of length `L` starting at position `s` is: sum of `coinValues[s..s+L-1]` plus, for every `(c, y)` in `bonuses` with `c == L`, add `y` (note: only one bonus applies per exact length, not per coin). The goal is to return the maximum possible total score over all non-empty contiguous segments. You may assume `n >= 1`, `coinValues` has exactly `n` elements, `bonuses` has exactly `m` elements (given), and each `c` in `bonuses` is between 1 and `n` inclusive, with no duplicate `c` values. The function should handle large values (use `long long`).

// The problem is a generalization of the classic maximum subarray sum (Kadane's algorithm) with a length-dependent bonus. The brute-force approach would be to try all O(n^2) contiguous segments and compute their sum plus the bonus for that length, taking O(n) to compute the sum each time, leading to O(n^3) time, which is too slow for large n. The intended solution uses dynamic programming (DP) similar to the provided snippet: define `dp[i][j]` as the maximum score achievable by considering the first `i` coins (i from 0 to n) and ending with exactly `j` consecutive coins selected (i.e., the last `j` coins are all selected, and if `j == 0`, then the current position is not selected). Transitions:  
// - If you start a new segment at position `i` (i.e., previous had 0 selected), then `dp[i][0]` is max of all `dp[i-1][k]` for any k (meaning you can stop any previous segment and start fresh).  
// - If you continue a segment by selecting the `i`-th coin with `j` selected so far (j≥1), then `dp[i][j] = dp[i-1][j-1] + coinValues[i-1] + bonus[j]` where `bonus[j]` is the bonus for length `j` (0 if not defined).  
// The final answer is the maximum over all `dp[n][j]` for j from 1 to n (excluding j=0 because a non-empty segment is required). This DP has O(n^2) states and O(n^2) transitions (since each state transitions from one previous state). However, we can do O(n^2) time and O(n^2) space, which is fine for n up to ~5000. Edge cases:  
// - The segment must be non-empty, so j cannot be 0 in the final answer.  
// - If no bonus exists for a length, treat bonus as 0.  
// - The DP initializes with `dp[0][0]=0` and all other `dp[0][j]= -∞` (e.g., use a very negative number).  
// - Since we allow starting a new segment at any point, `dp[i][0]` is the maximum score of any completed segment up to position i, but we don't use it directly for the final answer.  
// Correct implementation carefully avoids index out-of-bounds. Time complexity O(n^2), space O(n^2) (could be optimized to O(n) by keeping only two rows, but the task requires a clear and simple solution).

#include <vector>
#include <algorithm>
#include <cstdint>
#include <limits>

// Returns the maximum total score from picking a non-empty contiguous segment,
// where each segment of length L gets a bonus if a bonus is defined for that length.
long long maxCoinsAfterFlips(int n,
                             const std::vector<long long>& coinValues,
                             const std::vector<std::pair<int, long long>>& bonuses) {
    // Build bonus lookup: bonusForLength[L] = bonus value or 0 if not defined.
    std::vector<long long> bonusForLength(n + 1, 0);
    for (const auto& p : bonuses) {
        bonusForLength[p.first] = p.second;
    }

    const long long NEG_INF = std::numeric_limits<long long>::min() / 4;
    // dp[i][j] = max score considering first i coins (1-indexed) ending with j selected.
    // j == 0 means no segment is currently active (or already ended).
    std::vector<std::vector<long long>> dp(n + 1, std::vector<long long>(n + 1, NEG_INF));
    dp[0][0] = 0;

    for (int i = 1; i <= n; ++i) {
        // If we start a fresh segment (or end any previous), dp[i][0] is max of dp[i-1][*]
        long long bestPrev = NEG_INF;
        for (int k = 0; k <= n; ++k) {
            bestPrev = std::max(bestPrev, dp[i-1][k]);
        }
        dp[i][0] = bestPrev;

        // Continue a segment by selecting coin i. j is the new length of the segment.
        for (int j = 1; j <= i; ++j) {
            // To have j selected at i, we must have j-1 selected at i-1.
            if (dp[i-1][j-1] != NEG_INF) {
                dp[i][j] = dp[i-1][j-1] + coinValues[i-1] + bonusForLength[j];
            }
        }
    }

    // Find maximum over all positive lengths (non-empty segment).
    long long answer = NEG_INF;
    for (int j = 1; j <= n; ++j) {
        answer = std::max(answer, dp[n][j]);
    }
    return answer;
}

#include <cassert>
#include <vector>
#include <utility>

// Function declaration (defined elsewhere)
long long maxCoinsAfterFlips(int n,
                             const std::vector<long long>& coinValues,
                             const std::vector<std::pair<int, long long>>& bonuses);

int main() {
    // Test 1: Basic no bonuses, max subarray is 5 (the segment [5] or [2,3]? Actually [5] gives 5, [2,3] gives 5, so answer 5)
    assert(maxCoinsAfterFlips(3, {1, -2, 3}, {}) == 3);
    // Test 2: With a bonus on length 2, segment of length 2 may be better.
    // Values: [1,2], bonus for length 2 = 10 → total = 13. Length 1 max is 2, length 3 total = 1+2+3? actually values [1,2,3] no bonus length 3 → 6. So answer 13.
    assert(maxCoinsAfterFlips(3, {1, 2, 3}, {{2, 10}}) == 13);
    // Test 3: Negative values, bonus on length 1 makes it worthwhile.
    // Values: [-5, -5], bonus for length 1 = 100 → picking one coin gives -5+100=95, picking both gives -10 (no bonus length2). So answer 95.
    assert(maxCoinsAfterFlips(2, {-5, -5}, {{1, 100}}) == 95);
    // Test 4: All negative, no bonus → must pick one (largest negative).
    assert(maxCoinsAfterFlips(3, {-7, -2, -9}, {}) == -2);
    // Test 5: Bonus on length 3 with a large sum.
    // Values [1,2,3] sum 6, bonus length 3 = 20 → total 26, better than any shorter.
    assert(maxCoinsAfterFlips(3, {1, 2, 3}, {{3, 20}}) == 26);
    // Test 6: Bonus on length 2 but best is still length 1.
    // Values [5,-1], bonus length2 = 1 → total 5-1+1=5, length1 max 5. So answer 5.
    assert(maxCoinsAfterFlips(2, {5, -1}, {{2, 1}}) == 5);
    // Test 7: Larger n, ensure DP handles length up to n.
    // n=4, values [1,1,1,1], bonus on length 4 = 100 → total 104.
    assert(maxCoinsAfterFlips(4, {1, 1, 1, 1}, {{4, 100}}) == 104);
    // Test 8: With multiple bonuses, but only the exact length applies.
    // n=3, values [2,2,2], bonuses {{1,5},{2,10},{3,100}} → length 3 gives 6+100=106.
    assert(maxCoinsAfterFlips(3, {2, 2, 2}, {{1,5},{2,10},{3,100}}) == 106);
    // Test 9: Ensure empty segment is not allowed, even if all bonuses are zero and all values negative.
    // n=2, values [-1,-1] → best is -1.
    assert(maxCoinsAfterFlips(2, {-1, -1}, {}) == -1);
    // Test 10: Single element with bonus.
    assert(maxCoinsAfterFlips(1, {42}, {{1, 7}}) == 49);
    return 0;
}
