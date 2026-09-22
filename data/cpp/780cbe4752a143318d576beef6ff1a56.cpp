// Write a C++ function named `findRootSCCNodes` that takes as input a directed graph with vertices numbered from `0` to `N-1`, represented by an adjacency list (`vector<vector<int>>`), and an integer `N` for the number of vertices. The function should return a `vector<int>` containing all vertices in the unique source strongly connected component (SCC), i.e., the SCC that has no incoming edges from any other SCC. If there is more than one source SCC, return an empty vector. The graph may have up to 100,000 vertices and up to 100,000 edges. The function must use Tarjan's SCC algorithm to compute the SCCs, then determine indegree between SCCs, and finally return the sorted list of vertices in the unique source SCC, or an empty vector if the source is not unique.

// The problem reduces to finding strongly connected components (SCCs) of a directed graph. Tarjan's algorithm performs a single DFS, assigning each vertex a discovery rank and a low-link value. Upon finishing a node where low-link equals its rank, we pop all nodes from a stack that form one SCC, mark them with a component ID, and store the component's vertex list (sorted for output consistency). After all SCCs are identified, we compute the indegree of each component by iterating over every edge; for each edge from vertex `u` to `v`, if `u` and `v` belong to different components, increment the indegree of the component of `v`. A source SCC is one with indegree zero. We count how many such components exist. If exactly one exists, we return its sorted vertices; otherwise we return an empty vector. Edge cases: a graph with a single SCC (fully connected or isolated vertices) will have exactly one source SCC, so return all vertices. A graph with no edges but multiple vertices yields each vertex as its own SCC, and every vertex is a source, so return empty. Complexity: Tarjan's algorithm runs in O(V + E) time, and the indegree computation also O(V + E); total O(V + E). Space is O(V + E) for the graph and O(V) for auxiliary arrays and the stack.

#include <vector>
#include <algorithm>
#include <stack>
#include <cstddef>

std::vector<int> findRootSCCNodes(const std::vector<std::vector<int>>& graph, int N) {
    int time = 0;
    int component_count = 0;
    std::vector<int> discovery(N, 0);
    std::vector<int> low(N, 0);
    std::vector<int> component_id(N, -1);
    std::vector<int> indegree;
    std::vector<std::vector<int>> components;
    std::stack<int> dfs_stack;
    std::vector<bool> on_stack(N, false);
    
    // Tarjan's DFS
    std::function<void(int)> tarjan = [&](int u) {
        discovery[u] = low[u] = ++time;
        dfs_stack.push(u);
        on_stack[u] = true;
        
        for (int v : graph[u]) {
            if (discovery[v] == 0) {
                tarjan(v);
                low[u] = std::min(low[u], low[v]);
            } else if (on_stack[v]) {
                low[u] = std::min(low[u], discovery[v]);
            }
        }
        
        if (low[u] == discovery[u]) {
            std::vector<int> comp;
            while (true) {
                int v = dfs_stack.top();
                dfs_stack.pop();
                on_stack[v] = false;
                component_id[v] = component_count;
                comp.push_back(v);
                if (v == u) break;
            }
            std::sort(comp.begin(), comp.end());
            components.push_back(comp);
            component_count++;
        }
    };
    
    for (int i = 0; i < N; ++i) {
        if (discovery[i] == 0) {
            tarjan(i);
        }
    }
    
    indegree.assign(component_count, 0);
    for (int u = 0; u < N; ++u) {
        for (int v : graph[u]) {
            if (component_id[u] != component_id[v]) {
                indegree[component_id[v]]++;
            }
        }
    }
    
    std::vector<int> sources;
    for (int cid = 0; cid < component_count; ++cid) {
        if (indegree[cid] == 0) {
            sources.push_back(cid);
        }
    }
    
    if (sources.size() == 1) {
        return components[sources[0]];
    }
    return {};
}

#include <cassert>
#include <vector>

// The function from the solution is assumed available.
// Add the function implementation here or include the header.

int main() {
    // Test 1: Two SCCs, chain 0->1, source is {0}
    std::vector<std::vector<int>> g1 = {{1}, {}};
    auto r1 = findRootSCCNodes(g1, 2);
    assert((r1 == std::vector<int>{0}));
    
    // Test 2: Two source SCCs, should return empty
    std::vector<std::vector<int>> g2 = {{}, {}};
    auto r2 = findRootSCCNodes(g2, 2);
    assert(r2.empty());
    
    // Test 3: Single SCC (cycle), source is all vertices
    std::vector<std::vector<int>> g3 = {{1}, {2}, {0}};
    auto r3 = findRootSCCNodes(g3, 3);
    assert((r3 == std::vector<int>{0, 1, 2}));
    
    // Test 4: Graph with edges between SCCs: 0->1, 1->2, 2->1, 0->3, 3 isolated. SCCs: {0}, {1,2}, {3}. Sources: 0 and 3 -> empty
    std::vector<std::vector<int>> g4 = {{1, 3}, {2}, {1}, {}};
    auto r4 = findRootSCCNodes(g4, 4);
    assert(r4.empty());
    
    // Test 5: DAG with one source SCC: 0->1->2, plus edge 0->2. Source=0
    std::vector<std::vector<int>> g5 = {{1, 2}, {2}, {}};
    auto r5 = findRootSCCNodes(g5, 3);
    assert((r5 == std::vector<int>{0}));
    
    // Test 6: Single vertex no edges
    std::vector<std::vector<int>> g6 = {{}};
    auto r6 = findRootSCCNodes(g6, 1);
    assert((r6 == std::vector<int>{0}));
    
    // Test 7: Two disjoint cycles, each source => empty
    std::vector<std::vector<int>> g7 = {{1}, {0}, {3}, {2}};
    auto r7 = findRootSCCNodes(g7, 4);
    assert(r7.empty());
    
    // Test 8: Chain with 3 singletons: 0->1->2, sources=0
    std::vector<std::vector<int>> g8 = {{1}, {2}, {}};
    auto r8 = findRootSCCNodes(g8, 3);
    assert((r8 == std::vector<int>{0}));
    
    // Test 9: Mixed: 0 and 1 both point to 2, which points to 3. Sources: 0 and1 -> empty
    std::vector<std::vector<int>> g9 = {{2}, {2}, {3}, {}};
    auto r9 = findRootSCCNodes(g9, 4);
    assert(r9.empty());
    
    // Test 10: Self loop only, single SCC
    std::vector<std::vector<int>> g10 = {{0}};
    auto r10 = findRootSCCNodes(g10, 1);
    assert((r10 == std::vector<int>{0}));
    
    return 0;
}
