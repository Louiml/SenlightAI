Write a standalone C++ function named `collect_node_names` that takes a graph object of the same type shown in the snippet (i.e., a `leda::GRAPH<std::string, int>`) by const reference and returns a `std::vector<std::string>` containing the string values stored at every vertex, in the order produced by iterating over all vertices. The function must not modify the graph and must work correctly even for an empty graph. You may assume the necessary LEDA and Boost headers are available. Provide a complete free function with proper `const` correctness, include necessary headers, and return the vector of node names.
// The solution requires iterating over all vertices of the graph using the `boost::graph_traits` interface, exactly as shown in the code snippet, but without printing. The key steps:  
// 1. Use `boost::vertices(g)` to get a pair of iterators (`vi`, `vi_end`).  
// 2. Use a property map obtained via `get(boost::vertex_all, g)` to extract the string name for each vertex.  
// 3. For each vertex in the range, push the mapped name into a `std::vector<std::string>`.  
// 4. Return that vector.  
//
// Edge cases:  
// - An empty graph: the iterator pair is equal, so the loop does nothing and returns an empty vector.  
// - The property map type is `boost::property_map<graph_t, boost::vertex_all_t>::type`; we need to use `boost::get` or `get` with the correct namespace.  
// - `const` correctness: the function takes `const graph_t&`, so the property map must be retrieved as `const` (the `get` function returns a const-compatible map). The iterators from `vertices(g)` on a const graph are const iterators, but that's fine.  
// - Complexity: Time \(O(V)\) where \(V\) is the number of vertices, because we visit each vertex exactly once. Space: \(O(V)\) for the output vector, plus constant auxiliary space for the iterators and property map.
#include <vector>
#include <string>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <LEDA/graph/graph.h>
#include <LEDA/core/string.h>
#include <LEDA/graph/graph_traits.h>

// Collect the string values of all vertices in a graph, in iteration order.
std::vector<std::string> collect_node_names(const leda::GRAPH<std::string, int>& g) {
    std::vector<std::string> names;
    
    // Property map to access the vertex's associated string.
    typedef boost::property_map<leda::GRAPH<std::string, int>, boost::vertex_all_t>::const_type NodeMap;
    NodeMap node_name_map = boost::get(boost::vertex_all, g);
    
    // Iterate over all vertices.
    boost::graph_traits<leda::GRAPH<std::string, int>>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(g); vi != vi_end; ++vi) {
        names.push_back(node_name_map[*vi]);
    }
    
    return names;
}
#include <cassert>
#include <vector>
#include <string>
#include <LEDA/graph/graph.h>

// Include the solution function (assumed in the same translation unit or header).
// For completeness, declare the function here if not already included.
std::vector<std::string> collect_node_names(const leda::GRAPH<std::string, int>& g);

int main() {
    // Test 1: Empty graph.
    leda::GRAPH<std::string, int> empty_graph;
    assert(collect_node_names(empty_graph).empty());

    // Test 2: Single vertex.
    leda::GRAPH<std::string, int> single_graph;
    single_graph.new_node("Solo");
    auto single_result = collect_node_names(single_graph);
    assert(single_result.size() == 1);
    assert(single_result[0] == "Solo");

    // Test 3: Multiple vertices in insertion order (LEDA typically preserves order).
    leda::GRAPH<std::string, int> multi_graph;
    multi_graph.new_node("Philoctetes");
    multi_graph.new_node("Heracles");
    multi_graph.new_node("Alcmena");
    multi_graph.new_node("Eurystheus");
    multi_graph.new_node("Amphitryon");
    auto multi_result = collect_node_names(multi_graph);
    std::vector<std::string> expected = {"Philoctetes", "Heracles", "Alcmena", "Eurystheus", "Amphitryon"};
    assert(multi_result == expected);

    // Test 4: Duplicate names are allowed and preserved.
    leda::GRAPH<std::string, int> dup_graph;
    dup_graph.new_node("A");
    dup_graph.new_node("B");
    dup_graph.new_node("A");
    auto dup_result = collect_node_names(dup_graph);
    assert(dup_result.size() == 3);
    assert(dup_result[0] == "A");
    assert(dup_result[1] == "B");
    assert(dup_result[2] == "A");

    // Test 5: Graph with many nodes, ensure order matches iteration.
    leda::GRAPH<std::string, int> big_graph;
    for (int i = 0; i < 100; ++i) {
        big_graph.new_node("Node" + std::to_string(i));
    }
    auto big_result = collect_node_names(big_graph);
    assert(big_result.size() == 100);
    for (int i = 0; i < 100; ++i) {
        assert(big_result[i] == "Node" + std::to_string(i));
    }

    return 0;
}
