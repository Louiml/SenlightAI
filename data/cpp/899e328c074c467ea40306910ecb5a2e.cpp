Implement a C++ function that processes a binary tree represented by a simplified `Node` structure with integer `id`, a string `label`, and `left`/`right` child pointers. The function should return a `std::vector<std::string>` of all root-to-leaf paths where each path is a string formed by concatenating labels of nodes along the path, separated by hyphens (`-`). Only paths that end at a leaf (a node with no children) and have at least two nodes should be included. Traverse the tree in depth-first order, visiting left children before right children. The input tree is non-empty, and labels are non-empty strings free of hyphens. If no such path exists, return an empty vector.

// The task requires a depth-first traversal of a binary tree to collect all root-to-leaf paths of length at least two. The main algorithm recursively explores each node, maintaining a current path string. At each non-leaf node, we append the node's label to the current path (with a hyphen separator if the path is non-empty) and recurse into children. When we encounter a leaf node, we check if the accumulated path has at least two nodes (i.e., the path string contains at least one hyphen). If so, we add the path to the result. Because we visit left children before right children, the output order follows a natural left-to-right depth-first order, which matches the expected traversal. Edge cases include: a tree with only a root (no valid paths because a path must have at least two nodes), a tree where all leaves are the root itself (again no valid paths), and paths that pass through nodes with single children (still valid as long as the end is a leaf). Time complexity is O(N) where N is the number of nodes, because each node is visited once and path string concatenation costs O(path length) but the total cost across all nodes is O(N^2) in the worst case for a skewed tree, though typical balanced trees yield O(N log N). Space complexity is O(H) for the recursion stack plus the storage for the result, where H is the tree height, and O(total path length) for the output.

#include <vector>
#include <string>

struct Node {
    int id;
    std::string label;
    Node* left;
    Node* right;
    Node(int i, const std::string& l) : id(i), label(l), left(nullptr), right(nullptr) {}
};

// Helper function for DFS traversal
void collectPaths(const Node* node, const std::string& currentPath, std::vector<std::string>& result) {
    if (!node) return;

    // Build the path string up to this node
    std::string newPath;
    if (currentPath.empty()) {
        newPath = node->label;
    } else {
        newPath = currentPath + "-" + node->label;
    }

    // If leaf, check if path has at least two nodes (contains a hyphen)
    if (!node->left && !node->right) {
        if (newPath.find('-') != std::string::npos) {
            result.push_back(newPath);
        }
        return;
    }

    // Recurse into children (left first)
    collectPaths(node->left, newPath, result);
    collectPaths(node->right, newPath, result);
}

// Main function: returns all root-to-leaf paths of length >= 2
std::vector<std::string> getAllRootToLeafPaths(const Node* root) {
    std::vector<std::string> result;
    if (!root) return result;
    collectPaths(root, "", result);
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// Node and function declarations (include the solution code above)

int main() {
    // Test 1: Simple tree with one valid path
    Node n1(1, "A");
    Node n2(2, "B");
    Node n3(3, "C");
    n1.left = &n2;
    n2.left = &n3;
    auto paths1 = getAllRootToLeafPaths(&n1);
    std::vector<std::string> expected1 = {"A-B-C"};
    assert(paths1 == expected1);

    // Test 2: Tree with left and right leaves
    Node t1(1, "X");
    Node t2(2, "Y");
    Node t3(3, "Z");
    t1.left = &t2;
    t1.right = &t3;
    auto paths2 = getAllRootToLeafPaths(&t1);
    std::vector<std::string> expected2 = {"X-Y", "X-Z"};
    assert(paths2 == expected2);

    // Test 3: Root only (no valid paths)
    Node r1(1, "Solo");
    auto paths3 = getAllRootToLeafPaths(&r1);
    std::vector<std::string> expected3 = {};
    assert(paths3 == expected3);

    // Test 4: A single-child chain of length 2 (valid path)
    Node c1(1, "P");
    Node c2(2, "Q");
    c1.left = &c2;
    auto paths4 = getAllRootToLeafPaths(&c1);
    std::vector<std::string> expected4 = {"P-Q"};
    assert(paths4 == expected4);

    // Test 5: Mixed tree with a short path (length 1) and longer paths
    Node m1(1, "Root");
    Node m2(2, "Left");     // leaf, invalid (only one node)
    Node m3(3, "Right");
    Node m4(4, "RLeft");
    Node m5(5, "RRight");
    m1.left = &m2;
    m1.right = &m3;
    m3.left = &m4;
    m3.right = &m5;
    auto paths5 = getAllRootToLeafPaths(&m1);
    std::vector<std::string> expected5 = {"Root-Right-RLeft", "Root-Right-RRight"};
    assert(paths5 == expected5);

    // Test 6: Null root (empty tree)
    Node* nullRoot = nullptr;
    auto paths6 = getAllRootToLeafPaths(nullRoot);
    std::vector<std::string> expected6 = {};
    assert(paths6 == expected6);

    // Test 7: Multiple leaves at same depth
    Node d1(1, "Top");
    Node d2(2, "L1");
    Node d3(3, "R1");
    Node d4(4, "L2");
    Node d5(5, "R2");
    d1.left = &d2;
    d1.right = &d3;
    d2.left = &d4;
    d2.right = &d5;
    auto paths7 = getAllRootToLeafPaths(&d1);
    std::vector<std::string> expected7 = {"Top-L1-L2", "Top-L1-R2", "Top-R1"};
    assert(paths7 == expected7);

    return 0;
}
