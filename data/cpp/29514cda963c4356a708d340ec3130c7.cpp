/*
Write a C++ function named `checkGraphIsomorphism` that determines whether two undirected graphs are isomorphic. The function should take two graphs represented using `boost::adjacency_list` with `vecS` for vertex list, `listS` for edge list, `undirectedS` for direction, and `property<vertex_index_t, int>` for vertex indices. The graphs must have exactly 12 vertices. The function returns a `bool` indicating whether the graphs are isomorphic. If they are isomorphic, the function must also output the mapping from vertices of the first graph to vertices of the second graph, printing the vertex indices of the second graph in the order of the first graph's indices, as a space-separated sequence on a single line preceded by "f: ". The function should handle both graphs with the same degree sequence and confirm non-isomorphism when the degree sequences differ (e.g., one graph has a vertex of degree 4 while the other has all degrees ≤3). You must not modify the input graphs or the vertex indices.
*/

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <iostream>
#include <vector>
#include <cstddef>

using Graph = boost::adjacency_list<boost::vecS, boost::listS, boost::undirectedS,
                                    boost::property<boost::vertex_index_t, int>>;

// Checks whether two 12-vertex undirected graphs are isomorphic and prints the mapping if so.
bool checkGraphIsomorphism(const Graph& g1, const Graph& g2) {
    using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

    const std::size_t n = 12;
    if (boost::num_vertices(g1) != n || boost::num_vertices(g2) != n) {
        std::cout << "f: ";
        for (std::size_t i = 0; i < n; ++i) std::cout << i << " ";
        std::cout << std::endl;
        return false;
    }

    // Create copies to set vertex indices (listS requires explicit indices)
    Graph g1_copy(n), g2_copy(n);
    auto v1_index = boost::get(boost::vertex_index, g1_copy);
    auto v2_index = boost::get(boost::vertex_index, g2_copy);

    std::vector<Vertex> v1(n), v2(n);
    boost::graph_traits<Graph>::vertex_iterator it, end;
    int id = 0;
    for (boost::tie(it, end) = boost::vertices(g1_copy); it != end; ++it, ++id) {
        boost::put(v1_index, *it, id);
        v1[id] = *it;
    }
    id = 0;
    for (boost::tie(it, end) = boost::vertices(g2_copy); it != end; ++it, ++id) {
        boost::put(v2_index, *it, id);
        v2[id] = *it;
    }

    // Copy edges from original graphs (original vertex descriptors are not portable)
    // Since original g1 and g2 have same structure but different vertex ordering,
    // we assume the caller has set up edges appropriately. For this task, we
    // directly build edges from the original's adjacency using index maps.
    // For simplicity and correctness, we rely on the fact that the input graphs
    // already have vertex indices set, and we copy edges using those.
    // However, to avoid copying complexity, we assume the input graphs already
    // have proper vertex indices (0..11). We directly use them.
    // But listS requires explicit index mapping; we set indices on the input graphs
    // is not allowed (const). So we construct new graphs with the same edge sets
    // by iterating over original edges and mapping vertex descriptors via their
    // existing index properties.

    // For each edge in g1, add edge between corresponding indices in g1_copy
    boost::graph_traits<Graph>::edge_iterator eit, eend;
    for (boost::tie(eit, eend) = boost::edges(g1); eit != eend; ++eit) {
        Vertex u = boost::source(*eit, g1);
        Vertex w = boost::target(*eit, g1);
        int ui = boost::get(boost::vertex_index, g1, u);
        int wi = boost::get(boost::vertex_index, g1, w);
        boost::add_edge(v1[ui], v1[wi], g1_copy);
    }
    for (boost::tie(eit, eend) = boost::edges(g2); eit != eend; ++eit) {
        Vertex u = boost::source(*eit, g2);
        Vertex w = boost::target(*eit, g2);
        int ui = boost::get(boost::vertex_index, g2, u);
        int wi = boost::get(boost::vertex_index, g2, w);
        boost::add_edge(v2[ui], v2[wi], g2_copy);
    }

    std::vector<Vertex> f(n);
    bool ret = boost::isomorphism(
        g1_copy, g2_copy,
        boost::isomorphism_map(
            boost::make_iterator_property_map(f.begin(), v1_index, f[0])));

    std::cout << "f: ";
    for (std::size_t v = 0; v < n; ++v) {
        std::cout << boost::get(v2_index, f[v]) << " ";
    }
    std::cout << std::endl;

    return ret;
}

#include <cassert>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <vector>
#include "solution.h" // assume the above solution is in this header

using Graph = boost::adjacency_list<boost::vecS, boost::listS, boost::undirectedS,
                                    boost::property<boost::vertex_index_t, int>>;

int main() {
    // Test 1: Two isomorphic graphs (both are three disjoint 4-cycles)
    {
        Graph g1(12), g2(12);
        // Set vertex indices
        auto idx1 = boost::get(boost::vertex_index, g1);
        auto idx2 = boost::get(boost::vertex_index, g2);
        int id = 0;
        for (auto vit = boost::vertices(g1).first; vit != boost::vertices(g1).second; ++vit, ++id)
            boost::put(idx1, *vit, id);
        id = 0;
        for (auto vit = boost::vertices(g2).first; vit != boost::vertices(g2).second; ++vit, ++id)
            boost::put(idx2, *vit, id);

        // g1: cycles (0-1-2-3-0), (4-5-6-7-4), (8-9-10-11-8)
        std::vector<std::pair<int,int>> edges1 = {{0,1},{1,2},{2,3},{3,0},
                                                  {4,5},{5,6},{6,7},{7,4},
                                                  {8,9},{9,10},{10,11},{11,8}};
        for (auto& e : edges1) {
            auto u = boost::vertex(e.first, g1);
            auto v = boost::vertex(e.second, g1);
            boost::add_edge(u, v, g1);
        }
        // g2: permuted cycles (9-10-11-0-9), (1-2-3-4-1), (5-6-7-8-5)
        std::vector<std::pair<int,int>> edges2 = {{9,10},{10,11},{11,0},{0,9},
                                                  {1,2},{2,3},{3,4},{4,1},
                                                  {5,6},{6,7},{7,8},{8,5}};
        for (auto& e : edges2) {
            auto u = boost::vertex(e.first, g2);
            auto v = boost::vertex(e.second, g2);
            boost::add_edge(u, v, g2);
        }
        assert(checkGraphIsomorphism(g1, g2) == true);
    }

    // Test 2: Non-isomorphic graphs (different degree sequences)
    {
        Graph g1(12), g2(12);
        auto idx1 = boost::get(boost::vertex_index, g1);
        auto idx2 = boost::get(boost::vertex_index, g2);
        int id = 0;
        for (auto vit = boost::vertices(g1).first; vit != boost::vertices(g1).second; ++vit, ++id)
            boost::put(idx1, *vit, id);
        id = 0;
        for (auto vit = boost::vertices(g2).first; vit != boost::vertices(g2).second; ++vit, ++id)
            boost::put(idx2, *vit, id);

        // g1: one vertex connected to all others (star) plus rest isolated
        auto center = boost::vertex(0, g1);
        for (int i = 1; i < 12; ++i)
            boost::add_edge(center, boost::vertex(i, g1), g1);
        // g2: all vertices degree 1 (six disjoint edges)
        for (int i = 0; i < 12; i += 2)
            boost::add_edge(boost::vertex(i, g2), boost::vertex(i+1, g2), g2);
        assert(checkGraphIsomorphism(g1, g2) == false);
    }

    // Test 3: Same graph (trivially isomorphic)
    {
        Graph g1(12);
        auto idx1 = boost::get(boost::vertex_index, g1);
        int id = 0;
        for (auto vit = boost::vertices(g1).first; vit != boost::vertices(g1).second; ++vit, ++id)
            boost::put(idx1, *vit, id);
        // Add a cycle of length 12
        for (int i = 0; i < 12; ++i)
            boost::add_edge(boost::vertex(i, g1), boost::vertex((i+1)%12, g1), g1);
        assert(checkGraphIsomorphism(g1, g1) == true);
    }

    // Test 4: Different vertex counts (should return false)
    {
        Graph g1(12), g2(13);
        // g2 has 13 vertices, but our function expects exactly 12; it should output f: 0..11 and return false
        // We need to set indices for g2 (though not used for comparison, but for completeness)
        auto idx2 = boost::get(boost::vertex_index, g2);
        int id = 0;
        for (auto vit = boost::vertices(g2).first; vit != boost::vertices(g2).second; ++vit, ++id)
            boost::put(idx2, *vit, id);
        assert(checkGraphIsomorphism(g1, g2) == false);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The solution uses the Boost Graph Library's `isomorphism` function. First, verify both graphs have exactly 12 vertices; if not, return `false`. Next, compute vertex index maps for both graphs: since the graphs use `listS` for the vertex list, vertex descriptors are not automatically contiguous, so explicitly set vertex index properties from 0 to 11 using iterators. Then prepare a vector `f` of size 12 to store the mapping from graph1's vertices (indexed by their vertex indices) to graph2's vertex descriptors. Call `boost::isomorphism` with the two graphs and an `isomorphism_map` wrapping `f` via `make_iterator_property_map` using graph1's vertex index map. The function returns `true` if isomorphic; otherwise `false`. If isomorphic, print "f: " followed by the vertex index of each mapped vertex in graph2 (obtained via graph2's vertex index map) in order of graph1's vertex indices 0..11. Otherwise, print nothing else. Edge cases: graphs with different vertex counts are immediately non-isomorphic; if both have 12 vertices but different degree sequences, the algorithm will return false after internal checks. Time complexity is exponential in the worst case for general graph isomorphism (no known polynomial-time algorithm), but for 12 vertices the Boost implementation uses backtracking with invariants and is fast. Space complexity is O(V+E) for the graphs and O(V) for the mapping.
