Write a C++ function `int maximumBipartiteMatching(int leftCount, int rightCount, const std::vector<std::pair<int,int>>& edges)` that, given the number of vertices on the left side (`leftCount`, indexed 1..leftCount) and right side (`rightCount`, indexed 1..rightCount) of a bipartite graph, plus a list of undirected edges (each edge connects a left vertex `a` to a right vertex `b`), returns the size of the maximum matching (the largest set of edges such that no two edges share a vertex). The graph is simple (no duplicate edges or self-loops). Vertices on the right are internally renumbered as `leftCount + b` for algorithmic convenience. The function must handle up to 100,000 vertices on each side and up to 200,000 edges, and it should run efficiently, typically using the Hopcroft–Karp algorithm. If the input is empty (no edges), the result is 0. Edge cases include vertices with no edges, multiple edges between the same pair (though guaranteed not to happen), and the case where leftCount or rightCount is zero.

The core problem is finding a maximum cardinality matching in an unweighted bipartite graph. The straightforward augmenting path algorithm (e.g., Kuhn’s algorithm) runs in O(V·E) which is too slow for large inputs. Instead, we use the Hopcroft–Karp algorithm, which has a worst-case time complexity of O(E·√V) and in practice runs much faster for sparse graphs. The algorithm works in phases: in each phase, it performs a BFS from all unmatched left vertices to build a layered graph (with layers based on distances) and finds the shortest augmenting paths (paths that start at an unmatched left vertex, alternate between unmatched and matched edges, and end at an unmatched right vertex). Then it runs a DFS from each unmatched left vertex to find vertex-disjoint augmenting paths in that layer graph, augmenting the matching along them. The algorithm repeats until no more augmenting paths are found. Key details: we maintain two match arrays `pairU` (matching for left vertices, 0 means unmatched) and `pairV` (matching for right vertices, renumbered as leftCount + b). The BFS starts by enqueuing all unmatched left vertices with distance 0, and uses a dummy node 0 to represent the "NIL" vertex; BFS explores edges and updates distances for left vertices that can be reached via a right vertex. The DFS recursively attempts to find an augmenting path from a given left vertex; if successful, it flips the matching edges. The algorithm terminates when BFS cannot reach the dummy node (distance of dummy is INF). The implementation must be careful about 1-indexing and renumbering right vertices. Memory usage is O(V+E) for adjacency lists. We also need to handle potential duplicate edges by ignoring them (or the input guarantees no duplicates). Worst-case time is O(E·√V), but for typical inputs it is near O(E). All operations are linear in the number of edges per BFS/DFS phase.

#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

// Returns the maximum matching size in a bipartite graph with leftCount left vertices
// (indexed 1..leftCount) and rightCount right vertices (indexed 1..rightCount).
// Edges are given as pairs (leftVertex, rightVertex).
int maximumBipartiteMatching(int leftCount, int rightCount, const std::vector<std::pair<int,int>>& edges) {
    const int INF = std::numeric_limits<int>::max() / 2;
    const int NIL = 0;
    int totalVertices = leftCount + rightCount;

    // Build adjacency list: right vertices are renumbered as leftCount + b
    std::vector<std::vector<int>> adj(totalVertices + 1);
    for (const auto& e : edges) {
        int u = e.first;
        int v = leftCount + e.second;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // pairU[u] = matched right vertex for left u, 0 if unmatched
    // pairV[v] = matched left vertex for right v, 0 if unmatched
    std::vector<int> pairU(leftCount + 1, NIL);
    std::vector<int> pairV(totalVertices + 1, NIL);
    std::vector<int> dist(leftCount + 1);

    auto bfs = [&]() -> bool {
        std::queue<int> q;
        for (int u = 1; u <= leftCount; ++u) {
            if (pairU[u] == NIL) {
                dist[u] = 0;
                q.push(u);
            } else {
                dist[u] = INF;
            }
        }
        dist[NIL] = INF;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (dist[u] < dist[NIL]) {
                for (int v : adj[u]) {
                    int nextU = pairV[v];
                    if (nextU == NIL) nextU = NIL; // placeholder, but pairV[v] is 0 if unmatched
                    if (dist[nextU] == INF) {
                        dist[nextU] = dist[u] + 1;
                        q.push(nextU);
                    }
                }
            }
        }
        return dist[NIL] != INF;
    };

    // DFS to find augmenting paths; returns true if an augmenting path from u is found
    std::function<bool(int)> dfs = [&](int u) -> bool {
        if (u != NIL) {
            for (int v : adj[u]) {
                int nextU = pairV[v];
                if (dist[nextU] == dist[u] + 1 && dfs(nextU)) {
                    pairU[u] = v;
                    pairV[v] = u;
                    return true;
                }
            }
            dist[u] = INF;
            return false;
        }
        return true;
    };

    int matching = 0;
    while (bfs()) {
        for (int u = 1; u <= leftCount; ++u) {
            if (pairU[u] == NIL && dfs(u)) {
                matching++;
            }
        }
    }

    return matching;
}

#include <cassert>
#include <vector>
#include <utility>

int maximumBipartiteMatching(int leftCount, int rightCount, const std::vector<std::pair<int,int>>& edges);

int main() {
    // Test 1: Empty graph
    assert(maximumBipartiteMatching(3, 3, {}) == 0);

    // Test 2: Single edge
    assert(maximumBipartiteMatching(1, 1, {{1,1}}) == 1);

    // Test 3: Simple star: left 1 connects to all rights, others have no edges
    assert(maximumBipartiteMatching(3, 2, {{1,1},{1,2}}) == 1);

    // Test 4: Perfect matching (bijection)
    assert(maximumBipartiteMatching(2, 2, {{1,1},{1,2},{2,1},{2,2}}) == 2);

    // Test 5: Path that requires careful augmenting
    // Left 1-2, Left 2-1, Left 2-2, Left 3-1 → maximum 2 (e.g., 1-2 and 3-1)
    assert(maximumBipartiteMatching(3, 2, {{1,2},{2,1},{2,2},{3,1}}) == 2);

    // Test 6: No possible matching at all
    assert(maximumBipartiteMatching(3, 3, {{1,1}}) == 1); // actually one edge is possible

    // Test 7: Larger pattern where greedy fails
    // left 1 connects to right 1, left 2 connects to right 1 and 2, left 3 connects to right 2 → max 2
    assert(maximumBipartiteMatching(3, 2, {{1,1},{2,1},{2,2},{3,2}}) == 2);

    // Test 8: One side has zero vertices
    assert(maximumBipartiteMatching(0, 5, {}) == 0);
    assert(maximumBipartiteMatching(5, 0, {}) == 0);

    // Test 9: Multiple edges between same pair (should not happen but just in case)
    // Here we assume input is simple, but test with duplicates if allowed (we'll ignore duplicates)
    assert(maximumBipartiteMatching(2, 2, {{1,1},{1,1},{2,2}}) == 2);

    // Test 10: Non-trivial graph with 4 left, 4 right, maximum matching 3
    assert(maximumBipartiteMatching(4, 4, {{1,1},{1,2},{2,2},{2,3},{3,3},{3,4},{4,4}}) == 3);

    return 0;
}
