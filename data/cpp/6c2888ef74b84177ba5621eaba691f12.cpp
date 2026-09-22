Write a C++ function that takes an undirected graph represented as an `adjacency_list` from Boost.Graph with `vecS` for vertex and edge storage, `undirectedS`, and a `vertex_index_t` property. The function must add the minimum number of edges to make the graph connected, and then return the number of edges added. The function should work for any graph with at least one vertex, including graphs that are already connected (where it should add zero edges) and graphs with isolated vertices or multiple components. You may use Boost.Graph algorithms such as `connected_components`, `add_edge`, and `vertices`. The solution must be deterministic and produce a minimal edge count (but not necessarily the exact edges, only the count). Do not modify the original graph; instead, work on a copy to ensure the input remains unchanged.

The core idea is to first compute the connected components of the input graph using `connected_components`. If there is only one component, the graph is already connected, so no edges need to be added. Otherwise, we need to connect every component into a single chain: pick one representative vertex from each component, then add edges between consecutive representatives. This adds exactly `(number_of_components - 1)` edges, which is the minimum possible to connect `k` components (since a connected graph with `n` vertices must have at least `n-1` edges, and each component already has at least `n_i - 1` edges internally, so the minimum additional edges is `(sum n_i - 1) - (sum (n_i - 1)) = k - 1`). The algorithm works as follows: copy the graph, compute component IDs for each vertex (which are indices from 0 to k-1). For each component ID from 1 to k-1, find a vertex belonging to that component (e.g., iterate over all vertices and pick the first one whose component ID matches), and also have the representative from component 0. Then add an edge between the representative of component 0 and the representative of component `i` (or between consecutive components, either way works). Since the graph is undirected and uses `vecS`, adding an edge is straightforward. Edge cases: graphs with 0 vertices (the problem specifies at least one vertex, but handle gracefully by returning 0), already connected graphs (return 0), and graphs with a single vertex (return 0). Time complexity: copying the graph takes O(V+E), computing components takes O(V+E), and iterating over vertices for each component takes O(k*V) which is at most O(V^2) in the worst case, but since k ≤ V, it's acceptable; overall O(V^2 + E) worst-case. Space complexity: O(V) for the component vector and the copy. The function returns the count of added edges.

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <vector>
#include <cstddef>

// Returns the minimum number of edges needed to make the input graph connected.
// The input graph is not modified; a copy is used internally.
template <typename Graph>
std::size_t minimal_edges_to_connect(const Graph& g) {
    // Handle empty or single-vertex graphs (already connected)
    if (boost::num_vertices(g) <= 1) {
        return 0;
    }

    // Work on a copy to avoid modifying the original graph
    Graph copy = g;

    // Compute connected components on the copy
    std::vector<std::size_t> component(boost::num_vertices(copy));
    std::size_t num_components = boost::connected_components(copy, &component[0]);

    // If already connected, no edges needed
    if (num_components <= 1) {
        return 0;
    }

    // Find a representative vertex for each component
    // We'll use the first vertex encountered for each component ID
    std::vector<typename boost::graph_traits<Graph>::vertex_descriptor> representatives(num_components);
    std::vector<bool> found(num_components, false);
    auto v_range = boost::vertices(copy);
    for (auto it = v_range.first; it != v_range.second; ++it) {
        std::size_t comp_id = component[*it];
        if (!found[comp_id]) {
            representatives[comp_id] = *it;
            found[comp_id] = true;
        }
    }

    // Add edges between consecutive representatives to form a chain
    std::size_t added = 0;
    for (std::size_t i = 0; i + 1 < num_components; ++i) {
        boost::add_edge(representatives[i], representatives[i+1], copy);
        ++added;
    }

    return added;
}

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <cassert>

// The solution function is assumed to be available here.

int main() {
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                                  boost::property<boost::vertex_index_t, int>> Graph;

    // Test 1: Already connected graph (a simple triangle)
    {
        Graph g(3);
        boost::add_edge(0, 1, g);
        boost::add_edge(1, 2, g);
        boost::add_edge(2, 0, g);
        assert(minimal_edges_to_connect(g) == 0);
    }

    // Test 2: Two isolated vertices -> need 1 edge
    {
        Graph g(2);
        assert(minimal_edges_to_connect(g) == 1);
    }

    // Test 3: Three separate components: {0,1}, {2}, {3}
    {
        Graph g(4);
        boost::add_edge(0, 1, g);
        assert(minimal_edges_to_connect(g) == 2);
    }

    // Test 4: Single vertex -> already connected
    {
        Graph g(1);
        assert(minimal_edges_to_connect(g) == 0);
    }

    // Test 5: Multiple components with cycles (like the snippet)
    {
        Graph g(11);
        boost::add_edge(0, 1, g);
        boost::add_edge(2, 3, g);
        boost::add_edge(3, 4, g);
        boost::add_edge(5, 6, g);
        boost::add_edge(6, 7, g);
        boost::add_edge(8, 9, g);
        boost::add_edge(9, 10, g);
        boost::add_edge(10, 8, g);
        // Components: {0,1}, {2,3,4}, {5,6,7}, {8,9,10} -> 4 components
        assert(minimal_edges_to_connect(g) == 3);
    }

    // Test 6: Empty graph (0 vertices) - should return 0 for safety
    {
        Graph g(0);
        assert(minimal_edges_to_connect(g) == 0);
    }

    // Test 7: Graph with two components, one is a single vertex, other has a cycle
    {
        Graph g(4);
        boost::add_edge(0, 1, g);
        boost::add_edge(1, 2, g);
        boost::add_edge(2, 0, g);
        // vertex 3 isolated -> 2 components
        assert(minimal_edges_to_connect(g) == 1);
    }

    return 0;
}
