// Write a C++ function that, given a directed graph represented as an adjacency list with vertex name properties and edge weight properties, computes the shortest-path tree from a specified source vertex using Dijkstra's algorithm and returns a map from each reachable vertex name to its next-hop vertex name along the shortest path to the source. The graph is provided as a `std::map<std::string, std::map<std::string, int>>` where the outer key is the source vertex name, the inner key is the destination vertex name, and the value is the edge weight. The function should take the graph and the source vertex name as inputs, and return a `std::map<std::string, std::string>` where each key is a reachable vertex name (excluding the source) and each value is the name of the immediate next hop from that vertex toward the source on the shortest path. If the source vertex does not exist in the graph, return an empty map. Edge weights are non-negative integers. If multiple shortest paths exist, any valid next-hop choice is acceptable. The solution must be self-contained, using only standard C++ libraries, and must not rely on external graph libraries.

// The core algorithm is Dijkstra's shortest path algorithm applied from the source vertex to all other vertices. Since the goal is to find the next hop toward the source, we run Dijkstra from the source and track predecessor information for each vertex: the predecessor is the vertex that comes immediately before the current vertex on the shortest path from the source. After computing predecessors, for each vertex `v` (not the source), we need to find the first vertex after `v` on the path back to the source. This can be done by repeatedly following the predecessor chain from `v` until we reach a vertex whose predecessor is the source; that vertex is the next hop. An efficient way is to store the direct next hop during the relaxation step: when we update the distance to a neighbor `u` via vertex `v`, we set `next_hop[u] = next_hop[v]` if `v` is the source, otherwise `next_hop[u] = next_hop[v]` (since the path from source to `v` to `u` shares the next hop of `v`). This way, we only need one pass. Edge cases: source not present (return empty map), graph empty (empty map), unreachable vertices (excluded from output), zero-weight edges (need strict inequality when updating to preserve predecessor correctness but with zero-weight edges multiple paths may exist; we update if new distance is strictly less, or if equal we can choose either, but to keep deterministic, we update only on strict less). Time complexity is O((V+E) log V) using a priority queue, where V is number of vertices and E is number of edges. Space complexity is O(V) for the distance, predecessor/next-hop, and visited arrays.

#include <map>
#include <string>
#include <vector>
#include <queue>
#include <limits>
#include <stdexcept>

// Compute next-hop map for shortest paths from a source to all reachable vertices.
// Returns empty map if source not found.
std::map<std::string, std::string> shortestPathNextHops(
    const std::map<std::string, std::map<std::string, int>>& graph,
    const std::string& source) {
    
    // Check if source exists
    if (graph.find(source) == graph.end()) return {};
    
    // Collect all vertex names
    std::vector<std::string> vertices;
    for (const auto& [v, neighbors] : graph) {
        vertices.push_back(v);
        for (const auto& [u, w] : neighbors) {
            // Ensure destination is in vertex list (if not, add it)
            if (graph.find(u) == graph.end()) {
                // Graph may have missing sink vertices; add them as empty adjacency
                // But to keep consistent, we don't mutate original; handle in Dijkstra.
            }
        }
    }
    // To handle all possible vertices including those appearing only as destinations,
    // we collect from both keys and all neighbor keys.
    std::vector<std::string> all_vertices;
    std::map<std::string, int> index_map;
    for (const auto& [v, neighbors] : graph) {
        if (index_map.find(v) == index_map.end()) {
            index_map[v] = all_vertices.size();
            all_vertices.push_back(v);
        }
        for (const auto& [u, w] : neighbors) {
            if (index_map.find(u) == index_map.end()) {
                index_map[u] = all_vertices.size();
                all_vertices.push_back(u);
            }
        }
    }
    
    int n = all_vertices.size();
    int src_idx = index_map[source];
    
    const int INF = std::numeric_limits<int>::max();
    std::vector<int> dist(n, INF);
    std::vector<int> next_hop(n, -1); // -1 means none
    std::vector<bool> visited(n, false);
    
    // Min-heap: (distance, vertex_index)
    using P = std::pair<int, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    dist[src_idx] = 0;
    pq.push({0, src_idx});
    
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();
        if (visited[v]) continue;
        visited[v] = true;
        
        const std::string& v_name = all_vertices[v];
        // Check if v_name has outgoing edges in the graph
        auto it = graph.find(v_name);
        if (it == graph.end()) continue; // no outgoing edges
        for (const auto& [u_name, w] : it->second) {
            int u = index_map[u_name];
            if (visited[u]) continue;
            int new_dist = d + w;
            if (new_dist < dist[u]) {
                dist[u] = new_dist;
                // set next hop from u toward source
                if (v == src_idx) {
                    next_hop[u] = u; // reachable directly from source; next hop is u itself
                } else {
                    next_hop[u] = next_hop[v]; // same next hop as predecessor
                }
                pq.push({new_dist, u});
            }
        }
    }
    
    // Build result map for all reachable vertices (dist < INF) except source itself
    std::map<std::string, std::string> result;
    for (int i = 0; i < n; ++i) {
        if (i != src_idx && dist[i] < INF) {
            const std::string& u_name = all_vertices[i];
            if (next_hop[i] == -1) continue; // shouldn't happen if reachable
            result[u_name] = all_vertices[next_hop[i]];
        }
    }
    return result;
}

#include <cassert>
#include <map>
#include <string>

// The function is declared above; include or re-declare here for clarity.
std::map<std::string, std::string> shortestPathNextHops(
    const std::map<std::string, std::map<std::string, int>>& graph,
    const std::string& source);

int main() {
    // Test 1: Simple graph A->B->C
    std::map<std::string, std::map<std::string, int>> g1 = {
        {"A", {{"B", 1}}},
        {"B", {{"C", 2}}},
        {"C", {}}
    };
    auto r1 = shortestPathNextHops(g1, "A");
    assert(r1.size() == 2);
    assert(r1["B"] == "B");
    assert(r1["C"] == "B"); // path A->B->C, next hop from C to A is B

    // Test 2: Graph with branching and alternative paths
    std::map<std::string, std::map<std::string, int>> g2 = {
        {"S", {{"A", 1}, {"B", 4}}},
        {"A", {{"B", 1}, {"C", 5}}},
        {"B", {{"C", 2}}},
        {"C", {}}
    };
    auto r2 = shortestPathNextHops(g2, "S");
    // Shortest to A: S->A (cost 1), next hop A
    // Shortest to B: S->A->B (cost 2) or S->B (4), so next hop A
    // Shortest to C: S->A->B->C (cost 1+1+2=4), next hop A
    assert(r2["A"] == "A");
    assert(r2["B"] == "A");
    assert(r2["C"] == "A");

    // Test 3: Source not in graph
    auto r3 = shortestPathNextHops(g2, "X");
    assert(r3.empty());

    // Test 4: Empty graph
    std::map<std::string, std::map<std::string, int>> g4;
    auto r4 = shortestPathNextHops(g4, "A");
    assert(r4.empty());

    // Test 5: Disconnected vertex
    std::map<std::string, std::map<std::string, int>> g5 = {
        {"S", {{"A", 1}}},
        {"A", {}},
        {"B", {}}
    };
    auto r5 = shortestPathNextHops(g5, "S");
    assert(r5.size() == 1);
    assert(r5["A"] == "A");

    // Test 6: Zero-weight edges and multiple paths
    std::map<std::string, std::map<std::string, int>> g6 = {
        {"S", {{"A", 0}, {"B", 1}}},
        {"A", {{"B", 0}}},
        {"B", {}}
    };
    auto r6 = shortestPathNextHops(g6, "S");
    assert(r6.size() == 2);
    // B can be reached via S->A->B with cost 0, next hop A, or S->B cost 1
    assert(r6["A"] == "A");
    assert(r6["B"] == "A"); // shortest path cost 0 via A

    // Test 7: Large weights and multiple hops
    std::map<std::string, std::map<std::string, int>> g7 = {
        {"S", {{"U", 10}, {"V", 5}}},
        {"U", {{"W", 1}}},
        {"V", {{"W", 2}}},
        {"W", {}}
    };
    auto r7 = shortestPathNextHops(g7, "S");
    assert(r7["U"] == "U");
    assert(r7["V"] == "V");
    // Shortest to W: S->V->W cost 7 vs S->U->W cost 11, so next hop V
    assert(r7["W"] == "V");

    return 0;
}
