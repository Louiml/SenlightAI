Write a C++ function `long long shortestJourney(const std::vector<std::vector<int>>& graph, const std::vector<std::vector<int>>& revGraph, int n, int c, int a, int b)` that takes an unweighted directed graph with `n` vertices (numbered 0 to n-1), the original adjacency list, the reversed adjacency list, a special central node `c`, and two query nodes `a` and `b`. The function must return the shortest total number of edges in a journey that starts at node `a`, travels along directed edges to reach node `c` (any path, possibly of length 0 if `a == c`), and then travels along directed edges from `c` to node `b` (again possibly length 0 if `c == b`). If it is impossible to reach `c` from `a` or impossible to reach `b` from `c`, return `-1`. The graph is guaranteed to have no self-loops but may contain multiple edges. You must implement the algorithm efficiently for large graphs, and the function should **not** use any global or static arrays (all state must be local).

#include <cassert>
#include <vector>

// The solution function is assumed to be defined above (in the same file).
// For this test, we include it here or reference it.
// (In a real combined file, the function is already available.)

int main() {
    // Test 1: Simple chain 0->1->2, c=1, a=0, b=2 -> 1+1=2
    {
        int n = 3;
        std::vector<std::vector<int>> graph(n), revGraph(n);
        graph[0].push_back(1);
        graph[1].push_back(2);
        revGraph[1].push_back(0);
        revGraph[2].push_back(1);
        assert(shortestJourney(graph, revGraph, n, 1, 0, 2) == 2);
    }

    // Test 2: a==c and b==c -> 0
    {
        int n = 2;
        std::vector<std::vector<int>> graph(n), revGraph(n);
        assert(shortestJourney(graph, revGraph, n, 1, 1, 1) == 0);
    }

    // Test 3: impossible to reach c from a (a=0, c=1, no edge)
    {
        int n = 2;
        std::vector<std::vector<int>> graph(n), revGraph(n);
        assert(shortestJourney(graph, revGraph, n, 1, 0, 1) == -1);
    }

    // Test 4: impossible to reach b from c (c=0, b=1, no edge)
    {
        int n = 2;
        std::vector<std::vector<int>> graph(n), revGraph(n);
        assert(shortestJourney(graph, revGraph, n, 0, 0, 1) == -1);
    }

    // Test 5: multiple edges and cycle, c=0, a=2->0 cost 1, 0->1 cost 1 -> total 2
    {
        int n = 3;
        std::vector<std::vector<int>> graph(n), revGraph(n);
        graph[0].push_back(1);
        graph[1].push_back(0); // cycle
        graph[2].push_back(0);
        graph[2].push_back(0); // multiple edges
        revGraph[1].push_back(0);
        revGraph[0].push_back(1);
        revGraph[0].push_back(2);
        revGraph[0].push_back(2);
        assert(shortestJourney(graph, revGraph, n, 0, 2, 1) == 2);
    }

    // Test 6: longer path, c=0, a=3->2->1->0 cost 3, 0->4->5 cost 2 => 5
    {
        int n = 6;
        std::vector<std::vector<int>> graph(n), revGraph(n);
        // edges for toC path: 3->2, 2->1, 1->0
        graph[3].push_back(2);
        graph[2].push_back(1);
        graph[1].push_back(0);
        // edges for fromC path: 0->4, 4->5
        graph[0].push_back(4);
        graph[4].push_back(5);
        // reverse edges
        revGraph[2].push_back(3);
        revGraph[1].push_back(2);
        revGraph[0].push_back(1);
        revGraph[4].push_back(0);
        revGraph[5].push_back(4);
        assert(shortestJourney(graph, revGraph, n, 0, 3, 5) == 5);
    }

    // Test 7: c not reachable from a, but c can reach b -> -1
    {
        int n = 4;
        std::vector<std::vector<int>> graph(n), revGraph(n);
        // c=0, b=2: 0->1->2; a=3 with no outgoing edge
        graph[0].push_back(1);
        graph[1].push_back(2);
        revGraph[1].push_back(0);
        revGraph[2].push_back(1);
        assert(shortestJourney(graph, revGraph, n, 0, 3, 2) == -1);
    }

    // Test 8: a can reach c, but c cannot reach b -> -1
    {
        int n = 4;
        std::vector<std::vector<int>> graph(n), revGraph(n);
        // a=3, c=0: 3->2->0 ; b=1: no outgoing from 0 to 1
        graph[3].push_back(2);
        graph[2].push_back(0);
        revGraph[2].push_back(3);
        revGraph[0].push_back(2);
        assert(shortestJourney(graph, revGraph, n, 0, 3, 1) == -1);
    }

    return 0;
}

#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

// Return the shortest journey distance (edges) from a to c then c to b,
// or -1 if not possible. Uses BFS on the original and reversed graphs.
long long shortestJourney(const std::vector<std::vector<int>>& graph,
                          const std::vector<std::vector<int>>& revGraph,
                          int n, int c, int a, int b) {
    const int INF = std::numeric_limits<int>::max();
    std::vector<int> distFromC(n, INF); // distances from c in original graph
    std::vector<int> distToC(n, INF);   // distances to c (from node to c)

    // BFS from c on original graph to find distances to all nodes
    std::queue<int> q;
    distFromC[c] = 0;
    q.push(c);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int u : graph[v]) {
            if (distFromC[u] == INF) {
                distFromC[u] = distFromC[v] + 1;
                q.push(u);
            }
        }
    }

    // BFS from c on reversed graph to find distances from each node to c
    q.push(c);
    distToC[c] = 0;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int u : revGraph[v]) {
            if (distToC[u] == INF) {
                distToC[u] = distToC[v] + 1;
                q.push(u);
            }
        }
    }

    if (distToC[a] == INF || distFromC[b] == INF) {
        return -1;
    }
    return static_cast<long long>(distToC[a]) + distFromC[b];
}

// The key observation is that the journey from `a` to `c` uses the original graph, while the journey from `c` to `b` also uses the original graph. However, to compute the distance from every node to `c` efficiently, we can run a BFS on the **reversed** graph starting from `c`. In the reversed graph, a path from `c` to some node `x` corresponds exactly to a path from `x` to `c` in the original graph. Similarly, to compute distances from `c` to every node, we run a BFS on the original graph starting from `c`. Then for a query `(a,b)`, the answer is `distToC[a] + distFromC[b]` if both distances are finite; otherwise return `-1`. Edge cases: if `a == c`, then `distToC[a] = 0` (BFS initializes it), and similarly `b == c` gives `0`; if the graph is disconnected, some distances remain `INF`. Since we use BFS, each BFS takes `O(n+m)` time, and memory is `O(n+m)` for the graph plus `O(n)` for distance arrays. Multiple edges do not affect correctness because BFS only considers the first visit (shorter distance will be found first). Self-loops are irrelevant as they would not shorten any path. The solution avoids global state by passing graphs by const reference.
