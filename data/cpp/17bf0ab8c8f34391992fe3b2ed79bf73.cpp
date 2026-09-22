You are given a grid with n rows and m columns. Initially, all cells are unmarked. You will process q operations, each of which toggles the state of a cell (row r, column c). After each toggle, you must determine whether it is possible to place non-attacking rooks such that each marked cell is covered by exactly one rook, and each rook occupies a cell that is either marked or unmarked. Specifically, a configuration is valid if there exists a permutation of columns such that for every row i, we assign a column p[i], and for each row, if there is any marked cell in that row, then the rook in that row must be placed in the leftmost marked cell of that row (i.e., the rook's column equals the minimum column among all marked cells in that row). If there are no marked cells in a row, the rook can be any column. For rows with marked cells, the rook's column must be exactly the minimum marked column. Two rows cannot share the same column. The grid has columns indexed from 1 to m. After each toggle, output "YES" if such a placement exists, else "NO". Note: The rows are numbered from 1 to n. Each operation toggles a single cell; if the cell was unmarked, it becomes marked, and vice versa. Your task is to process all queries and output the result after each toggle. Constraints: 1 ≤ n, m, q ≤ 10^5.
The problem reduces to a classic matching condition: each row i that has at least one marked cell imposes a requirement that we must assign it the column equal to the minimum column among its marked cells. Let L_i = min column of marked cells in row i (if any). Rows with no marked cells are "free" and can take any remaining columns. We need to check if there exists an assignment where each row i (with marked cells) gets column L_i, and all these assigned columns are distinct. This is equivalent to checking that for every row, the intervals [L_i, R_i]? Actually, we need to check that the set of required columns can be matched injectively to rows. A known condition: If we sort rows by their L_i, then for each k, the number of rows with L_i ≤ k must be ≤ k. This is Hall's theorem for intervals [L_i, ∞) because each row requires a column ≥ L_i. But rows have no upper bound. So we need to ensure that for every prefix of columns, the count of required rows whose L_i is in that prefix does not exceed the prefix size. That is exactly the condition: for all k, number of rows with L_i ≤ k ≤ k. We can maintain this with a segment tree. For each row i, define a node with maxL = L_i, minR = something? Actually, the condition that all L_i are distinct and each L_i ≤ m and count condition can be checked by: For each row i, we have an interval [L_i, ∞). We need to assign distinct columns. This is possible iff for every prefix of columns, the number of rows requiring columns in that prefix is ≤ prefix length. Equivalently, if we sort L_i, then L_i ≥ i for all i. This is same as checking that after sorting, the i-th smallest L is ≥ i. We can maintain a segment tree over rows where each leaf stores L_i (or INF if no marked cells). The tree supports point updates and interval queries for the "bad" condition: whether there exists a prefix where count exceeds length. The code snippet uses a segment tree where each node stores maxL and minR and bad flag. Actually, the snippet uses onlyL and onlyR sets to compute maxL = max column among marked cells in a row? Wait, the snippet flips cells and updates row r/2. The original code is for a different problem (probably Codeforces problem F). But we adapt the idea: We need to maintain for each row the minimum marked column (call it L_i). The validity condition is that the set of L_i (for rows with at least one marked cell) can be assigned one per row, with distinct columns. This holds iff after sorting L_i, we have L_i ≥ i. We can maintain this using a segment tree that supports range maximum of (L_i - row_index?) Actually, let's think: If we have rows indexed 1..n. For each row i, if it has marked cells, we define need[i] = L_i. Else need[i] = INF (meaning no constraint). We need to check that we can select a distinct column for each row with a constraint, such that the column is exactly need[i]. That is possible iff all need[i] are distinct and each need[i] ≤ m. But due to the "free" rows, we can always fill gaps. So condition reduces to: For all k from 1 to n, the number of rows i with need[i] ≤ k must be ≤ k. This is equivalent to: For each prefix of rows sorted by need, the i-th smallest need must be ≥ i. That is, if we sort the need values (including INF for free rows), then for each position p (1-based) in that sorted list, need_sorted[p] ≥ p. But we can also check this by scanning through rows in index order? Actually, the typical solution for this problem (Codeforces 1498F? Not sure) uses a segment tree over columns. Let me design a simpler approach: For each row, we can compute L_i (min marked column). The condition is that the set of L_i values (for rows with at least one marked) can be assigned to distinct columns. Since we can use any free row to take any column, the only obstruction is if two rows require the same column, or if a required column is larger than the number of rows with required column ≤ that column. Formally, define an array A[1..m] where A[c] = number of rows with L_i = c. Then we need that for all c, the cumulative sum from 1 to c is ≤ c. That is equivalent to: for all c, sum_{j=1}^c A[j] ≤ c. Let prefix[c] = sum_{j=1}^c A[j]. Then condition is prefix[c] ≤ c for all c. We can maintain A with point updates (when a cell toggles, the L_i of its row may change, so we remove old contribution and add new one). We can use a segment tree that stores for each column segment the maximum of (prefix[x] - x). If this maximum is > 0, then condition fails. Actually, we need to maintain prefix array dynamically. When a row's L_i changes from old to new, we decrement A[old] and increment A[new] (if old/new exist). This affects prefix for all columns ≥ old and ≥ new. So we need range add on prefix. Then we query global maximum of (prefix[x] - x). If max ≤ 0, then YES. This can be done with a lazy segment tree over columns 1..m. For each column c, store value = prefix[c] - c. When we add delta to A[x], we add delta to all columns c ≥ x. So we do range add on [x, m] by delta. Then the condition is max over all c of value ≤ 0. Complexity O(log m) per query. Edge cases: rows with no marked cells contribute nothing (no requirement). When a row has multiple marked cells, L_i is the minimum column among them. We need to maintain for each row a set of marked columns to find the minimum quickly. Also, when a cell toggles, only the row containing it may change its L_i. So we maintain for each row a set (or multiset) of marked column indices. The row's L_i is the smallest element in that set, or INF if empty. Then we update A[old] -= 1 and A[new] += 1, and apply range adds. Initially all A are 0. We must also handle the case where a row has a new L_i equal to old (if toggling a non-minimum cell) – then no change. Also, columns are 0-indexed in code? We'll use 1-indexed for clarity. We also need to consider that m can be up to 1e5, n up to 1e5, q up to 1e5. This solution is O((n+m+q) log m) time, O(n+m) space.

However, the snippet uses a segment tree over rows, not columns. The snippet's approach is different: it maintains for each row the maximum L and minimum R? Actually, the snippet has onlyL and onlyR sets per row, and then computes maxL = max element in onlyL +1? That seems like a different problem. But we can adapt the snippet's structure to our problem: Instead of columns, we can use a segment tree over rows where each node stores the maximum of (L_i - i) maybe? Let me find a simpler way to explain in the Analysis. I'll present the column-prefix segment tree approach because it is clear and correct. But the test code expects a single function? The task says "write a C++ function that ..." So I'll create a function `std::string processGrid(int n, int m, const std::vector<std::pair<int,int>>& queries)` that returns a string of "YES"/"NO" separated by newlines. Or better, a function that takes queries and returns a vector<string>. But the test section calls the solution function directly. I'll design a function `std::vector<std::string> solveGrid(int n, int m, const std::vector<std::pair<int,int>>& queries)`.

Let me write the solution using a lazy segment tree for range add and global max. We maintain arrays colCount of size m+1 (1-indexed). For each row, maintain a set<int> markedCols. For each row, we compute its current minimal col (or INF). When we toggle a cell (r,c): (r,c) 1-indexed. If marked, remove from set; else insert. Let oldMin = previous min (before toggle), newMin = new min. If oldMin != newMin, then if oldMin != INF, colCount[oldMin]--; if newMin != INF, colCount[newMin]++. Then we need to apply range updates: for each change in colCount at position p with delta d, we add d to all columns j >= p in the segment tree (since prefix[j] changes by d for all j >= p). Actually, if we change colCount[p] by delta, then prefix[j] for j >= p increases by delta. So we do rangeAdd(p, m, delta). Then we check if global max of (prefix[j]-j) <= 0. If yes, output YES else NO. Initial prefix all zero, so all prefix[j]-j = -j, max negative. Note: We must consider columns 1..m. If a row has no marked cells, its L_i = INF, we ignore it.

Edge case: When a row has its minimum column > m? No, columns are 1..m. Also, if a row's min is INF, we don't contribute to colCount. If toggling a cell that is not the minimum, oldMin==newMin, no change.

We need to be careful with 1-indexing. I'll implement a segment tree class with lazy propagation.

Complexity: O((q + n) log m). Space O(n + m).

Let me write the solution.
#include <bits/stdc++.h>
using namespace std;

// Segment tree with range add and global maximum query.
class SegTree {
    int n;
    vector<int> tree, lazy;
public:
    SegTree(int _n) : n(_n) {
        tree.resize(4 * n + 5, 0);
        lazy.resize(4 * n + 5, 0);
    }
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
    void range_add(int node, int l, int r, int ql, int qr, int val) {
        if (ql > r || qr < l) return;
        if (ql <= l && r <= qr) {
            apply(node, val);
            return;
        }
        push(node);
        int mid = (l + r) / 2;
        range_add(node * 2, l, mid, ql, qr, val);
        range_add(node * 2 + 1, mid + 1, r, ql, qr, val);
        tree[node] = max(tree[node * 2], tree[node * 2 + 1]);
    }
    void range_add(int ql, int qr, int val) {
        if (ql > qr) return;
        range_add(1, 1, n, ql, qr, val);
    }
    int get_max() {
        return tree[1];
    }
};

vector<string> solveGrid(int n, int m, const vector<pair<int,int>>& queries) {
    const int INF = 1e9;
    // For each row, maintain a set of marked columns.
    vector<set<int>> rowMarks(n + 1);
    // colCount[c] = number of rows whose minimal marked column is c.
    vector<int> colCount(m + 2, 0); // 1..m
    SegTree seg(m);
    // Initially all prefix zero, so prefix[j]-j = -j, max is -1 (since j>=1)
    // The seg tree stores values prefix[j] - j for each j.
    // Initially prefix all 0, so tree[j] = -j. We need to initialize that.
    // But instead of initializing, we can just subtract j during building? Simpler: we can start seg with all zeros, and then we maintain the condition max(prefix[j]-j) <= 0.
    // To avoid complex init, we can store prefix[j] in seg and then compare with j? But we want max of (prefix - j). We can store directly prefix, then compute max(prefix[j]-j) by iterating? Not O(log). So better to initialize seg with values -j for all j.
    // Build initial tree with -j at index j.
    // But we can just add -j for all j initially? We'll do a loop.
    for (int j = 1; j <= m; ++j) {
        seg.range_add(j, j, -j);
    }

    auto get_min_col = [&](int row) -> int {
        if (rowMarks[row].empty()) return INF;
        return *rowMarks[row].begin();
    };

    vector<string> ans;
    ans.reserve(queries.size());

    for (auto [r, c] : queries) {
        // r,c are 1-indexed as given
        int oldMin = get_min_col(r);
        if (rowMarks[r].find(c) != rowMarks[r].end()) {
            rowMarks[r].erase(c);
        } else {
            rowMarks[r].insert(c);
        }
        int newMin = get_min_col(r);
        if (oldMin != newMin) {
            if (oldMin != INF) {
                colCount[oldMin]--;
                seg.range_add(oldMin, m, -1); // prefix[j] for j>=oldMin decreases by 1
            }
            if (newMin != INF) {
                colCount[newMin]++;
                seg.range_add(newMin, m, 1); // prefix[j] for j>=newMin increases by 1
            }
        }
        if (seg.get_max() <= 0) {
            ans.push_back("YES");
        } else {
            ans.push_back("NO");
        }
    }
    return ans;
}
#include <cassert>
#include <vector>
#include <string>
#include <utility>
using namespace std;

// Include the solution function here (or assume it's defined above)
// For completeness, we copy the solution code here, but in actual test it would be linked.

// ... (solution code from above)

int main() {
    // Test 1: Simple case
    {
        int n = 2, m = 2;
        vector<pair<int,int>> queries = {{1,1},{1,2},{2,1},{2,2}};
        vector<string> res = solveGrid(n, m, queries);
        vector<string> expected = {"YES","YES","YES","NO"};
        assert(res == expected);
    }
    // Test 2: Single row, single column
    {
        int n = 1, m = 1;
        vector<pair<int,int>> queries = {{1,1}};
        vector<string> res = solveGrid(n, m, queries);
        assert(res == vector<string>({"YES"}));
    }
    // Test 3: Two rows same column conflict
    {
        int n = 2, m = 2;
        // Row1 mark col1, row2 mark col1 -> conflict
        vector<pair<int,int>> queries = {{1,1},{2,1}};
        vector<string> res = solveGrid(n, m, queries);
        vector<string> expected = {"YES","NO"}; // after first yes, second makes two rows require col1 -> NO
        assert(res == expected);
    }
    // Test 4: More columns than rows, always possible?
    {
        int n = 2, m = 5;
        vector<pair<int,int>> queries = {{1,1},{2,2},{1,1}}; // toggle 1,1 on, 2,2 on, then 1,1 off
        vector<string> res = solveGrid(n, m, queries);
        vector<string> expected = {"YES","YES","YES"};
        assert(res == expected);
    }
    // Test 5: Edge case: row with multiple marks, toggling non-min doesn't change
    {
        int n = 1, m = 3;
        vector<pair<int,int>> queries = {{1,2},{1,1},{1,3},{1,1}}; // mark 2, mark 1, mark 3, unmark 1
        vector<string> res = solveGrid(n, m, queries);
        // After mark2: L=2 -> possible (only row, col2 free) YES
        // After mark1: L becomes 1 -> YES
        // After mark3: L still 1 -> YES
        // After unmark1: L becomes 2 -> YES
        vector<string> expected = {"YES","YES","YES","YES"};
        assert(res == expected);
    }
    // Test 6: Larger grid with conflict after toggling
    {
        int n = 3, m = 3;
        vector<pair<int,int>> queries = {{1,1},{2,1},{3,1}}; // three rows all mark col1
        vector<string> res = solveGrid(n, m, queries);
        vector<string> expected = {"YES","NO","NO"}; // after third, three rows require col1 -> impossible
        assert(res == expected);
    }
    // Test 7: No queries
    {
        int n = 1, m = 1;
        vector<pair<int,int>> queries;
        vector<string> res = solveGrid(n, m, queries);
        assert(res.empty());
    }
    // Test 8: Column overflow check: row requires col > m? impossible by constraints
    // Test 9: Random small test (manual)
    {
        int n = 2, m = 3;
        vector<pair<int,int>> queries = {{1,3},{2,2},{2,2}}; // mark (1,3), mark (2,2), unmark (2,2)
        vector<string> res = solveGrid(n, m, queries);
        vector<string> expected = {"YES","YES","YES"}; // row1 L=3, row2 L=2 -> distinct, yes
        assert(res == expected);
    }
    // Test 10: Check that when all rows have distinct L_i it's always yes
    {
        int n = 3, m = 4;
        vector<pair<int,int>> queries = {{1,2},{2,1},{3,3}};
        vector<string> res = solveGrid(n, m, queries);
        vector<string> expected = {"YES","YES","YES"};
        assert(res == expected);
    }
    return 0;
}
