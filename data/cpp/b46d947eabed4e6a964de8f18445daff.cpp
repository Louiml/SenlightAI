Write a C++ function that takes the root of a binary tree and returns a vector of integers representing the bottom view of the tree from left to right. The bottom view contains the nodes that are visible when the tree is viewed from directly below, where multiple nodes with the same horizontal distance are represented by the one that appears deepest in the tree (and, if at the same depth, the rightmost one). The horizontal distance of the root is 0; a left child is at distance −1, and a right child is at distance +1. The tree may be empty, in which case return an empty vector. Assume all node values are distinct integers. Your solution must work for any valid binary tree and handle large inputs efficiently (up to 10^5 nodes). Implement the function as `vector<int> bottomView(Node* root)` where `Node` is defined as a struct with integer `data` and pointers `left` and `right`. Provide a self-contained implementation with all necessary headers and a `using namespace std;`. Do not write a `main` function or any test harness in this section; the function must be directly callable.
The main algorithm uses a level-order traversal (BFS) to visit nodes in increasing depth. For each node, we keep track of its horizontal distance (hd) from the root. We use a queue of pairs `(node, hd)` starting with `(root, 0)`. For each popped node, we update a map `map<int,int>` that stores the latest node value encountered for each horizontal distance. Because BFS processes nodes level by level, the last value stored for any horizontal distance will be the deepest node at that distance. If two nodes have the same horizontal distance and same depth (e.g., a left child of a right child vs. a right child of a left child), the one that appears later in the BFS (i.e., the rightmost one when processed in left-to-right order) will overwrite the previous, which matches the required "rightmost" tie-breaker. After the traversal, we iterate through the map keys (which are sorted ascending) and collect the stored values to form the bottom view, left to right. Edge cases: empty tree returns empty vector; a tree with only a root yields a single-element vector; nodes with only left or only right subtrees produce sparse horizontal distances. Time complexity: O(N) for traversing all nodes and O(N) for iterating the map, where N is the number of nodes. Space complexity: O(N) for the queue and O(N) for the map (since horizontal distances are bounded by N in worst case). The map uses O(N) auxiliary space in total.
#include <bits/stdc++.h>
using namespace std;

// Define the tree node structure as per problem specification.
struct Node {
    int data;
    Node* left;
    Node* right;
};

/**
 * Return the bottom view of a binary tree as a vector of integers,
 * ordered from leftmost horizontal distance to rightmost.
 * The bottom view contains the deepest node (or rightmost if tied)
 * for each horizontal distance.
 */
vector<int> bottomView(Node* root) {
    vector<int> result;
    if (root == nullptr) {
        return result;
    }

    // Map from horizontal distance to node value (deepest seen so far).
    map<int, int> distanceToValue;

    // Queue for level-order traversal, storing node and its horizontal distance.
    queue<pair<Node*, int>> q;
    q.push({root, 0});

    while (!q.empty()) {
        Node* current = q.front().first;
        int hd = q.front().second;
        q.pop();

        // Overwrite with the current (deeper or rightmost) value for this hd.
        distanceToValue[hd] = current->data;

        if (current->left != nullptr) {
            q.push({current->left, hd - 1});
        }
        if (current->right != nullptr) {
            q.push({current->right, hd + 1});
        }
    }

    // Map keys are already sorted by horizontal distance.
    for (const auto& entry : distanceToValue) {
        result.push_back(entry.second);
    }
    return result;
}
#include <bits/stdc++.h>
using namespace std;

// Copy of the Node struct and bottomView function (or include the solution above).
// For brevity in this test section, we assume the function is already defined.
// We'll write a helper to build a tree for testing.

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// The solution function from above (included for completeness in the test).
vector<int> bottomView(Node* root) {
    vector<int> result;
    if (root == nullptr) return result;
    map<int, int> distanceToValue;
    queue<pair<Node*, int>> q;
    q.push({root, 0});
    while (!q.empty()) {
        Node* current = q.front().first;
        int hd = q.front().second;
        q.pop();
        distanceToValue[hd] = current->data;
        if (current->left) q.push({current->left, hd - 1});
        if (current->right) q.push({current->right, hd + 1});
    }
    for (const auto& entry : distanceToValue) result.push_back(entry.second);
    return result;
}

// Helper to delete tree (not required for tests but good practice).
void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    // Test 1: Empty tree.
    {
        Node* root = nullptr;
        assert(bottomView(root).empty());
    }

    // Test 2: Single node tree.
    {
        Node* root = new Node(10);
        assert(bottomView(root) == vector<int>({10}));
        deleteTree(root);
    }

    // Test 3: Simple tree: root with left and right children.
    //         1
    //        / \
    //       2   3
    // Bottom view: 2 1 3
    {
        Node* root = new Node(1);
        root->left = new Node(2);
        root->right = new Node(3);
        assert(bottomView(root) == vector<int>({2, 1, 3}));
        deleteTree(root);
    }

    // Test 4: Deeper nodes override shallower ones at same horizontal distance.
    //         1
    //        / \
    //       2   3
    //        \
    //         4
    // Horizontal distances: root 0, left -1 (2), right +1 (3), node 4 is at -1 (as right child of 2)
    // Bottom view: 4 1 3
    {
        Node* root = new Node(1);
        root->left = new Node(2);
        root->right = new Node(3);
        root->left->right = new Node(4);
        assert(bottomView(root) == vector<int>({4, 1, 3}));
        deleteTree(root);
    }

    // Test 5: Both children at same hd but different depths; rightmost should win.
    //         1
    //        / \
    //       2   3
    //      /   /
    //     4   5
    // hd of 4 = -1 (left of 2), hd of 5 = 0 (left of 3) — actually hd of 5 is 0? Let's compute: root=0, 3=+1, 5 (left of 3) = 0. So hd 0 has nodes root (depth0) and 5 (depth2). Bottom view picks 5. Values: 4 at hd -1, 5 at hd 0, 3 at hd +1. Bottom view: 4 5 3.
    {
        Node* root = new Node(1);
        root->left = new Node(2);
        root->right = new Node(3);
        root->left->left = new Node(4);
        root->right->left = new Node(5);
        assert(bottomView(root) == vector<int>({4, 5, 3}));
        deleteTree(root);
    }

    // Test 6: Tie at same hd and depth; rightmost should win.
    //         1
    //        / \
    //       2   3
    //          / \
    //         4   5
    // hd -1: node 2 only. hd 0: root. hd 1: 3. hd 2: 5? Let's compute: 3 at +1, 4 at 0, 5 at +2. So hd 0 has root (depth0) and 4 (depth1) – picks 4. hd +1 has only 3. hd +2 has 5. Bottom view: 2 4 3 5.
    {
        Node* root = new Node(1);
        root->left = new Node(2);
        root->right = new Node(3);
        root->right->left = new Node(4);
        root->right->right = new Node(5);
        assert(bottomView(root) == vector<int>({2, 4, 3, 5}));
        deleteTree(root);
    }

    // Test 7: More complex tree with multiple levels.
    //         20
    //        /  \
    //       8    22
    //      / \     \
    //     5   3     25
    //        / \
    //       10  14
    // Horizontal distances: 20:0, 8:-1, 22:+1, 5:-2, 3:0, 25:+2, 10:-1, 14:+1
    // Bottom view (deepest for each hd): hd -2:5, -1:10, 0:3, +1:14, +2:25 → 5 10 3 14 25
    {
        Node* root = new Node(20);
        root->left = new Node(8);
        root->right = new Node(22);
        root->left->left = new Node(5);
        root->left->right = new Node(3);
        root->right->right = new Node(25);
        root->left->right->left = new Node(10);
        root->left->right->right = new Node(14);
        assert(bottomView(root) == vector<int>({5, 10, 3, 14, 25}));
        deleteTree(root);
    }

    cout << "All tests passed!" << endl;
    return 0;
}
