/*
Write a C++ function `vector<int> shortestPathDAG(const vector<vector<pair<int, int>>>& adj, int src)` that computes the shortest path distances from a given source node to all other nodes in a directed acyclic graph (DAG). The graph is represented as an adjacency list where `adj[u]` contains pairs `(v, weight)` for each directed edge from `u` to `v`. The function must handle negative edge weights, which are valid in DAGs, and return a vector of distances where unreachable nodes are represented as `INT_MAX`. The graph has nodes numbered from 0 to `n-1` where `n = adj.size()`. The function must use topological sort and edge relaxation in topological order to achieve linear time complexity. Assume the input graph is always a valid DAG (no cycles) and contains at least one node.
*/
#include <vector>
#include <stack>
#include <limits>
#include <unordered_map>

// Helper function for topological sort using DFS
void topoSortDFS(int node, const std::vector<std::vector<std::pair<int, int>>>& adj, 
                 std::unordered_map<int, bool>& visited, std::stack<int>& topo) {
    visited[node] = true;
    for (const auto& edge : adj[node]) {
        int neighbor = edge.first;
        if (!visited[neighbor]) {
            topoSortDFS(neighbor, adj, visited, topo);
        }
    }
    topo.push(node);
}

// Compute shortest paths from src in a DAG using topological ordering
std::vector<int> shortestPathDAG(const std::vector<std::vector<std::pair<int, int>>>& adj, int src) {
    int n = adj.size();
    std::vector<int> distance(n, std::numeric_limits<int>::max());
    distance[src] = 0;

    // Step 1: Perform topological sort
    std::stack<int> topo;
    std::unordered_map<int, bool> visited;
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            topoSortDFS(i, adj, visited, topo);
        }
    }

    // Step 2: Process nodes in topological order and relax edges
    while (!topo.empty()) {
        int u = topo.top();
        topo.pop();
        if (distance[u] != std::numeric_limits<int>::max()) {
            for (const auto& edge : adj[u]) {
                int v = edge.first;
                int weight = edge.second;
                if (distance[u] + weight < distance[v]) {
                    distance[v] = distance[u] + weight;
                }
            }
        }
    }

    return distance;
}
#include <cassert>
#include <vector>
#include <utility>
#include <limits>

// Declaration of the function to test (assume it's included from solution)
std::vector<int> shortestPathDAG(const std::vector<std::vector<std::pair<int, int>>>& adj, int src);

int main() {
    // Test case 1: Simple DAG with positive weights
    {
        std::vector<std::vector<std::pair<int, int>>> adj(4);
        adj[0] = {{1, 5}, {2, 3}};
        adj[1] = {{2, 2}, {3, 6}};
        adj[2] = {{3, 7}};
        auto result = shortestPathDAG(adj, 0);
        std::vector<int> expected = {0, 5, 3, 10};
        assert(result == expected);
    }

    // Test case 2: DAG with negative weights
    {
        std::vector<std::vector<std::pair<int, int>>> adj(6);
        adj[0] = {{1, 5}, {2, 3}};
        adj[1] = {{2, 2}, {3, 6}};
        adj[2] = {{3, 7}, {4, 4}, {5, 2}};
        adj[3] = {{4, -1}};
        adj[4] = {{5, -2}};
        auto result = shortestPathDAG(adj, 1);
        std::vector<int> expected = {INT_MAX, 0, 2, 6, 5, 3};
        assert(result == expected);
    }

    // Test case 3: Source unreachable to some nodes
    {
        std::vector<std::vector<std::pair<int, int>>> adj(3);
        adj[0] = {{1, 1}};
        adj[2] = {}; // isolated node
        auto result = shortestPathDAG(adj, 0);
        std::vector<int> expected = {0, 1, INT_MAX};
        assert(result == expected);
    }

    // Test case 4: Single node
    {
        std::vector<std::vector<std::pair<int, int>>> adj(1);
        auto result = shortestPathDAG(adj, 0);
        std::vector<int> expected = {0};
        assert(result == expected);
    }

    // Test case 5: Disconnected but reachable chain with negative edge
    {
        std::vector<std::vector<std::pair<int, int>>> adj(3);
        adj[0] = {{1, -5}};
        adj[1] = {{2, 3}};
        auto result = shortestPathDAG(adj, 0);
        std::vector<int> expected = {0, -5, -2};
        assert(result == expected);
    }

    return 0;
}
// The solution uses a two-phase approach: first perform a topological sort of the DAG using depth-first search (DFS), then process nodes in topological order to relax all outgoing edges. Topological ordering ensures that when we process a node, all its predecessors have already been processed, so the distance to it is final. The algorithm starts by initializing all distances to `INT_MAX` except the source, which is set to 0. Then for each node in topological order, if its current distance is not infinity, we relax all its outgoing edges by checking if `distance[u] + weight` is less than the current `distance[v]`. Since the graph is a DAG, this processes each edge exactly once, and all paths are considered correctly. Negative weights are handled naturally because there are no cycles to cause infinite loops; the topological order guarantees no path is revisited incorrectly. Edge cases include a source node that may not reach all other nodes (those remain `INT_MAX`), negative weight edges that may produce shorter paths, and isolated nodes. Time complexity is O(V + E) for the topological sort and O(V + E) for relaxation, giving a total of O(V + E). Space complexity is O(V) for the visited map, stack, and distance vector.
