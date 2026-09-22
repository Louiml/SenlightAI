// You are given an array `x` of `n` integers (1-indexed) and `q` queries. Each query provides a range `[L, R]` (1 ≤ L ≤ R ≤ n). For each query, consider all subarrays that start at index `L` and end at some index `e` where `L ≤ e ≤ R`. For each such subarray `x[L..e]`, define its "contribution" as the sum of the maximum element of every prefix of that subarray. More formally, for a subarray starting at `L`, define `S(e) = sum_{k=L}^{e} max_{j=L}^{k} x[j]`. Your task is to compute, for each query `[L, R]`, the value `sum_{e=L}^{R} S(e)`. In other words, for each query, sum over all ending positions `e` in `[L, R]` the sum of prefix maxima for the subarray `x[L..e]`. Output the result for each query as a 64-bit integer.
//
// **Input format:** The first line contains two integers `n` and `q`. The second line contains `n` integers `x[1..n]`. Then `q` lines follow, each with two integers `L` and `R` (1 ≤ L ≤ R ≤ n). Output `q` lines, one per query, the answer modulo nothing (use 64-bit long long). The array values satisfy `1 ≤ x[i] ≤ 10^9` and `n, q ≤ 10^5`.
//
// **Example:** For `n=3`, `x = [3,1,2]`, query `[1,3]`:
// - Subarrays starting at 1: `[3]` prefix maxima sum = 3; `[3,1]` prefix maxima sum = 3+3=6; `[3,1,2]` prefix maxima sum = 3+3+3=9 → S(1)=3, S(2)=6, S(3)=9, total = 18. Query answer = 18.

#include <cassert>
#include <vector>
using namespace std;

vector<ll> prefixMaxRangeQueries(const vector<ll>& x, const vector<pair<int,int>>& queries);

int main() {
    // Example from statement
    vector<ll> x1 = {0, 3, 1, 2}; // 1-indexed
    vector<pair<int,int>> q1 = {{1,3}};
    auto r1 = prefixMaxRangeQueries(x1, q1);
    assert(r1.size() == 1 && r1[0] == 18);

    // Single element array
    vector<ll> x2 = {0, 7};
    vector<pair<int,int>> q2 = {{1,1}};
    auto r2 = prefixMaxRangeQueries(x2, q2);
    assert(r2[0] == 7); // S(1)=7, sum=7

    // Decreasing array: each subarray prefix max is always x[L]
    vector<ll> x3 = {0, 5, 4, 3};
    vector<pair<int,int>> q3 = {{1,3}};
    // L=1: S(1)=5, S(2)=5+5=10, S(3)=5+5+5=15 total=30
    auto r3 = prefixMaxRangeQueries(x3, q3);
    assert(r3[0] == 30);

    // Increasing array: prefix max of subarray x[L..e] is x[e]
    vector<ll> x4 = {0, 1, 2, 3};
    vector<pair<int,int>> q4 = {{1,3}};
    // S(1)=1, S(2)=1+2=3, S(3)=1+2+3=6 total=10
    auto r4 = prefixMaxRangeQueries(x4, q4);
    assert(r4[0] == 10);

    // Multiple queries
    vector<ll> x5 = {0, 2, 1, 3};
    vector<pair<int,int>> q5 = {{1,4},{2,3},{1,2}};
    auto r5 = prefixMaxRangeQueries(x5, q5);
    // Query [1,4]: manually compute S(e)
    // e=1: [2] -> 2
    // e=2: [2,1] -> 2+2=4
    // e=3: [2,1,3] -> 2+2+3=7
    // e=4: [2,1,3,?] no, array length 4? Actually x5 has indices 1..4 with values 2,1,3,? wait x5 is {0,2,1,3} so only 3 elements. So n=3. Adjust test.
    // Let's redo: x5 = {0,2,1,3} means n=3. Queries are {1,3}? Let's fix.
    vector<ll> x6 = {0, 2, 1, 3};
    vector<pair<int,int>> q6 = {{1,3},{2,3},{1,1}};
    auto r6 = prefixMaxRangeQueries(x6, q6);
    // [1,3]: S(1)=2, S(2)=2+2=4, S(3)=2+2+3=7 total=13
    assert(r6[0] == 13);
    // [2,3]: L=2, S(2)=1, S(3)=1+3=4 total=5? Wait S(e) for e from L to R:
    // e=2: subarray [1]? Actually subarray x[2..2]=[1] prefix max =1
    // e=3: subarray [1,3] prefix max: max(1)=1, max(1,3)=3 sum=4 total=5
    assert(r6[1] == 5);
    // [1,1]: S(1)=2
    assert(r6[2] == 2);

    // All equal values
    vector<ll> x7 = {0, 4, 4, 4};
    vector<pair<int,int>> q7 = {{1,3}};
    // L=1: S(1)=4, S(2)=4+4=8, S(3)=4+4+4=12 total=24
    auto r7 = prefixMaxRangeQueries(x7, q7);
    assert(r7[0] == 24);

    return 0;
}

#include <vector>
#include <functional>
using namespace std;
using ll = long long;

// Computes for each query [L,R] the sum over e in [L,R] of sum of prefix maxima of subarray x[L..e].
// x is 1-indexed, queries are 0-indexed lists of pairs (L,R) with 1 <= L <= R <= n.
vector<ll> prefixMaxRangeQueries(const vector<ll>& x, const vector<pair<int,int>>& queries) {
    int n = x.size() - 1; // x[0] is unused
    int q = queries.size();
    vector<vector<pair<int,int>>> byLeft(n + 1);
    for (int idx = 0; idx < q; ++idx) {
        int L = queries[idx].first;
        int R = queries[idx].second;
        byLeft[L].push_back({R, idx});
    }

    vector<ll> bit(n + 2, 0);
    auto update = [&](int idx, ll val) {
        while (idx <= n + 1) {
            bit[idx] += val;
            idx += idx & -idx;
        }
    };
    auto query = [&](int idx) {
        ll s = 0;
        while (idx > 0) {
            s += bit[idx];
            idx -= idx & -idx;
        }
        return s;
    };

    vector<ll> pref(n + 1, 0);
    for (int i = 1; i <= n; ++i) pref[i] = pref[i-1] + x[i];

    vector<pair<ll,int>> st; // (value, index)
    vector<ll> contrib(n + 2, 0); // contribution of each stack element (1-based position)
    vector<ll> ans(q, 0);

    for (int i = n; i >= 1; --i) {
        // maintain monotonic decreasing stack (strictly decreasing values)
        while (!st.empty() && st.back().first <= x[i]) {
            int pos = st.size(); // 1-based position of last element
            update(pos, -contrib[pos]);
            contrib[pos] = 0;
            st.pop_back();
        }
        int nextIndex = st.empty() ? n + 1 : st.back().second;
        int len = nextIndex - i;
        int newPos = st.size() + 1; // 1-based position after push
        contrib[newPos] = (ll)len * x[i];
        update(newPos, contrib[newPos]);
        st.push_back({x[i], i});

        for (auto& qr : byLeft[i]) {
            int R = qr.first;
            int qid = qr.second;
            // find first stack element with index <= R (i.e., the block containing R)
            int lo = 0, hi = st.size() - 1, valid = -1;
            while (lo <= hi) {
                int mid = lo + (hi - lo) / 2;
                if (st[mid].second <= R) {
                    valid = mid; // 0-based index in st
                    hi = mid - 1;
                } else {
                    lo = mid + 1;
                }
            }
            // valid must be found because R >= i
            ll sum1 = query(st.size() + 1) - query(valid + 1); // full blocks after valid
            ll sum2 = (ll)(R - st[valid].second + 1) * st[valid].first; // partial block
            ll pref_sub = pref[R] - pref[i-1]; // sum of x[i..R]
            ans[qid] = sum1 + sum2 - pref_sub;
        }
    }
    return ans;
}

// We process queries offline by sorting them by their left endpoint in descending order. A monotonic decreasing stack of pairs `(value, index)` is maintained for the suffix of the array from current left index `i` to `n`. When moving `i` from `n` down to `1`, we pop elements from the stack that are ≤ `x[i]`, and for each popped element at stack position `p` (0-based), we need to revert its contribution to a Fenwick tree over stack positions. Each stack element at position `p` (1-based in the Fenwick) represents a block of consecutive array indices whose maximum is that element's value. The block length is the difference between the next stack element's index (or `n+1`) and the current element's index. For a new `x[i]`, after popping, the new element becomes the top of stack with block length equal to `(nextIndex - i)`, and we add its contribution `len * x[i]` to the Fenwick at the new stack position. For a query `[i, R]`, we need to compute the sum of `S(e)` for `e` from `i` to `R`. This sum can be decomposed into two parts: contributions from complete blocks that lie entirely within `[i, R]` (these are obtained from the Fenwick over stack positions up to the block that contains `R`), plus a partial contribution from the block that contains `R`. The partial contribution is `(R - blockStart + 1) * value`. However, we must subtract the baseline sum of all `x[k]` from `i` to `R` because each `S(e)` includes the sum of the original elements, which we compensate by subtracting the prefix sum of the original array over that range. To find the block containing `R`, we binary search on the stack to find the smallest stack index with `index ≤ R`; that block's value is the maximum for all positions from `i` to that index. Then `sum1` is the Fenwick sum over stack positions 1..`size` minus the sum over positions ≤ `valid` (the block containing `R`), `sum2` is the partial block contribution, and finally subtract `pref[R+1] - pref[i]`. Complexity: O((n+q) log n) time, O(n+q) space.
