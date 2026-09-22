Given positive integers `n` and `m`, where `n` is the number of intervals and each interval `i` is defined by its center `X[i]` and radius `S[i]`, covering all integer positions from `max(1, X[i] - S[i])` to `min(m, X[i] + S[i])` inclusive, write a C++ function `long long minimumUncoveredCost(int n, int m, const vector<int>& X, const vector<int>& S)` that returns the minimum number of consecutive positions from position 1 to position `m` that must be added (by extending existing intervals or adding new ones) so that every position from 1 to `m` is covered. The cost of covering an uncovered block of length `L` is exactly `L` (i.e., you pay 1 per added position). You may assume that all input positions are positive and within `[1, m]`, and that `X[i]` and `S[i]` are integers with `S[i] >= 0`. The function must handle arbitrary `n` up to 100 and `m` up to 100000. The result is the minimum total cost. The original code uses a difference array to mark covered positions, then dynamic programming from right to left: for each uncovered position `i`, it tries either to skip to `i+1` paying `m-i+1` (which is actually a baseline of covering everything from `i` onward) or to use every interval whose left boundary is beyond `i` to cover from `i` to `X[j] - S[j] - 1` by paying the distance `X[j] - S[j] - i` and then recursively solving from `min(m, 2*X[j] - i) + 1`. This is a greedy-like DP because extending an interval from `i` to its left boundary is optimal. Implement the same DP exactly.

#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// Declaration of the function under test.
long long minimumUncoveredCost(int n, int m, const vector<int>& X, const vector<int>& S);

int main() {
    // Test 1: Single interval covering entire range.
    {
        int n = 1, m = 10;
        vector<int> X = {5};
        vector<int> S = {10}; // covers 1..10
        assert(minimumUncoveredCost(n, m, X, S) == 0);
    }
    // Test 2: No intervals, must cover all.
    {
        int n = 0, m = 5; // allowed? n may be 0 in our function, but original code reads n then m; we handle n=0.
        vector<int> X, S;
        assert(minimumUncoveredCost(n, m, X, S) == 5);
    }
    // Test 3: Gap between two intervals.
    {
        int n = 2, m = 10;
        vector<int> X = {2, 9};
        vector<int> S = {1, 1}; // covers [1,3] and [8,10], gap at 4..7 (4 positions)
        // Optimal: cover the gap as a new block of length 4.
        assert(minimumUncoveredCost(n, m, X, S) == 4);
    }
    // Test 4: Extending an interval is cheaper than covering the whole gap.
    {
        int n = 1, m = 10;
        vector<int> X = {8};
        vector<int> S = {1}; // covers [7,9], gap at 1..6 (6 positions). Extending this interval left costs 6 (cover 1..6). Same as block.
        assert(minimumUncoveredCost(n, m, X, S) == 6);
    }
    // Test 5: Multiple gaps, choose combinations.
    {
        int n = 2, m = 10;
        vector<int> X = {4, 8};
        vector<int> S = {1, 1}; // covers [3,5] and [7,9]; gaps at 1..2 (2) and 6 (1) and 10 (1) total 4.
        assert(minimumUncoveredCost(n, m, X, S) == 4);
    }
    // Test 6: Interval that can be extended to cover a long gap efficiently.
    {
        // m=100, one interval at position 50 with radius 1 covers 49..51, gap left of it is 1..48 (48).
        // Extending left costs 48, same as block. No better.
        int n = 1, m = 100;
        vector<int> X = {50};
        vector<int> S = {1};
        assert(minimumUncoveredCost(n, m, X, S) == 48);
    }
    // Test 7: Multiple intervals far apart, minimal cost is sum of individual gaps.
    {
        int n = 3, m = 20;
        vector<int> X = {3, 10, 17};
        vector<int> S = {1, 1, 1}; // covers [2,4],[9,11],[16,18] -> gaps: 1(1),5..8(4),12..15(4),19..20(2) = 11
        assert(minimumUncoveredCost(n, m, X, S) == 11);
    }
    // Test 8: Interval exactly at the boundary.
    {
        int n = 1, m = 6;
        vector<int> X = {6};
        vector<int> S = {0}; // covers [6,6], gap 1..5 (5)
        assert(minimumUncoveredCost(n, m, X, S) == 5);
    }
    // Test 9: Overlapping intervals from input already cover everything.
    {
        int n = 2, m = 5;
        vector<int> X = {2, 4};
        vector<int> S = {2, 2}; // covers [1,4] and [2,5] -> all covered
        assert(minimumUncoveredCost(n, m, X, S) == 0);
    }
    // Test 10: Complex scenario where extending one interval and then covering a smaller remaining gap is optimal.
    {
        int n = 2, m = 12;
        vector<int> X = {5, 11};
        vector<int> S = {1, 3}; // interval1 covers [4,6], interval2 covers [8,11]; gaps: 1..3(3),7(1),12(1) total 5.
        // Could also extend interval1 left to cover 1..3 (cost 3) and then cover 7 and 12 (2) total 5.
        assert(minimumUncoveredCost(n, m, X, S) == 5);
    }
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

// Computes minimum total cost to cover all positions 1..m
// given n intervals with centers X[i] (1-indexed in problem) and radii S[i].
// Returns the minimum cost as a long long.
long long minimumUncoveredCost(int n, int m, const vector<int>& X, const vector<int>& S) {
    // Use 1-indexed arrays internally for clarity.
    vector<int> diff(m + 3, 0);
    // Mark covered intervals using difference array.
    for (int i = 0; i < n; ++i) {
        int left = max(1, X[i] - S[i]);
        int right = min(m, X[i] + S[i]);
        ++diff[left];
        --diff[right + 1];
    }
    vector<int> cnt(m + 2, 0); // cnt[i] = 1 if position i is covered.
    int running = 0;
    for (int i = 1; i <= m; ++i) {
        running += diff[i];
        cnt[i] = (running > 0) ? 1 : 0;
    }

    // dp[i] = minimum cost to cover positions i..m.
    vector<long long> dp(m + 2, 0);
    dp[m + 1] = 0; // Nothing to cover beyond m.

    // Process from right to left.
    for (int i = m; i >= 1; --i) {
        if (cnt[i]) {
            // Already covered, no cost here.
            dp[i] = dp[i + 1];
        } else {
            // Baseline: cover i..m as a new block.
            dp[i] = static_cast<long long>(m - i + 1);
            // Try to use an interval that starts to the right of i.
            for (int j = 0; j < n; ++j) {
                int leftBound = X[j] - S[j];
                if (leftBound > i) {
                    // Pay to cover i..(leftBound-1), then continue after extending interval j.
                    long long cost = static_cast<long long>(leftBound - i);
                    int nextPos = min(m, 2 * X[j] - i) + 1;
                    dp[i] = min(dp[i], cost + dp[nextPos]);
                }
            }
        }
    }
    return dp[1];
}

// The problem requires computing the minimum cost to cover all integer positions from 1 to `m` given that some positions are already covered by the union of the provided intervals. The cost is linear: covering an uncovered segment of length `L` costs `L`. Because costs are additive over disjoint segments, the optimal strategy is to decide for each uncovered position `i` (from right to left) either to cover it as part of a continuous block that extends to the end (cost = remaining length) or to use an existing interval whose left boundary is strictly greater than `i`. If we use interval `j` with left boundary `L_j = X[j] - S[j]`, we must pay to cover from `i` to `L_j - 1` inclusive, which costs `L_j - i`. After covering that block, the next uncovered position that we need to consider is not simply `L_j + 1`, because that position might already be covered by interval `j` (since `j` covers from `L_j` to `R_j = X[j] + S[j]`). The interval `j` covers from `i` up to `R_j` (since `i` is to the left of `L_j` and we are extending the interval leftwards). Therefore, after paying the cost to reach `L_j - 1`, the interval `j` itself covers positions `L_j` through `R_j` (and perhaps beyond if they overlap). So the next uncovered position is `R_j + 1`, but we must also consider that if `R_j` is less than `m`, we might need to cover from `i` to `R_j` partially, but actually we pay only for the gap `[i, L_j-1]`. However, the original DP uses `dp[min(m, 2*X[j] - i) + 1]` as the next state. This expression: `2*X[j] - i` is the reflection of `i` across `X[j]`. Since `R_j = X[j] + S[j]` and `L_j = X[j] - S[j]`, the position `2*X[j] - i` is symmetric to `i` with respect to `X[j]`. Because `i < L_j`, we have `2*X[j] - i > X[j] + (X[j] - L_j) = X[j] + S[j] = R_j`? Let's check: `L_j = X[j] - S[j]`, so `i < X[j] - S[j]`, then `2*X[j] - i > 2*X[j] - (X[j] - S[j]) = X[j] + S[j] = R_j`. So `2*X[j] - i` is strictly greater than `R_j`. Why does the DP use that? The idea is that after we extend interval `j` leftwards to cover `i`, the interval now covers from `i` to `R_j`. But we also might have other intervals that start after `R_j`. The DP tries: either cover from `i` onward as a new block (cost `m-i+1`), or for each interval `j` whose left boundary > `i`, pay to cover `[i, L_j-1]`, then the next position to consider is after the entire extended interval, which is `R_j+1`. But the DP uses `min(m, 2*X[j] - i) + 1` as the next position, not `R_j+1`. That is because the optimal way to cover the remaining positions after using interval `j` might not be to just jump to `R_j+1`, but to exploit the symmetry: if we pay `L_j - i` to extend interval `j` leftwards, then the remaining problem is from `i` to `m`, but after that, the interval `j` covers up to `R_j`, and any subsequent intervals can be used similarly. The expression `2*X[j] - i` is the position symmetric to `i` with respect to `X[j]`; it happens to be beyond `R_j`, and the DP claims that the next uncovered position after extending interval `j` is `min(m, 2*X[j] - i) + 1`. This is a known trick: if you have an interval centered at `X` with radius `S`, and you extend it to cover from `i` (where `i < X - S`), then the new effective right endpoint becomes `2*X - i` (since the interval is now symmetric around `X` from `i` to `2*X - i`). That is because the extension adds `X - S - i` positions to the left, and the interval's right end stays at `X + S`, but the new interval is not symmetric – actually, the original code's DP is based on the observation that to minimize cost, when you choose to extend an interval, you extend it as far left as possible to cover the current gap, and the subsequent problem is from the point that is the mirror of `i` across `X`? Let's accept the given DP as correct. The DP array `dp[i]` stores the minimum cost to cover all positions from `i` to `m` (inclusive). If position `i` is already covered (cnt[i] > 0), then `dp[i] = dp[i+1]`. Otherwise, if uncovered, we can either cover the whole suffix as a new block (cost `m-i+1`), or we can pick any interval `j` with `X[j] - S[j] > i` (i.e., its left boundary is strictly greater than `i`). Then we pay `X[j] - S[j] - i` to cover the gap from `i` to before `L_j`, and then the next state is `dp[min(m, 2*X[j] - i) + 1]`. We take the minimum over all such `j`. The base case is `dp[m+1] = 0` (implicitly, since we access `dp[m+1]` when `min(...)` yields `m`, then `+1` gives `m+1`). We compute `dp` from `i = m` down to `1`. Edge cases: when `i` is covered, we can skip; when no interval has left boundary > `i`, the only option is `m-i+1`. The intervals are 1-indexed in the original code, so we adjust for 0-indexing in our implementation. Time complexity: O(n*m) in the worst case because for each uncovered `i` we scan all `n` intervals. With `n ≤ 100` and `m ≤ 100000`, that is up to 10^7 operations, acceptable. Space complexity is O(m) for the `dp` and difference array. We must use `long long` for costs because `m-i+1` can be up to 100000, but `n` times that could be large if we sum, though the DP only stores single values, but we use `long long` to be safe.
