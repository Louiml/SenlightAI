Write a C++ function `std::vector<int> reverseSegment(const std::vector<int>& arr, int l, int r)` that takes a 1-indexed sequence of distinct integers from 1 to n (where n is the size of the input) and two indices `l` and `r` (with `1 <= l <= r <= n`). The function should reverse the subarray from position `l` to position `r` and return the resulting sequence as a vector. The reversal must be performed using a treap (implicit key, random priority) with lazy reverse flags, mirroring the provided snippet. The input vector represents the initial sequence, and the function should apply exactly one reversal operation. You are not allowed to simply use `std::reverse`; you must implement the treap-based split/merge logic. Assume the input is always valid and contains the numbers 1..n in some order.

// The solution uses an implicit-treap (also known as a randomized binary search tree on positions) where each node stores a value, a subtree size, and a lazy reverse flag. The key insight is that the treap's in-order traversal gives the current sequence. To reverse a segment `[l, r]`:
// 1. Split the treap into three parts: the first `l-1` nodes (`a`), the segment of length `r-l+1` (`b`), and the remaining nodes (`c`). The split by size is done using `split(root, k, x, y)` which splits the first `k` nodes into `x` and the rest into `y`.
// 2. Toggle the lazy flag on `b` (`fl[b] ^= 1`). This marks that the subtree `b` should be reversed.
// 3. Merge the parts back in order `a`, `b`, `c`.
// When traversing or splitting later, the lazy flag must be pushed down: if `fl[rt]` is true, swap the left and right children and propagate the flag to children.
//
// The function builds the initial treap from the input vector using a recursive build that creates a balanced structure (though random priorities could also be used, here we use sequential values for simplicity). The `split` functions must call `push_down` before recursing to ensure that any pending reversals are applied. The `merge` function also calls `push_down` on the node it chooses as the root. After the reversal, an in-order traversal collects the values into a result vector.
//
// Edge cases: `l=1` or `r=n` will produce empty parts, but the split/merge logic handles null roots (0) gracefully. The case `l==r` requires no actual change; the lazy flag can be toggled but it will have no effect. The implementation must handle recursion depth; since the build creates a balanced tree, depth is `O(log n)`, and splits/merges are `O(log n)` amortized.
//
// Time complexity: Building the treap is `O(n)`. Each split and merge is `O(log n)` (expected), so the total is `O(n + log n)` = `O(n)`. Space complexity is `O(n)` for the treap nodes and the result vector.

#include <vector>
#include <cstdlib>

class ImplicitTreap {
    struct Node {
        int val;
        int priority;
        int size;
        bool reverse;
        Node* left;
        Node* right;
        Node(int v) : val(v), priority(std::rand()), size(1), reverse(false), left(nullptr), right(nullptr) {}
    };

    Node* root;

    int getSize(Node* t) const {
        return t ? t->size : 0;
    }

    void pushUp(Node* t) {
        if (t) {
            t->size = 1 + getSize(t->left) + getSize(t->right);
        }
    }

    void pushDown(Node* t) {
        if (t && t->reverse) {
            std::swap(t->left, t->right);
            if (t->left) t->left->reverse = !t->left->reverse;
            if (t->right) t->right->reverse = !t->right->reverse;
            t->reverse = false;
        }
    }

    Node* build(const std::vector<int>& arr, int l, int r) {
        if (l > r) return nullptr;
        int mid = (l + r) / 2;
        Node* node = new Node(arr[mid]);
        node->left = build(arr, l, mid - 1);
        node->right = build(arr, mid + 1, r);
        pushUp(node);
        return node;
    }

    void split(Node* t, int k, Node*& a, Node*& b) {
        if (!t) {
            a = b = nullptr;
            return;
        }
        pushDown(t);
        if (k <= getSize(t->left)) {
            split(t->left, k, a, t->left);
            b = t;
        } else {
            split(t->right, k - getSize(t->left) - 1, t->right, b);
            a = t;
        }
        pushUp(t);
    }

    Node* merge(Node* a, Node* b) {
        if (!a || !b) return a ? a : b;
        if (a->priority < b->priority) {
            pushDown(a);
            a->right = merge(a->right, b);
            pushUp(a);
            return a;
        } else {
            pushDown(b);
            b->left = merge(a, b->left);
            pushUp(b);
            return b;
        }
    }

    void collect(Node* t, std::vector<int>& result) const {
        if (!t) return;
        pushDown(const_cast<Node*>(t));
        collect(t->left, result);
        result.push_back(t->val);
        collect(t->right, result);
    }

public:
    explicit ImplicitTreap(const std::vector<int>& arr) {
        std::srand(42); // fixed seed for reproducibility
        root = build(arr, 0, static_cast<int>(arr.size()) - 1);
    }

    std::vector<int> reverseSegment(int l, int r) {
        Node* a = nullptr;
        Node* b = nullptr;
        Node* c = nullptr;
        split(root, l - 1, a, b);
        split(b, r - l + 1, b, c);
        if (b) b->reverse = !b->reverse;
        root = merge(a, merge(b, c));
        std::vector<int> result;
        collect(root, result);
        return result;
    }
};

// Reverses the segment [l, r] (1-indexed) of the input sequence and returns the result.
std::vector<int> reverseSegment(const std::vector<int>& arr, int l, int r) {
    ImplicitTreap treap(arr);
    return treap.reverseSegment(l, r);
}

#include <cassert>
#include <vector>

// The solution function is declared above (assume it is included).
int main() {
    // Basic reversal in the middle
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    assert(reverseSegment(v1, 2, 4) == std::vector<int>({1, 4, 3, 2, 5}));

    // Reverse entire sequence
    std::vector<int> v2 = {1, 2, 3};
    assert(reverseSegment(v2, 1, 3) == std::vector<int>({3, 2, 1}));

    // Reverse a single element (no change)
    std::vector<int> v3 = {5, 1, 4, 2, 3};
    assert(reverseSegment(v3, 3, 3) == v3);

    // Reverse from first to some position
    std::vector<int> v4 = {2, 4, 6, 8};
    assert(reverseSegment(v4, 1, 2) == std::vector<int>({4, 2, 6, 8}));

    // Reverse from some position to last
    std::vector<int> v5 = {3, 1, 2, 5, 4};
    assert(reverseSegment(v5, 2, 5) == std::vector<int>({3, 4, 5, 2, 1}));

    // Large sequence to test correctness
    std::vector<int> v6;
    for (int i = 1; i <= 100; ++i) v6.push_back(i);
    std::vector<int> expected6 = v6;
    std::reverse(expected6.begin() + 10, expected6.begin() + 20); // indices 11..20 zero-based
    assert(reverseSegment(v6, 11, 20) == expected6);

    // Reverse entire sequence of even length
    std::vector<int> v7 = {1, 2, 3, 4};
    assert(reverseSegment(v7, 1, 4) == std::vector<int>({4, 3, 2, 1}));

    // Non-contiguous values but distinct
    std::vector<int> v8 = {7, 3, 9, 1, 5};
    assert(reverseSegment(v8, 2, 5) == std::vector<int>({7, 5, 1, 9, 3}));

    // Reversal at the beginning
    std::vector<int> v9 = {4, 3, 2, 1, 5};
    assert(reverseSegment(v9, 1, 2) == std::vector<int>({3, 4, 2, 1, 5}));

    // Reversal at the end
    std::vector<int> v10 = {1, 5, 3, 2, 4};
    assert(reverseSegment(v10, 4, 5) == std::vector<int>({1, 5, 3, 4, 2}));

    return 0;
}
