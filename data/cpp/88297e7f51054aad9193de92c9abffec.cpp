/*
You are given a simplified model of a fractal tree node. Each node stores an integer `value`, a vector of child nodes (each node has at most 4 children, but the vector may be empty), and a boolean flag `canGrow` that indicates whether this node is allowed to generate new children during a growth operation. Write a C++ function `simulateFractalGrowth` that takes a root node (passed by value, so modifications do not affect the caller) and an unsigned integer `growthSteps`. The function must return a new root node (a deep copy of the input root) after applying the following growth rule for exactly `growthSteps` iterations: in each iteration, for every node in the current tree (processed in breadth‑first order, i.e., level by level from the root downward), if the node’s `canGrow` flag is true and the node currently has fewer than 4 children, add one new child node to that node. The new child’s `value` is set to the parent’s `value` plus 1, and its `canGrow` flag is set to `false` (so new children never grow further in subsequent steps). If a node already has 4 children or its `canGrow` is false, it does not generate a new child in that step. After all `growthSteps` iterations, return the deep‑copied root with all modifications applied. The input root is guaranteed to have `value >= 0` and no cycles. The function must be implemented with a descriptive name, use appropriate `const` where applicable, and be efficient. For example, if the root has `value=0`, `canGrow=true`, and no children, after one growth step the root will have one child with `value=1` and `canGrow=false`.
*/
#include <vector>
#include <queue>

// A node in the fractal tree.
struct FractalNode {
    int value;
    bool canGrow;
    std::vector<FractalNode*> children;
    
    FractalNode(int v, bool g) : value(v), canGrow(g) {}
};

// Recursive helper to deep copy a tree.
FractalNode* copyTree(const FractalNode* src) {
    if (!src) return nullptr;
    FractalNode* dst = new FractalNode(src->value, src->canGrow);
    for (const FractalNode* child : src->children) {
        dst->children.push_back(copyTree(child));
    }
    return dst;
}

// Simulate fractal growth on a deep copy of the given root.
FractalNode* simulateFractalGrowth(const FractalNode& root, unsigned int growthSteps) {
    FractalNode* newRoot = copyTree(&root);
    
    for (unsigned int step = 0; step < growthSteps; ++step) {
        // Collect all current nodes in breadth-first order.
        std::queue<FractalNode*> q;
        q.push(newRoot);
        std::vector<FractalNode*> currentNodes;
        while (!q.empty()) {
            FractalNode* node = q.front();
            q.pop();
            currentNodes.push_back(node);
            for (FractalNode* child : node->children) {
                q.push(child);
            }
        }
        
        // Process each node in the collected list.
        for (FractalNode* node : currentNodes) {
            if (node->canGrow && node->children.size() < 4) {
                node->children.push_back(new FractalNode(node->value + 1, false));
            }
        }
    }
    
    return newRoot;
}
#include <cassert>
#include <vector>

// Helper to recursively count nodes in a tree (for testing).
int countNodes(const FractalNode* node) {
    if (!node) return 0;
    int total = 1;
    for (const FractalNode* child : node->children) {
        total += countNodes(child);
    }
    return total;
}

// Helper to check if all children of a node have canGrow == false and value == parent->value+1.
bool checkChildProperties(const FractalNode* node) {
    for (const FractalNode* child : node->children) {
        if (child->canGrow != false || child->value != node->value + 1) return false;
        if (!checkChildProperties(child)) return false;
    }
    return true;
}

int main() {
    // Test 1: Root can grow, no children, one step -> root gets one child.
    FractalNode root(0, true);
    FractalNode* result1 = simulateFractalGrowth(root, 1);
    assert(result1->value == 0);
    assert(result1->canGrow == true);
    assert(result1->children.size() == 1);
    assert(result1->children[0]->value == 1);
    assert(result1->children[0]->canGrow == false);
    assert(countNodes(result1) == 2);
    delete result1;

    // Test 2: Root cannot grow, no children, many steps -> no children.
    FractalNode root2(5, false);
    FractalNode* result2 = simulateFractalGrowth(root2, 10);
    assert(result2->children.empty());
    assert(result2->value == 5);
    delete result2;

    // Test 3: Root with one existing child, root can grow, both can grow? (child canGrow=true) -> one step adds child to root and to child.
    FractalNode root3(0, true);
    root3.children.push_back(new FractalNode(1, true));
    FractalNode* result3 = simulateFractalGrowth(root3, 1);
    // Root gets one new child (now has 2 children), existing child gets one new child (now has 1 child).
    assert(result3->children.size() == 2);
    assert(result3->children[1]->canGrow == false);
    assert(result3->children[1]->value == 1);
    assert(result3->children[0]->children.size() == 1);
    assert(result3->children[0]->children[0]->value == 2);
    assert(checkChildProperties(result3));
    assert(countNodes(result3) == 4);
    delete result3;

    // Test 4: Root can grow, but already has 4 children -> no new child added.
    FractalNode root4(0, true);
    for (int i = 0; i < 4; ++i) {
        root4.children.push_back(new FractalNode(1, false));
    }
    FractalNode* result4 = simulateFractalGrowth(root4, 5);
    assert(result4->children.size() == 4);
    assert(countNodes(result4) == 5);
    delete result4;

    // Test 5: Two steps, root can grow, child created in step 1 cannot grow.
    FractalNode root5(0, true);
    FractalNode* result5 = simulateFractalGrowth(root5, 2);
    // Step1: root adds child (value1, canGrow=false). Step2: only root can add another child? Actually root still has 1 child and canGrow=true, so it adds another child. The existing child cannot grow.
    assert(result5->children.size() == 2);
    assert(result5->children[0]->children.empty());
    assert(result5->children[1]->children.empty());
    assert(countNodes(result5) == 3);
    delete result5;

    // Test 6: Original root remains unchanged (deep copy).
    FractalNode root6(10, true);
    root6.children.push_back(new FractalNode(11, true));
    FractalNode* result6 = simulateFractalGrowth(root6, 1);
    assert(root6.children.size() == 1);
    assert(result6->children.size() == 2);
    delete result6;
}
// The core operation is a breadth‑first traversal of the tree per growth step. In each step, we need to visit all nodes currently present in the tree, and for each node that qualifies (has `canGrow` true and fewer than 4 children), we append a new child. Since we are modifying the tree while traversing, we must collect the list of nodes to process first (so we do not accidentally process newly added children in the same step). The algorithm:
// 1. Deep‑copy the input root (recursive copy of `value`, `canGrow`, and the children vector).
// 2. Repeat `growthSteps` times:
//    a. Perform a level‑order traversal (using a queue) of the current tree, collecting pointers to all nodes currently present.
//    b. For each collected node, if it qualifies, add a new child (with `value = parent->value + 1`, `canGrow = false`).
// 3. Return the modified deep‑copied root.
// Edge cases: An empty tree (but problem guarantees at least a root), a root with `canGrow=false` so no children ever appear, nodes reaching the 4‑child cap causing them to stop growing in later steps, and multiple growth steps where the tree grows level by level. Complexity: Each step processes all current nodes (at most 4^k where k is the step number, but bounded by the total number of children created). The worst‑case total number of nodes after `growthSteps` steps is O(4^nodes), but in practice it is limited. The time per step is O(number of nodes at that step) and the overall time is O(sum of node counts over all steps), which is at most O(4^(growthSteps+1)). Space is O(max frontier size) for the traversal queue and O(total nodes) for the copied tree.
