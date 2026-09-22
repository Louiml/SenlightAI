Write a standalone C++ function that performs a Breadth-First Search (BFS) traversal on an undirected graph represented by an adjacency list, starting from node 0. The graph is guaranteed to be connected (so all nodes will be reachable from node 0). The function must return a vector of integers containing the order in which nodes are visited. The graph can have up to 10,000 nodes, and edges are undirected, meaning that if an edge connects `u` and `v`, it appears in both `adj[u]` and `adj[v]`. The function should correctly handle graphs where nodes have multiple edges or self-loops (if present, they should be ignored after the first visit). The traversal must follow the standard BFS algorithm: visit the starting node, then visit all its neighbors, then the neighbors' neighbors, etc., using a queue. The returned order is determined by the order in which neighbors appear in the adjacency list (i.e., for each node, neighbors are processed in the order they are stored). The function signature should be `std::vector<int> bfsTraversal(int V, const std::vector<int> adj[])`, where `V` is the number of nodes (labeled from 0 to V-1). The function must be `const`-correct: the adjacency list is passed as `const` to indicate it will not be modified. Do not include a `main` function in the solution; that will be provided separately for testing.

The algorithm uses a queue to process nodes level by level. We maintain a boolean visited array of size `V`, initialized to `false`. We start by marking node 0 as visited and pushing it into the queue. Then, in a loop, we pop the front node, add it to the result vector, and iterate over all its neighbors in the adjacency list. For each neighbor, if it has not been visited yet, we mark it as visited and push it into the queue. This ensures each node is enqueued exactly once. Since the graph is connected, the BFS will visit all nodes. Edge cases include: a graph with only one node (return `{0}`), graphs with multiple edges between the same pair (handled by the visited check), and self-loops (handled similarly). Time complexity is O(V + E) because each node is enqueued/dequeued once and each edge is examined once (from both endpoints, but that's still O(E) since each edge contributes two adjacency entries). Space complexity is O(V) for the visited array and the queue, plus O(V) for the result vector.

#include <vector>
#include <queue>

// Perform BFS traversal starting from node 0 on an undirected graph.
// The graph is given as an array of adjacency lists, where adj[i] contains
// all neighbors of node i. The graph is assumed to be connected.
std::vector<int> bfsTraversal(int V, const std::vector<int> adj[]) {
    std::vector<bool> visited(V, false);
    std::queue<int> q;
    std::vector<int> bfsOrder;

    // Start from node 0
    visited[0] = true;
    q.push(0);

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        bfsOrder.push_back(node);

        // Visit all unvisited neighbors
        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    return bfsOrder;
}

#include <cassert>
#include <vector>

// Solution function is assumed to be declared above.
// We include a minimal main for testing.
int main() {
    // Test 1: Simple connected graph: 0-1, 1-2, 2-3, 0-4, 3-1 (same as in snippet)
    {
        std::vector<int> adj[5];
        adj[0] = {1, 4};
        adj[1] = {0, 2, 3};
        adj[2] = {1, 3};
        adj[3] = {2, 1};
        adj[4] = {0};
        std::vector<int> result = bfsTraversal(5, adj);
        // Expected: 0, then neighbors of 0 (1, 4), then neighbors of 1 (2, 3, but 3 may come after 2)
        // Since adj[0] = {1,4}, adj[1] = {0,2,3}, order is 0,1,4,2,3
        std::vector<int> expected = {0, 1, 4, 2, 3};
        assert(result == expected);
    }

    // Test 2: Single node graph
    {
        std::vector<int> adj[1]; // no edges
        std::vector<int> result = bfsTraversal(1, adj);
        std::vector<int> expected = {0};
        assert(result == expected);
    }

    // Test 3: Multi-edge and self-loop
    {
        std::vector<int> adj[3];
        adj[0] = {1, 1, 0}; // self-loop and duplicate edge
        adj[1] = {0, 2};
        adj[2] = {1};
        std::vector<int> result = bfsTraversal(3, adj);
        std::vector<int> expected = {0, 1, 2};
        assert(result == expected);
    }

    // Test 4: Graph where node order matters (neighbors in given order)
    {
        std::vector<int> adj[4];
        adj[0] = {2, 1};
        adj[1] = {0, 3};
        adj[2] = {0, 3};
        adj[3] = {1, 2};
        std::vector<int> result = bfsTraversal(4, adj);
        // BFS: 0, then 2, then 1 (since adj[0] = {2,1}), then from 2 we get 3, but 3 is already visited via 1? Actually 2's neighbors: 0 (visited), 3 (unvisited) => push 3. Then queue: [1,3]; pop 1, neighbors 0 visited, 3 unvisited? 3 is already in queue but not visited? Actually we mark visited when we enqueue, so 3 was marked when enqueued from 2. So pop 1, its neighbors 0 and 3 both visited, nothing. Then pop 3, done. Order: 0,2,1,3.
        std::vector<int> expected = {0, 2, 1, 3};
        assert(result == expected);
    }

    // Test 5: Larger connected graph, linear chain
    {
        std::vector<int> adj[4];
        adj[0] = {1};
        adj[1] = {0, 2};
        adj[2] = {1, 3};
        adj[3] = {2};
        std::vector<int> result = bfsTraversal(4, adj);
        std::vector<int> expected = {0, 1, 2, 3};
        assert(result == expected);
    }

    return 0;
}
