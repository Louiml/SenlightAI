/*
Write a C++ function that takes a directed graph with `n` vertices (numbered 1..n) and `e` directed edges, and returns the minimum number of new directed edges that must be added to make the graph strongly connected. A graph is strongly connected if there is a path from any vertex to every other vertex. The function signature is `int minEdgesToStronglyConnect(int n, const std::vector<std::pair<int,int>>& edges)`, where each pair represents a directed edge `(from, to)` with 1-based vertex numbers. The graph may contain self-loops, parallel edges, and may not be initially connected. The return value must be the smallest integer `k` such that adding exactly `k` directed edges (anywhere, between any vertices) makes the whole graph strongly connected. If the graph is already strongly connected, return 0.
*/

#include <vector>
#include <stack>
#include <unordered_set>
#include <algorithm>

// Return minimum number of directed edges to add to make the graph strongly connected.
int minEdgesToStronglyConnect(int n, const std::vector<std::pair<int,int>>& edges) {
    // Build adjacency and reverse adjacency lists (0-based internally)
    std::vector<std::vector<int>> adj(n), rev(n);
    for (const auto& e : edges) {
        int u = e.first - 1; // convert to 0-based
        int v = e.second - 1;
        adj[u].push_back(v);
        rev[v].push_back(u);
    }

    // Kosaraju's algorithm: first pass to get finish order
    std::vector<bool> visited(n, false);
    std::stack<int> order;
    
    // Lambda for first DFS
    std::function<void(int)> dfs1 = [&](int u) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) dfs1(v);
        }
        order.push(u);
    };
    
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) dfs1(i);
    }

    // Second pass on reversed graph
    std::vector<int> comp(n, -1);
    int compCount = 0;
    
    // Lambda for second DFS
    std::function<void(int)> dfs2 = [&](int u) {
        comp[u] = compCount;
        for (int v : rev[u]) {
            if (comp[v] == -1) dfs2(v);
        }
    };
    
    while (!order.empty()) {
        int u = order.top();
        order.pop();
        if (comp[u] == -1) {
            dfs2(u);
            compCount++;
        }
    }

    // If the whole graph is already one SCC
    if (compCount == 1) return 0;

    // Build condensation DAG: only edges between different components
    std::vector<std::unordered_set<int>> dag(compCount);
    for (int u = 0; u < n; ++u) {
        for (int v : adj[u]) {
            if (comp[u] != comp[v]) {
                dag[comp[u]].insert(comp[v]);
            }
        }
    }

    // Compute in-degree and out-degree of each component in the DAG
    std::vector<int> indeg(compCount, 0), outdeg(compCount, 0);
    for (int c = 0; c < compCount; ++c) {
        outdeg[c] = static_cast<int>(dag[c].size());
        for (int d : dag[c]) {
            indeg[d]++;
        }
    }

    // Count sources (indeg 0) and sinks (outdeg 0)
    int sources = 0, sinks = 0;
    for (int c = 0; c < compCount; ++c) {
        if (indeg[c] == 0) sources++;
        if (outdeg[c] == 0) sinks++;
    }

    // Minimum edges to add is max(sources, sinks)
    return std::max(sources, sinks);
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above (external linkage)
// Since we cannot include the solution here, we assume it is defined.
// For the test, we replicate the include and function definition.

// To make test standalone, we copy the solution here (without main)
#include <vector>
#include <stack>
#include <unordered_set>
#include <algorithm>
#include <functional>

// (Solution function copied here for the test to compile)
int minEdgesToStronglyConnect(int n, const std::vector<std::pair<int,int>>& edges) {
    std::vector<std::vector<int>> adj(n), rev(n);
    for (const auto& e : edges) {
        int u = e.first - 1, v = e.second - 1;
        adj[u].push_back(v);
        rev[v].push_back(u);
    }
    std::vector<bool> visited(n, false);
    std::stack<int> order;
    std::function<void(int)> dfs1 = [&](int u) {
        visited[u] = true;
        for (int v : adj[u]) if (!visited[v]) dfs1(v);
        order.push(u);
    };
    for (int i = 0; i < n; ++i) if (!visited[i]) dfs1(i);
    std::vector<int> comp(n, -1);
    int compCount = 0;
    std::function<void(int)> dfs2 = [&](int u) {
        comp[u] = compCount;
        for (int v : rev[u]) if (comp[v] == -1) dfs2(v);
    };
    while (!order.empty()) {
        int u = order.top(); order.pop();
        if (comp[u] == -1) { dfs2(u); compCount++; }
    }
    if (compCount == 1) return 0;
    std::vector<std::unordered_set<int>> dag(compCount);
    for (int u = 0; u < n; ++u)
        for (int v : adj[u])
            if (comp[u] != comp[v]) dag[comp[u]].insert(comp[v]);
    std::vector<int> indeg(compCount, 0), outdeg(compCount, 0);
    for (int c = 0; c < compCount; ++c) {
        outdeg[c] = (int)dag[c].size();
        for (int d : dag[c]) indeg[d]++;
    }
    int sources = 0, sinks = 0;
    for (int c = 0; c < compCount; ++c) {
        if (indeg[c] == 0) sources++;
        if (outdeg[c] == 0) sinks++;
    }
    return std::max(sources, sinks);
}

int main() {
    // Test 1: Already strongly connected (cycle)
    assert(minEdgesToStronglyConnect(3, {{1,2},{2,3},{3,1}}) == 0);
    // Test 2: Single vertex, no edges
    assert(minEdgesToStronglyConnect(1, {}) == 0);
    // Test 3: Two vertices, one edge from 1 to 2 (need one more edge back)
    assert(minEdgesToStronglyConnect(2, {{1,2}}) == 1);
    // Test 4: Three vertices in a line: 1->2->3 (sources=1, sinks=1, need 1)
    assert(minEdgesToStronglyConnect(3, {{1,2},{2,3}}) == 1);
    // Test 5: Two disconnected vertices (both sources and sinks) => max(2,2)=2
    assert(minEdgesToStronglyConnect(2, {}) == 2);
    // Test 6: Three vertices, two disjoint edges: 1->2 and 3 alone (sources: {1,3}=2, sinks: {2,3}=2 => max=2)
    assert(minEdgesToStronglyConnect(3, {{1,2}}) == 2);
    // Test 7: Complex DAG: 1->2, 2->3, 4->3 (sources: {1,4}=2, sinks: {3}=1 => max=2)
    assert(minEdgesToStronglyConnect(4, {{1,2},{2,3},{4,3}}) == 2);
    // Test 8: Self-loops and parallel edges don't affect SCC
    assert(minEdgesToStronglyConnect(2, {{1,1},{1,2},{2,2},{2,1}}) == 0);
    // Test 9: Two SCCs where one has out edge only: 1->2, 2->1, 2->3 (component {1,2} -> {3}) => sources=1, sinks=1 => 1
    assert(minEdgesToStronglyConnect(3, {{1,2},{2,1},{2,3}}) == 1);
    // Test 10: Three separate components each with self loop ( not needed but test)
    assert(minEdgesToStronglyConnect(3, {}) == 3);

    return 0;
}

// The standard approach is to first find the strongly connected components (SCCs) using Kosaraju's algorithm (or Tarjan's). After contracting each SCC into a single node, we obtain a directed acyclic graph (DAG) of components. To make the entire original graph strongly connected, we need to add edges so that the resulting DAG (after adding edges) becomes strongly connected. A known result: the minimum number of edges to add to a DAG to make it strongly connected is `max( number_of_source_components, number_of_sink_components )`, where a source component has in-degree 0 and a sink component has out-degree 0. If the graph has only one SCC (counter == 1), then it is already strongly connected, and the answer is 0. Edge cases: when there are multiple SCCs but some have both in-degree 0 and out-degree 0 (isolated components), they count as both sources and sinks, so the formula still works. Self-loops and parallel edges do not change SCC membership, but they may affect degree counts; however, since we only consider edges between different components when building the condensation DAG, self-loops are ignored. The algorithm runs in O(n + e) time for SCC detection and O(n + e) for building the condensation graph (using a set to avoid duplicate edges between components), and uses O(n + e) auxiliary space.
