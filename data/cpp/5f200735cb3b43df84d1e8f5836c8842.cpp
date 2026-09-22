// Write a C++ function that performs a breadth-first traversal of an undirected graph starting from vertex 0, returning the order in which vertices are first visited. The graph is represented using an adjacency list of `vector<int>` where the number of vertices `V` and an array `adj` of vectors are provided as function parameters. The traversal must visit all vertices reachable from vertex 0, and when multiple neighbors exist for a vertex, they must be visited in the order they appear in the adjacency list. The input graph is guaranteed to contain at least one vertex, and vertex indices range from 0 to V-1. If vertex 0 is isolated (no edges), the result should contain only vertex 0. The function should return a `vector<int>` containing the BFS traversal order.
The solution uses the standard BFS algorithm with an explicit queue. Start by initializing a visited boolean array (or vector) of size V all set to false, then mark vertex 0 as visited and push it into the queue. While the queue is not empty, pop the front vertex, append it to the result, and iterate through all its neighbors in the adjacency list. For each neighbor that has not been visited yet, mark it as visited immediately and push it into the queue. This immediate marking prevents duplicate enqueues and ensures each vertex appears exactly once in the result. The reason BFS produces a level-order traversal is the FIFO nature of the queue, exploring all vertices at distance 1 before distance 2, and so on. Edge cases include: V=1 (returns just {0}), a disconnected graph where some vertices are unreachable from 0 (those are simply not included), and an empty adjacency list for some vertices (they simply contribute no new neighbors). The time complexity is O(V + E) where E is the total number of edges, because each vertex is enqueued and dequeued at most once, and each edge is examined once when its source vertex is processed. The space complexity is O(V) for the visited array, the queue (which can hold up to V vertices), and the result vector.
#include <vector>
#include <queue>

// Perform breadth-first traversal of an undirected graph starting from vertex 0.
// adj is an array of vectors where adj[i] contains the neighbors of vertex i.
// Returns a vector<int> with the order in which vertices are first visited.
std::vector<int> bfsTraversal(int V, const std::vector<int> adj[]) {
    std::vector<int> result;
    std::vector<bool> visited(V, false);
    std::queue<int> q;
    
    // Start BFS from vertex 0
    visited[0] = true;
    q.push(0);
    
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        result.push_back(current);
        
        for (int neighbor : adj[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple linear graph 0-1-2
    std::vector<int> adj1[3];
    adj1[0] = {1};
    adj1[1] = {0, 2};
    adj1[2] = {1};
    std::vector<int> result1 = bfsTraversal(3, adj1);
    assert((result1 == std::vector<int>{0, 1, 2}));

    // Test 2: Star graph centered at 0 with neighbors 1,2,3
    std::vector<int> adj2[4];
    adj2[0] = {1, 2, 3};
    adj2[1] = {0};
    adj2[2] = {0};
    adj2[3] = {0};
    std::vector<int> result2 = bfsTraversal(4, adj2);
    assert((result2 == std::vector<int>{0, 1, 2, 3}));

    // Test 3: Single vertex
    std::vector<int> adj3[1];
    std::vector<int> result3 = bfsTraversal(1, adj3);
    assert((result3 == std::vector<int>{0}));

    // Test 4: Disconnected graph - vertex 0 isolated, vertex 1 connected to 2
    std::vector<int> adj4[3];
    adj4[1] = {2};
    adj4[2] = {1};
    std::vector<int> result4 = bfsTraversal(3, adj4);
    assert((result4 == std::vector<int>{0}));

    // Test 5: More complex graph with multiple levels
    std::vector<int> adj5[5];
    adj5[0] = {1, 2};
    adj5[1] = {0, 3};
    adj5[2] = {0, 4};
    adj5[3] = {1};
    adj5[4] = {2};
    std::vector<int> result5 = bfsTraversal(5, adj5);
    assert((result5 == std::vector<int>{0, 1, 2, 3, 4}));

    // Test 6: Graph where neighbor order matters
    std::vector<int> adj6[4];
    adj6[0] = {2, 1};  // Visit 2 before 1
    adj6[2] = {0, 3};
    adj6[1] = {0, 3};
    adj6[3] = {1, 2};
    std::vector<int> result6 = bfsTraversal(4, adj6);
    assert((result6 == std::vector<int>{0, 2, 1, 3}));

    // Test 7: Cycle 0-1-2-0
    std::vector<int> adj7[3];
    adj7[0] = {1, 2};
    adj7[1] = {0, 2};
    adj7[2] = {1, 0};
    std::vector<int> result7 = bfsTraversal(3, adj7);
    assert((result7 == std::vector<int>{0, 1, 2}));

    // Test 8: Larger graph with two parallel paths
    std::vector<int> adj8[6];
    adj8[0] = {1, 3};
    adj8[1] = {0, 2};
    adj8[2] = {1};
    adj8[3] = {0, 4, 5};
    adj8[4] = {3};
    adj8[5] = {3};
    std::vector<int> result8 = bfsTraversal(6, adj8);
    assert((result8 == std::vector<int>{0, 1, 3, 2, 4, 5}));

    return 0;
}
