// Write a C++ function `bool sameInorder(node* a, node* b)` that takes two pointers to binary trees whose nodes store `std::string` values, and returns `true` if and only if the two trees produce the same sequence of values during an in-order traversal (left subtree, current node, right subtree). The trees may have different shapes, different numbers of nodes, and the values may appear in any order consistent with the in-order traversal. The function must handle `nullptr` trees (returning `true` if both are `nullptr`, `false` if only one is) and must not modify the input trees. You may assume the `node` structure is defined as follows:
// ```cpp
// struct node {
//     node* left;
//     node* right;
//     std::string value;
//     node(const std::string& v) : left(nullptr), right(nullptr), value(v) {}
//     node(node* l, const std::string& v, node* r) : left(l), right(r), value(v) {}
// };
// ```
// The function must be implemented without relying on any external libraries beyond the C++ standard library — specifically, you cannot use Boost coroutines or similar. Instead, implement an iterative or recursive approach that compares the two in-order sequences lazily, without building a full vector of values if possible (to avoid unnecessary memory for large trees).
The core challenge is to compare two in-order traversals without materializing the entire sequence for either tree, because the trees can be large and of different shapes. The natural solution is to perform a synchronous, step-by-step in-order traversal of both trees using explicit stacks, yielding the next value from each traversal on demand; we compare the two values at each step and stop early if they differ. This is analogous to the classic `std::equal` algorithm applied to two input iterators, but here we implement the traversal manually.

Algorithm:
1. If both pointers are `nullptr`, return `true`. If exactly one is `nullptr`, return `false`.
2. Initialize two stacks, `stackA` and `stackB`, each storing nodes to visit. Also maintain a "current" pointer for each tree (`currA` and `currB`) that starts as the root.
3. Loop while either stack is non-empty or either current pointer is non-null:
   - While `currA` is non-null, push it onto `stackA` and set `currA = currA->left` (descend left).
   - While `currB` is non-null, push it onto `stackB` and set `currB = currB->left`.
   - If one stack is empty while the other is not (or one current is non-null while the other is null, but that's handled by the descent loop), the trees have different structures or lengths → return `false`. Actually, we only know at this point that we have at least one node to pop from each stack. If either stack is empty, then that traversal is exhausted while the other has more nodes → return `false`.
   - Pop the top from each stack, compare their `value` strings. If they differ, return `false`.
   - Set `currA = poppedNodeA->right` and `currB = poppedNodeB->right`.
4. After the loop, both traversals are exhausted, so return `true`.

Edge cases:
- Empty trees: both `nullptr` → `true`; one `nullptr`, one non-null → `false` (handled by the initial check; but also the loop would naturally return false because one stack/current would have nodes while the other doesn't).
- Single-node trees with the same value → `true`.
- Trees with the same values but different shapes (e.g., a degenerate chain vs balanced) → `true` if the in-order sequence matches.
- Trees with different lengths → `false` (the algorithm exits early when one stack is empty).
- Duplicate values: handled naturally by comparing each step; duplicates at different positions cause `false`.

Time complexity: Let `n` and `m` be the number of nodes in `a` and `b`. In the worst case, we traverse all nodes of the smaller tree and stop early upon mismatch, so O(min(n,m)) comparisons. Each node is pushed and popped exactly once per traversal, so O(n+m) stack operations in the worst case if both are fully traversed. Space complexity: O(h_a + h_b) where `h_a` and `h_b` are the heights of the trees, because the stacks hold at most the height of each tree during descent. For balanced trees, this is O(log n), but for degenerate trees it is O(n).
#include <string>
#include <stack>

struct node {
    node* left;
    node* right;
    std::string value;
    node(const std::string& v) : left(nullptr), right(nullptr), value(v) {}
    node(node* l, const std::string& v, node* r) : left(l), right(r), value(v) {}
};

// Compare the in-order traversal sequences of two binary trees.
// Returns true if both sequences are identical, false otherwise.
// Handles nullptr trees, different shapes, and stops early on mismatch.
bool sameInorder(node* a, node* b) {
    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr || b == nullptr) return false;

    std::stack<node*> stackA;
    std::stack<node*> stackB;
    node* currA = a;
    node* currB = b;

    while (currA != nullptr || !stackA.empty() || currB != nullptr || !stackB.empty()) {
        // Descend left on tree A
        while (currA != nullptr) {
            stackA.push(currA);
            currA = currA->left;
        }
        // Descend left on tree B
        while (currB != nullptr) {
            stackB.push(currB);
            currB = currB->left;
        }

        // If one stack is empty while the other has nodes, sequences differ
        if (stackA.empty() || stackB.empty()) {
            return false;
        }

        // Pop the next in-order node from each
        node* nextA = stackA.top(); stackA.pop();
        node* nextB = stackB.top(); stackB.pop();

        if (nextA->value != nextB->value) {
            return false;
        }

        // Move to right subtree
        currA = nextA->right;
        currB = nextB->right;
    }

    return true;
}
#include <cassert>
#include <string>

// Assume the node struct and sameInorder function are defined above.
// Include the solution code here (or link with it) before running main.

int main() {
    // Test 1: Both empty trees
    assert(sameInorder(nullptr, nullptr) == true);

    // Test 2: One empty, one non-empty
    node* single = new node("x");
    assert(sameInorder(nullptr, single) == false);
    assert(sameInorder(single, nullptr) == false);

    // Test 3: Single node same value
    node* single2 = new node("x");
    assert(sameInorder(single, single2) == true);

    // Test 4: Single node different value
    node* single3 = new node("y");
    assert(sameInorder(single, single3) == false);

    // Test 5: Same in-order sequence, different shapes
    // Tree A: root "b" with left "a" and right "c" (balanced)
    node* treeA = new node(new node("a"), "b", new node("c"));
    // Tree B: degenerate chain a -> b -> c (right only)
    node* treeB = new node("a");
    treeB->right = new node("b");
    treeB->right->right = new node("c");
    assert(sameInorder(treeA, treeB) == true);

    // Test 6: Different sequences, same length
    node* treeC = new node(new node("x"), "b", new node("c"));
    assert(sameInorder(treeA, treeC) == false);

    // Test 7: Different lengths
    node* treeD = new node("a");
    treeD->right = new node("b");
    assert(sameInorder(treeA, treeD) == false); // treeA has 3 nodes, treeD has 2

    // Test 8: Duplicate values but same sequence
    node* treeE = new node(new node("a"), "a", new node("a")); // in-order: a a a
    node* treeF = new node("a");
    treeF->right = new node("a");
    treeF->right->right = new node("a"); // in-order: a a a
    assert(sameInorder(treeE, treeF) == true);

    // Test 9: Duplicate values but different sequence
    node* treeG = new node("a");
    treeG->left = new node("a");
    treeG->right = new node("a"); // in-order: a a a, same as treeE? Actually left, root, right = a, a, a — yes same
    // Let's make different: left a, root b, right a -> a b a
    node* treeH = new node(new node("a"), "b", new node("a"));
    assert(sameInorder(treeE, treeH) == false);

    // Test 10: Complex same sequence
    // Tree I: root "d", left subtree (b with a,c), right e → a b c d e
    node* treeI = new node(new node(new node("a"), "b", new node("c")), "d", new node("e"));
    // Tree J: root "c", left (a, b), right (d, e) → a b c d e
    node* treeJ = new node(new node(new node("a"), "b", nullptr), "c", new node(new node("d"), "e", nullptr));
    // Check in-order of J: left subtree a b, root c, right subtree d e → a b c d e, yes.
    assert(sameInorder(treeI, treeJ) == true);

    // Cleanup (not strictly necessary in a test, but good practice)
    // For brevity, we skip full tree deletion; in a real context you'd use smart pointers.
}
