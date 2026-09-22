// Write a C++ function `void exploreGraph(int numVertices, int numEdges, const std::vector<std::pair<int,int>>& edges, int testCaseNumber)` that simulates a depth-first search (DFS) traversal on a directed graph. The graph has vertices labeled from 0 up to `numVertices - 1` (though not all vertices might appear as sources). The function must print output in a specific format: for each DFS start vertex (in ascending order of vertex label), it first prints `"Caso <testCaseNumber>:"` on its own line (this should be printed once per call, at the very beginning). Then, for each unvisited vertex (iterating through all vertices from 0 to numVertices-1 in ascending order), if that vertex has outgoing edges, start a DFS from it. During the DFS traversal, when exploring an edge from vertex `u` to neighbor `v` (neighbors processed in ascending order), if `v` is unvisited, print a line with indentation equal to 2 spaces per recursion depth, followed by `u-v pathR(G,v)` (where `u` and `v` are the vertex numbers). If `v` is already visited, print a line with the same indentation but just `u-v` (without the `pathR` part). After completing all DFS from a particular start vertex, print an empty line. After processing all vertices, the function ends (no trailing newline required beyond the last blank line). The function should use the provided edges (which may include duplicates and self-loops, and vertices may have no outgoing edges) and must handle the case where no edges exist at all (including not printing any `Caso` line or blank lines in that case? No—based on the original code, the `Caso` line is always printed, and the blank line after each DFS start is printed even if that vertex has no outgoing edges; but since we iterate over all vertices in ascending order, we only start DFS on a vertex if it appears as a source in the edge list. To be precise, the original code iterates over keys in the graph map, which only contains vertices that have at least one outgoing edge. So the function should only consider vertices that are sources in the edges; it should ignore vertices with no outgoing edges. Also, the original code's loop iterates over the map which is sorted by key, so we will iterate over all distinct source vertices in ascending order.)
The key is to build an adjacency list from the given edges, keeping only vertices that are sources (i.e., appear as first element in some edge). Store the adjacency list in a `std::map<int, std::vector<int>>` where keys are automatically sorted ascending. For each source vertex, sort its neighbor list in ascending order because the output requires processing neighbors in ascending order. The DFS uses a recursive function that maintains a global (or passed-by-reference) `visited` set and a `spaces` string for indentation. For each call, mark the current vertex as visited, then iterate over its sorted neighbors. For each neighbor, if it hasn't been visited, print the line `spaces + to_string(u) + "-" + to_string(v) + " pathR(G," + to_string(v) + ")"` (with spaces representing depth), then recurse on that neighbor. If the neighbor is already visited, just print `spaces + to_string(u) + "-" + to_string(v)`. Before processing neighbors, increase indentation by two spaces, and after finishing all neighbors, decrease it back. The main function should: print `"Caso <testCaseNumber>:"`, then iterate over all keys in the map (which are source vertices in ascending order). For each key, if it hasn't been visited, call DFS on it, then print a newline (blank line). The edge cases: if the graph has no edges, the map is empty, so the function prints only the `Caso` line and nothing else. If a vertex has a self-loop, it will be visited already, so it will print just `u-u` without recursion. Duplicate edges are handled naturally; the neighbor list may contain duplicates, but since we sort, they remain adjacent, and each will be processed; the first occurrence will trigger a recursive call if unvisited (but for a duplicate, the second occurrence will see it as visited and print plain). Time complexity is O(V + E log E) due to sorting neighbor lists, plus O(E) for traversal. Space complexity is O(V + E) for adjacency and visited set.
#include <map>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <iostream>

void exploreGraph(int numVertices, int numEdges, const std::vector<std::pair<int,int>>& edges, int testCaseNumber) {
    // Build adjacency list, but only for vertices that have outgoing edges
    std::map<int, std::vector<int>> graph;
    for (const auto& edge : edges) {
        graph[edge.first].push_back(edge.second);
    }
    // Sort each neighbor list in ascending order
    for (auto& kv : graph) {
        std::sort(kv.second.begin(), kv.second.end());
    }

    std::set<int> visited;
    std::string spaces = "";

    // Recursive DFS helper (lambda using std::function to allow recursion)
    std::function<void(int)> dfs = [&](int v) {
        visited.insert(v);
        if (!graph[v].empty()) {
            spaces += "  ";
            for (int e : graph[v]) {
                if (visited.count(e) == 0) {
                    std::cout << spaces << v << "-" << e << " pathR(G," << e << ")\n";
                    dfs(e);
                } else {
                    std::cout << spaces << v << "-" << e << "\n";
                }
            }
            spaces.pop_back();
            spaces.pop_back();
        }
    };

    std::cout << "Caso " << testCaseNumber << ":\n";
    for (const auto& kv : graph) {
        int vertex = kv.first;
        if (visited.count(vertex) == 0) {
            dfs(vertex);
            std::cout << "\n";
        }
    }
}
#include <cassert>
#include <sstream>
#include <iostream>

// Redirect cout to test output
// We'll use a helper to capture output
std::string captureOutput(int numVertices, int numEdges, const std::vector<std::pair<int,int>>& edges, int testCaseNumber) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    exploreGraph(numVertices, numEdges, edges, testCaseNumber);
    std::cout.rdbuf(old);
    return buffer.str();
}

int main() {
    // Test 1: Simple graph with two edges
    std::vector<std::pair<int,int>> e1 = {{0,1},{0,2}};
    assert(captureOutput(3,2,e1,1) == "Caso 1:\n  0-1 pathR(G,1)\n  0-2 pathR(G,2)\n\n");

    // Test 2: Graph with cycle and multiple starts
    std::vector<std::pair<int,int>> e2 = {{1,2},{2,1},{3,4}};
    std::string out2 = captureOutput(5,3,e2,2);
    // Expected: Caso 2: then start from 1, then from 3
    // For vertex 1: neighbor 2 -> print and recurse, then 2's neighbor 1 (visited) -> print plain
    // For vertex 3: neighbor 4 -> print and recurse, 4 has no outgoing (ignored)
    // Blank line after each start
    assert(out2 == "Caso 2:\n  1-2 pathR(G,2)\n    2-1\n\n  3-4 pathR(G,4)\n\n");

    // Test 3: No edges
    std::vector<std::pair<int,int>> e3 = {};
    assert(captureOutput(2,0,e3,3) == "Caso 3:\n");

    // Test 4: Self-loop
    std::vector<std::pair<int,int>> e4 = {{0,0}};
    assert(captureOutput(1,1,e4,4) == "Caso 4:\n  0-0\n\n");

    // Test 5: Duplicate edges and sorting neighbors
    std::vector<std::pair<int,int>> e5 = {{0,2},{0,1},{0,2}};
    std::string out5 = captureOutput(3,3,e5,5);
    // Neighbors sorted: 1,2,2. First 1 unvisited -> pathR, then 2 unvisited -> pathR, then second 2 visited -> plain
    assert(out5 == "Caso 5:\n  0-1 pathR(G,1)\n  0-2 pathR(G,2)\n  0-2\n\n");

    // Test 6: Vertex with no outgoing edges ignored
    std::vector<std::pair<int,int>> e6 = {{5,6}};
    // Only vertex 5 is source; vertex 6 is ignored as source
    assert(captureOutput(7,1,e6,6) == "Caso 6:\n  5-6 pathR(G,6)\n\n");

    return 0;
}
