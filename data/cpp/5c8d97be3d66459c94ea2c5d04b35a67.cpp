/*
Write a C++ function `bool hasPairWithSum(Node* root, int target)` that takes the root of a Binary Search Tree (BST) and an integer target, and returns `true` if there exist two distinct nodes whose data values sum to exactly the target, and `false` otherwise. The BST nodes follow the standard structure with `int data`, `Node* left`, and `Node* right`. The function must be efficient for a BST of arbitrary size, and must handle edge cases like an empty tree, a tree with a single node, and duplicate values. The function should not modify the tree.
*/

#include <stack>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

class BSTIterator {
private:
    std::stack<Node*> stack_;
    bool reverse_;
    void pushAll(Node* node) {
        if (!reverse_) {
            for (; node != nullptr; stack_.push(node), node = node->left);
        } else {
            for (; node != nullptr; stack_.push(node), node = node->right);
        }
    }
public:
    BSTIterator(Node* root, bool reverse) : reverse_(reverse) {
        pushAll(root);
    }
    bool hasNext() const {
        return !stack_.empty();
    }
    int next() {
        Node* current = stack_.top();
        stack_.pop();
        if (!reverse_) {
            pushAll(current->right);
        } else {
            pushAll(current->left);
        }
        return current->data;
    }
};

bool hasPairWithSum(Node* root, int target) {
    if (root == nullptr) {
        return false;
    }
    BSTIterator leftIt(root, false);
    BSTIterator rightIt(root, true);
    int left = leftIt.next();
    int right = rightIt.next();
    while (left < right) {
        int sum = left + right;
        if (sum == target) {
            return true;
        } else if (sum < target) {
            left = leftIt.next();
        } else {
            right = rightIt.next();
        }
    }
    return false;
}

#include <cassert>

int main() {
    // Test 1: Empty tree
    Node* empty = nullptr;
    assert(hasPairWithSum(empty, 10) == false);

    // Test 2: Single node
    Node* single = new Node(5);
    assert(hasPairWithSum(single, 10) == false);
    assert(hasPairWithSum(single, 5) == false);

    // Test 3: Simple BST with pair
    Node* root1 = new Node(10);
    root1->left = new Node(5);
    root1->right = new Node(15);
    assert(hasPairWithSum(root1, 20) == true); // 5+15
    assert(hasPairWithSum(root1, 25) == true); // 10+15
    assert(hasPairWithSum(root1, 10) == false); // small + root is 15, not 10

    // Test 4: More complex BST
    Node* root2 = new Node(8);
    root2->left = new Node(3);
    root2->left->left = new Node(1);
    root2->left->right = new Node(6);
    root2->left->right->left = new Node(4);
    root2->left->right->right = new Node(7);
    root2->right = new Node(10);
    root2->right->right = new Node(14);
    root2->right->right->left = new Node(13);
    assert(hasPairWithSum(root2, 14) == true);  // 1+13
    assert(hasPairWithSum(root2, 15) == true);  // 1+14 or 8+7
    assert(hasPairWithSum(root2, 9) == true);   // 3+6
    assert(hasPairWithSum(root2, 2) == false);
    assert(hasPairWithSum(root2, 30) == false);

    // Test 5: Duplicate values
    Node* root3 = new Node(5);
    root3->left = new Node(5);
    root3->right = new Node(10);
    assert(hasPairWithSum(root3, 10) == true); // 5+5 (two distinct nodes)
    assert(hasPairWithSum(root3, 15) == true); // 5+10

    // Test 6: Negative and positive values
    Node* root4 = new Node(0);
    root4->left = new Node(-5);
    root4->right = new Node(5);
    assert(hasPairWithSum(root4, 0) == true);  // -5+5
    assert(hasPairWithSum(root4, -5) == true); // -5+0
    assert(hasPairWithSum(root4, 1) == false);

    return 0;
}

// The standard approach is to use BST iterators to simulate a two-pointer technique on the in-order traversal without needing to store the entire inorder array. A BSTIterator that can traverse in both forward (increasing) and reverse (decreasing) order is created. One iterator starts at the smallest element (leftmost node), and another starts at the largest element (rightmost node). At each step, compare the sum of the two current elements:
// - If the sum equals target, return true.
// - If the sum is less than target, advance the forward iterator to get a larger element.
// - If the sum is greater than target, advance the reverse iterator to get a smaller element.
// The loop continues as long as the forward iterator's value is less than the reverse iterator's value, ensuring we check each pair exactly once and avoid checking a node with itself. Edge cases: empty tree returns false; single node cannot form a pair, so false; if target is twice a node's value, we must ensure we use two distinct nodes, which the iterator approach naturally handles because we never compare a node with itself when the left iterator is strictly less than the right iterator. Time complexity is O(n) in the worst case (visiting each node at most once across both iterators), and space complexity is O(h) where h is the tree height due to the stack used by the iterators (O(n) worst-case for a skewed tree).
