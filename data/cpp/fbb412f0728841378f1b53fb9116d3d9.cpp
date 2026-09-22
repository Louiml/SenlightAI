Write a C++ function `std::string buildNewick(const std::vector<int>& left, const std::vector<int>& right)` that takes two parallel vectors representing the left and right child indices of a binary tree (where leaves are nodes with indices `0` to `n-1` for `n` leaves, and internal nodes have indices `n` to `m-1`, where `m = left.size()`). The tree is rooted at the last internal node (index `m-1`). The function should serialize the tree into a Newick string format: for a leaf, output its index followed by `:1.0`; for an internal node, output `(` + left subtree + `,` + right subtree + `):1.0`. The root should be enclosed in parentheses and end with a semicolon, e.g., `(0:1.0,1:1.0):1.0;` for a tree with two leaves connected to a root. The input guarantees that each node (except the root) has exactly one parent, and child indices are valid (either leaf or internal). Empty input (both vectors empty) should return the empty string. Assume the tree is a proper binary tree (every internal node has exactly two children, and no node is child of more than one parent). The function must be const-correct, use only standard headers, and not modify the input vectors.
The solution simulates an iterative post-order traversal of the tree using a stack or manual state tracking, similar to the `store` method in the provided code. The key is to traverse from the root (last index) and emit the Newick format exactly: for a leaf (index < `left.size()/2`? Wait, leaves are indices `0` to `n-1` where `n` is the number of leaves; but we don't know `n` directly. However, the number of leaves can be derived from the fact that in a full binary tree with `i` internal nodes, there are `i+1` leaves. But the vectors contain both leaves and internal nodes. The leaf nodes are those that never appear as a parent (i.e., they are not indices in `left` or `right`? Actually, internal node indices are from `n` to `m-1` where `n` is number of leaves and `m` is total nodes. So an index `i` is a leaf if `i < n` where `n = (m+1)/2` for full binary tree, but we don't know. Simpler: a node is a leaf if it is not a parent of any node, i.e., it does not appear as `left[i]` or `right[i]` for any `i`. However, the function doesn't know `n` explicitly. But we can infer: since every internal node has index >= number of leaves, and leaves have indices < some threshold, but we can just check: if `i` is less than the number of leaves, but we don't know that. Actually, the problem states that leaves are indices `0` to `n-1` and internal are `n` to `m-1`. So the number of leaves is `m - (number of internal nodes)`. In a full binary tree, internal nodes = m - n, and n = internal_nodes + 1? Actually, for a full binary tree where every internal node has exactly 2 children, the number of leaves = number of internal nodes + 1. So if total nodes = m = leaves + internal, and leaves = internal + 1, then m = 2*internal + 1, so internal = (m-1)/2, leaves = (m+1)/2. So the leaf indices are from 0 to (m-1)/2 inclusive? But this assumes a full binary tree and that indices are assigned contiguously. The problem statement says leaves are `0` to `n-1` and internal `n` to `m-1`, so we can compute n = (m+1)/2 for a full binary tree. But to be safe, we can determine leaf status by checking if the node index is not used as a parent: i.e., node `i` is a leaf if for all `j`, `left[j] != i` and `right[j] != i`. However, that would be O(m^2). Better: since the tree is proper and rooted at the last internal node, we know that the number of leaves = (m+1)/2. So we can compute `n = (left.size() + 1) / 2`? Wait, `m = left.size()`. For a full binary tree, number of internal nodes = (m-1)/2, leaves = (m+1)/2. So leaf indices are `0` to `(m-1)/2` (inclusive). For example, if m=3 (two leaves and one internal), internal node index is 2 (since leaves 0,1, internal 2). (m-1)/2 = 1, so leaves are 0 and 1. Yes. So condition: `i < (m+1)/2`? Actually leaf indices are 0.. (m-1)/2. Check: m=3, (m-1)/2=1, leaves 0,1. Yes. So if `i <= (m-1)/2` then leaf. But carefully: if m=1? That would mean no internal nodes, just a single leaf? But the problem says root is last internal node, so m must be at least? Actually, a tree with one leaf has no internal nodes, but the problem says vectors have at least one internal node? The spec says "rooted at the last internal node (index m-1)" so m must be at least 1? But if m=0 (empty), we return empty string. For m>=1, the root is an internal node, so m must be odd? Not necessarily; a proper binary tree with leaves count = internal+1, so total m = internal + leaves = internal + (internal+1) = 2*internal+1, so m is odd. So m>=1 and odd. Thus leaves are indices 0..internal-1? Wait, leaves count = internal+1. If internal nodes are indices from internal to 2*internal? Actually, let internal = k. Then leaves = k+1, total m = 2k+1. Leaves indices 0..k, internal indices k+1..2k. So root index = 2k = m-1. Thus leaf condition: i <= (m-1)/2. For m=3, k=1, leaves 0,1, (m-1)/2=1. Good.

So the algorithm: iterative DFS with explicit stack or using a visitation count array similar to the provided `store` method. We'll use a vector of state integers (0,1,2) to track how many children have been processed for each node. We start at root `m-1`. We'll build the output using a string stream. For each node:
- If it's a leaf (index <= (m-1)/2), output the index as string, then `:1.0`, and move to parent.
- If internal:
  - If state == 0: output `(`, push left child onto stack (prep for future), increment state, move to left child.
  - If state == 1: output `,`, push right child, increment state, move to right child.
  - If state == 2: output `):1.0`, if not root (i.e., not the start index) then move to parent; if root, output `;` and break.

We need to track parent pointers. We can use a stack of parent indices or a `prev` array. Since we don't know parent for each node, we can maintain a stack `parents` where we push parent when descending. Simpler: use a stack of tuples (node, state) but state is separate. We'll use an explicit stack `std::vector<int> nodeStack` and `state` array.

Edge cases: empty vectors return "". Single leaf? But spec says root is internal node, so m must be at least 3? Actually, could have a tree with two leaves and one internal, m=3. Could have a tree with one leaf? That would have no internal nodes, but vectors would be empty? The problem says proper binary tree, so every internal node has two children. A tree with a single leaf is not a proper binary tree (no internal nodes). So we can assume m>=3 if not empty. But to be safe, if m==1? That would mean one internal node with no children? Not allowed. So we can just handle general case: if left.empty() return "".

Time complexity: O(m) because each node visited a constant number of times. Space: O(m) for the state array and stack.

Test cases: Balanced tree with 4 leaves: left = {2,4,6?} Let's design a simple tree: leaves 0,1,2,3; internal nodes 4,5,6? Actually with 4 leaves, internal = 3 (nodes 4,5,6). Example: root=6 has left=4, right=5; node4 left=0, right=1; node5 left=2, right=3. So left vector = {0,2,4} (for nodes 4,5,6? Wait, need to index internal nodes 4,5,6. So left[0] for node4 =0, left[1] for node5=2, left[2] for node6=4. Right: right[0]=1, right[1]=3, right[2]=5. So left={0,2,4}, right={1,3,5}. Expected Newick: `((0:1.0,1:1.0):1.0,(2:1.0,3:1.0):1.0):1.0;` But order? The serialization should be exactly that.

Also test a degenerate left-skewed? But it's a binary tree, not necessarily balanced.

We'll implement `buildNewick` using the above.
#include <string>
#include <vector>
#include <sstream>

// Convert a binary tree represented by left/right child vectors into a Newick string.
// Leaves have indices 0..(m-1)/2, internal nodes have indices (m+1)/2..m-1, root is m-1.
std::string buildNewick(const std::vector<int>& left, const std::vector<int>& right) {
    if (left.empty()) return "";

    const int m = static_cast<int>(left.size());
    const int leafCount = (m + 1) / 2; // number of leaves

    std::ostringstream oss;
    std::vector<int> state(m, 0);
    std::vector<int> nodeStack;
    nodeStack.reserve(m);

    int cur = m - 1; // root index
    nodeStack.push_back(cur);

    while (!nodeStack.empty()) {
        cur = nodeStack.back();

        if (cur < leafCount) {
            // leaf node
            oss << cur << ":1.0";
            nodeStack.pop_back();
        } else {
            // internal node
            if (state[cur] == 0) {
                oss << '(';
                ++state[cur];
                nodeStack.push_back(left[cur]);
            } else if (state[cur] == 1) {
                oss << ',';
                ++state[cur];
                nodeStack.push_back(right[cur]);
            } else {
                // processed both children
                oss << "):1.0";
                nodeStack.pop_back();
            }
        }
    }

    // Add semicolon at the end
    std::string result = oss.str();
    if (!result.empty()) {
        result += ';';
    }
    return result;
}
#include <cassert>
#include <string>
#include <vector>

// Solution function declaration (as above)
std::string buildNewick(const std::vector<int>& left, const std::vector<int>& right);

int main() {
    // Simple tree: two leaves (0,1) and one internal root (2)
    std::vector<int> left1 = {0};
    std::vector<int> right1 = {1};
    assert(buildNewick(left1, right1) == "(0:1.0,1:1.0):1.0;");

    // Balanced tree with 4 leaves: leaves 0,1,2,3; internal nodes 4,5,6
    std::vector<int> left2 = {0, 2, 4};
    std::vector<int> right2 = {1, 3, 5};
    // Expected: ((0:1.0,1:1.0):1.0,(2:1.0,3:1.0):1.0):1.0;
    assert(buildNewick(left2, right2) == "((0:1.0,1:1.0):1.0,(2:1.0,3:1.0):1.0):1.0;");

    // Left-skewed tree: leaves 0,1,2; internal nodes 3,4
    // root=4 left=3 right=2; node3 left=0 right=1
    std::vector<int> left3 = {0, 3};
    std::vector<int> right3 = {1, 2};
    // Expected: ((0:1.0,1:1.0):1.0,2:1.0):1.0;
    assert(buildNewick(left3, right3) == "((0:1.0,1:1.0):1.0,2:1.0):1.0;");

    // Empty input
    std::vector<int> left4, right4;
    assert(buildNewick(left4, right4) == "");

    // Single internal node with two leaves (already tested, but duplicate)
    std::vector<int> left5 = {0};
    std::vector<int> right5 = {1};
    assert(buildNewick(left5, right5) == "(0:1.0,1:1.0):1.0;");

    // Larger: 8 leaves, full balanced tree
    // Leaves 0-7, internal 8-14
    // Node8:0,1; 9:2,3; 10:4,5; 11:6,7; 12:8,9; 13:10,11; 14:12,13
    std::vector<int> left6 = {0,2,4,6,8,10,12};
    std::vector<int> right6 = {1,3,5,7,9,11,13};
    std::string expected = "(((0:1.0,1:1.0):1.0,(2:1.0,3:1.0):1.0):1.0,((4:1.0,5:1.0):1.0,(6:1.0,7:1.0):1.0):1.0):1.0;";
    assert(buildNewick(left6, right6) == expected);

    return 0;
}
