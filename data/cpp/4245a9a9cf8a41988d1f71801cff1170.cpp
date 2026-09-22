// You are given the number of problem categories \(n\), the number of problem setters \(m\), a capacity \(c_i\) for each category \(i\) (the number of problems that must be assigned to that category), and for each setter \(j\) a list of category indices they are allowed to write for. Each setter can write at most one problem total. Write a C++ function `bool assignProblems(int n, int m, const std::vector<int>& capacities, const std::vector<std::vector<int>>& allowedCategories)` that returns `true` if every category can be filled exactly to its capacity using distinct setters, and `false` otherwise. If it returns `true`, the function must also output (via standard output) a line `1` followed by \(n\) lines, where the \(i\)-th line lists the indices of the setters assigned to category \(i\) (1‑based indices, separated by spaces). If it returns `false`, output only `0`. The problem is a maximum bipartite matching with capacity constraints on the left side, solved by max flow.
#include <bits/stdc++.h>
#include <cassert>

// Declare the solution function
bool assignProblems(int n, int m, const std::vector<int>& capacities,
                    const std::vector<std::vector<int>>& allowedCategories);

int main() {
    // Test 1: Simple feasible case
    // 2 categories, 2 setters, each category needs 1 problem.
    // Setter 1 can do cat 1, setter 2 can do cat 2.
    {
        std::vector<int> caps = {1, 1};
        std::vector<std::vector<int>> allowed = {{1}, {2}};
        assert(assignProblems(2, 2, caps, allowed) == true);
    }

    // Test 2: Infeasible because not enough setters
    {
        std::vector<int> caps = {2, 1};
        std::vector<std::vector<int>> allowed = {{1,2}, {1}};
        assert(assignProblems(2, 2, caps, allowed) == false);
    }

    // Test 3: Category with zero capacity
    {
        std::vector<int> caps = {0, 1};
        std::vector<std::vector<int>> allowed = {{}, {1}};
        assert(assignProblems(2, 1, caps, allowed) == true);
    }

    // Test 4: One setter can handle multiple categories? No, each setter writes at most one.
    // So if a category needs 2 but only one setter is allowed, impossible.
    {
        std::vector<int> caps = {2};
        std::vector<std::vector<int>> allowed = {{1}};
        assert(assignProblems(1, 1, caps, allowed) == false);
    }

    // Test 5: Feasible with one setter per category but multiple allowed categories per setter
    {
        std::vector<int> caps = {1, 1};
        std::vector<std::vector<int>> allowed = {{1,2}, {1,2}};
        assert(assignProblems(2, 2, caps, allowed) == true);
    }

    // Test 6: Larger feasible case from problem statement (first sample)
    {
        std::vector<int> caps = {3, 3, 4};
        std::vector<std::vector<int>> allowed = {
            {1,2}, {3}, {3}, {3}, {3}, {1,2,3}, {2,3}, {1,3}, {2}, {2}, {1,2}, {1,3}, {1,2}, {1}, {1,2,3}
        };
        assert(assignProblems(3, 15, caps, allowed) == true);
    }

    // Test 7: Infeasible case from problem statement (second sample)
    {
        std::vector<int> caps = {7, 3, 4};
        std::vector<std::vector<int>> allowed = {
            {1,2}, {1}, {2}, {2}, {3}, {1,2,3,2,2,3}, {2,3}, {2}, {2}, {2,3}, {2,3}, {1,2}, {1}, {1,2,3}, {}
        };
        assert(assignProblems(3, 15, caps, allowed) == false);
    }

    // Test 8: All capacities zero, any setters
    {
        std::vector<int> caps = {0, 0};
        std::vector<std::vector<int>> allowed = {{}, {}};
        assert(assignProblems(2, 0, caps, allowed) == true);
    }

    return 0;
}
#include <bits/stdc++.h>

// Returns true if a valid assignment exists, false otherwise.
// If true, prints "1" and then n lines (each with setter indices, space‑separated).
// If false, prints "0".
bool assignProblems(int n, int m, const std::vector<int>& capacities,
                    const std::vector<std::vector<int>>& allowedCategories) {
    int totalNodes = n + m + 2; // 0..n+m+1
    int source = 0;
    int sink = n + m + 1;
    std::vector<std::vector<int>> graph(totalNodes, std::vector<int>(totalNodes, 0));

    // Source to categories
    int requiredTotal = 0;
    for (int i = 1; i <= n; ++i) {
        graph[source][i] = capacities[i - 1];
        requiredTotal += capacities[i - 1];
    }

    // Categories to setters (1‑based category, setter node = n + j)
    for (int j = 1; j <= m; ++j) {
        for (int cat : allowedCategories[j - 1]) {
            // cat is 1‑based
            graph[cat][n + j] = 1;
        }
        // Setter to sink
        graph[n + j][sink] = 1;
    }

    // Edmonds‑Karp (BFS augmenting path)
    auto bfs = [&](int s, int t, std::vector<int>& parent) {
        std::vector<bool> visited(totalNodes, false);
        std::queue<int> q;
        q.push(s);
        visited[s] = true;
        parent[s] = -1;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v = 0; v < totalNodes; ++v) {
                if (!visited[v] && graph[u][v] > 0) {
                    parent[v] = u;
                    if (v == t) return true;
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        return false;
    };

    int maxFlow = 0;
    std::vector<int> parent(totalNodes);
    while (bfs(source, sink, parent)) {
        int pathFlow = INT_MAX;
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = std::min(pathFlow, graph[u][v]);
        }
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            graph[u][v] -= pathFlow;
            graph[v][u] += pathFlow;
        }
        maxFlow += pathFlow;
    }

    if (maxFlow != requiredTotal) {
        std::cout << "0\n";
        return false;
    }

    // Output assignment
    std::cout << "1\n";
    for (int i = 1; i <= n; ++i) {
        bool first = true;
        for (int j = 1; j <= m; ++j) {
            // Check if flow exists from setter node back to category i (residual edge)
            if (graph[n + j][i] == 1) {
                if (!first) std::cout << " ";
                std::cout << j;
                first = false;
            }
        }
        std::cout << "\n";
    }
    return true;
}
// Construct a flow network: source node 0, category nodes 1..n, setter nodes n+1..n+m, sink node n+m+1. Add edges from source to each category \(i\) with capacity \(c_i\) (the required number of problems). For each setter \(j\) and each allowed category \(i\), add a directed edge from category \(i\) to setter node \(n+j\) with capacity 1 (since each setter can write at most one problem). Then add edges from each setter node to sink with capacity 1. Compute max flow from source to sink using Ford‑Fulkerson (BFS‑based Edmonds‑Karp). The total required flow is the sum of all capacities; if max flow equals that sum, a valid assignment exists. To reconstruct the assignment, after the flow computation, check residual graph: if the edge from setter node \(n+j\) to category \(i\) has flow (i.e., `graph[n+j][i] == 1`), then setter \(j\) is assigned to category \(i\). Print category lines in order 1..n. Edge cases: capacities sum may exceed number of setters → impossible; a category with zero capacity requires no output line (but still print an empty line? The original prints an empty line even for zero capacity; we follow that). Also multiple setters may be allowed for same category, but each setter is used at most once. Time complexity: O(VE²) worst case for Edmonds‑Karp, but with V ≤ 1105 and E ≤ n·m, it is practical. Space: O(V²) adjacency matrix.
