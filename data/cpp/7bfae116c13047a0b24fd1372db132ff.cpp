/*
Write a C++ function `bool isTopologicalOrder(const std::vector<std::vector<int>>& graph, const std::vector<int>& candidate)` that takes a directed graph represented as an adjacency list (where vertices are numbered 0 to graph.size()-1) and a candidate topological ordering (a permutation of all vertices). The function must return `true` if the candidate is a valid topological ordering of the graph (i.e., for every directed edge u→v, u appears before v in the candidate), and `false` otherwise. You may assume the graph has no self-loops and the candidate contains exactly all vertices exactly once. The function must not modify the input and must handle an empty graph (size 0) gracefully by returning `true` for an empty candidate.
*/

#include <vector>
#include <algorithm> // optional, not required

// Returns true if `candidate` is a valid topological ordering of the directed graph.
bool isTopologicalOrder(const std::vector<std::vector<int>>& graph, const std::vector<int>& candidate) {
    const size_t n = graph.size();
    
    // Handle empty graph
    if (n == 0) {
        return candidate.empty();
    }
    
    // Build position map: position[vertex] = index in candidate
    std::vector<int> position(n, -1);
    for (size_t i = 0; i < candidate.size(); ++i) {
        const int vertex = candidate[i];
        // Basic validation: vertex in range and not duplicate (though problem says it's a permutation)
        if (vertex < 0 || static_cast<size_t>(vertex) >= n) return false;
        position[vertex] = static_cast<int>(i);
    }
    
    // Check every edge: u must appear before v
    for (size_t u = 0; u < n; ++u) {
        const int posU = position[u];
        if (posU == -1) return false; // missing vertex
        for (const int v : graph[u]) {
            const int posV = position[v];
            if (posV == -1) return false; // missing vertex
            if (posU >= posV) {
                return false;
            }
        }
    }
    return true;
}

#include <cassert>
#include <vector>

// Include the solution function here or via header
int main() {
    // Test 1: Simple DAG (0->1, 0->2, 1->2)
    {
        std::vector<std::vector<int>> graph = {{1,2}, {2}, {}};
        assert(isTopologicalOrder(graph, {0,1,2}) == true);
        assert(isTopologicalOrder(graph, {0,2,1}) == false); // edge 1->2 violated
        assert(isTopologicalOrder(graph, {1,0,2}) == false); // edge 0->1 violated
    }
    // Test 2: Cycle (0->1, 1->0)
    {
        std::vector<std::vector<int>> graph = {{1}, {0}};
        assert(isTopologicalOrder(graph, {0,1}) == false);
        assert(isTopologicalOrder(graph, {1,0}) == false);
    }
    // Test 3: Single vertex
    {
        std::vector<std::vector<int>> graph = {{}};
        assert(isTopologicalOrder(graph, {0}) == true);
        assert(isTopologicalOrder(graph, {}) == false);
    }
    // Test 4: Empty graph
    {
        std::vector<std::vector<int>> graph = {};
        assert(isTopologicalOrder(graph, {}) == true);
        assert(isTopologicalOrder(graph, {0}) == false);
    }
    // Test 5: Disconnected graph (0->1, 2->3)
    {
        std::vector<std::vector<int>> graph = {{1}, {}, {3}, {}};
        assert(isTopologicalOrder(graph, {0,2,1,3}) == true);
        assert(isTopologicalOrder(graph, {2,0,3,1}) == true);
        assert(isTopologicalOrder(graph, {0,1,3,2}) == false); // edge 2->3 violated
    }
    // Test 6: Multi-edge (duplicate edges) 
    {
        std::vector<std::vector<int>> graph = {{1,1}, {}};
        assert(isTopologicalOrder(graph, {0,1}) == true);
        assert(isTopologicalOrder(graph, {1,0}) == false);
    }
    // Test 7: Linear chain 0->1->2->3
    {
        std::vector<std::vector<int>> graph = {{1}, {2}, {3}, {}};
        assert(isTopologicalOrder(graph, {0,1,2,3}) == true);
        assert(isTopologicalOrder(graph, {0,2,1,3}) == false);
        assert(isTopologicalOrder(graph, {3,2,1,0}) == false);
    }
    
    return 0;
}

// The solution is straightforward: verify that for every directed edge `(u, v)` in the graph, the position of `u` in the candidate list is less than the position of `v`. To achieve this in O(1) per edge, build a position map (or vector) that stores the index of each vertex in the candidate. Then iterate through all vertices and their adjacency lists, checking the positional constraint. Edge cases: an empty graph and empty candidate (returns true), a graph with a single vertex (must be valid if candidate is [0]), and a graph with cycles (the function will catch violations because at least one edge will be reversed). Time complexity is O(V + E), where V is the number of vertices and E is the total number of edges, for building the position map and scanning all edges. Space complexity is O(V) for the position vector.
