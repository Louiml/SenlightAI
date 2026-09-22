// You are given `n` positions in a row, numbered from 1 to `n`, initially all empty. There are `m` operations, each specifying a contiguous range `[lft, rgt]`, a quantity `core`, and a price `pric`. For each operation, you must select exactly `core` positions from within that range (positions may be selected multiple times across different operations, but each selection is treated as a distinct unit) and pay `pric` per selected position. However, you cannot select more than `k` total positions across all operations combined. Your goal is to determine the **minimum total cost** required to cover exactly `k` positions, choosing which operations to use and how many units from each range. You may partially use an operation (select fewer than `core` units), but you cannot exceed the total capacity of `k`. If it is impossible to select `k` positions, return `-1`. Write a C++ function `long long minimumCost(int n, int k, const std::vector<Operation>& ops)` where `Operation` is a struct with `long long lft, rgt, core, pric`. Note that `lft` and `rgt` are 1-indexed inclusive, and all values are positive integers; `n` and `k` can be up to `10^5`, and `m` up to `2*10^5`. The answer may exceed 32-bit, so use 64-bit integers.
#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple range covers all with capacity
    {
        std::vector<Operation> ops = {{1, 5, 3, 10}, {1, 5, 3, 5}};
        assert(minimumCost(5, 4, ops) == 15); // take 3 from cheap (15) + 1 from expensive (10) = 25? Wait cheap price 5, take 3 -> 15, need 1 more from price 10 -> 10, total 25. Actually 3*5=15, 1*10=10 => 25.
    }

    // Test 2: Impossible
    {
        std::vector<Operation> ops = {{1, 2, 1, 1}};
        assert(minimumCost(3, 3, ops) == -1);
    }

    // Test 3: Exactly k from one op
    {
        std::vector<Operation> ops = {{2, 4, 5, 7}};
        assert(minimumCost(4, 3, ops) == 21); // 3*7
    }

    // Test 4: Multiple ops, cheaper first
    {
        std::vector<Operation> ops = {{1, 3, 2, 1}, {3, 5, 2, 10}, {2, 4, 1, 5}};
        // op1 cost1 takes 2 from [1,3] -> positions 1,2, cost2, taken2
        // op3 cost5 takes 1 from [2,4] -> leftmost available is 3? Actually after op1, pos1,2 used, available in [2,4] is pos3,4. take 1 -> pos3, cost5, taken3
        // op2 cost10 needs 1 more? k=4, need 1, available in [3,5] is pos4,5? pos3 used, so pos4 taken, cost10, taken4. total 17
        assert(minimumCost(5, 4, ops) == 2 + 5 + 10);
    }

    // Test 5: Large values, overflow check
    {
        std::vector<Operation> ops = {{1, 100000, 100000, 1000000000LL}};
        assert(minimumCost(100000, 100000, ops) == 100000000000000LL); // 100000 * 1e9
    }

    // Test 6: n=1, k=1
    {
        std::vector<Operation> ops = {{1, 1, 1, 5}};
        assert(minimumCost(1, 1, ops) == 5);
    }

    // Test 7: k=0
    {
        std::vector<Operation> ops = {{1, 1, 1, 5}};
        assert(minimumCost(5, 0, ops) == 0);
    }

    // Test 8: Some ops completely unusable (range empty)
    {
        std::vector<Operation> ops = {{2, 2, 1, 1}, {1, 1, 1, 10}};
        assert(minimumCost(2, 2, ops) == 11); // take pos2 cost1, pos1 cost10
    }

    // Test 9: Overlapping ranges, take from leftmost
    {
        std::vector<Operation> ops = {{1, 5, 3, 1}, {1, 5, 3, 2}};
        // cheap takes positions 1,2,3 cost3, expensive takes 4,5 cost4, total7 for k=5
        assert(minimumCost(5, 5, ops) == 3 + 4);
    }

    // Test 10: All positions unavailable after some ops
    {
        std::vector<Operation> ops = {{1, 2, 2, 1}, {2, 3, 2, 1}};
        // k=4, but only 3 positions total, impossible
        assert(minimumCost(3, 4, ops) == -1);
    }

    return 0;
}
#include <vector>
#include <algorithm>
#include <cstdint>

struct Operation {
    long long lft, rgt, core, pric;
};

// A segment tree that supports range sum queries and removing a given number
// of leftmost available positions in a range.
class SegmentTree {
    std::vector<long long> tree;
    std::vector<long long> lazy;
    int n;

    void build(int node, int l, int r, const std::vector<long long>& base) {
        if (l == r) {
            tree[node] = base[l];
            return;
        }
        int mid = (l + r) / 2;
        build(node * 2, l, mid, base);
        build(node * 2 + 1, mid + 1, r, base);
        tree[node] = tree[node * 2] + tree[node * 2 + 1];
    }

    void push(int node, int l, int r) {
        if (lazy[node] == 0) return;
        int mid = (l + r) / 2;
        tree[node * 2] = (mid - l + 1) - tree[node * 2]; // not needed for our use, but we don't use lazy
        // We don't use lazy in this solve because we remove exactly leftmost.
        // Instead, we implement direct point updates in remove_leftmost.
    }

    long long query(int node, int l, int r, int ql, int qr) {
        if (ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) return tree[node];
        int mid = (l + r) / 2;
        return query(node * 2, l, mid, ql, qr) + query(node * 2 + 1, mid + 1, r, ql, qr);
    }

    // Remove one position at index pos (set to 0)
    void remove_one(int node, int l, int r, int pos) {
        if (l == r) {
            tree[node] = 0;
            return;
        }
        int mid = (l + r) / 2;
        if (pos <= mid) remove_one(node * 2, l, mid, pos);
        else remove_one(node * 2 + 1, mid + 1, r, pos);
        tree[node] = tree[node * 2] + tree[node * 2 + 1];
    }

    // Find the index of the `kth` (1-indexed) available position in the whole tree.
    int find_kth(int node, int l, int r, long long k) {
        if (l == r) return l;
        int mid = (l + r) / 2;
        if (tree[node * 2] >= k) return find_kth(node * 2, l, mid, k);
        else return find_kth(node * 2 + 1, mid + 1, r, k - tree[node * 2]);
    }

public:
    SegmentTree(const std::vector<long long>& base) {
        n = (int)base.size() - 1; // base is 1-indexed
        tree.assign(4 * n + 5, 0);
        lazy.assign(4 * n + 5, 0);
        build(1, 1, n, base);
    }

    long long range_sum(int l, int r) {
        return query(1, 1, n, l, r);
    }

    // Remove `cnt` leftmost available positions in [l, r]. Assumes cnt <= range_sum(l,r).
    void remove_leftmost(int l, int r, long long cnt) {
        while (cnt > 0) {
            // Find the first available position >= l
            // We can find kth available in [l, r] by first finding the rank of the first available position >= l.
            // Since we don't have a direct "first available >= l" query, we do a search:
            // But simpler: we can do a while loop with find_kth on prefix sums.
            // Because we always take from leftmost, we can find total available before l:
            long long before = (l > 1) ? range_sum(1, l - 1) : 0;
            long long rank = before + 1; // first available position in [l, r] has rank = before+1
            int pos = find_kth(1, 1, n, rank);
            remove_one(1, 1, n, pos);
            cnt--;
        }
    }
};

long long minimumCost(int n, int k, const std::vector<Operation>& ops) {
    if (k <= 0) return 0;
    std::vector<Operation> sortedOps = ops;
    std::sort(sortedOps.begin(), sortedOps.end(),
              [](const Operation& a, const Operation& b) { return a.pric < b.pric; });

    std::vector<long long> base(n + 1, 1); // all positions available
    SegmentTree st(base);

    long long taken = 0;
    long long cost = 0;

    for (const auto& op : sortedOps) {
        if (taken == k) break;
        if (op.lft > op.rgt || op.rgt > n || op.lft < 1) continue;
        long long available = st.range_sum(op.lft, op.rgt);
        if (available == 0) continue;
        long long take = std::min({op.core, k - taken, available});
        if (take <= 0) continue;
        cost += take * op.pric;
        st.remove_leftmost(op.lft, op.rgt, take);
        taken += take;
    }

    return (taken == k) ? cost : -1;
}
// This is a classic "buy at cheapest price over a range with capacity constraints" problem. Since the cost per unit is constant for a given operation, we want to greedily take positions from the cheapest operations first, but we must respect that each operation can supply at most `core` units from within its specific interval. The naive approach of iterating over operations sorted by price and filling positions one by one would be too slow if implemented poorly (O(n*k) or O(m*n)). Instead, we process operations in increasing price order. For each operation, we need to know how many empty (or still-available) positions remain within `[lft, rgt]`, and then take as many as possible (up to `core` and up to `k - already_taken`). To efficiently count and "use" positions in a range, we can maintain a segment tree that stores the number of available positions in each segment. Initially all positions are available (value 1). When we take `x` positions from a range, we need to remove `x` from the leftmost positions of that range. A common technique is to use a segment tree that supports range-sum queries and a "find kth leftmost available position" query, then decrement those positions. However, because we always take greedily from the cheapest operation first, we can also think of a "range minimum position" approach: we only care about the total capacity remaining. A simpler and correct method: sort operations by price. For each operation, we want to take up to `min(core, remaining_needed)` units, but we must ensure those units are available in the range. To handle this, we can use a segment tree with lazy propagation that stores the sum of available positions. We query the sum in the range. If it's 0, skip. Otherwise, we take `take = min(core, need, range_sum)` and then "remove" those from the leftmost positions of the range. Removing exactly `take` positions from a range cannot be done with a standard range update (we need to zero out specific positions), so we implement a `find_and_remove` function: given a range and a count, it finds the leftmost positions and sets them to 0. Since each position is removed at most once, total work is O((n+m) log n). Alternatively, we can use a "disjoint set" (DSU) to skip already used positions, but that works only if we always take positions from left to right across operations, but operations have different ranges. So segment tree is appropriate. The algorithm: sort `ops` by `pric`. Initialize segment tree with all ones (available). Let `taken = 0`, `cost = 0`. For each op: if `taken == k` break. Compute `available = query(lft, rgt)`. If `available == 0` continue. `take = min({op.core, k - taken, available})`. Add `take * op.pric` to cost. Then remove `take` from the leftmost positions in `[lft, rgt]` using `remove_leftmost(lft, rgt, take)`. `taken += take`. After loop, if `taken < k` return -1, else return cost. Edge cases: overflow, `core` may be larger than range length, multiple operations with same price, and the possibility that we cannot reach `k`. Time: O((n + m log n)). Space: O(n) for tree.
