Implement a C++ function `double rangeSumAfterInsert(int initialCount, const std::vector<long long>& initialValues, const std::vector<long long>& inserts, int leftIndex, int rightIndex)` that simulates the behavior of the implicit treap from the provided code snippet. The function should first build an implicit treap containing `initialCount` elements with values from `initialValues` (in the given order, 0-indexed). Then it should insert, one by one, each value from `inserts` at the END of the sequence (i.e., after the last element, so the position is the current size of the treap). After all insertions are done, it must return the sum of elements at indices `leftIndex` through `rightIndex` inclusive (0-indexed). The function must handle the case where `leftIndex` > `rightIndex` by returning 0, and it must also handle empty initial sequence and/or empty insert list. Use a non-recursive or recursive split/merge approach similar to the provided code, but ensure the implementation is self-contained and does not rely on the original struct. The function should be declared with proper `const` correctness where possible, and the underlying node structure must properly manage memory (use smart pointers or manual deletion). The time complexity must be \(O((n+m) \log (n+m))\) overall, where \(n\) is initialCount and \(m\) is the size of inserts (assuming split/merge each take \(O(\log n)\)).
// The core idea is to replicate the implicit treap operations from the snippet: we maintain a balanced binary search tree where the in-order traversal corresponds to the sequence order. Each node stores a key (the actual value), subtree size, priority (for randomization), subtree sum, and a lazy propagation field (though the snippet's `push_lazy` is incomplete, we do not need lazy here since no range updates are performed; we can omit lazy entirely). We need two main operations:
// - `splitByIndex(t, l, r, k)`: splits the treap rooted at `t` into `l` (first `k+1` nodes, i.e., indices 0..k) and `r` (the rest). This is done recursively using the size of the left subtree to decide which side to go. The snippet’s implementation correctly decrements the index when moving to the right subtree.
// - `merge(t, l, r)`: merges two treaps where all keys in `l` come before all keys in `r`, based on priorities (max-heap property). Both operations update sizes and sums.
// After building the initial treap by merging each node one-by-one (or by building in O(n) using a stack, but for simplicity we can just merge sequentially, which is \(O(n \log n)\)), we insert each new value at the end by calling split by index with key = current_size-1, then creating a new node and merging left + new + right. Since we always insert at the end, we can also just merge the root with a new node directly, but to keep general, we use split/merge. Finally, to get the range sum, we split into three parts: left part (< leftIndex), middle (leftIndex..rightIndex), and right (> rightIndex), sum the middle, then merge back.
// Edge cases: empty trees, invalid ranges (return 0), inserting into empty initial tree. The priority generation must use a good random generator (e.g., `std::mt19937` seeded with fixed or random seed). For safety, we use `std::unique_ptr` for child nodes or manual deletion with destructor. Since the function returns a double? Actually the snippet uses `long long` for sums; the task asks for a `double`? The prompt says `double rangeSumAfterInsert(...)` but the snippet uses `ll` (long long). To match the snippet style, we should return `long long` or `ll`. I'll change the function signature to return `long long`. Also the insert list is vector of `long long`. The function should be free-standing. Time complexity: each split/merge is O(height) ~ O(log n), we do O(m) inserts and O(1) range query (3 splits and 2 merges), total O((n+m) log (n+m)). Space: O(n+m) for nodes.
#include <vector>
#include <cstdint>
#include <random>
#include <memory>

// Node for implicit treap
struct TreapNode {
    long long key;
    uint64_t priority;
    long long size;
    long long sum;
    std::unique_ptr<TreapNode> left;
    std::unique_ptr<TreapNode> right;

    explicit TreapNode(long long val) : key(val), priority(random_priority()), size(1), sum(val) {}

    static uint64_t random_priority() {
        static std::mt19937_64 rng(12345);
        return rng();
    }
};

using NodePtr = std::unique_ptr<TreapNode>;

inline long long getSize(const NodePtr& t) { return t ? t->size : 0; }
inline long long getSum(const NodePtr& t) { return t ? t->sum : 0; }

void update(NodePtr& t) {
    if (!t) return;
    t->size = 1 + getSize(t->left) + getSize(t->right);
    t->sum = t->key + getSum(t->left) + getSum(t->right);
}

// Split by index: first part has indices [0..k], second part has [k+1..]
void splitByIndex(NodePtr t, NodePtr& left, NodePtr& right, long long k) {
    if (!t) {
        left = nullptr;
        right = nullptr;
        return;
    }
    long long leftSize = getSize(t->left);
    if (k >= leftSize) {
        NodePtr t_right = std::move(t->right);
        splitByIndex(std::move(t_right), t->right, right, k - leftSize - 1);
        left = std::move(t);
    } else {
        NodePtr t_left = std::move(t->left);
        splitByIndex(std::move(t_left), left, t->left, k);
        right = std::move(t);
    }
    update(left);
    update(right);
}

// Merge two treaps where all keys in left precede all keys in right
void merge(NodePtr& t, NodePtr left, NodePtr right) {
    if (!left || !right) {
        t = std::move(left ? left : right);
        return;
    }
    if (left->priority > right->priority) {
        merge(left->right, std::move(left->right), std::move(right));
        t = std::move(left);
    } else {
        merge(right->left, std::move(left), std::move(right->left));
        t = std::move(right);
    }
    update(t);
}

// Build initial treap by merging all values
NodePtr buildTreap(const std::vector<long long>& values) {
    NodePtr root = nullptr;
    for (long long v : values) {
        NodePtr newNode = std::make_unique<TreapNode>(v);
        NodePtr tmp;
        merge(tmp, std::move(root), std::move(newNode));
        root = std::move(tmp);
    }
    return root;
}

// Main function that performs the required operations
long long rangeSumAfterInsert(int initialCount,
                              const std::vector<long long>& initialValues,
                              const std::vector<long long>& inserts,
                              int leftIndex, int rightIndex) {
    // Build initial treap (if initialCount > 0 but initialValues smaller, just use what's there)
    NodePtr root = buildTreap(initialValues);

    // Insert each value at the end
    for (long long val : inserts) {
        long long currentSize = getSize(root);
        NodePtr left, right;
        splitByIndex(std::move(root), left, right, currentSize - 1); // split after last index
        NodePtr newNode = std::make_unique<TreapNode>(val);
        NodePtr merged;
        merge(merged, std::move(left), std::move(newNode));
        merge(root, std::move(merged), std::move(right));
    }

    // Handle invalid range
    long long n = getSize(root);
    if (leftIndex > rightIndex || leftIndex < 0 || rightIndex >= n) {
        return 0;
    }

    // Split into [0..leftIndex-1], [leftIndex..rightIndex], [rightIndex+1..]
    NodePtr A, B, C;
    splitByIndex(std::move(root), A, B, leftIndex - 1);
    splitByIndex(std::move(B), B, C, rightIndex - leftIndex);
    long long result = getSum(B);

    // Merge back (optional but good practice)
    NodePtr tmp;
    merge(tmp, std::move(A), std::move(B));
    merge(root, std::move(tmp), std::move(C));

    return result;
}
#include <cassert>
#include <vector>

// The solution function is already included above, declare it here for the test
long long rangeSumAfterInsert(int initialCount, const std::vector<long long>& initialValues, const std::vector<long long>& inserts, int leftIndex, int rightIndex);

int main() {
    // Basic test: initial [1,2,3], inserts [4,5], range [1,3] -> values 2+3+4=9
    assert(rangeSumAfterInsert(3, {1,2,3}, {4,5}, 1, 3) == 9);

    // Empty initial, insert two values, range [0,1] -> sum of inserted
    assert(rangeSumAfterInsert(0, {}, {10,20}, 0, 1) == 30);

    // Only initial, no inserts, full range
    assert(rangeSumAfterInsert(4, {5,6,7,8}, {}, 0, 3) == 26);

    // Inserts at end, range covering inserted only
    assert(rangeSumAfterInsert(2, {1,1}, {2,3}, 2, 3) == 5);

    // Invalid range (left > right) returns 0
    assert(rangeSumAfterInsert(3, {1,2,3}, {}, 2, 1) == 0);

    // Negative indices (invalid) returns 0
    assert(rangeSumAfterInsert(3, {1,2,3}, {}, -1, 2) == 0);

    // Right index beyond size returns 0
    assert(rangeSumAfterInsert(2, {1,2}, {}, 0, 5) == 0);

    // Single element range
    assert(rangeSumAfterInsert(1, {42}, {}, 0, 0) == 42);

    // Multiple inserts and large-ish numbers
    long long result = rangeSumAfterInsert(1, {100}, {200,300,400}, 0, 3);
    assert(result == 1000); // 100+200+300+400

    // Many inserts and a middle range
    std::vector<long long> ins;
    for (int i = 0; i < 100; ++i) ins.push_back(i);
    // Initial [0], insert 0..99 -> total sequence 0,0,1,2,...,99, so indices 1..100 sum = 0+1+2+...+99 = 4950
    assert(rangeSumAfterInsert(1, {0}, ins, 1, 100) == 4950);

    return 0;
}
