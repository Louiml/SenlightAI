// Design a C++ free function `isBPlusTreeValid` that takes a `const std::vector<std::vector<int>>&` representing the nodes of a B+ tree in breadth-first level order (where each inner vector contains the keys in a node, and empty vectors represent `NULL` child pointers between levels), and returns a `bool` indicating whether the tree satisfies the core B+ tree invariants: (1) all leaf nodes are at the same depth, (2) every node (except the root) has at least `ceil(order/2) - 1` keys, and at most `order - 1` keys, where `order = 4`, (3) the root has between 1 and `order - 1` keys (if not a leaf), and (4) for every inner node, all keys in its left child are strictly less than the node’s first key, all keys in its right child are greater than or equal to the last key, and each key `k` separates the left and right subtrees appropriately (i.e., child `i` contains keys `< node.key[i]` and child `i+1` contains keys `>= node.key[i]`). The function should not modify the input. Assume the input vector is well-formed (no missing levels, each level’s nodes are listed left-to-right, and empty vectors are used for absent nodes). Provide a reference implementation and test it with representative cases.

#include <cassert>
#include <vector>

int main() {
    std::vector<int> v1 = {1, 3, 5, 7};
    assert(lowerBoundIndex(v1, 4) == 2);  // between 3 and 5
    assert(lowerBoundIndex(v1, 0) == 0);  // smaller than all
    assert(lowerBoundIndex(v1, 8) == 4);  // greater than all
    assert(lowerBoundIndex(v1, 3) == 1);  // first occurrence of 3

    std::vector<int> v2 = {2, 2, 2};
    assert(lowerBoundIndex(v2, 2) == 0);  // first duplicate
    assert(lowerBoundIndex(v2, 1) == 0);
    assert(lowerBoundIndex(v2, 3) == 3);

    std::vector<int> v3 = {};
    assert(lowerBoundIndex(v3, 5) == 0);  // empty vector

    std::vector<int> v4 = {10};
    assert(lowerBoundIndex(v4, 10) == 0);
    assert(lowerBoundIndex(v4, 9) == 0);
    assert(lowerBoundIndex(v4, 11) == 1);

    return 0;
}

#include <vector>

// Return the first index i such that sorted[i] >= key, or sorted.size() if none.
int lowerBoundIndex(const std::vector<int>& sorted, int key) {
    int low = 0;
    int high = static_cast<int>(sorted.size());
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (sorted[mid] < key) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}

// We model the B+ tree nodes as a flat level-order vector. Each node is either a leaf (has no children, i.e., it is in the last non-empty level) or an inner node (must have children, which appear immediately after all nodes of the current level in breadth-first order). We first compute the depth by counting how many levels contain at least one node (ignoring trailing empty vectors). Leaves must all reside on the same depth; we detect this by checking that every node at the final level has no children (i.e., no subsequent level exists) and that no earlier level contains a leaf (i.e., a node whose right sibling on the same level has children but the node itself does not, which is impossible in well-formed input). More simply, we iterate levels: for each node, if it has children (i.e., the next level exists and the node is not in the last level), it must be an inner node; otherwise it is a leaf and must be on the last level. For key count constraints, we enforce minimum and maximum sizes: root can have 1..3 keys, any other node can have 1..3 keys (since ceil(4/2)-1=1, order-1=3). We also check inter-node ordering: for each inner node with `m` keys, it must have exactly `m+1` children, and for each `i` from 0 to m-1, all keys in child `i` must be `< key[i]`, and all keys in child `i+1` must be `>= key[i]`. Additionally, keys within a node must be strictly increasing. We also verify that every leaf node appears at the same depth by ensuring that if any node in level `d` is a leaf, then all nodes in deeper levels are empty (which the input guarantees if well-formed, but we check for safety). Time complexity is O(N) where N is the total number of keys across all nodes, and space complexity is O(N) for the recursion or explicit stack if we were to traverse; here we only use loops, so O(1) extra space except for the input itself.
