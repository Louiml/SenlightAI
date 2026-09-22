Given an undirected graph where vertices are identified by positive integers and edges are provided as pairs of integers, write a C++ function that performs a BFS-based bipartite coloring starting from a specified source vertex (assumed to be 1) and returns the maximum count between the two color groups (black and white) reachable from that source. The graph is not necessarily connected; only vertices reachable from the source are considered. The function should handle up to 100,000 vertices and an arbitrary number of edges (provided in a separate container). The input to the function should be a vector of pairs representing edges, and it should ignore any vertex IDs beyond the given range. The coloring must be valid (no edge connects same-colored vertices) for the reachable component, but the graph is guaranteed to be a collection of trees or bipartite components, so no conflict detection is needed. The function should return 0 if the source vertex does not exist in the edge list.
// The approach is to build an adjacency list from the given edge pairs, then run a BFS starting from the source vertex (1). During BFS, we assign colors 0 (black) and 1 (white) alternately. We maintain counts of black and white vertices visited. Since the graph is guaranteed bipartite, we never need to check for conflicts. The BFS only visits vertices reachable from the source, so isolated components are ignored. Important edge cases: the source may not appear in any edge (then no vertices are reachable, return 0); there may be duplicate edges (we can ignore them by using a set or just pushing duplicates, but they won't affect counts because visited check prevents reprocessing); vertices may be numbered arbitrarily large, but we can limit adjacency list size to the maximum vertex ID present plus one, or use a vector of vectors sized to 100001 as in the snippet. To be robust, we should find the maximum vertex ID from the edges and allocate adjacency list accordingly. Time complexity is O(V_effective + E) where V_effective is the number of vertices reachable and E is the number of edges in the component. Space complexity is O(V_effective) for the adjacency list and color array.
#include <vector>
#include <queue>
#include <algorithm>

// Given a list of edges (undirected) and a source vertex, returns the maximum 
// number of vertices in one part of the bipartite coloring of the connected 
// component containing the source. If source is not present, returns 0.
int maxBipartiteGroupSize(int source, const std::vector<std::pair<int,int>>& edges) {
    // Determine the maximum vertex id to size the adjacency list.
    int maxId = source;
    for (const auto& e : edges) {
        maxId = std::max(maxId, std::max(e.first, e.second));
    }
    if (maxId == 0) return 0;

    std::vector<std::vector<int>> adj(maxId + 1);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    std::vector<int> color(maxId + 1, -1);
    std::queue<int> q;
    q.push(source);
    color[source] = 0;
    int black = 1, white = 0;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (color[v] == -1) {
                color[v] = 1 - color[u];
                if (color[v] == 1) ++white;
                else ++black;
                q.push(v);
            }
        }
    }

    return std::max(black, white);
}
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Basic connected component with 4 vertices, path: 1-2-3-4
    std::vector<std::pair<int,int>> e1 = {{1,2},{2,3},{3,4}};
    assert(maxBipartiteGroupSize(1, e1) == 2);

    // Star centered at 1 with 3 leaves
    std::vector<std::pair<int,int>> e2 = {{1,2},{1,3},{1,4}};
    assert(maxBipartiteGroupSize(1, e2) == 3);

    // Source not present in edges
    assert(maxBipartiteGroupSize(5, {{1,2}}) == 0);

    // Disconnected graph, only component containing 1 matters
    std::vector<std::pair<int,int>> e3 = {{1,2},{3,4},{3,5}};
    assert(maxBipartiteGroupSize(1, e3) == 1);

    // Single edge, two vertices
    assert(maxBipartiteGroupSize(1, {{1,2}}) == 1);

    // Large vertex IDs, many edges
    std::vector<std::pair<int,int>> e4;
    for (int i = 1; i <= 1000; ++i) {
        e4.push_back({i, i+1});
    }
    assert(maxBipartiteGroupSize(1, e4) == 500);
}
