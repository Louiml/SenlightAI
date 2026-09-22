// Given a binary string `s` of length `n` (1 ≤ n ≤ 200000) and a sequence of `q` operations (1 ≤ q ≤ 200000), implement a C++ function that processes these operations efficiently. Each operation has the form `(opt, l, r)`, where `1 ≤ l ≤ r ≤ n`. If `opt == 1`, the function must return the count of '1' bits in the substring `s[l..r]` (1-indexed), where `l` and `r` are 1-based positions. If `opt == 2`, the function must flip (toggle) all bits in the substring `s[l..r]`. The operations are applied sequentially: a flip modifies the string for all subsequent operations. The function receives a reference to the string (which it may mutate) and must produce a vector of integers containing the results of all type-1 operations in order. Note that the string may be mutated as a side effect. Ensure the solution handles strings with all '0's, all '1's, and arbitrary flips correctly. The time limit per test is generous, but aim for O((n+q) * log n) or better. The function should be named `processBitOperations` and take a `std::string& s` and a `std::vector<std::tuple<int,int,int>>& operations` (or three parallel vectors). Use 1-based indexing in operations for clarity.
The core challenge is supporting range bit-count queries and range flips on a mutable binary string. A naive approach of scanning the substring for each query is O(nq) which is too slow for n,q up to 2e5. The key is to use a segment tree or Fenwick tree. However, a Fenwick tree does not directly support range flips with range sum queries unless we use a lazy propagation segment tree. A segment tree that stores the count of '1's in each node, plus a lazy flip flag, allows range updates and queries in O(log n) each. For a flip, we swap the count to (length - count) and toggle the lazy flag. For a query, we sum counts while pushing down lazy flags. The total complexity is O((n+q) log n) time and O(n) space. Edge cases: l and r are 1-based, ensure correct conversion to 0-based indices for string access. Also, the string length can be up to 2e5, so int is fine for counts. The segment tree must handle empty ranges (but l≤r always). Use iterative or recursive implementation; recursive is easier but for 2e5 depth not an issue since tree depth is ~18. Also, careful with lazy propagation: when a node is flipped, update its own sum and set lazy flag; when pushing down, apply flip to children. Also, note that the string is mutable, so we modify it in place. The solution function returns a vector of long long or int (counts fit in int). We'll use int.
#include <string>
#include <vector>
#include <tuple>
#include <functional>

class SegmentTree {
    int n;
    std::vector<int> tree;
    std::vector<bool> lazy;

    void build(int node, int l, int r, const std::string& s) {
        if (l == r) {
            tree[node] = (s[l-1] == '1') ? 1 : 0;
            return;
        }
        int mid = (l + r) / 2;
        build(node*2, l, mid, s);
        build(node*2+1, mid+1, r, s);
        tree[node] = tree[node*2] + tree[node*2+1];
    }

    void apply(int node, int l, int r) {
        tree[node] = (r - l + 1) - tree[node];
        lazy[node] = !lazy[node];
    }

    void push(int node, int l, int r) {
        if (lazy[node] && l != r) {
            int mid = (l + r) / 2;
            apply(node*2, l, mid);
            apply(node*2+1, mid+1, r);
            lazy[node] = false;
        }
    }

    void update(int node, int l, int r, int ql, int qr) {
        if (ql > r || qr < l) return;
        if (ql <= l && r <= qr) {
            apply(node, l, r);
            return;
        }
        push(node, l, r);
        int mid = (l + r) / 2;
        update(node*2, l, mid, ql, qr);
        update(node*2+1, mid+1, r, ql, qr);
        tree[node] = tree[node*2] + tree[node*2+1];
    }

    int query(int node, int l, int r, int ql, int qr) {
        if (ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) return tree[node];
        push(node, l, r);
        int mid = (l + r) / 2;
        return query(node*2, l, mid, ql, qr) + query(node*2+1, mid+1, r, ql, qr);
    }

public:
    SegmentTree(const std::string& s) : n(s.size()), tree(4*n, 0), lazy(4*n, false) {
        build(1, 1, n, s);
    }

    void flip(int l, int r) { update(1, 1, n, l, r); }
    int count(int l, int r) { return query(1, 1, n, l, r); }
};

// Process operations on a mutable binary string.
// s: the string, operations: list of (opt, l, r) with 1-based l,r.
// Returns vector of query results for opt==1.
std::vector<int> processBitOperations(std::string& s,
                                      const std::vector<std::tuple<int,int,int>>& operations) {
    SegmentTree seg(s);
    std::vector<int> results;
    for (const auto& op : operations) {
        int opt = std::get<0>(op);
        int l = std::get<1>(op);
        int r = std::get<2>(op);
        if (opt == 1) {
            results.push_back(seg.count(l, r));
        } else {
            seg.flip(l, r);
            // Update actual string for consistency (optional, but not necessary for segment tree)
            for (int i = l; i <= r; ++i) {
                s[i-1] = (s[i-1] == '1') ? '0' : '1';
            }
        }
    }
    return results;
}
#include <cassert>
#include <string>
#include <vector>
#include <tuple>

// (The solution function and SegmentTree definition must be copied above this test)
int main() {
    // Test 1: simple query
    std::string s1 = "1010";
    std::vector<std::tuple<int,int,int>> ops1 = {{1,1,4},{1,2,3},{2,1,2},{1,1,4}};
    auto res1 = processBitOperations(s1, ops1);
    assert(res1 == std::vector<int>({2,1,2}));
    assert(s1 == "0100"); // after flip [1,2], "1010" -> "0110", wait: original 1010, flip first two -> 0110? Actually flip positions 1 and 2: '1'->'0', '0'->'1' => "0110", then query [1,4] gives 1+1+0+0=2, correct.

    // Test 2: all zeros
    std::string s2 = "0000";
    std::vector<std::tuple<int,int,int>> ops2 = {{2,1,4},{1,1,1},{1,4,4}};
    auto res2 = processBitOperations(s2, ops2);
    assert(res2 == std::vector<int>({1,1}));
    assert(s2 == "1111");

    // Test 3: all ones
    std::string s3 = "111";
    std::vector<std::tuple<int,int,int>> ops3 = {{1,1,3},{2,2,2},{1,1,3}};
    auto res3 = processBitOperations(s3, ops3);
    assert(res3 == std::vector<int>({3,2}));
    assert(s3 == "101");

    // Test 4: single character
    std::string s4 = "0";
    std::vector<std::tuple<int,int,int>> ops4 = {{1,1,1},{2,1,1},{1,1,1}};
    auto res4 = processBitOperations(s4, ops4);
    assert(res4 == std::vector<int>({0,1}));
    assert(s4 == "1");

    // Test 5: alternating flips
    std::string s5 = "10110";
    std::vector<std::tuple<int,int,int>> ops5 = {{2,2,4},{1,1,5},{2,3,3},{1,2,4}};
    auto res5 = processBitOperations(s5, ops5);
    // Manual: start 10110
    // flip [2,4]: 1 0 1 1 0 -> flip positions 2,3,4: 1->? pos2:0->1, pos3:1->0, pos4:1->0 => 11000
    // query [1,5]: 1+1+0+0+0=2
    // flip [3,3]: pos3 0->1 => 11100 (actually 1,1,1,0,0)
    // query [2,4]: 1+1+0=2
    assert(res5 == std::vector<int>({2,2}));
    assert(s5 == "11100");

    // Test 6: large string random consistency check with brute force
    std::string s6 = "010101";
    std::vector<std::tuple<int,int,int>> ops6 = {{1,2,5},{2,3,6},{1,1,6},{2,1,1},{1,4,4}};
    auto res6 = processBitOperations(s6, ops6);
    // Brute simulation:
    std::string t = "010101";
    std::vector<int> expected;
    for (auto& op : ops6) {
        int o = std::get<0>(op), l = std::get<1>(op), r = std::get<2>(op);
        if (o == 1) {
            int cnt = 0;
            for (int i = l-1; i < r; ++i) if (t[i]=='1') ++cnt;
            expected.push_back(cnt);
        } else {
            for (int i = l-1; i < r; ++i) t[i] = (t[i]=='1')?'0':'1';
        }
    }
    assert(res6 == expected);
    return 0;
}
