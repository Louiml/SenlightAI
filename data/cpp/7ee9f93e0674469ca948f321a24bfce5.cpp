/*
Write a C++ function that performs a level-order traversal of an N-ary tree (a tree where each node can have any number of children). The function takes a pointer to the root `Node` and returns a `std::vector<std::vector<int>>` where each inner vector contains the node values of one level, ordered from left to right across levels. Define a `Node` struct with an integer `val` and a `std::vector<Node*> children`. The tree may be empty (root is `nullptr`). The function must handle up to 5000 nodes and depth up to 1000, and must not modify the tree.
*/

#include <vector>
#include <queue>

// Definition for an N-ary tree node.
struct Node {
    int val;
    std::vector<Node*> children;
    Node() : val(0), children() {}
    Node(int _val) : val(_val), children() {}
    Node(int _val, std::vector<Node*> _children) : val(_val), children(_children) {}
};

// Perform level-order traversal of an N-ary tree.
std::vector<std::vector<int>> levelOrderTraversal(const Node* root) {
    std::vector<std::vector<int>> result;
    if (root == nullptr) {
        return result;
    }
    std::queue<const Node*> q;
    q.push(root);
    while (!q.empty()) {
        std::vector<int> level;
        int levelSize = q.size();
        for (int i = 0; i < levelSize; ++i) {
            const Node* node = q.front();
            q.pop();
            level.push_back(node->val);
            for (const Node* child : node->children) {
                q.push(child);
            }
        }
        result.push_back(level);
    }
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Test empty tree
    std::vector<std::vector<int>> result = levelOrderTraversal(nullptr);
    assert(result.empty());

    // Test single node
    Node n1(1);
    result = levelOrderTraversal(&n1);
    assert(result == std::vector<std::vector<int>>({{1}}));

    // Test simple 3-ary tree: root=1, children 3,2,4; child 3 has children 5,6
    Node n5(5), n6(6);
    std::vector<Node*> children3 = {&n5, &n6};
    Node n3(3, children3);
    Node n2(2), n4(4);
    std::vector<Node*> rootChildren = {&n3, &n2, &n4};
    Node root(1, rootChildren);
    result = levelOrderTraversal(&root);
    assert(result == std::vector<std::vector<int>>({{1}, {3,2,4}, {5,6}}));

    // Test tree with one level of many children
    Node c1(10), c2(11), c3(12);
    std::vector<Node*> children = {&c1, &c2, &c3};
    Node r2(100, children);
    result = levelOrderTraversal(&r2);
    assert(result == std::vector<std::vector<int>>({{100}, {10,11,12}}));

    // Test deeper tree (chain)
    Node d3(3), d2(2, std::vector<Node*>{&d3}), d1(1, std::vector<Node*>{&d2});
    result = levelOrderTraversal(&d1);
    assert(result == std::vector<std::vector<int>>({{1}, {2}, {3}}));

    // Test tree with mixed branching and no children at some nodes
    Node e4(4), e5(5);
    Node e3(3, std::vector<Node*>{&e5});
    Node e2(2, std::vector<Node*>{&e4});
    Node e1(1, std::vector<Node*>{&e2, &e3});
    result = levelOrderTraversal(&e1);
    assert(result == std::vector<std::vector<int>>({{1}, {2,3}, {4,5}}));

    return 0;
}

// We perform a breadth-first search (BFS) using a queue. Start by checking if `root` is null; if so, return an empty vector. Initialize a queue and push the root. While the queue is not empty, record the current queue size (`n`) to know how many nodes belong to the current level. Pop those `n` nodes one by one, append their values to a temporary level vector, and push all their children into the queue. After processing all `n` nodes, append the level vector to the result. This ensures each level is captured correctly regardless of how many children a node has. Edge cases: empty tree returns empty vector; a single node returns `[[value]]`. Time complexity: O(T) where T is the total number of nodes, because each node is enqueued and dequeued exactly once. Space complexity: O(W) for the queue, where W is the maximum width of the tree (worst-case O(T) for a star-like tree, but bounded by 5000 per constraints). The algorithm naturally handles arbitrary branching factors.
