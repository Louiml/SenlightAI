// Write a C++ function that takes a grid of `0`/`1` values as a `vector<vector<int>>` representing an `h x w` binary matrix. You may flip every cell in an entire row (toggle `0`↔`1`). The goal is to make the matrix have no isolated cells: no cell (within rows 1..h-1, 0-indexed) may have a value strictly different from all four orthogonal neighbors (up, down, left, right), unless the neighbor is outside the matrix. Cells on the top and bottom edges only need to satisfy the condition against existing neighbors. A cell is "isolated" if it is different from every existing neighbor in the four directions. Return the minimum number of row flips needed; if impossible, return `-1`. The function signature: `int minRowFlips(const std::vector<std::vector<int>>& a);`

This is a dynamic programming problem over rows. Since flips only affect whole rows, the state of a cell in row `i` depends only on rows `i-1`, `i`, and `i+1`. For each row, we decide whether to flip it (0 or 1). For rows `1` to `h-2`, a cell in row `i` must be "non‑isolated": either it equals at least one vertical neighbor, or it equals a horizontal neighbor in the same row. For the top row (`i=0`), it only has neighbor below and horizontal; for the bottom row (`i=h-1`), it only has neighbor above and horizontal.

The DP state: `dp[i][j][k]` = minimum flips used for rows `0..i` such that all cells in rows `0..i-1` are already non‑isolated, `j` indicates whether row `i-1` was flipped, `k` indicates whether row `i` was flipped. For `i=0` we initialize with `dp[0][0][0]=0` (row `-1` doesn't exist, treat as all‑zero dummy row) and `dp[0][0][1]=1`. Transition from `(i-1, j, k)` to `(i, k, l)` requires that row `i-1` cells are non‑isolated considering rows `i-2`, `i-1`, `i`. After processing all rows, we also verify row `h-1` using rows `h-2` and `h`. The answer is the minimum over `dp[h-1][j][k]`. Because each row has 2 flip options, the state space is `h * 2 * 2`, and each transition checks `O(w)` cells. Time complexity `O(h * w * 8)` ≈ `O(h*w)`. Space can be optimized to two rows, but we keep full for clarity. Edge cases: `h=1` (only one row), `h=2` (no middle rows), and impossible cases where no configuration satisfies the condition.

#include <vector>
#include <algorithm>
#include <climits>

// Returns minimum row flips to eliminate isolated cells, or -1 if impossible.
int minRowFlips(const std::vector<std::vector<int>>& a) {
    int h = (int)a.size();
    if (h == 0) return 0;
    int w = (int)a[0].size();
    const int INF = 1e9;
    // dp[i][j][k]: min flips for rows 0..i, where j = flip status of row i-1, k = flip status of row i
    // We'll use a 3D vector; for i=0 we pretend row -1 is all zeros and not flipped.
    std::vector<std::vector<std::vector<int>>> dp(h, std::vector<std::vector<int>>(2, std::vector<int>(2, INF)));

    // For i=0, only two states: row 0 not flipped (k=0) or flipped (k=1). j is always 0 (dummy row -1 not flipped).
    dp[0][0][0] = 0;
    dp[0][0][1] = 1;

    for (int i = 1; i < h; ++i) {
        for (int j = 0; j < 2; ++j) {       // flip status of row i-2 (for i==1 this is dummy row -1)
            for (int k = 0; k < 2; ++k) {   // flip status of row i-1
                if (dp[i-1][j][k] >= INF) continue;
                for (int l = 0; l < 2; ++l) { // flip status of row i
                    // Build effective values for rows i-2, i-1, i considering flips
                    std::vector<int> up(w), mid(w), down(w);
                    // row i-2: if i==1, use dummy all zeros; else use a[i-2]
                    for (int m = 0; m < w; ++m) {
                        int val = (i == 1) ? 0 : a[i-2][m];
                        if (j == 1) val = 1 - val;
                        up[m] = val;
                    }
                    // row i-1
                    for (int m = 0; m < w; ++m) {
                        int val = a[i-1][m];
                        if (k == 1) val = 1 - val;
                        mid[m] = val;
                    }
                    // row i
                    for (int m = 0; m < w; ++m) {
                        int val = a[i][m];
                        if (l == 1) val = 1 - val;
                        down[m] = val;
                    }

                    bool ok = true;
                    // Check cells in row i-1 (must not be isolated)
                    for (int m = 0; m < w; ++m) {
                        bool hasSame = false;
                        // vertical neighbor up (if i-2 exists, i.e., i>=2? Actually up always exists as dummy for i==1)
                        if (up[m] == mid[m]) hasSame = true;
                        // vertical neighbor down
                        if (down[m] == mid[m]) hasSame = true;
                        // horizontal left
                        if (m > 0 && mid[m-1] == mid[m]) hasSame = true;
                        // horizontal right
                        if (m+1 < w && mid[m+1] == mid[m]) hasSame = true;
                        if (!hasSame) { ok = false; break; }
                    }

                    // If this is the last row, also check row i (the bottom row)
                    if (ok && i == h-1) {
                        for (int m = 0; m < w; ++m) {
                            bool hasSame = false;
                            // vertical neighbor up (row i-1)
                            if (mid[m] == down[m]) hasSame = true;
                            // horizontal left
                            if (m > 0 && down[m-1] == down[m]) hasSame = true;
                            // horizontal right
                            if (m+1 < w && down[m+1] == down[m]) hasSame = true;
                            if (!hasSame) { ok = false; break; }
                        }
                    }

                    if (ok) {
                        dp[i][k][l] = std::min(dp[i][k][l], dp[i-1][j][k] + l);
                    }
                }
            }
        }
    }

    int ans = INF;
    for (int j = 0; j < 2; ++j) {
        for (int k = 0; k < 2; ++k) {
            ans = std::min(ans, dp[h-1][j][k]);
        }
    }
    return (ans == INF) ? -1 : ans;
}

#include <cassert>
#include <vector>

// The solution function is declared above; include it here (or link).

int main() {
    // Single row, w=1: [0] has no neighbors, but since w=1 and no vertical neighbor, it's not isolated? Actually it has no neighbors at all, so it's not isolated by definition (no different neighbor exists). So 0 flips needed.
    assert(minRowFlips({{0}}) == 0);
    assert(minRowFlips({{1}}) == 0);

    // 2x1: both cells must not be isolated. If they differ, each is isolated because no horizontal neighbor. So we need same values → flip one row.
    assert(minRowFlips({{0}, {1}}) == 1);
    assert(minRowFlips({{1}, {0}}) == 1);
    assert(minRowFlips({{1}, {1}}) == 0);
    assert(minRowFlips({{0}, {0}}) == 0);

    // 2x2 all zeros, already ok → 0 flips
    assert(minRowFlips({{0,0},{0,0}}) == 0);
    // 2x2 checkerboard: each cell different from all neighbors → need flips. Try flipping one row yields [[1,1],[0,0]] → top row cells now have same horizontal neighbor, bottom row cells have same horizontal neighbor, and vertical neighbors differ but that's fine because horizontal same exists → okay. So 1 flip.
    assert(minRowFlips({{0,1},{1,0}}) == 1);
    assert(minRowFlips({{1,0},{0,1}}) == 1);

    // 3x3 all zeros → 0
    assert(minRowFlips({{0,0,0},{0,0,0},{0,0,0}}) == 0);
    // 3x3 with a single isolated cell in middle: [[1,1,1],[1,0,1],[1,1,1]] → need to flip the middle row → [[1,1,1],[0,1,0],[1,1,1]] now each cell has a same horizontal neighbor? middle row: 0,1,0 → middle cell has left 0? actually left is 0, right is 0, so has same? 1 vs 0? No, 1 is different from 0, but has vertical neighbors 1 (up and down), so has same vertical → ok. So 1 flip.
    assert(minRowFlips({{1,1,1},{1,0,1},{1,1,1}}) == 1);

    // Impossible case: 2x2 [[0,1],[1,1]] Try all flips: no flip gives cell(0,0)=0 has neighbors right=1, down=1 → isolated? 0 different from both neighbors, no horizontal same → isolated. Flip top row: [[1,0],[1,1]] cell(0,1)=0 has left=1, down=1 → isolated. Flip bottom: [[0,1],[0,0]] cell(1,0)=1 has up=0, right=0 → isolated. Flip both: [[1,0],[0,0]] cell(1,1)=0 has up=0? up is a[0][1]=0, left=0, right none → has same left/up so not isolated? Actually cell(1,1)=0 has left=0, up=0, so has same → not isolated. cell(0,0)=1 has right=0, down=0 → different from both neighbors, no horizontal same? right is 0, left none, up none → isolated. So impossible → -1.
    assert(minRowFlips({{0,1},{1,1}}) == -1);
    assert(minRowFlips({{1,0},{0,0}}) == -1);

    // 1x3: each cell must have at least one same horizontal neighbor. [0,1,0] middle is 1, left 0, right 0 → isolated, so need flips. Flip middle row → [0,0,0] works in 1 flip. So answer 1.
    assert(minRowFlips({{0,1,0}}) == 1);
    assert(minRowFlips({{1,0,1}}) == 1);
    assert(minRowFlips({{0,0,0}}) == 0);
    assert(minRowFlips({{1,1,1}}) == 0);
}
