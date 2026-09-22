// Given a binary search tree (BST) where each node contains an integer value (possibly negative, zero, or positive) and a target integer `target`, write a C++ function `bool hasPairWithSum(Node* root, int target)` that returns `true` if there exist two distinct nodes in the tree whose values sum to `target`, and `false` otherwise. The BST property may hold, but the function must work for any binary tree (not necessarily BST). You may assume the tree has at least one node. The function should not modify the tree and must handle duplicates only if they appear as separate nodes (two different nodes with the same value count as distinct values if they are separate nodes in the tree). Return `true` if such a pair exists; otherwise `false`.

The solution uses an in‑order traversal of the tree (though the tree does not need to be a BST, in‑order traversal of any binary tree works) and maintains a hash set of previously visited node values. For each node, we compute the complement `target - node->data`. If that complement already exists in the hash set, then we have found two distinct nodes that sum to the target, so we set a flag and stop. Otherwise, we insert the current node’s value into the set and continue the traversal. Because the traversal visits every node exactly once, we check all possible pairs where one element is the current node and the other is any earlier‑visited node. This ensures that if any pair exists, it will be found. Edge cases: if the tree has only one node, the set will be empty initially, and no pair can exist; if two nodes have the same value and the target is twice that value, then on the second occurrence, the complement (equal to the value) will already be in the set, so the pair is correctly identified (as long as they are two different nodes). Time complexity is O(n) where n is the number of nodes, since each node is visited once and each set operation is O(1) on average. Space complexity is O(n) in the worst case for the hash set (which stores up to n-1 distinct values).

#include <unordered_set>

// Node structure for the binary tree
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Helper function to perform in-order traversal and check pairs
void traverseAndCheck(Node* node, int target, bool& found, std::unordered_set<int>& visited) {
    if (node == nullptr || found) {
        return;
    }
    // Left subtree
    traverseAndCheck(node->left, target, found, visited);
    
    // Check if complement exists
    int complement = target - node->data;
    if (visited.find(complement) != visited.end()) {
        found = true;
        return;
    }
    // Insert current value into set
    visited.insert(node->data);
    
    // Right subtree
    traverseAndCheck(node->right, target, found, visited);
}

// Main function: returns true if there exist two distinct nodes with sum equal to target
bool hasPairWithSum(Node* root, int target) {
    bool found = false;
    std::unordered_set<int> visited;
    traverseAndCheck(root, target, found, visited);
    return found;
}

#include <cassert>
#include <iostream>

// Solution code from above (Node, traverseAndCheck, hasPairWithSum) would be placed here
// for the test harness, but since the task says "Output code only", I'll include the functions
// in the test block as well for completeness. In practice, the above code would be included.

int main() {
    // Test 1: Simple BST with pair
    Node* root1 = new Node(10);
    root1->left = new Node(5);
    root1->right = new Node(15);
    root1->left->left = new Node(3);
    root1->left->right = new Node(7);
    assert(hasPairWithSum(root1, 17) == true);  // 10+7 or 5+12? 10+7=17, yes
    assert(hasPairWithSum(root1, 20) == true);  // 5+15=20
    assert(hasPairWithSum(root1, 22) == false); // no pair

    // Test 2: Single node
    Node* root2 = new Node(5);
    assert(hasPairWithSum(root2, 10) == false);
    assert(hasPairWithSum(root2, 5) == false);  // needs two distinct nodes

    // Test 3: Tree with duplicate values (two separate nodes)
    Node* root3 = new Node(4);
    root3->left = new Node(4);
    root3->right = new Node(6);
    assert(hasPairWithSum(root3, 8) == true);   // 4+4 (two separate nodes)
    assert(hasPairWithSum(root3, 10) == true);  // 4+6
    assert(hasPairWithSum(root3, 12) == false);

    // Test 4: Negative values
    Node* root4 = new Node(-3);
    root4->left = new Node(-7);
    root4->right = new Node(2);
    assert(hasPairWithSum(root4, -10) == true); // -3 + -7 = -10
    assert(hasPairWithSum(root4, -1) == true);  // -3 + 2 = -1
    assert(hasPairWithSum(root4, 5) == false);

    // Test 5: Unbalanced tree, pair at leaf and root
    Node* root5 = new Node(1);
    root5->right = new Node(2);
    root5->right->right = new Node(3);
    assert(hasPairWithSum(root5, 4) == true);   // 1+3
    assert(hasPairWithSum(root5, 5) == false);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
