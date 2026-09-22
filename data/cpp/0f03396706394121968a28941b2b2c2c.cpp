/*
Write a C++ function named `smallestFromLeaf` that, given the root of a binary tree where each node contains an integer from 0 to 25 representing a lowercase letter ('a' + value), returns the lexicographically smallest string that can be formed by concatenating characters along a path from a leaf node upward to the root. The string must be read from the leaf to the root (i.e., reverse order of the downward path). If the tree has only one node, that node's character is the result. The tree may be empty (null root), in which case return an empty string. The function should handle arbitrary unbalanced trees and nodes with only one child. The solution should use a depth-first search traversal and compare candidate strings efficiently.
*/

#include <string>
#include <algorithm>

// TreeNode definition as provided in the original snippet.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Helper function for DFS traversal.
void dfsHelper(const TreeNode* node, std::string& current, std::string& best) {
    if (!node) return;
    // Append the character corresponding to this node's value.
    current.push_back(static_cast<char>('a' + node->val));
    
    // Check if this is a leaf node.
    if (!node->left && !node->right) {
        // Create leaf-to-root string by reversing the current path.
        std::string candidate = current;
        std::reverse(candidate.begin(), candidate.end());
        if (best.empty() || candidate < best) {
            best = candidate;
        }
    } else {
        // Recurse into children.
        dfsHelper(node->left, current, best);
        dfsHelper(node->right, current, best);
    }
    
    // Backtrack: remove this node's character.
    current.pop_back();
}

// Public function to find the smallest leaf-to-root string.
std::string smallestFromLeaf(const TreeNode* root) {
    if (!root) return "";
    std::string best;
    std::string current;
    dfsHelper(root, current, best);
    return best;
}

#include <cassert>
#include <string>

// Include the TreeNode and smallestFromLeaf function here (or from a header).

int main() {
    // Test 1: Single node (value 0 -> 'a')
    TreeNode t1(0);
    assert(smallestFromLeaf(&t1) == "a");

    // Test 2: Root with two leaves: left 'a'(0), right 'z'(25). Paths: "a" and "z" (leaf-to-root single char). Smallest is "a".
    TreeNode left(0);
    TreeNode right(25);
    TreeNode root(1, &left, &right);
    assert(smallestFromLeaf(&root) == "a");

    // Test 3: Skewed tree: root 'b'(1) -> left 'a'(0) -> left 'c'(2). Path from leaf 'c' to root is "cab". Only one leaf.
    TreeNode leaf(2);
    TreeNode mid(0, &leaf, nullptr);
    TreeNode root3(1, &mid, nullptr);
    assert(smallestFromLeaf(&root3) == "cab");

    // Test 4: Compare two paths: leaf1 gives "ab" (a->b), leaf2 gives "ba" (b->a). Choose "ab".
    // Tree: root 'a'(0) has left child 'b'(1) and right child 'a'(0). Left leaf path "ab"; right leaf path "aa". Smallest is "aa".
    TreeNode l(1);
    TreeNode r(0);
    TreeNode root4(0, &l, &r);
    assert(smallestFromLeaf(&root4) == "aa");

    // Test 5: Empty tree.
    TreeNode* empty = nullptr;
    assert(smallestFromLeaf(empty) == "");

    // Test 6: Deeper tree with same prefix: paths "abc" and "abd". "abc" < "abd".
    TreeNode leaf_c(2); // 'c'
    TreeNode leaf_d(3); // 'd'
    TreeNode node_b(1, &leaf_c, &leaf_d); // 'b'
    TreeNode root6(0, &node_b, nullptr); // 'a'
    assert(smallestFromLeaf(&root6) == "cba"); // Only leaf_c exists? Actually two leaves: leaf_c and leaf_d. Paths: "cba" and "dba". Smallest is "cba".

    // Test 7: Unbalanced with right child only.
    TreeNode right_child(1); // 'b'
    TreeNode root7(0, nullptr, &right_child); // 'a' → only right leaf gives "ba"
    assert(smallestFromLeaf(&root7) == "ba");

    // Test 8: Duplicate strings from different leaves. Both paths give "aa" from two leaves. Still returns "aa".
    TreeNode l1(0);
    TreeNode r1(0);
    TreeNode root8(0, &l1, &r1);
    assert(smallestFromLeaf(&root8) == "aa");

    return 0;
}

// The approach uses a depth-first search (DFS) from the root downward, building the current path string by appending the character corresponding to each node's value. When a leaf is reached (both children are null), we reverse the accumulated path to obtain the leaf-to-root string, then compare it lexicographically with the best answer found so far, keeping the smaller one. Reversing each candidate string costs O(depth), and the total work across all leaves is O(N × height) in the worst case; however, since height ≤ N, the worst-case time complexity is O(N²) for a skewed tree, and O(N log N) for a balanced tree. Space complexity is O(height) for recursion stack and the current path string, plus O(height) for the temporary reversed string; auxiliary space is O(height). Key edge cases: empty tree (return empty), single node (return its character), nodes with one child (must not treat them as leaves), and duplicate strings (keep the first found or either). Lexicographic comparison uses standard string comparison where shorter strings are smaller if they are prefixes; this is handled naturally by C++ `std::string` operator<.
