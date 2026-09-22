// Write a C++ function `int minimumTotalCost(int n, const std::vector<std::vector<std::pair<int,int>>>& outgoing)`, where `n` is the number of nodes labeled `1` to `n`, and `outgoing[i]` (for `0 <= i < n`) contains a list of `(neighbor, cost)` pairs representing a directed edge from node `i+1` to node `neighbor` (1-indexed) with the given positive integer cost. The goal is to partition all `n` nodes into one or more directed cycles such that each node appears in exactly one cycle (i.e., each node has exactly one outgoing edge and one incoming edge chosen from the given edges). If such a partition is possible, return the minimum total cost of the selected edges. If it is impossible, return -1. Each node may have zero or multiple outgoing edges, and the graph is not necessarily complete. The input edges are guaranteed to have positive costs.
// This is a minimum-cost cycle cover problem on a directed graph, equivalent to finding a minimum-cost perfect matching in a bipartite graph. Construct a bipartite graph with left side representing each node's outgoing role (nodes `1..n`) and right side representing each node's incoming role (nodes `n+1..2n`). Add a source connected to each left node with capacity 1 and cost 0, and each right node connected to a sink with capacity 1 and cost 0. For each original directed edge `(u, v, cost)`, add an edge from left `u` to right `v` with capacity 1 and cost. A flow of value `n` corresponds to selecting exactly one outgoing edge per node and exactly one incoming edge per node, which forms a set of vertex-disjoint directed cycles covering all nodes. The minimum cost of such a flow gives the answer. If the max flow is less than `n`, no valid cycle cover exists, so return -1. Use the successive shortest augmenting path algorithm with Bellman-Ford (SPFA) to handle edge costs (all non-negative, so no negative cycles). Time complexity is O(n * E * V) in the worst case where E is the number of edges plus 2n, and V = 2n+2. Space complexity is O(E).
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>
#include <climits>

// Edge structure for min-cost max-flow
struct MCMFEdge {
    int from, to, cap, flow, cost;
    MCMFEdge(int u, int v, int c, int f, int w) : from(u), to(v), cap(c), flow(f), cost(w) {}
};

// MinCostMaxFlow class (SPFA-based, no negative cycles)
class MinCostMaxFlow {
public:
    int n;
    std::vector<MCMFEdge> edges;
    std::vector<std::vector<int>> G;
    std::vector<int> inq, d, p, a;

    MinCostMaxFlow(int nodes) : n(nodes), G(nodes) {}

    void addEdge(int from, int to, int cap, int cost) {
        edges.emplace_back(from, to, cap, 0, cost);
        edges.emplace_back(to, from, 0, 0, -cost);
        int m = edges.size();
        G[from].push_back(m - 2);
        G[to].push_back(m - 1);
    }

    bool spfa(int s, int t, int& flow, int& cost) {
        d.assign(n, INT_MAX);
        inq.assign(n, 0);
        p.assign(n, -1);
        a.assign(n, 0);
        std::queue<int> q;
        d[s] = 0;
        inq[s] = 1;
        a[s] = INT_MAX;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            inq[u] = 0;
            for (int idx : G[u]) {
                MCMFEdge& e = edges[idx];
                if (e.cap > e.flow && d[e.to] > d[u] + e.cost) {
                    d[e.to] = d[u] + e.cost;
                    p[e.to] = idx;
                    a[e.to] = std::min(a[u], e.cap - e.flow);
                    if (!inq[e.to]) {
                        inq[e.to] = 1;
                        q.push(e.to);
                    }
                }
            }
        }
        if (d[t] == INT_MAX) return false;
        flow += a[t];
        cost += d[t] * a[t];
        for (int u = t; u != s; u = edges[p[u]].from) {
            edges[p[u]].flow += a[t];
            edges[p[u] ^ 1].flow -= a[t];
        }
        return true;
    }

    // Returns pair (maxflow, mincost)
    std::pair<int,int> minCostMaxFlow(int s, int t) {
        int flow = 0, cost = 0;
        while (spfa(s, t, flow, cost));
        return {flow, cost};
    }
};

// Core function: return minimum total cost or -1 if impossible
int minimumTotalCost(int n, const std::vector<std::vector<std::pair<int,int>>>& outgoing) {
    // Build bipartite graph: left nodes 1..n (outgoing), right nodes n+1..2n (incoming)
    int V = 2 * n + 2;
    int source = 0;
    int sink = V - 1;
    MinCostMaxFlow mcmf(V);

    // Source to left nodes, right nodes to sink
    for (int u = 1; u <= n; ++u) {
        mcmf.addEdge(source, u, 1, 0);
        mcmf.addEdge(n + u, sink, 1, 0);
    }

    // Original edges: from left u to right v
    for (int u = 0; u < n; ++u) {
        for (const auto& edge : outgoing[u]) {
            int v = edge.first;  // 1-indexed target
            int cost = edge.second;
            mcmf.addEdge(u + 1, n + v, 1, cost);
        }
    }

    auto [flow, cost] = mcmf.minCostMaxFlow(source, sink);
    if (flow < n) return -1;
    return cost;
}
#include <cassert>
#include <vector>
#include <utility>

// Include the solution function here (or link appropriately)

int main() {
    // Test 1: Simple 2-node cycle
    {
        int n = 2;
        std::vector<std::vector<std::pair<int,int>>> outgoing = {
            {{2, 5}},
            {{1, 3}}
        };
        assert(minimumTotalCost(n, outgoing) == 8);
    }

    // Test 2: Impossible (node 2 has no outgoing edge)
    {
        int n = 2;
        std::vector<std::vector<std::pair<int,int>>> outgoing = {
            {{2, 1}},
            {}
        };
        assert(minimumTotalCost(n, outgoing) == -1);
    }

    // Test 3: Two separate cycles
    {
        int n = 4;
        std::vector<std::vector<std::pair<int,int>>> outgoing = {
            {{2, 10}},
            {{1, 10}},
            {{4, 1}},
            {{3, 1}}
        };
        assert(minimumTotalCost(n, outgoing) == 22);
    }

    // Test 4: Multiple choices, pick minimum cost
    {
        int n = 3;
        std::vector<std::vector<std::pair<int,int>>> outgoing = {
            {{2, 5}, {3, 100}},
            {{3, 1}, {1, 2}},
            {{1, 1}, {2, 7}}
        };
        // Options: 1->2 (5), 2->3 (1), 3->1 (1) total 7
        // or 1->3 (100) etc., so min is 7
        assert(minimumTotalCost(n, outgoing) == 7);
    }

    // Test 5: Single node self-loop
    {
        int n = 1;
        std::vector<std::vector<std::pair<int,int>>> outgoing = {
            {{1, 42}}
        };
        assert(minimumTotalCost(n, outgoing) == 42);
    }

    // Test 6: Self-loop not allowed but only alternative? impossible? 
    // Here node 1 only has self-loop, so cycle cover exists (self-loop counts)
    {
        int n = 1;
        std::vector<std::vector<std::pair<int,int>>> outgoing = {
            {{1, 7}}
        };
        assert(minimumTotalCost(n, outgoing) == 7);
    }

    // Test 7: Larger graph with multiple edges
    {
        int n = 3;
        std::vector<std::vector<std::pair<int,int>>> outgoing = {
            {{2, 1}, {3, 10}},
            {{1, 1}, {3, 2}},
            {{1, 1}, {2, 2}}
        };
        // Min: 1->2 (1), 2->3 (2), 3->1 (1) total 4
        assert(minimumTotalCost(n, outgoing) == 4);
    }

    // Test 8: All edges same cost, any cycle cover
    {
        int n = 3;
        std::vector<std::vector<std::pair<int,int>>> outgoing = {
            {{2, 2}, {3, 2}},
            {{1, 2}, {3, 2}},
            {{1, 2}, {2, 2}}
        };
        // Any perfect matching cost = 6
        assert(minimumTotalCost(n, outgoing) == 6);
    }

    return 0;
}
