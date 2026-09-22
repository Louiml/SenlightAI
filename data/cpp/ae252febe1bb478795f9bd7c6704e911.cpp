Write a C++ function `std::vector<int> lexicographicallySmallestTopologicalOrder(int n, const std::vector<std::pair<int,int>>& edges)` that, given a directed graph with vertices numbered `1` through `n` and a list of directed edges (where each edge `(from,to)` means `from` must appear before `to`), returns a topological ordering of all vertices. If multiple topological orders are possible, return the one that is lexicographically smallest when the sequence is read from left to right. If the graph contains a cycle (i.e., no topological order exists), return an empty vector. The function must be `const`-correct, use no global state, and handle up to `n = 500` vertices and any number of edges (including duplicate edges, which should be treated as a single logical constraint). The returned order must include every vertex exactly once.

The core algorithm is Kahn’s algorithm for topological sorting combined with a min-heap (priority queue) to always pick the smallest available vertex with zero in-degree. First, build an adjacency list and compute in-degrees from the given edges. Duplicate edges are naturally handled because we increment in-degree once per edge, but if duplicates exist, the adjacency list may contain duplicates, causing the in-degree to decrease multiple times when processing a node, which would incorrectly reduce the in-degree below zero. To avoid this, either deduplicate edges or maintain a structure that prevents multiple decrements. A simpler robust approach: use a `set` for each adjacency list to deduplicate edges before computing in-degrees. After deduplication, compute in-degrees by iterating over all unique edges. Initialize a min-heap (`priority_queue<int, vector<int>, greater<int>>`) with all vertices of in-degree zero. While the heap is not empty, pop the smallest vertex, append it to the result, and for each unique neighbor decrease its in-degree; if it reaches zero, push it into the heap. If the result length equals `n`, return it; otherwise, a cycle exists, so return an empty vector. Edge cases: an empty graph (`n=0` or no edges) should return all vertices in ascending order; a graph with a cycle should return empty; vertices with no incoming edges are initially ready. Time complexity is O(V + E) for the topological sort after deduplication, but deduplication using sets costs O(E log E) worst-case; overall O(n + m log m) where m is number of edges. Space complexity O(n + m) for adjacency and in-degree storage.

#include <vector>
#include <queue>
#include <set>
#include <functional>

// Returns a lexicographically smallest topological ordering of vertices 1..n.
// If a cycle exists, returns an empty vector.
std::vector<int> lexicographicallySmallestTopologicalOrder(
    int n,
    const std::vector<std::pair<int, int>>& edges) {
    // Deduplicate edges using adjacency sets
    std::vector<std::set<int>> adj(n + 1);
    std::vector<int> inDegree(n + 1, 0);
    for (const auto& [from, to] : edges) {
        if (from < 1 || from > n || to < 1 || to > n) continue; // ignore invalid
        if (adj[from].insert(to).second) { // only if new edge
            inDegree[to]++;
        }
    }

    // Min-heap to always pick smallest available vertex
    std::priority_queue<int, std::vector<int>, std::greater<int>> ready;
    for (int v = 1; v <= n; ++v) {
        if (inDegree[v] == 0) {
            ready.push(v);
        }
    }

    std::vector<int> order;
    order.reserve(n);
    while (!ready.empty()) {
        int u = ready.top();
        ready.pop();
        order.push_back(u);
        for (int v : adj[u]) {
            if (--inDegree[v] == 0) {
                ready.push(v);
            }
        }
    }

    if ((int)order.size() == n) {
        return order;
    }
    return {}; // cycle detected
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Simple chain: 1->2->3
    std::vector<std::pair<int,int>> e1 = {{1,2},{2,3}};
    assert(lexicographicallySmallestTopologicalOrder(3, e1) == std::vector<int>({1,2,3}));

    // Multiple orders: 1->2, 1->3, 2->4, 3->4
    std::vector<std::pair<int,int>> e2 = {{1,2},{1,3},{2,4},{3,4}};
    // Smallest lexicographic: start with 1, then 2 (since 2<3 after 1), then 3, then 4
    assert(lexicographicallySmallestTopologicalOrder(4, e2) == std::vector<int>({1,2,3,4}));

    // Cycle: 1->2, 2->1
    std::vector<std::pair<int,int>> e3 = {{1,2},{2,1}};
    assert(lexicographicallySmallestTopologicalOrder(2, e3).empty());

    // No edges: all isolated, should return sorted ascending
    std::vector<std::pair<int,int>> e4 = {};
    assert(lexicographicallySmallestTopologicalOrder(5, e4) == std::vector<int>({1,2,3,4,5}));

    // Duplicate edges: 1->2 repeated
    std::vector<std::pair<int,int>> e5 = {{1,2},{1,2},{2,3}};
    assert(lexicographicallySmallestTopologicalOrder(3, e5) == std::vector<int>({1,2,3}));

    // Disjoint components: 2->1, 4->3
    std::vector<std::pair<int,int>> e6 = {{2,1},{4,3}};
    // Vertices 1,3 have in-degree 1; 2,4 have 0. Lexicographically smallest: 2,4,1,3
    assert(lexicographicallySmallestTopologicalOrder(4, e6) == std::vector<int>({2,4,1,3}));

    // n=0
    assert(lexicographicallySmallestTopologicalOrder(0, {}).empty());

    // Larger test: multiple roots with constraints
    std::vector<std::pair<int,int>> e7 = {{3,1},{3,2},{4,1}};
    // Available roots: 3,4 (since 1,2 have incoming). Pick 3, then after removing 3, 4 and 1,2? Actually 1,2 still have in-degree from 4? Let's compute: in-degrees: 1 from 3 and 4 (2), 2 from 3 (1), 3 (0), 4 (0). Ready: 3,4. Pick 3, decrement 1 to 1, 2 to 0 -> push 2. Ready: 2,4. Pick 2. Then ready: 4. Pick 4, decrement 1 to 0 -> push 1. Then 1. Order: 3,2,4,1.
    assert(lexicographicallySmallestTopologicalOrder(4, e7) == std::vector<int>({3,2,4,1}));

    return 0;
}
