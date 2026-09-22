/*
You are given an array of `n` integers (which may be negative, zero, or positive). Define a *valid* sequence of indices `1 ≤ i₁ < i₂ < ... < iₖ ≤ n` such that the cumulative sum from the start of the array up to index `i₁` (i.e., `a₁ + ... + a_{i₁}`) is positive, and for every consecutive pair `(iₜ, i_{t+1})`, the sum of elements strictly between them plus the element at `i_{t+1}` (i.e., `a_{iₜ+1} + ... + a_{i_{t+1}}`) is also positive. More formally, if `P[j] = a₁ + a₂ + ... + aⱼ` (with `P[0] = 0`), then a valid sequence is defined by conditions: `P[i₁] > 0`, and for each `t ≥ 1`, `P[i_{t+1}] - P[iₜ] > 0` (this includes the element at `i_{t+1}`). Note that the first element `a₁` itself may be included only if `P[1] > 0` (which is true iff `a₁ > 0`). Write a C++ function that, given a vector of integers (1-indexed conceptually), returns the length of the **longest valid sequence** (maximum possible `k`). If no valid sequence exists (i.e., cannot even pick a first index with positive prefix sum), return `-1` (or `0` if you prefer, but be explicit). The function must be efficient for `n` up to `100,000` and values up to `10^9` in absolute value. You may assume the input vector is non‑empty.
*/

#include <vector>
#include <algorithm>
#include <limits>

// Returns the length of the longest valid sequence as described, or -1 if none.
int longestValidSequence(const std::vector<long long>& a) {
    int n = static_cast<int>(a.size());
    if (n == 0) return -1;

    // Prefix sums: S[0]=0, S[i]=a[0]+...+a[i-1]
    std::vector<long long> S(n + 1, 0);
    std::vector<long long> coords;
    coords.reserve(n);
    for (int i = 1; i <= n; ++i) {
        S[i] = S[i-1] + a[i-1];
        coords.push_back(S[i]);
    }

    // Coordinate compression
    std::sort(coords.begin(), coords.end());
    coords.erase(std::unique(coords.begin(), coords.end()), coords.end());
    int m = static_cast<int>(coords.size());

    // Segment tree: stores max dp value for each prefix sum value (1-indexed positions)
    std::vector<int> seg(4 * m + 5, -1);

    // Lambda functions for update and query
    auto update = [&](int pos, int val, int node, int l, int r) {
        if (l == r) {
            seg[node] = std::max(seg[node], val);
            return;
        }
        int mid = (l + r) / 2;
        if (pos <= mid) update(pos, val, node*2, l, mid);
        else update(pos, val, node*2+1, mid+1, r);
        seg[node] = std::max(seg[node*2], seg[node*2+1]);
    };

    std::function<int(int,int,int,int)> query = [&](int ql, int qr, int node, int l, int r) {
        if (ql > r || qr < l || ql > qr) return -1;
        if (ql <= l && r <= qr) return seg[node];
        int mid = (l + r) / 2;
        return std::max(query(ql, qr, node*2, l, mid),
                        query(ql, qr, node*2+1, mid+1, r));
    };

    int best = -1;
    for (int i = 1; i <= n; ++i) {
        // Coordinate of S[i]
        int pos = static_cast<int>(std::lower_bound(coords.begin(), coords.end(), S[i]) - coords.begin()) + 1;

        // Query for positions with value strictly less than S[i]
        int left = 1, right = pos - 1;
        int maxPrev = query(left, right, 1, 1, m);

        int dp_i;
        if (maxPrev != -1) {
            dp_i = maxPrev + 1;
        } else if (S[i] > 0) {
            dp_i = 1;
        } else {
            dp_i = -1;
        }

        if (dp_i != -1) best = std::max(best, dp_i);
        update(pos, dp_i, 1, 1, m);
    }

    return best;
}

#include <cassert>
#include <vector>
#include <iostream>

// Assume the solution function is included above.

int main() {
    // Example 1: simple increasing prefixes
    std::vector<long long> a1 = {1, 1, 1};
    assert(longestValidSequence(a1) == 3); // S: 1,2,3 -> all positive and increasing

    // Example 2: first element negative, can't start
    std::vector<long long> a2 = {-1, 5};
    assert(longestValidSequence(a2) == 1); // only S[2]=4 >0, but no previous valid, so length 1

    // Example 3: no valid sequence
    std::vector<long long> a3 = {-1, -1, -1};
    assert(longestValidSequence(a3) == -1);

    // Example 4: sequence with zeros
    std::vector<long long> a4 = {2, -1, 0, 3};
    // S: 2,1,1,4 -> valid indices: i1=1 (S=2), i2=2 (S=1? no, must increase) wait S2=1 <2 not allowed; try i1=1(S=2), i3=3(S=1 not >2), i4=4(S=4>2) so length 2; also i1=2(S=1) invalid because not positive? S2=1>0 but S1=2 already used, but if start at i=2, S=1>0, then i=4 S=4>1 -> length2. So max is 2.
    assert(longestValidSequence(a4) == 2);

    // Example 5: negative then positive recover
    std::vector<long long> a5 = {-2, 3, -1, 2};
    // S: -2,1,0,2 -> valid: i1=2 (S=1), i2=4 (S=2) -> length2; also i1=2(S=1) and i3=3(S=0 not >1) no; so length2.
    assert(longestValidSequence(a5) == 2);

    // Example 6: all positive but not increasing prefix? Actually prefix is always increasing if all positive.
    std::vector<long long> a6 = {5, 1, 10};
    // S:5,6,16 -> all increasing -> length3
    assert(longestValidSequence(a6) == 3);

    // Example 7: large values
    std::vector<long long> a7 = {1000000000LL, -999999999LL, 1000000000LL, -1LL};
    // S: 1e9, 1, 1000000001, 1000000000 -> valid? S1>0, S3> S1? 1e9+1 >1e9 yes, so length2; also S2=1>0 but then S4=1e9 >1, length2; so max2.
    assert(longestValidSequence(a7) == 2);

    // Example 8: single positive
    std::vector<long long> a8 = {7};
    assert(longestValidSequence(a8) == 1);

    // Example 9: single negative
    std::vector<long long> a9 = {-7};
    assert(longestValidSequence(a9) == -1);

    // Example 10: zero first then positive
    std::vector<long long> a10 = {0, 5};
    // S:0,5 -> S2>0, but S1=0 not >0; so only index2 valid -> length1
    assert(longestValidSequence(a10) == 1);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The problem is a longest increasing subsequence (LIS) variant on prefix sums. Let `S[i] = a₁ + ... + aᵢ` for `i=1..n` (with `S[0]=0`). A valid sequence of indices `i₁ < i₂ < ... < iₖ` must satisfy `S[i₁] > 0` and `S[iₜ] > S[i_{t-1}]` for all `t≥2`. Therefore, the problem reduces to finding the longest increasing subsequence of the prefix sums `S[1], S[2], ..., S[n]` where the first selected element must be positive. The first selected index must have `S[i₁] > 0`; after that, we can pick any later indices with strictly increasing `S` values. This can be solved with a segment tree over compressed prefix sum values. We process indices from left to right; for each `i`, we compute the best length of a sequence ending at `i`. For `i=1`, if `S[1] > 0` then `dp[1]=1` else `dp[1]=-∞`. For `i>1`, we query the maximum `dp[j]` among `j < i` with `S[j] < S[i]` (i.e., the longest increasing subsequence ending with a smaller prefix sum). But also we must ensure that the first element is positive. If we only consider `dp` values computed from valid starts, then any `dp[j]` represents a valid sequence, so we can just take `max` over all `j<i` with `S[j] < S[i]` and add 1. However, if no valid sequence exists yet (i.e., all `dp` are `-∞`), then the query returns `-∞`, so we check separately if `S[i] > 0` and set `dp[i]=1`. In practice, we can initialize `dp[i]` as `-1` (meaning invalid). We maintain a segment tree over compressed coordinates of `S` values, storing the maximum `dp` for each prefix sum value. For each `i`, we first query the maximum over all prefix sums less than `S[i]`; if that max is `≥0`, then `dp[i] = max+1`; else if `S[i] > 0`, `dp[i]=1`; else `dp[i]=-1` (invalid). Then we update the tree at position `get(S[i])` with `dp[i]`. The answer is `max(dp[1..n])`, but if all are `-1` then answer is `-1`. Edge cases: prefix sums can be negative, zero, or positive; values can overflow int, so use `long long`. Coordinate compression handles large values. Time complexity: `O(n log n)` for sorting and segment tree operations. Space: `O(n)` for storing prefix sums, compressed coordinates, and segment tree.
