/*
Write a C++ function `computeMaxFlow` that reads a directed graph from standard input in DIMACS max-flow format and returns the value of the maximum flow from the source to the sink. The input format follows the DIMACS specification: lines starting with `c` are comments, `p max` declares the problem (with the number of nodes and edges), `n` lines specify the source (`s`) and sink (`t`) nodes, and `a` lines specify directed edges with capacity (from `u` to `v` with capacity `cap`). The graph may have multiple edges between the same pair of nodes, self-loops, and zero-capacity edges. The function must use the push-relabel algorithm (not Ford-Fulkerson) and return the maximum flow as a `long` value. The source and sink are guaranteed to be valid node indices (0-based) present in the graph. You may assume the graph is connected and the input is well-formed, but you must handle cases where the maximum flow is zero due to no path from source to sink. The solution must not require any external files beyond standard headers and Boost.Graph library.
*/

#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/read_dimacs.hpp>
#include <boost/config.hpp>
#include <iostream>

// Reads a DIMACS max-flow graph from standard input and returns the maximum flow value.
long computeMaxFlow() {
    using namespace boost;

    typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
    typedef adjacency_list<listS, vecS, directedS,
        property<vertex_name_t, std::string>,
        property<edge_capacity_t, long,
            property<edge_residual_capacity_t, long,
                property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;

    Graph g;

    property_map<Graph, edge_capacity_t>::type capacity = get(edge_capacity, g);
    property_map<Graph, edge_reverse_t>::type rev = get(edge_reverse, g);
    property_map<Graph, edge_residual_capacity_t>::type residual_capacity = get(edge_residual_capacity, g);

    Traits::vertex_descriptor s, t;
    read_dimacs_max_flow(g, capacity, rev, s, t);

    long flow;
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
    property_map<Graph, vertex_index_t>::type indexmap = get(vertex_index, g);
    flow = push_relabel_max_flow(g, s, t, capacity, residual_capacity, rev, indexmap);
#else
    flow = push_relabel_max_flow(g, s, t);
#endif

    return flow;
}

#include <cassert>
#include <sstream>
#include <iostream>

// Forward declaration of the solution function.
long computeMaxFlow();

// Redirect cin to a stringstream containing DIMACS data and run the test.
void runTest(const std::string& input, long expected) {
    std::streambuf* oldBuf = std::cin.rdbuf();
    std::stringstream ss(input);
    std::cin.rdbuf(ss.rdbuf());
    long result = computeMaxFlow();
    std::cin.rdbuf(oldBuf);
    assert(result == expected);
}

int main() {
    // Test 1: Simple two-node graph with one edge of capacity 5.
    runTest("c simple\np max 2 1\nn 0 s\nn 1 t\na 0 1 5\n", 5);

    // Test 2: Disconnected graph (stream is still valid but no path).
    runTest("c disconnected\np max 3 1\nn 0 s\nn 2 t\na 0 1 3\n", 0);

    // Test 3: Multiple parallel edges from source to sink.
    runTest("c parallel\np max 2 3\nn 0 s\nn 1 t\na 0 1 2\na 0 1 3\na 0 1 4\n", 9);

    // Test 4: Zero-capacity edge and a self-loop, plus a valid path.
    runTest("c edge cases\np max 4 4\nn 0 s\nn 3 t\na 0 0 7\na 0 1 0\na 1 3 6\na 2 3 1\n", 6);

    // Test 5: Larger graph requiring multiple augmenting paths.
    runTest("c diamond\np max 4 5\nn 0 s\nn 3 t\na 0 1 3\na 0 2 4\na 1 2 2\na 1 3 2\na 2 3 5\n", 7);

    // Test 6: Flow must be zero when source is sink.
    runTest("c same node\np max 1 0\nn 0 s\nn 0 t\n", 0);

    std::cout << "All tests passed.\n";
    return 0;
}

// The solution uses the Boost Graph Library's `read_dimacs_max_flow` function to parse the DIMACS input into a directed `adjacency_list` graph with edge capacities, reverse edge descriptors, and residual capacity properties. The graph’s vertex list and edge list are built automatically by the reader. After parsing, the function calls `boost::push_relabel_max_flow` with the graph, source, and sink, which internally uses the named-parameter version (or the non-named version for older MSVC compilers) to compute the maximum flow. The algorithm runs in \(O(V^2 E)\) worst-case time, but in practice it is efficient for many real-world graphs; space complexity is \(O(V + E)\) for the graph and internal data structures. Edge cases include self-loops (which are ignored by the algorithm as they do not contribute to flow), multiple edges (handled separately), and zero-capacity edges (which are ignored). Since the input is guaranteed well-formed, no validation is needed, but the function returns 0 if the source and sink are disconnected.
