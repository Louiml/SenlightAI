Given an array `parent[]` of size `N` where `parent[i]` is the index of the parent of node `i` in a binary tree (with `parent[i] = -1` for the root), write a C++ function `Node* buildTreeFromParentArray(const vector<int>& parent)` that constructs and returns the root pointer to the binary tree. Each node's `data` field should equal its index `i`. The tree is guaranteed to be a valid binary tree (each node has at most one left and one right child), but the parent array may not be sorted; assign children in a left-first manner (i.e., the first child encountered for a parent becomes the left child, the second becomes the right child). If the parent array is empty, return `nullptr`.

The solution creates a vector of `Node*` pointers of size `N`, one for each index. Each node's data is initialized to its index. Then iterate through the parent array. For each non-root node `i` (parent[i] != -1), attach it to its parent: if the parent's left child is null, set it as left; otherwise set it as right (since constraints guarantee at most two children per parent). The root index is found where parent[i] == -1; if there are multiple -1 entries (shouldn't happen for a valid single-tree representation), the first encountered is used. For an empty input, return `nullptr`. Time complexity is O(N) as we make a single pass to create nodes and another to attach children. Space complexity is O(N) for the vector of node pointers (plus the tree itself, which the caller must manage). Edge cases: N=0 returns nullptr; parent array containing -1 at only one position (the root); ensure no out-of-bounds access by assuming parent[i] is always valid (0 <= parent[i] < N) for non-root entries.

#include <vector>
#include <queue>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Builds a binary tree from a parent array representation.
// Each parent[i] gives the index of the parent of node i, or -1 for the root.
// Returns the root pointer, or nullptr for an empty array.
Node* buildTreeFromParentArray(const std::vector<int>& parent) {
    int n = static_cast<int>(parent.size());
    if (n == 0) return nullptr;

    std::vector<Node*> nodePtrs(n, nullptr);
    for (int i = 0; i < n; ++i) {
        nodePtrs[i] = new Node(i);
    }

    int rootIndex = -1;
    for (int i = 0; i < n; ++i) {
        if (parent[i] == -1) {
            rootIndex = i;
        } else {
            int p = parent[i];
            if (nodePtrs[p]->left == nullptr) {
                nodePtrs[p]->left = nodePtrs[i];
            } else {
                nodePtrs[p]->right = nodePtrs[i];
            }
        }
    }

    return nodePtrs[rootIndex];
}

#include <cassert>
#include <vector>
#include <queue>
#include <iostream>

// Assume the Node struct and function from the solution are available.

// Helper to collect level order values (with null markers) into a vector for comparison.
std::vector<int> levelOrderValues(Node* root) {
    std::vector<int> result;
    if (!root) return result;
    std::queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* cur = q.front();
        q.pop();
        if (cur) {
            result.push_back(cur->data);
            q.push(cur->left);
            q.push(cur->right);
        } else {
            result.push_back(-1); // marker for null child
        }
    }
    // Remove trailing null markers for clarity (but we keep them for exactness maybe)
    return result;
}

// Helper to delete tree to prevent memory leaks (for testing only, not required in solution).
void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    // Test 1: simple tree with root 0, children 1(left) and 2(right)
    {
        std::vector<int> parent = {-1, 0, 0};
        Node* root = buildTreeFromParentArray(parent);
        assert(root->data == 0);
        assert(root->left->data == 1);
        assert(root->right->data == 2);
        deleteTree(root);
    }

    // Test 2: root is not index 0 (here index 2 is root)
    {
        std::vector<int> parent = {2, 2, -1};
        Node* root = buildTreeFromParentArray(parent);
        assert(root->data == 2);
        assert(root->left->data == 0);
        assert(root->right->data == 1);
        deleteTree(root);
    }

    // Test 3: deeper tree
    {
        std::vector<int> parent = {-1, 0, 0, 1, 1};
        // tree: 0 root, left=1, right=2, node1 left=3, right=4
        Node* root = buildTreeFromParentArray(parent);
        std::vector<int> vals = levelOrderValues(root);
        // level order: 0,1,2,3,4
        std::vector<int> expected = {0,1,2,3,4};
        // We need to ignore null placeholders; levelOrderValues may have extra -1 for missing children.
        // Actually level order for full? It returns 0,1,2,3,4,-1,-1,-1,-1? Let's manually check:
        // We'll just check directly without helper.
        assert(root->data == 0);
        assert(root->left->data == 1);
        assert(root->right->data == 2);
        assert(root->left->left->data == 3);
        assert(root->left->right->data == 4);
        deleteTree(root);
    }

    // Test 4: empty input
    {
        std::vector<int> parent;
        Node* root = buildTreeFromParentArray(parent);
        assert(root == nullptr);
    }

    // Test 5: single node (root)
    {
        std::vector<int> parent = {-1};
        Node* root = buildTreeFromParentArray(parent);
        assert(root->data == 0);
        assert(root->left == nullptr);
        assert(root->right == nullptr);
        deleteTree(root);
    }

    // Test 6: skewed tree (all left children)
    {
        std::vector<int> parent = {-1, 0, 1}; // 0->left=1, 1->left=2
        Node* root = buildTreeFromParentArray(parent);
        assert(root->data == 0);
        assert(root->left->data == 1);
        assert(root->left->left->data == 2);
        assert(root->left->right == nullptr);
        deleteTree(root);
    }

    // Test 7: parent not in sequential order, e.g., parent[3]=1, etc.
    {
        std::vector<int> parent = {1, -1, 0, 1, 2};
        // Build: root=1; children of 1: 0(left),3(right); children of 0: 2(left); children of 2: 4(left)
        Node* root = buildTreeFromParentArray(parent);
        assert(root->data == 1);
        assert(root->left->data == 0);
        assert(root->right->data == 3);
        assert(root->left->left->data == 2);
        assert(root->left->left->left->data == 4);
        deleteTree(root);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
