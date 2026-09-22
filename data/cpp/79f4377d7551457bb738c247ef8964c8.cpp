Write a C++ function `std::vector<int> topologicalOrder(const std::vector<std::vector<int>>& adj, int vertices)` that takes an adjacency list representation of a directed acyclic graph (DAG) and returns the topological ordering of its vertices as a vector of vertex indices. The graph has vertices numbered from `0` to `vertices-1`. The function should return a valid topological order (i.e., for every directed edge u→v, u appears before v in the result). If the graph contains a cycle, the function should return an empty vector. Ensure the implementation uses an explicit stack or recursion that pushes vertices after visiting all their neighbors (standard DFS-based approach) and handles disconnected components. The function must be `const`-correct for the adjacency list parameter. Do not modify the input graph.
// The solution uses a depth-first search (DFS) based topological sorting algorithm. The main idea is to perform DFS on every unvisited vertex. For each vertex, we recursively visit all its unvisited neighbors. After all neighbors of a vertex have been fully processed, we push that vertex onto a stack (or prepend to a list). This ensures that when we later pop from the stack, a vertex appears only after all vertices reachable from it, guaranteeing that for each edge u→v, u is placed before v. We maintain a visited array to avoid revisiting nodes. To detect cycles, we use an additional recursion stack (or a color marking: 0=unvisited, 1=in current recursion, 2=done). If we encounter a vertex that is currently in the recursion stack, a cycle exists, and we return an empty vector. Time complexity is O(V + E) since each vertex and edge is processed once. Space complexity is O(V) for the visited/recursion arrays and the recursion call stack (or explicit stack). Edge cases include: empty graph (return empty), single vertex (return that vertex), disconnected graph (process each component separately), and cyclic graph (return empty).
#include <vector>
#include <stack>

std::vector<int> topologicalOrder(const std::vector<std::vector<int>>& adj, int vertices) {
    // Color: 0 = unvisited, 1 = visiting (in current recursion), 2 = fully processed
    std::vector<int> color(vertices, 0);
    std::vector<int> result;
    std::stack<int> order;
    bool hasCycle = false;
    
    // DFS helper using recursion
    std::function<void(int)> dfs = [&](int u) {
        color[u] = 1; // mark as visiting
        for (int v : adj[u]) {
            if (color[v] == 0) {
                dfs(v);
                if (hasCycle) return;
            } else if (color[v] == 1) {
                hasCycle = true;
                return;
            }
        }
        color[u] = 2; // fully processed
        order.push(u);
    };
    
    for (int i = 0; i < vertices && !hasCycle; ++i) {
        if (color[i] == 0) {
            dfs(i);
        }
    }
    
    if (hasCycle) {
        return {};
    }
    
    while (!order.empty()) {
        result.push_back(order.top());
        order.pop();
    }
    return result;
}
#include <cassert>
#include <vector>
#include <functional>
#include <stack>

// Include the solution function here (or assume it's above)
// ...

int main() {
    // Test 1: Simple linear graph 0->1->2
    std::vector<std::vector<int>> adj1 = {{1}, {2}, {}};
    std::vector<int> res1 = topologicalOrder(adj1, 3);
    assert(res1 == std::vector<int>({0, 1, 2}));

    // Test 2: Disconnected graph: 0->1, 2 isolated, 3->4
    std::vector<std::vector<int>> adj2 = {{1}, {}, {}, {4}, {}};
    std::vector<int> res2 = topologicalOrder(adj2, 5);
    // Valid orders exist; check first two and last two relative positions
    assert(res2.size() == 5);
    int pos0 = -1, pos1 = -1, pos2 = -1, pos3 = -1, pos4 = -1;
    for (int i = 0; i < 5; ++i) {
        if (res2[i] == 0) pos0 = i;
        if (res2[i] == 1) pos1 = i;
        if (res2[i] == 2) pos2 = i;
        if (res2[i] == 3) pos3 = i;
        if (res2[i] == 4) pos4 = i;
    }
    assert(pos0 < pos1);
    assert(pos3 < pos4);
    // All vertices present exactly once
    std::vector<int> sorted_res = res2;
    std::sort(sorted_res.begin(), sorted_res.end());
    assert(sorted_res == std::vector<int>({0, 1, 2, 3, 4}));

    // Test 3: Cycle detection
    std::vector<std::vector<int>> adj3 = {{1}, {2}, {0}};
    assert(topologicalOrder(adj3, 3).empty());

    // Test 4: Single vertex no edges
    std::vector<std::vector<int>> adj4 = {{}};
    assert(topologicalOrder(adj4, 1) == std::vector<int>({0}));

    // Test 5: Empty graph (no vertices)
    std::vector<std::vector<int>> adj5 = {};
    assert(topologicalOrder(adj5, 0).empty());

    // Test 6: DAG with multiple edges and branching
    std::vector<std::vector<int>> adj6 = {{1, 2}, {3}, {3}, {}};
    std::vector<int> res6 = topologicalOrder(adj6, 4);
    assert(res6.size() == 4);
    int p0, p1, p2, p3;
    for (int i = 0; i < 4; ++i) {
        if (res6[i] == 0) p0 = i;
        if (res6[i] == 1) p1 = i;
        if (res6[i] == 2) p2 = i;
        if (res6[i] == 3) p3 = i;
    }
    assert(p0 < p1 && p0 < p2 && p1 < p3 && p2 < p3);

    return 0;
}
