// Write a C++ function `bool isSubtree(const TreeNode* root, const TreeNode* subRoot)` that determines whether the binary tree rooted at `subRoot` is a subtree of the binary tree rooted at `root`. A subtree of a binary tree is a tree consisting of a node in the original tree and all of its descendants. The original tree itself is considered a subtree of itself. You are given two pointers to the root nodes of the trees, where each node has integer values (which may be negative) and may be `nullptr`. The function must return `true` if `subRoot` appears as a subtree (with identical structure and node values) anywhere within `root`, and `false` otherwise. Both trees are non-empty (i.e., both `root` and `subRoot` are non-null). Note that a single node with a value equal to a multi-node subtree’s root is not sufficient; the entire structure must match. Implement the solution using a serialization approach: convert each tree into a string representation using preorder traversal with a sentinel for null pointers, then check if the serialized `subRoot` is a substring of the serialized `root`. Ensure your serialization uses a delimiter that cannot be confused with numeric values (e.g., append a special marker after every value and use a distinct marker for null).
The main algorithm is based on serializing both trees into a unique string representation using preorder traversal. The serialization must capture both the structure and node values, including null children, to avoid false positives (e.g., two different trees with the same value sequence but different shapes). We use a sentinel character (like `#`) appended after every node value to delimit numbers, and a special marker (like `"NULL"` with a delimiter) for null children. For each node, we append its value followed by `#`, then recursively serialize left and right subtrees. For a null node, we append `"NULL#"`. This produces a string where the subtree appears as a contiguous substring if and only if the subtree structure and values match exactly. Then the problem reduces to a substring search: if `serialize(subRoot)` is found in `serialize(root)`, return `true`. Edge cases: (1) Both trees are identical—then `subRoot` is a subtree of itself, and the substring will be found (in fact the entire string). (2) If `subRoot` is just a leaf node that appears at different depths—the serialization will correctly match as long as the parent context is also identical (since null markers force exact structure). (3) Negative numbers are handled by using `std::to_string`, which includes a minus sign; the delimiter prevents ambiguity. Time complexity: serializing each tree takes O(n) and O(m) where n and m are the number of nodes in `root` and `subRoot` respectively. The substring search using `std::string::find` is O((n+m)*L) in the worst case due to possible string comparisons (typically O(n+m) for a linear search). Overall complexity is O(n + m) for serialization plus the find operation, which is acceptable. Space complexity: O(n + m) for the two serialized strings (plus recursion stack depth O(height)).
#include <string>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Serialize a binary tree using preorder traversal with sentinels.
void serializeTree(const TreeNode* node, std::string& out) {
    if (!node) {
        out += "NULL#";
        return;
    }
    out += std::to_string(node->val) + "#";
    serializeTree(node->left, out);
    serializeTree(node->right, out);
}

// Returns true if subRoot is a subtree of root.
bool isSubtree(const TreeNode* root, const TreeNode* subRoot) {
    std::string rootStr = "#";
    serializeTree(root, rootStr);
    std::string subStr = "#";
    serializeTree(subRoot, subStr);
    return rootStr.find(subStr) != std::string::npos;
}
#include <cassert>

int main() {
    // Example 1: Subtree exists.
    // Root:      3
    //          /   \
    //         4     5
    //        / \
    //       1   2
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(4);
    root->right = new TreeNode(5);
    root->left->left = new TreeNode(1);
    root->left->right = new TreeNode(2);
    // SubRoot: 4
    //        / \
    //       1   2
    TreeNode* subRoot = new TreeNode(4);
    subRoot->left = new TreeNode(1);
    subRoot->right = new TreeNode(2);
    assert(isSubtree(root, subRoot) == true);

    // Example 2: Same structure and values, but subtree is not exact (different null children).
    // SubRoot: 4
    //        /
    //       1
    TreeNode* subRoot2 = new TreeNode(4);
    subRoot2->left = new TreeNode(1);
    assert(isSubtree(root, subRoot2) == false);

    // Example 3: Root is the same as subRoot (tree is subtree of itself).
    assert(isSubtree(root, root) == true);

    // Example 4: Single node tree.
    TreeNode* single = new TreeNode(5);
    assert(isSubtree(root, single) == true);

    // Example 5: SubRoot not present anywhere.
    TreeNode* notPresent = new TreeNode(10);
    assert(isSubtree(root, notPresent) == false);

    // Example 6: SubRoot is a leaf of root, but appears.
    TreeNode* leaf1 = new TreeNode(1);
    assert(isSubtree(root, leaf1) == true);

    // Example 7: Deep subtree exists.
    TreeNode* deepRoot = new TreeNode(1);
    deepRoot->left = new TreeNode(2);
    deepRoot->right = new TreeNode(3);
    deepRoot->left->left = new TreeNode(4);
    deepRoot->left->right = new TreeNode(5);
    deepRoot->left->left->left = new TreeNode(6);
    TreeNode* deepSub = new TreeNode(2);
    deepSub->left = new TreeNode(4);
    deepSub->right = new TreeNode(5);
    deepSub->left->left = new TreeNode(6);
    assert(isSubtree(deepRoot, deepSub) == true);

    // Example 8: Same values but different shape.
    TreeNode* shapeRoot = new TreeNode(1);
    shapeRoot->left = new TreeNode(2);
    shapeRoot->right = new TreeNode(3);
    TreeNode* shapeSub = new TreeNode(1);
    shapeSub->left = new TreeNode(3);
    shapeSub->right = new TreeNode(2);
    assert(isSubtree(shapeRoot, shapeSub) == false);

    // Example 9: Single node tree equal to single node root.
    TreeNode* singleRoot = new TreeNode(7);
    TreeNode* singleSub = new TreeNode(7);
    assert(isSubtree(singleRoot, singleSub) == true);

    // Example 10: SubRoot is null? Not per spec, but our function would treat as subtree if root null? Not tested.
    // Cleanup not necessary for assert checks.
    return 0;
}
