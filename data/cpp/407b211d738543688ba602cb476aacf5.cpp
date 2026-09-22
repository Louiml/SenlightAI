You are given a graph with `N` vertices (numbered 1 to N) and `M` undirected weighted edges. Each vertex `i` initially contains a cow whose destination is vertex `cows[i]` (1-indexed). Cows can move through the graph, but only along paths where every edge has weight **at least** `K`. Two cows can swap places if they are in the same connected component of the subgraph containing only edges with weight ≥ `K`. Your goal is to find the **maximum possible integer `K`** such that every cow `i` can reach its destination vertex `cows[i]` (i.e., for every `i`, vertices `i` and `cows[i]` are in the same connected component of the subgraph with edges of weight ≥ `K`). If no positive `K` works (i.e., the cows are already in their correct positions with no movement needed, or even `K=1` fails), output `-1`. Write a C++ function `int maxWormholeWidth(const std::vector<std::vector<std::pair<int,int>>>& graph, const std::vector<int>& cows)` that returns this maximum `K` (or `-1` if impossible). The graph is provided as an adjacency list where each pair is `{neighbor, weight}`.

#include <cassert>
#include <vector>
#include <utility>

// Include the solution function here (e.g., copy the code above)

int main() {
    // Test 1: Already sorted -> -1
    {
        std::vector<std::vector<std::pair<int,int>>> graph(3);
        graph[0].push_back({1, 5});
        graph[1].push_back({0, 5});
        graph[1].push_back({2, 3});
        graph[2].push_back({1, 3});
        std::vector<int> cows = {1, 2, 3};
        assert(maxWormholeWidth(graph, cows) == -1);
    }

    // Test 2: Simple swap requires width 5
    {
        std::vector<std::vector<std::pair<int,int>>> graph(2);
        graph[0].push_back({1, 5});
        graph[1].push_back({0, 5});
        std::vector<int> cows = {2, 1};
        assert(maxWormholeWidth(graph, cows) == 5);
    }

    // Test 3: Need width 2 in a chain
    {
        std::vector<std::vector<std::pair<int,int>>> graph(3);
        graph[0].push_back({1, 2});
        graph[1].push_back({0, 2});
        graph[1].push_back({2, 1});
        graph[2].push_back({1, 1});
        std::vector<int> cows = {3, 2, 1};
        // To swap 1 and 3, need path 0-1 (width 2) and 1-2 (width 1) -> max feasible is 1? 
        // Actually to reach from 0 to 2, both edges must be >=K => K<=1. So answer=1.
        assert(maxWormholeWidth(graph, cows) == 1);
    }

    // Test 4: Disconnected graph with no feasible positive K -> -1 (even if not sorted)
    {
        std::vector<std::vector<std::pair<int,int>>> graph(2); // no edges
        std::vector<int> cows = {2, 1};
        assert(maxWormholeWidth(graph, cows) == -1);
    }

    // Test 5: Multiple edges, max feasible is the bottleneck
    {
        std::vector<std::vector<std::pair<int,int>>> graph(4);
        graph[0].push_back({1, 10});
        graph[1].push_back({0, 10});
        graph[1].push_back({2, 7});
        graph[2].push_back({1, 7});
        graph[2].push_back({3, 4});
        graph[3].push_back({2, 4});
        std::vector<int> cows = {4, 2, 1, 3}; // need 0->3, 1->1, 2->0, 3->2
        // Path 0-1-2-3 requires min weight 4, so K=4 works. K=5 fails on edge 2-3.
        assert(maxWormholeWidth(graph, cows) == 4);
    }

    // Test 6: All edges have same weight
    {
        std::vector<std::vector<std::pair<int,int>>> graph(3);
        graph[0].push_back({1, 8});
        graph[1].push_back({0, 8});
        graph[1].push_back({2, 8});
        graph[2].push_back({1, 8});
        std::vector<int> cows = {2, 3, 1}; // cycle 0-1-2-0
        assert(maxWormholeWidth(graph, cows) == 8);
    }

    return 0;
}

#include <vector>
#include <algorithm>

// Depth-first search to label connected components using only edges with weight >= k
void dfsComponent(const std::vector<std::vector<std::pair<int,int>>>& graph,
                  std::vector<int>& comp, int node, int k, int label) {
    comp[node] = label;
    for (const auto& edge : graph[node]) {
        int neighbor = edge.first;
        int weight = edge.second;
        if (comp[neighbor] != -1) continue;
        if (weight < k) continue;
        dfsComponent(graph, comp, neighbor, k, label);
    }
}

// Check if with minimum width k, each cow can reach its destination
bool isFeasible(const std::vector<std::vector<std::pair<int,int>>>& graph,
                const std::vector<int>& cows, int k) {
    int n = (int)graph.size();
    std::vector<int> comp(n, -1);
    int label = 0;
    for (int i = 0; i < n; i++) {
        if (comp[i] == -1) {
            dfsComponent(graph, comp, i, k, label);
            label++;
        }
    }
    for (int i = 0; i < n; i++) {
        if (comp[i] != comp[cows[i] - 1]) return false;
    }
    return true;
}

// Returns the maximum wormhole width K, or -1 if the cows are already sorted
int maxWormholeWidth(const std::vector<std::vector<std::pair<int,int>>>& graph,
                     const std::vector<int>& cows) {
    int n = (int)graph.size();
    // Check if already sorted
    bool alreadySorted = true;
    for (int i = 0; i < n; i++) {
        if (cows[i] - 1 != i) {
            alreadySorted = false;
            break;
        }
    }
    if (alreadySorted) return -1;

    // Find maximum edge weight
    int maxWeight = 0;
    for (const auto& adj : graph) {
        for (const auto& edge : adj) {
            maxWeight = std::max(maxWeight, edge.second);
        }
    }

    // Binary search on answer in [1, maxWeight]
    int low = 1, high = maxWeight;
    int answer = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (isFeasible(graph, cows, mid)) {
            answer = mid;
            low = mid + 1; // try larger width
        } else {
            high = mid - 1; // need smaller width
        }
    }
    return answer;
}

// The solution uses binary search on the answer `K` over the range `[1, maxEdgeWeight]`. For a candidate `K`, we build a subgraph containing only edges with weight ≥ `K` and perform DFS (or union-find) to label connected components. Then we check for every vertex `i` whether `i` and `cows[i]-1` (0-indexed) are in the same component. If yes, `K` is feasible; otherwise not. Binary search finds the largest feasible `K`. A special case: if the cows are already perfectly placed (i.e., `cows[i]-1 == i` for all `i`), the answer is `-1` because no positive width is needed. Also, if even `K=1` is not feasible (which would only happen if the graph is disconnected in a way that prevents some cow from reaching its destination even with all edges available), then the binary search will eventually fail and we output `-1`. Time complexity is `O((N+M) * log(maxWeight))` per feasibility check, and space complexity is `O(N+M)` for the graph and visited array. Edge cases: the graph may be disconnected, weights may have duplicates, and `maxWeight` could be 0 (but since weights are positive via constraints, we handle the already-sorted case before binary search).
