/*
Write a C++ function named `getElementOcclusions` that takes a pointer to a `TreeNode` (a simple tree structure with a `value` field, a `zIndex` field, a boolean `isBox` flag, a boolean `isVisible` flag, and child/sibling pointers), an integer `targetValue` that identifies a specific node in the tree (the "plugin" node), a rectangle represented by four integers (x, y, width, height), and a reference to a vector of rectangles to populate. The function must return a vector of rectangles (each defined by x, y, width, height) representing regions of nodes that are "above" the target node in stacking order and intersect the given rectangle. A node is "above" another if it comes later in the tree traversal order **and** has a higher zIndex, or if it has an equal zIndex but is later in traversal order, or if the target node has a zIndex lower than the node in question. Only nodes with `isBox == true` and `isVisible == true` should produce output rectangles. The function should traverse the entire tree and for each qualifying node, add its axis-aligned bounding box (as given by its `x`, `y`, `width`, `height` fields) if it intersects the input rectangle. The output vector should be appended to, not cleared.
*/
#include <vector>
#include <algorithm>

struct Rectangle {
    int x;
    int y;
    int width;
    int height;
    
    bool intersects(const Rectangle& other) const {
        return !(other.x >= x + width || other.x + other.width <= x ||
                 other.y >= y + height || other.y + other.height <= y);
    }
};

struct TreeNode {
    int value;
    int zIndex;
    bool isBox;
    bool isVisible;
    int x;
    int y;
    int width;
    int height;
    TreeNode* firstChild;
    TreeNode* nextSibling;
    
    TreeNode(int v, int z, bool box, bool vis, int px, int py, int w, int h)
        : value(v), zIndex(z), isBox(box), isVisible(vis),
          x(px), y(py), width(w), height(h),
          firstChild(nullptr), nextSibling(nullptr) {}
};

// Helper: traverse tree and accumulate rectangles from nodes that are above target
// and intersect the given frameRect.
static void collectOcclusions(TreeNode* node, int targetValue, int targetZIndex,
                              bool& foundTarget, const Rectangle& frameRect,
                              std::vector<Rectangle>& occlusions) {
    if (!node) return;
    
    if (node->value == targetValue) {
        foundTarget = true;
    } else if (foundTarget && node->zIndex >= targetZIndex) {
        // This node is above target (later in traversal and zIndex qualifies)
        if (node->isBox && node->isVisible) {
            Rectangle r{node->x, node->y, node->width, node->height};
            if (r.intersects(frameRect)) {
                occlusions.push_back(r);
            }
        }
    }
    
    // Recurse over children (firstChild then nextSibling chain)
    // We must do a depth-first traversal where "later" means post-order of
    // the entire tree. Since we are using pre-order, but we need to traverse
    // all siblings after the target's subtree. However, if the target is in
    // an earlier sibling's subtree, then later siblings visited after that
    // subtree are "later". Our pre-order traversal with foundTarget flag
    // correctly marks nodes visited after the target as "later" including
    // siblings and their subtrees, because we traverse in order.
    for (TreeNode* child = node->firstChild; child; child = child->nextSibling) {
        collectOcclusions(child, targetValue, targetZIndex, foundTarget, frameRect, occlusions);
    }
}

// Main function: given a tree, a target value, and a frame rectangle,
// return all rectangles of boxes that are above the target and intersect the frame.
std::vector<Rectangle> getElementOcclusions(TreeNode* root, int targetValue,
                                            const Rectangle& frameRect) {
    std::vector<Rectangle> occlusions;
    if (!root) return occlusions;
    
    // First find the target node's zIndex (if it exists)
    int targetZIndex = 0;
    bool targetFound = false;
    // We need a separate traversal to find the target node's zIndex.
    // Use an iterative stack-based traversal to avoid recursion overhead.
    struct StackFrame {
        TreeNode* node;
        bool visited;
    };
    std::vector<StackFrame> stack;
    stack.push_back({root, false});
    while (!stack.empty()) {
        auto& top = stack.back();
        if (!top.visited) {
            top.visited = true;
            if (top.node->value == targetValue) {
                targetZIndex = top.node->zIndex;
                targetFound = true;
                break;
            }
            // Push children in reverse order to preserve pre-order
            std::vector<TreeNode*> children;
            for (TreeNode* child = top.node->firstChild; child; child = child->nextSibling)
                children.push_back(child);
            for (auto it = children.rbegin(); it != children.rend(); ++it)
                stack.push_back({*it, false});
        } else {
            stack.pop_back();
        }
    }
    
    if (!targetFound) return occlusions;
    
    // Now traverse the tree in pre-order and collect occlusions.
    bool found = false;
    collectOcclusions(root, targetValue, targetZIndex, found, frameRect, occlusions);
    return occlusions;
}
#include <cassert>

int main() {
    // Build a simple tree:
    // root (value=0, z=0, box, visible, rect (0,0,100,100))
    //   child1 (value=1, z=2, box, visible, rect (10,10,50,50))
    //   child2 (value=2, z=1, box, visible, rect (20,20,60,60))
    //   child3 (value=3, z=2, box, invisible, rect (0,0,10,10))
    //   child4 (value=4, z=0, non-box, visible, rect (5,5,5,5))
    TreeNode root(0, 0, true, true, 0, 0, 100, 100);
    TreeNode child1(1, 2, true, true, 10, 10, 50, 50);
    TreeNode child2(2, 1, true, true, 20, 20, 60, 60);
    TreeNode child3(3, 2, true, false, 0, 0, 10, 10);
    TreeNode child4(4, 0, false, true, 5, 5, 5, 5);
    
    root.firstChild = &child1;
    child1.nextSibling = &child2;
    child2.nextSibling = &child3;
    child3.nextSibling = &child4;
    
    Rectangle frame{0, 0, 100, 100};
    
    // Case 1: Target is root (z=0). All later nodes with z>=0 are above.
    // child1 (z=2) yes, child2 (z=1) yes, child3 invisible no, child4 non-box no.
    // Also, no other nodes.
    auto res = getElementOcclusions(&root, 0, frame);
    assert(res.size() == 2);
    // Verify rectangles (order may vary, but we can check contents)
    bool foundChild1 = false, foundChild2 = false;
    for (const auto& r : res) {
        if (r.x == 10 && r.y == 10 && r.width == 50 && r.height == 50) foundChild1 = true;
        if (r.x == 20 && r.y == 20 && r.width == 60 && r.height == 60) foundChild2 = true;
    }
    assert(foundChild1 && foundChild2);
    
    // Case 2: Target is child2 (z=1). Later nodes: child3 (invisible), child4 (non-box) -> none.
    // Also child1 is earlier, not above. So result empty.
    res = getElementOcclusions(&root, 2, frame);
    assert(res.empty());
    
    // Case 3: Target is child1 (z=2). Later nodes: child2 (z=1<2) not above, child3 (invisible), child4 z=0 not above. Empty.
    res = getElementOcclusions(&root, 1, frame);
    assert(res.empty());
    
    // Case 4: Target does not exist.
    res = getElementOcclusions(&root, 99, frame);
    assert(res.empty());
    
    // Case 5: Restrict frame to only intersect child1 (e.g., a small rect inside child1)
    Rectangle small{15, 15, 10, 10};
    res = getElementOcclusions(&root, 0, small);
    // Only child1 intersects (child2 rectangle starts at 20,20 so not intersecting this small rect)
    assert(res.size() == 1);
    assert(res[0].x == 10 && res[0].y == 10 && res[0].width == 50 && res[0].height == 50);
    
    return 0;
}
// The solution requires a depth-first traversal of the tree structure. We track the target node by its value. For each node encountered, we compare it against the target node to determine stacking order. The key rule: a node is "above" the target if (1) it is not the target itself, (2) its zIndex is greater than the target's zIndex, OR (3) its zIndex equals the target's zIndex but it appears later in tree traversal order (i.e., we visit it after the target). Since we are doing a pre-order traversal, we can simply record the traversal order index of the target during the first pass, then during a second pass (or by remembering order) decide if a node is "later". Alternatively, we can do a single traversal: maintain a flag `foundTarget` that becomes true after we visit the target. Any node visited after `foundTarget` is true and with zIndex >= target's zIndex is considered above (if zIndex equal, later order suffices). Note: If a node has zIndex less than the target's, it is not above even if visited later. Also, we must only output rectangles for nodes that are boxes, visible, and intersect the given rectangle. The `getTreeOcclusions` helper recursively traverses children. Edge cases: the target may not exist in the tree; then the function should return an empty result (no occlusions). Also, if the target node itself is encountered, we do not add it. The algorithm runs in O(N) time where N is the number of nodes, and uses O(H) auxiliary stack space for recursion, where H is the tree height. For a general tree, that means O(N) worst-case if it's a chain.
