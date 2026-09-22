Write a C++ function `std::vector<int> boundaryTraversal(Node* root)` that, given the root of a binary tree where each node contains an integer, returns a vector of the tree's boundary in counter-clockwise order starting from the root. The boundary consists of: the root node, then all nodes on the left boundary (excluding leaf nodes), then all leaf nodes from left to right, then all nodes on the right boundary in bottom-up order (excluding leaf nodes), finishing at the bottom-right. The input tree may be empty (root is `nullptr`), may have only one node, may be a skewed tree (only left or only right children), or may have missing children at any node (represented as `nullptr`). The function must not modify the tree and must handle all cases without duplicating nodes (e.g., a leaf on the left boundary should appear only once, in the leaf section). Your implementation should be generic and rely only on the following provided `Node` structure: each node has `int data`, `Node* left`, and `Node* right`. Return an empty vector if the root is `nullptr`.
The solution follows a standard three-step decomposition of the boundary traversal: (1) collect the left boundary (excluding root and leaves), (2) collect all leaves via an in-order traversal (left subtree then right subtree) starting from the root's children, and (3) collect the right boundary in reverse order (bottom-up) by traversing from the root's right child, again excluding leaves, and appending after accumulating so that the top-right node appears last. Care must be taken to avoid duplicate elements: when traversing the left boundary, if a node has a left child, go left; otherwise go right, but stop as soon as a leaf is reached. Similarly, for the right boundary, traverse right if possible else left, but do not push the node immediately; push it after the recursive call to get bottom-up order, and stop at leaves. For leaves, a recursive function that only appends when both children are `nullptr` covers the entire tree. Edge cases include: empty tree (return empty vector), single node (return just root), a tree where the left child of root is a leaf (then left boundary contributes nothing, but the leaf is collected in the leaf pass), and a skewed tree where right boundary may be the same as the left boundary (but due to the algorithm, no duplicates appear because leaves are excluded from boundary sides). Time complexity is O(n) since each node is visited at most a constant number of times (left/right boundary visits each node once, leaf traversal visits all nodes once). Space complexity is O(h) for recursion stack where h is tree height, plus O(n) for the output vector.
#include <vector>

// Definition for a binary tree node.
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

namespace {
    // Collect nodes on the left boundary, excluding root and leaves.
    void collectLeftBoundary(Node* node, std::vector<int>& result) {
        if (node == nullptr || (node->left == nullptr && node->right == nullptr)) {
            return;
        }
        result.push_back(node->data);
        if (node->left != nullptr) {
            collectLeftBoundary(node->left, result);
        } else {
            collectLeftBoundary(node->right, result);
        }
    }

    // Collect all leaf nodes from left to right.
    void collectLeaves(Node* node, std::vector<int>& result) {
        if (node == nullptr) {
            return;
        }
        if (node->left == nullptr && node->right == nullptr) {
            result.push_back(node->data);
            return;
        }
        collectLeaves(node->left, result);
        collectLeaves(node->right, result);
    }

    // Collect nodes on the right boundary in bottom-up order, excluding leaves and root.
    void collectRightBoundary(Node* node, std::vector<int>& result) {
        if (node == nullptr || (node->left == nullptr && node->right == nullptr)) {
            return;
        }
        if (node->right != nullptr) {
            collectRightBoundary(node->right, result);
        } else {
            collectRightBoundary(node->left, result);
        }
        result.push_back(node->data);
    }
}

// Return the boundary traversal of a binary tree in counter-clockwise order.
std::vector<int> boundaryTraversal(Node* root) {
    std::vector<int> result;
    if (root == nullptr) {
        return result;
    }
    result.push_back(root->data);
    
    collectLeftBoundary(root->left, result);
    collectLeaves(root->left, result);
    collectLeaves(root->right, result);
    collectRightBoundary(root->right, result);
    
    return result;
}
#include <cassert>
#include <vector>

// Unit tests for boundaryTraversal.
int main() {
    // Test 1: Empty tree.
    assert(boundaryTraversal(nullptr).empty());

    // Test 2: Single node.
    Node* single = new Node(10);
    assert(boundaryTraversal(single) == std::vector<int>({10}));
    delete single;

    // Test 3: Full binary tree.
    //        1
    //       / \
    //      2   3
    //     / \ / \
    //    4  5 6  7
    Node* root3 = new Node(1);
    root3->left = new Node(2);
    root3->right = new Node(3);
    root3->left->left = new Node(4);
    root3->left->right = new Node(5);
    root3->right->left = new Node(6);
    root3->right->right = new Node(7);
    assert(boundaryTraversal(root3) == std::vector<int>({1,2,4,5,6,7,3}));
    // Cleanup not shown for brevity.

    // Test 4: Left skewed tree.
    //    1
    //   /
    //  2
    // /
    //3
    Node* root4 = new Node(1);
    root4->left = new Node(2);
    root4->left->left = new Node(3);
    assert(boundaryTraversal(root4) == std::vector<int>({1,2,3}));

    // Test 5: Right skewed tree.
    //    1
    //     \
    //      2
    //       \
    //        3
    Node* root5 = new Node(1);
    root5->right = new Node(2);
    root5->right->right = new Node(3);
    assert(boundaryTraversal(root5) == std::vector<int>({1,3,2}));

    // Test 6: Only root with two children that are leaves.
    //    1
    //   / \
    //  2   3
    Node* root6 = new Node(1);
    root6->left = new Node(2);
    root6->right = new Node(3);
    assert(boundaryTraversal(root6) == std::vector<int>({1,2,3}));

    // Test 7: Complex tree where left child is leaf.
    //      1
    //     / \
    //    2   5
    //       / \
    //      6   7
    Node* root7 = new Node(1);
    root7->left = new Node(2);
    root7->right = new Node(5);
    root7->right->left = new Node(6);
    root7->right->right = new Node(7);
    assert(boundaryTraversal(root7) == std::vector<int>({1,2,6,7,5}));

    return 0;
}
