/*
Write a C++ function `stronglyConnectedComponents(const std::vector<std::vector<int>>& graph, std::vector<int>& componentIndex)` that computes the strongly connected components (SCCs) of a directed graph with vertices numbered from 0 to n-1. The function should return the number of SCCs as an integer, and fill the `componentIndex` vector (of size n) such that for each vertex `i`, `componentIndex[i]` contains the 1-based identifier of its SCC, where components are numbered in reverse topological order of the condensation graph (i.e., the order in which they are completed during Tarjan’s algorithm). The graph is given as an adjacency list; it may contain self-loops and parallel edges. If the graph is empty (n=0), return 0 and leave `componentIndex` empty. The function must be self-contained, using only standard library headers, and must not have side effects on the input graph.
*/

#include <vector>
#include <stack>
#include <algorithm>

// Computes strongly connected components of a directed graph.
// graph: adjacency list, vertices 0..n-1.
// componentIndex: output vector of size n, filled with 1-based SCC IDs.
// Returns the number of SCCs.
int stronglyConnectedComponents(const std::vector<std::vector<int>>& graph,
                                std::vector<int>& componentIndex) {
    const int n = graph.size();
    componentIndex.assign(n, 0);
    if (n == 0) return 0;

    std::vector<int> dfn(n, 0), low(n, 0);
    std::vector<bool> inStack(n, false);
    std::stack<int> stack;
    int timer = 0;
    int componentCount = 0;

    // Recursive lambda for Tarjan's DFS.
    std::function<void(int)> dfs = [&](int x) {
        dfn[x] = low[x] = ++timer;
        stack.push(x);
        inStack[x] = true;

        for (int y : graph[x]) {
            if (!dfn[y]) {
                dfs(y);
                low[x] = std::min(low[x], low[y]);
            } else if (inStack[y]) {
                low[x] = std::min(low[x], dfn[y]);
            }
        }

        if (low[x] == dfn[x]) {
            ++componentCount;
            while (true) {
                int y = stack.top();
                stack.pop();
                inStack[y] = false;
                componentIndex[y] = componentCount;
                if (y == x) break;
            }
        }
    };

    for (int i = 0; i < n; ++i) {
        if (!dfn[i]) dfs(i);
    }

    return componentCount;
}

#include <cassert>
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>

// Include the solution function here (or link to it).
int stronglyConnectedComponents(const std::vector<std::vector<int>>& graph,
                                std::vector<int>& componentIndex);

int main() {
    // Test 1: Empty graph.
    std::vector<std::vector<int>> g0;
    std::vector<int> comp0;
    int cnt0 = stronglyConnectedComponents(g0, comp0);
    assert(cnt0 == 0);
    assert(comp0.empty());

    // Test 2: Single vertex with self-loop.
    std::vector<std::vector<int>> g1 = {{0}};
    std::vector<int> comp1;
    int cnt1 = stronglyConnectedComponents(g1, comp1);
    assert(cnt1 == 1);
    assert(comp1 == std::vector<int>({1}));

    // Test 3: Disconnected vertices.
    std::vector<std::vector<int>> g2 = {{}, {}, {}};
    std::vector<int> comp2;
    int cnt2 = stronglyConnectedComponents(g2, comp2);
    assert(cnt2 == 3);
    // Each vertex in its own component, IDs in order 1,2,3.
    assert(comp2 == std::vector<int>({1, 2, 3}));

    // Test 4: Simple cycle 0->1->2->0.
    std::vector<std::vector<int>> g3 = {{1}, {2}, {0}};
    std::vector<int> comp3;
    int cnt3 = stronglyConnectedComponents(g3, comp3);
    assert(cnt3 == 1);
    // All vertices get the same ID.
    assert(comp3[0] == comp3[1] && comp3[1] == comp3[2]);
    assert(comp3[0] == 1);

    // Test 5: Two SCCs: 0->1, 1->0, and 2->1 (2 only reaches the first SCC).
    std::vector<std::vector<int>> g4 = {{1}, {0}, {1}};
    std::vector<int> comp4;
    int cnt4 = stronglyConnectedComponents(g4, comp4);
    assert(cnt4 == 2);
    // 0 and 1 are in same SCC, 2 is in the other SCC.
    assert(comp4[0] == comp4[1]);
    assert(comp4[2] != comp4[0]);
    // Reverse topological order: root of 0-1 SCC is completed first, so it gets ID 1.
    assert(comp4[0] == 1);
    assert(comp4[2] == 2);

    // Test 6: Chain 0->1->2 with extra parallel edges.
    std::vector<std::vector<int>> g5 = {{1, 1}, {2}, {}};
    std::vector<int> comp5;
    int cnt5 = stronglyConnectedComponents(g5, comp5);
    assert(cnt5 == 3);
    assert(comp5[0] == 1 && comp5[1] == 2 && comp5[2] == 3);

    // Test 7: DAG with two sources and two sinks, verify IDs increase along edges.
    std::vector<std::vector<int>> g6 = {{1, 2}, {3}, {3}, {}};
    std::vector<int> comp6;
    int cnt6 = stronglyConnectedComponents(g6, comp6);
    assert(cnt6 == 4);
    // Verify that for each edge (u,v), comp[u] < comp[v]? No: reverse topological order means comp[u] > comp[v] for edges? Actually we assign IDs in completion order, so sinks get lower IDs. Let's check manually:
    // Starting from 0: DFS 0->1->3, complete 3 first -> ID1; then 1 -> ID2; back to 0, go to 2->3 (already visited), complete 2 -> ID3; then 0 -> ID4.
    assert(comp6[0] == 4);
    assert(comp6[1] == 2);
    assert(comp6[2] == 3);
    assert(comp6[3] == 1);

    std::cout << "All tests passed.\n";
    return 0;
}

// The solution uses Tarjan’s strongly connected components algorithm, which performs a single depth-first search (DFS) over the graph. For each unvisited vertex, we run a DFS, assigning a discovery time `dfn` and a low-link value `low`. The `low` of a vertex is the minimum discovery time reachable from it via its subtree and at most one back edge to an ancestor that is still on the stack. When we finish processing a vertex and find `low[x] == dfn[x]`, that vertex is the root of an SCC: we pop vertices from a stack until we pop `x`, assigning them all the same component ID. The component IDs are assigned in increasing order as each SCC is completed, which naturally gives a reverse topological order of the condensation DAG (since a component is assigned only after all its descendants in the DFS tree have been assigned). The algorithm handles disconnected graphs by looping over all vertices. Edge cases include empty graphs, self-loops (which trivially put a vertex in its own SCC), and parallel edges (which do not affect correctness). The time complexity is O(V + E) because each vertex and edge is processed once, and the auxiliary space is O(V) for the stacks, discovery/low arrays, and the on-stack flag.
