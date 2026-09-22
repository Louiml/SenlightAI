// Given an array of integers and a set of range queries that either apply a bitwise XOR to every element in a subarray or ask for the size of the set of all possible XOR results of subsets of elements in a subarray, write a C++ function that processes the queries and outputs for each query of the second type the number of distinct XOR values obtainable from any non-empty subset of the queried range. The function must support dynamic updates (range XOR) and range queries, with the answer being a power of two modulo a fixed large integer (1,000,000,007). The implementation should use a segment tree where each node stores a basis (in reduced row-echelon form) of the XOR subspace of the elements in its interval, and must correctly handle lazy propagation of XOR updates.

The core idea is to represent the set of all subset XORs of a range as a linear basis over GF(2) using 30-bit integers (assuming values up to 10^9). Since the XOR of a subset is a linear combination of the basis vectors, the number of distinct XOR values (including zero) is \(2^k\), where \(k\) is the rank of the basis. For non-empty subsets, we subtract 1 if zero is achievable (which it always is when the basis is non-trivial), but the problem as described asks for the number of distinct XOR values possibly including zero; the original snippet excludes zero and outputs \(1 << \text{rank}\). A range XOR update adds the same value to every element; this is equivalent to XORing the whole subspace by that value. To handle this efficiently, each node stores a basis of the original elements. When applying a lazy XOR value `v` to a node, we rebuild its basis by XORing every basis vector with `v` (since the subspace shifts). This is done by taking the current basis, adding `(1<<30)` as a sentinel marker to represent "has been XORed" , then replacing each vector `x` with `x ^ v` if `x` has the sentinel bit, and finally recalculating the basis. Lazy tags are stored at internal nodes and propagated during updates/queries. The segment tree is built bottom-up: leaves store single elements (with sentinel bit set), and internal nodes merge the children's bases via Gaussian elimination. Range queries collect bases from covered nodes and merge them to get a final basis, then count the rank. Time complexity: build O(n * 30^2), each update/query O(30^2 * log n) roughly, because merging two bases of size at most 30 takes O(30^2). Space: O(n * 30) for storing bases. Edge cases: empty range not expected; values may be zero; lazy propagation must be applied carefully to avoid double-applying; sentinel bit must be masked out when counting rank. The answer is \(2^{\text{rank}}\) modulo 1,000,000,007; but original snippet prints `1 << (int)se.size()` which fits in int as rank ≤ 30.

#include <bits/stdc++.h>
using namespace std;

const int MOD = 1000000007;
const int MAX_BITS = 30;

// Build a reduced basis from a list of integers.
void reduceBasis(vector<int>& basis) {
    int pos = 0;
    int sz = (int)basis.size();
    for (int bit = 0; bit < MAX_BITS; ++bit) {
        for (int j = pos; j < sz; ++j) {
            if (basis[j] & (1 << bit)) {
                swap(basis[j], basis[pos]);
                for (int k = j + 1; k < sz; ++k) {
                    if (basis[k] & (1 << bit)) {
                        basis[k] ^= basis[pos];
                    }
                }
                ++pos;
                break;
            }
        }
    }
    basis.erase(basis.begin() + pos, basis.end());
}

// Merge two bases into a new basis.
vector<int> mergeBases(const vector<int>& a, const vector<int>& b) {
    vector<int> res = a;
    res.insert(res.end(), b.begin(), b.end());
    reduceBasis(res);
    return res;
}

class SegTree {
private:
    int n;
    int size;
    vector<vector<int>> tree;
    vector<int> lazy;

    void build(const vector<int>& arr, int node, int l, int r) {
        if (l == r) {
            if (l < n) {
                tree[node].push_back(arr[l] | (1 << MAX_BITS)); // sentinel bit
                reduceBasis(tree[node]);
            }
            return;
        }
        int mid = (l + r) >> 1;
        build(arr, node * 2, l, mid);
        build(arr, node * 2 + 1, mid + 1, r);
        tree[node] = mergeBases(tree[node * 2], tree[node * 2 + 1]);
    }

    // Apply XOR value v to the basis stored at node.
    void applyXor(int node, int v) {
        vector<int>& vc = tree[node];
        vector<int> nv;
        for (int x : vc) {
            if (x & (1 << MAX_BITS)) {
                x ^= v;
            }
            nv.push_back(x);
        }
        // Keep sentinel bit to indicate this node has been XORed.
        for (int& x : nv) x |= (1 << MAX_BITS);
        tree[node] = nv;
        reduceBasis(tree[node]);
    }

    void push(int node, bool hasChildren) {
        if (lazy[node] == 0) return;
        if (hasChildren) {
            lazy[node * 2] ^= lazy[node];
            lazy[node * 2 + 1] ^= lazy[node];
        }
        applyXor(node * 2, lazy[node]);
        applyXor(node * 2 + 1, lazy[node]);
        lazy[node] = 0;
    }

    void update(int node, int l, int r, int ql, int qr, int val) {
        if (ql <= l && r <= qr) {
            lazy[node] ^= val;
            applyXor(node, val);
            return;
        }
        int mid = (l + r) >> 1;
        push(node, l != mid);
        if (ql <= mid) update(node * 2, l, mid, ql, min(qr, mid), val);
        if (qr > mid) update(node * 2 + 1, mid + 1, r, max(ql, mid + 1), qr, val);
        tree[node] = mergeBases(tree[node * 2], tree[node * 2 + 1]);
    }

    vector<int> query(int node, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) {
            return tree[node];
        }
        int mid = (l + r) >> 1;
        push(node, l != mid);
        vector<int> res;
        if (ql <= mid) res = mergeBases(res, query(node * 2, l, mid, ql, min(qr, mid)));
        if (qr > mid) res = mergeBases(res, query(node * 2 + 1, mid + 1, r, max(ql, mid + 1), qr));
        return res;
    }

public:
    SegTree(const vector<int>& arr) {
        n = (int)arr.size();
        size = 1;
        while (size < n) size <<= 1;
        tree.resize(2 * size);
        lazy.assign(2 * size, 0);
        vector<int> padded(size, 0);
        for (int i = 0; i < n; ++i) padded[i] = arr[i];
        build(padded, 1, 0, size - 1);
    }

    void rangeXor(int l, int r, int val) {
        update(1, 0, size - 1, l, r, val);
    }

    long long distinctXorCount(int l, int r) {
        vector<int> basis = query(1, 0, size - 1, l, r);
        int rank = 0;
        for (int x : basis) {
            if ((x & ~(1 << MAX_BITS)) != 0) ++rank;
        }
        long long result = 1;
        for (int i = 0; i < rank; ++i) result = (result * 2) % MOD;
        return result;
    }
};

// Solve the problem: returns vector of answers for all type-2 queries.
vector<long long> processRangeXorQueries(
    const vector<int>& initial,
    const vector<pair<int, pair<int, int>>>& queries // (type, (l, r)) or (type, (l, r)) with val in a separate map? Better: define struct
) {
    // This function signature is too generic; we'll implement a clean one below.
    return vector<long long>();
}

// Clean solution function: takes initial array, and a list of operations.
// Each operation is (type, l, r, val). type 1 = XOR update, type 2 = query count.
vector<long long> solveRangeXorQueries(
    const vector<int>& arr,
    const vector<tuple<int,int,int,int>>& ops) {
    SegTree seg(arr);
    vector<long long> ans;
    for (auto& op : ops) {
        int type = get<0>(op);
        int l = get<1>(op);
        int r = get<2>(op);
        int val = get<3>(op);
        if (type == 1) {
            seg.rangeXor(l, r, val);
        } else {
            ans.push_back(seg.distinctXorCount(l, r));
        }
    }
    return ans;
}

#include <cassert>
#include <vector>
#include <tuple>

int main() {
    // Test 1: basic update and query
    {
        vector<int> arr = {1, 2, 3, 4};
        vector<tuple<int,int,int,int>> ops = {
            {2, 0, 3, 0}, // query entire array: subset XORs of {1,2,3,4} -> basis rank? Let's compute: 1,2,3,4; basis after reduction: rank = 2? Actually 1,2,3 -> 1,2,3 are dependent? 3=1^2, so basis {1,2} rank=2, 4 is not independent? 4=1^2? no. Let's compute: 1,2,3,4 -> reduce: bit0: pick 1; bit1: pick 2; bit2: from 4 we have 4 (100) but bit2 not set? Actually 4=100, so bit2 set. After eliminating, 4 remains. So rank=3? Let's not guess; we'll just assert count = power of 2 rank. The expected count for {1,2,3,4} is 8 (rank 3). So answer should be 8.
            {1, 0, 1, 1}, // XOR first two elements by 1 -> array becomes {0,3,3,4}
            {2, 0, 3, 0}  // query again
        };
        vector<long long> ans = solveRangeXorQueries(arr, ops);
        assert(ans.size() == 2);
        assert(ans[0] == 8);
        // After XOR by 1 on first two: {0,3,3,4} -> subset XORs? basis rank? 0 contributes nothing; 3,3,4 -> basis rank maybe 2? Let's just check output is a power of two.
        assert(ans[1] == 4 || ans[1] == 8); // I'll not be too strict; but better to compute exact: {0,3,3,4} -> basis: 3 (011), 4 (100) rank=2 -> count=4.
        assert(ans[1] == 4);
    }

    // Test 2: single element
    {
        vector<int> arr = {0};
        vector<tuple<int,int,int,int>> ops = {{2,0,0,0}};
        vector<long long> ans = solveRangeXorQueries(arr, ops);
        assert(ans.size() == 1);
        assert(ans[0] == 1); // only {0} -> 1 distinct (0), but original snippet excludes 0, but our function includes 0? The task says "subset XORs" including empty subset? The original output was 1<<rank, which counts all including empty. So for {0}, rank=0 -> count=1.
    }

    // Test 3: no updates, all zeros
    {
        vector<int> arr = {0,0,0};
        vector<tuple<int,int,int,int>> ops = {{2,0,2,0}};
        vector<long long> ans = solveRangeXorQueries(arr, ops);
        assert(ans[0] == 1);
    }

    // Test 4: update across range
    {
        vector<int> arr = {5, 9, 1};
        vector<tuple<int,int,int,int>> ops = {
            {1, 0, 2, 3},
            {2, 0, 2, 0}
        };
        vector<long long> ans = solveRangeXorQueries(arr, ops);
        // After XOR 3: {5^3=6, 9^3=10, 1^3=2} -> {6,10,2}; basis rank? 6(110),10(1010),2(010) -> maybe rank=3? count 8.
        assert(ans.size() == 1);
        assert(ans[0] == 8);
    }

    // Test 5: range query after partial update
    {
        vector<int> arr = {1, 2, 4, 8};
        vector<tuple<int,int,int,int>> ops = {
            {1, 1, 2, 1}, // XOR index 1,2 by 1 -> {1,3,5,8}
            {2, 1, 2, 0}, // query subarray [1,2] -> {3,5} -> basis rank? 3(011),5(101) -> rank=2 -> count=4
        };
        vector<long long> ans = solveRangeXorQueries(arr, ops);
        assert(ans.size() == 1);
        assert(ans[0] == 4);
    }

    return 0;
}
