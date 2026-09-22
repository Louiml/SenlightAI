// Given a directed graph represented as an adjacency list where each node may have zero or more outgoing edges, write a C++ function `bool hasSelfLoopEdge(int node, const std::vector<std::vector<int>>& adj)` that returns `true` if and only if there exists a directed edge from a given node to itself (i.e., a self-loop). The graph is 0-indexed, and the adjacency list may contain duplicate entries (multiple parallel edges) and may be unsorted. The function should safely handle an empty adjacency list or a node index out of range by returning `false`. The solution must not modify the input graph and must use appropriate `const` correctness.

// The solution approach is straightforward: check whether the given node index is valid (less than the size of the adjacency list). If invalid, return `false` immediately. Otherwise, iterate through all entries in `adj[node]`. If any entry equals `node`, a self-loop exists, so return `true`. If no match is found, return `false`. This is a simple linear scan over the outgoing edges of the node. We do not need to consider the entire graph, only the adjacency list of the specific node. The main edge cases are: (1) an empty adjacency list (node index out of range), (2) a node with no outgoing edges (returns `false`), (3) duplicate self-loops (still returns `true`), and (4) the node index being exactly the last valid index. Time complexity is \(O(d)\) where \(d\) is the out-degree of the given node, which is at most the total number of edges in the graph. Auxiliary space complexity is \(O(1)\) because we only use a loop variable and no additional data structures.

#include <vector>

// Returns true if there is a direct edge from 'node' to itself.
bool hasSelfLoopEdge(int node, const std::vector<std::vector<int>>& adj) {
    // If node is out of range, treat as no self-loop.
    if (node < 0 || node >= static_cast<int>(adj.size()))
        return false;

    // Scan all outgoing edges from 'node'.
    for (int neighbor : adj[node]) {
        if (neighbor == node)
            return true;
    }
    return false;
}

#include <cassert>
#include <vector>

// (The solution function is assumed to be defined above.)

int main() {
    // Basic graph with a self-loop.
    std::vector<std::vector<int>> g1 = {{0, 1}, {0}, {}};
    assert(hasSelfLoopEdge(0, g1) == true);
    assert(hasSelfLoopEdge(1, g1) == false);
    assert(hasSelfLoopEdge(2, g1) == false);

    // Parallel self-loops.
    std::vector<std::vector<int>> g2 = {{2, 2, 2}, {}, {0, 1}};
    assert(hasSelfLoopEdge(0, g2) == false);
    assert(hasSelfLoopEdge(2, g2) == false);
    // Add a self-loop to node 1.
    g2[1].push_back(1);
    assert(hasSelfLoopEdge(1, g2) == true);

    // Node with no outgoing edges.
    std::vector<std::vector<int>> g3 = { {}, {} };
    assert(hasSelfLoopEdge(0, g3) == false);
    assert(hasSelfLoopEdge(1, g3) == false);

    // Out-of-range node.
    assert(hasSelfLoopEdge(-1, g3) == false);
    assert(hasSelfLoopEdge(2, g3) == false);

    // Empty graph.
    std::vector<std::vector<int>> g4;
    assert(hasSelfLoopEdge(0, g4) == false);

    // Graph with a single node that has a self-loop.
    std::vector<std::vector<int>> g5 = {{0}};
    assert(hasSelfLoopEdge(0, g5) == true);

    // Unsorted adjacency list with normal edges and a self-loop.
    std::vector<std::vector<int>> g6 = {{3, 0, 2}, {}, {0, 1}, {3}};
    assert(hasSelfLoopEdge(0, g6) == true);
    assert(hasSelfLoopEdge(1, g6) == false);
    assert(hasSelfLoopEdge(2, g6) == false);
    assert(hasSelfLoopEdge(3, g6) == true);

    return 0;
}
