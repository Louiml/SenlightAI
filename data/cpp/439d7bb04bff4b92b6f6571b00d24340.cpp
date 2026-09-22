/*
You are given a directed graph with `n` nodes numbered from `0` to `n-1` and weighted edges. Write a standalone C++ function `longestOddPath` that, given the number of nodes `n`, a vector of edges `edges` (each edge as `{u, v, weight}` with integer weight, may be negative), a start node `start`, and a target node `target`, returns the maximum sum of a path from `start` to `target` that visits only nodes with odd indices (the start and target nodes may be odd or even, but all intermediate nodes on the path must have odd indices). If no such path exists, return `INT_MIN`. The path may revisit nodes? No — the function must not revisit any node (simple path). Edge weights can be negative, so the maximum sum may be negative. Assume `0 <= start, target < n` and `n >= 1`. If `start == target`, return `0` regardless of parity (empty path).
*/

#include <vector>
#include <climits>
using namespace std;

// Helper DFS function that explores simple paths from node 'node' to 'target'.
// 'visited' marks nodes on the current path. 'sum' is the accumulated edge weight sum.
int dfsOddPath(int node, int target, const vector<vector<pair<int,int>>>& graph,
               vector<bool>& visited, int sum) {
    if (node == target) return sum;
    int best = INT_MIN;
    for (const auto& edge : graph[node]) {
        int next = edge.first;
        int weight = edge.second;
        // Only allow moving to an unvisited node with odd index (parity == 1).
        if (!visited[next] && (next % 2 == 1)) {
            visited[next] = true;
            int candidate = dfsOddPath(next, target, graph, visited, sum + weight);
            if (candidate > best) best = candidate;
            visited[next] = false; // backtrack
        }
    }
    return best;
}

// Returns the maximum sum of a simple path from start to target where
// every intermediate node has an odd index. start and target may be even.
// If start == target, returns 0. If no path exists, returns INT_MIN.
int longestOddPath(int n, const vector<vector<int>>& edges, int start, int target) {
    if (start == target) return 0;

    // Build adjacency list: graph[u] = list of {v, weight}
    vector<vector<pair<int,int>>> graph(n);
    for (const auto& e : edges) {
        int u = e[0];
        int v = e[1];
        int w = e[2];
        graph[u].push_back({v, w});
    }

    vector<bool> visited(n, false);
    visited[start] = true; // start is considered visited to avoid revisiting
    return dfsOddPath(start, target, graph, visited, 0);
}

#include <cassert>
#include <climits>
#include <vector>
using namespace std;

int main() {
    // Test 1: Simple odd path from even start 0 to even target 2 via odd nodes 1,3
    // Path 0->1 (w=5) ->3 (w=7) ->2 (w=2) => sum=14
    assert(longestOddPath(5, {{0,1,5},{1,3,7},{3,2,2},{3,4,1}}, 0, 2) == 14);

    // Test 2: No path because all intermediate nodes are even or unreachable
    assert(longestOddPath(4, {{0,2,3},{2,1,1}}, 0, 1) == INT_MIN);

    // Test 3: Start equals target, returns 0 regardless of parity
    assert(longestOddPath(3, {{0,1,5}}, 2, 2) == 0);

    // Test 4: Negative weights: path may have negative sum, but must be maximum
    // Only path 0->1 (w=-5) ->2 (w=-2) => sum=-7
    assert(longestOddPath(3, {{0,1,-5},{1,2,-2},{0,2,100}}, 0, 2) == -7);

    // Test 5: Multiple paths, pick the maximum sum
    // Path0: 1->3 (w=10) ->5 (w=1) ->2 (w=2) => 13
    // Path1: 1->3 (w=10) ->5 (w=1) ->4 (w=3) ->2 (w=1) => 15
    assert(longestOddPath(6, {{1,3,10},{3,5,1},{5,2,2},{5,4,3},{4,2,1},{1,0,100}}, 1, 2) == 15);

    // Test 6: Even start, odd intermediate, even target
    // 2->3 (w=4) ->1 (w=6) ->4 (w=1) => sum=11
    assert(longestOddPath(5, {{2,3,4},{3,1,6},{1,4,1},{1,0,99}}, 2, 4) == 11);

    // Test 7: Cycle present but cannot revisit => must not loop
    // Self-loop at odd node 1 is ignored because visited prevents revisit.
    // Path 0->1 (w=1) ->2 (w=2) => 3, but also direct 0->2 (w=5) not allowed because intermediate none? Actually 0 even, 2 even, direct edge is allowed (no intermediate odd nodes). So max is 5.
    assert(longestOddPath(3, {{0,1,1},{1,1,100},{1,2,2},{0,2,5}}, 0, 2) == 5);

    // Test 8: Single node path where start==target, even
    assert(longestOddPath(1, {}, 0, 0) == 0);

    // Test 9: Target is odd, but start is even and no intermediate needed? Actually direct edge from even to odd is allowed because the target itself is odd, no intermediate nodes. Sum is the edge weight.
    assert(longestOddPath(2, {{0,1,-10}}, 0, 1) == -10);

    // Test 10: No outgoing edges from start and start != target
    assert(longestOddPath(2, {{1,0,5}}, 0, 1) == INT_MIN);

    return 0;
}

// This problem is a variation of finding the maximum-weight simple path in a directed graph with a node parity constraint. A depth-first search (DFS) with backtracking is appropriate because the graph is small (no explicit size limit in the task, but typical for such exercises). The main idea: maintain a `visited` boolean vector to track nodes on the current path, and recursively explore neighbors that are unvisited and have odd index. We start from `start` with sum `0` and visited[start] = true (pre‑marking start to avoid revisiting). For each recursive call at node `p`, if `p == target`, we return the accumulated sum. Otherwise, for each outgoing edge `(p, v, w)`, if `v` is unvisited and `v % 2 == 1` (odd), we recurse with sum + w. We track the maximum over all such recursive results. After exploring, unmark `p` (backtracking). If no path reaches target, the function returns `INT_MIN`. Edge cases: `start` or `target` may be even, but intermediate nodes must be odd; if `start` is even, it is still allowed as the starting point (the recursion begins there). If `start == target`, we return `0` immediately without checking parity. The graph may have cycles, but the visited set prevents infinite recursion. Time complexity is `O(V! * degree)` in the worst case (exponential due to simple paths), but for small graphs it's acceptable. Space complexity is `O(V)` for the visited array and recursion stack.
