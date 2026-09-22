/*
Write a C++ function `long long countColoringWays(int N, const std::vector<std::vector<int>>& adjacency)` that, given a number of nodes `N` and an undirected adjacency list (where `adjacency[i]` contains the list of nodes connected to node `i`), determines the number of valid ways to assign each node one of two colors (say, red or blue) such that no two adjacent nodes share the same color. If the graph is not bipartite (i.e., no such 2-coloring exists), return `0`. If it is bipartite, the graph may be disconnected; the answer is `2^C` modulo `1,000,000,007`, where `C` is the number of connected components in the graph. The nodes are numbered from `0` to `N-1`. Assume the adjacency list is undirected and may contain duplicate edges, but self-loops are not present. The function must handle up to `N = 20,000` nodes and a total number of edges up to `200,000` efficiently, using `const` references and no global variables.
*/
#include <vector>
#include <queue>

const long long MOD = 1000000007LL;

// Compute 2^n modulo MOD using fast exponentiation.
long long powerMod(long long n) {
    long long result = 1;
    long long base = 2;
    while (n > 0) {
        if (n & 1) {
            result = (result * base) % MOD;
        }
        base = (base * base) % MOD;
        n >>= 1;
    }
    return result;
}

// Count the number of valid 2-colorings modulo MOD.
// Returns 0 if the graph is not bipartite.
long long countColoringWays(int N, const std::vector<std::vector<int>>& adjacency) {
    std::vector<int> color(N, -1);
    int components = 0;
    bool isBipartite = true;

    // BFS from each unvisited node to find components and check bipartiteness.
    for (int start = 0; start < N; ++start) {
        if (color[start] != -1) continue;
        ++components;

        std::queue<int> q;
        q.push(start);
        color[start] = 0; // Assign first color

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (int neighbor : adjacency[current]) {
                if (color[neighbor] == -1) {
                    color[neighbor] = 1 - color[current]; // Alternate color
                    q.push(neighbor);
                } else if (color[neighbor] == color[current]) {
                    // Same color as current means an odd cycle — not bipartite.
                    isBipartite = false;
                }
            }
        }
    }

    if (!isBipartite) {
        return 0;
    }
    return powerMod(components);
}
#include <cassert>
#include <vector>

// The function is defined above (omitted here for brevity, but included in compilation).

int main() {
    // Test 1: Single node, no edges -> 2 colorings
    {
        int N = 1;
        std::vector<std::vector<int>> adj(N);
        assert(countColoringWays(N, adj) == 2);
    }

    // Test 2: Two disconnected nodes -> 2 components -> 4 colorings
    {
        int N = 2;
        std::vector<std::vector<int>> adj(N);
        assert(countColoringWays(N, adj) == 4);
    }

    // Test 3: A single edge (bipartite) -> 1 component -> 2 colorings
    {
        int N = 2;
        std::vector<std::vector<int>> adj(N);
        adj[0].push_back(1);
        adj[1].push_back(0);
        assert(countColoringWays(N, adj) == 2);
    }

    // Test 4: A triangle (odd cycle) -> not bipartite -> 0
    {
        int N = 3;
        std::vector<std::vector<int>> adj(N);
        adj[0].push_back(1);
        adj[1].push_back(0);
        adj[1].push_back(2);
        adj[2].push_back(1);
        adj[2].push_back(0);
        adj[0].push_back(2);
        assert(countColoringWays(N, adj) == 0);
    }

    // Test 5: An isolated node plus a connected bipartite pair (two components) -> 4
    {
        int N = 3;
        std::vector<std::vector<int>> adj(N);
        adj[1].push_back(2);
        adj[2].push_back(1);
        // Node 0 is isolated
        assert(countColoringWays(N, adj) == 4);
    }

    // Test 6: Duplicate edges on a bipartite graph -> still valid (2)
    {
        int N = 2;
        std::vector<std::vector<int>> adj(N);
        adj[0].push_back(1);
        adj[0].push_back(1); // duplicate
        adj[1].push_back(0);
        adj[1].push_back(0); // duplicate
        assert(countColoringWays(N, adj) == 2);
    }

    // Test 7: A path of 4 nodes (bipartite) -> 2
    {
        int N = 4;
        std::vector<std::vector<int>> adj(N);
        adj[0].push_back(1);
        adj[1].push_back(0);
        adj[1].push_back(2);
        adj[2].push_back(1);
        adj[2].push_back(3);
        adj[3].push_back(2);
        assert(countColoringWays(N, adj) == 2);
    }

    // Test 8: Large N (20000) with many isolated nodes -> 2^20000 mod MOD (known value)
    {
        int N = 20000;
        std::vector<std::vector<int>> adj(N);
        // Use a known result: 2^20000 mod 1e9+7 can be precomputed via fast exponentiation
        long long expected = 1;
        long long base = 2;
        long long exp = 20000;
        while (exp > 0) {
            if (exp & 1) expected = (expected * base) % MOD;
            base = (base * base) % MOD;
            exp >>= 1;
        }
        assert(countColoringWays(N, adj) == expected);
    }

    return 0;
}
// The problem reduces to checking whether the undirected graph is bipartite and counting its connected components. A graph is bipartite if and only if it contains no odd-length cycle; equivalently, we can 2-color it using a breadth-first search (BFS) from each unvisited node. During BFS, assign alternating colors (0 and 1) to nodes layer by layer. If we ever encounter an already visited neighbor with the same color as the current node (indicating an odd cycle), the graph is not bipartite, and the answer is `0`. Otherwise, after processing all components, each connected component can be colored independently in exactly 2 ways (choose which component gets color 0 on its first node, then all other colors are forced). Therefore, if there are `C` components, the total number of valid 2-colorings is `2^C` modulo `1,000,000,007`. Compute this via fast exponentiation. Edge cases include a graph with a single node (answer is 2), an empty graph with no edges but multiple isolated nodes (each isolated node is its own component, so answer is `2^N`), and duplicate edges (which are ignored safely because they do not affect the bipartiteness check). Time complexity is `O(N + E)` where `E` is the total number of edges (since we traverse each adjacency list once per BFS, and total degree sum is `2E`), and space complexity is `O(N)` for the visited/color arrays and queue.
