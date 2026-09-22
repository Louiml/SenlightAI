// Write a C++ function `int minimumEdgesToMakeStronglyConnected(const std::vector<std::vector<int>>& adj)` that takes a directed graph with `n` vertices (numbered `0` to `n-1`) represented as an adjacency list, and returns the minimum number of directed edges that must be added to make the entire graph strongly connected. The graph may be disconnected, may contain cycles, and may have parallel edges or self-loops (both should be ignored for the calculation). If the graph is already strongly connected, the function should return `0`. Additionally, if the graph has only one vertex, it is trivially strongly connected, so return `0`. The solution must compute strongly connected components (SCCs) using Tarjan's algorithm, then condense the graph into a DAG of components, count source components (in-degree 0) and sink components (out-degree 0) in that condensed graph, and return `max(sources, sinks)` when there are multiple components, else `0`.
#include <cassert>
#include <vector>

// Include the solution function here (or link).

int main() {
    // Test 1: Single vertex graph.
    {
        std::vector<std::vector<int>> adj(1);
        assert(minimumEdgesToMakeStronglyConnected(adj) == 0);
    }

    // Test 2: Graph already strongly connected (cycle).
    {
        std::vector<std::vector<int>> adj = {
            {1}, {2}, {0}
        };
        assert(minimumEdgesToMakeStronglyConnected(adj) == 0);
    }

    // Test 3: Two disconnected vertices, no edges.
    {
        std::vector<std::vector<int>> adj(2);
        assert(minimumEdgesToMakeStronglyConnected(adj) == 2); // Need two edges.
    }

    // Test 4: Single edge 0->1.
    {
        std::vector<std::vector<int>> adj = {
            {1}, {}
        };
        assert(minimumEdgesToMakeStronglyConnected(adj) == 2); // One edge 1->0.
    }

    // Test 5: Two components in a line: 0->1, 1->2.
    {
        std::vector<std::vector<int>> adj = {
            {1}, {2}, {}
        };
        assert(minimumEdgesToMakeStronglyConnected(adj) == 2); // Need 2->0 to form cycle.
    }

    // Test 6: Three components: source, middle, sink.
    {
        std::vector<std::vector<int>> adj = {
            {1}, {2}, {}
        };
        // Components: {0}, {1}, {2}; sources=1, sinks=1 => max=1 but actually need 2 edges? Wait, with 1 edge 2->0, you get 0->1->2->0, so yes 1.
        assert(minimumEdgesToMakeStronglyConnected(adj) == 1);
    }

    // Test 7: Graph with self-loops and parallel edges.
    {
        std::vector<std::vector<int>> adj = {
            {0, 0, 1}, {1, 0}, {}
        };
        // Edges: 0 self-loop, 0->1, 1->0, 1 self-loop. So 0 and 1 are same SCC? Actually 0<->1, so one SCC {0,1}, and vertex 2 isolated.
        // Two SCCs: {0,1} and {2}; sources=1, sinks=1 => 1 edge needed.
        assert(minimumEdgesToMakeStronglyConnected(adj) == 1);
    }

    // Test 8: Graph with multiple sources and sinks.
    {
        std::vector<std::vector<int>> adj = {
            {1}, // 0 -> 1
            {2}, // 1 -> 2
            {},  // 2 is sink
            {3}, // 3 self-loop? Actually 3->3, so it's a single SCC with no in/out to others. Let's make: 
            // Actually let's define: 0,1,2 as a chain; 3 as isolated. That's 2 components? Wait component for 3 is isolated, sources=2 (0 and 3), sinks=2 (2 and 3) => max=2.
        };
        // Let's construct a clean example: vertices 0->1, 1->2; and vertex 3 isolated.
        std::vector<std::vector<int>> adj2 = {
            {1}, {2}, {}, {}
        };
        assert(minimumEdgesToMakeStronglyConnected(adj2) == 2);
    }

    // Test 9: Graph with two SCCs but one has in/out.
    {
        std::vector<std::vector<int>> adj = {
            {1}, {0}, {0}
        };
        // SCC1 = {0,1} (cycle), SCC2 = {2} with edge 2->0. So sources: SCC2 has in-degree 0? Actually no incoming, so source; SCC1 has incoming from 2, so not source. Sinks: SCC1 has no outgoing to others? Actually SCC1 has edge to? no, so sink; SCC2 has outgoing to SCC1, so not sink. So sources=1, sinks=1 => need 1 edge.
        assert(minimumEdgesToMakeStronglyConnected(adj) == 1);
    }

    // Test 10: Large graph (performance sanity).
    {
        int n = 100000;
        std::vector<std::vector<int>> adj(n);
        for (int i = 0; i < n - 1; ++i) {
            adj[i].push_back(i + 1);
        }
        // Chain: 0->1->2->...->n-1. Need max(1 source, 1 sink) = 1 but must add edge n-1->0 to make cycle, so 1.
        assert(minimumEdgesToMakeStronglyConnected(adj) == 1);
    }

    return 0;
}
#include <vector>
#include <algorithm>
#include <functional>

// Returns the minimum number of edges to add to make the directed graph strongly connected.
int minimumEdgesToMakeStronglyConnected(const std::vector<std::vector<int>>& adj) {
    const int n = static_cast<int>(adj.size());
    if (n <= 1) {
        return 0; // Trivially strongly connected.
    }

    // Tarjan's SCC algorithm.
    std::vector<int> dfn(n, -1), low(n, -1), comp(n, -1);
    std::vector<int> stk;
    int timestamp = 0, scc_count = 0;

    std::function<void(int)> tarjan = [&](int u) {
        dfn[u] = low[u] = timestamp++;
        stk.push_back(u);
        for (int v : adj[u]) {
            if (dfn[v] == -1) {
                tarjan(v);
                low[u] = std::min(low[u], low[v]);
            } else if (comp[v] == -1) {
                // Back edge to a node in the current SCC stack.
                low[u] = std::min(low[u], dfn[v]);
            }
        }
        if (low[u] == dfn[u]) {
            while (true) {
                int v = stk.back();
                stk.pop_back();
                comp[v] = scc_count;
                if (v == u) break;
            }
            scc_count++;
        }
    };

    for (int i = 0; i < n; ++i) {
        if (dfn[i] == -1) {
            tarjan(i);
        }
    }

    if (scc_count == 1) {
        return 0; // Already strongly connected.
    }

    // Compute in-degree and out-degree in the condensed DAG.
    std::vector<int> indeg(scc_count, 0), outdeg(scc_count, 0);
    for (int u = 0; u < n; ++u) {
        for (int v : adj[u]) {
            int cu = comp[u];
            int cv = comp[v];
            if (cu != cv) {
                outdeg[cu]++;
                indeg[cv]++;
            }
        }
    }

    int sources = 0, sinks = 0;
    for (int i = 0; i < scc_count; ++i) {
        if (indeg[i] == 0) sources++;
        if (outdeg[i] == 0) sinks++;
    }

    return std::max(sources, sinks);
}
// The core idea is to decompose the directed graph into strongly connected components (SCCs) using Tarjan's algorithm. After condensation, each SCC becomes a node in a DAG. To make the entire graph strongly connected, every source component (with no incoming edges from other components) must receive at least one new incoming edge, and every sink component (with no outgoing edges to other components) must receive at least one new outgoing edge. The minimum number of edges to add is the maximum of the number of sources and the number of sinks, because each added edge can simultaneously fix one source and one sink (by connecting a sink to a source) until one category is exhausted. Edge cases: if there is only one SCC, the graph is already strongly connected, so return 0. Self-loops and parallel edges do not affect the counts since they stay within the same component. The algorithm runs Tarjan in O(V + E) time using iterative or recursive DFS; the adjacency list representation is used. Space complexity is O(V + E) for the stack, arrays, and adjacency list. The solution must handle graphs with up to 10^5 vertices and 10^5 edges efficiently.
