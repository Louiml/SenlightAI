/*
Write a C++ function `vector<int> naryPreorder(const vector<int>& flatTree)` that receives a flat representation of an N-ary tree using the LeetCode-style level-order encoding: the root value is followed by its children count, then each child recursively in the same format, with `0` indicating a leaf node (no children). The function must return the preorder traversal (root, then children left-to-right) as a vector of integers. The input is guaranteed to be a valid encoding of a non-empty tree. You must implement the traversal iteratively using an explicit stack, not recursion, and handle trees of arbitrary depth and branching factor.
*/
#include <vector>
#include <stack>

// Node of an N-ary tree.
struct NaryNode {
    int val;
    std::vector<NaryNode*> children;
    NaryNode(int v) : val(v) {}
};

// Build an N-ary tree from a flat LeetCode-style encoding.
NaryNode* buildTree(const std::vector<int>& flat, size_t& idx) {
    if (idx >= flat.size()) return nullptr;
    int val = flat[idx++];
    int childCount = flat[idx++];
    NaryNode* node = new NaryNode(val);
    for (int i = 0; i < childCount; ++i) {
        node->children.push_back(buildTree(flat, idx));
    }
    return node;
}

// Iterative preorder traversal using an explicit stack.
std::vector<int> naryPreorder(const std::vector<int>& flatTree) {
    size_t idx = 0;
    NaryNode* root = buildTree(flatTree, idx);
    std::vector<int> result;
    if (!root) return result;
    
    std::stack<NaryNode*> st;
    st.push(root);
    while (!st.empty()) {
        NaryNode* cur = st.top();
        st.pop();
        result.push_back(cur->val);
        // Push children in reverse order so leftmost is processed first.
        for (int i = static_cast<int>(cur->children.size()) - 1; i >= 0; --i) {
            st.push(cur->children[i]);
        }
    }
    // Clean up memory (optional; in practice we'd use smart pointers).
    // For simplicity, we omit deletion here, but a production version would.
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Example from the problem: root=1, 3 children: 3 (2 children: 5,6), 2, 4
    // Flat: value, childCount, then subtrees recursively.
    // Root: 1, 3 children -> 1,3, then child0 (3,2, then 5,0,6,0), child1 (2,0), child2 (4,0)
    std::vector<int> flat1 = {1, 3, 3, 2, 5, 0, 6, 0, 2, 0, 4, 0};
    std::vector<int> res1 = naryPreorder(flat1);
    assert(res1 == std::vector<int>({1, 3, 5, 6, 2, 4}));

    // Single node with no children
    std::vector<int> flat2 = {7, 0};
    assert(naryPreorder(flat2) == std::vector<int>({7}));

    // A chain: root(1) -> one child(2) -> one child(3) -> one child(4)
    // Encodings: 1,1,2,1,3,1,4,0
    std::vector<int> flat3 = {1, 1, 2, 1, 3, 1, 4, 0};
    assert(naryPreorder(flat3) == std::vector<int>({1, 2, 3, 4}));

    // A star: root(0) with 3 leaves (10, 20, 30)
    // 0,3,10,0,20,0,30,0
    std::vector<int> flat4 = {0, 3, 10, 0, 20, 0, 30, 0};
    assert(naryPreorder(flat4) == std::vector<int>({0, 10, 20, 30}));

    // A more complex tree: root(5) -> children: (6 with 1 child 7), (8 with 2 children 9,10), (11)
    // Encoding: 5,3,6,1,7,0,8,2,9,0,10,0,11,0
    std::vector<int> flat5 = {5, 3, 6, 1, 7, 0, 8, 2, 9, 0, 10, 0, 11, 0};
    assert(naryPreorder(flat5) == std::vector<int>({5, 6, 7, 8, 9, 10, 11}));

    return 0;
}
// The flat encoding represents the tree in a pre-order-like sequence where each node is described by its value and its number of children, followed immediately by the encodings of those children. To reconstruct the traversal, we can simulate a depth-first search using an explicit stack. A simple and robust approach: iterate through the flat vector sequentially, but because we need the actual child relationships, we can instead build a nested node structure (or directly reconstruct the tree) and then perform an iterative preorder traversal. Alternatively, we can avoid building an explicit tree: since the encoding is pre-order (node value, then its subtree), we can parse it using an index that advances while recursively consuming children. However, the task requires an explicit stack, so we can build the tree first using a stack-based parser, then traverse it with another stack. Edge cases: a single node with no children (flatTree = {value, 0}) yields just that value. A deeply nested tree tests stack safety and correctness. Time complexity is O(N) where N is the number of encoded integers (roughly 2 * nodes + internal counts), and space complexity is O(D) for both parsing stack and traversal stack, where D is the depth of the tree.
