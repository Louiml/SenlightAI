Write a C++ function `vector<int> iterativePostorder(Node* root)` that performs an iterative postorder traversal of a binary tree and returns a vector of node values in postorder (left, right, root) sequence. The tree is represented using the provided `Node` structure with integer `data`, and `left`/`right` child pointers. The function must not use recursion and must handle an empty tree (returning an empty vector). You may assume the input tree is valid with no cycles, and node values can be any integers.

// The standard recursive postorder visits left, then right, then root. For an iterative version, we can use a stack that stores each node along with a state (1 = need to process left child, 2 = need to process right child, 3 = ready to output). Starting with the root at state 1, we pop a pair: if state is 1, we increment the state, push it back, then push the left child (if any) with state 1. If state is 2, we increment and push back, then push the right child (if any) with state 1. If state is 3, we output the node's value. This simulates the recursion call stack explicitly. Edge cases: an empty root returns an empty vector; a leaf node transitions through states 1→2→3 and outputs its value. Time complexity is O(n) where n is the number of nodes, since each node is pushed and popped at most three times. Space complexity is O(h) for the stack, where h is the tree height (worst-case O(n) for a skewed tree).

#include <vector>
#include <stack>

// Node structure as provided
struct Node {
    int data;
    Node* left;
    Node* right;
};

// Perform iterative postorder traversal
// Returns vector of node values in postorder (left, right, root)
std::vector<int> iterativePostorder(Node* root) {
    std::vector<int> result;
    if (root == nullptr) return result;

    // Stack stores pairs of (node, state)
    // state 1: left child not processed yet
    // state 2: right child not processed yet
    // state 3: both processed, ready to output
    std::stack<std::pair<Node*, int>> st;
    st.push({root, 1});

    while (!st.empty()) {
        auto [node, state] = st.top();
        st.pop();

        if (state == 1) {
            st.push({node, 2});
            if (node->left != nullptr) st.push({node->left, 1});
        } else if (state == 2) {
            st.push({node, 3});
            if (node->right != nullptr) st.push({node->right, 1});
        } else {
            result.push_back(node->data);
        }
    }

    return result;
}

#include <cassert>
#include <vector>
#include <string>
#include <sstream>
#include <queue>

// Include the Node struct and iterativePostorder function here

// Helper to build tree from level-order string (like the original)
Node* buildTree(const std::string& str) {
    if (str.empty() || str[0] == 'N') return nullptr;
    std::istringstream iss(str);
    std::vector<std::string> tokens;
    for (std::string s; iss >> s;) tokens.push_back(s);
    Node* root = new Node{std::stoi(tokens[0]), nullptr, nullptr};
    std::queue<Node*> q;
    q.push(root);
    int i = 1;
    while (!q.empty() && i < tokens.size()) {
        Node* curr = q.front(); q.pop();
        if (tokens[i] != "N") {
            curr->left = new Node{std::stoi(tokens[i]), nullptr, nullptr};
            q.push(curr->left);
        }
        i++;
        if (i >= tokens.size()) break;
        if (tokens[i] != "N") {
            curr->right = new Node{std::stoi(tokens[i]), nullptr, nullptr};
            q.push(curr->right);
        }
        i++;
    }
    return root;
}

// Simple recursive inorder for cleanup? Not needed for tests

int main() {
    // Test 1: Empty tree
    assert(iterativePostorder(nullptr).empty());

    // Test 2: Single node
    Node* single = new Node{5, nullptr, nullptr};
    assert(iterativePostorder(single) == std::vector<int>{5});

    // Test 3: Small tree from level-order "1 2 3"
    Node* t1 = buildTree("1 2 3");
    assert(iterativePostorder(t1) == std::vector<int>({2, 3, 1}));

    // Test 4: Tree from "1 2 3 4 5 6 7"
    Node* t2 = buildTree("1 2 3 4 5 6 7");
    // Postorder: left subtree (4,5,2), right subtree (6,7,3), root 1
    assert(iterativePostorder(t2) == std::vector<int>({4, 5, 2, 6, 7, 3, 1}));

    // Test 5: Skewed left tree: "1 2 N 3 N 4"
    Node* t3 = buildTree("1 2 N 3 N 4");
    // path: 1->2->3->4 (all left children)
    assert(iterativePostorder(t3) == std::vector<int>({4, 3, 2, 1}));

    // Test 6: Skewed right tree: "1 N 2 N 3"
    Node* t4 = buildTree("1 N 2 N 3");
    assert(iterativePostorder(t4) == std::vector<int>({3, 2, 1}));

    // Test 7: Tree with negative values: "10 -5 15 N N 3 7"
    Node* t5 = buildTree("10 -5 15 N N 3 7");
    // Postorder: -5, 3, 7, 15, 10
    assert(iterativePostorder(t5) == std::vector<int>({-5, 3, 7, 15, 10}));

    // Cleanup (not required for correctness, but good practice in full program)
    // In a full program you would delete all nodes, but for brevity omitted.

    return 0;
}
