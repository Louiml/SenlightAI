/*
Given an integer array `a` of length `n` (1-indexed) whose values are a permutation of `1..n`, and an integer `k` with `1 <= k <= n`, consider the following iterative process: for each position `i` from `1` to `n`, an interval `[l[i], r[i]]` is defined by the subtree of `i` in a Cartesian tree built from `a` (where the parent of each element `i` is the nearest element to its right with a larger value; the root of the tree is the sentinel element `n+1`). For each `i`, take the set of elements among the first `i` (after applying a sliding window of size `k` that removes elements that are more than `k` positions behind) and compute the maximum depth (in terms of the number of active tree nodes covering a point) over the range `1..n`. More precisely, for each `i`, add a +1 over the interval `[l[i], r[i]]` to a segment tree; if `i > k`, remove the contribution of element `i-k` by applying a -1 over `[l[i-k], r[i-k]]`. The value for step `i` (for all `i` where `i >= k`; output it for each such `i` in order) is the maximum value over the entire range `[1,n]` of the segment tree. Write a function `vector<int> slidingCartesianDepths(const vector<int>& a, int k)` that returns this sequence of outputs for `i = k, k+1, ..., n`. The input `a` is 0-indexed in the function, but the underlying algorithm uses 1-indexing internally.
*/

#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>

// Segment tree with lazy propagation for range add and range max.
class LazySegmentTree {
private:
    int n;
    std::vector<int> tree, lazy;

    void apply(int node, int val) {
        tree[node] += val;
        lazy[node] += val;
    }

    void push(int node) {
        if (lazy[node] != 0) {
            apply(node * 2, lazy[node]);
            apply(node * 2 + 1, lazy[node]);
            lazy[node] = 0;
        }
    }

    void updateRange(int node, int l, int r, int ql, int qr, int val) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply(node, val);
            return;
        }
        push(node);
        int mid = (l + r) / 2;
        updateRange(node * 2, l, mid, ql, qr, val);
        updateRange(node * 2 + 1, mid + 1, r, ql, qr, val);
        tree[node] = std::max(tree[node * 2], tree[node * 2 + 1]);
    }

    int queryMax(int node, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return tree[node];
        push(node);
        int mid = (l + r) / 2;
        return std::max(queryMax(node * 2, l, mid, ql, qr),
                        queryMax(node * 2 + 1, mid + 1, r, ql, qr));
    }

public:
    explicit LazySegmentTree(int size) : n(size), tree(4 * size, 0), lazy(4 * size, 0) {}

    void update(int l, int r, int val) {
        if (l > r) return;
        updateRange(1, 0, n - 1, l, r, val);
    }

    int query(int l, int r) {
        return queryMax(1, 0, n - 1, l, r);
    }
};

// Class to encapsulate the Cartesian tree and the sliding window depths.
class CartesianTreeDepthCalculator {
private:
    int n, k;
    std::vector<int> parent;
    std::vector<std::vector<int>> children;
    std::vector<int> left, right;
    int timer;

    void dfs(int node) {
        left[node] = timer++;
        for (int child : children[node]) {
            dfs(child);
        }
        right[node] = timer - 1;
    }

public:
    CartesianTreeDepthCalculator(const std::vector<int>& a, int kk) : n(static_cast<int>(a.size())), k(kk) {
        // Convert to 1-indexed internally: value at position i (1-based) is a[i-1].
        // We add sentinel value n+1 at position n+1.
        const int sentinelPos = n + 1;
        parent.assign(n + 2, sentinelPos);
        children.assign(n + 2, {});
        left.assign(n + 2, 0);
        right.assign(n + 2, 0);

        // Fenwick tree that stores minimum position seen for values up to n.
        // For Fenwick we need to query minimum over suffix, so we use a reversed index.
        std::vector<int> bit(n + 2, n + 1);
        auto updateBit = [&](int idx, int val) {
            // Bit index is 1..n+1, we store minimum position for value idx.
            while (idx <= n + 1) {
                bit[idx] = std::min(bit[idx], val);
                idx += idx & -idx;
            }
        };
        auto queryBit = [&](int idx) {
            int res = n + 1;
            while (idx >= 1) {
                res = std::min(res, bit[idx]);
                idx -= idx & -idx;
            }
            return res;
        };

        // Build the Cartesian tree by scanning from right to left.
        for (int i = n; i >= 1; --i) {
            int val = a[i - 1];
            // Find nearest position to the right with value > val.
            // Our Fenwick stores minimum position for each value; we query values > val.
            // Since val is between 1 and n, we query with idx = n+1 - val to flip order.
            int idx = n + 1 - val;
            int pos = queryBit(n + 1 - (val + 1)); // Equivalent to query for values > val.
            // Actually our queryBit takes a reversed index; we need correct mapping.
            // Let's implement a simpler version: we need minimum position among values > val.
            // Use a Fenwick that supports prefix min query on reversed indices? Better to use a segment tree for clarity, but we can use a Fenwick with reversed indexing carefully.
            // Since this is a teaching solution, we'll implement a clean version with a Fenwick that stores min for value v at index n+1-v.
            // For querying values > val, we need values in [val+1, n], which correspond to reversed indices [1, n - val].
            // Fenwick can't do suffix min easily, so we reverse the array indexing: index = n+1 - value.
            // Then values > val have reversed index < n+1 - val. So we query prefix up to n - val.
            // Our bit size is n+1, and we update at position (n+1 - value).
            // Let's correct the Fenwick implementation in the final code.
            // For now, we'll do it properly below.
        }
    }
};

// Main solution function.
std::vector<int> slidingCartesianDepths(const std::vector<int>& a, int k) {
    int n = static_cast<int>(a.size());
    const int sentinel = n + 1;

    // Build Cartesian tree using Fenwick tree for nearest greater to right.
    std::vector<int> parent(n + 2, sentinel);
    std::vector<std::vector<int>> children(n + 2);
    std::vector<int> bit(n + 2, n + 1);
    auto bitUpdate = [&](int idx, int val) {
        // idx is 1..n+1, we store min position at value idx.
        while (idx <= n + 1) {
            bit[idx] = std::min(bit[idx], val);
            idx += idx & -idx;
        }
    };
    auto bitQuery = [&](int idx) {
        int res = n + 1;
        while (idx >= 1) {
            res = std::min(res, bit[idx]);
            idx -= idx & -idx;
        }
        return res;
    };

    for (int i = n; i >= 1; --i) {
        int val = a[i - 1];
        // We need minimum position among values > val.
        // Store value v at index n+1 - v. Then values > val are indices <= n - val.
        int idx = n + 1 - val;
        int pos = bitQuery(n + 1 - (val + 1)); // Since (val+1) maps to n - val.
        // But careful: we need all values from val+1 to n, which map to indices 1..n - val.
        // So query up to n - val.
        pos = bitQuery(n - val);
        parent[i] = pos; // pos is n+1 if no greater
        children[pos].push_back(i);
        bitUpdate(idx, i);
    }

    // DFS to compute l and r (Euler tour intervals).
    std::vector<int> left(n + 2), right(n + 2);
    int timer = 0;
    std::function<void(int)> dfs = [&](int node) {
        left[node] = timer++;
        for (int child : children[node]) {
            dfs(child);
        }
        right[node] = timer - 1;
    };
    dfs(sentinel);

    LazySegmentTree seg(n);
    std::vector<int> result;
    for (int i = 1; i <= n; ++i) {
        seg.update(left[i], right[i], 1);
        if (i - k >= 1) {
            int rem = i - k;
            seg.update(left[rem], right[rem], -1);
        }
        if (i - k >= 0) {
            result.push_back(seg.query(0, n - 1));
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// The solution function is declared above; we include it here for testing.

int main() {
    // Test 1: Single element
    std::vector<int> a1 = {1};
    std::vector<int> r1 = slidingCartesianDepths(a1, 1);
    assert(r1.size() == 1 && r1[0] == 1);

    // Test 2: Two elements, window size 1
    std::vector<int> a2 = {1, 2};
    std::vector<int> r2 = slidingCartesianDepths(a2, 1);
    assert(r2.size() == 2);
    assert(r2[0] == 1); // after adding 1: depth 1
    assert(r2[1] == 1); // after adding 2 and removing 1: depth 1

    // Test 3: Two elements, window size 2
    std::vector<int> r3 = slidingCartesianDepths(a2, 2);
    assert(r3.size() == 1);
    assert(r3[0] == 2); // both active, tree: 2 is parent of 1, root covers both, max depth = 2

    // Test 4: Larger permutation
    std::vector<int> a4 = {3, 1, 2};
    // Build Cartesian tree: 3 is root, 1 and 2 are children? Actually parent of 1 is 3? Let's compute:
    // i=3 val=2: nearest greater right? none -> parent sentinel. 
    // i=2 val=1: nearest greater right is position 3 (val 2) -> parent=3.
    // i=1 val=3: parent sentinel.
    // Tree: sentinel -> 3, and sentinel -> 1? Wait, both 1 and 3 have no greater to right -> parent sentinel. Children of sentinel: {1,3}, and 1 has child 2? No, 2's parent is 3, so children: 3->{2}, 1->{}.
    // DFS order: sentinel children: 1 (left=0), then 3 (left=1), then 2 (left=2). So l[1]=0,r[1]=0; l[3]=1,r[3]=2; l[2]=2,r[2]=2.
    // For k=1:
    // i=1: add [0,0] -> max=1, output 1
    // i=2: add [2,2], remove [0,0] -> max=1, output 1
    // i=3: add [1,2], remove [2,2] -> active: [1,2] and [0,0]? Actually after i=3, active are i=2 and i=3? Wait remove i=1, keep i=2 and i=3 -> intervals [2,2] and [1,2] -> max depth =2? Let's see: point 2 is covered by both, so max=2. Output 2.
    std::vector<int> r4 = slidingCartesianDepths(a4, 1);
    assert(r4.size() == 3);
    assert(r4[0] == 1);
    assert(r4[1] == 1);
    assert(r4[2] == 2);

    // Test 5: Full window equals array size
    std::vector<int> a5 = {4, 2, 3, 1};
    std::vector<int> r5 = slidingCartesianDepths(a5, 4);
    assert(r5.size() == 1);
    // Build tree: root sentinel, children: 4? Process:
    // i=4 val=1: parent sentinel.
    // i=3 val=3: nearest greater right? none -> sentinel.
    // i=2 val=2: nearest greater right is 3 (val3) -> parent=3.
    // i=1 val=4: parent sentinel.
    // DFS: sentinel children: 1 (left0), 4 (left1), 3 (left2) -> 3's child 2 (left3). Intervals: [0,0], [1,1], [2,3], [3,3].
    // All active: max depth = 2? At point 3, covered by 3 and 2, also maybe by sentinel not counted. So max=2.
    assert(r5[0] == 2);

    // Test 6: Decreasing sequence
    std::vector<int> a6 = {5, 4, 3, 2, 1};
    std::vector<int> r6 = slidingCartesianDepths(a6, 2);
    // Each element is parent of the one to its left? Actually for decreasing, each element has nearest greater to right as the next element. So tree is a chain: 5 -> 4 -> 3 -> 2 -> 1 (with sentinel parent of 5). DFS gives intervals: [0,4], [1,4], [2,4], [3,4], [4,4].
    // k=2: 
    // i=1 add [0,4] -> max=1 -> output? i-k= -? i>=2 only when i>=2. So i=1 not output.
    // i=2 add [1,4], remove [0,4] -> active [1,4] max=1 -> output 1
    // i=3 add [2,4], remove [1,4] -> active [2,4] max=1 -> output 1
    // i=4 add [3,4], remove [2,4] -> active [3,4] max=1
    // i=5 add [4,4], remove [3,4] -> active [4,4] max=1
    std::vector<int> r6_ans = slidingCartesianDepths(a6, 2);
    assert(r6_ans.size() == 4);
    for (int v : r6_ans) assert(v == 1);

    // Test 7: Increasing sequence
    std::vector<int> a7 = {1, 2, 3, 4, 5};
    // Each element has no greater to the right except sentinel. So all are children of sentinel. Intervals each single point.
    // k=3:
    // i=3 add [l3,r3]? intervals are single points. Max depth = 1 always. Outputs: for i=3,4,5 -> three 1's.
    std::vector<int> r7 = slidingCartesianDepths(a7, 3);
    assert(r7.size() == 3);
    for (int v : r7) assert(v == 1);

    return 0;
}

// The core of the solution uses a Cartesian tree constructed from the array with a stack-based approach in reverse order: for each element from right to left, find the nearest element to the right with a larger value using a Fenwick tree that stores the minimum index seen for each value. The parent of element `i` is that nearest larger element (or the sentinel `n+1` if none exists). After building the tree, a DFS assigns each node an interval `[l[i], r[i]]` in the Euler tour (preorder), where `l[i]` is the time of arrival and `r[i]` is the maximum arrival time in the subtree. This interval property means that every node's subtree maps to a contiguous range in the DFS order. Then, we maintain a segment tree with lazy propagation that supports range addition and range maximum query. For each `i` from 1 to n, we add +1 to `[l[i], r[i]]`. When `i > k`, we subtract 1 from the interval of element `i-k`. The answer for each `i` from `k` to `n` is the maximum value in the segment tree. We output these values in order. Edge cases: when `k = n`, only the final full window is output; when `k = 1`, we output every step after adding and immediately removing the previous element. The segment tree must handle lazy values correctly, and the Fenwick tree must be initialized to `n+1`. Time complexity is `O(n log n)` for building the tree and `O(n log n)` for the updates/queries. Space complexity is `O(n)` for the tree and segment tree.
