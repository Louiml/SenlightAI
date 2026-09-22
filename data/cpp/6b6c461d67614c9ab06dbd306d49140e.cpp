// Write a C++ function named `findCriticalComponents` that takes an undirected graph with `n` vertices (numbered `0` through `n-1`) represented as an adjacency list (`vector<vector<int>>`) and returns a `pair<vector<int>, set<pair<int,int>>>` where the first element is a vector of all articulation points (vertices whose removal increases the number of connected components) sorted in ascending order, and the second element is a set of unordered bridges (edges whose removal increases the number of connected components), each stored as a pair with the smaller vertex index first. The graph may be disconnected, may contain multiple edges, and may have self-loops; self-loops are not considered bridges. If a vertex is an articulation point, it should appear exactly once in the output vector. The function must correctly handle graphs with a single vertex, graphs with no edges, and graphs that are already trees or cycles.
#include <cassert>
#include <vector>
#include <set>
#include <utility>
using namespace std;

// The function declaration from solution (must be included or copy-pasted here)
// Assume findCriticalComponents is defined above

int main() {
    // Test 1: Simple triangle graph (no articulation, no bridge)
    vector<vector<int>> adj1 = {{1,2},{0,2},{0,1}};
    auto res1 = findCriticalComponents(adj1, 3);
    assert(res1.first.empty());
    assert(res1.second.empty());

    // Test 2: Line graph 0-1-2 (1 is articulation, bridge 0-1 and 1-2)
    vector<vector<int>> adj2 = {{1},{0,2},{1}};
    auto res2 = findCriticalComponents(adj2, 3);
    assert(res2.first == vector<int>({1}));
    assert(res2.second == set<pair<int,int>>({{0,1},{1,2}}));

    // Test 3: Disconnected graph with isolated vertex (no articulation, no bridge)
    vector<vector<int>> adj3 = {{1},{0},{}}; // vertex 2 isolated
    auto res3 = findCriticalComponents(adj3, 3);
    assert(res3.first.empty());
    assert(res3.second.empty());

    // Test 4: Single vertex with self-loop (no articulation, no bridge)
    vector<vector<int>> adj4 = {{0}};
    auto res4 = findCriticalComponents(adj4, 1);
    assert(res4.first.empty());
    assert(res4.second.empty());

    // Test 5: Cycle of 4 vertices (no articulation, no bridge)
    vector<vector<int>> adj5 = {{1,3},{0,2},{1,3},{0,2}};
    auto res5 = findCriticalComponents(adj5, 4);
    assert(res5.first.empty());
    assert(res5.second.empty());

    // Test 6: Two cycles sharing a vertex (articulation at 0, no bridges)
    // 0 connected to 1,2 and 3,4
    vector<vector<int>> adj6 = {{1,2,3,4},{0,2},{0,1},{0,4},{0,3}};
    auto res6 = findCriticalComponents(adj6, 5);
    assert(res6.first == vector<int>({0}));
    assert(res6.second.empty());

    // Test 7: Star graph (center 0 is articulation, all edges are bridges)
    vector<vector<int>> adj7 = {{1,2,3},{0},{0},{0}};
    auto res7 = findCriticalComponents(adj7, 4);
    assert(res7.first == vector<int>({0}));
    assert(res7.second == set<pair<int,int>>({{0,1},{0,2},{0,3}}));

    // Test 8: Multiple edges between same pair (parallel edges, not a bridge)
    vector<vector<int>> adj8 = {{1,1},{0,0}};
    auto res8 = findCriticalComponents(adj8, 2);
    assert(res8.first.empty());
    assert(res8.second.empty());

    // Test 9: Tree with 5 nodes: 0-1,1-2,2-3,3-4
    // Articulations: 1,2,3 (all internal nodes)
    // Bridges: all edges
    vector<vector<int>> adj9 = {{1},{0,2},{1,3},{2,4},{3}};
    auto res9 = findCriticalComponents(adj9, 5);
    assert(res9.first == vector<int>({1,2,3}));
    assert(res9.second == set<pair<int,int>>({{0,1},{1,2},{2,3},{3,4}}));

    // Test 10: Graph with one articulation and one bridge in different components
    // Component1: 0-1-2 (1 is articulation, bridges 0-1,1-2)
    // Component2: 3-4 (bridge 3-4, no articulation)
    vector<vector<int>> adj10 = {{1},{0,2},{1},{4},{3}};
    auto res10 = findCriticalComponents(adj10, 5);
    assert(res10.first == vector<int>({1}));
    assert(res10.second == set<pair<int,int>>({{0,1},{1,2},{3,4}}));

    return 0;
}
#include <vector>
#include <set>
#include <algorithm>
#include <utility>

using namespace std;

// Returns {articulation points sorted ascending, bridges as sorted pairs}
pair<vector<int>, set<pair<int,int>>> findCriticalComponents(const vector<vector<int>>& adj, int n) {
    vector<int> num(n, -1), low(n), artic(n, 0);
    set<pair<int,int>> bridges;
    int timer = 0;

    function<void(int, int)> dfs = [&](int parent, int u) {
        num[u] = low[u] = timer++;
        int children = 0;
        bool isArtic = false;

        for (int v : adj[u]) {
            if (v == parent) continue; // Skip the direct parent edge
            if (num[v] == -1) {
                children++;
                dfs(u, v);
                low[u] = min(low[u], low[v]);
                if (low[v] >= num[u]) isArtic = true;
                if (low[v] > num[u]) {
                    auto e = (u < v) ? make_pair(u, v) : make_pair(v, u);
                    bridges.insert(e);
                }
            } else {
                // Back edge to an already visited vertex (not parent)
                low[u] = min(low[u], num[v]);
            }
        }
        // Root articulation condition is handled outside for each component
        if (parent == -1) {
            artic[u] = (children > 1) ? 1 : 0;
        } else {
            artic[u] = isArtic ? 1 : 0;
        }
    };

    for (int i = 0; i < n; ++i) {
        if (num[i] == -1) {
            dfs(-1, i);
        }
    }

    vector<int> result_artic;
    for (int i = 0; i < n; ++i) {
        if (artic[i] == 1) result_artic.push_back(i);
    }
    // Already in ascending order because we iterate i from 0 to n-1
    return {result_artic, bridges};
}
// The solution uses Tarjan's algorithm for finding articulation points and bridges in an undirected graph via depth-first search (DFS). We maintain two arrays: `num` (discovery time) and `low` (the smallest discovery time reachable from the subtree rooted at that vertex, including back edges). During DFS, for each tree edge from parent `u` to child `v`, we update `low[u] = min(low[u], low[v])`. If `low[v] >= num[u]`, then removing `u` disconnects the subtree of `v` from the rest (or `u` is the root with more than one child), so `u` is an articulation point. If `low[v] > num[u]`, then the edge `(u,v)` is a bridge. For the root, it is an articulation point only if it has more than one child in the DFS tree. We must also handle back edges by updating `low[u] = min(low[u], num[v])` when `v` is not the parent. Since the graph may be disconnected, we run DFS from each unvisited vertex, resetting the root and counting root children. Self-loops are ignored when considering back edges (since `a != parent` excludes the parent, and a self-loop would be `a == nnode` which is not the parent, but we should skip it to avoid treating it as an edge that affects low; actually a self-loop makes `num[a]` equal to `num[nnode]` and would update low to the same value, which is harmless but we can skip it). Multiple edges between the same pair are handled correctly because a back edge to the parent via a parallel edge is not the immediate parent edge and thus correctly updates `low`. Time complexity is O(V + E) for DFS. Space complexity is O(V + E) for the adjacency list, plus O(V) for auxiliary arrays.
