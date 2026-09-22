Write a C++ function `bool hasDiamondPath(const vector<vector<int>>& adj)` that, given an adjacency list representation of a directed graph with vertices numbered `0` to `n-1`, returns `true` if there exists any vertex from which there are two distinct paths to the same destination vertex (i.e., a diamond pattern: `u → v` and `u → w` with `v` and `w` both having a path to some common vertex `x`, where the paths are not identical), and `false` otherwise. The graph may contain self‑loops, multiple edges, and cycles. The function must not modify the input graph. The graph is unweighted; only reachability matters, not edge weights.

The problem reduces to detecting whether any two different paths of length at least 1 from a common start vertex converge at the same intermediate vertex. A straightforward approach is to run a modified BFS/DFS from each vertex as the start. For each start vertex `s`, we perform a BFS that records, for every visited vertex `v`, from which immediate predecessor it was first reached. If we ever encounter a vertex `v` that can be reached via two different immediate predecessors (i.e., from two distinct vertices `p1` and `p2` such that there are edges `p1→v` and `p2→v` and both `p1` and `p2` are reachable from `s`), then we have found two distinct paths from `s` to `v` (one passing through `p1`, the other through `p2`), which is a diamond. However, careful: if a vertex has a self‑loop, we must avoid counting the same edge twice. Also, if the graph has direct edges from `s` to `v` via two different parallel edges, that also counts as two distinct paths—so we need to check the number of distinct predecessors, not just one per vertex. A simpler method: for each start `s`, run a BFS that computes the set of all vertices reachable from `s`. Then, for each vertex `v` reachable (including possibly `s` itself if there is a cycle), count the number of distinct in‑neighbors `u` such that `u` is also reachable from `s` and there is an edge `u→v`. If that count ≥ 2, then there are two distinct paths from `s` to `v` (via different last hops). This correctly identifies a diamond. Edge cases: if `v == s` and there are two distinct ways to return to `s` (including self‑loops or two parallel edges from another vertex), that also counts. The algorithm runs BFS from each of the `n` vertices, each BFS is O(n + m) where `m` is total number of edges, so total time is O(n·(n+m)). Space complexity is O(n) per BFS for visited and for counting predecessors, plus the adjacency list storage. Since the problem is small (n ≤ 1000 in the original snippet), this is acceptable.

#include <vector>
#include <queue>
#include <unordered_set>

// Returns true if there exist two distinct paths from some vertex to another vertex.
bool hasDiamondPath(const std::vector<std::vector<int>>& adj) {
    const int n = static_cast<int>(adj.size());
    for (int s = 0; s < n; ++s) {
        // BFS to find all vertices reachable from s.
        std::vector<bool> reachable(n, false);
        std::queue<int> q;
        q.push(s);
        reachable[s] = true;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (!reachable[v]) {
                    reachable[v] = true;
                    q.push(v);
                }
            }
        }
        // For each reachable vertex v, count distinct in-neighbors that are also reachable.
        for (int v = 0; v < n; ++v) {
            if (!reachable[v]) continue;
            std::unordered_set<int> preds;
            for (int u = 0; u < n; ++u) {
                if (reachable[u]) {
                    for (int w : adj[u]) {
                        if (w == v) {
                            preds.insert(u);
                            break; // only need to record u once per edge (even if multiple parallel edges)
                        }
                    }
                }
            }
            // If v has at least two distinct reachable predecessors, we have two distinct paths.
            // But we must also ensure that the paths are not identical: if v == s, a self-loop on s
            // gives only one distinct path (the self-loop itself), but if there is also another edge
            // from some other reachable u to s, then preds.size() >= 2.
            if (preds.size() >= 2) {
                return true;
            }
        }
    }
    return false;
}

#include <cassert>
#include <vector>

int main() {
    // No edges: no diamond.
    std::vector<std::vector<int>> g0(3);
    assert(hasDiamondPath(g0) == false);

    // Simple chain 0->1->2: no diamond.
    std::vector<std::vector<int>> g1(3);
    g1[0].push_back(1);
    g1[1].push_back(2);
    assert(hasDiamondPath(g1) == false);

    // Diamond: 0->1, 0->2, 1->3, 2->3.
    std::vector<std::vector<int>> g2(4);
    g2[0].push_back(1);
    g2[0].push_back(2);
    g2[1].push_back(3);
    g2[2].push_back(3);
    assert(hasDiamondPath(g2) == true);

    // Two parallel edges from 0 to 1: two distinct paths to 1.
    std::vector<std::vector<int>> g3(2);
    g3[0].push_back(1);
    g3[0].push_back(1);
    assert(hasDiamondPath(g3) == true);

    // Cycle with two distinct ways back to start: 0->1, 1->0, 0->2, 2->0.
    std::vector<std::vector<int>> g4(3);
    g4[0].push_back(1);
    g4[1].push_back(0);
    g4[0].push_back(2);
    g4[2].push_back(0);
    assert(hasDiamondPath(g4) == true);

    // Single self-loop: only one path from 0 to 0 (the self-loop), no diamond.
    std::vector<std::vector<int>> g5(1);
    g5[0].push_back(0);
    assert(hasDiamondPath(g5) == false);

    // Self-loop plus another edge from another vertex to 0: two paths to 0.
    std::vector<std::vector<int>> g6(2);
    g6[0].push_back(0); // self-loop
    g6[1].push_back(0);
    assert(hasDiamondPath(g6) == true);

    // Disconnected graph with a diamond in one component.
    std::vector<std::vector<int>> g7(6);
    // Component A: 0->1, 0->2, 1->3, 2->3 (diamond)
    g7[0].push_back(1);
    g7[0].push_back(2);
    g7[1].push_back(3);
    g7[2].push_back(3);
    // Component B: 4->5 only
    g7[4].push_back(5);
    assert(hasDiamondPath(g7) == true);

    // No diamond even with multiple edges from different starts that do not converge.
    std::vector<std::vector<int>> g8(4);
    g8[0].push_back(1);
    g8[2].push_back(3);
    assert(hasDiamondPath(g8) == false);

    return 0;
}
