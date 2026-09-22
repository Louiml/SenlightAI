// Given a sequence of integers where each integer is either inserted into or erased from an initially empty self-balancing binary search tree, write a C++ function that simulates the operations and returns the number of distinct keys that remain in the tree after all operations. The function must accept a vector of pairs of integers representing the operation type and value: type `0` is an insertion operation, type `1` is a deletion operation. Deletions of keys not present in the tree must be silently ignored. The input vector may be empty. The function must correctly handle duplicate insertions (the tree stores distinct keys only), and after each insertion or deletion the tree must remain balanced (AVL). The return value must be the final count of distinct keys in the tree.

The problem is a direct simulation of an AVL tree with insert and erase operations. The solution approach is to implement an AVL tree from scratch, or to reuse the provided AVL tree structure from the snippet, but since we must produce a standalone task, we implement our own minimal AVL tree with integer keys and a `size` field that tracks the number of nodes. The core algorithm is standard: for insertion, recursively descend left or right, create a new node when reaching a null pointer, and on the way back, update height/size and perform rotations to restore balance. For deletion, find the node, and if it has two children, replace its value with the minimum of the right subtree (or maximum of the left) and delete that successor recursively. If it has one child or no children, splice it out. After each recursive step, call a balance function that checks balance factors (height of left minus height of right) and applies single or double rotations as needed. Important edge cases: deleting a key not present must return the tree unchanged; inserting an existing key must not create a duplicate and must not change the tree structure (we can simply return the same node); empty tree operations must work. After all operations, return the `size` of the root (or 0 if root is null). Time complexity per operation is O(log n) on average because the AVL tree maintains height O(log n); total for k operations and n distinct keys is O(k log n). Space complexity is O(n) for the tree nodes.

#include <cstdint>
#include <algorithm>
#include <vector>

struct AVLNode {
    int64_t value;
    int64_t height;
    int64_t size;
    AVLNode* left;
    AVLNode* right;
    AVLNode(int64_t v) : value(v), height(1), size(1), left(nullptr), right(nullptr) {}
};

static int64_t height(const AVLNode* v) {
    return v == nullptr ? 0 : v->height;
}

static int64_t size_of(const AVLNode* v) {
    return v == nullptr ? 0 : v->size;
}

static void update(AVLNode* v) {
    if (v != nullptr) {
        v->height = std::max(height(v->left), height(v->right)) + 1;
        v->size = size_of(v->left) + size_of(v->right) + 1;
    }
}

static int64_t balance_factor(const AVLNode* v) {
    return v == nullptr ? 0 : height(v->left) - height(v->right);
}

static AVLNode* rotate_right(AVLNode* v) {
    AVLNode* q = v->left;
    v->left = q->right;
    q->right = v;
    update(v);
    update(q);
    return q;
}

static AVLNode* rotate_left(AVLNode* v) {
    AVLNode* q = v->right;
    v->right = q->left;
    q->left = v;
    update(v);
    update(q);
    return q;
}

static AVLNode* large_rotate_right(AVLNode* v) {
    AVLNode* q = v->left;
    AVLNode* r = q->right;
    v->left = r->right;
    q->right = r->left;
    r->left = q;
    r->right = v;
    update(v);
    update(q);
    update(r);
    return r;
}

static AVLNode* large_rotate_left(AVLNode* v) {
    AVLNode* q = v->right;
    AVLNode* r = q->left;
    v->right = r->left;
    q->left = r->right;
    r->left = v;
    r->right = q;
    update(v);
    update(q);
    update(r);
    return r;
}

static AVLNode* balance(AVLNode* v) {
    update(v);
    int64_t bf = balance_factor(v);
    if (bf == 2) {
        if (balance_factor(v->left) < 0) {
            return large_rotate_right(v);
        } else {
            return rotate_right(v);
        }
    } else if (bf == -2) {
        if (balance_factor(v->right) > 0) {
            return large_rotate_left(v);
        } else {
            return rotate_left(v);
        }
    }
    return v;
}

static AVLNode* insert_value(AVLNode* v, int64_t key) {
    if (v == nullptr) return new AVLNode(key);
    if (key < v->value) {
        v->left = insert_value(v->left, key);
    } else if (key > v->value) {
        v->right = insert_value(v->right, key);
    } else {
        return v; // duplicate, ignore
    }
    return balance(v);
}

static std::pair<int64_t, AVLNode*> find_min(AVLNode* v) {
    if (v->left == nullptr) {
        return {v->value, v->right};
    }
    auto res = find_min(v->left);
    v->left = res.second;
    return {res.first, v};
}

static AVLNode* erase_value(AVLNode* v, int64_t key) {
    if (v == nullptr) return nullptr; // not found
    if (key < v->value) {
        v->left = erase_value(v->left, key);
    } else if (key > v->value) {
        v->right = erase_value(v->right, key);
    } else {
        if (v->left == nullptr && v->right == nullptr) {
            delete v;
            return nullptr;
        } else if (v->right != nullptr) {
            auto min_pair = find_min(v->right);
            v->value = min_pair.first;
            v->right = min_pair.second;
        } else { // left child exists, no right child
            AVLNode* max_node = v->left;
            while (max_node->right != nullptr) max_node = max_node->right;
            v->value = max_node->value;
            // remove max_node from left subtree
            v->left = erase_value(v->left, max_node->value);
        }
    }
    return balance(v);
}

// Simulate insert/erase operations on an AVL tree and return final distinct key count.
int64_t simulate_avl_operations(const std::vector<std::pair<int, int64_t>>& operations) {
    AVLNode* root = nullptr;
    for (const auto& op : operations) {
        if (op.first == 0) {
            root = insert_value(root, op.second);
        } else if (op.first == 1) {
            root = erase_value(root, op.second);
        }
        // ignore other op codes? The task says only 0 and 1 are used.
    }
    int64_t result = size_of(root);
    // Optional: cleanup to avoid memory leak - but in a standalone solution we skip for brevity.
    // In test code, we might not need to delete.
    return result;
}

#include <cassert>
#include <vector>
#include <cstdint>
int main() {
    // Empty input -> 0 distinct keys
    assert(simulate_avl_operations({}) == 0);

    // Insert 1,2,3 => 3 distinct
    assert(simulate_avl_operations({{0,1},{0,2},{0,3}}) == 3);

    // Insert duplicate 1 twice then erase 2 => 2 distinct
    assert(simulate_avl_operations({{0,1},{0,2},{0,1},{1,2}}) == 2);

    // Erase a non-existent key -> unchanged
    assert(simulate_avl_operations({{0,10},{0,20},{1,30}}) == 2);

    // Insert many, then delete all => 0
    assert(simulate_avl_operations({{0,5},{0,3},{0,8},{1,5},{1,3},{1,8}}) == 0);

    // Insert negative and zero
    assert(simulate_avl_operations({{0,-5},{0,0},{0,7},{1,-5}}) == 2);

    // Complex mixed operations
    assert(simulate_avl_operations({{0,100},{0,50},{0,150},{0,25},{1,100},{0,200}}) == 4);

    // Large values
    assert(simulate_avl_operations({{0,1000000000000LL},{0,-1000000000000LL},{1,1}}) == 2);

    // Insert and erase same pattern repeatedly
    assert(simulate_avl_operations({{0,1},{1,1},{0,1},{1,1},{0,1}}) == 1);

    // Delete the only element -> 0
    assert(simulate_avl_operations({{0,42},{1,42}}) == 0);
}
