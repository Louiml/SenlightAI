/*
Given a binary tree stored in an array of nodes (with 1-based indexing), where each node contains an integer value and the indices of its left and right children (0 means no child), write a C++ function that computes the height and balance factor (right subtree height minus left subtree height) for every node, and returns a `std::vector<int>` containing the balance factors for nodes 1 through n in order. The input tree may be unbalanced, and any node may have zero, one, or two children. The function must compute heights and balance factors recursively without modifying the original vector, and must handle the typical representation where node 1 is the root. The output vector should have size exactly n, with index 0 corresponding to node 1, index 1 to node 2, etc.
*/

#include <vector>
#include <algorithm> // for std::max

// Compute heights and balance factors for all nodes.
// nodes is 1-based indexed (index 0 unused). Returns balance factors for nodes 1..n.
std::vector<int> computeBalanceFactors(const std::vector<Node>& nodes) {
    int n = static_cast<int>(nodes.size()) - 1; // because index 0 is dummy
    std::vector<int> balance(n);
    if (n == 0) return balance;

    // Helper function: compute height of subtree rooted at idx, and fill balance vector.
    // Uses a lambda that captures nodes and balance by reference.
    std::function<int(int)> dfs = [&](int idx) -> int {
        if (idx == 0) return 0; // empty subtree
        const Node& node = nodes[idx];
        int left_h = dfs(node.left);
        int right_h = dfs(node.right);
        int h = 1 + std::max(left_h, right_h);
        balance[idx-1] = right_h - left_h;
        return h;
    };

    dfs(1); // root is node 1
    return balance;
}

#include <cassert>
#include <vector>

// Assuming the Node struct from the snippet is available.
struct Node {
    int value;
    int left;
    int right;
    int h;  // not used in computation
    int b;  // not used in computation
};

// Include the solution function here (or link it).

int main() {
    // Test 1: Empty tree (n=0)
    std::vector<Node> nodes1 = {{0,0,0,0,0}}; // only dummy
    std::vector<int> res1 = computeBalanceFactors(nodes1);
    assert(res1.empty());

    // Test 2: Single node
    std::vector<Node> nodes2 = {{0,0,0,0,0}, {1,0,0,1,0}};
    std::vector<int> res2 = computeBalanceFactors(nodes2);
    assert(res2.size() == 1);
    assert(res2[0] == 0);

    // Test 3: Balanced tree: root 1, left 2, right 3
    std::vector<Node> nodes3 = {
        {0,0,0,0,0},
        {1,2,3,1,0},
        {2,0,0,1,0},
        {3,0,0,1,0}
    };
    std::vector<int> res3 = computeBalanceFactors(nodes3);
    assert((res3 == std::vector<int>{0,0,0}));

    // Test 4: Left-heavy tree: root 1, left 2, left 3
    std::vector<Node> nodes4 = {
        {0,0,0,0,0},
        {1,2,0,1,0},
        {2,3,0,1,0},
        {3,0,0,1,0}
    };
    std::vector<int> res4 = computeBalanceFactors(nodes4);
    assert((res4 == std::vector<int>{-2,-1,0}));

    // Test 5: Right-heavy tree: root 1, right 2, right 3
    std::vector<Node> nodes5 = {
        {0,0,0,0,0},
        {1,0,2,1,0},
        {2,0,3,1,0},
        {3,0,0,1,0}
    };
    std::vector<int> res5 = computeBalanceFactors(nodes5);
    assert((res5 == std::vector<int>{2,1,0}));

    // Test 6: Tree with missing left child but right child present
    std::vector<Node> nodes6 = {
        {0,0,0,0,0},
        {1,0,2,1,0},
        {2,0,0,1,0}
    };
    std::vector<int> res6 = computeBalanceFactors(nodes6);
    assert((res6 == std::vector<int>{1,0}));

    // Test 7: Complex tree from the snippet example with 6 nodes
    // Nodes: 1(40,2,3), 2(20,4,5), 3(60,0,6), 4(10,0,0), 5(30,0,0), 6(50,0,0)
    std::vector<Node> nodes7 = {
        {0,0,0,0,0},
        {40,2,3,1,0},
        {20,4,5,1,0},
        {60,0,6,1,0},
        {10,0,0,1,0},
        {30,0,0,1,0},
        {50,0,0,1,0}
    };
    std::vector<int> res7 = computeBalanceFactors(nodes7);
    // Node1: right_h=1 (node6), left_h=2 (node2) => b=-1
    // Node2: right_h=0, left_h=0 => b=0
    // Node3: right_h=1 (node6), left_h=0 => b=1
    // Node4,5,6: b=0
    assert((res7 == std::vector<int>{-1,0,1,0,0,0}));
}

// The solution uses a recursive post-order traversal. For a given node index, if the node has no children, its height is 1 and balance factor is 0. Otherwise, we recursively compute heights of left and right subtrees (if a child index is 0, treat that subtree height as 0). Then the node's height = 1 + max(left_height, right_height) and balance = right_height - left_height. Since we process children before the parent, we can store heights in a local vector or use a helper that returns height while filling a results vector. The recursive function should be const-correct: it takes the input vector by const reference and returns a new vector. Edge cases: n=0 (should return empty vector), a tree with only one node (balance factor 0), and nodes with one missing child (missing side height = 0). Time complexity is O(n) because each node is visited once. Space complexity is O(n) for the recursion stack in the worst case (skewed tree) plus O(n) for the result vector.
