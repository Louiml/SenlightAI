// Given a weighted graph represented as an adjacency list where vertices are identified by integers and edges have double weights, write a C++ function `shortestPathWithChecks` that takes the graph (as a `std::unordered_map<int, std::unordered_map<int, double>>` for outgoing edges), the total number of vertices, and two vertex IDs (start and end), and returns the shortest path distance as a `double`. If there is no path between start and end, return `-1.0`. The graph may have zero-weight edges, self-loops, and multiple edges between the same pair of vertices (the latter should be handled by keeping the minimum weight). The function must handle the case where start equals end (distance = 0), and must not rely on any external priority queue implementation beyond the C++ standard library.
// The solution uses Dijkstra's algorithm with a priority queue to compute the shortest distance from the start vertex to all others, stopping early when the end vertex is finalized. The adjacency list stores only outgoing edges; for each vertex, we initialize its distance to infinity (or a very large value) and set the start’s distance to 0. We push `(0, start)` into a min-heap (using `std::priority_queue` with a custom comparator for pairs of `(distance, vertex)`). While the heap is not empty, we pop the pair with the smallest distance. If that vertex is the end, we break and return its distance. Otherwise, we iterate over all neighbors of the current vertex; for each neighbor, we compute a candidate distance as current distance plus edge weight. If the candidate is smaller than the stored distance, we update and push `(candidate, neighbor)`. Because we use a standard priority queue that does not support decrease-key, we allow duplicate entries; we skip a popped entry if its stored distance is greater than the current known distance for that vertex. Edge cases: if start or end IDs are not present in the graph, or if they are present but no path exists, we return `-1.0`. Self-loops are processed but never improve the distance. For multiple edges between the same vertices, we either keep the minimum during construction (if we control that) or handle by checking all edges; but the function itself just uses whatever weights are in the adjacency list, and Dijkstra will automatically consider the smallest because we update distances. Time complexity is O((V + E) log V) due to each edge potentially causing a heap push, with V vertices and E edges. Space complexity is O(V) for the distance map and O(V) for the heap in the worst case.
#include <unordered_map>
#include <queue>
#include <vector>
#include <limits>
#include <utility>

// Compute shortest path distance from start to end in a weighted directed graph.
// Graph is stored as adjacency list: vertex -> (neighbor, weight).
// If no path exists or either vertex is missing, return -1.0.
double shortestPathWithChecks(
    const std::unordered_map<int, std::unordered_map<int, double>>& graph,
    int start,
    int end
) {
    // If start or end not in graph, return -1
    if (graph.find(start) == graph.end() || graph.find(end) == graph.end()) {
        return -1.0;
    }

    // Distance map initialized to infinity
    std::unordered_map<int, double> dist;
    for (const auto& [vertex, _] : graph) {
        dist[vertex] = std::numeric_limits<double>::infinity();
    }
    dist[start] = 0.0;

    // Min-heap of (distance, vertex). We use a custom comparator.
    using Pair = std::pair<double, int>;
    auto cmp = [](const Pair& a, const Pair& b) { return a.first > b.first; };
    std::priority_queue<Pair, std::vector<Pair>, decltype(cmp)> pq(cmp);
    pq.push({0.0, start});

    while (!pq.empty()) {
        auto [currentDist, current] = pq.top();
        pq.pop();

        // If this entry is outdated, skip
        if (currentDist > dist[current]) continue;

        // Early exit when we pop the destination
        if (current == end) {
            return currentDist;
        }

        // Relax edges
        auto it = graph.find(current);
        if (it != graph.end()) {
            for (const auto& [neighbor, weight] : it->second) {
                double newDist = currentDist + weight;
                if (newDist < dist[neighbor]) {
                    dist[neighbor] = newDist;
                    pq.push({newDist, neighbor});
                }
            }
        }
    }

    // If loop ends without reaching end, no path
    return -1.0;
}
#include <cassert>
#include <unordered_map>

int main() {
    // Build a simple graph: 0 -> 1 (1.0), 1 -> 2 (2.0), 0 -> 2 (5.0)
    std::unordered_map<int, std::unordered_map<int, double>> g1;
    g1[0][1] = 1.0;
    g1[1][2] = 2.0;
    g1[0][2] = 5.0;
    assert(shortestPathWithChecks(g1, 0, 2) == 3.0);
    assert(shortestPathWithChecks(g1, 0, 0) == 0.0);
    
    // No path: vertex 3 unreachable
    assert(shortestPathWithChecks(g1, 0, 3) == -1.0);
    
    // Missing vertex in graph
    assert(shortestPathWithChecks(g1, 5, 6) == -1.0);
    
    // Graph with self-loop and multiple edges
    std::unordered_map<int, std::unordered_map<int, double>> g2;
    g2[0][0] = 10.0; // self-loop
    g2[0][1] = 3.0;
    g2[0][1] = 1.0; // override with smaller weight
    g2[1][2] = 1.0;
    assert(shortestPathWithChecks(g2, 0, 2) == 2.0); // 0->1 (1) + 1->2 (1)
    
    // Graph where start==end but only one vertex
    std::unordered_map<int, std::unordered_map<int, double>> g3;
    g3[7][8] = 2.0;
    assert(shortestPathWithChecks(g3, 7, 7) == 0.0);
    
    // Zero-weight edges
    std::unordered_map<int, std::unordered_map<int, double>> g4;
    g4[0][1] = 0.0;
    g4[1][2] = 0.0;
    assert(shortestPathWithChecks(g4, 0, 2) == 0.0);
    
    // Disconnected components, but both vertices exist
    std::unordered_map<int, std::unordered_map<int, double>> g5;
    g5[0][1] = 1.0;
    g5[2][3] = 1.0;
    assert(shortestPathWithChecks(g5, 0, 2) == -1.0);
}
