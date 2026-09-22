/*
Write a C++ function `int minimumCostPath(int numNodes, const std::vector<std::vector<int>>& edges, int startNode, int targetNode)` that takes a positive integer number of nodes (nodes are labeled 1 through `numNodes`), a list of undirected edges where each edge is a vector of three integers `{from, to, price}`, a start node, and a target node. The function must return the minimum total price to travel from `startNode` to `targetNode` along the edges. All prices are positive integers, and the graph is connected. If `startNode == targetNode`, return 0. The function should handle up to 50,000 nodes and 100,000 edges efficiently. Assume there is always at least one path between any two nodes.
*/
#include <vector>
#include <queue>
#include <climits>

// Returns the minimum total price from startNode to targetNode.
// Edges are given as {from, to, price}; graph is undirected.
int minimumCostPath(int numNodes,
                    const std::vector<std::vector<int>>& edges,
                    int startNode,
                    int targetNode) {
    if (startNode == targetNode) return 0;

    // Build adjacency list: for each node, list of {price, neighbor}.
    std::vector<std::vector<std::pair<int, int>>> adj(numNodes + 1);
    for (const auto& e : edges) {
        int from = e[0], to = e[1], price = e[2];
        adj[from].push_back({price, to});
        adj[to].push_back({price, from});
    }

    // Distance array, initialize to large value
    std::vector<int> dist(numNodes + 1, INT_MAX / 2);
    dist[startNode] = 0;

    // Min-heap: {distance, node}
    using P = std::pair<int, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({0, startNode});

    while (!pq.empty()) {
        auto [currentDist, currentNode] = pq.top();
        pq.pop();

        // Skip stale entries
        if (currentDist != dist[currentNode]) continue;

        // Early exit when we pop the target
        if (currentNode == targetNode) break;

        for (const auto& [price, neighbor] : adj[currentNode]) {
            int newDist = currentDist + price;
            if (newDist < dist[neighbor]) {
                dist[neighbor] = newDist;
                pq.push({newDist, neighbor});
            }
        }
    }

    return dist[targetNode];
}
#include <cassert>
#include <vector>

int minimumCostPath(int numNodes,
                    const std::vector<std::vector<int>>& edges,
                    int startNode,
                    int targetNode);

int main() {
    // Basic 3-node triangle
    std::vector<std::vector<int>> e1 = {{1,2,5},{2,3,3},{1,3,10}};
    assert(minimumCostPath(3, e1, 1, 3) == 8);  // 1-2-3

    // Start equals target
    assert(minimumCostPath(2, {{1,2,7}}, 1, 1) == 0);

    // Two parallel edges, pick the cheaper
    std::vector<std::vector<int>> e2 = {{1,2,4},{1,2,2}};
    assert(minimumCostPath(2, e2, 1, 2) == 2);

    // Longer path with a detour
    std::vector<std::vector<int>> e3 = {{1,2,1},{2,3,1},{3,4,1},{1,4,100}};
    assert(minimumCostPath(4, e3, 1, 4) == 3);

    // Larger graph with 5 nodes
    std::vector<std::vector<int>> e4 = {{1,2,10},{2,3,20},{3,5,5},
                                        {1,4,2},{4,5,3}};
    assert(minimumCostPath(5, e4, 1, 5) == 5); // 1-4-5

    // Disconnected would be invalid, but ensure no crash on single edge
    std::vector<std::vector<int>> e5 = {{1,2,9}};
    assert(minimumCostPath(2, e5, 2, 1) == 9);

    // Self-loop does not improve
    std::vector<std::vector<int>> e6 = {{1,1,3},{1,2,6}};
    assert(minimumCostPath(2, e6, 1, 2) == 6);
}
// The problem is a classic shortest-path on an undirected weighted graph with non‑negative weights. The optimal algorithm is Dijkstra’s algorithm using a priority queue (min‑heap) to always expand the node with the smallest current known distance. Initialize a distance array with a large value (e.g., `INT_MAX / 2`) for all nodes except the start node, which gets distance 0. Push `{0, startNode}` into the priority queue. While the queue is not empty, pop the pair with smallest distance; if the popped distance is greater than the stored distance for that node, skip it (lazy deletion). For each neighbor, if the new distance through the current node is smaller than the stored distance, update and push `{newDistance, neighbor}`. Since all edge weights are positive, Dijkstra’s algorithm is correct and will terminate when we pop the target node (or when the queue is empty). Edge cases: start equals target → return 0 immediately; duplicate edges between the same pair – the algorithm handles them naturally by considering all; self‑loops are allowed but harmless because they never improve the distance. Time complexity is \(O((V+E) \log V)\) due to heap operations; space complexity is \(O(V+E)\) for the adjacency list and distance array.
