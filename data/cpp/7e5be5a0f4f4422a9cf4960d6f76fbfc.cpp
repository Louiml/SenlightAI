// Write a C++ function `countMaximalMatchingSize` that takes an undirected unweighted graph represented as an adjacency list (`std::vector<std::vector<long>>`), where vertex indices are 0-based and edges are undirected (each edge appears in both adjacency lists). The function must return the size (number of edges) of a *maximal* matching, not necessarily maximum. A maximal matching is a set of edges such that no two edges share a vertex, and adding any additional edge (from the original graph) to the set would violate the matching property. The function must compute this using the following approach: first, partition the vertices into two color classes using BFS (graph is bipartite, but you may assume it is; if not, the algorithm may fail, so you only need to handle bipartite graphs). Then, build a directed graph from one partition to the other using the original undirected edges, and find a maximum matching on that bipartite graph using the Kuhn algorithm (augmenting paths via DFS). Return the size of that maximum matching, which equals the size of a maximal matching in the original graph because in a bipartite graph, a maximum matching is also maximal. The input graph may be disconnected, may contain isolated vertices (adjacency lists empty), and may have up to 10^5 vertices and 10^6 edges; all vertex degrees are non‑negative. Your function should be efficient and use appropriate data structures.
The problem reduces to finding a maximum matching in a bipartite graph, which is also maximal. Steps: 
1. **Bipartite coloring**: Use BFS from every unvisited vertex to assign color 0 or 1. Because the graph is bipartite, BFS will succeed. We also need to sort each adjacency list in increasing order (though not strictly required for correctness, it helps with determinism) – but for performance, we can skip sorting, as Kuhn's algorithm works regardless of order. However, the original snippet sorts to ensure deterministic output; we don't need that here. 
2. **Directed adjacency**: Create a new adjacency list `g1` for vertices of color 0 only, where each such vertex has edges to its neighbors that are of color 1 (since original edges are undirected, we only need one direction). 
3. **Kuhn's algorithm**: Run DFS from every vertex in the left partition (color 0) to find augmenting paths. Maintain a `matching` array of size `n` where `matching[v]` is the left vertex matched to right vertex `v`, or -1. For each left vertex `v`, run a DFS with a `visited` array reset each time. The DFS tries to match `v` to a neighbor `to`; if `to` is unmatched, match; else recursively try to rematch the vertex currently matched to `to`. If a path is found, increment matching count. 
4. **Edge cases**: 
   - Empty graph or no edges → matching size 0. 
   - Disconnected components: BFS handles each component independently; Kuhn's algorithm naturally processes all left vertices. 
   - Isolated vertices: no edges, ignored. 
   - Duplicate edges? The input is a simple graph, but if duplicates exist, Kuhn's still works as search visits each neighbor once. 
   - Self-loops? Not allowed in a simple graph, but if present, bipartite coloring would fail; ignore.
5. **Complexity**: BFS is O(n+m). Sorting adjacency lists would be O(Σ deg log deg) but we skip sorting. Kuhn's algorithm runs in O(n*m) in the worst case (since each DFS can traverse all edges). With n up to 10^5 and m up to 10^6, this is too slow in worst case. But typical constraints for Kuhn are lower; however, for a teaching task, we accept it. For a more robust solution, we could use Hopcroft-Karp, but the problem specifically asks for Kuhn. We'll note that in practice, if n and m are large, Kuhn might be too slow, but the task is educational. For the test, we use small graphs.
Finally, return the size of the matching, not the pairs themselves. The task says "countMaximalMatchingSize", so we return a `long` (matching size).
#include <vector>
#include <queue>

// Count the size of a maximal matching in an undirected bipartite graph.
// The graph is given as an adjacency list (0-based indices). The graph is assumed bipartite.
// The function returns the size of a maximum matching, which is also maximal.
long countMaximalMatchingSize(const std::vector<std::vector<long>>& g) {
    long n = static_cast<long>(g.size());
    if (n == 0) return 0;

    // Step 1: Bipartite coloring
    std::vector<char> part(n, -1);
    std::queue<long> q;
    for (long st = 0; st < n; ++st) {
        if (part[st] == -1) {
            part[st] = 0;
            q.push(st);
            while (!q.empty()) {
                long v = q.front(); q.pop();
                for (long to : g[v]) {
                    if (part[to] == -1) {
                        part[to] = !part[v];
                        q.push(to);
                    }
                }
            }
        }
    }

    // Step 2: Build directed adjacency from left (color 0) to right (color 1)
    std::vector<std::vector<long>> g1(n);
    for (long v = 0; v < n; ++v) {
        if (part[v] == 0) {
            for (long to : g[v]) {
                if (part[to] == 1) {
                    g1[v].push_back(to);
                }
            }
        }
    }

    // Step 3: Kuhn's algorithm for maximum bipartite matching
    std::vector<long> matching(n, -1);
    std::vector<bool> used;
    long matchingSize = 0;

    // Recursive helper for DFS augmenting path
    std::function<bool(long)> dfs = [&](long v) -> bool {
        if (used[v]) return false;
        used[v] = true;
        for (long to : g1[v]) {
            if (matching[to] == -1 || dfs(matching[to])) {
                matching[to] = v;
                return true;
            }
        }
        return false;
    };

    for (long v = 0; v < n; ++v) {
        if (part[v] == 0) {
            used.assign(n, false);
            if (dfs(v)) {
                matchingSize++;
            }
        }
    }

    return matchingSize;
}
#include <cassert>
#include <vector>

// The function to test (provided above)
long countMaximalMatchingSize(const std::vector<std::vector<long>>& g);

int main() {
    // Test 1: Simple path of 2 vertices (1 edge)
    std::vector<std::vector<long>> g1 = {{1}, {0}};
    assert(countMaximalMatchingSize(g1) == 1);

    // Test 2: Triangle (not bipartite, but our function assumes bipartite; we won't test that)
    // Instead, test a disjoint set of two edges: vertices 0-1 and 2-3
    std::vector<std::vector<long>> g2 = {{1}, {0}, {3}, {2}};
    assert(countMaximalMatchingSize(g2) == 2);

    // Test 3: Star with center 0 connected to leaves 1,2,3 – max matching is 1 (only one edge can be chosen)
    std::vector<std::vector<long>> g3 = {{1,2,3}, {0}, {0}, {0}};
    assert(countMaximalMatchingSize(g3) == 1);

    // Test 4: Path of 4 vertices (0-1-2-3) – maximum matching size 2
    std::vector<std::vector<long>> g4 = {{1}, {0,2}, {1,3}, {2}};
    assert(countMaximalMatchingSize(g4) == 2);

    // Test 5: Empty graph with 3 isolated vertices – matching size 0
    std::vector<std::vector<long>> g5 = {{}, {}, {}};
    assert(countMaximalMatchingSize(g5) == 0);

    // Test 6: Complete bipartite K2,2 – maximum matching size 2
    std::vector<std::vector<long>> g6 = {{2,3}, {2,3}, {0,1}, {0,1}};
    assert(countMaximalMatchingSize(g6) == 2);

    // Test 7: Single edge connecting 0-1, plus isolated vertex 2
    std::vector<std::vector<long>> g7 = {{1}, {0}, {}};
    assert(countMaximalMatchingSize(g7) == 1);

    // Test 8: Two parallel edges (not allowed in simple graph, but we ignore duplicate edges – still size 1)
    std::vector<std::vector<long>> g8 = {{1,1}, {0,0}};
    assert(countMaximalMatchingSize(g8) == 1);

    // Test 9: More complex: cycle of length 4 – max matching 2
    std::vector<std::vector<long>> g9 = {{1,3}, {0,2}, {1,3}, {0,2}};
    assert(countMaximalMatchingSize(g9) == 2);

    // Test 10: Disconnected: edge 0-1 and edge 2-3, each single edge, total 2
    std::vector<std::vector<long>> g10 = {{1}, {0}, {3}, {2}};
    assert(countMaximalMatchingSize(g10) == 2);

    return 0;
}
