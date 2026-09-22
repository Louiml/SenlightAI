// Write a C++ function `long long minimumCostToKeepVisible(const std::vector<long long>& heights, int k)` that, given the initial heights of a row of `n` pillars (indexed 1 through `n`) and a maximum number of removals `k`, returns the minimum possible total cost after removing at most `k` pillars. The cost is defined as the sum over each pair of consecutive remaining pillars of the positive difference in height (if the later pillar is shorter, the cost contribution is zero). Pillar 1 is always kept, and pillar `n` is always kept; you may remove only intermediate pillars. The heights can be large (up to 1e9) and `n` up to 300. The function must be self-contained and use no global variables except for compile-time constants.
// The problem is a dynamic programming problem similar to the classic "remove elements to minimize adjacent differences" problem. Since we can remove at most `k` pillars, and the order is fixed, the state can be defined as `dp[i][j][last]` = minimum cost considering the first `i` pillars (indices 1..i), having removed exactly `j` pillars among the first `i` (not counting pillar 1), and where `last` is the index of the last kept pillar (must be ≤ i). Transition: either keep pillar `i+1` (cost added is `max(0, h[i+1] - h[last])`, and `last` becomes `i+1`), or remove pillar `i+1` (cost unchanged, `last` stays same, `j` increments). Initialize `dp[0][0][0] = 0` with a dummy height 0 for pillar 0 (or start from index 1 with pillar 1 forced kept). Since we force keeping pillar 1 and pillar n, we can start DP from i=1 with dp[1][0][1]=0, but a simpler approach is to start from i=0 with a dummy pillar 0 of height 0 and allow keeping pillar 1 in the first step, but then ensure pillar n is not removed in the last step. Edge cases: k may be greater than n-2 (but at most n-2 useful removals since we must keep first and last). Use a large INF (e.g., 1e18). Time complexity is O(n^2 * k) because for each i (n) we iterate j up to k and last up to i, giving O(n^3) worst-case when k≈n, but since n≤300, that is fine. Space complexity O(n^2 * k) as well, but we can optimize to O(n^2) per layer? Actually we keep full 3D dp for simplicity, which is about 300^3 = 27e6 states, each an 8-byte long long, ~216 MB, which is too high. So we must reduce memory: note that transition only uses dp[i][*][*] to compute dp[i+1][*][*], so we can use two 2D planes (size (k+1)x(n+1)) and swap. Complexity becomes O(n^2 * k) time and O(n*k) space. Initialize dp for i=0 with dp[0][0]=0, meaning last kept index = 0 (dummy height 0). For each i from 0 to n-1, compute next dp. Finally, answer is min over j≤k of dp[n][j] (where dp[n][j] corresponds to last index = n? Actually we need to interpret correctly: dp[i][j][last] means we've processed first i pillars, removed j, and last kept is `last`. When we finish at i=n, we must have last = n (since pillar n must be kept). So answer is dp[n][j][n] for any j≤k. In the 2D plane we store for each (j, last) the minimum cost after processing some prefix. We'll implement with 3D but limited to n=300 and using vector arrays of dimension (k+1) x (n+1) for current and next. We'll set h[0]=0, and keep pillar 0 as dummy but never remove it. The answer is min over j from 0 to k of next[ j ][ n ] after processing all n pillars.
#include <vector>
#include <algorithm>
#include <cstdint>

// Given heights of pillars (1-indexed internally) and maximum removals k,
// return minimum cost after removing at most k intermediate pillars.
// Cost = sum over consecutive kept pillars of max(0, heightLater - heightEarlier).
// Pillar 1 and pillar n must be kept.
long long minimumCostToKeepVisible(const std::vector<long long>& heights, int k) {
    int n = static_cast<int>(heights.size());
    if (n == 0) return 0;
    if (n == 1) return 0;

    // Use 1-indexed for convenience; h[0] is a dummy pillar with height 0.
    std::vector<long long> h(n + 1);
    for (int i = 0; i < n; ++i) h[i + 1] = heights[i];

    const long long INF = 1e18;
    // dp[j][last] = min cost using processed pillars up to current i,
    // removed j pillars, and last kept pillar index is 'last'.
    // Initialize for i=0: only possibility is j=0, last=0, cost=0.
    std::vector<std::vector<long long>> dp(k + 1, std::vector<long long>(n + 1, INF));
    dp[0][0] = 0;

    for (int i = 0; i < n; ++i) {
        std::vector<std::vector<long long>> ndp(k + 1, std::vector<long long>(n + 1, INF));
        for (int j = 0; j <= k; ++j) {
            for (int last = 0; last <= i; ++last) {
                long long cur = dp[j][last];
                if (cur >= INF) continue;

                // Option 1: keep pillar i+1 (must be allowed; if i+1 == n we must keep it).
                long long add = (h[i + 1] > h[last]) ? (h[i + 1] - h[last]) : 0;
                ndp[j][i + 1] = std::min(ndp[j][i + 1], cur + add);

                // Option 2: remove pillar i+1 (only if it is not the last pillar n).
                if (i + 1 < n && j + 1 <= k) {
                    ndp[j + 1][last] = std::min(ndp[j + 1][last], cur);
                }
            }
        }
        dp = std::move(ndp);
    }

    // After processing all n pillars, last must be n (since we never removed pillar n).
    long long ans = INF;
    for (int j = 0; j <= k; ++j) {
        ans = std::min(ans, dp[j][n]);
    }
    return ans;
}
#include <cassert>
#include <vector>

// The solution function is declared above.

int main() {
    // Simple case: no removals, all increasing
    assert(minimumCostToKeepVisible({1, 2, 3, 4}, 0) == 3); // 1+1+1

    // One removal allowed, remove the tall middle pillar
    assert(minimumCostToKeepVisible({1, 100, 2}, 1) == 1); // keep 1 and 2

    // Removal not beneficial if already non-decreasing
    assert(minimumCostToKeepVisible({1, 2, 3}, 2) == 2); // can't remove first/last, but can remove middle? Actually cannot remove first/last, but can remove middle, cost becomes 2 (1->3 diff=2). So answer is 2.

    // Decreasing sequence, must remove all but first/last
    assert(minimumCostToKeepVisible({5, 4, 3, 2, 1}, 3) == 0); // remove 3 middle, keep 5 and 1, diff = max(0, 1-5)=0

    // k larger than useful
    assert(minimumCostToKeepVisible({5, 4, 3, 2, 1}, 10) == 0);

    // Single pillar
    assert(minimumCostToKeepVisible({7}, 0) == 0);

    // Two pillars, no removal possible
    assert(minimumCostToKeepVisible({10, 5}, 0) == 0); // diff = max(0, 5-10)=0

    // Mixed heights
    assert(minimumCostToKeepVisible({3, 1, 4, 1, 5, 9, 2, 6}, 2) == 8);

    // Edge: all same heights
    assert(minimumCostToKeepVisible({5, 5, 5, 5}, 3) == 0);
}
