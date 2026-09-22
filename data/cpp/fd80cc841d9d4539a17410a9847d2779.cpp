Write a standalone C++ function that takes the root of a binary tree (where each node has an integer `val` and pointers to `left` and `right` children) and returns a `std::vector<int>` containing the node values in breadth-first (level-order) traversal order, from left to right at each level. The tree is guaranteed to be non-empty, but may be skewed or unbalanced. The function must be `const`-correct (accept a pointer-to-const node) and must not modify the tree. It must allocate all output storage dynamically via the vector and must use a queue to process nodes level by level. The function should be named `breadthFirstTraversal` and must reside in the global namespace with no `main` function.

#include <cassert>
#include <vector>

// Reuse the struct and function declaration (already defined above).
int main() {
    // Test 1: single node
    node n1{1, nullptr, nullptr};
    std::vector<int> res1 = breadthFirstTraversal(&n1);
    assert(res1.size() == 1 && res1[0] == 1);

    // Test 2: balanced tree of 3 nodes: root=2, left=1, right=3
    node left{1, nullptr, nullptr};
    node right{3, nullptr, nullptr};
    node root{2, &left, &right};
    std::vector<int> res2 = breadthFirstTraversal(&root);
    assert(res2.size() == 3);
    assert(res2[0] == 2 && res2[1] == 1 && res2[2] == 3);

    // Test 3: left-skewed tree: 10 -> 9 -> 8
    node leaf{8, nullptr, nullptr};
    node mid{9, &leaf, nullptr};
    node top{10, &mid, nullptr};
    std::vector<int> res3 = breadthFirstTraversal(&top);
    assert(res3.size() == 3);
    assert(res3[0] == 10 && res3[1] == 9 && res3[2] == 8);

    // Test 4: right-skewed tree: 5 -> 6 -> 7
    node r_leaf{7, nullptr, nullptr};
    node r_mid{6, nullptr, &r_leaf};
    node r_top{5, nullptr, &r_mid};
    std::vector<int> res4 = breadthFirstTraversal(&r_top);
    assert(res4.size() == 3);
    assert(res4[0] == 5 && res4[1] == 6 && res4[2] == 7);

    // Test 5: irregular tree: root=1, left=2, right=3; left->left=4, left->right=5, right->left=6
    node n4{4, nullptr, nullptr};
    node n5{5, nullptr, nullptr};
    node n6{6, nullptr, nullptr};
    node n2{2, &n4, &n5};
    node n3{3, &n6, nullptr};
    node n1{1, &n2, &n3};
    std::vector<int> res5 = breadthFirstTraversal(&n1);
    assert(res5.size() == 6);
    assert(res5[0] == 1 && res5[1] == 2 && res5[2] == 3);
    assert(res5[3] == 4 && res5[4] == 5 && res5[5] == 6);

    // Test 6: empty tree (nullptr) should return empty vector
    std::vector<int> res6 = breadthFirstTraversal(nullptr);
    assert(res6.empty());

    return 0;
}

#include <vector>
#include <queue>

struct node {
    int val;
    node *left;
    node *right;
};

// Perform a breadth-first (level-order) traversal of a binary tree.
// Returns a vector of node values in the order they are visited.
// The tree is not modified, and the function works on const trees.
std::vector<int> breadthFirstTraversal(const node* root) {
    std::vector<int> result;
    if (root == nullptr) {
        return result;
    }

    std::queue<const node*> q;
    q.push(root);

    while (!q.empty()) {
        const node* current = q.front();
        q.pop();
        result.push_back(current->val);

        if (current->left != nullptr) {
            q.push(current->left);
        }
        if (current->right != nullptr) {
            q.push(current->right);
        }
    }

    return result;
}

// We use a standard iterative level-order traversal with a queue initialized with the root. At each step, we pop the front node, record its value, then push its left and right children (if non-null) in that order. Because the tree is non-empty, the queue never becomes empty before at least one value is recorded. Edge cases include a tree with only a root (vector of one element), a left-skewed tree (left child only), and a right-skewed tree – all handled naturally since we check for null before pushing. Time complexity is O(n) where n is the number of nodes because each node is visited exactly once. Space complexity is O(w) where w is the maximum width of the tree (the maximum number of nodes in a queue at any one time), which in the worst case is O(n) for a complete binary tree, but typically O(n) in worst-case unbalanced skewed trees as only one node is in the queue at a time.
