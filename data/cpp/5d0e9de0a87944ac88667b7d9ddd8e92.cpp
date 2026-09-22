/*
Given a binary search tree (BST) with integer values, write a C++ function that returns a vector containing all the modes of the BST. A mode is defined as the value(s) that occur with the highest frequency in the BST. The function must be named `findModes` and take a single parameter: a pointer to the root `TreeNode` of the BST (where `TreeNode` is a struct with integer `val`, and `left` and `right` pointers to its children, or `nullptr`). The returned vector should be sorted in ascending order of the values. The BST may contain duplicate values (it is not necessarily a strict BST in the sense that duplicates may appear on either side), and the tree may be empty (root is `nullptr`), in which case the function should return an empty vector. You must define the `TreeNode` struct exactly as provided in the problem snippet. The solution should not use any global or static variables, and must be self-contained (include all necessary headers).
*/
#include <vector>
#include <map>
#include <algorithm>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Helper function to traverse the tree and update frequency counts and max frequency.
void traverseAndCount(const TreeNode* node, std::map<int, int>& freq, int& maxFreq) {
    if (node == nullptr) {
        return;
    }
    // In-order traversal (left, root, right) — any order works for counting.
    traverseAndCount(node->left, freq, maxFreq);
    int count = ++freq[node->val];
    if (count > maxFreq) {
        maxFreq = count;
    }
    traverseAndCount(node->right, freq, maxFreq);
}

// Returns all modes of the BST (values with maximum frequency), sorted ascending.
std::vector<int> findModes(const TreeNode* root) {
    std::vector<int> result;
    if (root == nullptr) {
        return result;
    }
    std::map<int, int> freq;
    int maxFreq = 0;
    traverseAndCount(root, freq, maxFreq);
    
    // Collect all values with frequency equal to maxFreq (map iterates in sorted key order).
    for (const auto& pair : freq) {
        if (pair.second == maxFreq) {
            result.push_back(pair.first);
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// TreeNode definition is provided in the solution section, but we include it again here for completeness in the test file.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// The findModes function from the solution (assume included above).
std::vector<int> findModes(const TreeNode* root);

int main() {
    // Test 1: Empty tree
    assert(findModes(nullptr).empty());

    // Test 2: Single node
    TreeNode* t2 = new TreeNode(5);
    assert(findModes(t2) == std::vector<int>({5}));
    delete t2;

    // Test 3: All unique values (each appears once) -> all are modes
    TreeNode* t3 = new TreeNode(2);
    t3->left = new TreeNode(1);
    t3->right = new TreeNode(3);
    assert(findModes(t3) == std::vector<int>({1, 2, 3}));
    delete t3->left; delete t3->right; delete t3;

    // Test 4: Duplicate values in a tree
    TreeNode* t4 = new TreeNode(2);
    t4->left = new TreeNode(1);
    t4->right = new TreeNode(2);
    t4->left->left = new TreeNode(1);
    // Frequencies: 1 appears twice, 2 appears twice -> both are modes
    assert(findModes(t4) == std::vector<int>({1, 2}));
    delete t4->left->left; delete t4->left; delete t4->right; delete t4;

    // Test 5: Single dominant mode
    TreeNode* t5 = new TreeNode(3);
    t5->left = new TreeNode(3);
    t5->right = new TreeNode(3);
    t5->left->left = new TreeNode(2);
    // Frequency of 3 is 3, others 1 -> only 3 is mode
    assert(findModes(t5) == std::vector<int>({3}));
    delete t5->left->left; delete t5->left; delete t5->right; delete t5;

    // Test 6: Duplicate left and right skewed (but still BST-like with duplicates)
    TreeNode* t6 = new TreeNode(5);
    t6->left = new TreeNode(5);
    t6->left->left = new TreeNode(5);
    // Only 5 appears three times; return {5}
    assert(findModes(t6) == std::vector<int>({5}));
    delete t6->left->left; delete t6->left; delete t6;

    // Test 7: All same value repeated
    TreeNode* t7 = new TreeNode(7);
    t7->right = new TreeNode(7);
    t7->right->right = new TreeNode(7);
    assert(findModes(t7) == std::vector<int>({7}));
    delete t7->right->right; delete t7->right; delete t7;

    return 0;
}
// The approach is to traverse the entire tree and count the frequency of each node value. Since we need the modes (values with maximum frequency) and the result must be sorted, we can use an associative container like `std::map` to store value-to-frequency pairs, because iterating over a `map` yields keys in ascending order. Perform an in-order traversal (or any traversal, but in-order naturally visits nodes sorted if the tree is a true BST, though here duplicates can appear anywhere, so order does not affect counts) to accumulate frequencies into the map. While incrementing each count, track the current maximum frequency. After the traversal, iterate over the map and collect all keys whose frequency equals the maximum. Edge cases: empty tree (return empty vector), all values unique (then all are modes? No—only values with frequency equal to maximum; if all unique, maximum is 1, so all values are modes), and presence of duplicates. Time complexity: O(N log N) due to map insertion (each insertion costs O(log K) where K is number of distinct values, but upper-bound O(N log N)). In-order traversal visits each node once: O(N). Overall O(N log N). Space complexity: O(N) for the map and the result vector, excluding the recursion stack which is O(H) where H is tree height (could be O(N) in worst-case skewed tree).
