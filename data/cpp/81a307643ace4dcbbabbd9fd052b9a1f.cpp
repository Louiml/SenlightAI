Write a C++ function `computeMinCutWeight` that takes an undirected weighted graph as a `boost::adjacency_list` with `boost::vecS` for vertex and edge storage, `boost::undirectedS`, and an edge weight property of type `int`. The function should return the weight of the global minimum cut of the graph using the Stoer-Wagner algorithm. The graph is guaranteed to have at least 2 vertices and may contain parallel edges (multiple edges between the same pair of vertices). The function must not modify the input graph (pass by const reference). Use the Boost Graph Library's `stoer_wagner_min_cut` function, providing it with the edge weight property map and a parity map (e.g., created with `boost::make_one_bit_color_map`) to hold the partition result, but only the weight is returned. The function should be robust for graphs with zero-weight edges and non-negative weights (the algorithm requires non-negative weights). The function signature should be: `int computeMinCutWeight(const undirected_graph& g);` where `undirected_graph` is a type alias defined in your solution.
#include <cassert>
#include <iostream>

// Include the solution's header (or here we assume it's in the same file)
// For testing, we replicate the function directly or include it.
// Below we just declare the function and test it.
// (In a real test, you would include the solution's file.)
int computeMinCutWeight(const undirected_graph& g);

int main() {
    // Test 1: Simple graph with 2 vertices and 1 edge of weight 5
    {
        undirected_graph g(2);
        boost::add_edge(0, 1, 5, g);
        assert(computeMinCutWeight(g) == 5);
    }

    // Test 2: Triangle with equal weights -> each edge weight is min-cut (weight 1)
    {
        undirected_graph g(3);
        boost::add_edge(0, 1, 1, g);
        boost::add_edge(1, 2, 1, g);
        boost::add_edge(2, 0, 1, g);
        assert(computeMinCutWeight(g) == 1);
    }

    // Test 3: Parallel edges between two vertices (weights 2 and 3) -> total 5
    {
        undirected_graph g(2);
        boost::add_edge(0, 1, 2, g);
        boost::add_edge(0, 1, 3, g);
        assert(computeMinCutWeight(g) == 5);
    }

    // Test 4: Zero-weight edges can make min-cut weight zero
    {
        undirected_graph g(2);
        boost::add_edge(0, 1, 0, g);
        assert(computeMinCutWeight(g) == 0);
    }

    // Test 5: Disconnected graph (no edges) -> min-cut weight is 0
    {
        undirected_graph g(3);
        assert(computeMinCutWeight(g) == 0);
    }

    // Test 6: Larger graph from the original snippet (weights as given),
    // expected min-cut weight is 7
    {
        undirected_graph g(8);
        // edges: {3,4}, {3,6}, {3,5}, {0,4}, {0,1}, {0,6}, {0,7}, {0,5},
        //        {0,2}, {4,1}, {1,6}, {1,5}, {6,7}, {7,5}, {5,2}, {3,4}
        // weights: 0, 3, 1, 3, 1, 2, 6, 1, 8, 1, 1, 80, 2, 1, 1, 4
        boost::add_edge(3, 4, 0, g);
        boost::add_edge(3, 6, 3, g);
        boost::add_edge(3, 5, 1, g);
        boost::add_edge(0, 4, 3, g);
        boost::add_edge(0, 1, 1, g);
        boost::add_edge(0, 6, 2, g);
        boost::add_edge(0, 7, 6, g);
        boost::add_edge(0, 5, 1, g);
        boost::add_edge(0, 2, 8, g);
        boost::add_edge(4, 1, 1, g);
        boost::add_edge(1, 6, 1, g);
        boost::add_edge(1, 5, 80, g);
        boost::add_edge(6, 7, 2, g);
        boost::add_edge(7, 5, 1, g);
        boost::add_edge(5, 2, 1, g);
        boost::add_edge(3, 4, 4, g); // second parallel edge (same as first line but weight 4 total? Actually original has two edges: first weight 0, second weight 4, total 4)
        // Note: In original snippet, edges[0] and edges[15] both are {3,4} with weights 0 and 4, so total weight between 3 and 4 is 4, not 5.
        assert(computeMinCutWeight(g) == 7);
    }

    // Test 7: Chain graph 0-1-2 with weights 10 and 20 -> min-cut is 10 (edge 0-1)
    {
        undirected_graph g(3);
        boost::add_edge(0, 1, 10, g);
        boost::add_edge(1, 2, 20, g);
        assert(computeMinCutWeight(g) == 10);
    }

    // Test 8: Star graph center 0 connected to leaves 1,2,3 with weights 1,2,3
    // The minimum cut is the smallest edge weight (1) because removing it isolates leaf 1.
    {
        undirected_graph g(4);
        boost::add_edge(0, 1, 1, g);
        boost::add_edge(0, 2, 2, g);
        boost::add_edge(0, 3, 3, g);
        assert(computeMinCutWeight(g) == 1);
    }

    // Test 9: Graph with 4 vertices in a square with all edges weight 1
    // Minimum cut is 2 (either two opposite edges separate one pair from the other)
    {
        undirected_graph g(4);
        boost::add_edge(0, 1, 1, g);
        boost::add_edge(1, 2, 1, g);
        boost::add_edge(2, 3, 1, g);
        boost::add_edge(3, 0, 1, g);
        // Also add diagonal to make it 4-clique? Actually for square without diagonal, min-cut is 2.
        assert(computeMinCutWeight(g) == 2);
    }

    // Test 10: Graph with 2 vertices and two parallel edges of weight 3 each -> min-cut 6
    {
        undirected_graph g(2);
        boost::add_edge(0, 1, 3, g);
        boost::add_edge(0, 1, 3, g);
        assert(computeMinCutWeight(g) == 6);
    }

    std::cout << "All tests passed!\n";
    return 0;
}
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/stoer_wagner_min_cut.hpp>
#include <boost/graph/one_bit_color_map.hpp>
#include <boost/property_map/property_map.hpp>
#include <cstddef>

// Type alias for the undirected graph with int edge weights
using undirected_graph = boost::adjacency_list<
    boost::vecS, boost::vecS, boost::undirectedS,
    boost::no_property, boost::property<boost::edge_weight_t, int>>;

// Computes the weight of the global minimum cut of an undirected weighted graph.
// The graph is passed by const reference and is not modified.
// Requires non-negative edge weights (as per Stoer-Wagner).
// Returns the integer weight of the global minimum cut.
int computeMinCutWeight(const undirected_graph& g) {
    // Get the edge weight property map
    auto weight_map = get(boost::edge_weight, g);

    // Create a parity map to store the partition (required by the algorithm)
    // We do not need the partition, but the function requires the map.
    auto parities = boost::make_one_bit_color_map(
        num_vertices(g), get(boost::vertex_index, g));

    // Run Stoer-Wagner algorithm and return the min-cut weight
    return boost::stoer_wagner_min_cut(
        g, weight_map, boost::parity_map(parities));
}
// The solution uses the Stoer-Wagner algorithm implemented in Boost's `stoer_wagner_min_cut` function. This algorithm finds the global minimum cut in an undirected, weighted graph in \(O(n^3)\) time for a graph with \(n\) vertices, assuming adjacency list with `vecS` and edge weights are non-negative. The function must create a parity map (one-bit color map) to satisfy the algorithm's requirement of a parity output parameter; this map can be created locally and does not affect the result. The algorithm handles parallel edges automatically because they are stored as separate edges in the adjacency list and their weights sum appropriately during the algorithm's contraction phases. Edge cases include graphs with zero-weight edges (the min-cut weight can be zero if there is a cut with no edges crossing) and graphs with a single edge (the weight of that edge is the min-cut). Time complexity is \(O(n^3)\) (actually \(O(n^3)\) for dense graphs, but for sparse graphs it is \(O(m n + n^2 \log n)\) using priority queues; here we accept the Boost default which is fine). Space complexity is \(O(n + m)\) for the graph representation and the parity map.
