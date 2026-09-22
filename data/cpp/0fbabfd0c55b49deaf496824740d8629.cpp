/*
Write a C++ function named `bottomView` that takes a binary tree root (pointer to `Node` where each node has an integer `data` and left/right child pointers) and returns a `vector<int>` representing the bottom view of the tree. The bottom view is defined as the set of nodes visible when looking at the tree from the bottom, considering horizontal distance (HD). Each node's HD is defined as: root has HD=0, moving to left child decrements HD by 1, moving to right child increments HD by 1. For each distinct HD, we need the value of the *lowest* node at that HD (i.e., the node with maximum depth). If two nodes share the same HD and same depth, choose the *rightmost* one (i.e., the one encountered later in a level-order traversal). The result should be ordered from the smallest HD (leftmost) to largest HD (rightmost). The tree is non-empty. Edge cases: single node, skewed trees, nodes at same HD but different depths, and trees with only right or only left children.
*/
#include <vector>
#include <map>
#include <queue>
#include <utility>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int x) : data(x), left(nullptr), right(nullptr) {}
};

// Return the bottom view of the binary tree as a vector of node values.
// Horizontal distance: root=0, left child -1, right child +1.
// For each HD, pick the node with maximum depth; ties go to rightmost.
std::vector<int> bottomView(const Node* root) {
    std::vector<int> result;
    if (root == nullptr) return result;

    // Map from horizontal distance to node value (overwritten with deeper nodes)
    std::map<int, int> hdToValue;
    // Queue for BFS: pair of (node, horizontal distance)
    std::queue<std::pair<const Node*, int>> q;
    q.push({root, 0});

    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        const Node* node = current.first;
        int hd = current.second;

        // Overwrite: since BFS goes level by level, later entries are deeper
        hdToValue[hd] = node->data;

        // Push children with adjusted HD
        if (node->left)  q.push({node->left,  hd - 1});
        if (node->right) q.push({node->right, hd + 1});
    }

    // Collect values in increasing HD order
    for (const auto& entry : hdToValue) {
        result.push_back(entry.second);
    }
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: Single node
    Node* root1 = new Node(1);
    assert((bottomView(root1) == std::vector<int>{1}));
    delete root1;

    // Test 2: Balanced tree (from example)
    Node* root2 = new Node(1);
    root2->left = new Node(2);
    root2->right = new Node(3);
    root2->left->right = new Node(4);
    root2->right->right = new Node(6);
    root2->right->left = new Node(5);
    // Expected bottom view: 2, 4, 5, 6 (HDs: -1,0,1,2)
    // But note: at HD=0 we have nodes 1 (depth0) and 4 (depth2), so 4 wins.
    assert((bottomView(root2) == std::vector<int>{2, 4, 5, 6}));

    // Test 3: Skewed right
    Node* root3 = new Node(1);
    root3->right = new Node(2);
    root3->right->right = new Node(3);
    // HDs: 0,1,2 → bottom view: 1,2,3
    assert((bottomView(root3) == std::vector<int>{1, 2, 3}));

    // Test 4: Nodes at same HD and depth (tie – rightmost wins)
    Node* root4 = new Node(1);
    root4->left = new Node(2);
    root4->right = new Node(3);
    root4->left->left = new Node(4);
    root4->right->right = new Node(5);
    // At HD=-1: node 2 (depth1) only; HD=0: nodes 1 (depth0), deep? none else; but node 4 at HD=-2, node 5 at HD=2.
    // Actually bottom: HD=-2→4, HD=-1→2, HD=0→1? Wait BFS order: level0:1(HD0), level1:2(HD-1),3(HD1), level2:4(HD-2),5(HD2). No ties.
    // Test tie: root with left=2, right=3, left.right=4, right.left=5. Then HD=0 has both 1 and 4 and 5? Let's compute:
    Node* root4b = new Node(1);
    root4b->left = new Node(2);
    root4b->right = new Node(3);
    root4b->left->right = new Node(4); // HD = -1+1 = 0
    root4b->right->left = new Node(5); // HD = 1-1 = 0
    // BFS level0: 1(HD0) → map[0]=1
    // level1: 2(HD-1) → map[-1]=2; 3(HD1) → map[1]=3
    // level2: 4(HD0) → map[0]=4; 5(HD0) → map[0]=5 (rightmost tie)
    // So bottom view: HD-1:2, HD0:5, HD1:3 → [2,5,3]
    assert((bottomView(root4b) == std::vector<int>{2, 5, 3}));

    // Test 5: Empty tree (but spec says non-empty, still test for completeness)
    Node* root5 = nullptr;
    assert(bottomView(root5).empty());

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The problem is solved using a level-order traversal (BFS) with a queue that stores pairs of {node, HD}. We maintain a map from HD to the node's data value. Since BFS processes nodes level by level (by depth), when we encounter a node at a given HD, we *overwrite* the value in the map with the current node's data. This ensures that at the end, for each HD, we have the data of the last (deepest) node processed at that HD. Because BFS processes nodes from left to right within the same level, if two nodes share the same HD and depth, the later (rightmost) one overwrites the earlier, satisfying the tie-breaking rule. After processing all nodes, iterate over the map (sorted by HD) and collect values into a vector. Edge cases: empty tree (return empty vector), single node (return its value), and uneven tree where some HDs have only one node. Time complexity: O(N log N) due to map insertions, where N is the number of nodes; space O(N) for queue and map. If using an unordered_map with sorting at the end, it would be O(N) but map is simpler.
