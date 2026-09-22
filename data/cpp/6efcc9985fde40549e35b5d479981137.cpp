/*
Write a C++ function `std::vector<std::vector<int>> findSCCs(int n, const std::vector<std::pair<int,int>>& edges)` that takes a directed graph with `n` vertices (numbered 1 through `n`) and a list of directed edges, and returns a vector of vectors representing the strongly connected components (SCCs) of the graph. Each inner vector contains the vertices of one SCC (in any order), and the order of the inner vectors themselves is arbitrary. The function must handle graphs with up to 100,000 vertices and 200,000 edges efficiently, must work for cycles, self-loops, and disconnected graphs, and must not modify the input. Return an empty vector if `n` is 0 or negative. The function should be self-contained, use only standard library components, and be callable from external code.
*/

#include <vector>
#include <utility>
#include <algorithm>

// Returns strongly connected components of a directed graph with vertices 1..n.
std::vector<std::vector<int>> findSCCs(int n, const std::vector<std::pair<int,int>>& edges) {
    if (n <= 0) return {};
    
    // Build adjacency lists: 1-indexed, so size n+1.
    std::vector<std::vector<int>> adj(n + 1);
    std::vector<std::vector<int>> radj(n + 1);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        radj[e.second].push_back(e.first);
    }
    
    std::vector<char> visited(n + 1, 0);
    std::vector<int> order; // finishing order from first DFS
    
    // First pass: DFS on original graph, record post-order.
    // Use iterative DFS to avoid recursion depth issues.
    for (int start = 1; start <= n; ++start) {
        if (visited[start]) continue;
        std::vector<int> stack = {start};
        std::vector<int> path; // to mimic recursion and post-order
        visited[start] = 1;
        while (!stack.empty()) {
            int v = stack.back();
            if (!path.empty() && path.back() == v) {
                // all children processed
                stack.pop_back();
                path.pop_back();
                order.push_back(v);
                continue;
            }
            bool has_unvisited = false;
            for (int next : adj[v]) {
                if (!visited[next]) {
                    visited[next] = 1;
                    stack.push_back(next);
                    path.push_back(v);
                    has_unvisited = true;
                    break;
                }
            }
            if (!has_unvisited) {
                // no unvisited children, finish v
                stack.pop_back();
                order.push_back(v);
            }
        }
    }
    
    // Reset visited for second pass.
    std::fill(visited.begin(), visited.end(), 0);
    std::vector<std::vector<int>> components;
    
    // Second pass: process vertices in reverse finishing order on reversed graph.
    for (int idx = n - 1; idx >= 0; --idx) {
        int start = order[idx];
        if (visited[start]) continue;
        std::vector<int> comp;
        std::vector<int> stack = {start};
        visited[start] = 1;
        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();
            comp.push_back(v);
            for (int next : radj[v]) {
                if (!visited[next]) {
                    visited[next] = 1;
                    stack.push_back(next);
                }
            }
        }
        components.push_back(std::move(comp));
    }
    
    return components;
}

#include <cassert>
#include <vector>
#include <utility>
#include <algorithm>

// Include the solution function here (or link to it).

int main() {
    // Test 1: Simple 3-cycle plus an extra edge.
    std::vector<std::pair<int,int>> edges1 = {{1,2},{2,3},{3,1},{3,4}};
    auto res1 = findSCCs(4, edges1);
    // Expect two SCCs: one containing {1,2,3} and one containing {4}.
    assert(res1.size() == 2);
    int count = 0;
    for (auto& comp : res1) {
        std::sort(comp.begin(), comp.end());
        if (comp.size() == 3 && comp[0]==1 && comp[1]==2 && comp[2]==3) count++;
        if (comp.size() == 1 && comp[0]==4) count++;
    }
    assert(count == 2);

    // Test 2: Single vertex with self-loop.
    std::vector<std::pair<int,int>> edges2 = {{1,1}};
    auto res2 = findSCCs(1, edges2);
    assert(res2.size() == 1);
    assert(res2[0].size() == 1 && res2[0][0] == 1);

    // Test 3: Disconnected graph.
    std::vector<std::pair<int,int>> edges3 = {{1,2},{3,3}};
    auto res3 = findSCCs(4, edges3);
    // SCCs: {1,2}, {3}, {4} => 3 components.
    assert(res3.size() == 3);
    int sum3 = 0;
    for (auto& comp : res3) sum3 += comp.size();
    assert(sum3 == 4);

    // Test 4: DAG (no cycles) with 5 vertices: each vertex is its own SCC.
    std::vector<std::pair<int,int>> edges4 = {{1,2},{2,3},{3,4},{4,5}};
    auto res4 = findSCCs(5, edges4);
    assert(res4.size() == 5);
    for (auto& comp : res4) assert(comp.size() == 1);

    // Test 5: Empty graph with n=3 (no edges).
    std::vector<std::pair<int,int>> edges5 = {};
    auto res5 = findSCCs(3, edges5);
    assert(res5.size() == 3);
    for (auto& comp : res5) assert(comp.size() == 1);

    // Test 6: n = 0 returns empty vector.
    auto res6 = findSCCs(0, {});
    assert(res6.empty());

    // Test 7: Larger graph with two separate cycles.
    std::vector<std::pair<int,int>> edges7 = {{1,2},{2,1},{3,4},{4,3}};
    auto res7 = findSCCs(4, edges7);
    assert(res7.size() == 2);
    int sum7 = 0;
    for (auto& comp : res7) {
        assert(comp.size() == 2);
        sum7 += comp.size();
    }
    assert(sum7 == 4);

    return 0;
}

// The problem asks for strongly connected components of a directed graph. The classic algorithm is Kosaraju’s two-pass DFS. First, we build both the original adjacency list and the reverse adjacency list. In the first pass, we perform DFS on the original graph, recording vertices in post-order (when a vertex’s DFS finishes, we append it to a stack). This yields an ordering such that if we process vertices in reverse of this order on the reversed graph, each DFS will discover exactly one SCC. In the second pass, we iterate through the stack in reverse order, and for each unvisited vertex, we perform DFS on the reversed graph, collecting all reachable vertices into a new component. Important edge cases: (1) vertices with no edges form a singleton SCC; (2) self-loops are handled naturally because the vertex reaches itself; (3) disconnected components are handled because each unvisited start in the second pass begins a new component. Complexity: Each edge and vertex is processed a constant number of times in both DFS passes, so time is O(n + m) and auxiliary space is O(n + m) for adjacency lists (beyond the input). We must ensure the function does not rely on global variables and uses local data structures. Use `vector<vector<int>>` for adjacency, a `vector<bool>` or `vector<char>` for visited state, and a `vector<int>` for the finishing order stack.
