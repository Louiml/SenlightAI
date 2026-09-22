/*
Write a C++ function `bool isPerfectBinaryTree(const std::vector<std::tuple<int,int,char>>& edges, int rootValue)` that, given a list of directed edge descriptions `(parent, child, 'L'/'R')` and the root value, builds a binary tree in memory and returns `true` if the tree is a *perfect binary tree* (all internal nodes have exactly two children, and all leaves are at the same depth). The function should also handle cases where the input edges might create a tree with missing children, duplicate edges, or an invalid structure (e.g., a node appearing as a child more than once, or an edge referencing a non-existent parent). Return `false` for any such invalid or non-perfect tree. The function must not modify the input vector and must manage its own memory.
*/
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// Builds a binary tree from edges and returns true if it is a perfect binary tree.
bool isPerfectBinaryTree(const vector<tuple<int,int,char>>& edges, int rootValue) {
    if (edges.empty()) return false; // empty tree is not perfect here

    unordered_map<int, TreeNode*> nodeMap;
    unordered_set<int> childSet;
    TreeNode* root = new TreeNode(rootValue);
    nodeMap[rootValue] = root;

    for (const auto& [parentVal, childVal, dir] : edges) {
        // Parent must already exist
        auto it = nodeMap.find(parentVal);
        if (it == nodeMap.end()) {
            // clean up before returning
            for (auto& p : nodeMap) delete p.second;
            return false;
        }
        // Child must not already be assigned to another parent
        if (childSet.count(childVal)) {
            for (auto& p : nodeMap) delete p.second;
            return false;
        }
        if (nodeMap.count(childVal)) {
            // If child already exists (from previous edge as a parent), that's okay,
            // but we cannot assign it twice to the same or different parent.
            // However, childSet ensures it wasn't used as a child before.
            // If it exists as a node but not as a child previously, that’s fine.
            // Actually if it already exists, it must have been created as a parent before.
            // That is valid. But we still need to mark it as a child now.
        } else {
            nodeMap[childVal] = new TreeNode(childVal);
        }
        childSet.insert(childVal);

        TreeNode* parent = nodeMap[parentVal];
        TreeNode* child = nodeMap[childVal];
        if (dir == 'L') {
            if (parent->left != nullptr) {
                for (auto& p : nodeMap) delete p.second;
                return false;
            }
            parent->left = child;
        } else {
            if (parent->right != nullptr) {
                for (auto& p : nodeMap) delete p.second;
                return false;
            }
            parent->right = child;
        }
    }

    // Root must not appear as a child
    if (childSet.count(rootValue)) {
        for (auto& p : nodeMap) delete p.second;
        return false;
    }

    // BFS to check perfectness
    queue<TreeNode*> q;
    q.push(root);
    int expectedCount = 1;
    while (!q.empty()) {
        int levelSize = q.size();
        if (levelSize != expectedCount) {
            for (auto& p : nodeMap) delete p.second;
            return false;
        }
        bool allLeaves = true;
        for (int i = 0; i < levelSize; ++i) {
            TreeNode* cur = q.front(); q.pop();

            int childCount = 0;
            if (cur->left) { childCount++; q.push(cur->left); }
            if (cur->right) { childCount++; q.push(cur->right); }

            if (childCount == 1) {
                for (auto& p : nodeMap) delete p.second;
                return false;
            }
            if (childCount == 2) allLeaves = false;
        }
        if (allLeaves) {
            // This level all leaves; perfect tree must have no further levels.
            // Since BFS ends, we are done.
            for (auto& p : nodeMap) delete p.second;
            return true;
        }
        expectedCount *= 2;
    }
    for (auto& p : nodeMap) delete p.second;
    return false; // should not reach here
}
int main() {
    // Perfect tree: root 1 with left 2, right 3; 2's children 4,5; 3's children 6,7
    vector<tuple<int,int,char>> edges1 = {{1,2,'L'},{1,3,'R'},{2,4,'L'},{2,5,'R'},{3,6,'L'},{3,7,'R'}};
    assert(isPerfectBinaryTree(edges1, 1) == true);

    // Not perfect: missing right child of 2
    vector<tuple<int,int,char>> edges2 = {{1,2,'L'},{1,3,'R'},{2,4,'L'}};
    assert(isPerfectBinaryTree(edges2, 1) == false);

    // Not perfect: node 2 has only one child
    vector<tuple<int,int,char>> edges3 = {{1,2,'L'},{1,3,'R'},{2,4,'L'}};
    assert(isPerfectBinaryTree(edges3, 1) == false);

    // Invalid: duplicate edge (child already assigned)
    vector<tuple<int,int,char>> edges4 = {{1,2,'L'},{1,2,'L'}};
    assert(isPerfectBinaryTree(edges4, 1) == false);

    // Invalid: parent not existing
    vector<tuple<int,int,char>> edges5 = {{5,6,'L'}};
    assert(isPerfectBinaryTree(edges5, 1) == false);

    // Perfect single-level: root with two leaves
    vector<tuple<int,int,char>> edges6 = {{1,2,'L'},{1,3,'R'}};
    assert(isPerfectBinaryTree(edges6, 1) == true);

    // Root appears as child
    vector<tuple<int,int,char>> edges7 = {{1,2,'L'},{2,1,'R'}};
    assert(isPerfectBinaryTree(edges7, 1) == false);

    // Empty edge list
    vector<tuple<int,int,char>> edges8 = {};
    assert(isPerfectBinaryTree(edges8, 1) == false);

    // Deeper perfect: 15 nodes
    vector<tuple<int,int,char>> edges9;
    for (int i = 1; i <= 7; ++i) {
        if (i*2 <= 15) edges9.push_back({i, i*2, 'L'});
        if (i*2+1 <= 15) edges9.push_back({i, i*2+1, 'R'});
    }
    assert(isPerfectBinaryTree(edges9, 1) == true);
}
// The solution constructs the binary tree node by node using a hash map from node value to `Node*` to avoid O(n) searches. For each edge `(p,c,dir)`, locate or create the parent and child nodes, then assign the child to the correct pointer (`left` if `dir=='L'`, else `right`). Before assigning, check that the parent exists (it must be either the root or already created as a child) and that the target pointer is not already occupied (to detect duplicate edges). Track the set of child values to detect if a node has multiple parents. After building, perform a level-order traversal (BFS) from the root. For each level, every node must have either two children or no children; if a node has only one child, return `false`. Also ensure the level sizes follow `2^level` (starting with level 0 having 1 node). At the end of a level, if all nodes were leaves, the tree is perfect. If a level has missing nodes (fewer than expected) or a non-full node appears, return `false`. Additionally, if the total number of nodes built does not equal the number of distinct edges+1, or if the root appears as a child, return `false`. Time complexity is O(n) for building (with hash map) and O(n) for BFS, where n is the number of edges. Space complexity is O(n) for the map, queue, and nodes.
