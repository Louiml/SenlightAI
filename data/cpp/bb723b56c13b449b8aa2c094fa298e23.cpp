// Write a C++ function `int maxFlowWithDinic(int source, int sink, const std::vector<std::vector<int>>& capacity)` that computes the maximum flow in a directed, weighted graph with integer capacities using the Dinic's algorithm as shown in the provided snippet. The graph has vertices numbered from 1 to n (where n is the number of vertices, inferred from the capacity matrix dimensions). The input capacity matrix is square of size (n+1) x (n+1) (since indices are 1-based) and stores the capacity from vertex i to vertex j. The function should handle multiple edges between the same pair by summing their capacities (as done in the snippet). The flow must be computed from a given source (e.g., 1) to a given sink (e.g., n). The function should return the maximum possible flow as an integer. The algorithm must use the BFS-based level graph and DFS-based blocking flow approach as in the snippet, but re-implement it cleanly with modern C++ and proper function signatures. The solution must be self-contained and not rely on global mutable state. Edge cases include a graph with no paths from source to sink (return 0), very large capacities (so use `long long` for safety, though the snippet uses `int`; but we'll use `long long` to be robust), and a graph where source equals sink (return 0). The time complexity must be O(E * V^2) in the worst case for Dinic's, and space O(V^2) for the adjacency matrix.

// The solution implements Dinic's algorithm. The key steps are: (1) Build a residual graph as a 2D vector `flow` initially equal to the given capacities. (2) Repeatedly construct a BFS level graph from the sink to all reachable vertices, where an edge from i to j exists in the residual graph if there is remaining capacity (i.e., `flow[i][j] > 0`). The BFS starts at the sink with level 0, and for each vertex x, it visits all i such that `dis[i] == -1` and `flow[i][x] > 0` (reverse direction because we are building from sink to source). This sets `dis[i] = dis[x] + 1`. (3) Once the level graph is built, perform a DFS from the source to the sink, sending as much flow as possible along the shortest paths in the level graph. The DFS uses recursion, passing the current vertex and the remaining allowed flow (`allo`). At each step, it tries to push flow along edges from the current vertex to vertices with `dis[i] == dis[x] - 1` (since we built BFS from sink, levels decrease as we go from source to sink). It updates the residual capacities accordingly. (4) The total flow is the sum of all delta values returned by each DFS call. The loop continues until no augmenting path is found (i.e., DFS returns 0). Edge cases: If source equals sink, maximum flow is 0. If the graph has no path from source to sink, the BFS will not reach the source, so `dis[source]` remains -1 and the DFS loop will not execute, returning 0. The algorithm works with directed edges; for reverse edges, the residual capacity is handled implicitly by adding back when sending flow. Complexity: Each BFS takes O(V^2) because we scan all vertices for each dequeued vertex, and there are at most V phases (since each phase increases the distance from source to sink by at least 1). Each DFS in a phase is O(V*E) in total across all calls due to the blocking flow property (with E being the number of edges). So worst-case O(V^2 * E) time, but for dense graphs the O(V^3) is typical. Space is O(V^2) for the residual matrix.

#include <vector>
#include <queue>
#include <cstring>

// Computes maximum flow from source to sink in a directed graph using Dinic's algorithm.
// capacity is a (n+1)x(n+1) matrix (1-based indexing), where capacity[i][j] is the original capacity from i to j.
// source and sink are vertex indices in [1, n].
// Returns the maximum flow value.
long long maxFlowWithDinic(int source, int sink, const std::vector<std::vector<long long>>& capacity) {
    int n = (int)capacity.size() - 1; // number of vertices (since 1-based indices)
    if (source == sink) return 0;

    // Residual graph copy
    std::vector<std::vector<long long>> flow = capacity;

    long long totalFlow = 0;
    const long long INF = 1e18;

    // BFS to build level graph from sink backwards
    auto bfs = [&]() -> bool {
        std::vector<int> dis(n + 1, -1);
        std::queue<int> q;
        dis[sink] = 0;
        q.push(sink);
        while (!q.empty()) {
            int x = q.front(); q.pop();
            for (int i = 1; i <= n; ++i) {
                if (dis[i] == -1 && flow[i][x] > 0) { // reverse edge from i to x has residual capacity
                    dis[i] = dis[x] + 1;
                    q.push(i);
                }
            }
        }
        // We only need to continue if source is reachable
        if (dis[source] == -1) return false;

        // DFS to push blocking flow along level graph
        // We need to store dis in a mutable structure for DFS, so define it here
        // But since we use recursion, we can pass dis by reference
        // Implementation of dfs as a lambda is tricky; use a helper function with recursion.
        // We'll restructure: implement a recursive function inside the loop.
        // For clarity, define a local struct or use a separate recursion.
        // Simpler: use a nested function via std::function, but that adds overhead.
        // Since this is for teaching, we can write a helper lambda with recursion using 'auto&& self'.
        long long dfsFlow = 0;
        std::function<long long(int, long long)> dfs = [&](int x, long long allo) -> long long {
            if (x == sink) return allo;
            long long used = 0;
            for (int i = 1; i <= n && allo > 0; ++i) {
                if (dis[i] == dis[x] - 1 && flow[x][i] > 0) {
                    long long delta = dfs(i, std::min(allo, flow[x][i]));
                    if (delta > 0) {
                        flow[x][i] -= delta;
                        flow[i][x] += delta;
                        used += delta;
                        allo -= delta;
                    }
                }
            }
            return used;
        };

        // Run DFS from source with infinite capacity
        long long pushed = dfs(source, INF);
        while (pushed > 0) {
            totalFlow += pushed;
            pushed = dfs(source, INF);
        }
        return true;
    };

    // Run BFS repeatedly until no augmenting path
    while (bfs()) { /* continue */ }
    return totalFlow;
}

#include <cassert>
#include <vector>

// The solution function is defined above; here we test it.

int main() {
    // Test 1: Simple graph with 4 vertices, source=1, sink=4
    // Edges: 1->2 cap 3, 1->3 cap 2, 2->3 cap 1, 2->4 cap 2, 3->4 cap 3
    // Max flow = 4
    std::vector<std::vector<long long>> cap1(5, std::vector<long long>(5, 0));
    cap1[1][2] = 3;
    cap1[1][3] = 2;
    cap1[2][3] = 1;
    cap1[2][4] = 2;
    cap1[3][4] = 3;
    assert(maxFlowWithDinic(1, 4, cap1) == 4);

    // Test 2: Multiple edges between same pair (1->2 twice: 2 and 3) sum to 5
    std::vector<std::vector<long long>> cap2(4, std::vector<long long>(4, 0));
    cap2[1][2] = 2;
    cap2[1][2] += 3; // simulate multiple edges
    cap2[2][3] = 1;
    assert(maxFlowWithDinic(1, 3, cap2) == 1);

    // Test 3: No path from source to sink
    std::vector<std::vector<long long>> cap3(4, std::vector<long long>(4, 0));
    cap3[1][2] = 5;
    cap3[3][4] = 7; // source=1, sink=3, unreachable
    assert(maxFlowWithDinic(1, 3, cap3) == 0);

    // Test 4: Source equals sink
    std::vector<std::vector<long long>> cap4(3, std::vector<long long>(3, 0));
    cap4[1][2] = 10;
    assert(maxFlowWithDinic(1, 1, cap4) == 0);

    // Test 5: Larger graph with bottleneck
    // 1->2 cap 10, 1->3 cap 5, 2->4 cap 8, 3->4 cap 5, 4->5 cap 6
    // Max flow = 6
    std::vector<std::vector<long long>> cap5(6, std::vector<long long>(6, 0));
    cap5[1][2] = 10;
    cap5[1][3] = 5;
    cap5[2][4] = 8;
    cap5[3][4] = 5;
    cap5[4][5] = 6;
    assert(maxFlowWithDinic(1, 5, cap5) == 6);

    // Test 6: Single vertex (n=1), source=sink=1 return 0
    std::vector<std::vector<long long>> cap6(2, std::vector<long long>(2, 0));
    assert(maxFlowWithDinic(1, 1, cap6) == 0);

    // Test 7: Capacity values larger than int (e.g., 2e9) but within long long
    std::vector<std::vector<long long>> cap7(3, std::vector<long long>(3, 0));
    cap7[1][2] = 2000000000LL;
    cap7[2][3] = 2000000000LL;
    assert(maxFlowWithDinic(1, 3, cap7) == 2000000000LL);

    // Test 8: Cyclic graph
    // 1->2 cap 3, 2->1 cap 2, 1->3 cap 1, 2->3 cap 2, 3->4 cap 3, 2->4 cap 1
    // Max flow from 1 to 4 = 3 (path 1->3->4 cap 1 + 1->2->4 cap 1 + 1->2->3->4 cap 1)
    // But note reverse edges don't carry additional flow; compute manually: 
    // Saturated paths: 1->3 (1) plus 1->2->4 (1) plus 1->2->3->4 (1) = 3
    std::vector<std::vector<long long>> cap8(5, std::vector<long long>(5, 0));
    cap8[1][2] = 3;
    cap8[2][1] = 2;
    cap8[1][3] = 1;
    cap8[2][3] = 2;
    cap8[3][4] = 3;
    cap8[2][4] = 1;
    assert(maxFlowWithDinic(1, 4, cap8) == 3);

    // Test 9: Disconnected graph with one path
    std::vector<std::vector<long long>> cap9(5, std::vector<long long>(5, 0));
    cap9[1][2] = 3;
    cap9[2][4] = 4;
    cap9[1][3] = 2;
    cap9[3][4] = 2;
    // Max flow = 5 (1->2->4:3, 1->3->4:2)
    assert(maxFlowWithDinic(1, 4, cap9) == 5);

    // Test 10: All capacities zero
    std::vector<std::vector<long long>> cap10(4, std::vector<long long>(4, 0));
    assert(maxFlowWithDinic(1, 3, cap10) == 0);

    return 0;
}
