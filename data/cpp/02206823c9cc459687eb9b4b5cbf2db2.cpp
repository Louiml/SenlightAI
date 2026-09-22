/*
Write a C++ function `minimumSpanningTree(int vertices, const vector<tuple<int,int,int>>& edges, int start)` that takes the number of vertices (0-indexed), a vector of undirected edges as tuples `(weight, u, v)`, and a starting vertex index. The function must return a pair: first, a `vector<pair<int,int>>` containing the edges of a Minimum Spanning Tree (MST) found using Prim's algorithm starting from the given source, and second, the total MST weight. Assume the graph is connected and edge weights are distinct (no equal weights). The function must output in the returned vector the MST edges in the order they were added, using the notation `(u,v)` where `u` is the node already in the tree and `v` is the newly added node. Handle the case where the graph has exactly one vertex (MST has no edges, weight 0).
*/
#include <vector>
#include <tuple>
#include <set>
#include <utility>
#include <climits>
#include <algorithm>

// Compute the Minimum Spanning Tree using Prim's algorithm.
// vertices: number of vertices (0-indexed)
// edges: vector of (weight, u, v) for undirected edges
// start: starting vertex
// Returns: pair of (MST edges as (u,v) pairs, total weight)
std::pair<std::vector<std::pair<int,int>>, int> minimumSpanningTree(
    int vertices,
    const std::vector<std::tuple<int,int,int>>& edges,
    int start)
{
    // Build adjacency list: graph[u] = list of (neighbor, weight)
    std::vector<std::vector<std::pair<int,int>>> graph(vertices);
    for (const auto& [weight, u, v] : edges) {
        graph[u].push_back({v, weight});
        graph[v].push_back({u, weight});
    }

    std::set<int> visited;
    std::vector<std::pair<int,int>> mst_edges;
    int total_weight = 0;

    visited.insert(start);

    // If only one vertex, MST has no edges
    if (vertices == 1) {
        return {mst_edges, 0};
    }

    while (mst_edges.size() < static_cast<size_t>(vertices - 1) && 
           visited.size() < static_cast<size_t>(vertices)) {
        int min_weight = INT32_MAX;
        int min_u = -1, min_v = -1;

        // Scan all visited nodes and their neighbors
        for (int node : visited) {
            for (const auto& [neighbor, weight] : graph[node]) {
                if (weight < min_weight && 
                    visited.find(neighbor) == visited.end()) {
                    min_weight = weight;
                    min_u = node;
                    min_v = neighbor;
                }
            }
        }

        // Add the found edge to MST
        visited.insert(min_u);
        visited.insert(min_v);
        mst_edges.push_back({min_u, min_v});
        total_weight += min_weight;
    }

    return {mst_edges, total_weight};
}
#include <cassert>
#include <vector>
#include <tuple>
#include <utility>

// Function declared here for testing (included from solution)
// Assume the solution code above is included before main.

int main() {
    // Simple triangle graph: 0-1 weight 1, 0-2 weight 3, 1-2 weight 2
    std::vector<std::tuple<int,int,int>> edges1 = {{1,0,1},{3,0,2},{2,1,2}};
    auto res1 = minimumSpanningTree(3, edges1, 0);
    assert(res1.second == 3);
    assert(res1.first.size() == 2);
    assert((res1.first[0] == std::make_pair(0,1)));
    assert((res1.first[1] == std::make_pair(0,2) || res1.first[1] == std::make_pair(1,2)));

    // Single vertex
    std::vector<std::tuple<int,int,int>> edges2;
    auto res2 = minimumSpanningTree(1, edges2, 0);
    assert(res2.first.empty());
    assert(res2.second == 0);

    // Line graph: 0-1 weight 5, 1-2 weight 4, 2-3 weight 3
    std::vector<std::tuple<int,int,int>> edges3 = {{5,0,1},{4,1,2},{3,2,3}};
    auto res3 = minimumSpanningTree(4, edges3, 2);
    assert(res3.second == 12);
    assert(res3.first.size() == 3);
    // Starting from 2, first edge must be (2,3) weight 3
    assert((res3.first[0] == std::make_pair(2,3)));

    // Star graph: center 0 connected to 1,2,3 with weights 10,20,30
    std::vector<std::tuple<int,int,int>> edges4 = {{10,0,1},{20,0,2},{30,0,3}};
    auto res4 = minimumSpanningTree(4, edges4, 1);
    assert(res4.second == 60);
    assert(res4.first.size() == 3);
    // Starting from 1, first edge must be (1,0) weight 10
    assert((res4.first[0] == std::make_pair(1,0)));

    // Graph with 5 vertices and 6 edges, has unique MST
    std::vector<std::tuple<int,int,int>> edges5 = {
        {1,0,1}, {2,1,2}, {3,2,3}, {4,3,4}, {5,4,0}, {6,0,2}
    };
    auto res5 = minimumSpanningTree(5, edges5, 1);
    // MST edges: only weights 1,2,3,4 (total 10)
    assert(res5.second == 10);
    assert(res5.first.size() == 4);
    // The edge (1,2) weight 2 and (0,1) weight 1 must be in there
    bool has01 = false, has12 = false;
    for (auto& e : res5.first) {
        if ((e.first==0&&e.second==1)||(e.first==1&&e.second==0)) has01 = true;
        if ((e.first==1&&e.second==2)||(e.first==2&&e.second==1)) has12 = true;
    }
    assert(has01 && has12);

    return 0;
}
// The solution uses Prim's algorithm, which grows the MST one vertex at a time from an arbitrary starting vertex. Maintain a set of visited vertices (initially containing `start`). In each iteration, scan all visited vertices and their adjacency lists to find the minimum-weight edge that connects a visited vertex to an unvisited one. Add that edge to the MST list, and insert both endpoints into the visited set. Repeat until either the MST has `vertices-1` edges or all vertices are visited. Since the graph is connected and weights are distinct, this greedy choice always yields the MST. Edge cases: if `vertices==1`, the loop never runs and the MST list is empty with total weight 0. If the graph is disconnected (not possible per assumption), the loop would stop early; we assume connected input. Time complexity: The naive scanning over visited nodes and their adjacency lists in each iteration leads to O(V * E) in the worst case, with O(V) space for visited set and MST vector. This matches the given snippet's approach. For better performance one could use a priority queue, but the task expects the simple approach consistent with the code snippet.
