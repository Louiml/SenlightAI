/*
Write a C++ function `bool isBalancedBST(const std::vector<std::pair<std::string,std::string>>& entries)` that takes a vector of word/definition pairs, builds a binary search tree (BST) where the word is the key and the definition is the associated value, and returns `true` if the resulting BST is height-balanced (i.e., for every node, the absolute difference between the heights of its left and right subtrees is at most 1), and `false` otherwise. The input vector may contain duplicate words; if duplicates occur, keep only the first occurrence and ignore subsequent duplicates. The function must handle empty input (return `true`), a single entry (return `true`), and entries with arbitrary strings (including words that are lexicographically equal to each other). You must implement your own node structure and tree-building logic; do not use any standard library containers for the tree itself (though `std::vector`, `std::string`, `std::pair` are allowed for input). The function should be `const`-correct, take the input by const reference, and avoid leaking memory by properly deleting the tree before returning.
*/
#include <vector>
#include <string>
#include <utility>
#include <cstdlib>   // for abs

// Node structure for BST
struct BSTNode {
    std::string word;
    std::string definition;
    BSTNode* left;
    BSTNode* right;
    BSTNode(const std::string& w, const std::string& d)
        : word(w), definition(d), left(nullptr), right(nullptr) {}
};

// Helper to insert a new pair, returns true if inserted, false if duplicate
bool insertNode(BSTNode*& node, const std::string& word, const std::string& definition) {
    if (node == nullptr) {
        node = new BSTNode(word, definition);
        return true;
    }
    if (word < node->word) {
        return insertNode(node->left, word, definition);
    } else if (word > node->word) {
        return insertNode(node->right, word, definition);
    } else {
        return false;  // duplicate, ignore
    }
}

// Helper to compute height and check balance simultaneously
// Returns height if balanced, -2 if unbalanced
int checkBalanceAndGetHeight(const BSTNode* node) {
    if (node == nullptr) {
        return -1;  // height of empty tree
    }
    int leftHeight = checkBalanceAndGetHeight(node->left);
    if (leftHeight == -2) return -2;
    int rightHeight = checkBalanceAndGetHeight(node->right);
    if (rightHeight == -2) return -2;
    if (std::abs(leftHeight - rightHeight) > 1) {
        return -2;  // unbalanced
    }
    return std::max(leftHeight, rightHeight) + 1;
}

// Helper to delete the entire tree
void deleteTree(BSTNode* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

// Main function: build BST and check if height-balanced
bool isBalancedBST(const std::vector<std::pair<std::string,std::string>>& entries) {
    BSTNode* root = nullptr;
    for (const auto& entry : entries) {
        insertNode(root, entry.first, entry.second);
    }
    int result = checkBalanceAndGetHeight(root);
    deleteTree(root);
    return result != -2;  // -2 indicates unbalanced
}
#include <cassert>
#include <vector>
#include <string>
#include <utility>

// The solution function declaration is assumed to be included above.
// The following is the test main.

int main() {
    // Empty input
    std::vector<std::pair<std::string,std::string>> empty;
    assert(isBalancedBST(empty) == true);

    // Single entry
    std::vector<std::pair<std::string,std::string>> single = {{"apple", "fruit"}};
    assert(isBalancedBST(single) == true);

    // Balanced tree: insertion order "b", "a", "c" -> root b, left a, right c
    std::vector<std::pair<std::string,std::string>> balanced = {{"b", "bee"}, {"a", "aye"}, {"c", "see"}};
    assert(isBalancedBST(balanced) == true);

    // Skewed tree (sorted order) -> height 2, left subtree height -1, diff 3 > 1 -> false
    std::vector<std::pair<std::string,std::string>> skewed = {{"a", "one"}, {"b", "two"}, {"c", "three"}};
    assert(isBalancedBST(skewed) == false);

    // Duplicates ignored, leaves balanced tree
    std::vector<std::pair<std::string,std::string>> dup = {{"x", "first"}, {"x", "second"}, {"y", "why"}, {"z", "zed"}};
    // Insertion: x, y, z -> right-skewed (height 2) -> not balanced
    assert(isBalancedBST(dup) == false);

    // Tree that becomes balanced after ignoring duplicates
    std::vector<std::pair<std::string,std::string>> dupBalanced = {{"m", "middle"}, {"m", "dup"}, {"f", "first"}, {"r", "right"}, {"c", "child"}};
    // Built tree: root m, left f (with left c), right r -> heights: left subtree height 1 (f->c), right height 0 -> diff 1 -> true
    assert(isBalancedBST(dupBalanced) == true);

    // Larger balanced case
    std::vector<std::pair<std::string,std::string>> balancedLarge = {
        {"d", "d"}, {"b", "b"}, {"f", "f"}, {"a", "a"}, {"c", "c"}, {"e", "e"}, {"g", "g"}
    };
    assert(isBalancedBST(balancedLarge) == true);

    // Unbalanced deep tree
    std::vector<std::pair<std::string,std::string>> deep = {
        {"a", "a"}, {"b", "b"}, {"c", "c"}, {"d", "d"}, {"e", "e"}
    };
    assert(isBalancedBST(deep) == false);

    // All duplicates -> tree empty -> balanced
    std::vector<std::pair<std::string,std::string>> allDup = {{"z", "1"}, {"z", "2"}, {"z", "3"}};
    assert(isBalancedBST(allDup) == true);

    return 0;
}
// The solution builds a BST by inserting each (word, definition) pair in the order they appear in the input vector. For each word, we traverse from the root: if the word is less than the current node's word, go left; if greater, go right; if equal, skip the duplicate (do not insert). After inserting all entries, we compute the height of each subtree recursively: a null node has height -1 (or 0 if counting edges, but here we'll use height = number of edges on longest path, so empty tree height = -1). For each node, we compute the heights of left and right children, check if the absolute difference is ≤ 1, and also verify that both subtrees are balanced recursively. The function returns `true` if both the balance condition holds for every node and the tree is a valid BST (which is guaranteed by the insertion logic). Time complexity is O(n * h) for building, where h is the average height (O(n) worst-case for a skewed tree), and O(n) for the balance check. Space complexity is O(h) for recursion stack (O(n) worst-case) plus O(n) for the tree nodes. Edge cases: empty input (return true), all duplicates (tree remains empty → true), already balanced (true), pathological insertion order like sorted input (skewed tree → false if height difference >1), and entries with empty strings (treated as normal strings). The implementation must deallocate all nodes to prevent memory leaks.
