Write a C++ function `countFoodChains` that takes two integers `n` (number of species, nodes) and `m` (number of predator-prey relationships, edges) followed by `m` pairs of integers `(a, b)` meaning species `a` is eaten by species `b` (i.e., `b` is a predator of `a`). The function should return the total number of distinct food chains (directed paths that start at a species with no predators, i.e., a "bottom" node, and end at a species with no prey, i.e., a "top" node) modulo `80112002`. A chain of length zero (a single species that is both top and bottom, i.e., isolated node) counts as 1. The graph is a directed acyclic graph (DAG) guaranteed to have no cycles. Multiply edges or multiple paths to the same node from different predecessors count as separate chains. The function must handle up to `n = 5000` and `m` up to large values, using memoization to avoid exponential time. Return the result as an `int`.

// This is essentially counting the number of paths in a DAG from source nodes (nodes with in-degree 0) to sink nodes (nodes with out-degree 0), summing over all sources. Since the graph is a DAG, we can use dynamic programming with memoization. Define `dp[i]` = number of distinct paths starting from node `i` and ending at any sink (including the trivial path of length 0 if `i` is a sink). For a leaf (no outgoing edges), `dp[i] = 1` (the chain consisting of just that node). For a non-leaf, `dp[i] = sum(dp[child])` over all outgoing edges `i -> child`, modulo `80112002`. The answer is the sum of `dp[i]` for all nodes with in-degree 0 (no predators). Use an adjacency list (from predator to prey) to traverse forward. The recursion depth could be up to `n` (10000), but typical stack can handle up to ~20000, so it's fine. Alternatively, use iterative topological order. Edge cases: isolated nodes (in-degree 0, out-degree 0) contribute 1 each. Multiple edges between same pair could appear but using a set ensures uniqueness; but the problem statement likely gives distinct edges, but we can handle duplicates gracefully. Time complexity: O(n + m) due to memoization each node processed once and each edge traversed once. Space: O(n + m) for adjacency and memo array.

#include <vector>
#include <functional>

const int MOD = 80112002;

// Counts the total number of distinct food chains (paths) in a DAG,
// starting from any node with no incoming edges (no predators) and
// ending at any node with no outgoing edges (no prey). Returns the count mod MOD.
int countFoodChains(int n, int m, const std::vector<std::pair<int,int>>& edges) {
    // Build adjacency list: node -> its prey (outgoing edges to children)
    std::vector<std::vector<int>> out(n);
    std::vector<bool> has_predator(n, false);
    for (const auto& e : edges) {
        int a = e.first; // prey
        int b = e.second; // predator
        // b eats a, so b's chain goes to a
        out[b].push_back(a);
        has_predator[a] = true;
    }

    // Memoization array, -1 means not computed yet
    std::vector<int> memo(n, -1);

    // Recursive DFS with memoization
    std::function<int(int)> dfs = [&](int node) -> int {
        if (memo[node] != -1) return memo[node];
        int total = 0;
        if (out[node].empty()) {
            // leaf node: chain of just itself
            total = 1;
        } else {
            for (int child : out[node]) {
                total = (total + dfs(child)) % MOD;
            }
        }
        memo[node] = total;
        return total;
    };

    int result = 0;
    // Sum for all nodes that have no predators (in-degree 0)
    for (int i = 0; i < n; ++i) {
        if (!has_predator[i]) {
            result = (result + dfs(i)) % MOD;
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be declared above.
// For this test, we include it inline for completeness.
#include <vector>
#include <functional>
const int MOD = 80112002;
int countFoodChains(int n, int m, const std::vector<std::pair<int,int>>& edges) {
    std::vector<std::vector<int>> out(n);
    std::vector<bool> has_predator(n, false);
    for (const auto& e : edges) {
        int a = e.first; int b = e.second;
        out[b].push_back(a);
        has_predator[a] = true;
    }
    std::vector<int> memo(n, -1);
    std::function<int(int)> dfs = [&](int node) -> int {
        if (memo[node] != -1) return memo[node];
        int total = 0;
        if (out[node].empty()) total = 1;
        else for (int child : out[node]) total = (total + dfs(child)) % MOD;
        memo[node] = total;
        return total;
    };
    int result = 0;
    for (int i = 0; i < n; ++i) if (!has_predator[i]) result = (result + dfs(i)) % MOD;
    return result;
}

int main() {
    // Test 1: Single isolated node
    assert(countFoodChains(1, 0, {}) == 1);
    // Test 2: Two nodes, one eats the other: 1 -> 0 (0 is prey of 1). Source = 1, sink = 0, chain: 1->0 count=1
    assert(countFoodChains(2, 1, {{0,1}}) == 1);
    // Test 3: Chain of three: 2 eats 1, 1 eats 0. Sources: 2, sinks: 0. Only path 2-1-0 => 1
    assert(countFoodChains(3, 2, {{0,1},{1,2}}) == 1);
    // Test 4: Two separate chains: each 1 node and 2 nodes => total 1 + 1 = 2
    // Nodes: 0,1,2. Edge: 2 eats 1. Sources: 0 and 2. Sinks: 0 and 1. Paths: [0] length 0, [2,1] length 1 => 2
    assert(countFoodChains(3, 1, {{1,2}}) == 2);
    // Test 5: Diverge: node 2 eats both 0 and 1. Source: 2, sinks: 0,1. Two chains: 2-0, 2-1 => 2
    assert(countFoodChains(3, 2, {{0,2},{1,2}}) == 2);
    // Test 6: Converge: nodes 2 and 3 both eat 0. Sources: 2,3. Sinks: 0. Each chain: 2-0, 3-0 => 2
    assert(countFoodChains(4, 2, {{0,2},{0,3}}) == 2);
    // Test 7: Complex DAG: 3->2, 3->1, 2->0, 1->0. Sources: 3. Paths: 3-2-0 and 3-1-0 => 2
    // Edges: (0,2), (2,3), (0,1), (1,3) i.e., a=prey, b=predator
    assert(countFoodChains(4, 4, {{0,2},{2,3},{0,1},{1,3}}) == 2);
    // Test 8: Multiple sources, one sink: nodes 3,4 both eat 2, node 2 eats 0,1. Sources: 3,4. Sinks: 0,1. Paths: 3-2-0,3-2-1,4-2-0,4-2-1 => 4
    assert(countFoodChains(5, 4, {{2,3},{2,4},{0,2},{1,2}}) == 4);
    // Test 9: Long chain of 10 nodes, count=1
    int n=10; std::vector<std::pair<int,int>> edges;
    for (int i=0;i<n-1;i++) edges.push_back({i, i+1});
    assert(countFoodChains(n, (int)edges.size(), edges) == 1);
    // Test 10: Large modulo check: n=5000, each pair i eats i-1 (chain). Should be 1
    n=5000; edges.clear();
    for (int i=1;i<n;i++) edges.push_back({i-1, i});
    assert(countFoodChains(n, (int)edges.size(), edges) == 1);

    return 0;
}
