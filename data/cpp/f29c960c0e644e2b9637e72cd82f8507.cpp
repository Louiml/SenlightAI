/*
Write a C++ function that, given an array `h` of `n` positive integers representing heights (indices 1-based), and a list of `m` queries, each specifying two indices `x` and `y`, returns a vector of `long long` answers. For each query, consider the subarray from `min(x,y)` to `max(x,y)`. If `x > y`, the cost multiplier for the left endpoint is `4` and the right endpoint is `1`; otherwise, the left endpoint multiplier is `1` and the right endpoint is `4`. The cost is computed as follows: first, find the maximum height `T` in the subarray. Add `(T - h[x]) * a + (T - h[y]) * b`, where `a` and `b` are the multipliers as described. Then, let `m` be the index of the maximum height in the subarray (if ties, choose the smallest index). Let `L` be the maximum value of `(h[i] + (n - i) + 1)` over the subarray, and let `R` be the maximum value of `(h[i] + i)` over the subarray. Compute `can_use_l = (m - x) - (L - (h[x] + (n - x) + 1))` and `can_use_r = (R - (h[x] + x)) - (y - m)`. Finally, add `2 * (max(0, (m - x) - can_use_l) + max(0, (y - m) - can_use_r))` to the answer. Return the vector of answers.
*/
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>

class SparseTableQueries {
public:
    // Returns answers for queries on the given heights. h is 1-indexed (size n+1).
    static std::vector<long long> solveQueries(
        const std::vector<int>& h,
        const std::vector<std::pair<int, int>>& queries) 
    {
        int n = static_cast<int>(h.size()) - 1;
        int m = static_cast<int>(queries.size());

        // Precompute derived arrays
        std::vector<int> l(n + 1), r(n + 1);
        for (int i = 1; i <= n; ++i) {
            l[i] = h[i] + (n - i) + 1;
            r[i] = h[i] + i;
        }

        // Sparse table levels
        int LOG = 0;
        while ((1 << LOG) <= n) ++LOG;
        std::vector<std::vector<int>> stH(LOG, std::vector<int>(n + 1));
        std::vector<std::vector<int>> stIdx(LOG, std::vector<int>(n + 1));
        std::vector<std::vector<int>> stL(LOG, std::vector<int>(n + 1));
        std::vector<std::vector<int>> stR(LOG, std::vector<int>(n + 1));

        // Base level
        for (int i = 1; i <= n; ++i) {
            stH[0][i] = h[i];
            stIdx[0][i] = i;
            stL[0][i] = l[i];
            stR[0][i] = r[i];
        }

        // Build sparse tables
        for (int k = 1; k < LOG; ++k) {
            int len = 1 << (k - 1);
            int range = 1 << k;
            for (int i = 1; i + range - 1 <= n; ++i) {
                // Height value
                stH[k][i] = std::max(stH[k-1][i], stH[k-1][i+len]);
                // Index of max, leftmost on ties
                int idx1 = stIdx[k-1][i];
                int idx2 = stIdx[k-1][i+len];
                stIdx[k][i] = (h[idx1] > h[idx2]) ? idx1 : idx2; // leftmost on tie
                // Derived max arrays
                stL[k][i] = std::max(stL[k-1][i], stL[k-1][i+len]);
                stR[k][i] = std::max(stR[k-1][i], stR[k-1][i+len]);
            }
        }

        auto queryMax = [&](int ql, int qr) {
            int len = qr - ql + 1;
            int k = 0;
            while ((1 << (k+1)) <= len) ++k;
            return std::max(stH[k][ql], stH[k][qr - (1 << k) + 1]);
        };

        auto queryIdx = [&](int ql, int qr) {
            int len = qr - ql + 1;
            int k = 0;
            while ((1 << (k+1)) <= len) ++k;
            int i1 = stIdx[k][ql];
            int i2 = stIdx[k][qr - (1 << k) + 1];
            return (h[i1] > h[i2]) ? i1 : i2; // leftmost on tie
        };

        auto queryMaxL = [&](int ql, int qr) {
            int len = qr - ql + 1;
            int k = 0;
            while ((1 << (k+1)) <= len) ++k;
            return std::max(stL[k][ql], stL[k][qr - (1 << k) + 1]);
        };

        auto queryMaxR = [&](int ql, int qr) {
            int len = qr - ql + 1;
            int k = 0;
            while ((1 << (k+1)) <= len) ++k;
            return std::max(stR[k][ql], stR[k][qr - (1 << k) + 1]);
        };

        std::vector<long long> answers;
        answers.reserve(m);

        for (const auto& q : queries) {
            int x = q.first;
            int y = q.second;
            long long a, b;
            if (x > y) {
                std::swap(x, y);
                a = 4;
                b = 1;
            } else {
                a = 1;
                b = 4;
            }

            int T = queryMax(x, y);
            long long ans = static_cast<long long>(T - h[x]) * a + static_cast<long long>(T - h[y]) * b;

            int mid = queryIdx(x, y);
            int Lmax = queryMaxL(x, y);
            int Rmax = queryMaxR(x, y);

            int can_use_l = (mid - x) - (Lmax - l[x]);
            int can_use_r = (Rmax - r[x]) - (y - mid);

            ans += 2LL * (std::max(0, (mid - x) - can_use_l) + std::max(0, (y - mid) - can_use_r));
            answers.push_back(ans);
        }

        return answers;
    }
};
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Example from problem statement
    {
        std::vector<int> h = {0, 1, 3, 2, 5}; // 1-based: h[1]=1, h[2]=3, h[3]=2, h[4]=5
        std::vector<std::pair<int,int>> queries = {{1,4}, {2,3}, {3,1}, {4,4}};
        auto ans = SparseTableQueries::solveQueries(h, queries);
        std::vector<long long> expected = {16, 8, 8, 0};
        assert(ans.size() == expected.size());
        for (size_t i = 0; i < ans.size(); ++i) {
            assert(ans[i] == expected[i]);
        }
    }

    // Single element
    {
        std::vector<int> h = {0, 7};
        std::vector<std::pair<int,int>> queries = {{1,1}};
        auto ans = SparseTableQueries::solveQueries(h, queries);
        assert(ans.size() == 1);
        assert(ans[0] == 0);
    }

    // Two elements, same height
    {
        std::vector<int> h = {0, 5, 5};
        std::vector<std::pair<int,int>> queries = {{1,2}};
        auto ans = SparseTableQueries::solveQueries(h, queries);
        // T=5, both deltas 0. mid=1 (leftmost). L = max(l[1]=5+2+1=8, l[2]=5+1+1=7) =8, Lmax=8, l[x]=l[1]=8, can_use_l=0. R = max(r[1]=5+1=6, r[2]=5+2=7) =7, r[x]=r[1]=6, can_use_r=0. Then ans += 2*( (0) + (1) ) =2. ans=2.
        assert(ans[0] == 2);
    }

    // Larger random test, check against brute force
    {
        std::vector<int> h = {0, 2, 4, 1, 3, 5, 2, 8};
        std::vector<std::pair<int,int>> queries = {{2,5}, {6,1}, {7,3}, {4,4}, {8,2}};
        // Brute force computation
        auto brute = [&](int x, int y) -> long long {
            int ox = x, oy = y;
            long long a, b;
            if (x > y) { std::swap(x,y); a=4; b=1; } else { a=1; b=4; }
            int n = static_cast<int>(h.size())-1;
            int T = h[x];
            for (int i=x; i<=y; ++i) T = std::max(T, h[i]);
            long long ans = (long long)(T - h[ox]) * a + (long long)(T - h[oy]) * b;
            // find leftmost max index
            int mid = x;
            for (int i=x; i<=y; ++i) if (h[i] > h[mid]) mid = i;
            int Lmax = h[x] + (n-x) + 1;
            for (int i=x; i<=y; ++i) Lmax = std::max(Lmax, h[i] + (n-i) + 1);
            int Rmax = h[x] + x;
            for (int i=x; i<=y; ++i) Rmax = std::max(Rmax, h[i] + i);
            int lx = h[x] + (n-x) + 1;
            int rx = h[x] + x;
            int can_use_l = (mid - x) - (Lmax - lx);
            int can_use_r = (Rmax - rx) - (y - mid);
            ans += 2LL * (std::max(0, (mid - x) - can_use_l) + std::max(0, (y - mid) - can_use_r));
            return ans;
        };

        auto ans = SparseTableQueries::solveQueries(h, queries);
        assert(ans.size() == queries.size());
        for (size_t i = 0; i < queries.size(); ++i) {
            long long expected = brute(queries[i].first, queries[i].second);
            assert(ans[i] == expected);
        }
    }

    return 0;
}
// The problem requires efficient range queries: maximum height in a subarray, the index of that maximum, and the maximum of two derived arrays (`l[i] = h[i] + n - i + 1` and `r[i] = h[i] + i`). We precompute sparse tables for each of these four quantities (height value, index of maximum, max of `l`, max of `r`). For each query, using sparse table range queries we obtain `T`, `m`, `L`, `R` in `O(1)`. The rest of the computation is constant time. Edge cases: when `x` and `y` are swapped, the multipliers must be swapped accordingly; the maximum index must be chosen as the smallest index on ties (we implement `mx` accordingly using `p[x] > p[y] ? x : y`, i.e., choose leftmost on ties). The formulas for `can_use_l` and `can_use_r` may yield negative values; we take the maximum with `0`. We must use `long long` for the answer because intermediate sums can exceed 32-bit range. Time complexity: `O(n log n)` preprocessing and `O(1)` per query, total `O(n log n + m)`. Space complexity: `O(n log n)`.
