// Write a standalone C++ function that takes a directed graph represented as an adjacency list (using `boost::adjacency_list<>`) where each vertex is identified by an integer index from 0 to N-1, and a vector of human‑readable names for the vertices. The function must print, for every vertex, its name followed by either “ has no children” if the out‑degree is zero, or “ is the parent of ” followed by a comma‑separated list of the names of its adjacent (outgoing) vertices, and end with a newline. Use only the Boost graph library macros `BGL_FORALL_VERTICES` and `BGL_FORALL_ADJ`, and ensure the output order follows the natural vertex iteration order (`vertex_iterator`). The function signature should be `void printParentInfo(const boost::adjacency_list<>& g, const std::vector<std::string>& names)`. The graph is guaranteed to have at least one vertex, and `names` has size equal to the number of vertices.
#include <boost/graph/adjacency_list.hpp>
#include <cassert>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

// The solution function is declared here (or included from the header).
void printParentInfo(const boost::adjacency_list<>& g, const std::vector<std::string>& names);

// Helper to capture output of printParentInfo into a string.
std::string captureOutput(const boost::adjacency_list<>& g, const std::vector<std::string>& names) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    printParentInfo(g, names);
    std::cout.rdbuf(old);
    return buffer.str();
}

int main() {
    using namespace boost;
    enum { A, B, C, D, N }; // 4 vertices

    // Test 1: Simple chain A->B->C, D isolated.
    {
        adjacency_list<> g(N);
        add_edge(A, B, g);
        add_edge(B, C, g);
        std::vector<std::string> names = {"A", "B", "C", "D"};
        std::string out = captureOutput(g, names);
        assert(out == "A is the parent of B, \nB is the parent of C, \nC has no children\nD has no children\n");
    }

    // Test 2: Graph with multiple children.
    {
        adjacency_list<> g(N);
        add_edge(A, B, g);
        add_edge(A, C, g);
        add_edge(A, D, g);
        std::vector<std::string> names = {"Root", "Child1", "Child2", "Child3"};
        std::string out = captureOutput(g, names);
        assert(out == "Root is the parent of Child1, Child2, Child3, \nChild1 has no children\nChild2 has no children\nChild3 has no children\n");
    }

    // Test 3: Graph with a cycle (A->B, B->A) – both have children.
    {
        adjacency_list<> g(N);
        add_edge(A, B, g);
        add_edge(B, A, g);
        std::vector<std::string> names = {"X", "Y", "Z", "W"};
        std::string out = captureOutput(g, names);
        assert(out == "X is the parent of Y, \nY is the parent of X, \nZ has no children\nW has no children\n");
    }

    // Test 4: All vertices have no children.
    {
        adjacency_list<> g(N);
        std::vector<std::string> names = {"lonely1", "lonely2", "lonely3", "lonely4"};
        std::string out = captureOutput(g, names);
        assert(out == "lonely1 has no children\nlonely2 has no children\nlonely3 has no children\nlonely4 has no children\n");
    }

    // Test 5: Vertex with self-loop (still counts as a child).
    {
        adjacency_list<> g(N);
        add_edge(A, A, g);
        std::vector<std::string> names = {"Self", "B", "C", "D"};
        std::string out = captureOutput(g, names);
        assert(out == "Self is the parent of Self, \nB has no children\nC has no children\nD has no children\n");
    }

    return 0;
}
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <iostream>
#include <vector>
#include <string>

// Print parent/child info for each vertex of a directed graph.
void printParentInfo(const boost::adjacency_list<>& g, const std::vector<std::string>& names) {
    // Vertex index map to obtain the integer identifier of each vertex.
    boost::property_map<boost::adjacency_list<>, boost::vertex_index_t>::type index_map = boost::get(boost::vertex_index, g);

    BGL_FORALL_VERTICES(i, g, boost::adjacency_list<>) {
        int idx = boost::get(index_map, i);
        std::cout << names[idx];

        if (boost::out_degree(i, g) == 0) {
            std::cout << " has no children";
        } else {
            std::cout << " is the parent of ";
        }

        BGL_FORALL_ADJ(i, j, g, boost::adjacency_list<>) {
            std::cout << names[boost::get(index_map, j)] << ", ";
        }
        std::cout << std::endl;
    }
}
// The solution uses Boost’s graph traversal macros. The main algorithm:  
// - Obtain `vertex_iterator` over all vertices via `BGL_FORALL_VERTICES(i, g, adjacency_list<>)`.  
// - For each vertex, retrieve its index using `get(vertex_index, g)` (which returns the integer ID).  
// - Print the name from the `names` vector at that index.  
// - Check `out_degree(i, g)`: if zero, print “ has no children”; otherwise print “ is the parent of ” followed by the names of all adjacent vertices obtained via `BGL_FORALL_ADJ(i, j, g, adjacency_list<>)`. After the inner loop, print a comma‑separated list with trailing “, ” (as in the original snippet) and then a newline.  
// Edge cases:  
// - A vertex with no outgoing edges – handled by the `out_degree == 0` branch.  
// - Duplicate edges (if any) will print the neighbour twice – acceptable per specification.  
// - The graph is directed, so only outgoing edges are considered.  
// Time complexity: O(V + E) because each vertex and each edge is visited once. Space complexity: O(V) for the names vector, plus O(1) auxiliary for iterators.  
// The function is `const`‑correct (takes `const` reference to graph and `const` reference to names).
