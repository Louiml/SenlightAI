Write a C++ function that takes a directed graph represented as an adjacency list with `bidirectionalS` edges and returns a `std::vector<std::vector<int>>` (or equivalent) where each element at index `i` contains the list of predecessors (incoming neighbors) of vertex `i`, sorted in ascending order. The graph may have any number of vertices (including zero) and edges, with no self-loops or parallel edges. The function should be non-mutating and work with `const` graph references. The result must list every vertex, even those with no incoming edges (their predecessor list should be empty).

The solution iterates over all vertices in the graph using `vertices(g)`. For each vertex, it obtains the range of in-edges using `in_edges(v, g)`. For each in-edge, it retrieves the source vertex using `source(e, g)` and appends it to that vertex's predecessor list. Because the Boost graph iterators for in-edges may not guarantee a specific order, the predecessor lists must be sorted before returning to ensure deterministic output. Edge cases include an empty graph (returns an empty vector), a vertex with no in-edges (its vector remains empty), and a graph with a single vertex and no edges. The time complexity is \(O(V + E \log E_{\text{max}})\) due to sorting each vertex's predecessor list (where \(E_{\text{max}}\) is the maximum in-degree) – in practice, for sparse graphs it is \(O(V + E)\) if the in-edge order is ignored, but sorting adds a logarithmic factor per vertex. Space complexity is \(O(V + E)\) to store the result.

#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>

// Given a directed graph, return a vector where result[v] contains all
// predecessors (incoming neighbors) of vertex v, sorted in ascending order.
std::vector<std::vector<int>> get_predecessors(
    const boost::adjacency_list<boost::listS, boost::vecS, boost::bidirectionalS>& g)
{
    std::vector<std::vector<int>> predecessors(boost::num_vertices(g));

    boost::graph_traits<boost::adjacency_list<boost::listS, boost::vecS, boost::bidirectionalS>>::vertex_iterator vi, vi_end;
    boost::graph_traits<boost::adjacency_list<boost::listS, boost::vecS, boost::bidirectionalS>>::in_edge_iterator ei, ei_end;

    for (boost::tie(vi, vi_end) = boost::vertices(g); vi != vi_end; ++vi) {
        int v = static_cast<int>(*vi);
        for (boost::tie(ei, ei_end) = boost::in_edges(*vi, g); ei != ei_end; ++ei) {
            predecessors[v].push_back(static_cast<int>(boost::source(*ei, g)));
        }
        std::sort(predecessors[v].begin(), predecessors[v].end());
    }

    return predecessors;
}

#include <cassert>
#include <vector>
#include <boost/graph/adjacency_list.hpp>

// Include the solution function here or via appropriate header.

int main() {
    using Graph = boost::adjacency_list<boost::listS, boost::vecS, boost::bidirectionalS>;
    using PredVec = std::vector<std::vector<int>>;

    // Empty graph
    {
        Graph g;
        assert(get_predecessors(g) == PredVec{});
    }

    // Single vertex, no edges
    {
        Graph g(1);
        assert(get_predecessors(g) == PredVec{{}});
    }

    // Graph from the sample
    {
        Graph g(5);
        boost::add_edge(0, 1, g);
        boost::add_edge(1, 2, g);
        boost::add_edge(1, 3, g);
        boost::add_edge(2, 4, g);
        boost::add_edge(3, 4, g);
        PredVec expected = {{}, {0}, {1}, {1}, {2, 3}};
        assert(get_predecessors(g) == expected);
    }

    // Graph with vertex with multiple predecessors (unsorted insertion order)
    {
        Graph g(3);
        boost::add_edge(2, 0, g);
        boost::add_edge(1, 0, g);
        boost::add_edge(2, 1, g);
        PredVec expected = {{1, 2}, {2}, {}};
        assert(get_predecessors(g) == expected);
    }

    // Graph with no edges but multiple vertices
    {
        Graph g(4);
        PredVec expected = {{}, {}, {}, {}};
        assert(get_predecessors(g) == expected);
    }

    // Fully connected directed graph (no self-loops) for n=4
    {
        Graph g(4);
        boost::add_edge(0, 1, g);
        boost::add_edge(0, 2, g);
        boost::add_edge(0, 3, g);
        boost::add_edge(1, 0, g);
        boost::add_edge(1, 2, g);
        boost::add_edge(1, 3, g);
        boost::add_edge(2, 0, g);
        boost::add_edge(2, 1, g);
        boost::add_edge(2, 3, g);
        boost::add_edge(3, 0, g);
        boost::add_edge(3, 1, g);
        boost::add_edge(3, 2, g);
        PredVec expected = {{1, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2}};
        assert(get_predecessors(g) == expected);
    }

    return 0;
}
