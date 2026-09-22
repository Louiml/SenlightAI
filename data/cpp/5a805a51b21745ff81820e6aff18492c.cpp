Design a C++ function that processes an array of 64-bit integers and answers two types of queries: (1) compute the sum of all elements in a given inclusive range `[L, R]`, and (2) add a given value `X` to every element in that range. The function should take the initial array, the number of queries, and a list of queries (each encoded as either `{type, L, R}` for sum queries or `{type, L, R, X}` for update queries) and return a vector of the answers to all sum queries in the order they appear. The array indices are 0-based, ranges are inclusive, and all values (including `X`) are 64-bit integers that may be negative. The function must be efficient for up to 100,000 elements and 100,000 queries, using a segment tree with lazy propagation. Assume the input is always valid (e.g., `L <= R`, ranges within bounds).
The core solution is a segment tree with lazy propagation that supports range addition and range sum queries in `O(log n)` each. The tree is built over the input array, storing at each node the sum of its segment. A separate "lazy" array stores pending updates that have not yet been pushed to children. When performing an update or query, before recursing or reading a node's value, we first propagate any pending lazy value: we add the lazy value multiplied by the segment length to the node's sum, then push the lazy value to both children, and clear the node's own lazy. For a full cover of the query range, we apply the update directly to the node and set lazy for its children (if internal). For a partial cover, we recurse into children and then recalculate the parent's sum from the children. A sum query similarly propagates lazy and returns the node value for a full cover; otherwise, it combines the results from children. Edge cases include empty ranges (should never occur per constraints), single-element arrays, and negative values. Time complexity is `O(n)` for building and `O(q log n)` for all queries; space complexity is `O(n)` for the tree and lazy arrays (using the standard `4*n` sizing to avoid overflow of indices).
#include <vector>
#include <cstdint>

// Process range sum and range add queries on a 64-bit integer array.
// Queries are encoded as: {type, L, R} for type==1 (sum), or {type, L, R, X} for type==2 (add).
// Returns the answers to all sum queries in order.
std::vector<int64_t> processRangeQueries(
    const std::vector<int64_t>& initial,
    const std::vector<std::vector<int64_t>>& queries) {
    
    int64_t n = static_cast<int64_t>(initial.size());
    if (n == 0) return {};
    
    std::vector<int64_t> tree(4 * n, 0);
    std::vector<int64_t> lazy(4 * n, 0);
    
    // Build the segment tree.
    auto build = [&](auto&& self, int64_t node, int64_t l, int64_t r) -> void {
        if (l == r) {
            tree[node] = initial[l];
            return;
        }
        int64_t mid = (l + r) / 2;
        self(self, 2 * node + 1, l, mid);
        self(self, 2 * node + 2, mid + 1, r);
        tree[node] = tree[2 * node + 1] + tree[2 * node + 2];
    };
    build(build, 0, 0, n - 1);
    
    // Push lazy updates to children.
    auto push = [&](int64_t node, int64_t l, int64_t r) -> void {
        if (lazy[node] != 0) {
            tree[node] += (r - l + 1) * lazy[node];
            if (l != r) {
                lazy[2 * node + 1] += lazy[node];
                lazy[2 * node + 2] += lazy[node];
            }
            lazy[node] = 0;
        }
    };
    
    // Range add: add `val` to all elements in [ql, qr].
    auto update = [&](auto&& self, int64_t node, int64_t l, int64_t r, int64_t ql, int64_t qr, int64_t val) -> void {
        push(node, l, r);
        if (qr < l || ql > r) return;
        if (ql <= l && r <= qr) {
            tree[node] += (r - l + 1) * val;
            if (l != r) {
                lazy[2 * node + 1] += val;
                lazy[2 * node + 2] += val;
            }
            return;
        }
        int64_t mid = (l + r) / 2;
        self(self, 2 * node + 1, l, mid, ql, qr, val);
        self(self, 2 * node + 2, mid + 1, r, ql, qr, val);
        tree[node] = tree[2 * node + 1] + tree[2 * node + 2];
    };
    
    // Range sum query: return sum of [ql, qr].
    auto query = [&](auto&& self, int64_t node, int64_t l, int64_t r, int64_t ql, int64_t qr) -> int64_t {
        push(node, l, r);
        if (qr < l || ql > r) return 0;
        if (ql <= l && r <= qr) return tree[node];
        int64_t mid = (l + r) / 2;
        return self(self, 2 * node + 1, l, mid, ql, qr) +
               self(self, 2 * node + 2, mid + 1, r, ql, qr);
    };
    
    std::vector<int64_t> answers;
    for (const auto& q : queries) {
        int64_t type = q[0];
        int64_t L = q[1];
        int64_t R = q[2];
        if (type == 1) {
            answers.push_back(query(query, 0, 0, n - 1, L, R));
        } else {
            int64_t X = q[3];
            update(update, 0, 0, n - 1, L, R, X);
        }
    }
    return answers;
}
#include <cassert>
#include <vector>
#include <cstdint>

// The solution function is declared above (or included from elsewhere).
// For testing, include the function here.

int main() {
    // Basic test: no updates, just sum queries.
    {
        std::vector<int64_t> arr = {1, 2, 3, 4, 5};
        std::vector<std::vector<int64_t>> queries = {{1, 0, 4}, {1, 1, 3}, {1, 2, 2}};
        auto res = processRangeQueries(arr, queries);
        assert(res.size() == 3);
        assert(res[0] == 15);
        assert(res[1] == 9);
        assert(res[2] == 3);
    }
    
    // Single update covering whole range.
    {
        std::vector<int64_t> arr = {10, 20, 30};
        std::vector<std::vector<int64_t>> queries = {{2, 0, 2, 5}, {1, 0, 2}};
        auto res = processRangeQueries(arr, queries);
        assert(res.size() == 1);
        assert(res[0] == 75);
    }
    
    // Partial updates and multiple queries.
    {
        std::vector<int64_t> arr = {1, 1, 1, 1, 1};
        std::vector<std::vector<int64_t>> queries = {
            {2, 1, 3, 10},  // add 10 to indices 1..3 => [1,11,11,11,1]
            {1, 0, 4},      // sum = 35
            {2, 2, 4, -1}, // add -1 to indices 2..4 => [1,11,10,10,0]
            {1, 2, 4},      // sum = 20
            {1, 0, 0}       // sum = 1
        };
        auto res = processRangeQueries(arr, queries);
        assert(res.size() == 3);
        assert(res[0] == 35);
        assert(res[1] == 20);
        assert(res[2] == 1);
    }
    
    // Negative values and negative updates.
    {
        std::vector<int64_t> arr = {-5, -10, -15, -20};
        std::vector<std::vector<int64_t>> queries = {
            {1, 0, 3},      // sum = -50
            {2, 1, 2, -3}, // add -3 => [-5,-13,-18,-20]
            {1, 1, 2},      // sum = -31
            {2, 0, 0, 100}, // add 100 => [95,-13,-18,-20]
            {1, 0, 3}       // sum = 44
        };
        auto res = processRangeQueries(arr, queries);
        assert(res.size() == 3);
        assert(res[0] == -50);
        assert(res[1] == -31);
        assert(res[2] == 44);
    }
    
    // Empty array.
    {
        std::vector<int64_t> arr;
        std::vector<std::vector<int64_t>> queries;
        auto res = processRangeQueries(arr, queries);
        assert(res.empty());
    }
    
    // Single element array.
    {
        std::vector<int64_t> arr = {42};
        std::vector<std::vector<int64_t>> queries = {{1, 0, 0}, {2, 0, 0, 8}, {1, 0, 0}};
        auto res = processRangeQueries(arr, queries);
        assert(res.size() == 2);
        assert(res[0] == 42);
        assert(res[1] == 50);
    }
    
    return 0;
}
