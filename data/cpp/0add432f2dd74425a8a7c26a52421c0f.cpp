// Write a C++ function that takes a positive integer `V` (number of vertices) and an adjacency list `adj` (represented as `vector<int> adj[]`, where `adj[i]` contains the neighbors of vertex `i` in an undirected graph), and returns a `vector<int>` containing the vertices in the order they are visited by a Depth-First Search (DFS) starting from vertex `0`. The graph may be disconnected, meaning that some vertices may not be reachable from vertex `0`. However, your function should only collect vertices reachable from vertex `0` (i.e., perform a single DFS traversal from the source, not a full forest traversal). The vertices in the adjacency list are not necessarily sorted, and the order of visiting depends on the order of neighbors as given. Handle the case where `V` is 0 (return an empty vector) and where the graph has no edges (return just `{0}` if `V > 0`). You must implement the DFS recursively or iteratively, but must not use global or static variables.
The solution uses a recursive DFS helper function that marks a vertex as visited when it is first encountered, appends it to the result vector, then iterates over all its neighbors in the order they appear in the adjacency list. For each unvisited neighbor, it recursively performs DFS. The main function initializes a `vector<bool> visited` of size `V` set to `false`, and a result vector. It then calls the helper starting from vertex `0`. Edge cases: if `V == 0`, return an empty vector immediately (since vertex 0 does not exist). If `V > 0` but vertex 0 has no neighbors, the function returns `{0}`. Because the graph may be disconnected, vertices not reachable from 0 are simply never visited, and the result contains only the reachable component. Time complexity is `O(V + E)` where `E` is the total number of edges in the entire graph, since we iterate over each adjacency list of visited vertices exactly once (but we may check unvisited neighbors from visited vertices; all edges from reachable vertices are examined). Space complexity is `O(V)` for the visited array and the recursion stack in the worst case (a chain graph). The result vector itself takes `O(V)` space. No additional data structures are needed.
#include <vector>
#include <functional>

// Perform DFS from vertex 0 and return the traversal order.
std::vector<int> dfsFromSource(int V, const std::vector<int> adj[]) {
    std::vector<int> result;
    if (V == 0) {
        return result; // No vertices
    }

    std::vector<bool> visited(V, false);
    std::function<void(int)> dfs = [&](int u) {
        visited[u] = true;
        result.push_back(u);
        for (int v : adj[u]) {
            if (!visited[v]) {
                dfs(v);
            }
        }
    };

    dfs(0); // Start from vertex 0
    return result;
}
#include <cassert>
#include <vector>

// The solution function is declared above (assume it is available).

int main() {
    // Test 1: Simple chain 0-1-2
    {
        std::vector<int> adj[3];
        adj[0].push_back(1);
        adj[1].push_back(2);
        std::vector<int> result = dfsFromSource(3, adj);
        assert(result == std::vector<int>({0, 1, 2}));
    }

    // Test 2: Disconnected graph, only vertex 0 reachable
    {
        std::vector<int> adj[5];
        adj[1].push_back(2); // component not connected to 0
        adj[2].push_back(1);
        std::vector<int> result = dfsFromSource(5, adj);
        assert(result == std::vector<int>({0}));
    }

    // Test 3: Star graph with 0 at center
    {
        std::vector<int> adj[4];
        adj[0] = {1, 2, 3};
        adj[1].push_back(0);
        adj[2].push_back(0);
        adj[3].push_back(0);
        std::vector<int> result = dfsFromSource(4, adj);
        // Order depends on neighbor order: visits 0, then 1, then 2, then 3
        assert(result == std::vector<int>({0, 1, 2, 3}));
    }

    // Test 4: V=0
    {
        std::vector<int> adj[0];
        std::vector<int> result = dfsFromSource(0, adj);
        assert(result.empty());
    }

    // Test 5: V=1, no edges
    {
        std::vector<int> adj[1];
        std::vector<int> result = dfsFromSource(1, adj);
        assert(result == std::vector<int>({0}));
    }

    // Test 6: More complex graph with branching
    {
        std::vector<int> adj[6];
        adj[0] = {1, 2};
        adj[1] = {0, 3};
        adj[2] = {0, 4};
        adj[3] = {1, 5};
        adj[4] = {2};
        adj[5] = {3};
        std::vector<int> result = dfsFromSource(6, adj);
        // Expected DFS order: 0, 1, 3, 5, 2, 4
        assert(result == std::vector<int>({0, 1, 3, 5, 2, 4}));
    }

    // Test 7: Graph with cycle
    {
        std::vector<int> adj[4];
        adj[0] = {1};
        adj[1] = {0, 2, 3};
        adj[2] = {1};
        adj[3] = {1};
        std::vector<int> result = dfsFromSource(4, adj);
        assert(result == std::vector<int>({0, 1, 2, 3}));
    }

    // Test 8: Node 0 not first in adjacency
    {
        std::vector<int> adj[3];
        adj[1] = {2};
        adj[2] = {1};
        // Vertex 0 isolated
        std::vector<int> result = dfsFromSource(3, adj);
        assert(result == std::vector<int>({0}));
    }

    return 0;
}
