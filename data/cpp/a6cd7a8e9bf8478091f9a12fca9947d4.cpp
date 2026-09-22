Given an undirected tree with `n` vertices (1-indexed in input, but stored 0-indexed internally) and a list of its `n-1` edges, write a C++ function that colors the tree in the following way: start at vertex 0 (the root) with color `false` (representing 0), and for every edge traversed away from the root, flip the color. Then, for each vertex from 0 to n-1, compute and return a vector of integers where the value at position `i` equals `(color[i] ? 1 : -1) * degree(i)`, with `degree(i)` being the number of neighbors of vertex `i`. The function should take the number of vertices `n` and a vector of pairs (edges) as input, where each pair contains two 1-indexed integers `u` and `v` denoting an edge. The function must handle all trees correctly, including the trivial case `n = 1` (where no edges exist), and it should be efficient for large `n`.
#include <cassert>
#include <vector>

// The solution function is declared above (signedDegreeFromTree).

int main() {
    // Test 1: single vertex
    {
        std::vector<int> res = signedDegreeFromTree(1, {});
        assert(res.size() == 1);
        assert(res[0] == 0);
    }
    
    // Test 2: simple edge between 1 and 2
    {
        std::vector<int> res = signedDegreeFromTree(2, {{1,2}});
        assert(res.size() == 2);
        // root (0) color=false -> -degree = -1
        // vertex 1 color=true -> +degree = +1
        assert(res[0] == -1);
        assert(res[1] == 1);
    }
    
    // Test 3: star with center vertex 1 and leaves 2,3,4
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{1,2}, {1,3}, {1,4}};
        std::vector<int> res = signedDegreeFromTree(n, edges);
        // vertex 0 (center) degree=3, color=false -> -3
        // each leaf degree=1, color=true -> +1
        std::vector<int> expected = {-3, 1, 1, 1};
        assert(res == expected);
    }
    
    // Test 4: chain of 5 vertices: 1-2-3-4-5
    {
        int n = 5;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,5}};
        std::vector<int> res = signedDegreeFromTree(n, edges);
        // depth: 0(-),1(+),2(-),3(+),4(-)
        // degrees: 1,2,2,2,1
        // signs: -,+,-,+,- -> [-1, +2, -2, +2, -1]
        std::vector<int> expected = {-1, 2, -2, 2, -1};
        assert(res == expected);
    }
    
    // Test 5: more complex tree (binary-like)
    // vertices: 1-2, 1-3, 2-4, 2-5
    {
        int n = 5;
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{2,4},{2,5}};
        std::vector<int> res = signedDegreeFromTree(n, edges);
        // root 0: degree=2 -> -2
        // vertex 1: degree=3 (neighbors 0,3,4) -> +3
        // vertex 2: degree=1 -> -1
        // vertex 3: degree=1 -> +1
        // vertex 4: degree=1 -> +1
        std::vector<int> expected = {-2, 3, -1, 1, 1};
        assert(res == expected);
    }
    
    // Test 6: 6-vertex tree where root has degree 1
    // edges: 1-2, 1-3, 3-4, 3-5, 5-6
    {
        int n = 6;
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{3,4},{3,5},{5,6}};
        std::vector<int> res = signedDegreeFromTree(n, edges);
        // depth: 0(-) degree=2 -> -2
        // 1(+) degree=2 -> +2
        // 2(-) degree=1 -> -1
        // 3(+) degree=3 -> +3
        // 4(-) degree=2 -> -2
        // 5(+) degree=1 -> +1
        std::vector<int> expected = {-2, 2, -1, 3, -2, 1};
        assert(res == expected);
    }
    
    return 0;
}
#include <vector>
#include <functional>

// Given a tree with n vertices (numbered 1..n in edges) and a list of edges,
// return a vector of length n where element i (0-indexed) equals (color[i] ? 1 : -1) * degree[i].
// color[0] = false (0), and colors alternate along edges.
std::vector<int> signedDegreeFromTree(int n, const std::vector<std::pair<int,int>>& edges) {
    std::vector<std::vector<int>> adj(n);
    for (const auto& [u, v] : edges) {
        --u; --v;  // convert to 0-indexed
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    std::vector<bool> color(n, false);
    
    std::function<void(int,int)> dfs = [&](int u, int p) {
        for (int v : adj[u]) {
            if (v != p) {
                color[v] = !color[u];
                dfs(v, u);
            }
        }
    };
    
    dfs(0, -1);
    
    std::vector<int> result(n);
    for (int i = 0; i < n; ++i) {
        int deg = static_cast<int>(adj[i].size());
        result[i] = (color[i] ? 1 : -1) * deg;
    }
    return result;
}
// The problem is a classic two-coloring of a tree using a DFS or BFS from an arbitrary root (here vertex 0). Since the graph is a tree, it has no cycles, so a simple depth-first traversal assigning alternating colors guarantees that every adjacent pair of vertices has different colors. The color flips on each edge traversal, so starting from the root with color `false`, all vertices at even depth get `false` and odd depth get `true`. The degree of each vertex is simply the size of its adjacency list. For each vertex, compute the signed degree: if color is `true`, output positive degree; otherwise output negative degree.  
//
// Edge cases:  
// - `n = 1`: The adjacency list is empty, degree is 0, color is `false`, so output `[0]` (since 0 * -1 = 0).  
// - Any tree: Works fine because there are no cycles, so the coloring is well-defined regardless of traversal order.  
// - The root's parent is set to -1 to avoid revisiting.  
//
// Time complexity: O(n) because we visit each vertex once and each edge twice (once from each endpoint) during DFS. Space complexity: O(n) for the adjacency list and the color/vector storage, plus the recursion stack in the worst case (a chain) O(n).
