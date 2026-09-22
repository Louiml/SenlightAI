// Write a C++ function `vector<int> safeNodes(const vector<vector<int>>& graph)` that takes a directed graph represented as an adjacency list, where `graph[i]` is a vector of nodes that node `i` has outgoing edges to. A node is considered "safe" if every path starting from that node leads to a terminal node (a node with no outgoing edges), meaning no path from it can reach a cycle. Return a sorted list of all safe node indices in increasing order.

#include <cassert>
#include <vector>

int main() {
    // Example 1: 0 -> 1, 1 -> 2, 2 -> 3, 3 is terminal. All nodes safe.
    std::vector<std::vector<int>> g1 = {{1}, {2}, {3}, {}};
    assert(safeNodes(g1) == std::vector<int>({0, 1, 2, 3}));

    // Example 2: 0 -> 1, 1 -> 0 (cycle), 2 -> 3, 3 terminal. Nodes 2 and 3 safe.
    std::vector<std::vector<int>> g2 = {{1}, {0}, {3}, {}};
    assert(safeNodes(g2) == std::vector<int>({2, 3}));

    // Example 3: all nodes in a single cycle, no terminal nodes.
    std::vector<std::vector<int>> g3 = {{1}, {2}, {0}};
    assert(safeNodes(g3).empty());

    // Example 4: node with self-loop is not safe.
    std::vector<std::vector<int>> g4 = {{0}, {}};
    assert(safeNodes(g4) == std::vector<int>({1}));

    // Example 5: empty graph.
    std::vector<std::vector<int>> g5;
    assert(safeNodes(g5).empty());

    // Example 6: single terminal node.
    std::vector<std::vector<int>> g6 = {{}};
    assert(safeNodes(g6) == std::vector<int>({0}));

    // Example 7: graph with multiple disconnected components, mixed.
    std::vector<std::vector<int>> g7 = {{1}, {0}, {3}, {}, {5}, {}};
    assert(safeNodes(g7) == std::vector<int>({2, 3, 4, 5}));

    // Example 8: more complex chain with a cycle elsewhere.
    std::vector<std::vector<int>> g8 = {{1}, {2}, {3}, {4}, {2}, {}};
    // 0,1,2,3,4 in a cycle (0->1->2->3->4->2), 5 terminal and safe.
    assert(safeNodes(g8) == std::vector<int>({5}));

    return 0;
}

#include <vector>
#include <queue>
#include <algorithm>

// Return the sorted list of safe nodes in a directed graph.
// A node is safe if every path from it ends at a terminal node (no outgoing edges).
std::vector<int> safeNodes(const std::vector<std::vector<int>>& graph) {
    int V = static_cast<int>(graph.size());
    std::vector<int> outDegree(V, 0);
    std::vector<std::vector<int>> reverseAdj(V);

    // Build reversed graph and compute out-degrees.
    for (int u = 0; u < V; ++u) {
        outDegree[u] = static_cast<int>(graph[u].size());
        for (int v : graph[u]) {
            reverseAdj[v].push_back(u);
        }
    }

    std::queue<int> q;
    // Terminal nodes (out-degree 0) are safe initially.
    for (int i = 0; i < V; ++i) {
        if (outDegree[i] == 0) {
            q.push(i);
        }
    }

    std::vector<int> safeNodesList;
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        safeNodesList.push_back(node);

        // For each predecessor, reduce its out-degree.
        for (int pred : reverseAdj[node]) {
            --outDegree[pred];
            if (outDegree[pred] == 0) {
                q.push(pred);
            }
        }
    }

    std::sort(safeNodesList.begin(), safeNodesList.end());
    return safeNodesList;
}

// The problem is solved using a topological sort on the reversed graph. First, compute the out-degree of each node from the original graph by counting the number of outgoing edges. Build a reversed adjacency list where for each original edge `u -> v`, we add an edge `v -> u` (meaning `v` is a predecessor of `u`). Nodes with out-degree 0 are terminal and are initially safe; push them into a queue. Then process these nodes in BFS order: when a node is popped, it is marked as safe; for each predecessor (in original direction, i.e., each node that points to this terminal node), decrement its out-degree. If a predecessor’s out-degree becomes 0, all its outgoing edges lead to already-known safe nodes, so it also becomes safe and is pushed into the queue. Continue until the queue is empty. The collected safe nodes are then sorted and returned. Edge cases include an empty graph (returns empty), graphs with no terminal nodes (returns empty), and self-loops (such nodes are never safe because they are part of a cycle). Complexity: building reversed graph and counting degrees takes O(V + E) time, BFS processing each node and edge once is O(V + E), sorting the result is O(V log V), so overall O(V + E + V log V) time, and O(V + E) space for reversed adjacency and degree arrays.
