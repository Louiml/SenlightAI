Given a tree structure where each node contains an integer value, a pointer to its parent, and a vector of child nodes, write a C++ function `bool bMoveSubtree(CNodeStatic* pcParentNode, CNodeStatic* pcNewChildNode)` that moves the subtree rooted at `pcNewChildNode` (including all its descendants) from its current parent to become a new child of `pcParentNode`. The function must deep-copy the moved subtree so that the original tree remains valid, remove the original subtree from its old parent, and return `true` on success. It must return `false` if either argument is null, if `pcNewChildNode` is a root (has no parent), or if `pcParentNode` is part of the same subtree as `pcNewChildNode` (i.e., moving a node into its own descendant would create a cycle). After the move, every node in the copied subtree must have its parent pointers correctly updated. The function should operate on a standard `CNodeStatic` class defined with public member functions for value access and child manipulation, and you may assume the tree is acyclic and contains no duplicate node pointers.
#include <cassert>

int main() {
    // Build two separate trees.
    CNodeStatic root1, root2;
    root1.vSetValue(10);
    root2.vSetValue(20);
    root1.vAddNewChild(); // child 0 of root1
    root1.vAddNewChild(); // child 1 of root1
    CNodeStatic* child1 = root1.pcGetChild(0);
    CNodeStatic* child2 = root1.pcGetChild(1);
    child1->vSetValue(11);
    child2->vSetValue(12);
    child1->vAddNewChild(); // grandchild of root1 via child1
    child1->pcGetChild(0)->vSetValue(111);

    // Test 1: Move child2 from root1 to root2.
    assert(bMoveSubtree(&root2, child2) == true);
    assert(root1.iGetChildrenNumber() == 1); // only child1 remains
    assert(root2.iGetChildrenNumber() == 1); // now has child with value 12
    assert(root2.pcGetChild(0)->iGetValue() == 12);
    assert(root2.pcGetChild(0)->pcGetParent() == &root2);
    // The move deep-copies, so original child2 pointer is stale; verify root2's child is independent.
    assert(root2.pcGetChild(0) != child2);

    // Test 2: Moving a root fails.
    assert(bMoveSubtree(&root1, &root2) == false); // root2 has no parent

    // Test 3: Moving a node into its own descendant fails (cycle prevention).
    CNodeStatic* grandchild = child1->pcGetChild(0);
    assert(bMoveSubtree(grandchild, child1) == false);

    // Test 4: Null pointers fail.
    assert(bMoveSubtree(nullptr, child1) == false);
    assert(bMoveSubtree(&root1, nullptr) == false);

    // Test 5: Move a leaf node (child1's grandchild) to root1.
    assert(bMoveSubtree(&root1, grandchild) == true);
    assert(root1.iGetChildrenNumber() == 2); // child1 + moved grandchild
    assert(root1.pcGetChild(1)->iGetValue() == 111);
    assert(root1.pcGetChild(1)->pcGetParent() == &root1);

    // Test 6: The original child1 now has no children after grandchild moved.
    assert(child1->iGetChildrenNumber() == 0);
}
#include <vector>

class CNodeStatic {
public:
    CNodeStatic() : i_val(0), pc_parent_node(nullptr) {}
    ~CNodeStatic() {}
    void vSetValue(int iNewVal) { i_val = iNewVal; }
    int iGetChildrenNumber() const { return (int)v_children.size(); }
    void vAddNewChild() { CNodeStatic c; c.pc_parent_node = this; v_children.push_back(c); }
    CNodeStatic* pcGetChild(int iChildOffset) {
        if (iChildOffset >= 0 && iChildOffset < (int)v_children.size())
            return &v_children[iChildOffset];
        return nullptr;
    }
    int iGetValue() const { return i_val; }
    CNodeStatic* pcGetParent() const { return pc_parent_node; }
    // Deep copy of the subtree rooted at this, with new parent pcNewParent.
    CNodeStatic* pcDeepCopy(CNodeStatic* pcNewParent) {
        CNodeStatic* pcCopy = new CNodeStatic();
        pcCopy->i_val = i_val;
        pcCopy->pc_parent_node = pcNewParent;
        for (CNodeStatic& child : v_children) {
            pcCopy->v_children.push_back(*(child.pcDeepCopy(pcCopy)));
        }
        return pcCopy;
    }
    // Search for a specific child pointer in this node's children vector.
    int iFindChild(const CNodeStatic* pcChild) const {
        for (int i = 0; i < (int)v_children.size(); ++i) {
            if (&v_children[i] == pcChild) return i;
        }
        return -1;
    }
    // Check if this node is an ancestor of or equal to pcOther (for cycle detection).
    bool bIsAncestorOf(const CNodeStatic* pcOther) const {
        const CNodeStatic* p = pcOther;
        while (p != nullptr) {
            if (p == this) return true;
            p = p->pc_parent_node;
        }
        return false;
    }
    // Access to children for internal manipulation (used by move function).
    std::vector<CNodeStatic>& vGetChildrenRef() { return v_children; }

private:
    int i_val;
    CNodeStatic* pc_parent_node;
    std::vector<CNodeStatic> v_children;
};

// Move the subtree rooted at pcNewChildNode to become a new child of pcParentNode.
bool bMoveSubtree(CNodeStatic* pcParentNode, CNodeStatic* pcNewChildNode) {
    if (pcParentNode == nullptr || pcNewChildNode == nullptr) return false;
    if (pcNewChildNode->pcGetParent() == nullptr) return false; // root cannot be moved
    if (pcParentNode->bIsAncestorOf(pcNewChildNode)) return false; // cycle prevention

    // Phase 1: Deep copy the subtree with new parent.
    CNodeStatic* pcCopiedRoot = pcNewChildNode->pcDeepCopy(pcParentNode);

    // Phase 2: Attach the copied root to the new parent's children.
    pcParentNode->vGetChildrenRef().push_back(*pcCopiedRoot);
    // The vector now owns a copy; free the temporary heap object.
    delete pcCopiedRoot;

    // Phase 3: Remove the original subtree from its current parent.
    CNodeStatic* pcOldParent = pcNewChildNode->pcGetParent();
    int iPos = pcOldParent->iFindChild(pcNewChildNode);
    if (iPos >= 0) {
        auto& vec = pcOldParent->vGetChildrenRef();
        vec.erase(vec.begin() + iPos);
    }

    return true;
}
// The solution requires a two-phase approach: first, create a deep copy of the subtree rooted at `pcNewChildNode` with the new parent set to `pcParentNode`, and second, remove the original subtree from its current parent. The deep copy must be performed recursively to preserve the entire subtree structure, including all descendant nodes and their parent pointers, which are re-assigned during the copy. After the copy is created and attached to `pcParentNode`, the original child node must be located in its parent's children vector and erased. The main algorithm involves: (1) validating the move conditions to prevent null pointers, root moves, and cycle creation (where `pcParentNode` is within the subtree of `pcNewChildNode`), (2) invoking a recursive copy function that clones the subtree and sets each cloned node's parent appropriately, (3) appending the cloned root to `pcParentNode`'s children vector, and (4) removing the original subtree via an index search and erase operation. Edge cases include moving a leaf (trivial), moving a subtree with arbitrarily deep nesting, and ensuring that the original tree remains intact after the removal (since the copy is independent). The `bIsFromTheSameTreeAs` check must be performed from `pcParentNode` to detect if it lies inside the subtree of `pcNewChildNode`; this prevents invalid moves that would create cycles. Time complexity is O(N) where N is the number of nodes in the moved subtree, plus O(M) for the child search in the parent, which is O(N+M) overall. Space complexity is O(N) for the copied subtree due to recursive stack depth.
