// Given a directed acyclic graph (DAG) representing a food web with `n` nodes (1-indexed) and `m` directed edges from a prey to a predator, write a C++ function `long long countMaxFoodChains(int n, const vector<pair<int,int>>& edges)` that returns the total number of distinct maximal food chains (paths that start at a node with no incoming edges and end at a node with no outgoing edges), modulo 80112002. The graph is guaranteed to be a DAG, and there are no duplicate edges. A maximal food chain is defined as any path that begins at a source (in-degree 0) and ends at a sink (out-degree 0). The answer must be computed modulo 80112002 to avoid overflow.

#include<bits/stdc++.h>
using namespace std;

// Declare the function (as defined above)
long long countMaxFoodChains(int n, const vector<pair<int,int>>& edges);

int main() {
    // Case 1: Single node, no edges -> one chain (the node itself)
    assert(countMaxFoodChains(1, {}) == 1);

    // Case 2: Simple chain 1->2->3
    vector<pair<int,int>> e2 = {{1,2},{2,3}};
    assert(countMaxFoodChains(3, e2) == 1);

    // Case 3: Two sources (1 and 2) both lead to a single sink 3
    vector<pair<int,int>> e3 = {{1,3},{2,3}};
    assert(countMaxFoodChains(3, e3) == 2);

    // Case 4: Two independent chains
    vector<pair<int,int>> e4 = {{1,2},{3,4}};
    assert(countMaxFoodChains(4, e4) == 2);

    // Case 5: Branched paths: 1->2, 1->3, 2->4, 3->4
    vector<pair<int,int>> e5 = {{1,2},{1,3},{2,4},{3,4}};
    assert(countMaxFoodChains(4, e5) == 2); // paths: 1-2-4, 1-3-4

    // Case 6: Larger branching with multiple sinks
    vector<pair<int,int>> e6 = {{1,2},{1,3},{2,4},{3,5},{4,6},{5,6}};
    // Chain count: 1-2-4-6 and 1-3-5-6 = 2
    assert(countMaxFoodChains(6, e6) == 2);

    // Case 7: Multiple sources and multiple sinks, complex DAG
    // Sources: 1,2 ; Sinks: 6,7
    // Edges: 1->3, 1->4, 2->4, 3->5, 4->5, 5->6, 5->7
    // Chains:
    // 1-3-5-6
    // 1-3-5-7
    // 1-4-5-6
    // 1-4-5-7
    // 2-4-5-6
    // 2-4-5-7
    vector<pair<int,int>> e7 = {{1,3},{1,4},{2,4},{3,5},{4,5},{5,6},{5,7}};
    assert(countMaxFoodChains(7, e7) == 6);

    // Case 8: Cycle? Not allowed (DAG) but test with self-loop not given; test with isolated node plus edge
    vector<pair<int,int>> e8 = {{1,2},{3,3}}; // 3 has self-loop? But that is a cycle, so invalid. Instead test isolated node:
    // Use isolated node 3 without self-loop
    vector<pair<int,int>> e8b = {{1,2}};
    assert(countMaxFoodChains(3, e8b) == 2); // chains: 1-2, and node 3 alone

    // Case 9: Modulo check with large numbers
    // Create 100 nodes: edges from 1 to 2..100 (source 1, sinks 2..100) -> 99 chains
    vector<pair<int,int>> e9;
    for (int i = 2; i <= 100; ++i) e9.push_back({1,i});
    assert(countMaxFoodChains(100, e9) == 99);

    // Case 10: Stress modulo with 1000 chains
    // Create nodes 1..1000: 1->2, 2->3, ..., 999->1000 (single chain) and also 1->1000 directly? That adds one more chain.
    vector<pair<int,int>> e10;
    for (int i = 1; i <= 999; ++i) e10.push_back({i,i+1});
    e10.push_back({1,1000}); // adds second path
    assert(countMaxFoodChains(1000, e10) == 2);

    cout << "All tests passed." << endl;
    return 0;
}

#include<bits/stdc++.h>
using namespace std;

const int MOD = 80112002;

// Counts the number of maximal food chains (source-to-sink paths) in a DAG.
long long countMaxFoodChains(int n, const vector<pair<int,int>>& edges) {
    vector<vector<int>> adj(n + 1);
    vector<int> indeg(n + 1, 0);
    vector<int> outdeg(n + 1, 0);

    for (const auto& e : edges) {
        int a = e.first;
        int b = e.second;
        adj[a].push_back(b);
        indeg[b]++;
        outdeg[a]++;
    }

    vector<long long> ways(n + 1, 0);
    queue<int> q;

    for (int i = 1; i <= n; ++i) {
        if (indeg[i] == 0) {
            ways[i] = 1;
            q.push(i);
        }
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            ways[v] = (ways[v] + ways[u]) % MOD;
            indeg[v]--;
            if (indeg[v] == 0) {
                q.push(v);
            }
        }
    }

    long long ans = 0;
    for (int i = 1; i <= n; ++i) {
        if (outdeg[i] == 0) {
            ans = (ans + ways[i]) % MOD;
        }
    }
    return ans;
}

// The problem is a classic counting of paths in a DAG from any source to any sink. Since the graph is a DAG, we can use topological sorting (Kahn's algorithm) to process nodes in an order that respects dependencies. We initialize a dynamic programming array `ways[u]` = 1 for every source node (in-degree 0), because each source begins a new food chain. Then, for each node `u` processed in topological order, for every outgoing edge `u -> v`, we add `ways[u]` to `ways[v]` (modulo 80112002), and decrease the in-degree of `v`. When the in-degree of `v` becomes 0, we push `v` into the queue. After processing all nodes, the answer is the sum of `ways[v]` for all sink nodes (out-degree 0), modulo 80112002. Important edge cases: a single node with no edges is both source and sink, so `ways[1]` = 1 and answer is 1. Disconnected components are handled independently. The modulo is applied after each addition to keep numbers small. Time complexity is O(n + m) for graph traversal and topological sort, and space complexity is O(n + m) for adjacency list and degree arrays.
