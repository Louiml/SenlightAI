// Write a C++ function named `countReachableVertices` that takes a directed graph represented as a `boost::adjacency_list` (with `vecS` for vertex storage and `listS` or `vecS` for edge storage) and a starting vertex descriptor, and returns the number of vertices reachable from the starting vertex (including the start vertex itself). The graph may contain cycles, parallel edges, and vertices with no outgoing edges. Your function must work for any vertex descriptor type supported by `boost::graph_traits`. To test, you will read a graph from a file using the provided `read_graph_file` helper (or build a small graph manually) and verify the result using `assert`.

The solution uses a breadth-first search (BFS) or depth-first search (DFS) traversal from the given starting vertex, marking visited vertices in a `std::vector<bool>` of size `num_vertices(g)`. Since the graph is directed, we only follow outgoing edges. The traversal handles cycles naturally because we mark vertices when first encountered, so each vertex is processed at most once. Vertices with no outgoing edges simply end the traversal from that branch. The count is incremented for every vertex visited, including the start vertex. Time complexity is O(V + E) where V is the number of vertices and E is the number of edges, because each vertex and edge is examined at most once. Space complexity is O(V) for the visited array and the queue/stack used by the traversal.

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <vector>
#include <queue>

// Count the number of vertices reachable from a given start vertex in a directed graph.
// Uses BFS. Includes the start vertex itself.
template <typename Graph>
typename boost::graph_traits<Graph>::vertices_size_type
countReachableVertices(const Graph& g,
                       typename boost::graph_traits<Graph>::vertex_descriptor start)
{
    typedef typename boost::graph_traits<Graph>::vertices_size_type size_type;
    typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;
    typedef typename boost::graph_traits<Graph>::adjacency_iterator AdjIt;

    size_type n = boost::num_vertices(g);
    std::vector<bool> visited(n, false);
    std::queue<Vertex> q;
    size_type count = 0;

    visited[start] = true;
    q.push(start);
    ++count;

    while (!q.empty()) {
        Vertex u = q.front();
        q.pop();

        std::pair<AdjIt, AdjIt> adj = boost::adjacent_vertices(u, g);
        for (AdjIt it = adj.first; it != adj.second; ++it) {
            Vertex v = *it;
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
                ++count;
            }
        }
    }
    return count;
}

#include <cassert>
#include <fstream>
#include <sstream>
#include <boost/graph/adjacency_list.hpp>

// Helper function as given in the prompt
template < typename Graph > void
read_graph_file(std::istream & in, Graph & g)
{
  typedef typename boost::graph_traits < Graph >::vertex_descriptor Vertex;
  typedef typename boost::graph_traits < Graph >::vertices_size_type size_type;
  size_type n_vertices;
  in >> n_vertices;
  std::vector < Vertex > vertex_set(n_vertices);
  for (size_type i = 0; i < n_vertices; ++i)
    vertex_set[i] = boost::add_vertex(g);

  size_type u, v;
  while (in >> u)
    if (in >> v)
      boost::add_edge(vertex_set[u], vertex_set[v], g);
    else
      break;
}

int main() {
    typedef boost::adjacency_list< boost::listS, boost::vecS, boost::directedS > Graph;
    
    // Test 1: Small graph from string
    std::istringstream data("3\n0 1\n1 2\n2 0\n");  // 3 vertices, cycle 0->1->2->0
    Graph g1;
    read_graph_file(data, g1);
    assert(countReachableVertices(g1, 0) == 3);  // all reachable
    assert(countReachableVertices(g1, 1) == 3);
    
    // Test 2: Graph with isolated vertex
    std::istringstream data2("4\n0 1\n1 2\n");  // vertex 3 isolated
    Graph g2;
    read_graph_file(data2, g2);
    assert(countReachableVertices(g2, 0) == 3);  // 0,1,2
    assert(countReachableVertices(g2, 3) == 1);  // only itself
    
    // Test 3: Empty graph (no edges) with 2 vertices
    std::istringstream data3("2\n");
    Graph g3;
    read_graph_file(data3, g3);
    assert(countReachableVertices(g3, 0) == 1);
    assert(countReachableVertices(g3, 1) == 1);
    
    // Test 4: Parallel edges and self-loop
    std::istringstream data4("2\n0 1\n0 1\n1 1\n");
    Graph g4;
    read_graph_file(data4, g4);
    assert(countReachableVertices(g4, 0) == 2);
    assert(countReachableVertices(g4, 1) == 1);
    
    return 0;
}
