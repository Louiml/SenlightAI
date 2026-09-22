// Write a C++ function `int maxThroughput(int numMachines, const std::vector<int>& machineCapacities, const std::vector<std::vector<int>>& connections, const std::vector<int>& sources, const std::vector<int>& sinks)` that models a factory as a flow network. Each machine `i` (1-indexed) has an internal capacity `machineCapacities[i-1]`. Connections are directed edges from machine `u` to machine `v` with capacity `c`, but due to machine internals, an edge from `u` to `v` actually goes from the output of `u` to the input of `v`, and similarly sources and sinks connect to machine inputs/outputs. The function should return the maximum total flow from any source to any sink through this network, respecting all machine and edge capacities. The graph is guaranteed to be connected enough that at least one path exists from some source to some sink. Assume all inputs are valid (indices are within range, capacities non-negative, etc.), but `numMachines` may be 0 (then return 0). Implement the Edmonds–Karp (BFS-based) max flow algorithm. Use an augmented node splitting technique: split each machine into in-node and out-node with an internal capacity edge.

The problem requires computing the maximum flow in a directed graph with node capacities (each machine has an internal capacity). The standard technique is node splitting: for each original machine `i`, create two nodes: `in_i` (index `2*i-1`) and `out_i` (index `2*i`). Connect `in_i` to `out_i` with capacity equal to the machine's internal capacity. All original edges go from `out_u` to `in_v` with the given capacity. Sources connect from a super source `s` to `in_u` with infinite capacity (use a large constant, e.g., 1e9). Sinks connect from `out_u` to a super sink `t` with infinite capacity. Then run max flow (Edmonds–Karp) from `s` to `t`. Edge cases: if there are no machines, return 0. If a source or sink is duplicated, handle gracefully. The BFS must handle both forward edges (increasing flow) and backward edges (decreasing flow) using residual capacities. Time complexity is O(V * E^2) where V = 2*numMachines + 2, E = numMachines + number_of_connections + numSources + numSinks. Space is O(V + E).

#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>

// Compute maximum throughput from sources to sinks in a machine network.
// Machines are 1-indexed. Each machine has an internal capacity.
// connections: {u, v, capacity} edge from machine u to machine v.
// sources: list of machine indices that are entry points.
// sinks: list of machine indices that are exit points.
int maxThroughput(int numMachines,
                  const std::vector<int>& machineCapacities,
                  const std::vector<std::vector<int>>& connections,
                  const std::vector<int>& sources,
                  const std::vector<int>& sinks) {
    if (numMachines == 0) return 0;

    const int INF = 1000000000;
    // Node indexing: 0 = super source, 1..2*numMachines = split nodes, last = super sink
    int n = 2 * numMachines + 2;
    int s = 0;
    int t = 2 * numMachines + 1;

    // Build edge lists
    std::vector<std::vector<int>> adj(n);
    std::vector<int> from, to, cap, flow;

    auto addEdge = [&](int u, int v, int c) {
        int idx = from.size();
        from.push_back(u); to.push_back(v); cap.push_back(c); flow.push_back(0);
        adj[u].push_back(idx);
        adj[v].push_back(idx); // for reverse traversal
    };

    // Internal machine capacity edges: in_i -> out_i
    for (int i = 1; i <= numMachines; ++i) {
        int inNode = 2 * i - 1;
        int outNode = 2 * i;
        addEdge(inNode, outNode, machineCapacities[i-1]);
    }

    // Original connections: out_u -> in_v
    for (const auto& conn : connections) {
        int u = conn[0], v = conn[1], c = conn[2];
        int outU = 2 * u;
        int inV = 2 * v - 1;
        addEdge(outU, inV, c);
    }

    // Sources: super s -> in_u
    for (int src : sources) {
        addEdge(s, 2 * src - 1, INF);
    }

    // Sinks: out_u -> super t
    for (int snk : sinks) {
        addEdge(2 * snk, t, INF);
    }

    // BFS-based max flow (Edmonds–Karp)
    int totalFlow = 0;
    while (true) {
        std::vector<int> parent(n, -1);
        std::vector<int> parentEdge(n, -1);
        std::vector<int> minCap(n, 0);
        std::queue<int> q;
        parent[s] = s;
        minCap[s] = INF;
        q.push(s);

        while (!q.empty() && parent[t] == -1) {
            int v = q.front(); q.pop();
            for (int e : adj[v]) {
                int u;
                int remaining;
                if (from[e] == v) {
                    u = to[e];
                    remaining = cap[e] - flow[e];
                } else {
                    u = from[e];
                    remaining = flow[e]; // reverse edge can push flow back
                }
                if (parent[u] == -1 && remaining > 0) {
                    parent[u] = v;
                    parentEdge[u] = e;
                    minCap[u] = std::min(minCap[v], remaining);
                    if (u == t) break;
                    q.push(u);
                }
            }
        }

        if (parent[t] == -1) break; // no augmenting path

        int f = minCap[t];
        // Augment flow along path
        int cur = t;
        while (cur != s) {
            int e = parentEdge[cur];
            if (to[e] == cur) {
                flow[e] += f;
            } else {
                flow[e] -= f;
            }
            cur = parent[cur];
        }
        totalFlow += f;
    }

    return totalFlow;
}

#include <cassert>
#include <vector>

// (Include the solution function here or in a header)

int main() {
    // Single machine with sources/sinks
    assert(maxThroughput(1, {10}, {}, {1}, {1}) == 10);

    // Simple chain: machine1 -> machine2, each capacity 5
    assert(maxThroughput(2, {5,5}, {{1,2,7}}, {1}, {2}) == 5); // bottleneck is machine 1

    // Chain with edge bottleneck
    assert(maxThroughput(2, {10,10}, {{1,2,3}}, {1}, {2}) == 3);

    // Three machines: source->1->2->3->sink, internal caps 10,20,10, edges 5,8
    assert(maxThroughput(3, {10,20,10}, {{1,2,5},{2,3,8}}, {1}, {3}) == 5);

    // Two parallel sources to one sink
    assert(maxThroughput(2, {3,4}, {{1,2,100}}, {1,2}, {2}) == 7); // both machines feed into 2, but 2's internal cap is 4, plus source from 2 itself? Actually source from 2 directly also exists, so total = 3 (from 1) + 4 (from 2) = 7

    // No machines
    assert(maxThroughput(0, {}, {}, {}, {}) == 0);

    // Edge from source to sink directly through a machine with zero internal cap
    assert(maxThroughput(1, {0}, {}, {1}, {1}) == 0);

    // Complex network with back edge potential
    assert(maxThroughput(4, {10,10,10,10}, {{1,2,5},{2,3,6},{3,4,7},{1,4,3}}, {1}, {4}) == 8); // max flow = 3 via direct + 5 via 1->2->3->4 = 8

    // Duplicate sinks
    assert(maxThroughput(1, {7}, {}, {1}, {1,1}) == 7);

    // Large capacity, multiple sources
    assert(maxThroughput(2, {100,100}, {{1,2,50}}, {1,2}, {2}) == 150); // 100 from machine1 + 50 from machine2's own source? Actually machine2 source gives 100, plus machine1 source flows 50 through edge -> total 150

    return 0;
}
