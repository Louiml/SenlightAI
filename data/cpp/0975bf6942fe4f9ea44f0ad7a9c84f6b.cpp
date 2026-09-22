// You are given a directed graph with `n` nodes (numbered 1 to `n`) and `e` edges, where each node has a non-negative integer weight. The graph is guaranteed to be acyclic and has no self-loops. Your task is to write a C++ function that computes the maximum total path weight from any source node to the corresponding sink node, considering all paths that start at a node with no incoming edges and end at a node with no outgoing edges. The total path weight is the sum of the weights of the nodes along the path (including both the start and end nodes). For each source-to-sink path set, compute the maximum weight, and also count the number of distinct maximum-weight paths (where two paths are distinct if they differ by at least one edge). If the maximum weight exceeds 10^100, return the weight modulo 10^9+7 and the path count modulo 10^9+7. If no path exists, return 0 weight and 0 paths. The function signature is: `pair<long long, long long> maxPathWeightAndCount(int n, const vector<int>& weights, const vector<pair<int,int>>& edges)` where node indices are 0-based. The function returns a pair: first is the maximum weight modulo MOD, second is the number of maximum-weight paths modulo MOD. The input graph may have multiple edges between the same pair of nodes, and nodes may be isolated. You can assume n ≥ 1, weights are non-negative and fit in `int`.
// This is a classic longest path problem on a DAG, extended with path counting. First, we must build the graph and identify sources (nodes with in-degree 0) and sinks (nodes with out-degree 0). We add a super-source connected to all sources with weight 0 (so these edges contribute 0 to the path weight, but allow uniform start), and a super-sink connected from all sinks with edge weight equal to the sink's node weight? Wait careful: The path weight includes node weights. To handle this cleanly, we can transform edge weights: When we traverse an edge from u to v, we add the weight of the source node u. Then when we reach a sink, we need to add the sink's weight as well. So standard approach: Use topological sort (Kahn's algorithm) on the augmented graph with super-source (node n) and super-sink (node n+1). For each source node i, add edge super-source -> i with weight 0 (but since path weight includes node weights, we must eventually add the source node's weight). Better: For each source node, add edge super-source -> i with weight equal to weight[i]. For each sink node, add edge i -> super-sink with weight 0 (since the sink's weight is already included when we arrived at i). Actually if we treat edge weight w(u,v) = weight[u] (the weight of the node we are leaving), then starting from super-source, we need to add weight of first node. So set edge weight from super-source to source i = weight[i]. For a normal edge u->v, weight = weight[u]. For a sink edge to super-sink, weight = 0 (because when we arrive at the sink, we already added its weight as the source of the previous edge? No: If we have edge u->sink, weight = weight[u], but we also need to add weight[sink] at the end. So better: Add a virtual node for each sink? Alternative cleaner: When we compute longest path, we can compute the maximum path weight ending at each node. Initialize dist[i] = weight[i] for all nodes (since a path can start at i itself). Then process nodes in topological order: for each edge u->v, dist[v] = max(dist[v], dist[u] + weight[v])? That double counts? Let's think: If we define dist[i] as max total weight of a path ending at i, then when transitioning from u to v, we add weight[v]. For a path starting at source s, we initialize dist[s] = weight[s]. Then for edge u->v, dist[v] = max(dist[v], dist[u] + weight[v]). This works because each node's weight is added exactly once per path. So we don't need super-source/super-sink. We just need to find all sources (in-degree 0) and initialize dist for them. Then run topological sort (Kahn's) on the original graph. For path counting, we need to count the number of distinct paths that achieve the maximum dist at each node. For a source node with no predecessors, count[i] = 1. For each edge u->v, if dist[v] < dist[u] + weight[v], then dist[v] = dist[u] + weight[v] and count[v] = count[u]. If dist[v] == dist[u] + weight[v], then count[v] = (count[v] + count[u]) % MOD. Finally, the overall answer is the maximum dist among all sink nodes (out-degree 0). If multiple sinks have the same maximum, we sum their counts. If the maximum is 0 and all weights are 0? That could be valid. If there are no sources? That can't happen in a DAG (always at least one source if n>0). But if no path exists? In a DAG, any source-to-sink path exists? Not necessarily; for example a node with no incoming and no outgoing is both source and sink, so it's a path. So there is always at least one path. However the problem allows isolated nodes; those are both source and sink, and path weight is just the node weight. So we handle that. Time complexity: O(n+e). Space: O(n+e). Need to handle large weights for counting? The maximum path weight can be huge (up to 10^100) but the problem says return modulo for weight and count. So we can compute dist as modulo 1e9+7 as well? But careful: the comparison `dist[v] < dist[u] + weight[v]` must be done on actual values, not modulo, because modulo can cause incorrect comparisons. Since weights are up to 10^9 and n up to 10^5, actual path weight could be up to 10^14, which fits in long long (10^18). The problem says it could exceed 10^100, so we need to handle arbitrarily large numbers. But we can store actual values as big integers? That would be messy. Likely the intended solution is to compute the maximum weight using big integer (or use modulo for count and something else for comparison). But the problem statement says if max exceeds 10^100, return weight modulo and count modulo. So we can use a custom big integer for comparison, but that's overkill. Instead, since weights are non-negative and n up to 10^5, the maximum path weight is at most 10^9 * 10^5 = 10^14, which is far less than 10^100. So the "if exceeds 10^100" clause is a red herring; we can safely use long long for actual weights. The modulo return is for safety, but we can just output the actual weight modulo MOD anyway. So we'll store dist as long long (actual value) and count as long long modulo MOD. For counting, we only need to know when dist[v] == dist[u] + weight[v] using actual values. So we keep dist as long long. At the end, output dist mod MOD.
#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007LL;

// Returns pair {maxPathWeight % MOD, countOfMaxPaths % MOD}
// Nodes are 0-indexed, edges are directed acyclic.
pair<long long, long long> maxPathWeightAndCount(
    int n,
    const vector<int>& weights,
    const vector<pair<int,int>>& edges
) {
    vector<vector<int>> adj(n);
    vector<int> indeg(n, 0), outdeg(n, 0);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        indeg[e.second]++;
        outdeg[e.first]++;
    }

    // Longest path ending at each node (actual sum)
    vector<long long> dist(n, 0);
    // Number of paths achieving dist[i]
    vector<long long> cnt(n, 0);

    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (indeg[i] == 0) {
            dist[i] = weights[i];
            cnt[i] = 1;
            q.push(i);
        }
    }

    // Kahn's topological sort
    int processed = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        processed++;
        for (int v : adj[u]) {
            indeg[v]--;
            if (indeg[v] == 0) q.push(v);
            long long newDist = dist[u] + weights[v];
            if (newDist > dist[v]) {
                dist[v] = newDist;
                cnt[v] = cnt[u];
            } else if (newDist == dist[v]) {
                cnt[v] = (cnt[v] + cnt[u]) % MOD;
            }
        }
    }

    // If graph has cycles (should not happen per problem) but just in case:
    if (processed != n) return {0, 0};

    // Find best among sinks (outdeg == 0)
    long long best = -1;
    long long bestCnt = 0;
    for (int i = 0; i < n; ++i) {
        if (outdeg[i] == 0) {
            if (dist[i] > best) {
                best = dist[i];
                bestCnt = cnt[i];
            } else if (dist[i] == best) {
                bestCnt = (bestCnt + cnt[i]) % MOD;
            }
        }
    }

    if (best < 0) best = 0; // Should not happen if n>=1
    return {best % MOD, bestCnt};
}
#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// Include the solution function here (or declare it)
pair<long long, long long> maxPathWeightAndCount(
    int n, const vector<int>& weights, const vector<pair<int,int>>& edges
);

int main() {
    // Simple two-node chain: 0->1, weights {5, 7} => path total 12, count 1
    {
        auto res = maxPathWeightAndCount(2, {5,7}, {{0,1}});
        assert(res.first == 12 && res.second == 1);
    }
    // Two parallel edges from 0 to 1, weight {3, 4} => max total 7, but distinct paths? Two edges are distinct => count 2
    {
        auto res = maxPathWeightAndCount(2, {3,4}, {{0,1},{0,1}});
        assert(res.first == 7 && res.second == 2);
    }
    // Isolated node with weight 42 => path just that node
    {
        auto res = maxPathWeightAndCount(1, {42}, {});
        assert(res.first == 42 && res.second == 1);
    }
    // Graph with fork: 0->1 and 0->2, both sinks, weights {1,2,3} => max path 1+2=3? Wait includes weights: ending at 1: 1+2=3; ending at 2: 1+3=4 => max 4
    {
        auto res = maxPathWeightAndCount(3, {1,2,3}, {{0,1},{0,2}});
        assert(res.first == 4 && res.second == 1);
    }
    // Two sources with same max: 0->2 and 1->2, weights {2,3,1} => path 0->2 total 2+1=3, path 1->2 total 3+1=4 => max 4 count 1
    {
        auto res = maxPathWeightAndCount(3, {2,3,1}, {{0,2},{1,2}});
        assert(res.first == 4 && res.second == 1);
    }
    // Same max from two sources: weights {2,2,0} edges 0->2,1->2 => both paths total 2, count 2
    {
        auto res = maxPathWeightAndCount(3, {2,2,0}, {{0,2},{1,2}});
        assert(res.first == 2 && res.second == 2);
    }
    // Longer chain: 0->1->2, weights {1,2,3} => total 1+2+3=6
    {
        auto res = maxPathWeightAndCount(3, {1,2,3}, {{0,1},{1,2}});
        assert(res.first == 6 && res.second == 1);
    }
    // Multiple max paths in chain? If equal alternatives: 0->1, 0->2, both 1->3, 2->3, weights {1,1,1,1} edges: (0,1),(0,2),(1,3),(2,3) => all paths total 3, count 2
    {
        auto res = maxPathWeightAndCount(4, {1,1,1,1}, {{0,1},{0,2},{1,3},{2,3}});
        assert(res.first == 3 && res.second == 2);
    }
    // Check modulo: large weights but still fits in long long, but we can test count modulo
    // Use weights up to 1e9 and many paths
    {
        int n = 3;
        vector<int> w = {1000000000, 1000000000, 1000000000};
        vector<pair<int,int>> e = {{0,2},{1,2}};
        auto res = maxPathWeightAndCount(n, w, e);
        assert(res.first == 2000000000LL % 1000000007LL);
        assert(res.second == 2);
    }
    // No edges but two isolated nodes => each is its own path, max is max weight, count of nodes with that weight
    {
        vector<int> w = {5, 7, 7};
        auto res = maxPathWeightAndCount(3, w, {});
        assert(res.first == 7 && res.second == 2);
    }
    return 0;
}
