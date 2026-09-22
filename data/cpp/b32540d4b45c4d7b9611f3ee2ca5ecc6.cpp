Write a C++ function `bool hasSumProperty(const Node* root)` that checks whether every non-leaf node in a binary tree satisfies the "sum property": the value stored in the node equals the sum of the values of its existing children. For a leaf node (both children null), the property is trivially satisfied. A null tree (empty) is also considered to satisfy the property. The function must be `const`-correct and accept a pointer to a constant `Node`. The `Node` structure is defined as having an integer `data` field and left/right child pointers. Do not modify the tree. Return `true` if the entire tree satisfies the property, otherwise `false`.

#include <cassert>

int main() {
    // Test 1: Empty tree
    assert(hasSumProperty(nullptr) == true);
    
    // Test 2: Single leaf node
    Node* n1 = new Node(5);
    assert(hasSumProperty(n1) == true);
    delete n1;
    
    // Test 3: Simple valid tree: root=10, left=4, right=6
    Node* root3 = new Node(10);
    root3->left = new Node(4);
    root3->right = new Node(6);
    assert(hasSumProperty(root3) == true);
    delete root3->left;
    delete root3->right;
    delete root3;
    
    // Test 4: Invalid tree: root=9, left=4, right=6 (sum=10 != 9)
    Node* root4 = new Node(9);
    root4->left = new Node(4);
    root4->right = new Node(6);
    assert(hasSumProperty(root4) == false);
    delete root4->left;
    delete root4->right;
    delete root4;
    
    // Test 5: Node with one child: root=7, left=7 (valid because right absent contributes 0)
    Node* root5 = new Node(7);
    root5->left = new Node(7);
    assert(hasSumProperty(root5) == true);
    delete root5->left;
    delete root5->right; // right is nullptr, safe to delete
    delete root5;
    
    // Test 6: Node with one child, invalid: root=5, left=7
    Node* root6 = new Node(5);
    root6->left = new Node(7);
    assert(hasSumProperty(root6) == false);
    delete root6->left;
    delete root6->right;
    delete root6;
    
    // Test 7: Deeper valid tree:
    //        10
    //       /  \
    //      4    6
    //     / \    \
    //    1   3    6
    Node* root7 = new Node(10);
    root7->left = new Node(4);
    root7->right = new Node(6);
    root7->left->left = new Node(1);
    root7->left->right = new Node(3);
    root7->right->right = new Node(6);
    assert(hasSumProperty(root7) == true);
    delete root7->left->left;
    delete root7->left->right;
    delete root7->left;
    delete root7->right->right;
    delete root7->right;
    delete root7;
    
    // Test 8: Deeper invalid tree (right subtree fails)
    //        10
    //       /  \
    //      4    6
    //     / \    \
    //    2   2    7   (right child 6 has sum 7 != 6)
    Node* root8 = new Node(10);
    root8->left = new Node(4);
    root8->right = new Node(6);
    root8->left->left = new Node(2);
    root8->left->right = new Node(2);
    root8->right->right = new Node(7);
    assert(hasSumProperty(root8) == false);
    delete root8->left->left;
    delete root8->left->right;
    delete root8->left;
    delete root8->right->right;
    delete root8->right;
    delete root8;
    
    // Test 9: Negative values: root=0, left=-2, right=2
    Node* root9 = new Node(0);
    root9->left = new Node(-2);
    root9->right = new Node(2);
    assert(hasSumProperty(root9) == true);
    delete root9->left;
    delete root9->right;
    delete root9;
    
    // Test 10: Negative values invalid: root=1, left=-2, right=2 (sum=0 != 1)
    Node* root10 = new Node(1);
    root10->left = new Node(-2);
    root10->right = new Node(2);
    assert(hasSumProperty(root10) == false);
    delete root10->left;
    delete root10->right;
    delete root10;
    
    return 0;
}

#include <cstddef>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Checks whether every non-leaf node in the tree has value equal to the sum of its children's values.
bool hasSumProperty(const Node* root) {
    if (root == nullptr) {
        return true;
    }
    // Leaf node: property holds trivially.
    if (root->left == nullptr && root->right == nullptr) {
        return true;
    }
    
    int childSum = 0;
    if (root->left != nullptr) {
        childSum += root->left->data;
    }
    if (root->right != nullptr) {
        childSum += root->right->data;
    }
    
    if (childSum != root->data) {
        return false;
    }
    
    return hasSumProperty(root->left) && hasSumProperty(root->right);
}

// The solution uses a recursive depth-first traversal. At each node, we first handle the base case: if the node is null, return `true`. If the node is a leaf (both children null), return `true`. Otherwise, compute the sum of the data values of the left and right children (only counting those that exist). If this computed sum does not equal the node’s own data, return `false` immediately. Then recursively check both subtrees; if either subtree fails, return `false`. Otherwise, return `true`. This approach visits each node exactly once. Edge cases include: null tree, single node (leaf), nodes with only one child, negative values, and large values (no overflow concerns for typical ints, but note the summation could overflow if values are extreme; however, for the standard problem we assume values fit in an int). Time complexity is O(n) where n is the number of nodes, and space complexity is O(h) due to the recursion stack, where h is the height of the tree (worst-case O(n) for skewed trees, average O(log n) for balanced).
