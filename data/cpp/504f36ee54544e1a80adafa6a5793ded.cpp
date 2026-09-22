// Write a C++ function named `largestPathValue` that accepts a string `colors` (consisting of lowercase English letters, one per graph node) and a vector of directed edges `edges` where each edge is a pair `[a, b]` meaning there is a directed edge from node `a` to node `b`. The graph has exactly `n = colors.size()` nodes, numbered from `0` to `n-1`. The function must return the maximum possible count of any single color (i.e., the maximum frequency of the same character) along any directed path in the graph. If the graph contains a cycle, return -1. The path can start and end at any nodes and must follow edge directions. If the graph has no edges, the answer is 1 (since a single node path contains one color). Assume the graph may have duplicate edges and self-loops (a self-loop makes the answer -1). The function should handle up to 10^5 nodes and 2*10^5 edges efficiently.

This problem is a combination of topological sorting and dynamic programming on a DAG. We first build an adjacency list and compute in-degrees for each node. We initialize a `dp` table of size `n x 26` where `dp[node][color]` stores the maximum count of that color along any path ending at that node. For nodes with in-degree zero, we push them into a queue and set `dp[node][colors[node]-'a'] = 1`. Then we process nodes in topological order using Kahn’s algorithm: when we pop a node `u`, we increment a visited counter. For each outgoing neighbor `v`, we decrement `v`'s in-degree; if it becomes zero, we push `v` into the queue. For each color `k` from 0 to 25, we update `dp[v][k] = max(dp[v][k], dp[u][k] + (colors[v]-'a' == k))`. This extends the best path ending at `u` by one step to `v`, adding 1 if the color of `v` matches `k`. We keep track of the global maximum `ans` across all `dp` values. After processing all nodes, if the visited counter equals `n`, the graph is a DAG and we return `ans`; otherwise there is a cycle and we return -1. Time complexity is O(n + m * 26) and space complexity is O(n * 26 + n + m). Edge cases: n=1 with no edges → ans=1; self-loop or any cycle → -1; multiple edges are handled naturally because in-degree and adjacency list support duplicates.

#include <vector>
#include <string>
#include <queue>
#include <algorithm>

// Returns the maximum frequency of any single color along a directed path,
// or -1 if the graph contains a cycle.
int largestPathValue(const std::string& colors, const std::vector<std::vector<int>>& edges) {
    int n = static_cast<int>(colors.size());
    std::vector<std::vector<int>> graph(n);
    std::vector<int> indegree(n, 0);

    // Build adjacency list and compute in-degrees.
    for (const auto& edge : edges) {
        int from = edge[0];
        int to = edge[1];
        graph[from].push_back(to);
        ++indegree[to];
    }

    // dp[node][color] = max count of that color along any path ending at node.
    std::vector<std::vector<int>> dp(n, std::vector<int>(26, 0));
    std::queue<int> q;

    // Initialize nodes with zero in-degree.
    for (int i = 0; i < n; ++i) {
        if (indegree[i] == 0) {
            q.push(i);
            dp[i][colors[i] - 'a'] = 1;
        }
    }

    int visited = 0;
    int answer = 1; // minimum possible for any non-empty graph is 1

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        ++visited;

        for (int v : graph[u]) {
            // Reduce in-degree and enqueue if it becomes zero.
            if (--indegree[v] == 0) {
                q.push(v);
            }

            int colorV = colors[v] - 'a';
            for (int c = 0; c < 26; ++c) {
                int candidate = dp[u][c] + (colorV == c ? 1 : 0);
                if (candidate > dp[v][c]) {
                    dp[v][c] = candidate;
                }
                answer = std::max(answer, dp[v][c]);
            }
        }
    }

    // If we couldn't visit all nodes, there is a cycle.
    return visited == n ? answer : -1;
}

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Single node, no edges.
    assert(largestPathValue("a", {}) == 1);

    // Simple chain: 0->1->2, colors: a,b,c -> each path has max frequency 1.
    assert(largestPathValue("abc", {{0,1},{1,2}}) == 1);

    // Chain with repeated color: a->b->c, colors: a,a,a -> path a,a,a has 3 a's.
    assert(largestPathValue("aaa", {{0,1},{1,2}}) == 3);

    // Two parallel chains from same node, colors: a (node0), b (node1), b (node2), all edges from 0 to 1 and 0 to 2.
    // Path 0->1 has colors a,b (max 1), path 0->2 has a,b (max 1), answer 1.
    assert(largestPathValue("abb", {{0,1},{0,2}}) == 1);

    // Diamond: 0->1, 0->2, 1->3, 2->3, colors: a,b,b,a. Path 0->1->3: a,b,a (max 2 since 'a' twice), similarly 0->2->3: a,b,a (max 2).
    assert(largestPathValue("abba", {{0,1},{0,2},{1,3},{2,3}}) == 2);

    // Self-loop: cycle, return -1.
    assert(largestPathValue("aa", {{0,0}}) == -1);

    // Two-node cycle: 0->1 and 1->0.
    assert(largestPathValue("ab", {{0,1},{1,0}}) == -1);

    // Duplicate edges still cycle: 0->1 twice and 1->0.
    assert(largestPathValue("ab", {{0,1},{0,1},{1,0}}) == -1);

    // Disconnected nodes: no edges, n=3, each node has one color, path length 1 -> answer 1.
    assert(largestPathValue("xyz", {}) == 1);

    // Long chain with alternating colors, max frequency of a single color is max of counts along path.
    // Nodes: a,b,a,b,a -> path 0->1->2->3->4 has 3 a's and 2 b's, max = 3.
    assert(largestPathValue("ababa", {{0,1},{1,2},{2,3},{3,4}}) == 3);

    return 0;
}
