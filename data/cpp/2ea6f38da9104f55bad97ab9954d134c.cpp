Write a C++ function `findMode` that accepts the root of a binary search tree (BST) where node values are integers, and returns a `std::vector<int>` containing all the most frequently occurring values (modes) in the tree. The BST may contain duplicate values, and nodes can appear in any valid BST arrangement. If multiple values share the maximum frequency, return all of them in any order. The function should not modify the tree, and the input tree is guaranteed to be non-empty.
#include <cassert>
#include <vector>

// TreeNode definition must match the one used in the solution.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Declaration (or include the solution header here).
std::vector<int> findMode(TreeNode* root);

int main() {
    // Test 1: Single node tree
    TreeNode* n1 = new TreeNode(5);
    std::vector<int> result1 = findMode(n1);
    assert(result1.size() == 1 && result1[0] == 5);

    // Test 2: All same values
    TreeNode* n2 = new TreeNode(2, new TreeNode(2), new TreeNode(2));
    std::vector<int> result2 = findMode(n2);
    assert(result2.size() == 1 && result2[0] == 2);

    // Test 3: Several distinct values with one mode
    TreeNode* n3 = new TreeNode(1, nullptr, new TreeNode(2, new TreeNode(2), nullptr));
    std::vector<int> result3 = findMode(n3);
    assert(result3.size() == 1 && result3[0] == 2);

    // Test 4: Multiple modes (1 and 3 both appear twice)
    TreeNode* n4 = new TreeNode(1, new TreeNode(3), new TreeNode(3, new TreeNode(1), nullptr));
    std::vector<int> result4 = findMode(n4);
    assert(result4.size() == 2);
    // Order not guaranteed, check membership
    bool has1 = false, has3 = false;
    for (int v : result4) {
        if (v == 1) has1 = true;
        if (v == 3) has3 = true;
    }
    assert(has1 && has3);

    // Test 5: Larger tree with unique frequency
    TreeNode* n5 = new TreeNode(4, new TreeNode(2, new TreeNode(2), new TreeNode(4)), new TreeNode(4));
    std::vector<int> result5 = findMode(n5);
    assert(result5.size() == 1 && result5[0] == 4);

    // Cleanup (optional but good practice in real tests)
    delete n1;
    delete n2;
    delete n3;
    delete n4;
    delete n5;

    return 0;
}
#include <unordered_map>
#include <vector>

// Forward declaration to avoid including full TreeNode definition in header.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Helper: recursively count each value's frequency.
void countFrequencies(TreeNode* node, std::unordered_map<int, int>& freq) {
    if (node == nullptr) {
        return;
    }
    freq[node->val]++;
    countFrequencies(node->left, freq);
    countFrequencies(node->right, freq);
}

// Return all values that appear most frequently in the binary tree.
std::vector<int> findMode(TreeNode* root) {
    std::unordered_map<int, int> freq;
    countFrequencies(root, freq);

    int maxFreq = 0;
    std::vector<int> modes;
    for (const auto& pair : freq) {
        if (pair.second > maxFreq) {
            maxFreq = pair.second;
            modes.clear();
            modes.push_back(pair.first);
        } else if (pair.second == maxFreq) {
            modes.push_back(pair.first);
        }
    }
    return modes;
}
// The simplest and most robust approach is to traverse the entire tree (e.g., using a depth-first search) and count the frequency of each value using an `unordered_map<int, int>`. After traversal, iterate over the map to find the maximum frequency. During this iteration, maintain a vector of results: if a value has a frequency greater than the current maximum, clear the vector and add that value; if it equals the maximum, add it to the vector. Edge cases include: a tree with only one node (the mode is that single value), all nodes having the same value (that value is the only mode), and multiple distinct values having the same maximum frequency (return all of them). This approach does not require the BST property to be used, making it correct even for non-BST binary trees. Time complexity is O(n) where n is the number of nodes, since each node is visited once and the map is iterated once (map size ≤ n). Space complexity is O(n) in the worst case for the map and recursion stack (if the tree is skewed). To avoid recursion depth issues in pathological trees, an iterative BFS/DFS could be used, but recursion is acceptable for typical test inputs.
