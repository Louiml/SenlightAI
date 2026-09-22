Given an array `a` of `n` positive integers and a non-negative integer `k`, write a C++ function `int solveMaxP(const std::vector<int>& a, int k)` that returns the maximum possible value `p` such that there exists an index `i` (1 ≤ i ≤ n) with the following property: after deleting at most `k` elements from the array (where deletion means removing an element from the array, and the relative order of the remaining elements is preserved), the element originally at position `i` can become the start of a valid "staircase" of length at least? Actually, the original problem asks: find the largest `p` such that there exists a position `i` where we can select a subsequence (not necessarily contiguous) of length at least `n - k` that starts with the element at position `i`, and the subsequence satisfies `a[i] >= p`, `a[i+1] >= p-1`, `a[i+2] >= p-2`, ..., down to `a[i + len - 1] >= p - (len-1)` (assuming the subsequence indices are sorted and start at i). In other words, we need to choose a subsequence of length at least `n - k` such that if the first chosen index is `i`, and the chosen indices are `i = i1 < i2 < ... < im`, then for each `t = 0..m-1`, we must have `a[i_{t+1}] >= p - t` (with the condition that `p - t` can be negative, and we only require non‑negative because array values are positive; if `p - t` is negative, the condition is automatically satisfied). We need to maximize `p` (which can be as large as 1e9). Write the function that returns that maximum `p`.
#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long;

// Include the solution function here (in an actual environment, it would be included or defined above).
// For brevity, we assume solveMaxP is defined above.

int main() {
    // Test 1: Simple case, whole array works
    vector<ll> a1 = {5, 4, 3, 2, 1};
    int k1 = 0;
    // Need p such that [5>=p,4>=p-1,3>=p-2,...] works. Max p=5 works? 5>=5,4>=4,3>=3,2>=2,1>=1 -> yes. p=6? 5>=6 no. So answer 5.
    assert(solveMaxP(a1, k1) == 5);

    // Test 2: Can delete one element
    vector<ll> a2 = {1, 10, 9, 8, 1};
    int k2 = 1;
    // Try p=9: start at index 2, subsequence [2,3,4] gives lengths 10>=9,9>=8,8>=7 -> works, length 3 = n-k (5-2? Actually n=5,k=1 -> need >=4, so length 3 not enough). Let's compute manually: n=5, need >=4. Try p=3: start at 2, pick 2,3,4,5? 10>=3,9>=2,8>=1,1>=0 -> works, length 4. So answer at least 3. Probably answer 3 or 4? Test p=4: 10>=4,9>=3,8>=2,1>=1 -> 1>=1 OK, length 4 works. p=5: 10>=5,9>=4,8>=3,1>=2 no, so maybe start at 1? 1>=5 no, start at 2 works up to 3 elements? length 3 <4. So answer 4. 
    assert(solveMaxP(a2, k2) == 4);

    // Test 3: All same values
    vector<ll> a3 = {7, 7, 7, 7};
    int k3 = 2;
    // need length >=2. p can be 7? Starting at any, e.g., first two: 7>=7,7>=6 works. p=8 fails. So answer 7.
    assert(solveMaxP(a3, k3) == 7);

    // Test 4: k = n-1, can keep one element
    vector<ll> a4 = {1, 2, 3};
    int k4 = 2;
    // need length >=1, any p up to max(a) works, max is 3.
    assert(solveMaxP(a4, k4) == 3);

    // Test 5: k = 0, impossible to satisfy for high p
    vector<ll> a5 = {1, 1, 1};
    int k5 = 0;
    // need length 3. p=1: 1>=1,1>=0,1>=-1 works. p=2 fails. Answer 1.
    assert(solveMaxP(a5, k5) == 1);

    // Test 6: Large p possible with one element
    vector<ll> a6 = {1000000000LL};
    int k6 = 0;
    assert(solveMaxP(a6, k6) == 1000000000LL);

    // Test 7: Single element with k=0
    vector<ll> a7 = {5};
    int k7 = 0;
    assert(solveMaxP(a7, k7) == 5);

    // Test 8: Edge with negative thresholds (p small)
    vector<ll> a8 = {1, 2, 3, 4, 5};
    int k8 = 0;
    // whole array works for p=1? 1>=1,2>=0,3>=-1,... yes, so p=1. Actually p=2? 1>=2 no, so answer 1.
    assert(solveMaxP(a8, k8) == 1);

    // Test 9: More complex with deletion
    vector<ll> a9 = {10, 1, 9, 8, 7};
    int k9 = 1;
    // n=5, need >=4. Try p=7: start at 1? 10>=7, then 1>=6 no. Start at 3? 9>=7,8>=6,7>=5, length 3 <4. Try p=6: start at 1: 10>=6, then skip 1 (delete), then 9>=5,8>=4,7>=3 -> length 4 works. So p=6 works. p=7? start at 1: 10>=7, need next >=6, skip 1, take 9>=6,8>=5,7>=4 -> length 4 works! So p=7 works? Actually thresholds: start at 1: need 7,6,5,4,... 10>=7, 9>=6,8>=5,7>=4 -> works, length 4. So p=7. p=8? 10>=8,9>=7,8>=6,7>=5 length 4 works, p=8. p=9? 10>=9,9>=8,8>=7,7>=6 length 4 works, p=9. p=10? 10>=10,9>=9,8>=8,7>=7 length 4 works! p=10 works? Actually start at 1 length 4, yes. p=11? 10>=11 no. So answer 10.
    assert(solveMaxP(a9, k9) == 10);

    // Test 10: Random stress test (small) – compare with brute force for small n
    {
        mt19937 rng(12345);
        for (int t = 0; t < 20; t++) {
            int n = 1 + rng() % 6;
            int k = rng() % n;
            vector<ll> a(n);
            for (int i = 0; i < n; i++) a[i] = 1 + rng() % 20;
            int brute = 0;
            for (int p = 1; p <= 30; p++) {
                bool ok = false;
                // brute force over all subsequences of length >= n-k
                vector<int> idx(n);
                iota(idx.begin(), idx.end(), 0);
                // generate all non‑empty subsets
                for (int mask = 1; mask < (1 << n); mask++) {
                    if (__builtin_popcount(mask) < n - k) continue;
                    vector<int> sub;
                    for (int i = 0; i < n; i++) if (mask & (1 << i)) sub.push_back(i);
                    // check condition starting at sub[0]
                    bool valid = true;
                    for (int pos = 0; pos < (int)sub.size(); pos++) {
                        if (a[sub[pos]] < p - pos) { valid = false; break; }
                    }
                    if (valid) { ok = true; break; }
                }
                if (ok) brute = p;
            }
            int fast = solveMaxP(a, k);
            assert(fast == brute);
        }
    }

    cout << "All tests passed!" << endl;
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 1e18;

// Lazy segment tree supporting range add (subtract) and query min + index of min.
struct SegTree {
    int n;
    vector<pair<ll, int>> tree; // {value, index}
    vector<ll> lazy;

    SegTree(int sz) : n(sz), tree(4 * sz), lazy(4 * sz, 0) {}

    void build(int node, int l, int r) {
        if (l == r) {
            tree[node] = {INF, l};
            return;
        }
        int mid = (l + r) / 2;
        build(node * 2, l, mid);
        build(node * 2 + 1, mid + 1, r);
        tree[node] = min(tree[node * 2], tree[node * 2 + 1]);
    }

    void apply(int node, ll val) {
        tree[node].first += val;
        lazy[node] += val;
    }

    void push(int node) {
        if (lazy[node] != 0) {
            apply(node * 2, lazy[node]);
            apply(node * 2 + 1, lazy[node]);
            lazy[node] = 0;
        }
    }

    void range_add(int node, int l, int r, int ql, int qr, ll val) {
        if (l > qr || r < ql) return;
        if (ql <= l && r <= qr) {
            apply(node, val);
            return;
        }
        push(node);
        int mid = (l + r) / 2;
        range_add(node * 2, l, mid, ql, qr, val);
        range_add(node * 2 + 1, mid + 1, r, ql, qr, val);
        tree[node] = min(tree[node * 2], tree[node * 2 + 1]);
    }

    void point_set(int node, int l, int r, int idx, ll val) {
        if (l == r) {
            tree[node] = {val, l};
            return;
        }
        push(node);
        int mid = (l + r) / 2;
        if (idx <= mid) point_set(node * 2, l, mid, idx, val);
        else point_set(node * 2 + 1, mid + 1, r, idx, val);
        tree[node] = min(tree[node * 2], tree[node * 2 + 1]);
    }

    pair<ll, int> query_min() {
        return tree[1];
    }
};

// Compute for a fixed p the array b where b[i] = max length of valid subsequence starting at i.
vector<int> computeB(const vector<ll>& a, ll p) {
    int n = (int)a.size();
    SegTree st(n);
    st.build(1, 0, n - 1);
    vector<int> b(n, 0);
    vector<bool> active(n, false);
    int need = p;
    int got = 0;

    for (int i = 0; i < n; i++) {
        if (a[i] >= need) {
            got++;
            st.point_set(1, 0, n - 1, i, a[i] - need);
            need--;
            active[i] = true;
        }

        // Remove any active positions that are now invalid (slack < 0)
        while (true) {
            auto [minVal, pos] = st.query_min();
            if (minVal < 0) {
                got--;
                active[pos] = false;
                st.point_set(1, 0, n - 1, pos, INF);
                if (pos + 1 < n) st.range_add(1, 0, n - 1, pos + 1, n - 1, -1);
            } else break;
        }

        // Finish subsequence starting at i
        if (active[i]) {
            b[i] = got;
            active[i] = false;
            got--;
            st.point_set(1, 0, n - 1, i, INF);
            st.range_add(1, 0, n - 1, 0, n - 1, -1);
        }
    }
    return b;
}

// Solves the problem: returns maximum p such that there exists a subsequence of length at least n - k
// with condition a[first] >= p, a[next] >= p-1, ..., a[last] >= p - (len-1).
int solveMaxP(const std::vector<ll>& a, int k) {
    int n = (int)a.size();
    ll lo = 0, hi = 1000000000LL;
    while (lo < hi) {
        ll mid = (lo + hi + 1) / 2;
        vector<int> b = computeB(a, mid);
        vector<ll> revA(a.rbegin(), a.rend());
        vector<int> cRev = computeB(revA, mid);
        vector<int> c(n);
        for (int i = 0; i < n; i++) c[i] = cRev[n - 1 - i];

        bool good = false;
        for (int i = 0; i < n; i++) {
            int total = b[i] + c[i] - 1;
            if (total >= n - k) {
                good = true;
                break;
            }
        }
        if (good) lo = mid;
        else hi = mid - 1;
    }
    return (int)lo;
}
// The key observation: for a fixed `p`, we need to know, for each possible starting position `i`, what is the maximum length of a valid subsequence starting at `i` following the decreasing threshold pattern. Because we are allowed to delete up to `k` elements, we want to know if there exists an `i` such that the length of the longest valid subsequence starting at `i` is at least `n - k`. 
//
// We can compute two arrays using a greedy + lazy segment tree technique:
// - `b[i]` = maximum length of a valid subsequence that *starts at index i* (from left to right) with threshold `p` at position `i`, then `p-1` at the next selected index, etc.
// - `c[i]` = maximum length of a valid subsequence that *ends at index i* when processing from right to left (i.e., equivalent to starting at `i` in the reversed array). 
// Then for any `i`, the total length of a valid subsequence that has `i` as a "pivot" is `b[i] + c[i] - 1` (because `i` is counted in both), and we need this total to be at least `n - k`. The existence of such `i` means `p` is feasible.
//
// To compute `b[i]` efficiently for a given `p`, we scan `i` from 1 to n. Maintain a set of "active" positions that are currently valid followed by a decreasing threshold. Use a lazy segment tree over positions 1..n storing for each position the "slack" = `a[pos] - neededThresholdForPos` (initially INF). When we start a new subsequence at `i`, we set that position's slack to `a[i] - p` and mark it as active. Then, as we move to the next index, all active positions have their required threshold decreased by 1, so we apply a range subtract 1 to all positions from `i+1` to `n`. Any active position whose slack becomes negative is "invalid" and must be removed (because the element can no longer satisfy the threshold). When we remove a position, we set its slack back to INF and also subtract 1 from all later positions (since the required threshold for the rest also drops by 1 when we lose that element). While iterating `i`, we also finish the subsequence starting at `i` when we move past it: we record its current count `got` as `b[i]`, then remove `i` from active set (set slack to INF) and subtract 1 from all positions (since the threshold for all later positions becomes lower by 1).
//
// The greedy works because at each step we always take the earliest possible element that satisfies the current threshold, and if an earlier element becomes invalid due to threshold dropping, we necessarily have to drop it (no alternative because the subsequence must be increasing in index). This gives the maximum length for each starting index.
//
// For the reversed array, we compute `c` similarly, and then reverse `c` back to align with original indices.
//
// Binary search on `p` from 0 to 1e9 (inclusive). For each `p`, compute `b` and `c`, then check if any `i` gives `b[i] + c[i] - 1 >= n - k`. If yes, lower bound is feasible; else not.
//
// Time complexity: For each `p`, the computation runs in O(n log n) due to segment tree operations, and binary search adds a factor of log(maxA) ≈ 30. So total O(n log n log maxA). Space complexity O(n).
//
// Edge cases: `p` can be 0 (always feasible because thresholds are all ≤ 0 and all elements are positive). Also, when `k = 0`, we must take entire array, so the only possible `i` is 1, and we need the entire array to satisfy the thresholds; the algorithm handles this. The segment tree uses INF = 1e18; careful with overflow.
