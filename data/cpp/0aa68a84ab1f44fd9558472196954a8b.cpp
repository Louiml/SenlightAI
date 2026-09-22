// Write a C++ function `bool isTree(const std::map<int, std::vector<int>>& treeMap)` that determines whether a directed graph represented as an adjacency list (where each key is a source node and its vector contains destinations of directed edges) forms a valid tree. A valid directed tree must satisfy: (1) there is exactly one node with no incoming edges (the root), and every other node has exactly one incoming edge; (2) the graph is connected (all nodes reachable from the root); (3) there are no cycles (including self-loops and multiple paths), and no node has more than one parent. The graph may be empty (no edges), which is considered a valid tree. The function should return `true` if the graph is a valid tree, `false` otherwise. The input map may contain nodes with no outgoing edges (leaf nodes) or even nodes that only appear as destinations but not as keys in the map—you must account for all nodes reachable via edges as well as isolated nodes (if any node is unreachable from the root, it's not a tree). Edge cases include: a node with no outgoing edges but appears as a key, a node that appears only as a destination, and a graph with multiple roots or cycles.
#include <cassert>
#include <map>
#include <vector>

int main() {
    // Test 1: Empty graph
    std::map<int, std::vector<int>> m1;
    assert(isTree(m1) == true);

    // Test 2: Single node, no edges
    std::map<int, std::vector<int>> m2;
    m2[1] = {};
    assert(isTree(m2) == true);

    // Test 3: Simple tree: 1 -> 2, 1 -> 3
    std::map<int, std::vector<int>> m3;
    m3[1] = {2, 3};
    m3[2] = {};
    m3[3] = {};
    assert(isTree(m3) == true);

    // Test 4: Cycle: 1 -> 2, 2 -> 1
    std::map<int, std::vector<int>> m4;
    m4[1] = {2};
    m4[2] = {1};
    assert(isTree(m4) == false);

    // Test 5: Two roots (no incoming edges): 1 -> 3, 2 -> 3 (but 3 has two parents)
    std::map<int, std::vector<int>> m5;
    m5[1] = {3};
    m5[2] = {3};
    m5[3] = {};
    assert(isTree(m5) == false);

    // Test 6: Disconnected: 1 -> 2, and isolated node 3 (as key with no edges)
    std::map<int, std::vector<int>> m6;
    m6[1] = {2};
    m6[2] = {};
    m6[3] = {};
    assert(isTree(m6) == false);

    // Test 7: Chain: 1 -> 2, 2 -> 3, 3 -> 4
    std::map<int, std::vector<int>> m7;
    m7[1] = {2};
    m7[2] = {3};
    m7[3] = {4};
    m7[4] = {};
    assert(isTree(m7) == true);

    // Test 8: Self-loop: 1 -> 1
    std::map<int, std::vector<int>> m8;
    m8[1] = {1};
    assert(isTree(m8) == false);

    // Test 9: Leaf node appears only as destination, not as key
    std::map<int, std::vector<int>> m9;
    m9[1] = {2, 3};
    m9[2] = {3};
    // Node 3 appears only as destination, but has two parents (1 and 2) → not a tree
    assert(isTree(m9) == false);

    // Test 10: Correct tree with node only as destination (leaf)
    std::map<int, std::vector<int>> m10;
    m10[1] = {2};
    // Node 2 appears only as destination, but that's fine for a tree
    assert(isTree(m10) == true);

    return 0;
}
#include <map>
#include <vector>
#include <queue>
#include <set>

// Determines if a directed graph given as adjacency lists is a valid tree.
// A valid tree has exactly one root (no incoming edges), all other nodes have exactly one incoming edge,
// is connected, and has no cycles. Empty graph (no nodes) is a tree.
bool isTree(const std::map<int, std::vector<int>>& treeMap) {
    // Collect all distinct nodes that appear either as a source or as a destination.
    std::set<int> allNodes;
    for (const auto& pair : treeMap) {
        allNodes.insert(pair.first);
        for (int dest : pair.second) {
            allNodes.insert(dest);
        }
    }

    // Edge count must be exactly (number of nodes - 1) for a non-empty tree.
    int edgeCount = 0;
    for (const auto& pair : treeMap) {
        edgeCount += pair.second.size();
    }
    if (allNodes.empty()) return true; // empty graph is valid
    if (edgeCount != static_cast<int>(allNodes.size()) - 1) return false;

    // Compute in-degree for each node.
    std::map<int, int> inDegree;
    for (int node : allNodes) inDegree[node] = 0;
    for (const auto& pair : treeMap) {
        for (int dest : pair.second) {
            inDegree[dest]++;
            if (inDegree[dest] > 1) return false; // multiple parents
        }
    }

    // Find the root: exactly one node with in-degree 0.
    int root = -1;
    for (int node : allNodes) {
        if (inDegree[node] == 0) {
            if (root != -1) return false; // more than one root
            root = node;
        }
    }
    if (root == -1) return false; // cycle: no root

    // BFS from root, check for cycles and connectivity.
    std::map<int, bool> visited;
    for (int node : allNodes) visited[node] = false;
    visited[root] = true;
    std::queue<int> q;
    q.push(root);

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        auto it = treeMap.find(current);
        if (it != treeMap.end()) {
            for (int neighbor : it->second) {
                // If already visited, cycle exists.
                if (visited[neighbor]) return false;
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    // All nodes must be visited (connected).
    for (const auto& pair : visited) {
        if (!pair.second) return false;
    }
    return true;
}
// The main algorithm involves three steps: identify the root, validate edge count and in-degree constraints, then perform BFS from the root to check connectivity and absence of cycles. First, compute the in-degree of each node by iterating over all edges in the map. The root is the only node with in-degree 0 among all distinct nodes that appear either as a key or as a destination. If there is not exactly one such root, the graph is not a tree. Also, the total number of distinct nodes (set of all keys plus all destinations) must equal the total number of edges plus 1 (for a tree with n nodes, there are n-1 edges). This condition catches cycles and extra edges. Then perform BFS starting from the root: mark visited nodes, and if we encounter a node already visited (other than the root’s initial mark), a cycle exists. After BFS, ensure all distinct nodes were visited; if any node remains unvisited, the graph is disconnected and thus not a tree. Time complexity is O(V + E) where V is the number of distinct nodes and E is the number of edges, because we iterate through all edges multiple times but each is constant work. Space complexity is O(V) for the visited map and BFS queue.
