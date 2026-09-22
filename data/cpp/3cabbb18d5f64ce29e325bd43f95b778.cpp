Implement a C++ class `OrderedSet` that internally stores unique integers in a binary search tree (BST) and provides three public methods: `bool add(int key)`, `bool remove(int key)`, and `bool contains(int key) const`. The `add` method should return `true` if the key was not already present (and was successfully inserted), and `false` if it already existed. The `remove` method should return `true` if the key was present and removed, and `false` if it was not found. The `contains` method should return whether the key exists in the set. The BST must support duplicate prevention, so inserting an existing key must not alter the tree. Removal must handle all three cases: leaf, node with one child, and node with two children (use the minimum of the right subtree as the successor). Your class must manage memory correctly (deep copy not required, but destructor must free all nodes). No `main` function should be provided; write only the class definition and implementation. The class should be self-contained with appropriate headers.
The core structure is a binary search tree where each node stores an integer and pointers to left and right children. Insertion recursively navigates left/right based on comparison with the current node's value; if the value already exists, return `false` without modifying the tree. Otherwise, create a new node at the appropriate null child and return `true`. Contains uses an iterative loop or recursive search, traversing left/right until the value is found or a null is reached. Removal is more involved: first find the node; if missing, return `false`. If the node has zero or one child, replace it with its null/non-null child and delete the old node. If it has two children, locate the minimum node in its right subtree, copy that minimum's value into the current node, then recursively remove the minimum from the right subtree. Edge cases include removing the root, removing a node with only a left child, only a right child, and the two-child case where the minimum might be the immediate right child. The recursive helpers must return new subtree roots to correctly reconnect parents. Memory safety: each node must be deleted exactly once; use a recursive destructor for the node that deletes both children, and careful pointer handling in removal to avoid double-free. Time complexity: each operation averages `O(log n)` on a balanced tree but worst-case `O(n)` if the tree degenerates (e.g., inserting sorted keys). Space complexity: `O(n)` for storing nodes, and recursion stack depth `O(height)`.
#include <memory>

struct Node {
    int val;
    std::unique_ptr<Node> left, right;
    explicit Node(int v) : val(v), left(nullptr), right(nullptr) {}
};

class OrderedSet {
private:
    std::unique_ptr<Node> root;

    bool insertRec(std::unique_ptr<Node>& node, int key) {
        if (!node) {
            node = std::make_unique<Node>(key);
            return true;
        }
        if (key < node->val) {
            return insertRec(node->left, key);
        } else if (key > node->val) {
            return insertRec(node->right, key);
        }
        return false; // duplicate
    }

    bool containsRec(const std::unique_ptr<Node>& node, int key) const {
        if (!node) return false;
        if (key < node->val) return containsRec(node->left, key);
        if (key > node->val) return containsRec(node->right, key);
        return true;
    }

    // Find minimum node in subtree rooted at node (must be non-null).
    int getMin(const std::unique_ptr<Node>& node) const {
        const Node* cur = node.get();
        while (cur->left) cur = cur->left.get();
        return cur->val;
    }

    // Remove the minimum node from the subtree rooted at node.
    std::unique_ptr<Node> removeMin(std::unique_ptr<Node>& node) {
        if (!node->left) {
            return std::move(node->right);
        }
        node->left = removeMin(node->left);
        return std::move(node);
    }

    // Remove key from subtree rooted at node; return new subtree root.
    std::unique_ptr<Node> removeRec(std::unique_ptr<Node>& node, int key) {
        if (!node) return nullptr;
        if (key < node->val) {
            node->left = removeRec(node->left, key);
            return std::move(node);
        } else if (key > node->val) {
            node->right = removeRec(node->right, key);
            return std::move(node);
        }
        // Key found
        if (!node->left && !node->right) {
            return nullptr; // leaf
        }
        if (!node->left) {
            return std::move(node->right);
        }
        if (!node->right) {
            return std::move(node->left);
        }
        // Two children
        node->val = getMin(node->right);
        node->right = removeMin(node->right);
        return std::move(node);
    }

public:
    OrderedSet() : root(nullptr) {}

    bool add(int key) {
        return insertRec(root, key);
    }

    bool remove(int key) {
        if (!contains(key)) return false;
        root = removeRec(root, key);
        return true;
    }

    bool contains(int key) const {
        return containsRec(root, key);
    }
};
#include <cassert>

int main() {
    OrderedSet s;
    // Initial add
    assert(s.add(5) == true);
    assert(s.add(3) == true);
    assert(s.add(8) == true);
    assert(s.contains(5) == true);
    assert(s.contains(3) == true);
    assert(s.contains(8) == true);
    assert(s.contains(1) == false);
    // Duplicate add
    assert(s.add(5) == false);
    assert(s.add(3) == false);
    // Remove leaf
    assert(s.remove(3) == true);
    assert(s.contains(3) == false);
    assert(s.remove(3) == false);
    // Remove node with one child
    assert(s.add(7) == true);
    assert(s.add(9) == true);
    assert(s.remove(8) == true); // 8 has right child 9
    assert(s.contains(8) == false);
    assert(s.contains(9) == true);
    // Remove node with two children
    assert(s.add(10) == true);
    assert(s.add(12) == true);
    assert(s.remove(9) == true); // 9 has left? none, right 10? actually add 10 after; but let's build a specific case
    // Clear and rebuild for two-child test
    OrderedSet t;
    t.add(50);
    t.add(30);
    t.add(70);
    t.add(20);
    t.add(40);
    t.add(60);
    t.add(80);
    assert(t.remove(50) == true); // two children, successor 60
    assert(t.contains(50) == false);
    assert(t.contains(60) == true);
    assert(t.contains(30) == true);
    assert(t.contains(70) == true);
    assert(t.remove(30) == true); // one child 20? Actually 30 has left 20 and right 40 -> two children? Wait 30 has both, so it's two children, but we just need a valid check
    assert(t.contains(30) == false);
    // Remove root with only left child
    OrderedSet u;
    u.add(100);
    u.add(50);
    assert(u.remove(100) == true);
    assert(u.contains(100) == false);
    assert(u.contains(50) == true);
    // Remove last node
    assert(u.remove(50) == true);
    assert(u.contains(50) == false);
    assert(u.remove(50) == false);
    // Large sequence
    OrderedSet v;
    for (int i = 0; i < 100; ++i) assert(v.add(i) == true);
    for (int i = 0; i < 100; ++i) assert(v.contains(i) == true);
    for (int i = 0; i < 100; i += 2) assert(v.remove(i) == true);
    for (int i = 0; i < 100; ++i) {
        if (i % 2 == 0) assert(v.contains(i) == false);
        else assert(v.contains(i) == true);
    }
    return 0;
}
