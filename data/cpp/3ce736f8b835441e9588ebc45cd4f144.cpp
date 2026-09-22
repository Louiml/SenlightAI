/*
Implement a C++ function `double maxFlow(int n, const std::vector<std::vector<double>>& capacity, int source, int sink)` that computes the maximum flow from `source` to `sink` in a directed graph with `n` nodes (0-indexed). The graph is given as an `n x n` matrix `capacity` where `capacity[u][v]` is the non-negative capacity of the edge from `u` to `v` (use 0 for no edge). The function must return the maximum flow value, assuming real-valued capacities (use `double`). The graph may contain cycles, multiple edges (represented as sum of capacities), and self-loops (which should be ignored). The algorithm must be the Edmonds-Karp variant of the Ford-Fulkerson method, using breadth-first search to find augmenting paths in the residual network, as in the provided snippet. Handle the edge case where `source == sink` by returning `std::numeric_limits<double>::infinity()`. The function must not modify the input matrix and must be `const`-correct.
*/
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

/**
 * Compute the maximum flow from source to sink in a directed graph.
 * @param n number of nodes (0-indexed)
 * @param capacity n x n matrix of edge capacities (non-negative doubles, 0 for no edge)
 * @param source source node index
 * @param sink sink node index
 * @return maximum flow value, or infinity if source == sink
 */
double maxFlow(int n, const std::vector<std::vector<double>>& capacity, int source, int sink) {
    if (source == sink) {
        return std::numeric_limits<double>::infinity();
    }

    // flow matrix to track current flow on each directed edge
    std::vector<std::vector<double>> flow(n, std::vector<double>(n, 0.0));

    double totalFlow = 0.0;

    // BFS to find augmenting path
    while (true) {
        std::vector<int> parent(n, -1);
        std::vector<double> bottleneck(n, 0.0);
        std::queue<int> q;

        parent[source] = source;
        bottleneck[source] = std::numeric_limits<double>::infinity();
        q.push(source);

        bool found = false;

        while (!q.empty() && !found) {
            int u = q.front();
            q.pop();

            for (int v = 0; v < n; ++v) {
                // skip self-loops and already visited nodes
                if (parent[v] != -1 || u == v) continue;

                // forward edge residual capacity
                double residualForward = capacity[u][v] - flow[u][v];
                // reverse edge residual capacity (flow that can be pushed back)
                double residualReverse = flow[u][v];

                if (residualForward > 0.0) {
                    parent[v] = u;
                    bottleneck[v] = std::min(bottleneck[u], residualForward);
                    if (v == sink) {
                        found = true;
                        break;
                    }
                    q.push(v);
                } else if (residualReverse > 0.0) {
                    parent[v] = -u - 1; // mark as reverse edge (negative encoding)
                    bottleneck[v] = std::min(bottleneck[u], residualReverse);
                    if (v == sink) {
                        found = true;
                        break;
                    }
                    q.push(v);
                }
            }
        }

        if (!found) break; // no augmenting path

        double addFlow = bottleneck[sink];
        totalFlow += addFlow;

        // update flow along the augmenting path
        int v = sink;
        while (v != source) {
            int p = parent[v];
            if (p >= 0) {
                // forward edge p -> v
                flow[p][v] += addFlow;
                v = p;
            } else {
                // reverse edge: p = -u - 1, so u = -p - 1
                int u = -p - 1;
                flow[v][u] -= addFlow; // note: actual flow on u->v is reduced
                v = u;
            }
        }
    }

    return totalFlow;
}
#include <cassert>
#include <cmath>
#include <vector>

// solution function declared above (or include the solution header)

int main() {
    // Test 1: simple path 0->1->2
    {
        int n = 3;
        std::vector<std::vector<double>> cap(n, std::vector<double>(n, 0.0));
        cap[0][1] = 10.0;
        cap[1][2] = 5.0;
        double result = maxFlow(n, cap, 0, 2);
        assert(std::fabs(result - 5.0) < 1e-9);
    }

    // Test 2: multiple paths with shared edge
    {
        int n = 4;
        std::vector<std::vector<double>> cap(n, std::vector<double>(n, 0.0));
        cap[0][1] = 10.0;
        cap[0][2] = 5.0;
        cap[1][3] = 10.0;
        cap[2][3] = 5.0;
        double result = maxFlow(n, cap, 0, 3);
        assert(std::fabs(result - 15.0) < 1e-9);
    }

    // Test 3: graph with reverse edge needed (cancellation)
    {
        int n = 4;
        std::vector<std::vector<double>> cap(n, std::vector<double>(n, 0.0));
        cap[0][1] = 3.0;
        cap[1][2] = 3.0;
        cap[2][3] = 3.0;
        cap[0][2] = 3.0;
        cap[1][3] = 3.0;
        double result = maxFlow(n, cap, 0, 3);
        assert(std::fabs(result - 6.0) < 1e-9);
    }

    // Test 4: source == sink returns infinity
    {
        int n = 3;
        std::vector<std::vector<double>> cap(n, std::vector<double>(n, 0.0));
        cap[0][1] = 5.0;
        double result = maxFlow(n, cap, 1, 1);
        assert(std::isinf(result));
    }

    // Test 5: no path exists => 0 flow
    {
        int n = 3;
        std::vector<std::vector<double>> cap(n, std::vector<double>(n, 0.0));
        cap[0][1] = 5.0;
        cap[1][2] = 0.0; // broken
        double result = maxFlow(n, cap, 0, 2);
        assert(std::fabs(result - 0.0) < 1e-9);
    }

    // Test 6: self-loops ignored
    {
        int n = 2;
        std::vector<std::vector<double>> cap(n, std::vector<double>(n, 0.0));
        cap[0][0] = 100.0;
        cap[1][1] = 100.0;
        cap[0][1] = 7.0;
        double result = maxFlow(n, cap, 0, 1);
        assert(std::fabs(result - 7.0) < 1e-9);
    }

    // Test 7: large capacities and decimal values
    {
        int n = 3;
        std::vector<std::vector<double>> cap(n, std::vector<double>(n, 0.0));
        cap[0][1] = 2.5;
        cap[1][2] = 1.5;
        cap[0][2] = 0.5;
        double result = maxFlow(n, cap, 0, 2);
        assert(std::fabs(result - 2.0) < 1e-9);
    }

    // Test 8: symmetric edges (both directions)
    {
        int n = 2;
        std::vector<std::vector<double>> cap(n, std::vector<double>(n, 0.0));
        cap[0][1] = 5.0;
        cap[1][0] = 3.0;
        double result = maxFlow(n, cap, 0, 1);
        assert(std::fabs(result - 5.0) < 1e-9);
    }

    return 0;
}
// The solution uses the standard Edmonds-Karp algorithm, which repeatedly finds the shortest augmenting path (in terms of number of edges) in the residual graph via BFS from `source` to `sink`. For each such path, the bottleneck capacity (minimum residual capacity along the path) is added to the total flow, and the residual capacities are updated: for each forward edge `u->v`, `flow[u][v]` increases by the bottleneck, and for the reverse edge `v->u`, `flow[u][v]` decreases by the bottleneck (allowing cancellation). The residual capacity for a forward edge `u->v` is `capacity[u][v] - flow[u][v]`, and for a reverse edge it is `flow[u][v]` (since flow can be pushed back). BFS ensures that each augmentation uses a path with the fewest edges, giving a polynomial time bound. The outer loop terminates when no augmenting path exists (BFS fails to reach the sink). Edge cases: `source == sink` returns positive infinity; zero-capacity edges are ignored; self-loops are skipped because `u==v` would either have no positive residual or be irrelevant. Time complexity: O(V * E^2) in the worst case, where V = n and E = number of edges, but for typical dense matrices it is acceptable. Space complexity: O(V^2) for the flow matrix and O(V) for BFS arrays.
