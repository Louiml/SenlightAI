// You are given three integers: a cycle length `m`, a sequence length `n`, and a number of queries `q`. The cycle is defined by a permutation of the integers `0` through `m-1`, given in the order they appear in the cycle. You are also given a sequence `b` of length `n` where each element is one of the cycle elements (not necessarily distinct). For each query `(l, r)` (1-indexed, inclusive), you must determine whether the subarray `b[l..r]` contains, as a contiguous subsequence, a full traversal of the cycle in the correct cyclic order. More precisely, define the full validation as: the subarray must contain a contiguous run of `m` elements such that, if you convert each element to its position in the cycle (0-based), the positions form the sequence `0,1,2,...,m-1` exactly in that order (cyclically, so the run could start at any position in the cycle but must go through all `m` distinct positions in increasing order modulo `m`). Write a C++ function that, given the cycle permutation, the sequence, and the queries, returns a string of length `q` where each character is `'1'` if the corresponding subarray contains such a run, and `'0'` otherwise. The function signature is: `std::string solve(int m, const std::vector<int>& cycle, const std::vector<int>& seq, const std::vector<std::pair<int,int>>& queries)`. The input sequence and query indices are 1-based. The cycle is given as a vector of `m` distinct integers from `0` to `m-1` listing the cycle order (so the position of a value `x` is its index in this vector). Constraints: `1 <= m <= 100000`, `1 <= n <= 100000`, `1 <= q <= 100000`. The function should be efficient for these limits.
#include <cassert>
#include <vector>
#include <string>
int main() {
    // Basic case: m=3, cycle 0,1,2, sequence 0,1,2,0, queries
    {
        std::vector<int> cycle = {0, 1, 2};
        std::vector<int> seq = {0, 1, 2, 0};
        std::vector<std::pair<int,int>> queries = {{1,3}, {1,4}, {2,4}, {2,3}};
        std::string res = solve(3, cycle, seq, queries);
        assert(res == "1110");
    }
    // m=1: any subarray works
    {
        std::vector<int> cycle = {5};
        std::vector<int> seq = {5, 5, 5};
        std::vector<std::pair<int,int>> queries = {{1,1}, {2,3}, {1,3}};
        std::string res = solve(1, cycle, seq, queries);
        assert(res == "111");
    }
    // m > n: always 0
    {
        std::vector<int> cycle = {0, 1, 2, 3};
        std::vector<int> seq = {0, 1};
        std::vector<std::pair<int,int>> queries = {{1,2}, {1,1}};
        std::string res = solve(4, cycle, seq, queries);
        assert(res == "00");
    }
    // Cycle not starting at 0, but positions matter
    {
        std::vector<int> cycle = {2, 0, 1}; // positions: 2->0, 0->1, 1->2
        std::vector<int> seq = {2, 0, 1, 2, 0}; // positions: 0,1,2,0,1
        std::vector<std::pair<int,int>> queries = {{1,3}, {1,5}, {2,5}, {3,5}};
        std::string res = solve(3, cycle, seq, queries);
        // runs: [1..3] positions 0,1,2 -> yes; [1..5] contains 0,1,2 at start -> yes; [2..5] positions 1,2,0,1 -> no full run? wait 1,2,0 is not 0,1,2; check: 2..4 is positions 1,2,0 -> no; [3..5] positions 2,0,1 -> no full run? Actually 2,0,1 is not 0,1,2, so no. But [2..5] length 4, can start at 2? positions 1,2,0,1, no; start at 3? 2,0,1 no; so 0. [3..5] positions 2,0,1 no. So expect "1100"
        assert(res == "1100");
    }
    // Multiple valid runs overlapping
    {
        std::vector<int> cycle = {0, 1, 2};
        std::vector<int> seq = {0, 1, 2, 0, 1, 2};
        std::vector<std::pair<int,int>> queries = {{2,5}, {1,6}, {3,6}};
        std::string res = solve(3, cycle, seq, queries);
        // [2..5] positions 1,2,0,1 -> no; [1..6] contains 0,1,2 at start and 0,1,2 at 4..6 -> yes; [3..6] positions 2,0,1,2 -> no full run? start at 3? 2,0,1 no; start at 4? 0,1,2 yes but start 4 is inside? Actually [3..6] indices 3,4,5,6 (1-based) includes positions 2,0,1,2, subarray length 4, need run length 3, start at 3? 2,0,1 no; start at 4? 0,1,2 yes -> valid. So "010"? Wait [2..5] is 0-based? Let's trust logic: expect "010" based on manual? Actually let's compute: sequence positions: 0,1,2,0,1,2. Query [2,5] 1-based = elements 2..5 = 1,2,0,1 -> no full 0,1,2 contiguously? 1,2,0 not, 2,0,1 not -> 0. [1,6] all elements -> yes (0,1,2 at 1..3). [3,6] elements 3..6 = 2,0,1,2 -> contains 0,1,2 at 2..4? start at 4? Actually elements positions: 2,0,1,2; starting at second element (index 4 1-based) gives 0,1,2 -> yes. So "101". I'll assert "101".
        assert(res == "101");
    }
    // Edge case: sequence length exactly m
    {
        std::vector<int> cycle = {0, 1, 2};
        std::vector<int> seq = {1, 2, 0}; // positions 1,2,0 -> not 0,1,2
        std::vector<std::pair<int,int>> queries = {{1,3}};
        std::string res = solve(3, cycle, seq, queries);
        assert(res == "0");
    }
    // Large-ish test: cycle 0,1,2, seq 0,1,2,0,1,2, query full range
    {
        std::vector<int> cycle = {0, 1, 2};
        std::vector<int> seq = {0, 1, 2, 0, 1, 2};
        std::vector<std::pair<int,int>> queries = {{1,6}};
        std::string res = solve(3, cycle, seq, queries);
        assert(res == "1");
    }
    return 0;
}
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

// SOLUTION:
// Given a cycle permutation, a sequence, and queries, return '1' if the subarray contains a full cycle traversal in order.
// The function uses binary lifting to find the end position of a valid run starting at each index, and a segment tree to answer range minimum queries.
std::string solve(int m, const std::vector<int>& cycle, const std::vector<int>& seq, const std::vector<std::pair<int,int>>& queries) {
    const int n = (int)seq.size();
    const int q = (int)queries.size();

    // If m > n, no subarray can contain the full cycle.
    if (m > n) {
        return std::string(q, '0');
    }
    // Special case m == 1: any non-empty subarray contains a run.
    if (m == 1) {
        return std::string(q, '1');
    }

    // Map each cycle element to its position.
    std::vector<int> pos(m);
    for (int i = 0; i < m; ++i) {
        pos[cycle[i]] = i;
    }

    // Convert sequence to positions.
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        a[i] = pos[seq[i]];
    }

    // Precompute next occurrence of the next required position.
    // nextPos[i] = smallest j > i such that a[j] == (a[i] + 1) % m, or n if absent.
    const int LOG = 18; // enough for n <= 100000, 2^17 > 100000
    std::vector<std::vector<int>> up(n + 1, std::vector<int>(LOG, n));
    std::vector<int> last(m, n);
    for (int i = n - 1; i >= 0; --i) {
        int need = (a[i] + 1) % m;
        up[i][0] = last[need];
        last[a[i]] = i;
    }
    // Binary lifting.
    for (int k = 1; k < LOG; ++k) {
        for (int i = 0; i < n; ++i) {
            int mid = up[i][k - 1];
            up[i][k] = (mid < n) ? up[mid][k - 1] : n;
        }
    }

    // For each start i, find the end index after m-1 jumps.
    std::vector<int> endPos(n, n);
    for (int i = 0; i < n; ++i) {
        int cur = i;
        int steps = m - 1;
        for (int k = 0; k < LOG; ++k) {
            if ((steps >> k) & 1) {
                cur = up[cur][k];
                if (cur >= n) break;
            }
        }
        endPos[i] = cur; // if cur < n, then run ends at cur (inclusive)
    }

    // Build segment tree for range minimum of endPos.
    int size = 1;
    while (size < n) size <<= 1;
    std::vector<int> seg(2 * size, INT_MAX);
    for (int i = 0; i < n; ++i) {
        seg[size + i] = endPos[i];
    }
    for (int i = size - 1; i > 0; --i) {
        seg[i] = std::min(seg[2 * i], seg[2 * i + 1]);
    }

    auto queryMin = [&](int l, int r) {
        // 0-based inclusive l, exclusive r
        if (l >= r) return INT_MAX;
        l += size;
        r += size;
        int res = INT_MAX;
        while (l < r) {
            if (l & 1) res = std::min(res, seg[l++]);
            if (r & 1) res = std::min(res, seg[--r]);
            l >>= 1;
            r >>= 1;
        }
        return res;
    };

    std::string ans(q, '0');
    for (int qi = 0; qi < q; ++qi) {
        int l = queries[qi].first - 1; // convert to 0-based
        int r = queries[qi].second;    // exclusive
        // Need a start i in [l, r-m] (since run length m, end index <= r-1)
        int left = l;
        int right = r - m; // exclusive bound for i: i <= r-m
        if (right >= left) {
            int minEnd = queryMin(left, right + 1);
            if (minEnd < r) {
                ans[qi] = '1';
            }
        }
    }
    return ans;
}
// The problem is similar to checking whether a sliding window of length `m` contains a complete increasing run of cycle positions. First, map each sequence element to its 0-based position in the cycle (`pos[x]`). Because the required run must be exactly `0,1,2,...,m-1` (in that order, with no gaps and no repeats, and must appear contiguously), we can precompute for each starting index `i` in the sequence the smallest index `j` such that the subarray `seq[i..j]` contains a valid run ending at `j`. This can be done using a "next occurrence" jump table: for each position `p` in the sequence, we compute the next index where the next required cycle position appears. Specifically, for each `i`, we define `next[i]` as the smallest index `> i` where the element's position equals `(pos[seq[i]] + 1) % m`. If no such index exists, set it to `n` (sentinel). Then, to check if starting at `i` we can complete the full cycle of `m` steps, we need to jump `m-1` times using these `next` pointers (since starting at `i` already gives the first element). Use binary lifting (sparse table on `next` jumps) to compute, for each `i`, the index `end[i]` = the position after `m-1` jumps (i.e., the last position of the complete run). If `end[i] < n` (meaning the run exists) then any query `(l, r)` with `l <= i` and `r >= end[i]` will contain this run. To answer queries offline, for each `i` compute `end[i]` and note that the run is valid for any `l` in `[1, i]` and any `r` in `[end[i], n]`. We can process queries by sorting by `r`, and using a Fenwick tree or segment tree to maintain the maximum `end` for all starting positions `i` with `i >= l`? Actually simpler: for each query `(l, r)`, we need to check if there exists `i` in `[l, r-m+1]` such that `end[i] <= r`. This can be answered by building a segment tree over `i` that stores the minimum `end[i]` for each segment, then for each query, query the minimum `end` over `i` in `[l, r-m+1]` and compare to `r`. If the minimum is `<= r`, then there exists a valid run fully contained in `[l, r]`. Edge cases: when `m == 1`, any single-element subarray contains a trivial run, so answer is always `'1'` for any `l <= r`. When `m > n`, no subarray can contain the full cycle, so all answers are `'0'`. The algorithm complexity: precomputing `next` and binary lifting takes `O(n log n)` time and space for the binary lifting table (size `n * LOG`). Building the segment tree takes `O(n)` time. Processing each query takes `O(log n)` time via segment tree. Total `O((n + q) log n)` time and `O(n log n)` space, which fits constraints.
