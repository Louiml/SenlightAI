// You are given `n` employees, each identified by a unique ID in the range `[1, n]` and associated with a salary. You are also given `m` queries, each query is specified by three integers `l, r, x` (1-indexed). For each query, you must determine how many employees with IDs in the inclusive range `[l, r]` have a salary strictly greater than `x`. Write a C++ function `std::vector<int> countEmployeesGreaterThanX(const vector<pair<int,int>>& employees, const vector<tuple<int,int,int>>& queries)` that takes the list of employee ID/salary pairs (not necessarily sorted) and the queries, and returns a vector containing the answer for each query in the original order. The IDs are guaranteed to be a permutation of `1..n`. Handle up to `n = 100,000` and `m = 100,000` efficiently.

#include <bits/stdc++.h>
using namespace std;

// Include the solution function here (as above) or link it.

int main() {
    // Test 1: basic example from prompt
    vector<pair<int,int>> emp1 = {{1,1},{2,2},{3,2},{4,3},{5,5}};
    vector<tuple<int,int,int>> q1 = {{1,5,2},{2,4,1}};
    vector<int> res1 = countEmployeesGreaterThanX(emp1, q1);
    assert(res1.size() == 2);
    assert(res1[0] == 2); // salaries >2 in [1..5]: 3,5 => 2
    assert(res1[1] == 3); // salaries >1 in [2..4]: 2,2,3 => 3

    // Test 2: single employee, threshold below and above
    vector<pair<int,int>> emp2 = {{1, 10}};
    vector<tuple<int,int,int>> q2 = {{1,1,5},{1,1,10},{1,1,11}};
    vector<int> res2 = countEmployeesGreaterThanX(emp2, q2);
    assert(res2 == vector<int>({1,0,0}));

    // Test 3: IDs not sorted, duplicate salaries, negative threshold
    vector<pair<int,int>> emp3 = {{3, -5},{1, 0},{2, -5}};
    vector<tuple<int,int,int>> q3 = {{1,3,-10},{2,3,-5},{1,1,0}};
    vector<int> res3 = countEmployeesGreaterThanX(emp3, q3);
    // After sorting by ID: salaries = [0, -5, -5]
    // query [1,3] -10: all 3 > -10 => 3
    // query [2,3] -5: salaries -5,-5 > -5? no, 0 > -5? not in range => 0
    // query [1,1] 0: salary 0 > 0? no => 0
    assert(res3 == vector<int>({3,0,0}));

    // Test 4: range covers only some elements, x in between
    vector<pair<int,int>> emp4 = {{1, 3},{2, 7},{3, 1},{4, 9}};
    vector<tuple<int,int,int>> q4 = {{2,3,2},{1,4,5},{3,4,10}};
    vector<int> res4 = countEmployeesGreaterThanX(emp4, q4);
    // salaries: [3,7,1,9]
    // [2,3] >2: 7,1? only 7 >2 =>1
    // [1,4] >5: 7,9 =>2
    // [3,4] >10: none =>0
    assert(res4 == vector<int>({1,2,0}));

    // Test 5: large n and m to verify performance/fill
    int n = 1000;
    vector<pair<int,int>> emp5(n);
    for (int i = 0; i < n; ++i) emp5[i] = {i+1, i%10}; // salaries 0..9 repeating
    vector<tuple<int,int,int>> q5;
    for (int i = 0; i < n; ++i) {
        q5.push_back({1, n, 5}); // count >5: salaries 6,7,8,9 => 4 per 10 block => 400
    }
    vector<int> res5 = countEmployeesGreaterThanX(emp5, q5);
    assert(res5.size() == n);
    for (int v : res5) assert(v == 400);

    cout << "All tests passed!" << endl;
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

// Merge sort tree: each node stores sorted salaries in its interval.
class MergeSortTree {
    int n;
    vector<vector<int>> tree;

    // Build tree recursively: node covers [l, r)
    void build(const vector<int>& arr, int node, int l, int r) {
        if (r - l == 1) {
            tree[node].push_back(arr[l]);
            return;
        }
        int mid = (l + r) / 2;
        build(arr, 2 * node + 1, l, mid);
        build(arr, 2 * node + 2, mid, r);
        tree[node].resize(tree[2*node+1].size() + tree[2*node+2].size());
        merge(tree[2*node+1].begin(), tree[2*node+1].end(),
              tree[2*node+2].begin(), tree[2*node+2].end(),
              tree[node].begin());
    }

    // Query: count elements > x in range [ql, qr) => [ql, qr-1]
    int query(int node, int l, int r, int ql, int qr, int x) const {
        if (qr <= l || r <= ql) return 0;
        if (ql <= l && r <= qr) {
            const auto& v = tree[node];
            // number of elements > x = size - count of elements <= x
            return v.size() - (upper_bound(v.begin(), v.end(), x) - v.begin());
        }
        int mid = (l + r) / 2;
        return query(2*node+1, l, mid, ql, qr, x) +
               query(2*node+2, mid, r, ql, qr, x);
    }

public:
    MergeSortTree(const vector<int>& arr) {
        n = arr.size();
        tree.resize(4 * n);
        if (n > 0) build(arr, 0, 0, n);
    }

    int countGreater(int l, int r, int x) const { // [l, r] inclusive, 0-indexed
        if (l > r || n == 0) return 0;
        return query(0, 0, n, l, r + 1, x);
    }
};

std::vector<int> countEmployeesGreaterThanX(
    const vector<pair<int,int>>& employees,
    const vector<tuple<int,int,int>>& queries) {
    int n = employees.size();
    // Sort by ID (first element)
    vector<pair<int,int>> sorted = employees;
    sort(sorted.begin(), sorted.end()); // by ID ascending
    vector<int> salaries(n);
    for (int i = 0; i < n; ++i) {
        salaries[i] = sorted[i].second;
    }
    MergeSortTree mst(salaries);
    vector<int> result;
    result.reserve(queries.size());
    for (const auto& q : queries) {
        int l, r, x;
        tie(l, r, x) = q;
        --l; --r; // convert to 0-indexed
        result.push_back(mst.countGreater(l, r, x));
    }
    return result;
}

// The core challenge is answering multiple range-count queries on salaries with a threshold. A naive scan per query costs O(n) per query → O(nm) which is too slow. Instead, note that IDs are unique and range from 1 to n, so we can sort the employees by ID and extract their salaries in ID order. We then need to answer: for range `[l, r]` on this 0-indexed array, count how many elements are > x. This is a classic "range greater than value" query that can be solved with a **merge sort tree** (segment tree where each node stores a sorted list of salaries in its interval). Building the tree takes O(n log n) time and O(n log n) memory. For each query, we visit O(log n) nodes; in each node we use binary search (`upper_bound`) to count elements > x, costing O(log n) per node, so each query is O(log² n). Total: O(n log n + m log² n). Space: O(n log n). Edge cases: l and r are 1-indexed but need converting to 0-indexed; x can be negative or larger than all salaries; duplicate salaries are fine. The merge function should be iterative and efficient, avoiding repeated vector allocations where possible. Use `const` references and `const` correctness throughout.
