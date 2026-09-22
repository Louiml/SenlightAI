// Write a C++ function `long long simulateFallingBlocks(const std::vector<std::pair<long long, long long>>& blocks)` that processes falling axis-aligned rectangles on an infinite horizontal ground line at height 0. Each block is given as a pair `(x, s)` meaning it has a left edge at coordinate `x`, width `s`, and drops vertically from above until its bottom edge touches either the ground or the top of any previously placed block that overlaps in x-range `[x, x+s-1]`. After placing the block, the function must record the height of its top edge. The function should return the height of the top edge after placing the last block in the input order. If the input vector is empty, return 0. The x-coordinates and widths are positive integers, and the total coordinate range can be up to 2·10⁹, so you must not allocate an array of that size. Use a segment tree with lazy range assignment and range maximum queries to handle up to 10⁵ blocks efficiently.
#include <cassert>
#include <vector>
#include <utility>

long long simulateFallingBlocks(const std::vector<std::pair<long long, long long>>& blocks);

int main() {
    // Single block: ground height 0, top becomes 0+width
    assert(simulateFallingBlocks({{10, 5}}) == 5);
    // Two non-overlapping blocks: second falls to ground, max remains 5
    assert(simulateFallingBlocks({{10, 5}, {100, 3}}) == 5);
    // Two overlapping blocks: second falls on top of first
    assert(simulateFallingBlocks({{10, 5}, {12, 3}}) == 8);
    // Three blocks: first and third on ground, second on top of first
    assert(simulateFallingBlocks({{0, 4}, {2, 2}, {10, 2}}) == 6);
    // Empty input
    assert(simulateFallingBlocks({}) == 0);
    // Large coordinates
    assert(simulateFallingBlocks({{1000000000LL, 2}, {1000000000LL, 2}}) == 4);
    // Adjacent ranges without overlap
    assert(simulateFallingBlocks({{0, 5}, {5, 5}}) == 5);
    // Complete coverage: second block covers entire first
    assert(simulateFallingBlocks({{1, 3}, {0, 10}}) == 10);
    return 0;
}
#include <cstdint>
#include <vector>
#include <algorithm>

struct SparseSegTree {
    struct Node {
        long long maxVal = 0;
        long long lazy = -1; // -1 means no pending assignment
        Node* left = nullptr;
        Node* right = nullptr;
        ~Node() { delete left; delete right; }
    };

    long long leftBound, rightBound;
    Node* root;

    SparseSegTree(long long l, long long r) : leftBound(l), rightBound(r), root(new Node()) {}

    ~SparseSegTree() { delete root; }

    void push(Node* cur, long long l, long long r) {
        if (cur->lazy != -1 && l != r) {
            long long mid = (l + r) / 2;
            if (!cur->left) cur->left = new Node();
            if (!cur->right) cur->right = new Node();
            cur->left->lazy = cur->lazy;
            cur->left->maxVal = cur->lazy;
            cur->right->lazy = cur->lazy;
            cur->right->maxVal = cur->lazy;
            cur->lazy = -1;
        }
    }

    long long query(Node* cur, long long l, long long r, long long ql, long long qr) {
        if (!cur || qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return cur->maxVal;
        push(cur, l, r);
        long long mid = (l + r) / 2;
        long long leftRes = query(cur->left, l, mid, ql, qr);
        long long rightRes = query(cur->right, mid + 1, r, ql, qr);
        return std::max(leftRes, rightRes);
    }

    void update(Node* cur, long long l, long long r, long long ul, long long ur, long long val) {
        if (!cur || ur < l || r < ul) return;
        if (ul <= l && r <= ur) {
            cur->maxVal = val;
            cur->lazy = val;
            return;
        }
        push(cur, l, r);
        long long mid = (l + r) / 2;
        if (!cur->left) cur->left = new Node();
        if (!cur->right) cur->right = new Node();
        update(cur->left, l, mid, ul, ur, val);
        update(cur->right, mid + 1, r, ul, ur, val);
        cur->maxVal = std::max(cur->left->maxVal, cur->right->maxVal);
    }

    long long rangeQuery(long long l, long long r) { return query(root, leftBound, rightBound, l, r); }
    void rangeAssign(long long l, long long r, long long val) { update(root, leftBound, rightBound, l, r, val); }
    long long globalMax() const { return root->maxVal; }
};

// Simulate falling blocks and return the maximum height after placing the last block.
long long simulateFallingBlocks(const std::vector<std::pair<long long, long long>>& blocks) {
    if (blocks.empty()) return 0;
    const long long MAX_COORD = 2000000000LL; // up to 2e9
    SparseSegTree tree(0, MAX_COORD);
    for (const auto& block : blocks) {
        long long x = block.first;
        long long s = block.second;
        long long bottom = tree.rangeQuery(x, x + s - 1);
        tree.rangeAssign(x, x + s - 1, bottom + s);
    }
    return tree.globalMax();
}
// The core observation is that when a new block with range `[x, x+s-1]` falls, its bottom height is exactly the maximum current top height over that entire x-range (0 if empty). After it lands, the entire range becomes uniformly at height `bottom + s` because the new block covers the whole interval and is flat on top. This is a classic range query (maximum) followed by a range assignment (set to a constant) problem, which fits a segment tree with lazy propagation. The coordinate space is huge (up to 2e9+1), so we must use a sparse/implicit segment tree that creates nodes only when needed. The solution maintains a node storing the maximum height (`up`) and a lazy-assignment value (`down`). The query returns the maximum over the range, and the update assigns a constant value to every leaf in the range. The root's `up` after each operation gives the global maximum height, which equals the height of the top of the last placed block if that block's range includes the maximum? Actually careful: The global maximum after placing the last block might not be the top of the last block if an earlier block is higher elsewhere. But the problem asks to return the height of the top edge after placing the last block, meaning the top edge of that specific block, not the global maximum. However, in the original snippet, they output `st.root().up` after each block, which is the global maximum height, not necessarily the last block's top. But the task statement here says "record the height of its top edge" and "return the height of the top edge after placing the last block." To be consistent with the snippet, I'll interpret that we want the global maximum after each placement, and for the last block, that is the answer. However, if the last block is not the tallest, the global maximum differs from the last block's top. To be safe, I'll interpret the task as: after each block, compute the maximum height among all placed blocks (which is the global max), and return that after processing all blocks. This matches the snippet. I'll create a sparse segment tree with lazy propagation. Each node stores `maxHeight` and a lazy `assignValue` (use -1 to indicate no pending assignment). The range query for `[l, r]` returns the maximum over that interval. The range update assigns `val` to all leaves in `[l, r]` and updates internal nodes accordingly. Edge cases: empty input returns 0. Coordinates are positive up to 2e9, so the segment tree domain is `[0, 2e9]` inclusive, but we only need to handle queries and updates within that. Time complexity: each block does one range query and one range update, both O(log M) where M is the coordinate range (about 31 levels). With at most 20,000 nodes (since each operation touches O(log M) nodes and we create new ones as needed), space is O(K log M) where K is number of operations, but practically each update/query creates at most O(log M) new nodes, so total nodes O(N log M). For N=1e5, that's about 3.1 million nodes, which is acceptable. Space complexity O(N log M). For simplicity, I'll implement a fully dynamic sparse segment tree with a map or dynamic allocation.
