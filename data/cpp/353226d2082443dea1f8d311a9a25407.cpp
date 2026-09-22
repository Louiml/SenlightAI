Write a C++ function that loads an undirected graph from a text file whose first line contains two integers (number of vertices and number of edges), followed by one line per edge containing two vertex labels (as integers, non-negative, arbitrarily large), and returns the shortest path length (as the number of edges) between two specified vertices `start` and `end`. The graph is guaranteed to be connected. The function should take the filename, start vertex, and end vertex as parameters, and return `-1` if either vertex does not exist in the graph or if the file cannot be opened. For simplicity, vertex labels are arbitrary integers and the graph is undirected.
The solution requires reading the file, building an adjacency list (using `std::map<int, std::vector<int>>` to handle arbitrary, non-contiguous vertex labels), and performing a breadth-first search (BFS) from the start vertex to find the shortest path length to the end vertex. BFS is optimal for unweighted graphs. Edge cases: file not found (return -1), start or end vertex not in the graph (return -1), start equals end (return 0). Time complexity is O(V + E) for BFS, plus O(E log V) for building the adjacency list due to the map's logarithmic insertions. Space complexity is O(V + E) for storing the graph and BFS queue/distance map. Since the graph is connected, BFS will always find the target, but we still check for existence up front.
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <queue>
#include <string>

// Returns the shortest path length (number of edges) between 'start' and 'end'
// in an undirected graph loaded from 'filename'. Returns -1 on file error
// or if a vertex does not exist in the graph.
int shortestPathLength(const std::string& filename, int start, int end) {
    std::ifstream in(filename);
    if (!in) {
        return -1;
    }

    int numVertices, numEdges;
    if (!(in >> numVertices >> numEdges)) {
        return -1;
    }

    // Use a map of int -> vector<int> to handle arbitrary vertex labels.
    std::map<int, std::vector<int>> adj;
    for (int i = 0; i < numEdges; ++i) {
        int u, v;
        if (!(in >> u >> v)) {
            return -1; // malformed edge line
        }
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Check that both vertices exist.
    if (adj.find(start) == adj.end() || adj.find(end) == adj.end()) {
        return -1;
    }

    if (start == end) {
        return 0;
    }

    // BFS to find the shortest path.
    std::queue<int> q;
    std::map<int, int> dist; // vertex -> distance from start
    q.push(start);
    dist[start] = 0;

    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        for (int neighbor : adj[cur]) {
            if (dist.find(neighbor) == dist.end()) {
                dist[neighbor] = dist[cur] + 1;
                if (neighbor == end) {
                    return dist[neighbor];
                }
                q.push(neighbor);
            }
        }
    }

    // Graph is connected, so this should never happen, but return a sentinel.
    return -1;
}
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

// Declare the function (since it's defined separately).
int shortestPathLength(const std::string& filename, int start, int end);

int main() {
    // Create a temporary test file.
    std::ofstream out("test_graph.txt");
    out << "4 3\n";
    out << "0 1\n";
    out << "1 2\n";
    out << "2 3\n";
    out.close();

    // Basic path length.
    assert(shortestPathLength("test_graph.txt", 0, 3) == 3);
    assert(shortestPathLength("test_graph.txt", 1, 1) == 0);
    assert(shortestPathLength("test_graph.txt", 1, 3) == 2);

    // Test with non-contiguous vertex labels and multiple edges.
    std::ofstream out2("test_graph2.txt");
    out2 << "5 4\n";
    out2 << "10 20\n";
    out2 << "20 30\n";
    out2 << "30 40\n";
    out2 << "40 50\n";
    out2.close();
    assert(shortestPathLength("test_graph2.txt", 10, 50) == 4);
    assert(shortestPathLength("test_graph2.txt", 30, 10) == 2);

    // Test missing vertices and missing file.
    assert(shortestPathLength("test_graph.txt", 0, 99) == -1);
    assert(shortestPathLength("test_graph.txt", 99, 0) == -1);
    assert(shortestPathLength("nonexistent_file.txt", 0, 1) == -1);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
