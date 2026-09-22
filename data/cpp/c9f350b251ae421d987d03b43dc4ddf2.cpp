Write a C++ function that performs a level-order traversal of a general tree (each node may have any number of children) and returns a vector of vectors, where index `i` of the outer vector contains the values of all nodes at depth `i` (root at depth 0). The node structure is `class Node { public: int val; std::vector<Node*> children; };`. You must implement the function as `std::vector<std::vector<int>> levelOrder(Node* root)`, which accepts a pointer to the root (possibly `nullptr` for an empty tree). The traversal must preserve left‑to‑right order among siblings at each level. You may not use any external libraries beyond the standard C++ headers. The returned structure must have exactly one inner vector for each non‑empty level present in the tree, and levels must appear in order from root downward.

// The solution uses a recursive depth‑first approach that builds levels based on the current depth. The helper function receives a node, its current level `lvl`, and a reference to the output vector `levels`. If the node is `nullptr`, we return immediately. If `lvl` equals the current size of `levels`, that means we are visiting this depth for the first time, so we append a new empty inner vector. We then push the node’s value into `levels[lvl]`. Finally, we loop over all child pointers and recursively call the helper with `lvl + 1`. Because the recursion visits children in the order they appear in the `children` vector, sibling order is preserved. The base case for an empty tree (`root == nullptr`) naturally results in an empty outer vector. Edge cases include a tree where some parent has no children (that level simply has that parent’s value and no deeper levels), and a tree with only a root (returns a single inner vector with one value). Time complexity is O(N) where N is the total number of nodes, because each node is visited exactly once. Space complexity is O(H) for the recursion stack in the worst case (a degenerate chain), plus O(N) for the output structure itself, so the auxiliary space is O(H) and total space is O(N + H). Since the maximum depth H in a general tree can be up to N, the worst-case auxiliary space is O(N).

#include <vector>

class Node {
public:
    int val;
    std::vector<Node*> children;
};

// Perform a level-order traversal of a general tree.
// Returns a vector of vectors, levels[i] contains node values at depth i.
std::vector<std::vector<int>> levelOrder(Node* root) {
    std::vector<std::vector<int>> levels;
    
    // Recursive helper that fills levels based on current depth.
    // Passing levels by reference so all recursive calls share the same structure.
    // 'lvl' is the current depth (0 for root).
    // The helper is a lambda to keep everything inside the function.
    // Using auto&& to allow recursion with appropriate reference capturing.
    auto order_recursive = [&](Node* node, unsigned lvl, auto&& self) -> void {
        if (!node) {
            return;
        }
        // If this is the first time we visit this level, add a new empty vector.
        if (lvl == static_cast<unsigned>(levels.size())) {
            levels.push_back({});
        }
        // Add the current node's value to its level.
        levels[lvl].push_back(node->val);
        // Recurse into all children, increasing the level by one.
        for (Node* child : node->children) {
            self(child, lvl + 1, self);
        }
    };
    
    // Start the recursion from the root at level 0.
    order_recursive(root, 0, order_recursive);
    
    return levels;
}

#include <cassert>
#include <vector>

// Node and levelOrder are assumed to be defined above (or included from header).

int main() {
    // Helper to create a node
    auto createNode = [](int val) {
        Node* n = new Node();
        n->val = val;
        return n;
    };

    // Test 1: Empty tree
    {
        std::vector<std::vector<int>> result = levelOrder(nullptr);
        assert(result.empty());
    }

    // Test 2: Single root
    {
        Node* root = createNode(5);
        auto result = levelOrder(root);
        assert(result.size() == 1);
        assert(result[0] == std::vector<int>{5});
        delete root;
    }

    // Test 3: Root with two children (each leaf)
    {
        Node* root = createNode(1);
        root->children.push_back(createNode(2));
        root->children.push_back(createNode(3));
        auto result = levelOrder(root);
        assert(result.size() == 2);
        assert(result[0] == std::vector<int>{1});
        assert(result[1] == std::vector<int>{2, 3});
        delete root->children[0];
        delete root->children[1];
        delete root;
    }

    // Test 4: Deeper tree with mixed branching
    {
        Node* root = createNode(1);
        root->children.push_back(createNode(2));
        root->children.push_back(createNode(3));
        // Add children to node 2 (depth 1)
        root->children[0]->children.push_back(createNode(4));
        root->children[0]->children.push_back(createNode(5));
        // Add one child to node 3 (depth 1)
        root->children[1]->children.push_back(createNode(6));
        // Add a leaf to node 6 (depth 3)
        root->children[1]->children[0]->children.push_back(createNode(7));

        auto result = levelOrder(root);
        assert(result.size() == 4);
        assert(result[0] == std::vector<int>{1});
        assert(result[1] == std::vector<int>{2, 3});
        assert(result[2] == std::vector<int>{4, 5, 6});
        assert(result[3] == std::vector<int>{7});

        // Clean up (recursive deletion omitted for brevity – in real code would need proper cleanup)
        // For this test, we'll manually delete all nodes to avoid leaks.
        delete root->children[0]->children[1]; // 5
        delete root->children[0]->children[0]; // 4
        delete root->children[1]->children[0]->children[0]; // 7
        delete root->children[1]->children[0]; // 6
        delete root->children[0]; // 2
        delete root->children[1]; // 3
        delete root;
    }

    // Test 5: Chain (each node has one child)
    {
        Node* root = createNode(10);
        root->children.push_back(createNode(20));
        root->children[0]->children.push_back(createNode(30));
        auto result = levelOrder(root);
        assert(result.size() == 3);
        assert(result[0] == std::vector<int>{10});
        assert(result[1] == std::vector<int>{20});
        assert(result[2] == std::vector<int>{30});
        delete root->children[0]->children[0];
        delete root->children[0];
        delete root;
    }

    return 0;
}
