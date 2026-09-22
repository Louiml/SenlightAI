// Write a C++ function that takes a graph represented as an adjacency list with integer edge weights and two vertex IDs, and removes all edges that connect the two specified vertices. The function should mutate the graph in place. Use the Boost Graph Library (`boost::adjacency_list`) with the same graph type as in the example: `adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_weight_t, int>>`. The function should be named `removeEdgesBetween` and accept the graph by reference, plus the two vertex IDs as parameters. After removal, the graph's vertices and remaining edges (with their original weights) should be intact. The function should correctly handle cases where there are multiple parallel edges between the two vertices (remove all of them), and it should do nothing if the vertices don't exist (out-of-range IDs → just ignore). It must not modify any edge weights and must not remove edges that involve other vertices. Assume the graph is undirected, so an edge `{u,v}` is considered connecting `u` and `v` regardless of direction. The function should be efficient for graphs with many edges and should not rely on copying the graph.
// The solution leverages Boost's `remove_out_edge_if` algorithm combined with a predicate that checks whether the target of an outgoing edge matches the second vertex. Since the graph is undirected, removing edges from vertex `u` that connect to `v` covers all edges between them, because in undirected adjacency lists, each edge appears as an outgoing edge from both endpoints. However, `remove_out_edge_if` only removes edges from the specified vertex's out-edge list. To ensure that all edges between `u` and `v` are removed from both sides, we must call `remove_out_edge_if` twice: once on `u` targeting `v`, and once on `v` targeting `u`. This handles parallel edges as well—each removal pass deletes every matching edge from that vertex's list. Edge cases: if `u == v`, we must not accidentally remove all self-loops? The function should remove all self-loops at that vertex (edges connecting `u` to itself) because they also connect the specified vertices. The predicate uses `target(e, g) == v` for the first pass and `target(e, g) == u` for the second. If vertex IDs are out of range (`u >= num_vertices(g)` or `v >= num_vertices(g)`), the function can simply return without doing anything. Time complexity is O(deg(u) + deg(v)) because we iterate over the out-edge lists of the two vertices. Space complexity is O(1) auxiliary. The function modifies the graph in place and preserves edge weights because removal only deletes edges without touching the weight property map.
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                                    boost::no_property,
                                    boost::property<boost::edge_weight_t, int>>;

// Remove all edges connecting vertex u and vertex v from the graph.
// The graph is undirected; all parallel edges between them are removed.
// If the vertices do not exist, the function does nothing.
void removeEdgesBetween(Graph& g, int u, int v) {
    // Ensure vertices are valid
    if (u < 0 || v < 0 || 
        static_cast<std::size_t>(u) >= boost::num_vertices(g) ||
        static_cast<std::size_t>(v) >= boost::num_vertices(g)) {
        return;
    }

    // Remove edges from u to v (covers all edges from u's perspective)
    boost::remove_out_edge_if(u, 
                              [&](const boost::graph_traits<Graph>::edge_descriptor& e) {
                                  return boost::target(e, g) == v;
                              },
                              g);

    // Remove edges from v to u (covers all edges from v's perspective, including parallel ones)
    boost::remove_out_edge_if(v,
                              [&](const boost::graph_traits<Graph>::edge_descriptor& e) {
                                  return boost::target(e, g) == u;
                              },
                              g);
}
#include <cassert>
#include <iostream>
#include <boost/graph/graph_utility.hpp>

// The function under test is assumed to be defined above.
// For testing, we include the function definition here or in a header.
// (In a real scenario, include the header; here we just declare.)

int main() {
    // Test 1: basic removal of a single edge
    {
        Graph g(3);
        boost::add_edge(0, 1, g);
        boost::add_edge(1, 2, g);
        removeEdgesBetween(g, 0, 1);
        assert(boost::num_edges(g) == 1);
        // The remaining edge should be (1,2)
    }

    // Test 2: remove all parallel edges
    {
        Graph g(2);
        boost::add_edge(0, 1, g);
        boost::add_edge(0, 1, g);
        boost::add_edge(0, 1, g);
        removeEdgesBetween(g, 0, 1);
        assert(boost::num_edges(g) == 0);
    }

    // Test 3: self-loop removal
    {
        Graph g(2);
        boost::add_edge(0, 0, g);
        boost::add_edge(0, 1, g);
        removeEdgesBetween(g, 0, 0);
        assert(boost::num_edges(g) == 1); // only (0,1) remains
    }

    // Test 4: invalid vertex IDs do nothing
    {
        Graph g(2);
        boost::add_edge(0, 1, g);
        removeEdgesBetween(g, 5, 0);
        assert(boost::num_edges(g) == 1);
        removeEdgesBetween(g, 0, -1);
        assert(boost::num_edges(g) == 1);
    }

    // Test 5: no edges between vertices
    {
        Graph g(3);
        boost::add_edge(0, 1, g);
        boost::add_edge(2, 1, g);
        removeEdgesBetween(g, 0, 2);
        assert(boost::num_edges(g) == 2);
    }

    // Test 6: weights are preserved on remaining edges
    {
        Graph g(3);
        auto e1 = boost::add_edge(0, 1, g).first;
        auto e2 = boost::add_edge(1, 2, g).first;
        boost::property_map<Graph, boost::edge_weight_t>::type weight = boost::get(boost::edge_weight, g);
        weight[e1] = 42;
        weight[e2] = 7;
        removeEdgesBetween(g, 0, 1);
        assert(boost::num_edges(g) == 1);
        // The remaining edge should be (1,2) with weight 7
        auto edges = boost::edges(g);
        auto it = edges.first;
        assert(boost::source(*it, g) == 1 && boost::target(*it, g) == 2);
        assert(weight[*it] == 7);
    }

    std::cout << "All tests passed.\n";
    return 0;
}
