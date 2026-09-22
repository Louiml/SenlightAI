// Write a C++ function that takes the root of a binary tree and a non-negative integer k, and returns the number of distinct nodes in the tree that have at least one leaf in their subtree at exactly distance k from that node. A node counts only once even if multiple leaves in its subtree are at distance k. The tree is defined by a `struct Node` with `int data`, `Node* left`, and `Node* right`. You may use any necessary standard library includes. The function should be named `countNodesAtDistanceKFromLeaf` and must have signature `int countNodesAtDistanceKFromLeaf(Node* root, int k)`.

// The solution performs a depth-first traversal of the tree while maintaining the current path from the root to the current node. For each leaf encountered, if the path length (including the leaf itself) is at least k+1, then the node that is exactly k steps above the leaf (i.e., the ancestor at distance k) is a valid candidate. Because the same internal node may be reached via multiple leaves, we store valid nodes in a set to deduplicate. The algorithm runs in O(n) time and O(n) space in the worst case (for the path vector and the set), where n is the number of nodes. Edge cases include: k = 0 (the leaf itself is the node at distance 0), trees with a single node, and trees where k exceeds the depth of the deepest leaf (in which case no nodes are counted). The use of a set ensures each node is counted only once, regardless of how many leaves satisfy the condition for that node.

#include <set>
#include <vector>

struct Node {
    int data;
    Node* left;
    Node* right;
};

// Helper function for recursive traversal
void collectValidNodes(Node* root, int k, int currentDepth, std::vector<Node*>& path, std::set<Node*>& validNodes) {
    if (root == nullptr) return;

    path.push_back(root);

    // Check if current node is a leaf
    if (root->left == nullptr && root->right == nullptr) {
        // If current path length is at least k+1 (currentDepth >= k), then the node at distance k is path[currentDepth - k]
        if (currentDepth >= k) {
            validNodes.insert(path[currentDepth - k]);
        }
    }

    // Recurse on left and right children
    collectValidNodes(root->left, k, currentDepth + 1, path, validNodes);
    collectValidNodes(root->right, k, currentDepth + 1, path, validNodes);

    // Backtrack
    path.pop_back();
}

// Main function to count distinct nodes at distance k from at least one leaf
int countNodesAtDistanceKFromLeaf(Node* root, int k) {
    if (root == nullptr || k < 0) return 0;

    std::set<Node*> validNodes;
    std::vector<Node*> path;
    collectValidNodes(root, k, 0, path, validNodes);

    return static_cast<int>(validNodes.size());
}

#include <cassert>

int main() {
    // Test 1: Example from problem statement
    // Tree: 1(2(4,5),3(6(N,8),7))
    Node* n8 = new Node{8, nullptr, nullptr};
    Node* n4 = new Node{4, nullptr, nullptr};
    Node* n5 = new Node{5, nullptr, nullptr};
    Node* n6 = new Node{6, nullptr, n8};
    Node* n7 = new Node{7, nullptr, nullptr};
    Node* n2 = new Node{2, n4, n5};
    Node* n3 = new Node{3, n6, n7};
    Node* root1 = new Node{1, n2, n3};
    assert(countNodesAtDistanceKFromLeaf(root1, 2) == 2);

    // Test 2: k=0, each leaf itself is counted
    Node* a = new Node{1, nullptr, nullptr};
    Node* b = new Node{2, nullptr, nullptr};
    Node* root2 = new Node{3, a, b};
    assert(countNodesAtDistanceKFromLeaf(root2, 0) == 2);

    // Test 3: Single node tree, k=0
    Node* single = new Node{5, nullptr, nullptr};
    assert(countNodesAtDistanceKFromLeaf(single, 0) == 1);

    // Test 4: Single node tree, k>0
    assert(countNodesAtDistanceKFromLeaf(single, 1) == 0);

    // Test 5: k larger than depth
    // Tree: 1(2,3)
    Node* l = new Node{2, nullptr, nullptr};
    Node* r = new Node{3, nullptr, nullptr};
    Node* root3 = new Node{1, l, r};
    assert(countNodesAtDistanceKFromLeaf(root3, 2) == 0);

    // Test 6: Only one leaf qualifies a node once
    // Tree: 1(2(4,5),3)
    Node* leaf4 = new Node{4, nullptr, nullptr};
    Node* leaf5 = new Node{5, nullptr, nullptr};
    Node* inner2 = new Node{2, leaf4, leaf5};
    Node* leaf3 = new Node{3, nullptr, nullptr};
    Node* root4 = new Node{1, inner2, leaf3};
    // For k=1: leaves at distance 1 from inner2 (both 4 and 5) -> only inner2 counted once
    assert(countNodesAtDistanceKFromLeaf(root4, 1) == 2); // inner2 and root1 (distance 1 from leaf3)
    // For k=2: root is distance 2 from leaves 4 and 5, and distance 1 from leaf3; only root counted
    assert(countNodesAtDistanceKFromLeaf(root4, 2) == 1);

    // Test 7: Empty tree
    assert(countNodesAtDistanceKFromLeaf(nullptr, 1) == 0);

    // Test 8: Negative k (should return 0)
    assert(countNodesAtDistanceKFromLeaf(root1, -1) == 0);

    return 0;
}
