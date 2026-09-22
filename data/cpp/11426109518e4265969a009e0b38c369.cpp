/*
Write a C++ function that takes the root of a binary tree where each node stores an integer value, and transforms the tree in-place so that every node's data becomes the sum of the values of the nodes in its left and right subtrees in the original tree. The original value of a node is discarded after transformation. For a leaf node (no children), its new value becomes 0. The transformation must be applied to the entire tree, and the tree's structure (pointers/links) must remain unchanged. The function should return nothing (void) and must handle an empty tree (null root) gracefully without any effect.
*/
#include <bits/stdc++.h>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Recursive helper that transforms the subtree rooted at 'node'
// and returns the sum of all original values in that subtree.
int transformAndGetSum(Node* node) {
    if (!node) return 0;
    
    int leftSum = transformAndGetSum(node->left);
    int rightSum = transformAndGetSum(node->right);
    
    int originalValue = node->data;
    node->data = leftSum + rightSum;  // new value = sum of left and right subtrees
    
    return originalValue + leftSum + rightSum;  // total original sum of this subtree
}

// Transforms the given tree so each node's data becomes the sum of
// the values in its left and right subtrees in the original tree.
void toSumTree(Node* root) {
    transformAndGetSum(root);
}
#include <bits/stdc++.h>
using namespace std;

// Node definition (same as above for completeness)
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Function declarations (to be tested)
void toSumTree(Node* root);
int transformAndGetSum(Node* node) {
    if (!node) return 0;
    int leftSum = transformAndGetSum(node->left);
    int rightSum = transformAndGetSum(node->right);
    int originalValue = node->data;
    node->data = leftSum + rightSum;
    return originalValue + leftSum + rightSum;
}
void toSumTree(Node* root) {
    transformAndGetSum(root);
}

// Helper to build tree from level-order string like "1 2 3 N N 4 5"
Node* buildTree(const string& s) {
    if (s.empty() || s[0] == 'N') return nullptr;
    istringstream iss(s);
    string val;
    iss >> val;
    Node* root = new Node(stoi(val));
    queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* curr = q.front();
        q.pop();
        if (iss >> val && val != "N") {
            curr->left = new Node(stoi(val));
            q.push(curr->left);
        }
        if (iss >> val && val != "N") {
            curr->right = new Node(stoi(val));
            q.push(curr->right);
        }
    }
    return root;
}

// Helper to get inorder traversal as vector for comparison
void inorderVec(Node* node, vector<int>& res) {
    if (!node) return;
    inorderVec(node->left, res);
    res.push_back(node->data);
    inorderVec(node->right, res);
}

// Helper to delete tree
void freeTree(Node* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

int main() {
    // Test 1: Single node
    Node* root1 = new Node(5);
    toSumTree(root1);
    vector<int> arr1;
    inorderVec(root1, arr1);
    assert(arr1.size() == 1 && arr1[0] == 0);
    freeTree(root1);

    // Test 2: Simple two-level tree: 1 2 3
    Node* root2 = buildTree("1 2 3");
    toSumTree(root2);
    vector<int> arr2;
    inorderVec(root2, arr2);
    // Original: left=2, right=3, root=1. New: left->0, right->0, root->5
    vector<int> expected2 = {0, 5, 0};
    assert(arr2 == expected2);
    freeTree(root2);

    // Test 3: Three-level tree: 10 20 30 40 50 N N
    Node* root3 = buildTree("10 20 30 40 50 N N");
    toSumTree(root3);
    vector<int> arr3;
    inorderVec(root3, arr3);
    // Original values: 40,50 under 20; 30 leaf; 10 root.
    // After: 40->0, 50->0, so 20's new = 0+0=0; 30->0; root's new = (sum of left subtree originals = 20+40+50=110) + (sum of right subtree originals = 30) = 140.
    vector<int> expected3 = {0, 0, 0, 140, 0}; // inorder: left,20,left's left,left's right,root,right? Actually inorder: [40's left? no] – properly, tree: 10(L:20(L:40,L:50), R:30). Inorder: 40,20,50,10,30. After transform: 40->0, 20->0+0=0, 50->0, 10->(20+40+50)+(30)=140, 30->0. So inorder: [0,0,0,140,0].
    assert(arr3 == expected3);
    freeTree(root3);

    // Test 4: Empty tree
    Node* root4 = nullptr;
    toSumTree(root4); // should not crash

    // Test 5: Only left subtree: 1 2 N 3 N
    Node* root5 = buildTree("1 2 N 3 N");
    toSumTree(root5);
    vector<int> arr5;
    inorderVec(root5, arr5);
    // Original: 3 under 2 under 1. New: 3->0, 2->0, 1->(2+3)=5. Inorder: [3,2,1] -> after: [0,0,5]
    vector<int> expected5 = {0, 0, 5};
    assert(arr5 == expected5);
    freeTree(root5);

    cout << "All tests passed." << endl;
    return 0;
}
// The solution uses a post-order traversal (left, right, root). For each node, we recursively compute the sum of all nodes in its left subtree and the sum of all nodes in its right subtree from the original tree. To achieve this, the recursive helper function `solve` returns the original sum of the entire subtree rooted at the given node (including the node's original data). Inside `solve`, we first recursively call on the left and right children, obtaining their original subtree sums (`a` and `b`). We then save the current node's original data (`dat`). Next, we set `node->data = a + b` (the sum of the two subtree sums, which is exactly the sum of all nodes in the left and right subtrees excluding the node itself). Finally, the helper returns `dat + a + b`, which is the total sum of the original subtree rooted at this node (including the node's original value), so that the parent can use it to compute its own transformed value. This ensures that the computation uses original values and does not get corrupted by the updates already performed on children. Edge cases: an empty tree (null root) – the helper immediately returns 0 and the main function does nothing; a leaf node – both children are null, so `a=0`, `b=0`, node's data becomes 0, and the function returns the original leaf value. The time complexity is O(n), where n is the number of nodes, as each node is visited exactly once. The space complexity is O(h) due to recursion stack, where h is the height of the tree (worst-case O(n) for a skewed tree, average O(log n) for balanced trees).
