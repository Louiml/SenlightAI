/*
Given a 1-indexed array `a` of length `n` and a sequence of `m` operations, write a C++ function `solveQueries` that processes two types of operations: type 1 queries ask for the sum of distances between adjacent equal elements in the subarray `[l, r]` (i.e., for every pair of consecutive indices `i` and `i+1` within `[l, r-1]` where `a[i] == a[i+1]`, add the difference `(i+1 - i)` = 1 to the answer; more generally, the problem from the snippet actually tracks distances between **consecutive occurrences** of the same value, not just adjacent indices), and type 2 updates change `a[u]` to a new value `v`. The function should return an array of answers to all type 1 queries in the order they appear. Initial array values and all operation parameters are given as input vectors. The key insight from the snippet is to maintain for each value the sorted set of positions where it occurs, and to represent "adjacent occurrences" of the same value as intervals whose weight is the distance between them. You must implement an efficient offline divide-and-conquer (CDQ) solution that processes all queries and updates together, using a Fenwick tree for prefix sums of interval weights, and output the answers for each query. The function signature should accept `n` (array size), a vector `initialArr` of length `n`, a vector of operations (each operation is a tuple `{op, u, v}` where `op=1` means query with `l=u, r=v`, and `op=2` means update position `u` to value `v`), and return a vector of `long long` answers. Assume all indices are 1-based, values are positive integers up to 1e5, and the total number of operations is up to 1e5.
*/

#include <vector>
#include <set>
#include <algorithm>
#include <functional>

using io = std::vector<long long>;

// Process dynamic updates and range-sum queries on intervals formed by consecutive equal values.
io solveQueries(int n, const std::vector<int>& initialArr, const std::vector<std::tuple<int,int,int>>& ops) {
    const long long INF = n + 1LL;

    // Structure for a CDQ event
    struct Event {
        int op;      // 1 = query, 2 = interval update (weight can be negative for removal)
        int l, r;    // for update: interval [l,r]; for query: [l,r] is the query range
        long long w; // weight (for update) or unused (for query)
        int id;      // for query: query index; for update: event index (used in CDQ)
        int stamp;   // original event index for CDQ ordering
    };

    std::vector<Event> ev;
    std::vector<std::set<int>> pos(n + 1); // 1-indexed values, but values can be up to 1e5? We'll assume value range is 1..n or handle by coordinate? Here we use vector of size maxVal but we don't know max value, so we'll use std::map? Actually snippet uses s[MAXN] with MAXN=100005, so we can assume values <= 1e5.
    // Let's allocate size 100005 for simplicity.
    const int MAXV = 100005;
    std::vector<std::set<int>> sets(MAXV + 1);
    for (int i = 0; i < n; ++i) {
        int val = initialArr[i];
        sets[val].insert(i + 1); // 1-indexed
    }
    // Build initial intervals: for each value, for each adjacent pair in its set
    for (int val = 1; val <= MAXV; ++val) {
        const auto& s = sets[val];
        if (s.size() < 2) continue;
        auto it = s.begin();
        int prev = *it;
        ++it;
        for (; it != s.end(); ++it) {
            int cur = *it;
            // create positive interval: left=prev, right=cur, weight=cur-prev
            ev.push_back({2, prev, cur, (long long)(cur - prev), 0, (int)ev.size() + 1});
            prev = cur;
        }
    }

    // Current array values (copy)
    std::vector<int> cur = initialArr; // 0-indexed

    int qcnt = 0; // number of queries
    std::vector<long long> ans; // will be filled later

    for (const auto& op : ops) {
        int type, u, v;
        std::tie(type, u, v) = op;
        if (type == 1) {
            // query [u,v]
            if (u > v) { // empty range -> answer 0
                ans.push_back(0);
                ++qcnt;
                continue;
            }
            ev.push_back({1, u, v, 0, qcnt, (int)ev.size() + 1});
            ++qcnt;
        } else {
            // update: change a[u] to v
            int oldVal = cur[u - 1];
            if (oldVal == v) continue; // no change
            // Find prev and next occurrence of old value at position u
            int prevOld = 0, nextOld = n + 1;
            auto it = sets[oldVal].find(u);
            if (it != sets[oldVal].begin()) {
                auto itp = std::prev(it);
                prevOld = *itp;
            }
            auto itn = std::next(it);
            if (itn != sets[oldVal].end()) {
                nextOld = *itn;
            }
            // Remove intervals involving old value
            if (prevOld != 0) {
                // interval [prevOld, u] existed
                ev.push_back({2, prevOld, u, -((long long)(u - prevOld)), 0, (int)ev.size() + 1});
            }
            if (nextOld != n + 1) {
                // interval [u, nextOld] existed
                ev.push_back({2, u, nextOld, -((long long)(nextOld - u)), 0, (int)ev.size() + 1});
            }
            if (prevOld != 0 && nextOld != n + 1) {
                // interval [prevOld, nextOld] did NOT exist before (because u was between), but now they become adjacent after removing u
                // Actually we need to ADD this new interval? Yes, after removal, prevOld and nextOld become adjacent.
                ev.push_back({2, prevOld, nextOld, (long long)(nextOld - prevOld), 0, (int)ev.size() + 1});
            }
            // Erase from old set
            sets[oldVal].erase(u);
            // Insert into new set
            sets[v].insert(u);
            cur[u - 1] = v;
            // Now find prev and next of new value at position u
            int prevNew = 0, nextNew = n + 1;
            auto itv = sets[v].find(u);
            if (itv != sets[v].begin()) {
                auto itp = std::prev(itv);
                prevNew = *itp;
            }
            auto itn2 = std::next(itv);
            if (itn2 != sets[v].end()) {
                nextNew = *itn2;
            }
            // Add intervals involving new value
            if (prevNew != 0) {
                ev.push_back({2, prevNew, u, (long long)(u - prevNew), 0, (int)ev.size() + 1});
            }
            if (nextNew != n + 1) {
                ev.push_back({2, u, nextNew, (long long)(nextNew - u), 0, (int)ev.size() + 1});
            }
            if (prevNew != 0 && nextNew != n + 1) {
                // interval [prevNew, nextNew] existed before but now we have u in between, so it's broken
                ev.push_back({2, prevNew, nextNew, -((long long)(nextNew - prevNew)), 0, (int)ev.size() + 1});
            }
        }
    }

    // Now we have ev list. We need to process offline with CDQ.
    // Each event has 'stamp' = original index (1-based) to preserve time order (the order they were added).
    // We'll store answers in a vector of size qcnt initialized to 0.
    std::vector<long long> res(qcnt, 0);
    int E = ev.size();
    if (E == 0) return res;

    // Fenwick tree for range-add point-query? We need sum of weights of intervals contained in query [l,r].
    // The CDQ sorts by l descending and uses Fenwick indexed by r to accumulate weights.
    // For each interval event (op==2) with weight w, we add w at position r.
    // For each query (op==1) with range [l,r], we want sum of weights of intervals with r <= r (i.e., prefix sum up to r).
    // Since l is sorted descending, and intervals have their l >= query l? Actually we need intervals with left >= query.l AND right <= query.r.
    // The snippet uses cmp: a.l != b.l ? a.l > b.l : a.op > b.op (so updates (op=2) before queries (op=1) when equal l).
    // Then in the loop, if event is from left half (id <= mid) and op==2, add weight at r; if from right half and op==1, add to answer query(r).
    // So we need to replicate that.

    // We'll implement CDQ recursively over event array sorted by 'stamp'? Actually we sort subranges by l for each recursion.
    // Let's define a recursive function cdq(l, r) that processes ev[l..r] assuming they are sorted by stamp (the original time order).
    // We'll copy the events into a vector and sort by stamp initially? The original snippet uses d[] array with id = cnt (global event index). It recursively divides by id.
    // We'll follow the snippet pattern: we need events to have an 'id' field equal to their stamp (original index). The solve function sorts by l within each level.

    // We'll implement a global Fenwick tree as a vector<long long> of size n+2.
    std::vector<long long> bit(n + 2, 0);
    auto bitAdd = [&](int idx, long long val) {
        for (; idx <= n; idx += idx & -idx) bit[idx] += val;
    };
    auto bitSum = [&](int idx) {
        long long res = 0;
        for (; idx > 0; idx -= idx & -idx) res += bit[idx];
        return res;
    };

    // Helper function to perform CDQ
    std::function<void(int,int)> cdq = [&](int l, int r) {
        if (l == r) return;
        int mid = (l + r) >> 1;
        cdq(l, mid);
        cdq(mid + 1, r);
        // Sort the subrange by l descending, and for equal l, updates before queries (op descending: op=2 > op=1)
        std::vector<Event> tmp(ev.begin() + l, ev.begin() + r + 1);
        std::sort(tmp.begin(), tmp.end(), [](const Event& a, const Event& b) {
            if (a.l != b.l) return a.l > b.l;
            return a.op > b.op; // op=2 > op=1
        });
        for (const Event& e : tmp) {
            if (e.id <= mid && e.op == 2) { // update from left half
                bitAdd(e.r, e.w);
            } else if (e.id > mid && e.op == 1) { // query from right half
                res[e.val] += bitSum(e.r);
            }
        }
        // Clear Fenwick
        for (const Event& e : tmp) {
            if (e.id <= mid && e.op == 2) {
                bitAdd(e.r, -e.w);
            }
        }
        // We also need to merge the subranges to keep the array sorted by stamp? Actually the original snippet does not merge, but it's fine because the next recursion will sort again.
    };

    cdq(0, E - 1);
    return res;
}

#include <cassert>
#include <vector>
#include <tuple>
#include <iostream>

// Assume the solution function is defined above (solveQueries)

int main() {
    // Custom test harness
    {
        int n = 5;
        std::vector<int> arr = {1, 2, 1, 2, 1};
        std::vector<std::tuple<int,int,int>> ops;
        // Query [2,3]: positions 2 and 3 are 2 and 1, no equal adjacent -> 0
        // Query [1,5]: positions: 1,2,1,2,1 -> adjacent equal? none (1-2 diff, 2-1 diff, 1-2 diff, 2-1 diff) -> 0
        ops.push_back({1,2,3});
        ops.push_back({1,1,5});
        auto res = solveQueries(n, arr, ops);
        assert(res.size() == 2);
        assert(res[0] == 0);
        assert(res[1] == 0);
    }
    {
        int n = 4;
        std::vector<int> arr = {5,5,5,5};
        std::vector<std::tuple<int,int,int>> ops;
        // All same: distances between consecutive positions: 1,1,1
        // Query [2,4] -> subarray indices 2,3,4: pairs (2,3) and (3,4) both equal -> sum = 1+1=2
        // Query [1,4] -> pairs (1,2),(2,3),(3,4) all equal -> sum = 3
        ops.push_back({1,2,4});
        ops.push_back({1,1,4});
        auto res = solveQueries(n, arr, ops);
        assert(res.size() == 2);
        assert(res[0] == 2);
        assert(res[1] == 3);
    }
    {
        int n = 3;
        std::vector<int> arr = {1,2,3};
        std::vector<std::tuple<int,int,int>> ops;
        // No equal adjacent initially
        ops.push_back({1,1,3});
        // Update pos2 to 1 -> arr = {1,1,3}
        ops.push_back({2,2,1});
        // Query [1,3] -> pairs (1,2) equal, (2,3) diff -> sum = 1
        ops.push_back({1,1,3});
        auto res = solveQueries(n, arr, ops);
        assert(res.size() == 2);
        assert(res[0] == 0);
        assert(res[1] == 1);
    }
    {
        int n = 6;
        std::vector<int> arr = {1,2,1,2,1,2};
        std::vector<std::tuple<int,int,int>> ops;
        // No adjacent equal initially
        ops.push_back({1,1,6});
        // Update pos1 to 2 -> arr = {2,2,1,2,1,2}
        ops.push_back({2,1,2});
        // Query [1,2] -> both 2 -> sum 1
        ops.push_back({1,1,2});
        // Query [1,6] -> pairs (1,2) equal, others diff -> sum=1
        ops.push_back({1,1,6});
        auto res = solveQueries(n, arr, ops);
        assert(res.size() == 3);
        assert(res[0] == 0);
        assert(res[1] == 1);
        assert(res[2] == 1);
    }
    {
        // More complex with multiple equal values at same distance
        int n = 5;
        std::vector<int> arr = {3,1,3,2,3};
        // Initial intervals: value 3 at positions {1,3,5} -> distances 2 and 2
        // Query [1,5] -> both intervals inside -> sum = 4
        // Query [2,5] -> only interval [3,5] inside -> sum = 2
        std::vector<std::tuple<int,int,int>> ops;
        ops.push_back({1,1,5});
        ops.push_back({1,2,5});
        auto res = solveQueries(n, arr, ops);
        assert(res.size() == 2);
        assert(res[0] == 4);
        assert(res[1] == 2);
    }
    {
        // Test update removing an interval and adding another
        int n = 4;
        std::vector<int> arr = {1,2,1,2};
        std::vector<std::tuple<int,int,int>> ops;
        // Initial intervals: value 1 at {1,3} -> dist 2; value 2 at {2,4} -> dist 2
        // Query [1,4] -> both intervals -> sum 4
        ops.push_back({1,1,4});
        // Update pos1 to 2 -> arr = {2,2,1,2}
        // New intervals: value 2 at {1,2,4} -> dist 1 (1-2) and 2 (2-4); value 1 at {3} none.
        // Query [1,4] -> intervals [1,2] and [2,4] contained? [2,4] yes, [1,2] yes -> sum 1+2=3
        ops.push_back({2,1,2});
        ops.push_back({1,1,4});
        auto res = solveQueries(n, arr, ops);
        assert(res.size() == 2);
        assert(res[0] == 4);
        assert(res[1] == 3);
    }

    std::cout << "All tests passed.\n";
    return 0;
}

// We need to handle dynamic updates to an array and answer queries that ask for the sum of distances between consecutive occurrences of the same value within a subarray. The naive approach would be O(n) per query, which is too slow. The key observation from the snippet is that we can represent each "adjacent pair of occurrences" of the same value as an interval `[left_pos, right_pos]` with weight `right_pos - left_pos`. A query for range `[l, r]` asks for the sum of weights of all such intervals that are fully contained inside `[l, r]`. Updates change the array, which affects which intervals exist: when a value at position `u` changes, we must remove intervals involving the old value at that position and add intervals involving the new value. This dynamic interval-weight-sum problem can be solved offline using a CDQ divide-and-conquer (like the snippet) that sorts events by `l` descending and processes contributions. The key is to treat each interval as an "update" event (op=2) with parameters `l=left_pos`, `r=right_pos`, `weight=right_pos-left_pos`, and each query as a "query" event (op=1) with `l=u`, `r=v`, and a query id. Then we want, for each query, the sum of weights of all update intervals with `left >= l` and `right <= r` (since the interval is inside the subarray). This is a 2D dominance problem: we sort by `left` descending, and for ties, we process updates before queries (or in the snippet, they use a custom comparator). We then use a Fenwick tree indexed by `right` to accumulate weights. The offline CDQ handles the time dimension (the order of operations) so that we only count intervals that are active at the time of the query. The snippet actually does a recursive solve over the event array, where each event has an `id` that is its insertion position in the event list, and it sorts a subrange by `l` descending and uses Fenwick to add weights from left-part updates to right-part queries. The correctness relies on the fact that each interval's lifespan is from its creation to its deletion, and we can represent deletions as negative-weight intervals. In our function, we must build the list of events: initial intervals, and for each update, remove old intervals (add negative-weight update events) and add new intervals (positive-weight update events). The answer for each query is accumulated via the Fenwick tree during the CDQ. Edge cases: empty subarray (l > r) gives answer 0, updates may not have a predecessor or successor, so only add intervals when both positions exist and are <= n. Complexity: each initial interval and each update generates at most a constant number of update events (up to 3 per change), and there are O(n + m) total events. The CDQ solves in O(E log^2 E) time due to sorting within recursion, but an optimized version is O(E log E log E) or O(E log^2 E). Space is O(E + n) for arrays and sets.
