You are given a graph with `N` nodes (numbered 1 through `N`), `M` undirected edges, and `S` initially activated nodes. Each edge has a base cost `E`, and additionally there is a fixed "connection cost" `L` added to every edge traversal, so traversing an edge from `u` to `v` costs exactly `E + L`. Starting from the initially activated nodes, you may activate a new node by paying the cost of one edge from an already activated node to that new node. Your goal is to activate all `N` nodes with the minimum total cost. Write a C++ function `long long minimumActivationCost(int N, int M, int L, int S, const vector<int>& initialStations, const vector<tuple<int,int,int>>& edges)` that returns this minimum total cost. If the graph is disconnected and some nodes cannot be reached, the function should still return the sum of costs for all reachable activated nodes (i.e., you only need to activate nodes in the connected components that contain at least one initial station). The problem guarantees that there is at least one initial station (`S ≥ 1`) and that the graph may have multiple edges between the same pair of nodes. The total number of nodes `N` can be up to 100,000 and edges `M` up to 200,000; `L` and each `E` are non-negative integers up to 10^9. The function should handle large inputs efficiently.

This problem is a classic minimum spanning tree (MST) variant where we already have a set of "super-source" nodes that are initially connected. The key insight is to treat all initial stations as a single virtual root with zero-cost edges to them, then compute the MST of the resulting graph (including the virtual root). However, since edges from the virtual root cost 0, they will always be included, effectively merging the initial stations into one component from the start. Equivalently, we can run Prim's algorithm starting from all initial stations simultaneously: mark all initial stations as visited, push all their incident edges into a priority queue, and repeatedly pop the smallest edge connecting a visited node to an unvisited node, adding its cost and expanding. This is exactly what the provided code does. Important edge cases: multiple edges between the same nodes—the priority queue handles them since only the cheapest will be chosen first per node; self-loops are irrelevant; disconnected components without any initial station are never visited, so they contribute nothing; and because all edge weights are non-negative (E + L ≥ 0), the greedy Prim approach is optimal. Time complexity is O((N + M) log M) due to the priority queue operations; space complexity is O(N + M) for adjacency lists and visited array. The function must be careful with 64-bit integers since total cost can exceed 32-bit range (N up to 1e5, edge cost up to 2e9, so total up to ~2e14).

#include <vector>
#include <queue>
#include <tuple>
using namespace std;

long long minimumActivationCost(
    int N, int M, int L, int S,
    const vector<int>& initialStations,
    const vector<tuple<int,int,int>>& edges
) {
    // Build adjacency list: node -> (edge_cost = E+L, neighbor)
    vector<vector<pair<int,int>>> adj(N);
    for (const auto& [u, v, E] : edges) {
        int cost = E + L;
        adj[u].push_back({cost, v});
        adj[v].push_back({cost, u});
    }

    // Visited flags for nodes
    vector<bool> visited(N, false);

    // Min-heap storing (cost, node)
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> heap;

    // Initialize with all initial stations (convert to 0-based)
    for (int s : initialStations) {
        int idx = s - 1;
        if (!visited[idx]) {
            visited[idx] = true;
            for (const auto& [cost, nb] : adj[idx]) {
                heap.push({cost, nb});
            }
        }
    }

    long long totalEnergy = 0;

    // Prim's algorithm from multi-source
    while (!heap.empty()) {
        auto [cost, node] = heap.top();
        heap.pop();
        if (!visited[node]) {
            visited[node] = true;
            totalEnergy += cost;
            for (const auto& [cost2, nb] : adj[node]) {
                heap.push({cost2, nb});
            }
        }
    }

    return totalEnergy;
}

#include <cassert>
#include <vector>
#include <tuple>
using namespace std;

// Assume the solution function is declared above

int main() {
    // Test 1: Simple line graph, two initial stations at ends
    {
        int N = 3, M = 2, L = 1, S = 2;
        vector<int> initial = {1, 3};
        vector<tuple<int,int,int>> edges = {
            {1, 2, 5}, {2, 3, 7}
        };
        // Costs: 1-2: 6, 2-3: 8. Since both ends initially, only need to activate middle via min(6,8)=6
        assert(minimumActivationCost(N, M, L, S, initial, edges) == 6);
    }

    // Test 2: Triangle graph, one initial station
    {
        int N = 3, M = 3, L = 0, S = 1;
        vector<int> initial = {1};
        vector<tuple<int,int,int>> edges = {
            {1, 2, 10}, {1, 3, 20}, {2, 3, 5}
        };
        // MST cost: 1-2=10, 2-3=5, total=15
        assert(minimumActivationCost(N, M, L, S, initial, edges) == 15);
    }

    // Test 3: Disconnected graph, only one component reachable
    {
        int N = 4, M = 1, L = 2, S = 1;
        vector<int> initial = {1};
        vector<tuple<int,int,int>> edges = {
            {1, 2, 3}
        };
        // Only nodes 1 and 2 are reachable; edge cost = 3+2=5; nodes 3,4 unreachable
        assert(minimumActivationCost(N, M, L, S, initial, edges) == 5);
    }

    // Test 4: All initially activated
    {
        int N = 3, M = 2, L = 100, S = 3;
        vector<int> initial = {1, 2, 3};
        vector<tuple<int,int,int>> edges = {
            {1, 2, 1}, {2, 3, 1}
        };
        // No additional cost needed
        assert(minimumActivationCost(N, M, L, S, initial, edges) == 0);
    }

    // Test 5: Multiple edges between same nodes
    {
        int N = 2, M = 2, L = 1, S = 1;
        vector<int> initial = {1};
        vector<tuple<int,int,int>> edges = {
            {1, 2, 10}, {1, 2, 1}
        };
        // Best edge cost = 1+1=2
        assert(minimumActivationCost(N, M, L, S, initial, edges) == 2);
    }

    // Test 6: Large value check (ensuring long long)
    {
        int N = 2, M = 1, L = 1000000000, S = 1;
        vector<int> initial = {1};
        vector<tuple<int,int,int>> edges = {
            {1, 2, 1000000000}
        };
        // Cost = 2,000,000,000
        assert(minimumActivationCost(N, M, L, S, initial, edges) == 2000000000LL);
    }

    // Test 7: Empty edges but multiple initial stations
    {
        int N = 3, M = 0, L = 5, S = 2;
        vector<int> initial = {1, 3};
        vector<tuple<int,int,int>> edges = {};
        // Only nodes 1 and 3 are activated, node 2 unreachable; cost = 0
        assert(minimumActivationCost(N, M, L, S, initial, edges) == 0);
    }

    return 0;
}
