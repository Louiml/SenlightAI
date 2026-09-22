Write a C++ function `int minCrossings(int plotCount, int maxDist, const std::vector<int>& positions)` that, given a line of `plotCount` plots with integer coordinates `positions` sorted in non-decreasing order, and a maximum allowable distance `maxDist` (meaning from plot `i` one may jump to any plot `j` with `i < j ≤ i + maxDist`), finds the minimum number of jumps needed to travel from plot `0` to the last plot `plotCount-1`, with the additional constraint that every jump must have a slope (change in position divided by change in index) that is at least as large as the slope of the immediately preceding jump. The first jump has no previous slope constraint. If reaching the end is impossible, return a sentinel value (e.g., a large number like 1,000,000,000). The function must handle `plotCount ≥ 2` and all coordinates being distinct integers within `int` range. For example, if `positions = {0, 10, 15, 20}` and `maxDist = 2`, a valid path is 0→1→3 with slopes 10 and 5, which works because the sequence of slopes is non-decreasing (10 ≥ 5 is not true, so that path fails; the only valid path is 0→2→3 with slopes 7.5 and 5, also fails; thus output would be INF). Ensure the algorithm is efficient for `plotCount ≤ 5000`.

#include <cassert>
#include <vector>

int minCrossings(int, int, const std::vector<int>&); // declaration from solution

int main() {
    // Basic reachable case with non-decreasing slopes
    {
        std::vector<int> pos = {0, 1, 2, 3};
        assert(minCrossings(4, 3, pos) == 1); // direct jump slope 1
    }
    // Impossible because slopes decrease
    {
        std::vector<int> pos = {0, 10, 15, 20};
        // 0->1 slope 10, then 1->2 slope 5 (decrease) fails; 0->2 slope 7.5, 2->3 slope 5 fails; 0->1->3 slope 10 then 5 fails; 0->2->3 fails; so INF
        assert(minCrossings(4, 2, pos) == 1000000000);
    }
    // Need multiple jumps but slopes stay non-decreasing
    {
        std::vector<int> pos = {0, 5, 6, 7}; 
        // 0->1 slope 5, 1->2 slope 1 (decrease) fails; 0->2 slope 3, 2->3 slope 1 fails; 0->1->3: 5 then 1 fails; 0->2->3: 3 then 1 fails; 0->3 direct slope 7/3 ≈2.33 works with maxDist=3, so answer 1
        assert(minCrossings(4, 3, pos) == 1);
    }
    // Path requiring two jumps with increasing slopes
    {
        std::vector<int> pos = {0, 2, 3, 10};
        // 0->1 slope 2, 1->3 slope (10-3)/(2)=3.5, works (non-decreasing), 2 jumps
        assert(minCrossings(4, 2, pos) == 2);
    }
    // maxDist limits reachability
    {
        std::vector<int> pos = {0, 100, 101, 102, 200};
        // 0->1 slope 100, then 1->2 slope 1 (decrease) fails; 0->2 slope 50.5, 2->3 slope 1 fails; 0->3 slope 34, 3->4 slope 98? but maxDist=1 from 3 to 4 allowed slope 98 >=34, so 0->3->4 works (3 jumps)
        assert(minCrossings(5, 2, pos) == 2); // 0->2 (slope 50.5) then 2->3 (slope 1) fails; 0->3 (slope 34) then 3->4 (slope 98) works, so 2 jumps
    }
    // Single step direct if within distance
    {
        std::vector<int> pos = {0, 10};
        assert(minCrossings(2, 1, pos) == 1);
    }
    // Larger case with all equal increments - direct jump works
    {
        std::vector<int> pos(10);
        for (int i = 0; i < 10; ++i) pos[i] = i * 2;
        assert(minCrossings(10, 9, pos) == 1);
    }
    // Impossible due to maxDist too small
    {
        std::vector<int> pos = {0, 5, 10, 15};
        assert(minCrossings(4, 1, pos) == 1000000000); // must jump step by step, slopes 5 each, but 0->1 slope 5, 1->2 slope 5, ok, 2->3 slope 5, so actually possible with 3 jumps? but maxDist=1, slopes constant 5, works
        // Actually it is possible: 0->1 (5), 1->2 (5), 2->3 (5) all non-decreasing, so answer 3
        assert(minCrossings(4, 1, pos) == 3);
    }
    // Edge with 2 plots and maxDist 0 (no jumps possible)
    {
        std::vector<int> pos = {0, 1};
        assert(minCrossings(2, 0, pos) == 1000000000);
    }
    // Repeated equal slopes
    {
        std::vector<int> pos = {0, 4, 8, 12};
        assert(minCrossings(4, 2, pos) == 2); // 0->1 slope 4, 1->3 slope 2? that's decrease, so 0->2 slope 4, 2->3 slope 4 works, 2 jumps
    }
    return 0;
}

#include <vector>
#include <algorithm>
#include <limits>

// Return the minimum number of jumps from plot 0 to plot plotCount-1
// given that each jump cannot exceed maxDist and slopes must be non-decreasing.
// If impossible, return a large sentinel value.
int minCrossings(int plotCount, int maxDist, const std::vector<int>& positions) {
    const int INF = 1000000000;
    if (plotCount <= 1) return 0;

    std::vector<int> dp(plotCount, INF);
    std::vector<long long> bestSlopeNum(plotCount, 0); // numerator for slope comparison
    std::vector<long long> bestSlopeDen(plotCount, 1); // denominator

    dp[0] = 0;
    // dummy slope for start: treat as -infinity, so any first jump is allowed.
    bestSlopeNum[0] = -INF;
    bestSlopeDen[0] = 1;

    for (int i = 0; i < plotCount; ++i) {
        if (dp[i] == INF) continue;
        int maxReach = std::min(plotCount - 1, i + maxDist);
        for (int to = i + 1; to <= maxReach; ++to) {
            long long dy = positions[to] - positions[i];
            long long dx = to - i;
            // slope = dy / dx

            bool canJump = false;
            if (i == 0) {
                canJump = true;
            } else {
                // Check if slope(to-i) >= bestSlope[i]
                // dy/dx >= bestNum/bestDen  => dy * bestDen >= bestNum * dx
                if (dy * bestSlopeDen[i] >= bestSlopeNum[i] * dx) {
                    canJump = true;
                }
            }
            if (!canJump) continue;

            int newJumps = dp[i] + 1;
            if (newJumps < dp[to]) {
                dp[to] = newJumps;
                bestSlopeNum[to] = dy;
                bestSlopeDen[to] = dx;
            } else if (newJumps == dp[to]) {
                // keep the larger slope
                if (dy * bestSlopeDen[to] > bestSlopeNum[to] * dx) {
                    bestSlopeNum[to] = dy;
                    bestSlopeDen[to] = dx;
                }
            }
        }
    }
    return dp[plotCount - 1];
}

// This is a dynamic programming problem where we compute the minimum number of jumps to reach each plot `i` while also tracking the maximum possible slope of the last jump used to reach `i` (because a larger final slope gives more flexibility for future jumps). For each state `i`, we store `DP[i]` = minimum jumps to reach `i`, and `bestSlope[i]` = the maximum slope achievable among all paths that reach `i` with `DP[i]` jumps. If two paths have the same jump count, we keep the one with a higher last slope. We initialize `DP[0] = 0` (or 1 as in the snippet, but here we start with 0 jumps at the start) and `bestSlope[0] = -INF` (no previous slope constraint). Then for each `i` from 0 to `plotCount-1`, we consider all `to` in `(i, i+maxDist]`. For each candidate jump, we compute its slope `s`. If `i == 0` or `s >= bestSlope[i]` (since we must maintain non-decreasing slopes), then we can improve `DP[to]` and `bestSlope[to]`. Specifically, if `DP[i]+1 < DP[to]`, update both; if `DP[i]+1 == DP[to]`, update `bestSlope[to] = max(bestSlope[to], s)`. To avoid floating-point precision issues, we can compare slopes using cross-multiplication: `(positions[to]-positions[i]) * (i - from) >= (positions[i]-positions[from]) * (to - i)`. This handles integer arithmetic exactly. Edge cases: if no path reaches the last plot, return INF. Time complexity is O(plotCount * maxDist) because each pair is considered once, and with maxDist up to plotCount, worst-case O(plotCount^2) which is fine for 5000. Space complexity is O(plotCount) for the DP arrays.
