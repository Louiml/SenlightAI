// Write a standalone C++ function `int maxTreeDepth(TreeNode<int>* root)` that computes the maximum depth (height) of a k-ary tree, where each node stores an integer and has a `vector` of child pointers. The depth is defined as the number of nodes along the longest path from the root down to the farthest leaf. The root being `nullptr` has depth 0, a single node has depth 1, and so on. The function must handle arbitrarily branching trees, not just binary trees. You are provided a `TreeNode` template class with `data` and `children` members. The function should be `const`-correct where appropriate and must not modify the tree.

#include <cassert>

int main() {
    // Test 1: Empty tree
    assert(maxTreeDepth(nullptr) == 0);

    // Test 2: Single node, no children
    TreeNode<int>* single = new TreeNode<int>(5);
    assert(maxTreeDepth(single) == 1);
    delete single;

    // Test 3: Chain of 3 nodes (linear tree)
    TreeNode<int>* chain3 = new TreeNode<int>(1);
    chain3->children.push_back(new TreeNode<int>(2));
    chain3->children[0]->children.push_back(new TreeNode<int>(3));
    assert(maxTreeDepth(chain3) == 3);

    // Test 4: Root with two children, each with one leaf -> depth 3
    TreeNode<int>* root4 = new TreeNode<int>(1);
    auto* childA = new TreeNode<int>(2);
    auto* childB = new TreeNode<int>(3);
    childA->children.push_back(new TreeNode<int>(4));
    childB->children.push_back(new TreeNode<int>(5));
    root4->children.push_back(childA);
    root4->children.push_back(childB);
    assert(maxTreeDepth(root4) == 3);

    // Test 5: Wider tree: root with 3 leaf children -> depth 2
    TreeNode<int>* root5 = new TreeNode<int>(1);
    root5->children.push_back(new TreeNode<int>(2));
    root5->children.push_back(new TreeNode<int>(3));
    root5->children.push_back(new TreeNode<int>(4));
    assert(maxTreeDepth(root5) == 2);

    // Test 6: Asymmetric tree: one child is chain of 4, other is leaf -> depth 5
    TreeNode<int>* root6 = new TreeNode<int>(1);
    auto* deep = new TreeNode<int>(2);
    deep->children.push_back(new TreeNode<int>(3));
    deep->children[0]->children.push_back(new TreeNode<int>(4));
    deep->children[0]->children[0]->children.push_back(new TreeNode<int>(5));
    root6->children.push_back(deep);
    root6->children.push_back(new TreeNode<int>(6));
    assert(maxTreeDepth(root6) == 5);

    // Cleanup for tests 3–6 (simple delete manually, not shown for brevity)
    // In a real test, you would delete all nodes to avoid memory leaks.
    // For brevity, we skip full cleanup here but include a simple delete for root nodes.
    delete chain3;
    delete root4;
    delete root5;
    delete root6;

    // Note: Full memory cleanup would require recursive deletion; omitted for exercise.
    return 0;
}

#include <vector>
#include <algorithm>

template <typename T>
class TreeNode {
public:
    T data;
    std::vector<TreeNode*> children;
    TreeNode(T val) : data(val) {}
};

// Compute the maximum depth (height) of a k-ary tree.
// Depth is the number of nodes on the longest root-to-leaf path.
// An empty tree (null root) has depth 0.
int maxTreeDepth(const TreeNode<int>* root) {
    if (!root) return 0;
    
    int maxChildDepth = 0;
    for (const auto* child : root->children) {
        maxChildDepth = std::max(maxChildDepth, maxTreeDepth(child));
    }
    return 1 + maxChildDepth;
}

// The problem is a standard tree-depth computation generalization of binary-tree height to k-ary trees. The main algorithm is recursive: for a given node, if it is `nullptr`, return 0. Otherwise, compute the maximum depth among all its children by recursing on each child, take the maximum of those depths, and add 1 to account for the current node. The base case is when a node has no children—then the maximum of an empty set is 0, so adding 1 gives depth 1, which is correct. Edge cases include an empty tree (`nullptr` root → depth 0), a single node, nodes with many children, and deep chains. The recursion visits each node exactly once, so time complexity is O(N) where N is the number of nodes. Space complexity is O(H) where H is the height of the tree due to recursion stack; in the worst case of a skewed tree, H = N, giving O(N) space. The solution requires checking all children, so a loop over the `children` vector is needed, tracking the maximum child depth.
