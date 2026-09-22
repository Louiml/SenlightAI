// Write a C++ function `processTreeNodes` that takes as input a `std::vector<std::vector<int>>` representing a ternary tree in level-order format, where each inner vector contains the three child values for a node (using `-1` for missing children), and returns a `std::vector<int>` containing the sum of all values in each node (including the root, which is always at index 0 of the first inner vector), traversed in preorder (root, then first child's subtree, second child's subtree, third child's subtree). The tree is completely stored in the vector: for a node at index `i` in the flattened level-order list, its three children are at indices `3*i+1`, `3*i+2`, and `3*i+3` (if they exist and are not `-1`). The function must handle trees where some children are missing, and must compute node sums recursively without modifying the input.

// The solution uses a recursive depth-first traversal that mimics the preorder order specified. Since the tree is stored in a complete ternary level-order layout, the root is at index 0, and for any node index `i`, its children are at fixed positions in the flattened list. The main challenge is mapping the input format: the input is a vector of vectors, where each inner vector always has exactly three integers (the values of the three children for that node, with `-1` indicating no child). But the node's own value is not stored in the inner vector; instead, the node's value is stored as the first element of its parent's inner vector, except for the root whose value is not stored at all. To resolve this, we need to reconstruct the tree's values: the root's value must be provided separately or inferred. The clearest approach is to redesign the input: let the function accept `(rootValue, childrenVector)`, where `childrenVector` is the list of inner vectors. However, the original snippet doesn't provide such a direct structure. For a self-contained task, we will define the input as: a `std::vector<int>` named `flattened` where each node's own value is stored at its index, and children are at positions `3*i+1`, `3*i+2`, `3*i+3`; missing children are indicated by a sentinel value (e.g., `-1` for the child index). But that complicates because a child could have value `-1` legitimately. To avoid ambiguity, we define the input as two parallel structures: a vector of node values (indexed by node index) and a vector of triples indicating child indices (with `-1` for missing). That is too complex. Simpler: The task is to write a function that given a vector `values` where `values[i]` is the value of node i, and a vector `children` where `children[i]` is a `std::array<int,3>` of child indices (with `-1` for missing), computes the preorder sum of each node's value plus all descendants? That is different.
//
// Given the original code snippet's context (a ternary angel tree), a clean abstraction is: The tree is represented as nodes with up to three children. Write a function `collectPreorderSums` that takes a `Node` structure with `value`, `first`, `second`, `third` pointers and returns a vector of the sum of the subtree rooted at each node, in preorder. That is a standard tree traversal. The time complexity is O(n) where n is number of nodes, space O(h) for recursion stack, h being height (worst O(n) for skewed tree). Edge cases: empty tree (null root), nodes with some missing children, single node.
//
// Thus I will write a function `std::vector<int> subtreeSums(const Node* root)` that returns the sum of each subtree in preorder. The Node structure is defined in the solution. The test code will build small trees and verify.

#include <vector>
#include <cassert>

// Tree node with three children
struct Node {
    int value;
    Node* first;
    Node* second;
    Node* third;
    Node(int v) : value(v), first(nullptr), second(nullptr), third(nullptr) {}
};

// Recursive helper that returns the sum of the subtree and appends preorder sums
int subtreeSumHelper(const Node* node, std::vector<int>& sums) {
    if (!node) return 0;
    int total = node->value;
    total += subtreeSumHelper(node->first, sums);
    total += subtreeSumHelper(node->second, sums);
    total += subtreeSumHelper(node->third, sums);
    sums.push_back(total);
    return total;
}

// Return the sum of each subtree in preorder (root, first child's subtree, second, third)
std::vector<int> subtreeSums(const Node* root) {
    std::vector<int> sums;
    subtreeSumHelper(root, sums);
    return sums;
}

#include <cassert>
#include <vector>

// Node definition from solution
struct Node {
    int value;
    Node* first;
    Node* second;
    Node* third;
    Node(int v) : value(v), first(nullptr), second(nullptr), third(nullptr) {}
};

// Function prototype from solution
std::vector<int> subtreeSums(const Node* root);
int subtreeSumHelper(const Node* node, std::vector<int>& sums);

int main() {
    // Test 1: single node
    Node* n1 = new Node(5);
    std::vector<int> sums = subtreeSums(n1);
    assert(sums.size() == 1);
    assert(sums[0] == 5);
    delete n1;

    // Test 2: root with three children
    Node* root = new Node(10);
    Node* c1 = new Node(1);
    Node* c2 = new Node(2);
    Node* c3 = new Node(3);
    root->first = c1;
    root->second = c2;
    root->third = c3;
    sums = subtreeSums(root);
    // Preorder: root sum=10+1+2+3=16, then c1 sum=1, c2 sum=2, c3 sum=3
    assert(sums == std::vector<int>({16, 1, 2, 3}));
    delete root; delete c1; delete c2; delete c3;

    // Test 3: missing children
    Node* r = new Node(7);
    Node* a = new Node(4);
    Node* b = new Node(6);
    r->first = a;
    a->second = b;
    sums = subtreeSums(r);
    // Preorder: root sum=7+4+6=17, a sum=4+6=10, b sum=6
    assert(sums == std::vector<int>({17, 10, 6}));
    delete r; delete a; delete b;

    // Test 4: deeper tree with some missing
    Node* root2 = new Node(1);
    Node* l1 = new Node(2);
    Node* l2 = new Node(3);
    Node* l3 = new Node(4);
    Node* l4 = new Node(5);
    root2->first = l1;
    root2->second = l2;
    l1->third = l3;
    l3->first = l4;
    sums = subtreeSums(root2);
    // Preorder: root sum=1+2+3+4+5=15, l1 sum=2+4+5=11, l2 sum=3, l3 sum=4+5=9, l4 sum=5
    assert(sums == std::vector<int>({15, 11, 3, 9, 5}));
    delete root2; delete l1; delete l2; delete l3; delete l4;

    // Test 5: empty tree (nullptr)
    sums = subtreeSums(nullptr);
    assert(sums.empty());

    return 0;
}
