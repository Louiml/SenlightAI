// Write a standalone C++ function that, given a vector of integers representing CFG node indices and an adjacency list of directed edges between those nodes, computes and returns a vector of the "loop hoist candidates": nodes that are not part of any cycle but have at least one incoming edge from a node inside a cycle. The function must detect cycles using a depth-first search and, for each node not in a cycle, check whether any of its predecessors belong to a cycle. The input is guaranteed to have at least one node, edge indices are valid, and the graph can be disconnected. Return the candidate nodes sorted in ascending order. Do not rely on any external graph libraries; implement the cycle detection and candidate collection manually.

#include <cassert>
#include <vector>
#include <algorithm>

// Solution function declaration (stub to avoid duplication; in real code include the above implementation).
std::vector<int> findLoopHoistCandidates(int n, const std::vector<std::vector<int>>& adjacency);

int main() {
    // Test 1: Simple cycle 0->1->2->0, and node 3 has incoming from 2 (cycle node).
    {
        std::vector<std::vector<int>> adj(4);
        adj[0] = {1};
        adj[1] = {2};
        adj[2] = {0, 3};
        adj[3] = {};
        std::vector<int> result = findLoopHoistCandidates(4, adj);
        assert(result == std::vector<int>{3});
    }

    // Test 2: Self-loop at node 0, node 1 has incoming from 0.
    {
        std::vector<std::vector<int>> adj(2);
        adj[0] = {0, 1};
        adj[1] = {};
        std::vector<int> result = findLoopHoistCandidates(2, adj);
        assert(result == std::vector<int>{1});
    }

    // Test 3: Two disjoint cycles and a node from second cycle points to a non-cycle node, but first cycle has no outgoing.
    {
        std::vector<std::vector<int>> adj(5);
        adj[0] = {1};
        adj[1] = {0};      // cycle: 0-1
        adj[2] = {3};
        adj[3] = {2, 4};   // cycle: 2-3, and edge to 4
        adj[4] = {};
        std::vector<int> result = findLoopHoistCandidates(5, adj);
        assert(result == std::vector<int>{4});
    }

    // Test 4: No cycles at all, no candidates.
    {
        std::vector<std::vector<int>> adj(3);
        adj[0] = {1};
        adj[1] = {2};
        adj[2] = {};
        std::vector<int> result = findLoopHoistCandidates(3, adj);
        assert(result.empty());
    }

    // Test 5: Non-cycle node with predecessor in a cycle, but also in another cycle itself? No, that's contradictory.
    // Ensure loop node with cycle predecessor is excluded.
    {
        std::vector<std::vector<int>> adj(3);
        adj[0] = {1};
        adj[1] = {0, 2}; // cycle 0-1, 1->2
        adj[2] = {1};    // 2->1 creates another cycle? Actually 2->1->2 makes 1 and 2 a cycle as well, but 0 is also in 0-1 cycle.
        // Nodes 0,1,2 all in cycles. So no candidates.
        std::vector<int> result = findLoopHoistCandidates(3, adj);
        assert(result.empty());
    }

    // Test 6: Empty adjacency for all nodes (no edges). Expect empty.
    {
        std::vector<std::vector<int>> adj(2);
        std::vector<int> result = findLoopHoistCandidates(2, adj);
        assert(result.empty());
    }

    return 0;
}

#include <vector>
#include <algorithm>
#include <functional>

// Given a graph with 'n' nodes (indices 0..n-1) and an adjacency list,
// return all nodes that are NOT in any cycle but have at least one
// incoming edge from a node that IS in a cycle. Result is sorted ascending.
std::vector<int> findLoopHoistCandidates(int n, const std::vector<std::vector<int>>& adjacency) {
    // Build reverse adjacency (predecessors) for quick incoming-edge lookup.
    std::vector<std::vector<int>> predecessors(n);
    for (int u = 0; u < n; ++u) {
        for (int v : adjacency[u]) {
            predecessors[v].push_back(u);
        }
    }

    // Colors: 0 = unvisited, 1 = in recursion stack, 2 = fully processed.
    std::vector<int> color(n, 0);
    std::vector<bool> inCycle(n, false);

    // Recursive DFS to detect cycles and mark nodes that are part of any cycle.
    std::function<bool(int)> dfs = [&](int u) -> bool {
        color[u] = 1; // mark as in stack
        bool foundCycle = false;
        for (int v : adjacency[u]) {
            if (color[v] == 1) {
                // Back edge: cycle detected, v and u are in cycle (at least v is).
                // We'll mark v and u after recursion; marking here ensures
                // correctness even for self-loops.
                inCycle[v] = true;
                inCycle[u] = true;
                foundCycle = true;
            } else if (color[v] == 0) {
                if (dfs(v)) {
                    // If v is in a cycle, u is also in that cycle path.
                    inCycle[u] = true;
                    foundCycle = true;
                }
            }
            // If color[v] == 2, v is done; no action.
        }
        color[u] = 2; // mark as fully processed
        return foundCycle || inCycle[u];
    };

    for (int i = 0; i < n; ++i) {
        if (color[i] == 0) {
            dfs(i);
        }
    }

    // Collect all non-cycle nodes that have at least one cycle predecessor.
    std::vector<int> candidates;
    for (int u = 0; u < n; ++u) {
        if (inCycle[u]) continue; // skip cycle nodes
        bool hasCyclePred = false;
        for (int p : predecessors[u]) {
            if (inCycle[p]) {
                hasCyclePred = true;
                break;
            }
        }
        if (hasCyclePred) {
            candidates.push_back(u);
        }
    }

    std::sort(candidates.begin(), candidates.end());
    return candidates;
}

// The solution uses a standard DFS-based cycle detection approach. We maintain three colors per node: 0 (unvisited), 1 (in current recursion stack), and 2 (fully processed). A node is part of a cycle if, during DFS, we encounter an edge from the current node to a node that is still in the recursion stack (color 1). We mark such nodes as "inCycle". After completing DFS on all nodes, we have a set of cycle nodes. Then, for every node not marked as inCycle, we examine all its incoming edges (predecessors). If any predecessor is inCycle, we add that node to the result. Finally, we sort the result and return it. Time complexity is O(V+E) for DFS plus O(V+E) for the predecessor scan, so overall O(V+E). Space complexity is O(V) for the color array, recursion stack, and result.
//
// Edge cases: 
// - A node that is a self-loop (edge from a node to itself) is trivially in a cycle.
// - If a non-cycle node has multiple cycle predecessors, it is added once.
// - If a cycle node also has a predecessor from another cycle, it is not added because we only consider non-cycle nodes.
// - Nodes with no incoming edges from cycle nodes are excluded.
// - The empty graph case is not possible per constraints (at least one node), but the function handles it defensively by returning an empty vector.
