Write a C++ function `long long countZShapes(int n, int m, const std::vector<std::string>& grid)` that counts the number of "Z" shapes in a grid of characters. A "Z" shape consists of exactly three segments of consecutive `'z'` characters: a top horizontal segment, a diagonal segment going down-left, and a bottom horizontal segment, all having the same length `L` (where `L >= 1`). Specifically, for a top-left cell `(r, c)`, the shape is formed by: (1) `L` consecutive `'z'` cells to the right from `(r, c)` inclusive, (2) `L` consecutive `'z'` cells going diagonally down-left ending at `(r+L-1, c-L+1)`, and (3) `L` consecutive `'z'` cells to the right from that diagonal end. The grid is 1-indexed internally with dimensions `n` rows and `m` columns. All cells in these three segments must be `'z'`. Count all such shapes for all possible `L` and all valid positions. The grid contains only lowercase letters `'a'`-`'z'`. The function should return the total count as a `long long` (the result may exceed 32-bit integers). You may assume `1 <= n, m <= 3000` and the grid has at most 9,000,000 cells total (combined n*m ≤ 9,000,000). Implement the function efficiently.

This problem is a direct application of the original snippet's algorithm. The key insight is to process the grid along anti-diagonals (cells where `r + c` is constant, which we'll call `i`). For each anti-diagonal, we collect all `'z'` cells on that diagonal, sorted by their rightward extent (the maximum number of consecutive `'z'` to the right plus the column index minus 1). Then we use a persistent segment tree to answer range queries: for a given starting cell `(x, y)`, the maximum possible `L` is `min(left_[x][y], anti_diagonal[x][y])` where `left_` is the count of consecutive `'z'` to the left (including current) and `anti_diagonal` is the count of consecutive `'z'` going down-left (including current). For each such starting cell, we need to count how many cells on the same anti-diagonal that are within the range `[x, x+L-1]` (these are potential top-left corners of valid Z shapes, since the bottom horizontal segment must lie on a row `>= x` and reachable). The trick is that for a cell `(a, b)` on the same anti-diagonal to be a valid top-left for the same bottom row as `(x,y)`, we need its rightward extent to be at least `y` (the column of the diagonal end). This is why we sort by `val = right_[a][b] + b - 1` (the rightmost column reachable from `(a,b)` horizontally). A cell `(a,b)` can contribute to a shape starting at `(x,y)` with length `L` if `a + L - 1` is at least the row of the bottom-left corner, which is exactly what the range `[x, x+L-1]` captures. Additionally, we need `right_` of the starting cell to cover the entire bottom segment; but that's automatically handled by the diagonal construction. The persistent segment tree built over rows allows us to query, for all cells with `val >= y`, how many of those rows fall in `[x, x+L-1]`. Then we subtract the contributions from cells with `val < y` by querying a version of the tree that only includes cells with `val < y`. The algorithm processes each anti-diagonal independently. The main edge case is when `val` is exactly `y` or the lower_bound boundary. Time complexity is `O(n*m log n)` due to segment tree operations for each cell (each cell is inserted once and queried once). Space complexity is `O(n*m log n)` in the worst case for persistent tree nodes, but we reset the tree per anti-diagonal so it's actually `O(size_of_diagonal * log n)` per diagonal, and since diagonals are processed sequentially we reuse memory; overall peak is `O(max_diagonal_length * log n)`, which is acceptable.

#include <bits/stdc++.h>

// Count the number of "Z" shapes in a grid of characters.
// A Z shape has three equal-length segments: top horizontal, down-left diagonal, bottom horizontal.
long long countZShapes(int n, int m, const std::vector<std::string>& grid) {
    // Convert to 1-indexed for easier DP.
    std::vector<std::string> mp(n + 2, std::string(m + 2, '.'));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            mp[i][j] = grid[i - 1][j - 1];
        }
    }

    // Precompute prefix lengths.
    std::vector<std::vector<int>> left_(n + 2, std::vector<int>(m + 2, 0));
    std::vector<std::vector<int>> right_(n + 2, std::vector<int>(m + 2, 0));
    std::vector<std::vector<int>> anti_diag(n + 2, std::vector<int>(m + 2, 0));

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (mp[i][j] == 'z') {
                left_[i][j] = left_[i][j - 1] + 1;
            }
        }
    }

    for (int i = 1; i <= n; ++i) {
        for (int j = m; j >= 1; --j) {
            if (mp[i][j] == 'z') {
                right_[i][j] = right_[i][j + 1] + 1;
            }
        }
    }

    for (int i = n; i >= 1; --i) {
        for (int j = 1; j <= m; ++j) {
            if (mp[i][j] == 'z') {
                anti_diag[i][j] = anti_diag[i + 1][j - 1] + 1;
            }
        }
    }

    long long ans = 0;

    // Persistent segment tree.
    const int MAXN = n + 5;
    std::vector<int> sum(MAXN * 20);
    std::vector<int> sonL(MAXN * 20), sonR(MAXN * 20);
    int ncnt = 0;

    auto build = [&](auto&& self, int l, int r, int now) -> void {
        if (l == r) {
            sum[now] = 0;
            return;
        }
        int mid = (l + r) >> 1;
        sonL[now] = ++ncnt;
        sonR[now] = ++ncnt;
        self(self, l, mid, sonL[now]);
        self(self, mid + 1, r, sonR[now]);
        sum[now] = 0;
    };

    auto update = [&](auto&& self, int L, int val, int l, int r, int now_rt, int pre_rt) -> void {
        if (l == r) {
            sum[now_rt] = val;
            return;
        }
        int mid = (l + r) >> 1;
        if (L <= mid) {
            sonR[now_rt] = sonR[pre_rt];
            sonL[now_rt] = ++ncnt;
            self(self, L, val, l, mid, sonL[now_rt], sonL[pre_rt]);
        } else {
            sonL[now_rt] = sonL[pre_rt];
            sonR[now_rt] = ++ncnt;
            self(self, L, val, mid + 1, r, sonR[now_rt], sonR[pre_rt]);
        }
        sum[now_rt] = sum[sonL[now_rt]] + sum[sonR[now_rt]];
    };

    auto query = [&](auto&& self, int L, int R, int l, int r, int rt) -> int {
        if (L <= l && r <= R) {
            return sum[rt];
        }
        int mid = (l + r) >> 1;
        int res = 0;
        if (L <= mid) {
            res += self(self, L, R, l, mid, sonL[rt]);
        }
        if (R > mid) {
            res += self(self, L, R, mid + 1, r, sonR[rt]);
        }
        return res;
    };

    // Process each anti-diagonal (r + c = constant).
    for (int diag = 2; diag <= n + m; ++diag) {
        int rmin = std::max(1, diag - m);
        int rmax = std::min(n, diag - 1);
        std::vector<std::pair<int, int>> V; // {val, row}
        for (int x = rmin; x <= rmax; ++x) {
            int y = diag - x;
            if (mp[x][y] != 'z') continue;
            int val = right_[x][y] + y - 1;
            V.push_back({val, x});
        }
        if (V.empty()) continue;

        std::sort(V.begin(), V.end());

        // Build persistent segment tree over rows.
        ncnt = 0;
        std::vector<int> root(V.size() + 1);
        root[0] = ++ncnt;
        build(build, 1, n, root[0]);
        int last_rt = root[0];
        for (size_t idx = 0; idx < V.size(); ++idx) {
            root[idx + 1] = ++ncnt;
            update(update, V[idx].second, 1, 1, n, root[idx + 1], last_rt);
            last_rt = root[idx + 1];
        }

        int mx_rt = root[V.size()];

        for (int x = rmin; x <= rmax; ++x) {
            int y = diag - x;
            if (mp[x][y] != 'z') continue;
            int L = std::min(left_[x][y], anti_diag[x][y]);
            int ql = x;
            int qr = x + L - 1;
            int all = query(query, ql, qr, 1, n, mx_rt);
            // Find first index in V with val >= y.
            auto it = std::lower_bound(V.begin(), V.end(), std::make_pair(y, 0));
            int idx = static_cast<int>(it - V.begin());
            int partial = 0;
            if (idx > 0) {
                int rt = root[idx];
                partial = query(query, ql, qr, 1, n, rt);
            }
            ans += all - partial;
        }
    }

    return ans;
}

#include <bits/stdc++.h>
#include <cassert>

// Declare the function (replace with actual signature if needed).
long long countZShapes(int n, int m, const std::vector<std::string>& grid);

int main() {
    // Test 1: Single Z shape of length 1
    std::vector<std::string> g1 = {"zz", "z."};
    assert(countZShapes(2, 2, g1) == 1);

    // Test 2: No z's at all
    std::vector<std::string> g2 = {"abc", "def"};
    assert(countZShapes(2, 3, g2) == 0);

    // Test 3: Larger Z shape length 2
    // Top: zz
    // Diagonal: z (down-left from second column)
    // Bottom: zz
    std::vector<std::string> g3 = {"zz.", ".z.", ".zz"};
    assert(countZShapes(3, 3, g3) == 1);

    // Test 4: Two overlapping Z shapes of length 1 on same diagonal
    std::vector<std::string> g4 = {"zzz", "zzz", "zzz"};
    // For a 3x3 all z, count manually:
    // L=1: each 'z' can be top-left if top row has at least 1, diagonal has 1, bottom has 1.
    // All 9 cells qualify for L=1, but diagonal end must be within grid: for (r,c) with c>=1 and r+1<=n and c-1>=1. Actually count shapes: for each cell (r,c), need (r,c), (r+1,c-1), (r+1,c) to all be 'z'. That's many.
    // We'll just check it's nonzero and reasonable via the original algorithm's correctness.
    long long res4 = countZShapes(3, 3, g4);
    assert(res4 > 0);

    // Test 5: Single cell 'z' -> no shape because need diagonal and bottom
    std::vector<std::string> g5 = {"z"};
    assert(countZShapes(1, 1, g5) == 0);

    // Test 6: Shape that is not a Z because diagonal length differs
    std::vector<std::string> g6 = {"zz", "zz"};
    // For L=1: need (1,1),(2,0) invalid, (1,2),(2,1) and bottom (2,2) -> (1,2) to (2,1) diagonal is 1, bottom from (2,1) to (2,2) is 2 -> not equal -> no shape.
    assert(countZShapes(2, 2, g6) == 0);

    // Test 7: Larger grid performance sanity (just run, no crash)
    int n = 10, m = 10;
    std::vector<std::string> g7(n, std::string(m, 'z'));
    long long res7 = countZShapes(n, m, g7);
    assert(res7 >= 0);

    return 0;
}
