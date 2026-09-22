// Given the `BinaryTree` class definition provided in the snippet (with `Position`, `Node`, `expendExternal`, `removeAboveExternal`, `positions()`, etc.), write a standalone C++ free function named `int treeHeight(const BinaryTree& T)` that computes the height of the binary tree. The height is defined as the maximum depth among all external (leaf) nodes, where depth is measured as the number of edges from the root to that node. The root has depth 0. The tree may be empty, in which case return -1. You must not modify the class; use only its public interface (`root()`, `positions()`, `left()`, `right()`, `isExternal()`, `size()`). The input tree is properly linked and all internal nodes have exactly two children (as guaranteed by the class's `expendExternal` method). Handle the case of an empty tree and a tree with only a root.
The approach is to traverse all positions in the tree using the `positions()` method, which performs a preorder traversal and returns a vector of `Position` objects. For each position, we can determine if it is external (i.e., a leaf) using `isExternal()`. If it is external, we need its depth. The depth of a position can be computed by following parent pointers upward until the root, counting the number of edges. Alternatively, we could compute the height recursively from the root, but since the task restricts to using the provided public interface, we can compute depth per leaf by iterating with `parent()`. The height is the maximum depth among all leaves. Edge cases: empty tree -> return -1; tree with only root -> depth of root is 0, so height = 0. Time complexity: O(n * h) where n is number of nodes and h is the tree height, since for each leaf we walk up to the root. In the worst case (skewed tree) this is O(n^2). We can improve by computing depth during the traversal using a recursive helper that tracks depth, but since we only use public interface, we can write a recursive function that takes a `Position` and current depth, updating max depth when external. That would be O(n) time. The provided code has a `depth` method that does exactly that per node, but it is not const and takes pointer; we can implement our own helper using `Position` by value. We'll use a helper that recursively descends left and right (if not external) and that naturally yields O(n) time because each node is visited once. Space complexity: O(h) for recursion stack.
#include <algorithm>

// Helper: recursively compute max depth of external nodes.
int treeHeightHelper(const BinaryTree::Position& p, int depth) {
    if (p.isExternal()) {
        return depth;
    }
    int leftHeight = treeHeightHelper(p.left(), depth + 1);
    int rightHeight = treeHeightHelper(p.right(), depth + 1);
    return std::max(leftHeight, rightHeight);
}

// Compute the height of a binary tree.
// Height = max depth of any leaf (root depth = 0). Empty tree -> -1.
int treeHeight(const BinaryTree& T) {
    if (T.empty()) {
        return -1;
    }
    return treeHeightHelper(T.root(), 0);
}
#include <cassert>

int main() {
    // Test 1: Empty tree
    BinaryTree t1;
    assert(treeHeight(t1) == -1);

    // Test 2: Tree with only root
    BinaryTree t2;
    t2.addRoot();  // root is external
    assert(treeHeight(t2) == 0);

    // Test 3: Full binary tree with 3 levels (root + 2 children + 4 leaves)
    BinaryTree t3;
    t3.addRoot();
    BinaryTree::Position r = t3.root();
    *r = 1;
    t3.expendExternal(r);  // now root has two children (external)
    BinaryTree::Position l = r.left();
    BinaryTree::Position rr = r.right();
    *l = 2; *rr = 3;
    t3.expendExternal(l);  // l has two children (external)
    t3.expendExternal(rr); // rr has two children (external)
    // Now leaves: l.left, l.right, rr.left, rr.right; height = 2
    assert(treeHeight(t3) == 2);

    // Test 4: Left-skewed tree (each internal has only left child)
    // We'll build manually: root -> left -> left, but class requires internal nodes have both children.
    // So use expendExternal always, then remove right child to create skew.
    BinaryTree t4;
    t4.addRoot();
    BinaryTree::Position root4 = t4.root();
    *root4 = 1;
    t4.expendExternal(root4); // root has left and right (both external)
    BinaryTree::Position left4 = root4.left();
    BinaryTree::Position right4 = root4.right();
    *left4 = 2; *right4 = 3;
    // Remove right external to make left a leaf's parent? Actually we want skew with left path.
    // Better: remove right external above root, resulting in root having only left child (and left is external)
    BinaryTree::Position afterRemove = t4.removeAboveExternal(right4); // removes right4 and its parent (root), but root is parent? Actually removeAboveExternal removes w (right4) and v (root), and sib (left4) becomes new root.
    // Now tree has single node left4, which is external.
    assert(treeHeight(t4) == 0);

    // Test 5: More complex tree constructed via expendExternal
    BinaryTree t5;
    t5.addRoot();
    BinaryTree::Position r5 = t5.root();
    *r5 = 1;
    t5.expendExternal(r5);
    BinaryTree::Position l5 = r5.left();
    BinaryTree::Position rr5 = r5.right();
    *l5 = 2; *rr5 = 3;
    t5.expendExternal(l5);
    // Now l5 has two leaves; r5 right child (rr5) is still external.
    // Height: leaves at depth 2 (l5's children) and depth 1 (rr5) -> max depth = 2.
    assert(treeHeight(t5) == 2);

    // Test 6: Tree with root having left child that is external, right child internal with one leaf
    BinaryTree t6;
    t6.addRoot();
    BinaryTree::Position r6 = t6.root();
    *r6 = 1;
    t6.expendExternal(r6);
    BinaryTree::Position l6 = r6.left();
    BinaryTree::Position rr6 = r6.right();
    *l6 = 2; *rr6 = 3;
    t6.expendExternal(rr6);
    // rr6 has two leaves; l6 is external. Height = 2 (from rr6's leaves)
    assert(treeHeight(t6) == 2);

    // Test 7: Chain of length 3 (root->left->left) using removeAboveExternal
    BinaryTree t7;
    t7.addRoot();
    BinaryTree::Position r7 = t7.root();
    *r7 = 1;
    t7.expendExternal(r7);
    BinaryTree::Position l7 = r7.left();
    BinaryTree::Position rr7 = r7.right();
    *l7 = 2; *rr7 = 3;
    // Remove rr7 to make a chain: now root has only left child l7, and l7 is external.
    t7.removeAboveExternal(rr7); // removes rr7 and root, but root is parent? Wait: removeAboveExternal(p) removes p and its parent, and returns sibling. If p=rr7, parent is root, sibling is l7. So root is removed, l7 becomes root, and l7 has no children (since it was external). That's not chain.
    // To build a chain, we need to keep root, remove right child, then expand left child, then remove its right child, etc.
    // Simpler: use a different approach: build a tree with root->left, then expand left, then remove left's right child.
    BinaryTree t8;
    t8.addRoot();
    BinaryTree::Position r8 = t8.root();
    *r8 = 1;
    t8.expendExternal(r8);
    BinaryTree::Position l8 = r8.left();
    BinaryTree::Position rr8 = r8.right();
    *l8 = 2; *rr8 = 3;
    // Remove right child of root to make root have only left child (which is external)
    // We need to remove rr8 and its parent (root), but that removes root. So instead, we can't have a node with only one child in this class structure.
    // Given the class design, every internal node must have exactly two children (expendExternal always adds two). So skewed trees are not possible.
    // We'll test with a balanced tree and a tree where one side is deeper.
    //
    // Test 7: Balanced tree of height 3 (15 nodes)
    BinaryTree t9;
    t9.addRoot();
    BinaryTree::Position r9 = t9.root();
    *r9 = 1;
    t9.expendExternal(r9);
    // Now expand both children
    BinaryTree::Position l9 = r9.left();
    BinaryTree::Position rr9 = r9.right();
    t9.expendExternal(l9);
    t9.expendExternal(rr9);
    // Now leaves at depth 2. Expand one leaf to make height 3
    BinaryTree::Position deep = l9.left(); // external
    t9.expendExternal(deep);
    // Now max depth is 3 (deep's children)
    assert(treeHeight(t9) == 3);
}
