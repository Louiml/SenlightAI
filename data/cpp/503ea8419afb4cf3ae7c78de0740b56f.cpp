Given an initially empty array of `n` elements (where `n` can be as large as 10^18), process a sequence of operations: each operation is either (1) a point update, setting the value at a given index `idx` to a new value `val`, or (2) a range sum query, returning the sum of all elements whose indices lie in a given inclusive interval `[l, r]`. Indices are 1-based and values are 64-bit signed integers. The array starts with all zeros. Because `n` may be enormous, you must use a dynamic segment tree that only creates nodes when needed. Write a C++ function that, given the number of elements `n` and a list of operations (each operation is a tuple: type, a, b, c where type=1 means update index `a` to value `b`, and type=2 means query sum over `[a, b]`; for type=2, `c` is unused, pass 0), returns a vector of outputs (one for each query) in the order they appear.
The problem requires a dynamic segment tree because `n` is too large to allocate a static array of size `n`. The tree structure supports both point updates and range sum queries in \(O(\log n)\) time per operation, with space proportional to the number of updates (since each update creates at most \(O(\log n)\) new nodes, and the root is pre-created). The tree's range is `[1, n]`. For each update, we recursively descend to the leaf representing the index, create child nodes only if they don't exist, set the leaf's value, and then update parent sums via pushup. For each query, we traverse only the existing nodes, returning 0 for any child that hasn't been created. We must handle the case where `n` is not a power of two; the midpoint split works with `(l+r)/2` and `[l,m]` and `[m+1,r]`. Edge cases: query ranges that are partially or fully outside the tree's domain must correctly return 0 (the tree's domain is always `[1,n]`, and queries are guaranteed within it, but the recursion handles it anyway). Since values are 64-bit, sums can exceed 32-bit but fit in `long long`. Time complexity: \(O(k \log n)\) where `k` is the total number of operations. Space complexity: \(O(u \log n)\) where `u` is the number of updates, because each update creates at most `ceil(log2(n))` nodes. The solution function should encapsulate the entire segment tree, using a fixed-size node array large enough for worst-case (e.g., 4 * 10^6 nodes if at most 10^5 updates). We'll use a struct to hold the tree and provide a method for update and query.
#include <vector>
#include <cstdint>

struct DynamicSegmentTree {
    struct Node {
        int64_t val = 0;
        int left = 0;
        int right = 0;
    };

    std::vector<Node> nodes;
    int64_t maxIndex;

    // Constructor: reserve a reasonable size, and initialize root at index 1.
    DynamicSegmentTree(int64_t n, int maxUpdates) {
        maxIndex = n;
        // Each update can create at most (log2(maxIndex)+1) nodes, so reserve enough.
        nodes.reserve(2 + maxUpdates * 64);
        nodes.push_back(Node()); // dummy at index 0
        nodes.push_back(Node()); // root at index 1
    }

    // Point update: set value at position pos to newVal.
    void update(int64_t pos, int64_t newVal) {
        updateRec(1, 1, maxIndex, pos, newVal);
    }

    // Range sum query over [ql, qr].
    int64_t query(int64_t ql, int64_t qr) {
        return queryRec(1, 1, maxIndex, ql, qr);
    }

private:
    void updateRec(int cur, int64_t l, int64_t r, int64_t pos, int64_t newVal) {
        if (l == r) {
            nodes[cur].val = newVal;
            return;
        }
        int64_t m = (l + r) / 2;
        if (pos <= m) {
            if (nodes[cur].left == 0) {
                nodes.push_back(Node());
                nodes[cur].left = (int)nodes.size() - 1;
            }
            updateRec(nodes[cur].left, l, m, pos, newVal);
        } else {
            if (nodes[cur].right == 0) {
                nodes.push_back(Node());
                nodes[cur].right = (int)nodes.size() - 1;
            }
            updateRec(nodes[cur].right, m + 1, r, pos, newVal);
        }
        // pushup
        int64_t sum = 0;
        if (nodes[cur].left != 0) sum += nodes[nodes[cur].left].val;
        if (nodes[cur].right != 0) sum += nodes[nodes[cur].right].val;
        nodes[cur].val = sum;
    }

    int64_t queryRec(int cur, int64_t l, int64_t r, int64_t ql, int64_t qr) {
        if (cur == 0) return 0;
        if (ql <= l && r <= qr) return nodes[cur].val;
        if (qr < l || r < ql) return 0;
        int64_t m = (l + r) / 2;
        return queryRec(nodes[cur].left, l, m, ql, qr) +
               queryRec(nodes[cur].right, m + 1, r, ql, qr);
    }
};

// Solution function: processes operations and returns query results.
// operation format: {type, a, b, c}
// type=1: update index a to value b (c unused, pass 0)
// type=2: query sum over [a, b] (c unused, pass 0)
std::vector<int64_t> processOperations(int64_t n, const std::vector<std::vector<int64_t>>& ops) {
    // Estimate max updates: count of type 1 operations
    int maxUpdates = 0;
    for (const auto& op : ops) {
        if (op[0] == 1) maxUpdates++;
    }
    DynamicSegmentTree tree(n, maxUpdates);
    std::vector<int64_t> results;
    for (const auto& op : ops) {
        if (op[0] == 1) {
            tree.update(op[1], op[2]);
        } else { // type 2
            results.push_back(tree.query(op[1], op[2]));
        }
    }
    return results;
}
#include <cassert>
#include <vector>
#include <cstdint>

// The solution function is defined above. Add declaration here if needed.
std::vector<int64_t> processOperations(int64_t n, const std::vector<std::vector<int64_t>>& ops);

int main() {
    // Test 1: Basic updates and queries
    {
        std::vector<std::vector<int64_t>> ops = {
            {1, 1, 5, 0},   // set index 1 to 5
            {1, 3, 10, 0},  // set index 3 to 10
            {2, 1, 3, 0},   // sum [1,3] -> 15
            {2, 2, 2, 0},   // sum [2,2] -> 0
            {1, 2, 7, 0},   // set index 2 to 7
            {2, 1, 3, 0}    // sum [1,3] -> 22
        };
        auto res = processOperations(10, ops);
        assert(res.size() == 3);
        assert(res[0] == 15);
        assert(res[1] == 0);
        assert(res[2] == 22);
    }

    // Test 2: Large index (n = 10^18) with update near the end
    {
        int64_t n = 1000000000000000000LL;
        std::vector<std::vector<int64_t>> ops = {
            {1, n, 42, 0},   // set last index to 42
            {2, n, n, 0},    // sum [n,n] -> 42
            {2, 1, n-1, 0},  // sum [1,n-1] -> 0
            {2, 1, n, 0}     // sum [1,n] -> 42
        };
        auto res = processOperations(n, ops);
        assert(res.size() == 3);
        assert(res[0] == 42);
        assert(res[1] == 0);
        assert(res[2] == 42);
    }

    // Test 3: Overlapping updates and queries (negative values)
    {
        std::vector<std::vector<int64_t>> ops = {
            {1, 2, -5, 0},
            {1, 4, 8, 0},
            {2, 2, 4, 0},  // -5 + 0 + 8 = 3
            {1, 3, -10, 0},
            {2, 1, 5, 0},  // -5 + (-10) + 8 = -7
            {2, 5, 5, 0},  // 0
        };
        auto res = processOperations(5, ops);
        assert(res.size() == 3);
        assert(res[0] == 3);
        assert(res[1] == -7);
        assert(res[2] == 0);
    }

    // Test 4: Single update, multiple queries
    {
        std::vector<std::vector<int64_t>> ops = {
            {1, 1, 100, 0},
            {2, 1, 1, 0},
            {2, 2, 10, 0}
        };
        auto res = processOperations(10, ops);
        assert(res[0] == 100);
        assert(res[1] == 0);
    }

    // Test 5: No updates, only queries
    {
        std::vector<std::vector<int64_t>> ops = {
            {2, 3, 7, 0},
            {2, 1, 1, 0}
        };
        auto res = processOperations(100, ops);
        assert(res.size() == 2);
        assert(res[0] == 0);
        assert(res[1] == 0);
    }

    return 0;
}
