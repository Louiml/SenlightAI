// Write a C++ function `vector<int> verticalOrderTraversal(Node* root)` that takes the root of a binary tree (where each `Node` contains an integer `data` and pointers to `left` and `right` children) and returns a vector of integers representing the vertical order traversal of the tree. In vertical order traversal, nodes are grouped by their horizontal distance (hd) from the root (root has hd = 0, moving left decreases hd by 1, moving right increases hd by 1). Within the same horizontal distance, nodes are ordered by their level (depth, root at level 0), and within the same (hd, level) pair, nodes are ordered left-to-right as encountered during a breadth-first search (level-order) traversal. The final output vector must list all nodes grouped first by increasing hd, then by increasing level within each hd, and finally by the order they were visited. If the tree is empty, return an empty vector. Assume the `Node` structure is already defined as: `struct Node { int data; Node* left; Node* right; };`. The function must be efficiently implemented and handle arbitrary binary tree shapes, including skewed and unbalanced trees.
The solution uses a breadth-first search (BFS) traversal with a queue that stores a triple: the current node, its horizontal distance (hd), and its level. A nested map `map<int, map<int, vector<int>>>` is used to store values: the outer key is hd, the inner key is level, and the vector stores node values in the order they are encountered during BFS. Starting from the root with (hd=0, lvl=0), we push it into the queue. For each node popped, we append its data to the corresponding position in the map. Then, if the node has a left child, we push it with (hd-1, lvl+1); if it has a right child, we push it with (hd+1, lvl+1). Because BFS processes nodes level by level, the order within the same (hd, level) is naturally left-to-right. After processing all nodes, we iterate over the outer map (which is sorted by hd ascending), then over the inner map (sorted by level ascending), and finally over the vector to build the answer. Edge cases include an empty tree (return empty vector) and single-node trees. Time complexity is O(n log n) due to map insertions, where n is the number of nodes, but with a hash map it could be O(n); however, using `map` ensures sorted hd order, so O(n log n) is acceptable. Space complexity is O(n) for the queue and the map.
#include <map>
#include <queue>
#include <vector>

// Definition for a binary tree node.
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Returns the vertical order traversal of a binary tree as a vector of ints.
std::vector<int> verticalOrderTraversal(Node* root) {
    // Map: hd -> level -> list of node values in BFS order
    std::map<int, std::map<int, std::vector<int>>> nodes;
    // Queue: node, (hd, level)
    std::queue<std::pair<Node*, std::pair<int, int>>> q;
    std::vector<int> ans;

    if (root == nullptr) {
        return ans;
    }

    q.push({root, {0, 0}});  // root at hd=0, level=0

    while (!q.empty()) {
        auto temp = q.front();
        q.pop();

        Node* frontNode = temp.first;
        int hd = temp.second.first;
        int lvl = temp.second.second;

        nodes[hd][lvl].push_back(frontNode->data);

        if (frontNode->left) {
            q.push({frontNode->left, {hd - 1, lvl + 1}});
        }
        if (frontNode->right) {
            q.push({frontNode->right, {hd + 1, lvl + 1}});
        }
    }

    // Flatten the map into the answer vector
    for (const auto& hd_entry : nodes) {
        for (const auto& lvl_entry : hd_entry.second) {
            for (int val : lvl_entry.second) {
                ans.push_back(val);
            }
        }
    }
    return ans;
}
#include <cassert>

int main() {
    // Test 1: Empty tree
    Node* empty = nullptr;
    assert(verticalOrderTraversal(empty).empty());

    // Test 2: Single node
    Node* single = new Node(5);
    assert(verticalOrderTraversal(single) == std::vector<int>({5}));

    // Test 3: Simple tree: root=1, left=2, right=3
    Node* root1 = new Node(1);
    root1->left = new Node(2);
    root1->right = new Node(3);
    // hd=-1: [2], hd=0: [1,3] (level 0 root, level 1 right? Actually BFS: root at lvl0, children at lvl1; hd0 has root lvl0, right at lvl1? No, right is hd+1=1, so hd0 only root)
    // Expected: hd=-1: level0: [2]; hd=0: level0: [1]; hd=1: level1: [3] -> [2,1,3]
    assert(verticalOrderTraversal(root1) == std::vector<int>({2, 1, 3}));

    // Test 4: More complex tree
    //        1
    //       / \
    //      2   3
    //     / \   \
    //    4   5   6
    // Expected BFS order: root(1, hd0,lvl0), 2(hd-1,lvl1), 3(hd+1,lvl1), 4(hd-2,lvl2),5(hd0,lvl2),6(hd+2,lvl2)
    // Map: hd-2->lvl2->[4]; hd-1->lvl1->[2]; hd0->lvl0->[1], lvl2->[5]; hd1->lvl1->[3]; hd2->lvl2->[6]
    // Output: [4,2,1,5,3,6]
    Node* root2 = new Node(1);
    root2->left = new Node(2);
    root2->right = new Node(3);
    root2->left->left = new Node(4);
    root2->left->right = new Node(5);
    root2->right->right = new Node(6);
    assert(verticalOrderTraversal(root2) == std::vector<int>({4, 2, 1, 5, 3, 6}));

    // Test 5: Skewed left tree
    Node* root3 = new Node(1);
    root3->left = new Node(2);
    root3->left->left = new Node(3);
    // hd: 1->0, 2->-1, 3->-2; levels: 0,1,2
    // Expected: [3,2,1]
    assert(verticalOrderTraversal(root3) == std::vector<int>({3, 2, 1}));

    // Test 6: Tree with overlapping hd and level (multiple nodes same hd and level from BFS)
    //        1
    //       / \
    //      2   3
    //     / \ / \
    //    4  5 6  7
    // BFS: 1(hd0,l0), 2(hd-1,l1),3(hd1,l1),4(hd-2,l2),5(hd0,l2),6(hd0,l2),7(hd2,l2)
    // Note: hd0 has root at lvl0, and nodes 5 and 6 at lvl2 in BFS order
    // Expected: hd-2:[4], hd-1:[2], hd0:[1,5,6], hd1:[3], hd2:[7] -> [4,2,1,5,6,3,7]
    Node* root4 = new Node(1);
    root4->left = new Node(2);
    root4->right = new Node(3);
    root4->left->left = new Node(4);
    root4->left->right = new Node(5);
    root4->right->left = new Node(6);
    root4->right->right = new Node(7);
    assert(verticalOrderTraversal(root4) == std::vector<int>({4,2,1,5,6,3,7}));

    // Clean up (optional in test, but not required for asserts)
    // In a full program, you'd delete nodes to avoid leaks.
    return 0;
}
