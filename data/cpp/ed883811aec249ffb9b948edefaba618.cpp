Write a C++ function `std::vector<int> topologicalOrder(const std::vector<std::vector<int>>& graph)` that takes an adjacency list representation of a directed graph (nodes numbered 0..n-1, where `graph[i]` contains the list of nodes that `i` points to) and returns a valid topological ordering of the nodes if the graph is a DAG; if the graph contains a cycle, return an empty vector. The function must not modify the input graph and must handle disconnected graphs. For example, given `graph = {{1,2},{2,3},{3},{}}`, a valid output is `{0,1,2,3}` or `{0,1,3,2}` (both accepted). If a cycle exists (e.g., `{{1},{0}}`), return `{}`. Ensure the solution uses a DFS with explicit node colors (0=unvisited, 1=in-process, 2=done) to detect cycles, and returns the order by reversing the DFS finish time list.

// The core algorithm is a DFS-based topological sort. We maintain three states per node: 0 (unvisited), 1 (visiting/in current DFS stack), 2 (fully processed). For each unvisited node, we call a recursive DFS that explores all outgoing edges. When we encounter a neighbor with state 1, it indicates a back edge, meaning a cycle exists; we immediately abort and return an empty result. After all neighbors of a node are processed, we mark it as state 2 and append it to a `finish_order` vector. Once all nodes are visited, reversing `finish_order` yields a valid topological order (because a node is appended only after all its successors). Edge cases include: empty graph (returns empty), single node (returns that node), disconnected components (DFS from each unvisited node), and graphs with self-loops or cycles (detected via state 1). Time complexity is O(V + E) since each node and edge is explored once. Space complexity is O(V) for the colors, recursion stack, and order vector, plus the graph itself (not counted as auxiliary).

#include <vector>
#include <algorithm>

// Return a topological ordering of nodes in a DAG; empty vector if a cycle exists.
std::vector<int> topologicalOrder(const std::vector<std::vector<int>>& graph) {
    int n = static_cast<int>(graph.size());
    std::vector<int> color(n, 0); // 0=white, 1=gray, 2=black
    std::vector<int> finish_order;
    bool has_cycle = false;

    // Recursive DFS lambda
    std::function<void(int)> dfs = [&](int node) {
        if (has_cycle) return;
        color[node] = 1;
        for (int neighbor : graph[node]) {
            if (color[neighbor] == 1) {
                has_cycle = true;
                return;
            }
            if (color[neighbor] == 0) {
                dfs(neighbor);
            }
        }
        color[node] = 2;
        finish_order.push_back(node);
    };

    for (int i = 0; i < n; ++i) {
        if (color[i] == 0) {
            dfs(i);
            if (has_cycle) return {};
        }
    }

    std::reverse(finish_order.begin(), finish_order.end());
    return finish_order;
}

#include <cassert>
#include <vector>
#include <iostream>

// Assume the solution function is declared above.

int main() {
    // Disconnected DAG: 0->1, 2->3
    std::vector<std::vector<int>> g1 = {{1}, {}, {3}, {}};
    auto r1 = topologicalOrder(g1);
    assert((r1 == std::vector<int>{0,2,1,3}) || (r1 == std::vector<int>{2,3,0,1}));

    // Simple chain 0->1->2->3
    std::vector<std::vector<int>> g2 = {{1}, {2}, {3}, {}};
    assert(topologicalOrder(g2) == std::vector<int>({0,1,2,3}));

    // Single node
    std::vector<std::vector<int>> g3 = {{}};
    assert(topologicalOrder(g3) == std::vector<int>({0}));

    // DAG with multiple edges
    std::vector<std::vector<int>> g4 = {{1,2}, {3}, {3}, {}};
    auto r4 = topologicalOrder(g4);
    assert((r4 == std::vector<int>{0,1,2,3}) || (r4 == std::vector<int>{0,2,1,3})); // both valid

    // Cycle: 0->1, 1->0
    std::vector<std::vector<int>> g5 = {{1}, {0}};
    assert(topologicalOrder(g5).empty());

    // Self-loop
    std::vector<std::vector<int>> g6 = {{0}};
    assert(topologicalOrder(g6).empty());

    // Larger DAG
    std::vector<std::vector<int>> g7 = {{1,2}, {3}, {3}, {}, {}};
    auto r7 = topologicalOrder(g7);
    // Node 0 must appear before 1,2; nodes 1,2 before 3.
    assert(std::find(r7.begin(), r7.end(), 0) < std::find(r7.begin(), r7.end(), 1));
    assert(std::find(r7.begin(), r7.end(), 0) < std::find(r7.begin(), r7.end(), 2));
    assert(std::find(r7.begin(), r7.end(), 1) < std::find(r7.begin(), r7.end(), 3));
    assert(std::find(r7.begin(), r7.end(), 2) < std::find(r7.begin(), r7.end(), 3));

    // Empty graph
    std::vector<std::vector<int>> g8 = {};
    assert(topologicalOrder(g8).empty());

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
