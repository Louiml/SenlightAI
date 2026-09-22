// Write a C++ function that takes an undirected weighted graph represented by a list of edges (with vertex indices `0` to `n-1` and edge weights as `long long`) and returns a vector of vertex indices (1-based in output) forming a negative cycle if one exists; if no negative cycle exists, return an empty vector. The function must detect any negative cycle reachable from any vertex, and if multiple exist, it may return any one of them. The return type is `std::vector<int>` where each element is the 1-based vertex label of the cycle in order (starting and ending at the same vertex). The graph may have up to `n` vertices and `m` edges, with weights that can be negative, zero, or positive. The algorithm must handle disconnected components and self-loops correctly. The function signature is:  
// `std::vector<int> findNegativeCycle(int n, const std::vector<Edge>& edges)`  
// where `Edge` is a struct with fields `int a, b; long long cost;` representing an edge from `a` to `b` with weight `cost`. The graph is directed; if an undirected edge is needed, treat it as two directed edges in the input list. The function must not modify the input edges list.
#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Test 1: Simple negative cycle (1 -> 2 -> 1, total weight -1)
    {
        std::vector<Edge> edges = {{0, 1, 1}, {1, 0, -2}};
        std::vector<int> cyc = findNegativeCycle(2, edges);
        assert(!cyc.empty());
        // The cycle should contain exactly two vertices (1 and 2) or a repeated cycle
        assert(cyc.size() == 3); // e.g., {1,2,1} or {2,1,2}
        assert(cyc.front() == cyc.back());
        assert(cyc[0] == 1 && cyc[1] == 2 || cyc[0] == 2 && cyc[1] == 1);
    }

    // Test 2: No negative cycle -> empty vector
    {
        std::vector<Edge> edges = {{0, 1, 1}, {1, 2, 1}, {2, 0, 1}}; // all positive
        std::vector<int> cyc = findNegativeCycle(3, edges);
        assert(cyc.empty());
    }

    // Test 3: Self-loop negative weight (vertex 2 (0-based) -> itself with -1)
    {
        std::vector<Edge> edges = {{1, 1, -5}};
        std::vector<int> cyc = findNegativeCycle(2, edges);
        assert(cyc.size() == 2); // {2,2}
        assert(cyc[0] == 2 && cyc[1] == 2);
    }

    // Test 4: Negative cycle in disconnected component
    {
        std::vector<Edge> edges = {{0, 0, 1}, {3, 3, -1}}; // self-loop at vertex 4 (0-based) -1
        std::vector<int> cyc = findNegativeCycle(4, edges);
        assert(cyc.size() == 2);
        assert(cyc[0] == 4 && cyc[1] == 4);
    }

    // Test 5: Negative cycle with mixed weights and longer path
    {
        std::vector<Edge> edges = {{0, 1, 1}, {1, 2, -2}, {2, 0, 0}}; // total -1
        std::vector<int> cyc = findNegativeCycle(3, edges);
        assert(!cyc.empty());
        assert(cyc.size() == 4); // e.g., {1,2,3,1}
        assert(cyc.front() == cyc.back());
    }

    // Test 6: Complex graph with negative cycle not reachable from 0 but reachable from somewhere
    {
        std::vector<Edge> edges = {{0,1,10}, {1,2,10}, {3,4,-1}, {4,3,-1}};
        std::vector<int> cyc = findNegativeCycle(5, edges);
        assert(cyc.size() == 3);
        assert(cyc[0] == 4 && cyc[1] == 5 && cyc[2] == 4 || cyc[0] == 5 && cyc[1] == 4 && cyc[2] == 5);
    }

    // Test 7: Large weights but still within long long
    {
        std::vector<Edge> edges = {{0,1,1000000000LL}, {1,0,-1000000001LL}};
        std::vector<int> cyc = findNegativeCycle(2, edges);
        assert(cyc.size() == 3);
        assert(cyc.front() == cyc.back());
    }

    // Test 8: Graph with only positive weights and no cycle at all (DAG)
    {
        std::vector<Edge> edges = {{0,1,5}, {0,2,3}, {1,3,2}, {2,3,1}};
        std::vector<int> cyc = findNegativeCycle(4, edges);
        assert(cyc.empty());
    }

    // Test 9: Single vertex with no edges
    {
        std::vector<Edge> edges;
        std::vector<int> cyc = findNegativeCycle(1, edges);
        assert(cyc.empty());
    }

    // Test 10: Negative cycle that uses all vertices
    {
        std::vector<Edge> edges = {{0,1,-1}, {1,2,-1}, {2,3,-1}, {3,0,-1}};
        std::vector<int> cyc = findNegativeCycle(4, edges);
        assert(cyc.size() == 5);
        assert(cyc.front() == cyc.back());
        // Must contain all four vertices
        std::vector<int> sorted = cyc;
        sorted.pop_back(); // remove repeated last
        std::sort(sorted.begin(), sorted.end());
        assert(sorted == std::vector<int>({1,2,3,4}));
    }

    return 0;
}
#include <vector>
#include <algorithm>
#include <limits>

struct Edge {
    int a, b;
    long long cost;
};

// Returns a negative cycle as a vector of 1-based vertex labels (starting and ending at the same vertex).
// If no negative cycle exists, returns an empty vector.
std::vector<int> findNegativeCycle(int n, const std::vector<Edge>& edges) {
    const long long INF = std::numeric_limits<long long>::max() / 4;
    std::vector<long long> d(n, 0); // initialize all distances to 0 to find any cycle
    std::vector<int> p(n, -1);
    int x = -1;

    // Relax all edges n times; the n-th relaxation detects a negative cycle.
    for (int i = 0; i < n; ++i) {
        x = -1;
        for (const Edge& e : edges) {
            if (d[e.a] + e.cost < d[e.b]) {
                d[e.b] = d[e.a] + e.cost;
                p[e.b] = e.a;
                x = e.b;
            }
        }
    }

    if (x == -1) {
        return {}; // No negative cycle
    }

    // Step back n times to ensure x is inside the cycle.
    for (int i = 0; i < n; ++i) {
        x = p[x];
    }

    // Collect the cycle vertices.
    std::vector<int> cycle;
    for (int v = x;; v = p[v]) {
        cycle.push_back(v);
        // Stop when we return to the start (after at least one edge)
        if (v == x && cycle.size() > 1) {
            break;
        }
    }
    std::reverse(cycle.begin(), cycle.end());

    // Convert to 1-based labels for output.
    std::vector<int> result;
    result.reserve(cycle.size());
    for (int v : cycle) {
        result.push_back(v + 1);
    }
    return result;
}
// The task is to detect a negative cycle in a directed weighted graph. The classic approach uses the Bellman-Ford algorithm, which after `n-1` relaxations will still be able to relax an edge in the `n`th iteration if and only if a negative cycle exists. We initialize distances to zero for all vertices (instead of infinity) so that we can find a negative cycle even if it is not reachable from a specific source; this works because starting all distances at zero guarantees that any negative cycle will cause a relaxation in the `n`th pass. We maintain a predecessor array `p` to reconstruct the cycle once a relaxation is detected in the `n`th iteration. After detecting a vertex `x` that was relaxed in the `n`th pass, we walk back `n` steps via predecessors to ensure we are inside the cycle (not just a path leading to it), then collect the cycle vertices until we return to the starting point. We must reverse the collected path to obtain correct order. Time complexity is O(n·m), and space complexity is O(n). Edge cases include: 1) no negative cycle → return empty vector; 2) self-loop with negative weight → cycle of length 1; 3) disconnected graph → still works due to zero initialization; 4) multiple cycles → any is acceptable; 5) weights may exceed int range so use `long long`. Output vertex labels are 1-based in the returned vector, matching the snippet’s output convention.
