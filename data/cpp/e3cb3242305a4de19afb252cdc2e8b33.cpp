Write a C++ function that takes a string containing a Graphviz DOT representation of a directed graph, along with a set of property names and values, and returns the graph in an adjacency_list format with custom properties (vertex name, vertex label, vertex root, edge label, graph name, and graph identifier). The function should parse the DOT input using `read_graphviz`, handle errors gracefully by returning an empty graph on failure, and expose the graph's properties through a dynamic property map so callers can query all parsed attributes. The solution must correctly handle graphs with nodes lacking explicit labels, edges with missing labels, and graph-level attributes, ensuring that missing properties get default values (empty string for labels, 0 for root). The function signature should be: `graph_t parseGraph(const std::string& dotInput, dynamic_properties& dp)`.

The core approach is to use Boost's `read_graphviz` to parse the DOT string into an empty `adjacency_list<vecS, vecS, directedS, vertex_p, edge_p, graph_p>` graph. Since the graph type has custom properties (vertex_name_t, vertex_label_t, vertex_root_t, edge_name_t, graph_name_t, graph_identifier_t), we must set up a `dynamic_properties` object that maps textual property names (as they appear in DOT) to the actual property maps. For vertex properties, we use `get(vertex_name, graph)` etc., and for graph properties we use `ref_property_map` to wrap the graph's internal property storage. The crucial mapping is `node_id` → vertex_name_t, `label` → both vertex_label_t and edge_name_t, `root` → vertex_root_t, `name` → graph_name_t, and `identifier` → graph_identifier_t. After reading, we check the return status; if false, we return a default-constructed empty graph with zero vertices. The function passes the `dp` by reference so the caller can access parsed properties via `get("label", dp, vertex)`. Complexity: reading the DOT takes O(V+E) time to build the graph and property maps; memory is O(V+E) for the graph itself plus the dynamic property maps. Edge cases include nodes with no `label`, edges without `label`, graphs without `name` or `identifier` — in these cases, `read_graphviz` leaves the corresponding property as default (empty string or 0). The function must ensure that `dp.property("label", vlabel)` and `dp.property("label", elabel)` are both registered so that the same property name can map to both vertex and edge properties — Boost handles this by associating the name with different property maps for different graph element types.

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <string>
#include <sstream>
#include <stdexcept>

// Custom property tags
struct graph_identifier_t { typedef boost::graph_property_tag kind; };
struct vertex_label_t { typedef boost::vertex_property_tag kind; };

// Property bundle types (same as in the snippet)
typedef boost::property<boost::vertex_name_t, std::string,
    boost::property<vertex_label_t, std::string,
        boost::property<boost::vertex_root_t, int> > > vertex_p;
typedef boost::property<boost::edge_name_t, std::string> edge_p;
typedef boost::property<boost::graph_name_t, std::string,
    boost::property<graph_identifier_t, std::string> > graph_p;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
    vertex_p, edge_p, graph_p> graph_t;

// Parse a DOT string into a graph with custom properties.
// On failure, returns an empty graph (zero vertices) and sets dp unchanged.
graph_t parseGraph(const std::string& dotInput, boost::dynamic_properties& dp) {
    graph_t graph(0);

    // Bind graph properties to dp
    boost::property_map<graph_t, boost::vertex_name_t>::type vname =
        boost::get(boost::vertex_name, graph);
    dp.property("node_id", vname);

    boost::property_map<graph_t, vertex_label_t>::type vlabel =
        boost::get(vertex_label_t(), graph);
    dp.property("label", vlabel);

    boost::property_map<graph_t, boost::vertex_root_t>::type root =
        boost::get(boost::vertex_root, graph);
    dp.property("root", root);

    boost::property_map<graph_t, boost::edge_name_t>::type elabel =
        boost::get(boost::edge_name, graph);
    dp.property("label", elabel);

    // Graph-level properties
    boost::ref_property_map<graph_t*, std::string> gname(
        boost::get_property(graph, boost::graph_name));
    dp.property("name", gname);

    boost::ref_property_map<graph_t*, std::string> gid(
        boost::get_property(graph, graph_identifier_t()));
    dp.property("identifier", gid);

    // Parse the DOT string
    std::istringstream input(dotInput);
    bool success = boost::read_graphviz(input, graph, dp, "node_id");

    if (!success) {
        // Return a fresh empty graph to signal failure
        return graph_t(0);
    }

    return graph;
}

#include <cassert>
#include <sstream>
#include <string>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>

// Include the solution code (or paste it here)
// (Assume the solution function and type definitions are available)

int main() {
    // Test 1: Full graph with all attributes
    {
        graph_t graph;
        boost::dynamic_properties dp;
        std::string dot = "digraph { graph [name=\"G1\", identifier=\"ID1\"] a [label=\"A\", root=\"1\"] b [label=\"B\", root=\"0\"] a -> b [label=\"E1\"] }";
        graph = parseGraph(dot, dp);
        assert(boost::num_vertices(graph) == 2);
        assert(boost::num_edges(graph) == 1);
        assert(boost::get("name", dp, &graph) == "G1");
        assert(boost::get("identifier", dp, &graph) == "ID1");
        auto v = *boost::vertices(graph).first;
        assert(boost::get("node_id", dp, v) == "a");
        assert(boost::get("label", dp, v) == "A");
        assert(boost::get("root", dp, v) == 1);
    }

    // Test 2: Missing labels default to empty string
    {
        graph_t graph;
        boost::dynamic_properties dp;
        std::string dot = "digraph { a -> b }";
        graph = parseGraph(dot, dp);
        assert(boost::num_vertices(graph) == 2);
        assert(boost::num_edges(graph) == 1);
        auto v = *boost::vertices(graph).first;
        assert(boost::get("node_id", dp, v) == "a");
        assert(boost::get("label", dp, v) == "");
        assert(boost::get("root", dp, v) == 0);
        auto eit = *boost::edges(graph).first;
        assert(boost::get("label", dp, eit) == "");
        // Graph properties default to empty string
        assert(boost::get("name", dp, &graph) == "");
        assert(boost::get("identifier", dp, &graph) == "");
    }

    // Test 3: Invalid DOT fails and returns empty graph
    {
        graph_t graph;
        boost::dynamic_properties dp;
        std::string bad = "digraph { a -> }"; // invalid syntax
        graph = parseGraph(bad, dp);
        assert(boost::num_vertices(graph) == 0);
    }

    // Test 4: Multiple nodes and edges with mixed attributes
    {
        graph_t graph;
        boost::dynamic_properties dp;
        std::string dot = "digraph { graph [name=\"Mixed\", identifier=\"M1\"] a [label=\"A1\", root=\"1\"] b [root=\"0\"] c [label=\"C1\"] a -> b [label=\"E1\"] b -> c }";
        graph = parseGraph(dot, dp);
        assert(boost::num_vertices(graph) == 3);
        assert(boost::num_edges(graph) == 2);
        assert(boost::get("name", dp, &graph) == "Mixed");
        assert(boost::get("identifier", dp, &graph) == "M1");
        // Check each vertex by scanning
        for (auto v : boost::make_iterator_range(boost::vertices(graph))) {
            std::string id = boost::get("node_id", dp, v);
            if (id == "a") {
                assert(boost::get("label", dp, v) == "A1");
                assert(boost::get("root", dp, v) == 1);
            } else if (id == "b") {
                assert(boost::get("label", dp, v) == "");
                assert(boost::get("root", dp, v) == 0);
            } else if (id == "c") {
                assert(boost::get("label", dp, v) == "C1");
                assert(boost::get("root", dp, v) == 0);
            } else {
                assert(false);
            }
        }
        // Edge labels
        for (auto e : boost::make_iterator_range(boost::edges(graph))) {
            std::string src = boost::get("node_id", dp, boost::source(e, graph));
            std::string tgt = boost::get("node_id", dp, boost::target(e, graph));
            if (src == "a" && tgt == "b") {
                assert(boost::get("label", dp, e) == "E1");
            } else if (src == "b" && tgt == "c") {
                assert(boost::get("label", dp, e) == "");
            } else {
                assert(false);
            }
        }
    }

    // Test 5: Empty graph (no nodes)
    {
        graph_t graph;
        boost::dynamic_properties dp;
        std::string dot = "digraph { }";
        graph = parseGraph(dot, dp);
        assert(boost::num_vertices(graph) == 0);
        assert(boost::num_edges(graph) == 0);
        assert(boost::get("name", dp, &graph) == "");
        assert(boost::get("identifier", dp, &graph) == "");
    }

    return 0;
}
