Write a C++ function `string treeOrdersFromEdges(int n, const vector<vector<char>>& edges)`. The input describes a binary tree whose nodes are uppercase letters `A` through `A+n-1`, where each edge entry `{parent, leftChild, rightChild}` gives the parent node (always lowercase-alphabet index 0..n-1) and its left and right children, or `.` if a child is missing. You must return a single string containing the preorder, inorder, and postorder traversals of the tree, each separated by a newline, with no trailing spaces. Assume the root is always `'A'` (index 0) and the tree is a valid binary tree (each node appears exactly once as a parent, and no node is a child of more than one parent). The total number of nodes `n` is between 1 and 26.

// The solution is a straightforward recursive depth-first traversal. Since only the root is known (always index 0), we can represent the tree using a fixed-size array of pairs (or two arrays) of size 26 (or `n`), where each entry stores the integer indices of the left and right child (or `-1` for missing). For each traversal, define a recursive function that visits the current node, then (for preorder) prints the node before children, (for inorder) prints between children, and (for postorder) prints after children. We must skip child calls if the child index is `-1`. Because the input is given as characters, we convert `'A'` to `0`, `'.'` to `-1`, etc. The three recursive calls produce a string by appending characters, then we concatenate with newlines. Time complexity is O(n) for each traversal, so O(n) total; space complexity is O(n) for recursion depth (worst-case skewed tree) and O(1) auxiliary.

#include <string>
#include <vector>

// Return preorder, inorder, postorder of a binary tree given edges.
// edges[i] = {parent, leftChild, rightChild} where child is '.' if absent.
std::string treeOrdersFromEdges(int n, const std::vector<std::vector<char>>& edges) {
    // Build left/right child arrays; -1 indicates no child.
    std::vector<int> left(n, -1), right(n, -1);
    for (const auto& e : edges) {
        int parent = e[0] - 'A';
        if (e[1] != '.') left[parent] = e[1] - 'A';
        if (e[2] != '.') right[parent] = e[2] - 'A';
    }

    std::string pre, in, post;

    // Recursive lambda for all three traversals in one pass.
    // We use a helper that fills all three strings.
    std::function<void(int)> traverse = [&](int node) {
        if (node < 0) return;
        char ch = static_cast<char>('A' + node);
        pre.push_back(ch);               // Preorder: visit before children
        traverse(left[node]);
        in.push_back(ch);                // Inorder: visit between children
        traverse(right[node]);
        post.push_back(ch);              // Postorder: visit after children
    };

    traverse(0);  // Root is always 'A'

    return pre + "\n" + in + "\n" + post;
}

#include <cassert>
#include <string>
#include <vector>

// Declaration (or include the solution header)

int main() {
    // Test 1: Simple left-skewed tree
    std::vector<std::vector<char>> e1 = {
        {'A', 'B', '.'}, {'B', 'C', '.'}, {'C', '.', '.'}
    };
    assert(treeOrdersFromEdges(3, e1) == "ABC\nCBA\nCBA");

    // Test 2: Simple right-skewed tree
    std::vector<std::vector<char>> e2 = {
        {'A', '.', 'B'}, {'B', '.', 'C'}, {'C', '.', '.'}
    };
    assert(treeOrdersFromEdges(3, e2) == "ABC\nABC\nCBA");

    // Test 3: Full binary tree of 7 nodes
    std::vector<std::vector<char>> e3 = {
        {'A', 'B', 'C'}, {'B', 'D', 'E'}, {'C', 'F', 'G'},
        {'D', '.', '.'}, {'E', '.', '.'}, {'F', '.', '.'}, {'G', '.', '.'}
    };
    assert(treeOrdersFromEdges(7, e3) == "ABDECFG\nDBEAFCG\nDEBFGCA");

    // Test 4: Single node
    std::vector<std::vector<char>> e4 = {{'A', '.', '.'}};
    assert(treeOrdersFromEdges(1, e4) == "A\nA\nA");

    // Test 5: Left child only at root, right child only at root mixed
    std::vector<std::vector<char>> e5 = {
        {'A', 'B', 'C'}, {'B', '.', '.'}, {'C', '.', '.'}
    };
    assert(treeOrdersFromEdges(3, e5) == "ABC\nBAC\nBCA");

    // Test 6: Deeper tree with missing right child
    std::vector<std::vector<char>> e6 = {
        {'A', 'B', '.'}, {'B', '.', 'C'}, {'C', 'D', '.'}, {'D', '.', '.'}
    };
    assert(treeOrdersFromEdges(4, e6) == "ABCD\nACDB\nCDBA");

    return 0;
}
