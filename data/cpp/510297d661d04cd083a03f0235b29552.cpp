// Write a C++ function `std::string adjacencyListRepresentation(int n, const std::vector<std::pair<int,int>>& edges)` that takes the number of nodes `n` (nodes are labeled from 1 to `n` inclusive) and a vector of undirected edges (each edge is a pair `(u,v)` with `1 ≤ u,v ≤ n`), and returns a formatted string representing the adjacency list of the graph. The output string should have exactly `n` lines, one per node in increasing order of node number. For each node `i` (1 to `n`), the line must start with `"Node: i -> "` followed by the node's neighbors separated by a single space, and then a newline. Neighbors should appear in the order they were added when reading edges (i.e., the order in the input vector). If a node has no neighbors, the line should end immediately after the arrow (no trailing space). Assume the input is always valid (no duplicate edges, no self-loops, node numbers within 1..n). The function must handle the case `n=0` (return an empty string) and the case with no edges.

The core task is to build an adjacency list from the given edges and then serialize it into the required format. We can use a `std::vector<std::vector<int>>` of size `n+1` (index 0 unused). For each edge `(u,v)` in the input order, we push `v` into `Graph[u]` and `u` into `Graph[v]`. This preserves the order of neighbors as they appear in the input. After building, we iterate over node indices 1 through `n`. For each node, we start a string with `"Node: " + std::to_string(i) + " -> "`. Then we iterate over its neighbor list, appending each neighbor followed by a space, but we carefully avoid a trailing space: we can either build the list with spaces by adding a space before every neighbor except the first, or we can append a space after each neighbor and then trim the final trailing space before adding the newline. We must ensure that even when the neighbor list is empty, we still output the arrow and then a newline (no extra spaces). Edge cases: `n=0` – return empty string; `n>0` but no edges – each line becomes `"Node: i -> \n"` (arrow then newline). Time complexity: O(n + e) where `e` is the number of edges, since we process each edge twice (push back) and then output each neighbor once. Space complexity: O(n + e) for the adjacency list plus the output string size O(total neighbors).

#include <string>
#include <vector>
#include <utility>

// Build an adjacency list string for an undirected graph with nodes 1..n.
std::string adjacencyListRepresentation(int n, const std::vector<std::pair<int,int>>& edges) {
    // Handle invalid/empty node count.
    if (n <= 0) return "";

    // Build adjacency list: index 0 unused, nodes 1..n.
    std::vector<std::vector<int>> graph(n + 1);

    // Add edges in input order to preserve neighbor order.
    for (const auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;
        // assume valid input, so 1 <= u,v <= n
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    std::string result;
    for (int i = 1; i <= n; ++i) {
        result += "Node: " + std::to_string(i) + " -> ";
        const auto& neighbors = graph[i];
        for (size_t j = 0; j < neighbors.size(); ++j) {
            if (j > 0) result += " ";
            result += std::to_string(neighbors[j]);
        }
        result += "\n";
    }
    return result;
}

#include <cassert>
#include <string>
#include <vector>
#include <utility>

// The solution function is assumed to be declared above.

int main() {
    // Test 1: Simple 3-node graph with 2 edges.
    std::vector<std::pair<int,int>> e1 = {{1,2}, {2,3}};
    assert(adjacencyListRepresentation(3, e1) == "Node: 1 -> 2\nNode: 2 -> 1 3\nNode: 3 -> 2\n");

    // Test 2: Single node, no edges.
    assert(adjacencyListRepresentation(1, {}) == "Node: 1 -> \n");

    // Test 3: n=0 returns empty.
    assert(adjacencyListRepresentation(0, {}) == "");

    // Test 4: Multiple edges and nodes, order preservation.
    std::vector<std::pair<int,int>> e2 = {{1,5}, {2,3}, {2,1}, {4,4}}; // self-loop? invalid but we ignore; assume valid test instead.
    // Use valid edges: 5 nodes, edges: 1-5, 2-3, 2-1
    std::vector<std::pair<int,int>> e3 = {{1,5}, {2,3}, {2,1}};
    assert(adjacencyListRepresentation(5, e3) == "Node: 1 -> 5 2\nNode: 2 -> 3 1\nNode: 3 -> 2\nNode: 4 -> \nNode: 5 -> 1\n");

    // Test 5: Graph with 4 nodes and a complete triangle 1-2, 2-3, 3-1.
    std::vector<std::pair<int,int>> e4 = {{1,2}, {2,3}, {3,1}};
    assert(adjacencyListRepresentation(4, e4) == "Node: 1 -> 2 3\nNode: 2 -> 1 3\nNode: 3 -> 2 1\nNode: 4 -> \n");

    // Test 6: Large single edge, verify output format no trailing spaces.
    std::vector<std::pair<int,int>> e5 = {{7,8}};
    assert(adjacencyListRepresentation(8, e5) == "Node: 1 -> \nNode: 2 -> \nNode: 3 -> \nNode: 4 -> \nNode: 5 -> \nNode: 6 -> \nNode: 7 -> 8\nNode: 8 -> 7\n");

    return 0;
}
