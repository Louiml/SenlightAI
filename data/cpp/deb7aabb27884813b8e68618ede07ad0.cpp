/*
You are given an array of positive integers and two types of operations: a point update that changes one element to a new positive integer, and a range query that asks for the number of pairs of indices \((i, j)\) with \(l \le i < j \le r\) such that \(\gcd(a_i, a_j) > 1\). Write a standalone C++ function that processes a sequence of such operations. The function should accept the initial array, a list of operations (each either update or query), and return the list of answers for all queries in order. The input sizes satisfy \(1 \le n, q \le 10^5\), each array element and update value is between 1 and \(10^9\), and all operations are valid. The function must be efficient enough to handle worst-case constraints.
*/
#include <bits/stdc++.h>
using namespace std;

struct Node {
    long long val;               // number of valid pairs in this segment
    vector<pair<int, int>> pre;  // (gcd, count) for prefixes from left
    vector<pair<int, int>> suff; // (gcd, count) for suffixes from right
    Node() : val(0) {}
};

// Merge two adjacent segments: left (x) then right (y)
Node mergeNodes(const Node& x, const Node& y, int L, int R) {
    (void)L; (void)R; // not needed, but kept for clarity
    Node res;
    res.val = x.val + y.val;

    // Count crossing pairs: for each suffix block of x and prefix block of y
    if (!x.suff.empty() && !y.pre.empty()) {
        // Total count of elements in x.suff blocks as we move from rightmost to leftmost
        long long cnt_x = 0;
        for (auto& p : x.suff) cnt_x += p.second;
        int cur = (int)x.suff.size() - 1;
        for (auto& q : y.pre) {
            // Move cur left while gcd(x.suff[cur].first, q.first) == 1
            while (cur > 0 && __gcd(x.suff[cur].first, q.first) == 1) {
                cnt_x -= x.suff[cur].second;
                cur--;
            }
            if (__gcd(x.suff[cur].first, q.first) != 1) {
                res.val += cnt_x * q.second;
            }
        }
    }

    // Build prefix GCD list: start from x.pre, then append y.pre with compression
    res.pre = x.pre;
    for (auto& q : y.pre) {
        int nxt = __gcd(res.pre.back().first, q.first);
        if (nxt == res.pre.back().first) {
            res.pre.back().second += q.second;
        } else {
            res.pre.push_back({nxt, q.second});
        }
    }

    // Build suffix GCD list: start from y.suff, then append x.suff
    res.suff = y.suff;
    for (auto& q : x.suff) {
        int nxt = __gcd(res.suff.back().first, q.first);
        if (nxt == res.suff.back().first) {
            res.suff.back().second += q.second;
        } else {
            res.suff.push_back({nxt, q.second});
        }
    }

    return res;
}

// Segment tree for range queries and point updates
class SegmentTree {
private:
    int n;
    vector<Node> tree;

    void build(const vector<int>& arr, int node, int l, int r) {
        if (l == r) {
            Node& cur = tree[node];
            cur.val = 0; // single element cannot form a pair
            cur.pre = {{arr[l], 1}};
            cur.suff = {{arr[l], 1}};
            return;
        }
        int mid = (l + r) / 2;
        build(arr, node*2, l, mid);
        build(arr, node*2+1, mid+1, r);
        tree[node] = mergeNodes(tree[node*2], tree[node*2+1], l, r);
    }

    void update(int node, int l, int r, int pos, int val) {
        if (pos < l || pos > r) return;
        if (l == r) {
            Node& cur = tree[node];
            cur.val = 0;
            cur.pre[0].first = val;
            cur.suff[0].first = val;
            return;
        }
        int mid = (l + r) / 2;
        update(node*2, l, mid, pos, val);
        update(node*2+1, mid+1, r, pos, val);
        tree[node] = mergeNodes(tree[node*2], tree[node*2+1], l, r);
    }

    Node query(int node, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return Node(); // empty
        if (ql <= l && r <= qr) return tree[node];
        int mid = (l + r) / 2;
        Node left = query(node*2, l, mid, ql, qr);
        Node right = query(node*2+1, mid+1, r, ql, qr);
        if (left.pre.empty()) return right;
        if (right.pre.empty()) return left;
        return mergeNodes(left, right, l, r);
    }

public:
    SegmentTree(const vector<int>& arr) {
        n = (int)arr.size();
        tree.resize(4*n + 5);
        if (n > 0) build(arr, 1, 0, n-1);
    }

    void update(int pos, int val) {
        if (n == 0) return;
        update(1, 0, n-1, pos, val);
    }

    long long query(int l, int r) {
        if (n == 0 || l > r) return 0;
        Node result = query(1, 0, n-1, l, r);
        return result.val;
    }
};

// Main function to process operations
// Operations are given as tuples (type, l, r):
//   type=1 : update a[l] = r  (0-indexed l)
//   type=2 : query range [l, r] inclusive, return answer
vector<long long> countNonCoprimePairs(const vector<int>& arr,
                                      const vector<tuple<int,int,int>>& ops) {
    SegmentTree st(arr);
    vector<long long> answers;
    for (const auto& op : ops) {
        int type = get<0>(op);
        int x = get<1>(op);
        int y = get<2>(op);
        if (type == 1) {
            st.update(x, y);
        } else {
            answers.push_back(st.query(x, y));
        }
    }
    return answers;
}
#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// (include the solution code here)

int main() {
    // Test 1: Simple two elements, both even -> one pair
    {
        vector<int> arr = {2, 4};
        vector<tuple<int,int,int>> ops = {{2, 0, 1}};
        vector<long long> ans = countNonCoprimePairs(arr, ops);
        assert(ans.size() == 1 && ans[0] == 1);
    }

    // Test 2: All ones -> no pairs
    {
        vector<int> arr = {1, 1, 1};
        vector<tuple<int,int,int>> ops = {{2, 0, 2}};
        vector<long long> ans = countNonCoprimePairs(arr, ops);
        assert(ans.size() == 1 && ans[0] == 0);
    }

    // Test 3: Range with mixed numbers
    {
        vector<int> arr = {2, 3, 4, 5, 6};
        vector<tuple<int,int,int>> ops = {{2, 0, 4}};
        vector<long long> ans = countNonCoprimePairs(arr, ops);
        // Pairs: (2,4),(2,6),(4,6) -> 3
        assert(ans.size() == 1 && ans[0] == 3);
    }

    // Test 4: Update changes answer
    {
        vector<int> arr = {2, 3, 4};
        vector<tuple<int,int,int>> ops = {{2, 0, 2}, {1, 1, 4}, {2, 0, 2}};
        vector<long long> ans = countNonCoprimePairs(arr, ops);
        // initial: pairs (2,4) -> 1
        // after update a[1]=4, array = [2,4,4] -> pairs (2,4),(2,4),(4,4) = 3
        assert(ans.size() == 2);
        assert(ans[0] == 1);
        assert(ans[1] == 3);
    }

    // Test 5: Single element query
    {
        vector<int> arr = {7};
        vector<tuple<int,int,int>> ops = {{2, 0, 0}};
        vector<long long> ans = countNonCoprimePairs(arr, ops);
        assert(ans[0] == 0);
    }

    // Test 6: Larger array with random values, verify against brute force
    {
        int n = 50;
        vector<int> arr(n);
        mt19937 rng(12345);
        for (int i = 0; i < n; i++) arr[i] = 1 + rng() % 1000;

        // Build random operations
        vector<tuple<int,int,int>> ops;
        for (int i = 0; i < 200; i++) {
            int type = (i % 3 == 0) ? 1 : 2;
            int l = rng() % n;
            int r = rng() % n;
            if (l > r) swap(l, r);
            if (type == 1) {
                int v = 1 + rng() % 1000;
                ops.push_back({type, l, v}); // update position l to v
            } else {
                ops.push_back({type, l, r});
            }
        }

        vector<long long> result = countNonCoprimePairs(arr, ops);
        // Brute force simulation
        vector<int> cur = arr;
        int q_idx = 0;
        for (auto& op : ops) {
            int type = get<0>(op);
            int x = get<1>(op);
            int y = get<2>(op);
            if (type == 1) {
                cur[x] = y;
            } else {
                long long expected = 0;
                for (int i = x; i <= y; i++)
                    for (int j = i+1; j <= y; j++)
                        if (__gcd(cur[i], cur[j]) > 1) expected++;
                assert(result[q_idx++] == expected);
            }
        }
    }

    printf("All tests passed!\n");
    return 0;
}
// This problem is a classic "count non-coprime pairs in range with point updates" problem solvable with a segment tree where each node stores:
// - The number of valid pairs within that segment (`val`).
// - A compressed list of prefix GCDs (`pre`) where each entry is (gcd_value, count_of_elements) representing the GCD of a prefix ending at the segment's left boundary.
// - A compressed list of suffix GCDs (`suff`) similarly for prefixes starting at the segment's right boundary.
//
// When merging two child nodes `x` (left) and `y` (right), new pairs crossing the boundary are counted. For each distinct suffix GCD of `x` and prefix GCD of `y`, if their combined GCD > 1, then all combinations of elements from those two groups form valid pairs. We can count them efficiently by iterating over the compressed lists, which are short: for any segment, the number of distinct prefix GCDs is at most \(O(\log \max a_i)\) because each step either reduces the GCD strictly or stays constant, and the GCD can only decrease at most \(\log_2(10^9)\) times. The compressed lists are built by appending new GCDs after combining with the neighbor segment.
//
// For a leaf with value `v`, `val = (v != 1)` because a single element cannot form a pair, but if `v == 1` then no pair with itself (but there is only one element, so val is 0 anyway; we set it to 0 for v==1 and 1 for v>1? Wait: The original snippet sets `val = (a[l] != 1)`. That counts the number of elements that are > 1. That is because later during merge, they add pairs formed by crossing the boundary. At a leaf, there are no pairs, so val should be 0. But the snippet sets it to (a[l] != 1) which is 1 if the element > 1. That seems wrong? Let's re-examine: In the original merge, `res.val = x.val + y.val` then they add `cnt * v.se` when gcd > 1. `cnt` is the total count of suffix elements from x. If x is a leaf and has one element, then x.suff has one entry (value a[l], count 1), x.pre same. So at a leaf, x.val is set to (a[l]!=1). But after merging two leaves, res.val = (a1!=1)+(a2!=1) + if gcd(a1,a2)>1 then 1*1 = 1. So for two elements both >1 and gcd>1, res.val = 1+1+1=3? That is wrong. Actually leaf val should be 0 because a single element cannot form a pair. The correct base case is `it[node].val = 0` always. The snippet's assignment `val = (a[l] != 1)` is a mistake if interpreted as pair count. However, let's check the original problem: In the original code, after building leaves, they set val = (a[l] != 1). Then merge adds x.val + y.val plus crossing pairs. So after building a subtree, the val stored is actually the number of elements > 1? Let's test: two leaves [2,3] both >1, gcd=1. Merge: res.val = 1+1 = 2, no crossing. So val=2, but correct answer for range [2,3] is 0 pairs. So that is incorrect. Wait maybe the original problem counts something else? Looking at the snippet: after query, they output `query(...).val`. In the test example? Not given. Let's re-analyze: The merge function adds `cnt * v.se` only when gcd > 1, where `cnt` is total count of suffix elements from x. At leaf, x.suff has one element, so cnt=1. So for leaf pair? Actually merge is called on segments of size > 1. For two leaves, x.suff has 1 element, y.pre has 1 element. They iterate over y.pre. They compute cur = SZE(x.suff)-1 = 0. Then while (cur > 0 ...) doesn't run. Then if gcd(x.suff[0], v.fi) != 1, add cnt * v.se = 1*1=1. So res.val = x.val+y.val+1 = (a1!=1)+(a2!=1)+1. For a1=2,a2=4, both >1, gcd>1: val=1+1+1=3, but correct pair count is 1. So indeed the leaf val should be 0. The original snippet is flawed but maybe they intended to count something else? Let's check: In the original problem, perhaps they want to count pairs with gcd > 1, and leaf val should be 0. But the snippet sets leaf val to (a[l]!=1). That means the root val will be total number of elements > 1 plus something? That doesn't match. I suspect the original snippet is buggy or part of a different problem. However, for our task, we must define correct behavior: Count pairs (i,j) with i<j in range such that gcd(a_i,a_j) > 1. So leaf val must be 0. The merge must correctly count crossing pairs. The correct merge logic: Let x be left segment, y right segment. For each suffix block of x (gcd value gx, count cx) and each prefix block of y (gcd value gy, count cy), if gcd(gx, gy) > 1, then all cx*cy pairs are valid. So we sum them. The total val = x.val + y.val + sum over blocks where gcd>1 of cx*cy. The compressed lists reduce the number of blocks because many adjacent elements share the same GCD for prefixes/suffixes. For any array segment, the number of distinct prefix GCDs is at most O(log max_value) (at most ~30 for 1e9). So merging two nodes takes O(log^2 max_value) time. Segment tree operations: build O(n log n), update O(log^2 n), query O(log^2 n) as well because at most O(log n) nodes are visited and each merge costs O(log^2). Overall O((n+q) log^2 n) time, O(n log n) memory.
//
// Edge cases: When a value is 1, it cannot form a valid pair with any number (gcd(1,x)=1). So leaf val remains 0. When merging, if both sides have only 1s, crossing pairs zero. The compressed lists must be built correctly: For prefix list, we start with the left child's prefix, then for each prefix of right child, we compute gcd(prefix_last_gcd, new_gcd) and either merge with last element if equal or append. Similarly for suffix.
//
// We must also handle the query merge correctly when returning a Node for a partial overlap. The function returns a temporary Node for empty segments, and we merge left and right results. The merge must be careful to handle empty nodes.
//
// Now to design the task: We need a function `countNonCoprimePairs` that takes the initial array as vector<int>, and operations as a vector of tuples (type, l, r) where type=1 means update l to r (0-indexed), type=2 means query range [l,r] inclusive. Return vector<long long> of answers.
//
// Because the original snippet uses 1-indexed and ll type, we adapt to 0-indexed and use long long for counts (max pairs ~ 1e10). The solution will implement a segment tree with Node structure as described. The self-contained solution function must not have a main, but the test will include a main calling it.
//
// We must produce a high-quality reference solution with appropriate const correctness and comments.
