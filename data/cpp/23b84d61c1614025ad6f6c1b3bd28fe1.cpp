// Write a C++ function named `graphHasCycle` that takes a `boost::undirected_graph<boost::no_property>` by const reference and returns a `bool` indicating whether the graph contains a cycle. The graph may have any number of vertices (including zero) and any number of edges, including parallel edges (multiple edges between the same pair of vertices) and self-loops (edges from a vertex to itself). Treat self-loops and parallel edges as cycles. The function must use iterative or recursive traversal (e.g., DFS) and must not modify the input graph. For an empty graph or a graph with no edges, return `false`. The solution must be self-contained, including only necessary Boost and standard headers, and must not include a `main` function.

// The algorithm uses a depth-first search (DFS) to detect cycles in an undirected graph. For each unvisited vertex, we start a DFS traversal. During the traversal, we keep track of the parent vertex from which we came. In an undirected graph, a cycle is present if we encounter a visited vertex that is not the parent of the current vertex. Self-loops are detected directly because when exploring an edge from a vertex to itself, the neighbor is the same vertex and is already visited and is not the parent (since the neighbor equals the current vertex, and a parent cannot be the same vertex for a self-loop). Parallel edges are detected because a second edge between two already-connected vertices creates a situation where a neighbor is visited and is not the parent (the first edge made it a child, the second edge makes it a back edge). We use DFS recursively with an explicit visited set to handle disconnected graphs. Edge cases: empty graph and graph with no edges trivially have no cycle; a single vertex with a self-loop is a cycle; a graph with two vertices and two parallel edges forms a cycle (the second edge connects two already-connected vertices). Time complexity is O(V + E) where V is the number of vertices and E is the number of edges, since each vertex and edge is explored once. Space complexity is O(V) for the visited set and recursion stack in the worst case (for a deep chain), plus the graph itself is not copied.

#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/graph_traits.hpp>
#include <unordered_set>
#include <functional>

typedef boost::undirected_graph<boost::no_property> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

// Returns true if the undirected graph contains a cycle (including self-loops and parallel edges).
bool graphHasCycle(const Graph& g) {
    std::unordered_set<Vertex> visited;

    // Depth-first search that returns true if a cycle is detected.
    std::function<bool(Vertex, Vertex)> dfs = [&](Vertex current, Vertex parent) -> bool {
        visited.insert(current);
        auto adj = boost::adjacent_vertices(current, g);
        for (auto it = adj.first; it != adj.second; ++it) {
            Vertex neighbor = *it;
            if (neighbor == current) {
                // Self-loop indicates a cycle.
                return true;
            }
            if (visited.find(neighbor) != visited.end()) {
                if (neighbor != parent) {
                    // A visited vertex that is not the parent means a back edge -> cycle.
                    return true;
                }
            } else {
                // Recurse to unvisited neighbor.
                if (dfs(neighbor, current)) {
                    return true;
                }
            }
        }
        return false;
    };

    auto vertices = boost::vertices(g);
    for (auto it = vertices.first; it != vertices.second; ++it) {
        Vertex v = *it;
        if (visited.find(v) == visited.end()) {
            if (dfs(v, v)) { // parent of a root is itself, but self-loops are handled in dfs.
                // But note: passing v as parent for root is fine because no neighbor equals parent unless self-loop.
                return true;
            }
        }
    }
    return false;
}

#include <cassert>

int main() {
    // Test 1: Empty graph -> no cycle.
    {
        Graph g;
        assert(graphHasCycle(g) == false);
    }

    // Test 2: Single vertex, no edges -> no cycle.
    {
        Graph g;
        g.add_vertex();
        assert(graphHasCycle(g) == false);
    }

    // Test 3: Single vertex with self-loop -> cycle.
    {
        Graph g;
        auto v = g.add_vertex();
        g.add_edge(v, v);
        assert(graphHasCycle(g) == true);
    }

    // Test 4: Two vertices with one edge -> no cycle.
    {
        Graph g;
        auto v0 = g.add_vertex();
        auto v1 = g.add_vertex();
        g.add_edge(v0, v1);
        assert(graphHasCycle(g) == false);
    }

    // Test 5: Two vertices with two parallel edges -> cycle.
    {
        Graph g;
        auto v0 = g.add_vertex();
        auto v1 = g.add_vertex();
        g.add_edge(v0, v1);
        g.add_edge(v0, v1);
        assert(graphHasCycle(g) == true);
    }

    // Test 6: Triangle (3 vertices, 3 edges) -> cycle.
    {
        Graph g;
        auto v0 = g.add_vertex();
        auto v1 = g.add_vertex();
        auto v2 = g.add_vertex();
        g.add_edge(v0, v1);
        g.add_edge(v1, v2);
        g.add_edge(v0, v2);
        assert(graphHasCycle(g) == true);
    }

    // Test 7: Tree with 4 vertices (no cycle) -> false.
    {
        Graph g;
        auto v0 = g.add_vertex();
        auto v1 = g.add_vertex();
        auto v2 = g.add_vertex();
        auto v3 = g.add_vertex();
        g.add_edge(v0, v1);
        g.add_edge(v1, v2);
        g.add_edge(v1, v3);
        assert(graphHasCycle(g) == false);
    }

    // Test 8: Disconnected graph with one cycle in one component -> true.
    {
        Graph g;
        auto v0 = g.add_vertex();
        auto v1 = g.add_vertex();
        auto v2 = g.add_vertex();
        auto v3 = g.add_vertex();
        // Component 1: triangle
        g.add_edge(v0, v1);
        g.add_edge(v1, v2);
        g.add_edge(v0, v2);
        // Component 2: isolated vertex v3
        assert(graphHasCycle(g) == true);
    }

    // Test 9: Disconnected graph with no cycles -> false.
    {
        Graph g;
        auto v0 = g.add_vertex();
        auto v1 = g.add_vertex();
        auto v2 = g.add_vertex();
        g.add_edge(v0, v1);
        // v2 isolated
        assert(graphHasCycle(g) == false);
    }

    // Test 10: Self-loop plus another isolated vertex -> true.
    {
        Graph g;
        auto v0 = g.add_vertex();
        auto v1 = g.add_vertex();
        g.add_edge(v0, v0);
        assert(graphHasCycle(g) == true);
    }

    return 0;
}
