/*
Write a C++ function that takes a directed graph represented as an adjacency list with `size_t` vertex indices and a vector of directed edges (as pairs `{from, to}`), and returns a new graph where all edges that go from any vertex to a specified "target" vertex have been removed. The function should accept the number of vertices, the list of edges, and the target vertex, and return the resulting graph as a `boost::adjacency_list<vecS, vecS, directedS>`. The original input graph must not be modified.
*/
#include <boost/graph/adjacency_list.hpp>
#include <vector>
#include <cstddef>

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>;

// Remove all edges that point to the given target vertex.
// Returns a new graph with those edges removed; the input edges are not modified.
Graph removeEdgesToTarget(std::size_t vertexCount,
                          const std::vector<std::pair<std::size_t, std::size_t>>& edges,
                          std::size_t target) {
    Graph g(vertexCount);
    for (const auto& e : edges) {
        boost::add_edge(e.first, e.second, g);
    }
    
    // Remove all out-edges from every vertex whose head is the target.
    for (std::size_t v = 0; v < vertexCount; ++v) {
        boost::remove_out_edge_if(v, 
            [target, &g](const Graph::edge_descriptor& ed) {
                return boost::target(ed, g) == target;
            }, g);
    }
    return g;
}
#include <cassert>
#include <vector>
#include <iostream>

// Include the solution function here (or link to it).

int main() {
    // Test 1: simple graph with edges to target 3
    {
        std::vector<std::pair<std::size_t, std::size_t>> edges = {
            {0,3},{0,2},{1,3},{2,0},{3,2}
        };
        Graph g = removeEdgesToTarget(4, edges, 3);
        // After removal, no edge should have target 3.
        for (auto v : boost::make_iterator_range(boost::vertices(g))) {
            for (auto e : boost::make_iterator_range(boost::out_edges(v, g))) {
                assert(boost::target(e, g) != 3);
            }
        }
        // Check that edge counts per vertex are as expected:
        // v0: had (0,3) and (0,2) -> after removal only 0->2 => out-degree 1
        // v1: had (1,3) -> removed => out-degree 0
        // v2: had (2,0) -> unaffected => out-degree 1
        // v3: had (3,2) -> unaffected (target is 2) => out-degree 1
        assert(boost::out_degree(0, g) == 1);
        assert(boost::out_degree(1, g) == 0);
        assert(boost::out_degree(2, g) == 1);
        assert(boost::out_degree(3, g) == 1);
    }

    // Test 2: no edges to target
    {
        std::vector<std::pair<std::size_t, std::size_t>> edges = {
            {0,1},{1,2},{2,0}
        };
        Graph g = removeEdgesToTarget(3, edges, 5); // target beyond vertices
        // Graph still has all 3 edges.
        assert(boost::num_edges(g) == 3);
    }

    // Test 3: multiple edges to same target
    {
        std::vector<std::pair<std::size_t, std::size_t>> edges = {
            {0,2},{0,2},{1,2},{2,2}
        };
        Graph g = removeEdgesToTarget(3, edges, 2);
        // All edges whose head is 2 should be removed, including self-loop 2->2.
        assert(boost::num_edges(g) == 0);
    }

    // Test 4: empty edge list
    {
        std::vector<std::pair<std::size_t, std::size_t>> edges;
        Graph g = removeEdgesToTarget(2, edges, 1);
        assert(boost::num_vertices(g) == 2);
        assert(boost::num_edges(g) == 0);
    }

    // Test 5: target with no incoming edges
    {
        std::vector<std::pair<std::size_t, std::size_t>> edges = {
            {0,1},{1,2}
        };
        Graph g = removeEdgesToTarget(3, edges, 0);
        // Only edge 0->1 and 1->2 remain; 0 has no incoming edges, so unaffected.
        assert(boost::num_edges(g) == 2);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The task requires constructing a graph from a list of edges and then removing all edges that point to a given target vertex. The approach is straightforward: create a graph with the given vertex count, add all input edges to it, then use `remove_out_edge_if` on each vertex that has an outgoing edge to the target. In a directed adjacency list with `vecS` for vertex storage, any vertex with out-edges to the target will have those edges remoced. To achieve this efficiently, iterate over all vertices (from 0 to `num_vertices-1`) and apply `remove_out_edge_if` with a predicate that checks if the target of the edge equals the given target vertex. This predicate can be written as a lambda that captures the target and the graph's edge descriptor. Alternatively, use `remove_edge_if` on the whole graph with an `incident_to` predicate, but that would also remove edges pointing *to* the target from any source, which is exactly what we want. However, `remove_edge_if` with `incident_to(target, g)` is not directly available in the main graph object; we need to use `remove_out_edge_if` per vertex. 
//
// Edge cases: if the target vertex does not exist (index out of range), the function should simply return the original graph unchanged or handle gracefully (we assume the vertex index is valid). If there are no edges to the target, no changes occur. Duplicate edges are fine—removal removes all occurrences. Time complexity is O(V + E) for building the graph and O(V + E) for removal (each edge is examined at most once per removal call). Space complexity is O(V + E) for the output graph.
