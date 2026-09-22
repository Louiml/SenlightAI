Write a standalone C++ function that takes a directed acyclic graph (DAG) represented as an adjacency list (using the provided `alGraph` class) and two vertex indices `source` and `end`, and returns the length of the longest path from `source` to `end` in the graph (also known as the critical path length). The graph has non-negative edge weights. If there is no path from `source` to `end`, the function should return `-1`. The graph is guaranteed to be a DAG (no cycles). You may use the existing member functions of the `alGraph` class (e.g., `GetInDegree`, `GetOutDegree`, `GetWeight`, `inRange`) but must not modify the class definition. The solution must be a free function named `longestPath` that takes a reference to a constant `alGraph` (but since the class lacks const-correct methods, you may take a non-const reference and avoid modifying it), the source vertex, and the end vertex, and returns an `int` representing the longest path length.
// The problem is a classic longest path in a DAG. Since the graph is acyclic, we can compute the longest path using dynamic programming on a topological order. First, we compute a topological ordering of all vertices. Because we only care about paths from `source`, we can initialize a distance array `dist` with negative infinity (or a very small number) and set `dist[source] = 0`. Then, we process vertices in topological order. For each vertex `u` in that order, if `dist[u]` is reachable (not negative infinity), we iterate over all outgoing edges `(u, v, w)` and relax: `dist[v] = max(dist[v], dist[u] + w)`. At the end, if `dist[end]` is still negative infinity (or equivalently we can use a sentinel like `INT_MIN` and check if it's still that value), there is no path, so return `-1`; otherwise return `dist[end]`. Edge cases: source equals end – the longest path from a vertex to itself should be 0 (if we consider an empty path, but often it's 0; the specification says "longest path", so we return 0). If the graph has negative weights, the algorithm still works because it's a DAG; but weights here are non-negative. To compute topological order, we can use Kahn's algorithm (indegree-based) or a DFS with post-order. Kahn's algorithm: compute indegree of every vertex, push all zero-indegree vertices into a queue, repeatedly pop and reduce indegree of neighbors; the order of popping gives a topological order. If during this process we don't visit all vertices (i.e., there's a cycle), but the problem guarantees DAG, so that won't happen. Time complexity: O(V + E) for topological sort plus O(V + E) for relaxation, total O(V + E). Space complexity: O(V) for the topological order array and distance array.
#include <queue>
#include <vector>
#include <climits>
#include "..\include\Graph\adjList.h"

// Helper function to get topological order using Kahn's algorithm.
std::vector<int> topologicalOrder(alGraph& graph) {
    int n = graph.vertex_num;
    std::vector<int> indeg(n);
    for (int i = 0; i < n; ++i) {
        indeg[i] = graph.GetInDegree(i);
    }
    std::queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (indeg[i] == 0) q.push(i);
    }
    std::vector<int> order;
    order.reserve(n);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);
        // Access adjacency list. Since alGraph does not have a const iterator,
        // we need to use its internal structure. We'll use GetOutDegree and
        // iterate via the public method? There is no public getter for edges.
        // But we can access g[i] via the public member? In the snippet, g is public
        // (though not shown in the class definition, but likely accessible).
        // We'll assume g is accessible. If not, we'd need to add a friend or use a method.
        // For safety, we use the given public methods: we can't iterate all edges directly.
        // However, we can use the fact that the class provides inRange and we can
        // use a helper method? The problem says we can use existing member functions.
        // But to iterate edges, we need to access g[i]->next. Since g is public (based on snippet),
        // we'll use it.
        for (vert* e = graph.g[u]->next; e != nullptr; e = e->next) {
            int v = e->index;
            if (--indeg[v] == 0) q.push(v);
        }
    }
    // If order size != n, there is a cycle, but problem guarantees DAG.
    return order;
}

// Returns the length of the longest path from source to end in the given DAG.
// Returns -1 if no path exists.
int longestPath(alGraph& graph, int source, int end) {
    int n = graph.vertex_num;
    if (source < 0 || source >= n || end < 0 || end >= n) return -1;

    // Get topological order.
    std::vector<int> topo = topologicalOrder(graph);

    // Initialize distances with a very small value (negative infinity).
    const int NEG_INF = INT_MIN / 2; // avoid overflow
    std::vector<int> dist(n, NEG_INF);
    dist[source] = 0;

    // Process vertices in topological order.
    for (int u : topo) {
        if (dist[u] == NEG_INF) continue; // not reachable from source
        // Relax all outgoing edges of u.
        for (vert* e = graph.g[u]->next; e != nullptr; e = e->next) {
            int v = e->index;
            int w = e->weight;
            if (dist[u] + w > dist[v]) {
                dist[v] = dist[u] + w;
            }
        }
    }

    // If dist[end] is still NEG_INF, no path.
    if (dist[end] == NEG_INF) return -1;
    return dist[end];
}
#include <cassert>
#include "..\include\Graph\adjList.h"

int main() {
    // Test 1: Simple 3-vertex path: 0->1 (weight 5), 1->2 (weight 3). Longest path from 0 to 2 = 8.
    {
        alGraph g(3);
        g.AddEdge(0, 1, 5);
        g.AddEdge(1, 2, 3);
        assert(longestPath(g, 0, 2) == 8);
    }

    // Test 2: Graph with branching: 0->1 (2), 0->2 (4), 1->3 (1), 2->3 (3). Longest 0->3 = 7 (via 2).
    {
        alGraph g(4);
        g.AddEdge(0, 1, 2);
        g.AddEdge(0, 2, 4);
        g.AddEdge(1, 3, 1);
        g.AddEdge(2, 3, 3);
        assert(longestPath(g, 0, 3) == 7);
    }

    // Test 3: No path: 0->1, but end=2. Should return -1.
    {
        alGraph g(3);
        g.AddEdge(0, 1, 1);
        assert(longestPath(g, 0, 2) == -1);
    }

    // Test 4: Source equals end: longest path from 0 to 0 is 0.
    {
        alGraph g(2);
        g.AddEdge(0, 1, 4);
        assert(longestPath(g, 0, 0) == 0);
    }

    // Test 5: Disconnected graph with source and end in different components.
    {
        alGraph g(4);
        g.AddEdge(0, 1, 2);
        g.AddEdge(2, 3, 5);
        assert(longestPath(g, 0, 3) == -1);
    }

    // Test 6: More complex DAG with multiple options: 0->1(1), 0->2(10), 1->3(100), 2->3(1). Longest 0->3 = 101.
    {
        alGraph g(4);
        g.AddEdge(0, 1, 1);
        g.AddEdge(0, 2, 10);
        g.AddEdge(1, 3, 100);
        g.AddEdge(2, 3, 1);
        assert(longestPath(g, 0, 3) == 101);
    }

    // Test 7: A path with zero-weight edges: 0->1 (0), 1->2 (0). Longest = 0.
    {
        alGraph g(3);
        g.AddEdge(0, 1, 0);
        g.AddEdge(1, 2, 0);
        assert(longestPath(g, 0, 2) == 0);
    }

    // Test 8: Graph with self-loop would break DAG, but we trust input. Not needed.

    return 0;
}
