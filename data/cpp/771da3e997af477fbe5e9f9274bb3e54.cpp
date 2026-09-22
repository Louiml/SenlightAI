// Write a standalone C++ function that parses a simplified parity game description from an input string and computes the winning region for a single-player (system-only) parity game, returning a boolean indicating whether the initial vertex is winning. The input format is: first line contains three integers `n m init` (number of vertices, number of edges, initial vertex id), followed by `n` lines each containing the priority of a vertex (a non-negative integer), followed by `m` lines each containing two integers `u v` representing a directed edge from `u` to `v`. A vertex is winning if the player can force that the maximum priority visited infinitely often is even. The function should output the set of winning vertices as a sorted vector of vertex ids, and return `true` if the initial vertex is in that set. Assume the graph is finite, deterministic (each vertex has at least one outgoing edge), and priorities are non-negative integers. The function signature: `bool solveParityGame(const std::string& input, std::vector<int>& winningVertices)`.
#include <cassert>
#include <vector>
#include <string>

// Function declaration (including from solution above)
bool solveParityGame(const std::string& input, std::vector<int>& winningVertices);

int main() {
    // Test 1: simple two-node cycle, priorities 1 and 2 (max even -> winning from both)
    {
        std::string input = "2 2 0\n1\n2\n0 1\n1 0\n";
        std::vector<int> win;
        assert(solveParityGame(input, win) == true);
        assert(win == std::vector<int>({0,1}));
    }
    // Test 2: two nodes, node0 has priority 1 self-loop, node1 priority 2 self-loop
    // Node0 cannot be winning because max priority on its only cycle is 1 (odd), but node1 is winning.
    {
        std::string input = "2 2 0\n1\n2\n0 0\n1 1\n";
        std::vector<int> win;
        assert(solveParityGame(input, win) == false);
        assert(win == std::vector<int>({1}));
    }
    // Test 3: chain 0->1, 1 has priority 2 self-loop, 0 priority 1. 0 can reach 1's good cycle, so winning.
    {
        std::string input = "2 2 0\n1\n2\n0 1\n1 1\n";
        std::vector<int> win;
        assert(solveParityGame(input, win) == true);
        assert(win == std::vector<int>({0,1}));
    }
    // Test 4: no good cycle (all priorities odd). All vertices losing.
    {
        std::string input = "2 2 0\n1\n3\n0 1\n1 0\n";
        std::vector<int> win;
        assert(solveParityGame(input, win) == false);
        assert(win.empty());
    }
    // Test 5: graph where only one SCC has even max, and other SCCs can reach it.
    // 3 vertices: 0->1, 1->2, 2->2 self-loop, priorities 1,3,2. Only 2's SCC good.
    {
        std::string input = "3 3 0\n1\n3\n2\n0 1\n1 2\n2 2\n";
        std::vector<int> win;
        assert(solveParityGame(input, win) == true);
        assert(win == std::vector<int>({0,1,2}));
    }
    // Test 6: multiple edges, ensure duplicates handled.
    {
        std::string input = "2 3 0\n1\n2\n0 1\n1 0\n1 0\n";
        std::vector<int> win;
        assert(solveParityGame(input, win) == true);
        assert(win == std::vector<int>({0,1}));
    }
    // Test 7: single vertex with self-loop even priority.
    {
        std::string input = "1 1 0\n4\n0 0\n";
        std::vector<int> win;
        assert(solveParityGame(input, win) == true);
        assert(win == std::vector<int>({0}));
    }
    // Test 8: single vertex with self-loop odd priority.
    {
        std::string input = "1 1 0\n3\n0 0\n";
        std::vector<int> win;
        assert(solveParityGame(input, win) == false);
        assert(win.empty());
    }
    return 0;
}
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <queue>

// Compute the winning region of a single-player parity game.
// Input format: first line "n m init", then n priority lines, then m edge lines "u v".
// Returns true if init is winning, and fills winningVertices with sorted winning vertex ids.
bool solveParityGame(const std::string& input, std::vector<int>& winningVertices) {
    std::istringstream iss(input);
    int n, m, init;
    iss >> n >> m >> init;

    std::vector<int> priority(n);
    for (int i = 0; i < n; ++i) {
        iss >> priority[i];
    }

    // Build adjacency and reverse adjacency
    std::vector<std::vector<int>> adj(n), radj(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        iss >> u >> v;
        adj[u].push_back(v);
        radj[v].push_back(u);
    }

    // Tarjan's SCC
    std::vector<int> index(n, -1), lowlink(n), scc_id(n, -1);
    std::vector<bool> on_stack(n, false);
    std::vector<int> stack;
    int idx = 0, scc_count = 0;

    std::function<void(int)> strongconnect = [&](int v) {
        index[v] = lowlink[v] = idx++;
        stack.push_back(v);
        on_stack[v] = true;
        for (int w : adj[v]) {
            if (index[w] == -1) {
                strongconnect(w);
                lowlink[v] = std::min(lowlink[v], lowlink[w]);
            } else if (on_stack[w]) {
                lowlink[v] = std::min(lowlink[v], index[w]);
            }
        }
        if (lowlink[v] == index[v]) {
            int w;
            do {
                w = stack.back();
                stack.pop_back();
                on_stack[w] = false;
                scc_id[w] = scc_count;
            } while (w != v);
            scc_count++;
        }
    };

    for (int v = 0; v < n; ++v) {
        if (index[v] == -1) strongconnect(v);
    }

    // For each SCC, compute max priority
    std::vector<int> scc_max_priority(scc_count, -1);
    for (int v = 0; v < n; ++v) {
        int sid = scc_id[v];
        scc_max_priority[sid] = std::max(scc_max_priority[sid], priority[v]);
    }

    // Mark good SCCs (max priority even)
    std::vector<bool> good_scc(scc_count, false);
    for (int s = 0; s < scc_count; ++s) {
        if (scc_max_priority[s] % 2 == 0) good_scc[s] = true;
    }

    // Build SCC condensation graph (reverse) to find vertices that can reach a good SCC
    std::vector<std::vector<int>> scc_radj(scc_count);
    for (int u = 0; u < n; ++u) {
        for (int v : adj[u]) {
            int su = scc_id[u], sv = scc_id[v];
            if (su != sv) {
                scc_radj[su].push_back(sv); // edge from su to sv in original, reverse for reverse reachability
            }
        }
    }

    // BFS on reverse condensation graph from all good SCCs
    std::vector<bool> can_reach_good(scc_count, false);
    std::queue<int> q;
    for (int s = 0; s < scc_count; ++s) {
        if (good_scc[s]) {
            can_reach_good[s] = true;
            q.push(s);
        }
    }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        // In reverse graph, we want edges that go into u in original graph
        // We have scc_radj[su] contains sv if original edge su->sv.
        // To go backward in original, we need from sv to su? Actually we need to traverse original edges backward, so we need reverse adjacency.
        // We built scc_radj as: for each original edge u->v, we push sv into scc_radj[su]. That is forward adjacency in condensation.
        // For reverse reachability, we need the transpose of condensation, so store reverse edges.
        // Let's fix: we should build original condensation adjacency and then BFS on reverse.
        // Simplify: directly BFS on reverse graph of original using radj, but only across SCC boundaries.
        // Better: build condensation forward and reverse.
    }

    // Instead of the above incomplete BFS, let's do a proper reverse BFS on the original graph
    // but only across SCC boundaries? Actually, we want vertices that can reach a good SCC, so we can BFS on reverse original graph (radj) starting from all vertices in good SCCs.
    // But that would include vertices inside good SCCs themselves, which is fine.
    // So simpler: start BFS on radj (reverse edges) from all vertices belonging to good SCCs.
    std::vector<bool> winning(n, false);
    std::queue<int> q2;
    for (int v = 0; v < n; ++v) {
        if (good_scc[scc_id[v]]) {
            winning[v] = true;
            q2.push(v);
        }
    }
    while (!q2.empty()) {
        int u = q2.front(); q2.pop();
        for (int w : radj[u]) {
            if (!winning[w]) {
                winning[w] = true;
                q2.push(w);
            }
        }
    }

    // Collect winning vertices
    winningVertices.clear();
    for (int v = 0; v < n; ++v) {
        if (winning[v]) winningVertices.push_back(v);
    }
    std::sort(winningVertices.begin(), winningVertices.end());

    return winning[init];
}
// The problem is a single-player (system-only) parity game, meaning the player controls all vertices. Since there is only one player, the game is actually a reachability/parity condition on a directed graph with all vertices controlled by the player. The winning condition: an infinite play is winning if the maximum priority visited infinitely often is even. For a single-player game, the winning region can be computed by a fixed-point iteration: a vertex is winning if there exists a path from it that eventually stays within a set where the maximum priority seen infinitely often is even. Since the player chooses all transitions, the player can force winning iff there is a path that reaches a cycle where the maximum priority on the cycle is even, and from that cycle the player never leaves. This reduces to: compute the set of vertices that can reach a "good" cycle (a cycle with maximum priority even) and that cycle is part of a closed set from which the player cannot be forced out (but since it's single-player, the player can just stay in the cycle). However, because the player controls all vertices, the player can choose to stay in a cycle forever if it exists reachable. Therefore, the winning region is simply the set of vertices that can reach at least one cycle whose maximum priority is even. If no such cycle is reachable from a vertex, that vertex is losing. Since the player can always choose to follow a path that eventually loops in such a cycle, reachability to a good cycle is sufficient and necessary. Algorithm: first compute the strongly connected components (SCCs) of the graph. For each SCC, compute its maximum priority. If that maximum is even, the SCC is a "good" SCC. Then, from every vertex, check if there exists a path to a good SCC. This can be done by reversing edges and performing a BFS/DFS from all vertices in good SCCs. All vertices that can reach a good SCC are winning. Complexity: SCC computation O(V+E), reverse BFS O(V+E). Space O(V+E). Edge cases: isolated vertices with no outgoing edges? The problem says each vertex has at least one outgoing edge, but if not, such a vertex would be losing because the play terminates? In standard parity games, infinite play is required; assuming each vertex has a self-loop or at least one outgoing edge to avoid terminal states. Also, if there are no cycles at all (DAG), then no vertex is winning because every play eventually ends? But with one outgoing edge per vertex and finite graph, there must be at least one cycle (pigeonhole principle). So SCCs with size >1 are cycles; single self-loop is size 1. Even a single vertex with a self-loop is a cycle.
