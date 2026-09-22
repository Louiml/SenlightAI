// Design a C++ function that simulates a simplified version of the conversion-path planning system seen in the snippet. You are given a fixed directed graph with exactly 8 nodes (representing internal coordinate frame identifiers) and a fixed adjacency list (edges) defined as two parallel arrays: one for the primary adjacency list (called `a_edges`) and one for a mirrored adjacency list (called `b_edges`). The function must, given a source node index (0–7) and a destination node index (0–7), perform a breadth-first search (BFS) on the graph to compute the shortest path (in terms of number of edges) from source to destination. If a path exists, return the sequence of nodes along that path (including source and destination) as a `std::vector<int>`; if no path exists, return an empty vector. The graph is directed, and all edge lists are null‑terminated with a sentinel value `-1`. The function must be robust to invalid inputs (e.g., source or destination out of range) and must not modify the graph structure. Your solution must be self-contained, include only necessary headers (`<vector>`, `<queue>`, `<cstddef>`), and avoid any global state.

The core algorithm is a standard breadth-first search (BFS) on a small, fixed, directed graph. The graph has 8 nodes, indexed 0 to 7, and each node has an associated adjacency list (stored in two separate arrays—one for the “A” variant and one for the “B” variant). For simplicity, we merge these two arrays into one effective adjacency structure: we create a single array of 8 adjacency lists, where each list is obtained by combining the matching A and B lists (both are read into a single `std::vector<int>` of neighbors, preserving order). The BFS starts from the source node: we initialize a queue with the source, mark it visited, and set its distance to 0. While the queue is not empty, we pop a node, iterate over its neighbors, and for each unvisited neighbor, we set its distance to current distance + 1, record its predecessor (parent) as the current node, and push it into the queue. When the destination is discovered, we can reconstruct the path by walking back from the destination using the predecessor array. If the destination is never reached, we return an empty vector. Edge cases: if source or destination are outside [0,7], return empty vector; if source == destination, return a vector containing just that single node (even though the path of length 0 is valid); if there are cycles or multiple edges, BFS still guarantees the shortest path because all edges have unit weight. The graph is small (8 nodes), so the time complexity is O(V + E) where V = 8 and E is the total number of edges (at most a few dozen); space complexity is O(V) for the visited, distance, and parent arrays, plus the queue.

#include <vector>
#include <queue>
#include <cstddef>

// Sentinel for edge lists
const int kNoEdge = -1;

// Fixed-size graph with 8 nodes.
// We combine the A and B adjacency lists into one effective list per node.
// The arrays are defined as static const to reflect fixed graph data.
static const int a_edges[8][6] = {
    {1, 2, -1},          // node 0: neighbors 1,2
    {3, -1},             // node 1: neighbor 3
    {4, -1},             // node 2: neighbor 4
    {5, 6, -1},          // node 3: neighbors 5,6
    {7, -1},             // node 4: neighbor 7
    {0, -1},             // node 5: neighbor 0
    {7, -1},             // node 6: neighbor 7
    {-1}                 // node 7: no neighbors
};

static const int b_edges[8][6] = {
    {2, 3, -1},          // node 0: neighbors 2,3 (mirror variant)
    {0, 4, -1},          // node 1: neighbors 0,4
    {1, 5, -1},          // node 2: neighbors 1,5
    {-1},                // node 3: no neighbors
    {6, -1},             // node 4: neighbor 6
    {7, -1},             // node 5: neighbor 7
    {0, -1},             // node 6: neighbor 0
    {1, 2, -1}           // node 7: neighbors 1,2
};

// Combine A and B lists into a single vector of neighbors for each node.
static std::vector<std::vector<int>> buildAdjacency() {
    std::vector<std::vector<int>> adj(8);
    for (int i = 0; i < 8; ++i) {
        // Copy from A list
        for (int j = 0; a_edges[i][j] != kNoEdge; ++j) {
            adj[i].push_back(a_edges[i][j]);
        }
        // Copy from B list
        for (int j = 0; b_edges[i][j] != kNoEdge; ++j) {
            adj[i].push_back(b_edges[i][j]);
        }
    }
    return adj;
}

/**
 * Find the shortest directed path from source to destination in the
 * fixed 8-node graph described above.
 *
 * @param source      Index of the source node (0-7).
 * @param destination Index of the destination node (0-7).
 * @return A vector of node indices representing the shortest path
 *         (including source and destination) if it exists; otherwise
 *         an empty vector.
 */
std::vector<int> shortestConversionPath(int source, int destination) {
    // Validate input range
    if (source < 0 || source >= 8 || destination < 0 || destination >= 8) {
        return {};
    }

    // Build the adjacency structure once (static to avoid rebuilding each call)
    static const std::vector<std::vector<int>> adjacency = buildAdjacency();

    // BFS setup
    std::vector<bool> visited(8, false);
    std::vector<int> parent(8, -1);  // -1 means no parent
    std::queue<int> queue;

    visited[source] = true;
    queue.push(source);

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        // Early exit if we reached destination
        if (current == destination) {
            break;
        }

        for (int neighbor : adjacency[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = current;
                queue.push(neighbor);
            }
        }
    }

    // If destination not reached, no path
    if (!visited[destination]) {
        return {};
    }

    // Reconstruct path from destination back to source
    std::vector<int> path;
    for (int node = destination; node != -1; node = parent[node]) {
        path.push_back(node);
        if (node == source) break;
    }

    // Reverse to get source -> destination order
    std::vector<int> result(path.rbegin(), path.rend());
    return result;
}

#include <cassert>
#include <vector>

// The solution function is declared in the enclosing translation unit.
std::vector<int> shortestConversionPath(int source, int destination);

int main() {
    // Direct edge: 0 -> 1
    std::vector<int> p1 = shortestConversionPath(0, 1);
    assert(p1.size() == 2 && p1[0] == 0 && p1[1] == 1);

    // Path with multiple edges: 0 -> 2 -> 4 -> 7 (using A/B combined lists)
    std::vector<int> p2 = shortestConversionPath(0, 7);
    assert(p2.size() == 4);
    assert(p2[0] == 0 && p2[1] == 2 && p2[2] == 4 && p2[3] == 7);

    // Same source and destination should return single-node path
    std::vector<int> p3 = shortestConversionPath(3, 3);
    assert(p3.size() == 1 && p3[0] == 3);

    // No direct path from 7 to 7? Actually 7 to 7 is zero-length, but we allow it.
    // Test unreachable: 7 can only go to 1 or 2 (from B list), but 1 and 2 have no edges to 5? Let's check.
    // Node 5 has an edge to 0, and 0 can reach 1,2,3 (A) and 2,3 (B). So 5 can reach 7 via 0->2->4->7.
    // Node 7 has neighbors 1 and 2 (B list). Node 1 can go to 0 or 4 (B), 3 (A). Node 3 can go to 5,6 (A). Node 5 can go to 0 (A). So 7 can reach 5 via 7->1->0? Actually 1->0 exists (B), 0->2 (A) etc. So all nodes are reachable from all others? Check 6: 6->0 (B), so reachable. So all pairs likely reachable. Test a known reachable: 3->5 is direct (A list). 
    std::vector<int> p4 = shortestConversionPath(3, 5);
    assert(p4.size() == 2 && p4[0] == 3 && p4[1] == 5);

    // Test a longer path: 5 -> 7 via 5->0 (A), then 0->2 (A), 2->4 (A), 4->7 (A)
    std::vector<int> p5 = shortestConversionPath(5, 7);
    assert(p5.size() == 5);
    assert(p5[0] == 5 && p5.back() == 7);

    // Test invalid input
    std::vector<int> p6 = shortestConversionPath(-1, 0);
    assert(p6.empty());
    std::vector<int> p7 = shortestConversionPath(0, 100);
    assert(p7.empty());

    // Test unreachable? In this graph, all nodes are reachable from any node? 
    // Let's verify: node 7 has no outgoing edges in A list, but B list gives 1,2. So 7 can reach 1 and 2. 
    // Node 1 reaches 0,3,4; node 2 reaches 1,4,5; all eventually reach all others. So no unreachable case possible with this data.
    // But we still test a case where path length might be larger: 6 -> 3
    std::vector<int> p8 = shortestConversionPath(6, 3);
    assert(p8.front() == 6 && p8.back() == 3);
    assert(p8.size() >= 2);

    return 0;
}
