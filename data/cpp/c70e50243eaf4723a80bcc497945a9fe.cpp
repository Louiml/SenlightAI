Given an undirected graph with n vertices and m edges, each vertex i has a value p[i] in its current permutation and a target value q[i] in the desired permutation. Write a C++ function `bool canPermuteConnectedComponents(int n, const std::vector<int>& p, const std::vector<int>& q, const std::vector<std::pair<int,int>>& edges)` that returns `true` if, within each connected component of the graph, it is possible to permute the values of p among the vertices of that component (by swapping values along edges arbitrarily) so that for every vertex i in the component, p[i] becomes equal to q[i]. Essentially, the function checks whether, for every connected component, the multiset of p-values in that component matches the multiset of q-values in that component. The graph may be disconnected, and vertices are 0-indexed in the input vectors but edges are provided 1-indexed, so you must convert them appropriately. The function should handle empty graphs, isolated vertices, and multiple edges or self-loops gracefully.
#include <cassert>
#include <vector>
#include <utility>
#include <iostream>

// Assume the solution function is declared above (or include the header)
// For test, copy the solution function here or include the file.

int main() {
    // Test 1: Simple connected component with swap
    {
        int n = 2;
        std::vector<int> p = {1, 2};
        std::vector<int> q = {2, 1};
        std::vector<std::pair<int,int>> edges = {{1,2}};
        assert(canPermuteConnectedComponents(n, p, q, edges) == true);
    }
    // Test 2: Disconnected components - one fails
    {
        int n = 4;
        std::vector<int> p = {1, 2, 3, 4};
        std::vector<int> q = {1, 3, 2, 4};
        // edges connect 1-2 and 3-4? Actually test component 1 (0,1) p={1,2}, q={1,3} fails
        std::vector<std::pair<int,int>> edges = {{1,2}, {3,4}};
        assert(canPermuteConnectedComponents(n, p, q, edges) == false);
    }
    // Test 3: All isolated vertices
    {
        int n = 3;
        std::vector<int> p = {5, 6, 7};
        std::vector<int> q = {5, 6, 7};
        std::vector<std::pair<int,int>> edges = {};
        assert(canPermuteConnectedComponents(n, p, q, edges) == true);
    }
    // Test 4: Isolated vertex mismatch
    {
        int n = 3;
        std::vector<int> p = {5, 6, 7};
        std::vector<int> q = {5, 8, 7};
        std::vector<std::pair<int,int>> edges = {{1,2}}; // only connects 0 and 1, vertex 2 isolated
        assert(canPermuteConnectedComponents(n, p, q, edges) == false);
    }
    // Test 5: Duplicate values in same component
    {
        int n = 3;
        std::vector<int> p = {1, 1, 2};
        std::vector<int> q = {1, 2, 1};
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,1}};
        assert(canPermuteConnectedComponents(n, p, q, edges) == true);
    }
    // Test 6: Self-loop and multi-edge
    {
        int n = 2;
        std::vector<int> p = {1, 2};
        std::vector<int> q = {2, 1};
        std::vector<std::pair<int,int>> edges = {{1,1},{1,2},{2,1}};
        assert(canPermuteConnectedComponents(n, p, q, edges) == true);
    }
    // Test 7: Large component with reversed order
    {
        int n = 5;
        std::vector<int> p = {10, 20, 30, 40, 50};
        std::vector<int> q = {50, 40, 30, 20, 10};
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,5},{5,1}};
        assert(canPermuteConnectedComponents(n, p, q, edges) == true);
    }
    // Test 8: Component fails because p has extra value not in q
    {
        int n = 2;
        std::vector<int> p = {1, 2};
        std::vector<int> q = {1, 3};
        std::vector<std::pair<int,int>> edges = {{1,2}};
        assert(canPermuteConnectedComponents(n, p, q, edges) == false);
    }
    // Test 9: Empty graph n=0
    {
        int n = 0;
        std::vector<int> p, q;
        std::vector<std::pair<int,int>> edges;
        assert(canPermuteConnectedComponents(n, p, q, edges) == true);
    }
    // Test 10: Graph with two components both match
    {
        int n = 4;
        std::vector<int> p = {1, 2, 3, 4};
        std::vector<int> q = {2, 1, 4, 3};
        std::vector<std::pair<int,int>> edges = {{1,2}, {3,4}};
        assert(canPermuteConnectedComponents(n, p, q, edges) == true);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>

// Check if we can permute p values within each connected component to match q.
bool canPermuteConnectedComponents(int n,
                                   const std::vector<int>& p,
                                   const std::vector<int>& q,
                                   const std::vector<std::pair<int,int>>& edges) {
    // Build adjacency list (0-indexed)
    std::vector<std::list<int>> adj(n);
    for (const auto& e : edges) {
        // Convert 1-indexed to 0-indexed
        int u = e.first - 1;
        int v = e.second - 1;
        if (u >= 0 && u < n && v >= 0 && v < n) { // defensive check
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }

    std::vector<bool> visited(n, false);

    for (int start = 0; start < n; ++start) {
        if (!visited[start]) {
            // Collect all vertices in this connected component via DFS (iterative stack)
            std::vector<int> component;
            std::vector<int> stack;
            stack.push_back(start);
            visited[start] = true;
            while (!stack.empty()) {
                int u = stack.back();
                stack.pop_back();
                component.push_back(u);
                for (int neighbor : adj[u]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        stack.push_back(neighbor);
                    }
                }
            }

            // Compare frequency of p and q in this component
            std::unordered_map<int, int> countP;
            std::unordered_map<int, int> countQ;
            for (int vertex : component) {
                countP[p[vertex]]++;
                countQ[q[vertex]]++;
            }

            // If any frequency differs, return false
            for (const auto& kv : countP) {
                if (countQ[kv.first] != kv.second) {
                    return false;
                }
            }
            // Also ensure no extra values in q not present in p (counts already match)
            for (const auto& kv : countQ) {
                if (countP[kv.first] != kv.second) {
                    return false;
                }
            }
        }
    }
    return true;
}
// The problem reduces to checking, for each connected component, whether the set (or multiset) of values from p and q restricted to vertices in that component are identical. Since swapping values along edges allows any permutation within a connected component (because we can move any value to any vertex in the same component via a path), the necessary and sufficient condition is that the multiset of p[i] equals the multiset of q[i] for every connected component. The algorithm: (1) Build adjacency list from the edge list (converting 1-indexed edges to 0-indexed). (2) Perform DFS or BFS from every unvisited vertex to collect all vertices in the current component. (3) For each component, compare the multiset of p-values and q-values. A simple way is to use a `std::multiset` or sort both lists and compare, but using an `unordered_map` counting frequencies gives O(component size) on average. If any component fails, return false. Edge cases: isolated vertices (a component of size 1) — check p[i]==q[i]. Duplicate values are fine because we compare multisets. Self-loops or multiple edges don't affect the component structure. Time complexity: O(n + m) for graph traversal plus O(n) for frequency comparisons, so overall O(n + m) on average with hash maps (or O(n log n) if using sorting). Space: O(n) for adjacency list and visited array, plus O(n) for frequency maps.
