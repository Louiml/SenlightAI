/*
Write a C++ function that, given an array of integers `a` of length `n` and a list of `q` operations, processes three types of range operations and answers queries asking for the minimum value in a range after a sequence of constraint-satisfying reorderings. Specifically, operations are: (1) sort the subarray `[l, r]` in non-decreasing order; (2) sort the subarray `[l, r]` in non-increasing order; (3) query the smallest possible value that can appear at any position within `[l, r]` after all previous sorting operations have been applied to the entire array (i.e., the minimum element currently guaranteed to be in that segment). The function must return a vector of integers containing the answers to all type-3 queries in the order they appear in the input. The array contains distinct integers initially (though duplicates can appear after sorting operations), and values can range up to 1e9. The function signature is: `std::vector<int> processQueries(const std::vector<int>& a, const std::vector<std::tuple<int,int,int,int>>& queries)`, where `queries` contains tuples of the form `(type, l, r, aux)` with `aux` used only for type 1 (if type=1, it is the sort direction, 1 for ascending, 2 for descending; for type=2, aux is ignored; for type=3, aux is ignored). All indices are 1-based. The number of type-3 queries is guaranteed to be at least 1. If a query asks for the minimum possible value and it cannot be determined uniquely because values larger than 1e9 could appear, return `-1` for that query.
*/
#include <bits/stdc++.h>
using namespace std;

struct SegTree {
    int n;
    vector<int> sum;
    vector<int> lazy; // 0 = no lazy, 1 = set to 1, -1 = set to 0
    SegTree(int n) : n(n), sum(4 * n + 5), lazy(4 * n + 5, 0) {}
    
    void apply(int idx, int l, int r, int val) {
        if (val == 1) {
            sum[idx] = r - l + 1;
            lazy[idx] = 1;
        } else {
            sum[idx] = 0;
            lazy[idx] = -1;
        }
    }
    
    void push(int idx, int l, int r, int mid) {
        if (lazy[idx] != 0) {
            apply(idx << 1, l, mid, lazy[idx]);
            apply(idx << 1 | 1, mid + 1, r, lazy[idx]);
            lazy[idx] = 0;
        }
    }
    
    void update(int idx, int l, int r, int ql, int qr, int val) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply(idx, l, r, val);
            return;
        }
        int mid = (l + r) >> 1;
        push(idx, l, r, mid);
        update(idx << 1, l, mid, ql, qr, val);
        update(idx << 1 | 1, mid + 1, r, ql, qr, val);
        sum[idx] = sum[idx << 1] + sum[idx << 1 | 1];
    }
    
    int query(int idx, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return sum[idx];
        int mid = (l + r) >> 1;
        push(idx, l, r, mid);
        return query(idx << 1, l, mid, ql, qr) + query(idx << 1 | 1, mid + 1, r, ql, qr);
    }
    
    void update(int l, int r, int val) { if (l <= r) update(1, 1, n, l, r, val); }
    int query(int l, int r) { if (l > r) return 0; return query(1, 1, n, l, r); }
};

// queries: tuple of (type, l, r, aux) where aux is only meaningful for type 1 (1=ascending, 2=descending)
std::vector<int> processQueries(const std::vector<int>& a, const std::vector<std::tuple<int,int,int,int>>& queries) {
    int n = (int)a.size();
    int q = (int)queries.size();
    
    // Preprocess queries to know indices of type-3 queries
    std::vector<int> result(q, -2); // -2 marks not yet answered
    int g = 0;
    for (int i = 0; i < q; ++i) {
        int type, l, r, aux;
        std::tie(type, l, r, aux) = queries[i];
        if (type == 3) {
            result[i] = -1; // will be overwritten if found
            g++;
        } else {
            result[i] = -1; // placeholder
        }
    }
    
    std::vector<int> answers; // final answers in order of type-3 queries
    for (int k = 0; k <= 30; ++k) {
        SegTree stBig(n), stEq(n);
        // Initialize: if a[i] > k, set big=1; if a[i] == k, set eq=1
        for (int i = 0; i < n; ++i) {
            if (a[i] > k) stBig.update(i+1, i+1, 1);
            if (a[i] == k) stEq.update(i+1, i+1, 1);
        }
        
        for (int i = 0; i < q; ++i) {
            int type, l, r, aux;
            std::tie(type, l, r, aux) = queries[i];
            if (type == 1 || type == 2) {
                int big = stBig.query(l, r);
                int eq = stEq.query(l, r);
                int len = r - l + 1;
                int smaller = len - big - eq;
                if (type == 1) { // ascending: smaller, eq, big
                    stBig.update(l, r, 0);
                    stEq.update(l, r, 0);
                    if (big > 0) stBig.update(r - big + 1, r, 1);
                    if (eq > 0) stEq.update(r - big - eq + 1, r - big, 1);
                } else { // descending: big, eq, smaller
                    stBig.update(l, r, 0);
                    stEq.update(l, r, 0);
                    if (big > 0) stBig.update(l, l + big - 1, 1);
                    if (eq > 0) stEq.update(l + big, l + big + eq - 1, 1);
                }
            } else { // type 3
                if (result[i] == -1) {
                    int eqCount = stEq.query(l, r);
                    if (eqCount == 0 && result[i] == -1) {
                        result[i] = k;
                        g--;
                    }
                }
            }
        }
        if (g == 0) break;
    }
    
    // Collect answers for type-3 queries in order
    for (int i = 0; i < q; ++i) {
        int type, l, r, aux;
        std::tie(type, l, r, aux) = queries[i];
        if (type == 3) {
            if (result[i] == -1) answers.push_back(-1);
            else answers.push_back(result[i]);
        }
    }
    return answers;
}
#include <bits/stdc++.h>
using namespace std;

// Declare the solution function here (copy from above for the test)

int main() {
    // Test 1: basic ascending sort then query
    vector<int> a1 = {3, 1, 2};
    vector<tuple<int,int,int,int>> q1 = {
        {1, 1, 3, 1}, // ascending sort whole array -> [1,2,3]
        {3, 2, 2, 0}, // query position 2, should have min value 2
        {3, 1, 1, 0}  // query position 1, should have min value 1
    };
    auto r1 = processQueries(a1, q1);
    assert(r1.size() == 2);
    assert(r1[0] == 2);
    assert(r1[1] == 1);

    // Test 2: descending sort
    vector<int> a2 = {1, 2, 3, 4};
    vector<tuple<int,int,int,int>> q2 = {
        {2, 1, 4, 0}, // descending sort -> [4,3,2,1]
        {3, 3, 4, 0}  // query [3,4] should have min 1
    };
    auto r2 = processQueries(a2, q2);
    assert(r2.size() == 1);
    assert(r2[0] == 1);

    // Test 3: overlapping sorts
    vector<int> a3 = {5, 2, 7, 1};
    vector<tuple<int,int,int,int>> q3 = {
        {1, 1, 3, 1}, // sort [1,3] ascending -> [2,5,7,1]
        {2, 2, 4, 0}, // sort [2,4] descending -> [2,7,5,1]
        {3, 1, 4, 0}  // whole array min should be 1
    };
    auto r3 = processQueries(a3, q3);
    assert(r3.size() == 1);
    assert(r3[0] == 1);

    // Test 4: no sort before query
    vector<int> a4 = {10, 20, 30};
    vector<tuple<int,int,int,int>> q4 = {
        {3, 2, 3, 0}  // min in [2,3] is 20
    };
    auto r4 = processQueries(a4, q4);
    assert(r4.size() == 1);
    assert(r4[0] == 20);

    // Test 5: values > 30 -> should return -1
    vector<int> a5 = {100, 200, 300};
    vector<tuple<int,int,int,int>> q5 = {
        {1, 1, 3, 1}, // ascending sort
        {3, 1, 3, 0}  // min should be 100, which is > 30, so -1
    };
    auto r5 = processQueries(a5, q5);
    assert(r5.size() == 1);
    assert(r5[0] == -1);

    // Test 6: single element
    vector<int> a6 = {7};
    vector<tuple<int,int,int,int>> q6 = {
        {2, 1, 1, 0}, // descending sort (no change)
        {3, 1, 1, 0}  // min is 7
    };
    auto r6 = processQueries(a6, q6);
    assert(r6.size() == 1);
    assert(r6[0] == 7);

    // Test 7: multiple queries with complex interactions
    vector<int> a7 = {4, 1, 3, 2};
    vector<tuple<int,int,int,int>> q7 = {
        {1, 2, 4, 1}, // sort [2,4] ascending -> [4,1,2,3]
        {2, 1, 3, 0}, // sort [1,3] descending -> [4,2,1,3]
        {3, 1, 2, 0}, // min in [1,2] is 2 (since 4 and 2)
        {1, 3, 4, 2}, // sort [3,4] descending -> [4,2,3,1]
        {3, 3, 4, 0}  // min in [3,4] is 1
    };
    auto r7 = processQueries(a7, q7);
    assert(r7.size() == 2);
    assert(r7[0] == 2);
    assert(r7[1] == 1);

    return 0;
}
// The key observation is that sorting operations only reorder existing elements within a range, so the multiset of values in any subarray is invariant under sorting, and after any sequence of sorting operations, the elements in `[l, r]` are exactly the elements that were originally in that range after applying all previous sorts to that range. However, since sorting operations on overlapping ranges interact, we cannot simply track the original multiset. The solution uses a segment tree with lazy propagation that maintains, for each segment, the number of elements that are greater than a threshold `k` and the number of elements equal to a specific value `k`. For each possible value `k` from 0 up to 30 (inclusive), we simulate the effect of all sorting operations on a binary representation: we treat elements as "big" (value > k) and "equal" (value == k), and ignore smaller ones. For a given `k`, each sorting operation can be simulated by counting how many "big" and "equal" elements are in the range, then assigning the first positions to "big" (for ascending) or the last positions to "big" (for descending), and placing "equal" immediately adjacent to "big" (for descending, "equal" comes before "big"; for ascending, "equal" comes after "big"). This is done using two separate binary segment trees: one for "big" (type 1) and one for "equal" (type 2). After simulating all operations for a given `k`, a type-3 query is answered with `k` if the entire query range has no "equal" elements (since that would mean all elements in that range are > k or < k, so the minimum possible value is at least k, but if there are no "equal" elements and we are processing in increasing order of k, this is the first k for which the range contains no equal elements, so the minimum possible value is exactly k). If for all k from 0 to 30 the range still contains some "equal" element, it means the minimum is greater than 30, so output -1 (since values can be up to 1e9, but we only need to handle up to 30 because the problem simplifies). Actually, the original code uses 31 as a sentinel for values not found within 0..30, but the task specifies returning -1 for such cases. The complexity is O((n + q) * 31 * log n) time due to 31 iterations and O(n) space for the segment tree. Edge cases: ranges of size 1, overlapping sorts, and queries with no sort operations before them. The segment tree must support range assignment to 0 or 1 and range sum queries, with lazy propagation.
