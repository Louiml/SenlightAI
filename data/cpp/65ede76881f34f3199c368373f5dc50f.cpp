// Given a directed weighted graph with `n` nodes (numbered 1 to n) and `m` edges, write a C++ function `long long maximumPathScore(int n, const vector<tuple<int,int,long long>>& edges)` that returns the maximum possible score from node 1 to node n, where the score is the sum of edge weights along a path. The path may repeat vertices and edges as long as the total score can be increased without bound — if such an infinite positive cycle is reachable from node 1 and can reach node n, the function must return a special sentinel value `LLONG_MAX` to indicate "Cycle Detected". If node n is unreachable from node 1, return `-INF` (use `LLONG_MIN/2`). If no infinite cycle exists but node n is reachable, return the maximum finite score. The graph may contain negative weights, zero weights, and parallel edges. The function must handle up to `n=2500` nodes and `m=10000` edges, and all edge weights fit in a 64-bit signed integer.
#include <cassert>
#include <climits>
#include <vector>
#include <tuple>
using namespace std;

int main() {
    // Basic path 1->2->3->4, but 1->4 direct is lower
    {
        vector<tuple<int,int,long long>> edges = {
            {0,1,3}, {1,3,-1}, {0,2,-2}, {2,3,7}, {0,3,4}
        };
        assert(maximumPathScore(4, edges) == 5); // 3 + (-1) = 2, -2+7=5, direct 4 => best 5
    }

    // Unreachable target (node 4 disconnected)
    {
        vector<tuple<int,int,long long>> edges = {
            {0,1,5}, {1,2,-1}
        };
        assert(maximumPathScore(5, edges) == LLONG_MIN/2);
    }

    // Positive cycle reachable and can reach target => LLONG_MAX
    {
        vector<tuple<int,int,long long>> edges = {
            {0,1,10}, {1,0,-10}, {1,2,100} // cycle 0<->1 but no net positive? Actually 10-10=0, not infinite
        };
        // That cycle is zero net, so no infinite. Add positive cycle:
        edges.clear();
        edges = {{0,1,1}, {1,0,1}, {1,2,5}}; // cycle 0->1->0 of +2 each iteration
        assert(maximumPathScore(3, edges) == LLONG_MAX);
    }

    // Positive cycle but not on path to target (cycle in a dead-end)
    {
        vector<tuple<int,int,long long>> edges = {
            {0,1,1}, {1,0,1}, {0,2,10} // cycle 0<->1 positive, but node 2 is target reachable via 0->2
        };
        // The cycle is reachable from 0 and can also reach target? 1 cannot reach 2, so no.
        assert(maximumPathScore(3, edges) == 10);
    }

    // Negative edge but no positive cycle
    {
        vector<tuple<int,int,long long>> edges = {
            {0,1,-5}, {1,2,10}
        };
        assert(maximumPathScore(3, edges) == 5);
    }

    // Self-loop positive at node 1 that can reach target
    {
        vector<tuple<int,int,long long>> edges = {
            {0,1,2}, {1,1,1}, {1,2,3}
        };
        assert(maximumPathScore(3, edges) == LLONG_MAX);
    }

    // Single node with no edges: start is target
    {
        vector<tuple<int,int,long long>> edges;
        assert(maximumPathScore(1, edges) == 0);
    }

    // Two nodes with zero-weight edge
    {
        vector<tuple<int,int,long long>> edges = {{0,1,0}};
        assert(maximumPathScore(2, edges) == 0);
    }

    // Long test: chain with large values
    {
        vector<tuple<int,int,long long>> edges;
        for (int i = 0; i < 99; ++i) {
            edges.emplace_back(i, i+1, 1000000LL);
        }
        assert(maximumPathScore(100, edges) == 99000000LL);
    }

    return 0;
}
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Returns the maximum score from node 0 to node n-1, or LLONG_MAX if an
// infinitely increasing cycle can be used, or LLONG_MIN/2 if unreachable.
ll maximumPathScore(int n, const vector<tuple<int,int,ll>>& edges) {
    const ll NEG_INF = LLONG_MIN / 2;
    const ll POS_INF = LLONG_MAX;

    vector<ll> dist(n, NEG_INF);
    dist[0] = 0;

    // Bellman-Ford relaxation for longest paths, n-1 iterations
    for (int i = 0; i < n - 1; ++i) {
        bool updated = false;
        for (const auto& [u, v, w] : edges) {
            if (dist[u] == NEG_INF) continue;
            if (dist[u] + w > dist[v]) {
                dist[v] = dist[u] + w;
                updated = true;
            }
        }
        if (!updated) break; // early exit if no changes
    }

    // Build forward and reverse adjacency lists for reachability checks
    vector<vector<int>> adj(n), radj(n);
    for (const auto& [u, v, w] : edges) {
        adj[u].push_back(v);
        radj[v].push_back(u);
    }

    // Nodes reachable from source (0)
    vector<bool> reachableFromSource(n, false);
    queue<int> q;
    q.push(0);
    reachableFromSource[0] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (!reachableFromSource[v]) {
                reachableFromSource[v] = true;
                q.push(v);
            }
        }
    }

    // Nodes that can reach target (n-1)
    vector<bool> canReachTarget(n, false);
    q.push(n-1);
    canReachTarget[n-1] = true;
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (int u : radj[v]) {
            if (!canReachTarget[u]) {
                canReachTarget[u] = true;
                q.push(u);
            }
        }
    }

    // Check for a positive cycle that is on a relevant path
    bool infiniteCycle = false;
    for (const auto& [u, v, w] : edges) {
        if (dist[u] == NEG_INF) continue;
        if (dist[u] + w > dist[v]) {
            // u and v are in a cycle; check if that cycle is reachable from 0
            // and can reach n-1 via either u or v
            if (reachableFromSource[u] && canReachTarget[u]) {
                infiniteCycle = true;
                break;
            }
            if (reachableFromSource[v] && canReachTarget[v]) {
                infiniteCycle = true;
                break;
            }
        }
    }

    if (infiniteCycle) return POS_INF;
    if (dist[n-1] == NEG_INF) return NEG_INF;
    return dist[n-1];
}
// The problem is a variant of the "Longest Path in a General Graph" which is NP-hard, but since we allow repeated vertices and edges, we can use Bellman-Ford for the longest path with cycle detection. Initialize distances to `-INF` for all nodes except node 1 (index 0) which is 0. Run the standard Bellman-Ford relaxation for `n-1` iterations: for each edge `(u,v,w)`, if `dist[u]` is not `-INF`, update `dist[v] = max(dist[v], dist[u]+w)`. After these iterations, if no update occurs in the `n`-th iteration, then no positive cycle exists and `dist[n-1]` is the answer (if it remains `-INF`, node n is unreachable). However, the presence of a cycle does not automatically mean the answer is infinite — the cycle must be reachable from node 1 and must be able to reach node n. To check this, after the `n`-th iteration detects any relaxation, we must verify that at least one node that was relaxed is on a path to node n. We can do this by running a reverse graph BFS from node n to mark nodes that can reach node n, and a forward BFS from node 1 to mark reachable nodes. If any node that was relaxed in the `n`-th iteration is both reachable from 1 and can reach n, then the answer is `LLONG_MAX`. Otherwise, the maximum finite distance after the `n-1` iterations is the answer (the cycle is irrelevant because it cannot affect the path from 1 to n). Edge cases: self-loops with positive weight cause immediate infinite if node is reachable and can reach n; negative cycles are ignored because we take maximum. Time complexity: O(n*m) for the Bellman-Ford portion, plus O(n+m) for the BFS checks, so overall O(n*m). Space complexity: O(n+m) for storing edges and adjacency lists.
