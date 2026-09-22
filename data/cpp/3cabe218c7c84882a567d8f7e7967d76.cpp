Given an array of positive integers (values up to 10^6) and a list of queries, each defined by a left index `l` and a right index `r` (1-indexed), write a C++ function `std::vector<int> solveQueries(int n, const std::vector<int>& arr, const std::vector<std::pair<int,int>>& queries)` that returns, for each query, the maximum value among the elements in the subarray from index `l` to `r` inclusive. The queries must be processed in non-decreasing order of `r`, and you must use a Fenwick tree (Binary Indexed Tree) where the tree is indexed in reverse order (i.e., index `i` in the tree corresponds to array position `n - i + 1`), and the tree stores prefix maximums during insertion. The function must handle up to `n = 25*10^5` and up to `m = 10^6` queries efficiently.
#include <cassert>
#include <vector>
#include <utility>

// Declaration of the solution function (already defined above).
std::vector<int> solveQueries(int n, const std::vector<int>& arr,
                              const std::vector<std::pair<int, int>>& queries);

int main() {
    // Basic test
    std::vector<int> arr1 = {3, 1, 4, 1, 5, 9, 2, 6};
    std::vector<std::pair<int, int>> q1 = {{1, 3}, {2, 5}, {4, 8}, {1, 1}, {8, 8}};
    std::vector<int> r1 = solveQueries(8, arr1, q1);
    assert(r1 == std::vector<int>({4, 5, 9, 3, 6}));

    // All same values
    std::vector<int> arr2 = {7, 7, 7, 7};
    std::vector<std::pair<int, int>> q2 = {{1, 4}, {2, 3}, {1, 2}};
    std::vector<int> r2 = solveQueries(4, arr2, q2);
    assert(r2 == std::vector<int>({7, 7, 7}));

    // Single element
    std::vector<int> arr3 = {42};
    std::vector<std::pair<int, int>> q3 = {{1, 1}};
    std::vector<int> r3 = solveQueries(1, arr3, q3);
    assert(r3 == std::vector<int>({42}));

    // Query order should not affect correctness (function sorts internally)
    std::vector<std::pair<int, int>> q4 = {{3, 4}, {1, 2}, {2, 3}};
    std::vector<int> r4 = solveQueries(4, {1, 2, 3, 4}, q4);
    assert(r4 == std::vector<int>({4, 2, 3}));

    // Larger test with duplicate max values
    std::vector<int> arr5 = {5, 2, 5, 3, 5};
    std::vector<std::pair<int, int>> q5 = {{1, 5}, {2, 4}, {3, 5}, {1, 1}};
    std::vector<int> r5 = solveQueries(5, arr5, q5);
    assert(r5 == std::vector<int>({5, 3, 5, 5}));

    return 0;
}
#include <vector>
#include <algorithm>

// Fenwick tree for prefix maximums, 1-indexed internally.
class FenwickMax {
private:
    int size;
    std::vector<int> bit;
public:
    FenwickMax(int n) : size(n), bit(n + 1, 0) {}

    // Update: set bit[i] = max(bit[i], val) for i = pos, pos += lowbit(pos)
    void update(int pos, int val) {
        for (int i = pos; i <= size; i += i & (-i)) {
            bit[i] = std::max(bit[i], val);
        }
    }

    // Query: maximum over [1, pos]
    int query(int pos) const {
        int res = 0;
        for (int i = pos; i > 0; i -= i & (-i)) {
            res = std::max(res, bit[i]);
        }
        return res;
    }
};

// Solve range maximum queries on `arr` (0-indexed), queries given as 1-indexed pairs (l, r).
std::vector<int> solveQueries(int n, const std::vector<int>& arr,
                              const std::vector<std::pair<int, int>>& queries) {
    int m = queries.size();
    std::vector<int> ans(m);

    // Sort queries by right endpoint (r).
    std::vector<int> order(m);
    for (int i = 0; i < m; ++i) order[i] = i;
    std::sort(order.begin(), order.end(),
              [&](int a, int b) { return queries[a].second < queries[b].second; });

    FenwickMax bit(n);
    int idx = 1; // current array position to insert (1-indexed)

    for (int i = 0; i < m; ++i) {
        int qIdx = order[i];
        int l = queries[qIdx].first;
        int r = queries[qIdx].second;

        // Insert all elements up to index r (1-indexed) into the reversed tree.
        while (idx <= r) {
            bit.update(n - idx + 1, arr[idx - 1]); // arr[0] corresponds to idx=1
            ++idx;
        }

        // Query the maximum from positions >= l up to r (in reversed indices).
        ans[qIdx] = bit.query(n - l + 1);
    }

    return ans;
}
// The problem is a static range maximum query (RMQ) with a special constraint: queries are sorted by right endpoint. We process queries in increasing order of `r`. As we sweep `r` from 1 to `n`, we insert each array element at the position `n - r + 1` in a Fenwick tree that supports prefix-maximum updates and prefix-maximum queries. Because the Fenwick tree is reversed, inserting at position `n - r + 1` means that when we want the maximum over `[l, r]`, we query prefix `n - l + 1` (which covers all positions from `n - r + 1` up to `n - l + 1`) and get the maximum of all elements inserted so far (i.e., with index ≤ current `r`) that are ≥ `l`. The Fenwick tree’s `update` sets `tree[i] = max(tree[i], value)` for indices moving upward; the `query` reads maximums moving downward. This works because we only need prefix maximums, not full sum queries. The time complexity is O((n + m) log n) overall, dominated by the Fenwick operations. Space is O(n + m). Edge cases: ensure 1-indexed conversion from 0-indexed arrays; when `l == r`, the answer is simply `arr[l-1]` – the algorithm still works because we query after inserting that element. Also, note that values are positive, but the algorithm works for any comparable type.
