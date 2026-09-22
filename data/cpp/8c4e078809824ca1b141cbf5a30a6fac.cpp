Write a C++ function `determineConsistency(int n, const std::vector<std::tuple<int,int,int>>& edges)` that, given `n` vertices labeled `0` to `n-1` and a list of directed edges `(u, v, w)` meaning there is a constraint `pos[v] - pos[u] = w`, determines whether there exists an assignment of integer values to `pos[0..n-1]` satisfying all given constraints simultaneously. The graph may contain cycles, multiple edges between the same pair, and isolated vertices. The function should return `true` if the system is consistent, and `false` otherwise. The input edges are guaranteed to form a valid DAG (no directed cycles in the constraint graph) — however, note that the equality constraints could still be contradictory (e.g., `pos[1]-pos[0]=1` and also `pos[1]-pos[0]=2`). You may assume the graph is a DAG and you are only checking consistency of the numeric labels.

#include <cassert>
#include <vector>
#include <tuple>

// include the solution function here (or link it)

int main() {
    // Simple consistent constraints: 0->1 (w=5), 1->2 (w=3) => pos[0]=0, pos[1]=5, pos[2]=8
    std::vector<std::tuple<int,int,int>> e1 = {{0,1,5}, {1,2,3}};
    assert(determineConsistency(3, e1) == true);

    // Contradictory multiple edges: 0->1 (w=1) and 0->1 (w=2) — same source/target with different weights
    std::vector<std::tuple<int,int,int>> e2 = {{0,1,1}, {0,1,2}};
    assert(determineConsistency(2, e2) == false);

    // Conflict via two different paths: 0->1 (w=1), 0->2 (w=2), 2->1 (w=2) => pos[1]=1 from direct, but pos[1]=2+2=4 from via 2
    std::vector<std::tuple<int,int,int>> e3 = {{0,1,1}, {0,2,2}, {2,1,2}};
    assert(determineConsistency(3, e3) == false);

    // Consistent path with alternative path that matches: 0->1 (w=3), 0->2 (w=1), 2->1 (w=2) => both give pos[1]=3
    std::vector<std::tuple<int,int,int>> e4 = {{0,1,3}, {0,2,1}, {2,1,2}};
    assert(determineConsistency(3, e4) == true);

    // Isolated vertex: n=2, only edge 0->1 (w=10) => consistent
    std::vector<std::tuple<int,int,int>> e5 = {{0,1,10}};
    assert(determineConsistency(2, e5) == true);

    // Empty edges, multi-vertex
    std::vector<std::tuple<int,int,int>> e6 = {};
    assert(determineConsistency(5, e6) == true);

    // Self-loop would break DAG, but we don't test that per spec.

    return 0;
}

#include <vector>
#include <tuple>
#include <queue>

// Determine if all constraints pos[v] - pos[u] = w are consistent for a DAG.
bool determineConsistency(int n, const std::vector<std::tuple<int,int,int>>& edges) {
    // Build adjacency list and in-degrees
    std::vector<std::vector<std::pair<int,int>>> adj(n);
    std::vector<int> in_deg(n, 0);
    for (const auto& e : edges) {
        int u, v, w;
        std::tie(u, v, w) = e;
        adj[u].emplace_back(v, w);
        ++in_deg[v];
    }

    // Kahn's algorithm for topological sort, assigning positions
    std::vector<int> pos(n, 0);
    std::queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (in_deg[i] == 0) {
            q.push(i);
        }
    }

    int visited = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        ++visited;
        for (const auto& [v, w] : adj[u]) {
            pos[v] = pos[u] + w;  // assign on first encounter (DAG guarantees single assignment)
            if (--in_deg[v] == 0) {
                q.push(v);
            }
        }
    }

    // If not all vertices visited, graph has a cycle — but problem guarantees DAG.
    // Still handle gracefully:
    if (visited != n) return false;

    // Verify all edges
    for (const auto& e : edges) {
        int u, v, w;
        std::tie(u, v, w) = e;
        if (pos[u] + w != pos[v]) {
            return false;
        }
    }
    return true;
}

// The problem is essentially checking whether a system of difference/equality constraints is consistent. Since the graph is a DAG, we can perform a topological sort. During the topological traversal, we assign tentative positions: starting from vertices with in-degree 0, we set their `pos` to 0 (or any arbitrary base value). Then, for each edge `(u,v,w)` traversed during the BFS/queue-based Kahn's algorithm, we compute `pos[v] = pos[u] + w`. Because the graph is a DAG, each vertex will have its `pos` set exactly once during the topological order (the first time it becomes reachable). After computing all positions, we verify every edge in the original list satisfies `pos[u] + w == pos[v]`. If any edge fails, the constraints are contradictory (e.g., multiple paths to the same vertex with conflicting sums). A subtle edge case is when the graph has multiple connected components — each component is handled independently, and since we start all roots with pos=0, there is no cross-component conflict. If the graph is not connected, it's fine. Also, if a vertex is isolated, its pos remains 0, which is trivially consistent. The time complexity is O(n+m) for the topological sort and verification, and space complexity is O(n+m) for adjacency lists. If the input graph were not a DAG, we would need a different approach (like Bellman-Ford), but per the problem statement we can rely on DAG property.
