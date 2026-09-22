/*
Write a C++ function `double maxProbability(int n, vector<vector<int>>& edges, vector<double>& succProb, int start, int end)` that, given an undirected weighted graph of `n` nodes (0-indexed) represented by an edge list `edges` where each edge connects nodes `a` and `b` with a success probability `succProb[i]` (a double between 0 and 1 inclusive), returns the maximum probability of successfully traversing from `start` to `end`. The probability of a path is the product of the success probabilities along its edges. If no path exists, return `0.0`. The answer must be accurate to within `1e-5`. The graph has at most one edge between any two nodes, `n` ranges from 2 to 10^4, and the number of edges is up to 2*10^4. The graph is undirected, so each edge can be traversed in both directions.
*/
#include <vector>
#include <queue>
#include <utility>

// Returns the maximum probability of reaching 'end' from 'start' in an undirected graph.
// Each edge has a success probability given in succProb. Returns 0.0 if unreachable.
double maxProbability(int n, std::vector<std::vector<int>>& edges, std::vector<double>& succProb, int start, int end) {
    // Build adjacency list: for each node, store pairs (neighbor, probability)
    std::vector<std::vector<std::pair<int, double>>> graph(n);
    for (size_t i = 0; i < edges.size(); ++i) {
        int a = edges[i][0];
        int b = edges[i][1];
        double p = succProb[i];
        graph[a].push_back({b, p});
        graph[b].push_back({a, p});
    }

    // prob[v] = best probability found so far to reach node v
    std::vector<double> prob(n, 0.0);
    prob[start] = 1.0;

    // Max-heap: pairs of (probability, node), ordered by highest probability
    std::priority_queue<std::pair<double, int>> pq;
    pq.push({1.0, start});

    while (!pq.empty()) {
        auto [currProb, u] = pq.top();
        pq.pop();

        // If we reached 'end', this is the best possible since we process max probability first
        if (u == end) {
            return currProb;
        }

        // Skip if we have a better probability already stored (stale entry)
        if (currProb < prob[u]) {
            continue;
        }

        // Relax edges
        for (const auto& [v, edgeProb] : graph[u]) {
            double newProb = currProb * edgeProb;
            if (newProb > prob[v]) {
                prob[v] = newProb;
                pq.push({newProb, v});
            }
        }
    }

    return 0.0;
}
#include <cassert>
#include <vector>

// Declaration of the function under test (assumed from the solution)
double maxProbability(int n, std::vector<std::vector<int>>& edges, std::vector<double>& succProb, int start, int end);

int main() {
    // Example 1 from problem: two paths, product vs single edge
    {
        int n = 3;
        std::vector<std::vector<int>> edges = {{0,1},{1,2},{0,2}};
        std::vector<double> succProb = {0.5, 0.5, 0.2};
        double result = maxProbability(n, edges, succProb, 0, 2);
        assert(std::abs(result - 0.25) < 1e-5);
    }

    // Example 2: direct edge has higher probability than multiplied path
    {
        int n = 3;
        std::vector<std::vector<int>> edges = {{0,1},{1,2},{0,2}};
        std::vector<double> succProb = {0.5, 0.5, 0.3};
        double result = maxProbability(n, edges, succProb, 0, 2);
        assert(std::abs(result - 0.3) < 1e-5);
    }

    // Example 3: no path exists
    {
        int n = 3;
        std::vector<std::vector<int>> edges = {{0,1}};
        std::vector<double> succProb = {0.5};
        double result = maxProbability(n, edges, succProb, 0, 2);
        assert(std::abs(result - 0.0) < 1e-5);
    }

    // Single edge with probability 1.0
    {
        int n = 2;
        std::vector<std::vector<int>> edges = {{0,1}};
        std::vector<double> succProb = {1.0};
        double result = maxProbability(n, edges, succProb, 0, 1);
        assert(std::abs(result - 1.0) < 1e-5);
    }

    // Longer path: product of probabilities
    {
        int n = 4;
        std::vector<std::vector<int>> edges = {{0,1},{1,2},{2,3},{0,3}};
        std::vector<double> succProb = {0.9, 0.9, 0.9, 0.5};
        // Path 0-1-2-3: 0.9*0.9*0.9 = 0.729; direct 0-3: 0.5; best = 0.729
        double result = maxProbability(n, edges, succProb, 0, 3);
        assert(std::abs(result - 0.729) < 1e-5);
    }

    // Disconnected graph, start and end in different components
    {
        int n = 4;
        std::vector<std::vector<int>> edges = {{0,1},{2,3}};
        std::vector<double> succProb = {0.8, 0.7};
        double result = maxProbability(n, edges, succProb, 0, 3);
        assert(std::abs(result - 0.0) < 1e-5);
    }

    // Multiple paths, best is not the direct edge
    {
        int n = 4;
        std::vector<std::vector<int>> edges = {{0,1},{1,2},{2,3},{0,3}};
        std::vector<double> succProb = {0.8, 0.8, 0.8, 0.9};
        // Direct 0-3: 0.9; path through 1,2: 0.8*0.8*0.8 = 0.512; best = 0.9
        double result = maxProbability(n, edges, succProb, 0, 3);
        assert(std::abs(result - 0.9) < 1e-5);
    }

    // Node with zero-probability edges: must ignore them
    {
        int n = 3;
        std::vector<std::vector<int>> edges = {{0,1},{1,2}};
        std::vector<double> succProb = {0.0, 0.5};
        double result = maxProbability(n, edges, succProb, 0, 2);
        assert(std::abs(result - 0.0) < 1e-5);
    }

    // Large n but simple path, ensure it works
    {
        int n = 5;
        std::vector<std::vector<int>> edges = {{0,1},{1,2},{2,3},{3,4}};
        std::vector<double> succProb = {0.9, 0.8, 0.7, 0.6};
        double expected = 0.9 * 0.8 * 0.7 * 0.6; // 0.3024
        double result = maxProbability(n, edges, succProb, 0, 4);
        assert(std::abs(result - expected) < 1e-5);
    }

    // Start equals end? Not per constraints but safe check: should return 1.0 if we had that case.
    // Not needed for testing as constraints say start != end.

    return 0;
}
// The problem is a variation of the shortest path problem, but instead of minimizing distance, we maximize the product of probabilities. Since probabilities are in `[0,1]`, taking the logarithm converts the product into a sum of negative values, but a more direct approach uses a max-heap (priority queue) variant of Dijkstra's algorithm. We initialize a distance/probability array `prob` of size `n` with zeros, and set `prob[start] = 1.0` (100% chance of being at the start). We push `{1.0, start}` onto a max-heap (priority queue ordered by highest probability first). While the heap is not empty, pop the node with the highest probability. If this node is `end`, we can immediately return its probability (because any later path will have a lower product). For each adjacent node, if the product of the current probability and the edge probability exceeds the stored probability for that neighbor, we update it and push the new pair onto the heap. This ensures we always explore the most promising path first. Edge cases: if there is no path, the heap will eventually empty and we return `0.0`. If `start` equals `end`? The constraints say `start != end`, but if it were, the answer would be `1.0`. The algorithm handles it naturally because we set `prob[start]=1.0` and would return immediately. Time complexity is `O((V+E) log V)` where `V=n` and `E` is the number of edges, because each edge creates at most one heap push (but could be multiple relaxations), and each node is popped at most once. Space complexity is `O(V+E)` for the adjacency list and `O(V)` for the probability array and heap.
