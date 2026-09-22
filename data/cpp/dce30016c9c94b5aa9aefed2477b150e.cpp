Design a C++ function `std::pair<double, int> findFastestPath(const std::unordered_map<int, std::unordered_map<int, Edge>>& graph, int start, int end)` that takes a directed graph represented as an adjacency map where each edge stores a `distance` (in miles) and a `speed` (in miles per hour). The function should return a pair containing (1) the minimum travel time (in hours) from `start` to `end` using Dijkstra's algorithm with edge weight computed as `distance / speed`, and (2) the number of vertices on that fastest path (including both endpoints). If no path exists, return `{-1.0, -1}`. Assume all edge weights are positive and the graph may contain up to 10,000 vertices and 100,000 edges. Handle the case where `start == end` by returning `{0.0, 1}`. The function must be `const`-correct and should not modify the input graph.

The solution uses Dijkstra's algorithm with a min-priority queue keyed by accumulated travel time. Each edge's travel time is computed as `edge.distance / edge.speed`. We initialize distances to infinity, set the start node's distance to 0, and push `{0.0, start}` into the priority queue. While the queue is not empty, we pop the node with the smallest tentative distance, skip if already visited, and relax all outgoing edges: for each neighbor, if `currentDist + weight < dist[neighbor]`, update the distance and push the new pair. We also maintain a `prev` map to reconstruct the path, allowing us to count vertices. If we reach the end node, we can stop early for efficiency. Edge cases: if `start == end`, return `{0.0, 1}` immediately. If the end node is not reachable, return `{-1.0, -1}`. The graph may contain duplicate edges between the same pair of nodes, but because we use the adjacency map, duplicates are merged naturally. Time complexity is O((V+E) log V) due to heap operations, and space complexity is O(V) for distances, visited set, and prev map.

#include <queue>
#include <vector>
#include <unordered_map>
#include <limits>
#include <utility>

// Edge structure: distance in miles, speed in mph
struct Edge {
    double distance;
    double speed;
    double time() const { return distance / speed; }
};

// Function to compute the fastest path from start to end
// Returns {minimum travel time, number of vertices on path}
// If no path exists, returns {-1.0, -1}
std::pair<double, int> findFastestPath(
    const std::unordered_map<int, std::unordered_map<int, Edge>>& graph,
    int start,
    int end) {
    
    // Handle trivial case
    if (start == end) {
        return {0.0, 1};
    }
    
    // Check if start or end exist in graph
    if (graph.find(start) == graph.end() || graph.find(end) == graph.end()) {
        return {-1.0, -1};
    }
    
    // Distances from start to every node
    std::unordered_map<int, double> dist;
    // Predecessor map for path reconstruction
    std::unordered_map<int, int> prev;
    // Visited set
    std::unordered_map<int, bool> visited;
    
    // Initialize distances
    for (const auto& node : graph) {
        dist[node.first] = std::numeric_limits<double>::infinity();
        visited[node.first] = false;
    }
    
    // Min-heap: (distance, node)
    std::priority_queue<std::pair<double, int>,
                        std::vector<std::pair<double, int>>,
                        std::greater<std::pair<double, int>>> pq;
    
    dist[start] = 0.0;
    pq.push({0.0, start});
    
    while (!pq.empty()) {
        int current = pq.top().second;
        pq.pop();
        
        if (visited[current]) continue;
        visited[current] = true;
        
        // Early exit when we reach the destination
        if (current == end) break;
        
        for (const auto& edge : graph.at(current)) {
            int neighbor = edge.first;
            double travelTime = edge.second.time();
            
            if (dist[current] + travelTime < dist[neighbor]) {
                dist[neighbor] = dist[current] + travelTime;
                prev[neighbor] = current;
                pq.push({dist[neighbor], neighbor});
            }
        }
    }
    
    // If end node not reachable
    if (dist[end] == std::numeric_limits<double>::infinity()) {
        return {-1.0, -1};
    }
    
    // Reconstruct path to count vertices
    int vertexCount = 0;
    int current = end;
    while (current != start) {
        vertexCount++;
        current = prev[current];
    }
    vertexCount++; // for start node
    
    return {dist[end], vertexCount};
}

#include <cassert>
#include <unordered_map>

// Assume Edge and findFastestPath are defined above

int main() {
    // Build a simple graph: 1->2 (dist 10, speed 5 => time 2), 2->3 (dist 6, speed 3 => time 2)
    std::unordered_map<int, std::unordered_map<int, Edge>> g;
    g[1][2] = {10.0, 5.0};
    g[2][1] = {10.0, 5.0};
    g[2][3] = {6.0, 3.0};
    g[3][2] = {6.0, 3.0};
    
    // Test path from 1 to 3: total time 4.0, vertices 3
    auto result = findFastestPath(g, 1, 3);
    assert(result.first == 4.0);
    assert(result.second == 3);
    
    // Test start == end
    result = findFastestPath(g, 2, 2);
    assert(result.first == 0.0);
    assert(result.second == 1);
    
    // Test unreachable node
    g[4][5] = {1.0, 1.0};
    g[5][4] = {1.0, 1.0};
    result = findFastestPath(g, 1, 4);
    assert(result.first == -1.0);
    assert(result.second == -1);
    
    // Test nonexistent start or end
    result = findFastestPath(g, 99, 1);
    assert(result.first == -1.0);
    assert(result.second == -1);
    
    // Test graph with single edge
    std::unordered_map<int, std::unordered_map<int, Edge>> g2;
    g2[10][20] = {100.0, 50.0};
    g2[20][10] = {100.0, 50.0};
    result = findFastestPath(g2, 10, 20);
    assert(result.first == 2.0);
    assert(result.second == 2);
    
    return 0;
}
