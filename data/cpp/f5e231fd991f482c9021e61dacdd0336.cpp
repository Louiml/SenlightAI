Write a C++ function `int chinesePostman(int n, const std::vector<std::pair<int,int>> adj[])` that takes the number of vertices `n` and an adjacency list of an undirected weighted graph, where each edge appears twice (once for each endpoint). The graph is guaranteed to be connected. The function must return the minimum total weight of a closed walk (starting and ending at the same vertex) that traverses every edge at least once. This is the Chinese Postman Problem. The edge weights are positive integers. If the graph already has an Eulerian circuit (all vertices have even degree), the answer is simply the sum of all edge weights (counting each edge once). Otherwise, the algorithm must duplicate the minimum total weight of edges so that all vertices become even, then add that duplication cost to the original total edge weight. The solution must handle up to `n = 20` vertices, and the number of odd-degree vertices may be up to `n`. The adjacency list may contain multiple edges between the same pair of vertices, but the graph is simple in the sense that no self-loops are present. The function must not modify the input graph.

The Chinese Postman Problem in an undirected connected graph asks for the minimum length closed walk that covers every edge at least once. If all vertices have even degree, the graph has an Eulerian circuit, and the optimal walk is the Eulerian circuit, whose weight equals the sum of all edge weights. If some vertices have odd degree, the graph is not Eulerian, so we must duplicate edges to make all degrees even; then an Eulerian circuit in the modified graph corresponds to a valid closed walk. The minimum extra weight equals the minimum-cost perfect matching on the set of odd-degree vertices, where the cost of matching two odd vertices is the shortest path distance between them in the original graph. We enumerate all perfect pairings of the odd vertices (there are `(k-1)!!` pairings for `k` odd vertices), compute the sum of shortest path distances for each pairing, and choose the minimum. We then return the original total edge weight plus that minimum matching cost. Since the graph can be weighted with nonnegative weights, we use Dijkstra's algorithm (or a simple Bellman-Ford-like relaxation with a queue, which is given in the snippet) for each pair of odd vertices. Note that the number of odd vertices is always even. The key edge cases: (1) no odd vertices (Eulerian graph) → return sum of edge weights; (2) exactly two odd vertices → the minimum matching is just the shortest path between them; (3) many odd vertices → we try all pairings. Time complexity: let `k` be the number of odd vertices. Generating all pairings takes `O((k-1)!!)` time. For each pairing, we run Dijkstra for each of `k/2` pairs, each Dijkstra takes `O((n + E) log n)` with a priority queue, or `O(n^2)` with a simple implementation. Since `k` can be up to `n` (max 20), the number of pairings is at most `19!! = 654729075`, which is huge. However, for the given constraints (n ≤ 20), the worst case is still potentially large, but we note that the typical test cases would be smaller. For the reference solution, we'll use a recursive generation with pruning, and we may compute all-pairs shortest paths once using Floyd-Warshall (`O(n^3)`) to answer pair distances in `O(1)`, reducing the per-pairing cost. The overall complexity then becomes `O(n^3 + (k-1)!!)`. Space complexity: `O(n^2)` for the distance matrix.

#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

// Compute the minimum weight Chinese Postman tour for an undirected connected weighted graph.
// Graph is given as adjacency list: adj[i] contains pairs (neighbor, weight). Each edge appears twice.
// n is the number of vertices (0-indexed).
int chinesePostman(int n, const std::vector<std::pair<int,int>> adj[]) {
    // Step 1: compute total edge weight (sum once per edge)
    int totalWeight = 0;
    for (int u = 0; u < n; ++u) {
        for (const auto& p : adj[u]) {
            if (p.first > u) totalWeight += p.second;
        }
    }

    // Step 2: find odd-degree vertices
    std::vector<int> odd;
    for (int u = 0; u < n; ++u) {
        if (adj[u].size() % 2 == 1) odd.push_back(u);
    }

    // If no odd vertices, Eulerian circuit exists
    if (odd.empty()) return totalWeight;

    // Step 3: compute all-pairs shortest paths using Floyd-Warshall
    const int INF = INT_MAX / 2;
    std::vector<std::vector<int>> dist(n, std::vector<int>(n, INF));
    for (int u = 0; u < n; ++u) dist[u][u] = 0;
    for (int u = 0; u < n; ++u) {
        for (const auto& p : adj[u]) {
            int v = p.first, w = p.second;
            if (w < dist[u][v]) dist[u][v] = w;
        }
    }
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    // Step 4: enumerate all perfect pairings of odd vertices
    int k = odd.size();
    int minExtra = INF;
    
    // Recursive function to generate pairings
    std::vector<bool> used(k, false);
    std::vector<int> pairCost;
    
    // Helper lambda for recursion
    std::function<void(int, int)> dfs = [&](int count, int currentCost) {
        if (count == k) {
            minExtra = std::min(minExtra, currentCost);
            return;
        }
        // find first unused odd vertex
        int first = -1;
        for (int i = 0; i < k; ++i) {
            if (!used[i]) { first = i; break; }
        }
        used[first] = true;
        for (int j = first + 1; j < k; ++j) {
            if (!used[j]) {
                used[j] = true;
                int d = dist[odd[first]][odd[j]];
                if (currentCost + d < minExtra) {
                    dfs(count + 2, currentCost + d);
                }
                used[j] = false;
            }
        }
        used[first] = false;
    };
    
    dfs(0, 0);

    // Step 5: answer is total edge weight plus minimum extra
    return totalWeight + minExtra;
}

#include <cassert>
#include <vector>
#include <utility>

// The function is declared above (linked in compilation)

int main() {
    // Test 1: Eulerian graph (all even degrees) - triangle with equal weights
    // 0-1 (1), 1-2 (1), 2-0 (1)
    {
        std::vector<std::pair<int,int>> adj[3];
        adj[0].push_back({1,1}); adj[0].push_back({2,1});
        adj[1].push_back({0,1}); adj[1].push_back({2,1});
        adj[2].push_back({0,1}); adj[2].push_back({1,1});
        assert(chinesePostman(3, adj) == 3);
    }

    // Test 2: Two odd vertices - line of 3 vertices with weights 1,2
    // 0-1 (1), 1-2 (2) => odd vertices: 0 and 2; total weight=3, extra=shortest path=3, answer=6
    {
        std::vector<std::pair<int,int>> adj[3];
        adj[0].push_back({1,1});
        adj[1].push_back({0,1}); adj[1].push_back({2,2});
        adj[2].push_back({1,2});
        assert(chinesePostman(3, adj) == 6);
    }

    // Test 3: Four odd vertices - square with diagonals? Use simple square plus one diagonal.
    // Vertices 0-1 (1), 1-2 (1), 2-3 (1), 3-0 (1), plus 0-2 (10)
    // Odd vertices: 1 and 3 (degree 2) - wait all even? Compute degrees:
    // 0: edges to 1,3,2 -> degree 3 (odd)
    // 1: edges to 0,2 -> degree 2 (even)
    // 2: edges to 1,3,0 -> degree 3 (odd)
    // 3: edges to 2,0 -> degree 2 (even)
    // Actually odd: 0 and 2? Let's recount: 
    // 0: to 1,3,2 => degree 3 (odd)
    // 1: to 0,2 => degree 2 (even)
    // 2: to 1,3,0 => degree 3 (odd)
    // 3: to 2,0 => degree 2 (even)
    // So odd: {0,2} -> shortest path between them is min(1+1=2 via 1, or 1+1=2 via 3, direct 10) => 2
    // total weight = 1+1+1+1+10=14, extra=2, answer=16
    {
        std::vector<std::pair<int,int>> adj[4];
        adj[0].push_back({1,1}); adj[0].push_back({3,1}); adj[0].push_back({2,10});
        adj[1].push_back({0,1}); adj[1].push_back({2,1});
        adj[2].push_back({1,1}); adj[2].push_back({3,1}); adj[2].push_back({0,10});
        adj[3].push_back({2,1}); adj[3].push_back({0,1});
        assert(chinesePostman(4, adj) == 16);
    }

    // Test 4: Larger graph with 4 odd vertices, from the snippet's example (6 vertices)
    // The example in the snippet has odd vertices? Let's compute degrees:
    // adj[0]: to 1,2,3 -> degree 3 (odd)
    // adj[1]: to 0,5,4 -> degree 3 (odd)
    // adj[2]: to 0,3 -> degree 2 (even)
    // adj[3]: to 2,4,0 -> degree 3 (odd)
    // adj[4]: to 3,1,5 -> degree 3 (odd)
    // adj[5]: to 1,4 -> degree 2 (even)
    // Odd: {0,1,3,4} (4 odd)
    // total weight: edges: (0,1)=3, (0,2)=1, (0,3)=5, (1,5)=1, (1,4)=6, (2,3)=2, (3,4)=4, (4,5)=1
    // Sum = 3+1+5+1+6+2+4+1 = 23
    // Need min perfect matching on odd vertices {0,1,3,4} using shortest paths.
    // Compute shortest paths:
    // dist(0,1)=3 (direct)
    // dist(0,3)=5 (direct) but via 2 is 1+2=3? Actually 0-2 (1) +2-3 (2)=3, so dist=3
    // dist(0,4)= via 1? 0-1 (3)+1-4 (6)=9, via 3? 0-3 (3)+3-4 (4)=7, via 0-2-3-4? 1+2+4=7, so min=7
    // dist(1,3)= via 0? 3+5=8, via 1-5-4-3? 1+1+4=6, via 1-4-3? 6+4=10, so min=6
    // dist(1,4)=6 (direct)
    // dist(3,4)=4 (direct)
    // Possible pairings:
    // (0,1)+(3,4): 3+4=7
    // (0,3)+(1,4): 3+6=9
    // (0,4)+(1,3): 7+6=13
    // Minimum extra = 7, answer = 23+7 = 30
    {
        std::vector<std::pair<int,int>> adj[6];
        adj[0].push_back({1,3}); adj[0].push_back({2,1}); adj[0].push_back({3,5});
        adj[1].push_back({0,3}); adj[1].push_back({5,1}); adj[1].push_back({4,6});
        adj[2].push_back({0,1}); adj[2].push_back({3,2});
        adj[3].push_back({2,2}); adj[3].push_back({4,4}); adj[3].push_back({0,5});
        adj[4].push_back({3,4}); adj[4].push_back({1,6}); adj[4].push_back({5,1});
        adj[5].push_back({1,1}); adj[5].push_back({4,1});
        assert(chinesePostman(6, adj) == 30);
    }

    // Test 5: Two vertices with multiple edges between them? Not allowed (simple graph), but test single edge
    // 0-1 (5) -> both odd, total=5, extra=5, answer=10
    {
        std::vector<std::pair<int,int>> adj[2];
        adj[0].push_back({1,5});
        adj[1].push_back({0,5});
        assert(chinesePostman(2, adj) == 10);
    }

    // Test 6: Graph with no odd vertices but multiple edges? Not applicable. Use a square cycle
    // 0-1-2-3-0 with weights 1 each, all even => total=4
    {
        std::vector<std::pair<int,int>> adj[4];
        adj[0].push_back({1,1}); adj[0].push_back({3,1});
        adj[1].push_back({0,1}); adj[1].push_back({2,1});
        adj[2].push_back({1,1}); adj[2].push_back({3,1});
        adj[3].push_back({2,1}); adj[3].push_back({0,1});
        assert(chinesePostman(4, adj) == 4);
    }

    return 0;
}
