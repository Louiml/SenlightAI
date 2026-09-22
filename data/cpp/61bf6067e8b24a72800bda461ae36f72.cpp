Write a C++ function `int findClosestValue(Node* root, int target)` that accepts the root of a valid binary search tree (BST) (where each node contains an integer value, with left subtree values strictly less and right subtree values strictly greater) and returns the value in the BST that is closest to the given `target`. If multiple values are equally close (e.g., target is exactly between two values), return the smaller value. The tree may be empty; in that case, return `-1`. Your function should avoid scanning the entire tree inefficiently—use the BST ordering to guide the search. The function must be `const`-correct (i.e., it should not modify the tree) and must handle negative numbers, duplicates not present by BST definition, and large inputs gracefully.

The solution uses a recursive or iterative traversal of the BST that exploits its ordering property. Starting from the root, maintain a variable `best` that holds the current closest value found (initialize it to `root->val` if root exists, or handle empty case). At each node, update `best` to the node’s value if the absolute difference between the node’s value and target is strictly smaller than the difference for the current `best`, or if the differences are equal but the node’s value is smaller (per the tie-breaking rule). Then decide whether to move left or right: if the target is less than the node’s value, the closest value could only be in the left subtree (since all right subtree values are even larger and thus farther from target), so recurse left; if target is greater, recurse right; if equal, the answer is immediately the node’s value (since difference zero is minimal and no tie can be smaller). The algorithm terminates when reaching a null pointer. Edge cases: empty tree returns -1; single-node tree returns that node’s value; target exactly equal to a node returns that node. Time complexity is O(h) where h is the height of the BST (could be O(n) in a skewed tree). Space complexity is O(h) for recursion stack (or O(1) if iterative).

#include <climits>
#include <cstdlib>

struct Node {
    int val;
    Node *left;
    Node *right;
    Node(int v) : val(v), left(nullptr), right(nullptr) {}
};

// Returns the value in the BST closest to target. Returns -1 if tree is empty.
// Tie-breaking: if two values are equally close, returns the smaller one.
int findClosestValue(Node* root, int target) {
    if (root == nullptr) {
        return -1;
    }
    
    int best = root->val;
    Node* current = root;
    
    while (current != nullptr) {
        int diffCurrent = std::abs(current->val - target);
        int diffBest = std::abs(best - target);
        
        if (diffCurrent < diffBest || 
            (diffCurrent == diffBest && current->val < best)) {
            best = current->val;
        }
        
        if (current->val == target) {
            return current->val; // Exact match, can't get closer
        } else if (target < current->val) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    return best;
}

#include <cassert>
#include <cstdlib>

// Helper to build a simple BST for testing
Node* createBST(const std::vector<int>& values) {
    if (values.empty()) return nullptr;
    Node* root = new Node(values[0]);
    for (size_t i = 1; i < values.size(); ++i) {
        Node* cur = root;
        while (true) {
            if (values[i] < cur->val) {
                if (!cur->left) { cur->left = new Node(values[i]); break; }
                cur = cur->left;
            } else {
                if (!cur->right) { cur->right = new Node(values[i]); break; }
                cur = cur->right;
            }
        }
    }
    return root;
}

// Helper to delete tree (not tested, but included for completeness)
void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    // Test 1: Basic tree
    Node* root1 = createBST({10, 5, 15, 3, 7, 12, 18});
    assert(findClosestValue(root1, 13) == 12);
    assert(findClosestValue(root1, 4) == 5);
    assert(findClosestValue(root1, 10) == 10);
    // Tie-breaking: target=8, values 7 and 12 are equally close (diff 1), choose smaller 7
    assert(findClosestValue(root1, 8) == 7);
    // Tie-breaking: target=9, 10 (diff 1) and 7 (diff 2), so 10
    assert(findClosestValue(root1, 9) == 10);
    deleteTree(root1);

    // Test 2: Empty tree
    Node* emptyRoot = nullptr;
    assert(findClosestValue(emptyRoot, 5) == -1);

    // Test 3: Single node
    Node* single = new Node(42);
    assert(findClosestValue(single, 100) == 42);
    assert(findClosestValue(single, -100) == 42);
    assert(findClosestValue(single, 42) == 42);
    delete single;

    // Test 4: Negative numbers and skewed tree
    Node* skewed = createBST({-10, -20, -30, -40});
    assert(findClosestValue(skewed, -35) == -30);
    assert(findClosestValue(skewed, -25) == -20);
    assert(findClosestValue(skewed, -15) == -10);
    deleteTree(skewed);

    // Test 5: Large tree (balanced) with exact middle
    Node* large = createBST({50, 25, 75, 12, 37, 62, 87});
    assert(findClosestValue(large, 60) == 62);
    assert(findClosestValue(large, 30) == 25);
    assert(findClosestValue(large, 55) == 50);
    deleteTree(large);

    return 0;
}
