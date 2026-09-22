// Write a C++ function that reads an undirected graph from a text file where each line contains two integers `u` and `v` representing an edge, with vertices numbered from 0 to 7 (8 vertices total). The function must return a vector of integers representing a Hamiltonian cycle (a cycle that visits every vertex exactly once and returns to the start) if one exists, or an empty vector if no such cycle exists. The graph may be disconnected, may contain self-loops (though these should be ignored), and may contain duplicate edges (which should have no effect). The function should use the backtracking approach to find any valid Hamiltonian cycle starting from vertex 0, and the returned vector should list vertices in order starting with 0 (e.g., {0, 1, 2, 3, 4, 5, 6, 7} for a complete graph). The input file may not exist; if so, treat the graph as having no edges (return empty vector). Constrain the solution to exactly 8 vertices and ensure no global mutable state is used — the function should be self-contained and operate only on its input filename parameter.

#include <cassert>
#include <vector>
#include <string>
#include <fstream>
#include <functional>

// The solution function is defined here (as above, with corrected includes).
// For brevity, assume it is included. Here we write the main with tests.
int main() {
    // Test 1: Empty file → no cycle
    { std::ofstream("test_empty.txt"); }
    assert(findHamiltonianCycle("test_empty.txt").empty());

    // Test 2: Single edge 0-1 → no cycle (cannot visit all 8)
    { std::ofstream f("test_single.txt"); f << "0 1\n"; }
    assert(findHamiltonianCycle("test_single.txt").empty());

    // Test 3: Complete graph K8 → cycle must have 9 elements starting and ending with 0, all vertices distinct
    {
        std::ofstream f("test_complete.txt");
        for (int i = 0; i < 8; ++i)
            for (int j = i+1; j < 8; ++j)
                f << i << " " << j << "\n";
    }
    auto cycle = findHamiltonianCycle("test_complete.txt");
    assert(cycle.size() == 9);
    assert(cycle[0] == 0 && cycle[8] == 0);
    std::vector<bool> seen(8, false);
    for (int i = 0; i < 8; ++i) { seen[cycle[i]] = true; }
    for (bool s : seen) assert(s);

    // Test 4: Graph with duplicate edges and a self-loop → still works (same as single edge)
    {
        std::ofstream f("test_dup.txt");
        f << "0 1\n1 0\n0 0\n1 1\n";
    }
    assert(findHamiltonianCycle("test_dup.txt").empty());

    // Test 5: Cycle with 8 vertices forming a ring 0-1-2-...-7-0 → unique cycle
    {
        std::ofstream f("test_ring.txt");
        for (int i = 0; i < 8; ++i) f << i << " " << (i+1)%8 << "\n";
    }
    auto ring = findHamiltonianCycle("test_ring.txt");
    assert(ring.size() == 9);
    assert(ring[0] == 0 && ring[8] == 0);
    for (int i = 0; i < 8; ++i) {
        assert(ring[i] == i);
    }

    // Clean up test files
    remove("test_empty.txt");
    remove("test_single.txt");
    remove("test_complete.txt");
    remove("test_dup.txt");
    remove("test_ring.txt");
    return 0;
}

#include <vector>
#include <fstream>
#include <string>

// Reads a graph from filename (8 vertices, 0-7) and returns a Hamiltonian cycle starting at 0 as a vector of vertices (including the closing 0), or empty if none exists.
std::vector<int> findHamiltonianCycle(const std::string& filename) {
    const int V = 8;
    bool adj[V][V] = {{false}};
    
    std::ifstream file(filename);
    if (file.is_open()) {
        int u, v;
        while (file >> u >> v) {
            if (u >= 0 && u < V && v >= 0 && v < V && u != v) {
                adj[u][v] = true;
                adj[v][u] = true;
            }
        }
    }
    
    std::vector<int> path;
    path.reserve(V + 1);
    std::vector<bool> visited(V, false);
    path.push_back(0);
    visited[0] = true;
    
    // Recursive helper: attempt to extend the path to include all vertices, then close back to 0.
    bool found = false;
    // Use a lambda or nested function; since C++ doesn't allow nested functions, use a separate recursive function below (inline in solution, but here we simulate).
    // Because we need a free function returning only the final result, we implement a private recursion using a lambda (C++11+).
    std::function<bool()> backtrack = [&]() -> bool {
        if (path.size() == V) {
            if (adj[path.back()][0]) {
                path.push_back(0); // close the cycle
                return true;
            }
            return false;
        }
        for (int v = 0; v < V; ++v) {
            if (!visited[v] && adj[path.back()][v]) {
                path.push_back(v);
                visited[v] = true;
                if (backtrack()) return true;
                visited[v] = false;
                path.pop_back();
            }
        }
        return false;
    };
    
    found = backtrack();
    if (found) {
        return path; // contains [0, ..., V-1, 0]
    } else {
        return {}; // empty
    }
}
(Note: The solution above uses `std::function` and includes `<functional>` implicitly; to be self-contained, add `#include <functional>` at top. The problem statement says output code only, so in the final answer we include that header.)

// The solution uses a backtracking algorithm to find a Hamiltonian cycle in an 8-vertex undirected graph. We represent the graph with an adjacency matrix (8×8 boolean array) initialized to `false`. After reading edges from the file (skipping lines where `u==v` to ignore self-loops), we call a recursive helper that builds a path. The helper takes the current path (a vector of vertices) and a visited boolean array. At each step, we try all neighbors of the last vertex in the path that have not yet been visited. Base case: when the path contains all 8 vertices, we check whether there is an edge from the last vertex back to vertex 0; if yes, return the complete path (including a final 0 to show closure), otherwise backtrack. Important edge cases: (1) File missing or empty → graph has no edges → no cycle → return empty vector. (2) Graph may be disconnected → backtracking will exhaust all possibilities and return empty. (3) Duplicate edges are harmless because adjacency matrix uses booleans. (4) The problem guarantees exactly 8 vertices, so we hardcode `V=8`. Time complexity is O(V!) in the worst case because backtracking may try many permutations; for V=8, this is at most 40320 permutations, which is feasible. Space complexity is O(V²) for the adjacency matrix plus O(V) for recursion stack and path storage.
