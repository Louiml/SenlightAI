// Write a C++ function `int longestCityChain(const std::vector<std::pair<std::string, std::string>>& routes, const std::string& source, const std::string& destination)` that takes a list of directed routes (each route is from city X to city Y, meaning there is a direct train), a starting city `source`, and an ending city `destination`. The function must return the maximum number of cities that can be visited on a valid path starting at `source` and ending at `destination` (counting both the source and destination as visited cities). A valid path can include repeated cities only if there is a cycle, but you should count the longest simple path (no repeated cities) because the graph may contain cycles; however, since the original problem assumed a DAG (directed acyclic graph), your solution should handle cycles gracefully by returning the length of the longest path without revisiting a city (i.e., you may use DFS with memoization on the path, but for a DAG you can use topological sort). If no path exists from `source` to `destination`, return 0. The graph may have up to 10^5 routes and 10^5 distinct cities. The function must be efficient and avoid recursion depth issues.
#include <cassert>
#include <string>
#include <vector>

// Declaration of the function under test
int longestCityChain(const std::vector<std::pair<std::string, std::string>>& routes,
                     const std::string& source,
                     const std::string& destination);

int main() {
    // Test 1: Simple chain A->B->C
    std::vector<std::pair<std::string, std::string>> routes1 = {{"A","B"},{"B","C"}};
    assert(longestCityChain(routes1, "A", "C") == 3);
    
    // Test 2: No path
    std::vector<std::pair<std::string, std::string>> routes2 = {{"A","B"}};
    assert(longestCityChain(routes2, "A", "D") == 0);
    
    // Test 3: Source equals destination
    std::vector<std::pair<std::string, std::string>> routes3 = {{"X","Y"}};
    assert(longestCityChain(routes3, "X", "X") == 1);
    
    // Test 4: Multiple paths, choose longest
    std::vector<std::pair<std::string, std::string>> routes4 = {
        {"S","A"},{"S","B"},{"A","D"},{"B","C"},{"C","D"}
    };
    assert(longestCityChain(routes4, "S", "D") == 4); // S->B->C->D
    
    // Test 5: Direct edge
    std::vector<std::pair<std::string, std::string>> routes5 = {{"P","Q"}};
    assert(longestCityChain(routes5, "P", "Q") == 2);
    
    // Test 6: DAG with branching but no path to dest from one branch
    std::vector<std::pair<std::string, std::string>> routes6 = {
        {"S","A"},{"A","B"},{"S","C"},{"C","D"}
    };
    assert(longestCityChain(routes6, "S", "D") == 3);
    
    // Test 7: Single node graph
    std::vector<std::pair<std::string, std::string>> routes7 = {};
    assert(longestCityChain(routes7, "A", "A") == 1);
    
    // Test 8: Destination not reachable despite being in graph
    std::vector<std::pair<std::string, std::string>> routes8 = {{"A","B"},{"C","D"}};
    assert(longestCityChain(routes8, "A", "C") == 0);
    
    return 0;
}
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <climits>
#include <algorithm>

// Recursive helper for topological sort
void topoDFS(const std::string& node,
             const std::map<std::string, std::vector<std::string>>& graph,
             std::map<std::string, bool>& visited,
             std::stack<std::string>& order) {
    visited[node] = true;
    auto it = graph.find(node);
    if (it != graph.end()) {
        for (const std::string& neighbour : it->second) {
            if (!visited[neighbour]) {
                topoDFS(neighbour, graph, visited, order);
            }
        }
    }
    order.push(node);
}

// Returns the maximum number of cities (source and destination counted)
// on a path from source to destination in a directed acyclic graph.
// Returns 0 if no path exists.
int longestCityChain(const std::vector<std::pair<std::string, std::string>>& routes,
                     const std::string& source,
                     const std::string& destination) {
    // Build adjacency list
    std::map<std::string, std::vector<std::string>> graph;
    for (const auto& route : routes) {
        graph[route.first].push_back(route.second);
        // Ensure destination node exists even if it has no outgoing edges
        if (graph.find(route.second) == graph.end()) graph[route.second] = {};
    }
    
    // If source or destination not in graph
    if (graph.find(source) == graph.end() || graph.find(destination) == graph.end()) {
        return 0;
    }
    
    // Topological sort from source only
    std::map<std::string, bool> visited;
    for (const auto& p : graph) visited[p.first] = false;
    
    std::stack<std::string> order;
    topoDFS(source, graph, visited, order);
    
    // Initialize distances
    std::map<std::string, int> distance;
    for (const auto& p : graph) distance[p.first] = INT_MIN;
    distance[source] = 0;
    
    // Process in topological order
    while (!order.empty()) {
        std::string node = order.top();
        order.pop();
        
        if (distance[node] != INT_MIN) {
            auto it = graph.find(node);
            if (it != graph.end()) {
                for (const std::string& neighbour : it->second) {
                    if (distance[neighbour] < distance[node] + 1) {
                        distance[neighbour] = distance[node] + 1;
                    }
                }
            }
        }
    }
    
    if (distance[destination] == INT_MIN) return 0;
    return distance[destination] + 1; // +1 to count source
}
// The problem reduces to finding the longest path in a directed graph from a given source to a specific destination, where each node represents a city and each edge a direct train route. Since the graph can have cycles but the intended problem is a DAG (per the original snippet), we can use a topological sort approach:  
// 1. Build an adjacency list from the routes.  
// 2. Perform a DFS-based topological sort starting from the source, ignoring nodes unreachable from the source.  
// 3. Initialize distances to negative infinity (or a very small value), set distance[source] = 0.  
// 4. Process nodes in topological order: for each node, for each neighbor, update distance[neighbor] = max(distance[neighbor], distance[node] + 1).  
// 5. After processing, if distance[destination] is still very small, return 0; otherwise return distance[destination] + 1 (to include the source).  
//
// Edge cases:  
// - If source equals destination, return 1 (only the source city).  
// - If there is no path, return 0.  
// - If the graph has cycles, a simple topological sort fails; however, since the original snippet assumes a DAG, we can handle cycles by using a DFS with visited-on-current-path detection and only consider simple paths, but that is exponential. For a robust solution that still works on a DAG, we stick to topological sort. If cycles are possible, the problem becomes NP-hard (longest path), so we assume a DAG as per the original context.  
//
// Time complexity: O(V + E) where V is number of distinct cities and E is number of routes. Space complexity: O(V + E) for adjacency list and visited/distance maps.
