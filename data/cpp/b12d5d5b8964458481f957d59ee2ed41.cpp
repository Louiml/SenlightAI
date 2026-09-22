// Implement a C++ free function named `countNodesInSubtree` that, given a pointer to a `NodeCharTree` object (representing a node in an n-ary tree, where each node stores a character and has a dynamic list of child pointers), returns the total number of nodes in the subtree rooted at that node, including the node itself. The function must handle a `nullptr` input by returning 0. You may assume the tree is acyclic and that all child pointers are valid (never dangling). The function should be `const`-correct, meaning it must not modify the tree structure or node data. You must not use any external libraries beyond the standard C++ headers, and you must define the `NodeCharTree` class exactly as provided in the snippet (including fixing any obvious syntax errors so it compiles correctly). The function should be iterative or recursive, but must not rely on global or static variables.

The core task is to traverse an n-ary tree starting from a given node and count all reachable nodes. The `NodeCharTree` class has a member `std::vector<NodeCharTree*> childs` that stores pointers to its children. The natural approach is a depth-first traversal: if the input pointer is `nullptr`, return 0; otherwise, initialize a count to 1 (for the current node) and recursively add the counts from each child pointer. This is straightforward recursion with base case `nullptr`. Edge cases: (1) a leaf node (no children) returns 1; (2) a single-node tree returns 1; (3) an empty tree pointer returns 0. Complexity: the function visits every node in the subtree exactly once, so time complexity is O(n) where n is the number of nodes in the subtree. Space complexity is O(h) due to recursion stack depth, where h is the height of the subtree (worst-case O(n) for a skewed tree, but typically less). To avoid the danger of stack overflow on very deep trees, an iterative traversal using an explicit stack would be possible, but recursion is simpler and matches the typical use in tree problems. The function should be declared `int countNodesInSubtree(const NodeCharTree* node)` and be `const`-safe.

#include <vector>
#include <cstddef>

// Definition of NodeCharTree as per the snippet, with minimal fixes.
class NodeCharTree {
public:
    NodeCharTree(char info) : info(info), parent(nullptr) {}
    ~NodeCharTree() {
        for (NodeCharTree* child : childs) {
            delete child;
        }
    }
    char getInfo() const { return info; }
    void setInfo(char newInfo) { info = newInfo; }
    NodeCharTree* getParent() const { return parent; }
    NodeCharTree* getChild(int index) const {
        if (index < 0 || index >= static_cast<int>(childs.size())) {
            return nullptr;
        }
        return childs[index];
    }
    bool isRoot() const { return parent == nullptr; }
    bool isInternal() const { return parent != nullptr && !childs.empty(); }
    bool isExternal() const { return childs.empty(); }
    int degree() const { return childs.size(); }
    int depth() const {
        int depth = 0;
        NodeCharTree* current = parent;
        while (current != nullptr) {
            ++depth;
            current = current->parent;
        }
        return depth;
    }
    int size() const {
        int total = 1;
        for (const NodeCharTree* child : childs) {
            total += child->size();
        }
        return total;
    }
    void addSubtree(NodeCharTree* subtree) {
        if (subtree == nullptr) return;
        childs.push_back(subtree);
        subtree->parent = this;
    }
    bool removeSubtree(NodeCharTree* subtree) {
        for (auto it = childs.begin(); it != childs.end(); ++it) {
            if (*it == subtree) {
                childs.erase(it);
                return true;
            }
        }
        return false;
    }
    bool contains(char info) const {
        if (this->info == info) return true;
        for (const NodeCharTree* child : childs) {
            if (child->contains(info)) return true;
        }
        return false;
    }
    const NodeCharTree* find(char info) const {
        if (this->info == info) return this;
        for (const NodeCharTree* child : childs) {
            const NodeCharTree* result = child->find(info);
            if (result != nullptr) return result;
        }
        return nullptr;
    }

private:
    char info;
    NodeCharTree* parent;
    std::vector<NodeCharTree*> childs;
};

// Count the total number of nodes in the subtree rooted at 'node'.
// Returns 0 if 'node' is nullptr.
int countNodesInSubtree(const NodeCharTree* node) {
    if (node == nullptr) {
        return 0;
    }
    int count = 1; // count the current node
    for (int i = 0; i < node->degree(); ++i) {
        count += countNodesInSubtree(node->getChild(i));
    }
    return count;
}

#include <cassert>

int main() {
    // Build a tree:
    //        'a'
    //      /   \
    //    'b'   'c'
    //    / \     \
    //  'd' 'e'   'f'
    NodeCharTree* root = new NodeCharTree('a');
    NodeCharTree* b = new NodeCharTree('b');
    NodeCharTree* c = new NodeCharTree('c');
    NodeCharTree* d = new NodeCharTree('d');
    NodeCharTree* e = new NodeCharTree('e');
    NodeCharTree* f = new NodeCharTree('f');
    root->addSubtree(b);
    root->addSubtree(c);
    b->addSubtree(d);
    b->addSubtree(e);
    c->addSubtree(f);

    // Test counts
    assert(countNodesInSubtree(root) == 6);
    assert(countNodesInSubtree(b) == 3);
    assert(countNodesInSubtree(c) == 2);
    assert(countNodesInSubtree(d) == 1);
    assert(countNodesInSubtree(nullptr) == 0);

    // Test with single node tree
    NodeCharTree* single = new NodeCharTree('x');
    assert(countNodesInSubtree(single) == 1);

    // Test with deeper node
    NodeCharTree* g = new NodeCharTree('g');
    f->addSubtree(g);
    assert(countNodesInSubtree(c) == 3);
    assert(countNodesInSubtree(f) == 2);
    assert(countNodesInSubtree(root) == 7);

    // Clean up (snippet's destructor deletes children recursively)
    delete root;
    delete single; // but careful: single was never added to root, so separate delete
    return 0;
}
