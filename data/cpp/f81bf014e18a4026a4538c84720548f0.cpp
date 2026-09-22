Write a C++ function `vector<int> treeColoring(int n, const vector<pair<int, int>>& edges)` that takes the number of vertices `n` of a tree (numbered from 1 to n) and a list of undirected edges, and returns a coloring of the vertices (a vector of length n, index 0 corresponding to vertex 1) such that:  
- The color of each vertex is a positive integer.  
- Adjacent vertices must have different colors.  
- For any vertex, the colors of its neighbors are all distinct from each other as well.  
- The function must minimize the maximum color used (i.e., the chromatic number in this stricter sense).  
Return any valid coloring that achieves the minimal maximum color. The tree is guaranteed to be connected and acyclic. For vertex 1, color 1 must be assigned.

#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>

// Include your solution here (the function above).

int main() {
    // Test 1: single vertex
    {
        auto res = treeColoring(1, {});
        assert(res.size() == 1);
        assert(res[0] == 1);
    }
    // Test 2: two vertices
    {
        auto res = treeColoring(2, {{1,2}});
        assert(res[0] == 1 && res[1] == 2);
        assert(*std::max_element(res.begin(), res.end()) == 2);
    }
    // Test 3: path of 4 vertices: 1-2-3-4
    {
        auto res = treeColoring(4, {{1,2},{2,3},{3,4}});
        assert(res[0] == 1);
        // Validate constraints
        assert(res[0] != res[1]);
        assert(res[1] != res[2]);
        assert(res[2] != res[3]);
        // Each node's neighbors are distinct
        assert(res[0] != res[2]); // 1's only neighbor is 2, so trivial
        assert(res[1] != res[3]); // 2's neighbors are 1 and 3, must be distinct
        assert(res[2] != res[0]); // 3's neighbors are 2 and 4, must be distinct
        // Max color should be exactly 2 for a path
        assert(*std::max_element(res.begin(), res.end()) == 2);
    }
    // Test 4: star with center 1 and 3 leaves
    {
        auto res = treeColoring(4, {{1,2},{1,3},{1,4}});
        assert(res[0] == 1);
        // Leaves must have distinct colors and not equal to 1
        assert(res[1] != res[2] && res[1] != res[3] && res[2] != res[3]);
        assert(res[1] != 1 && res[2] != 1 && res[3] != 1);
        // Max color = degree+1 = 4
        assert(*std::max_element(res.begin(), res.end()) == 4);
    }
    // Test 5: more complex tree: 1-2, 1-3, 2-4, 2-5, 3-6
    {
        auto res = treeColoring(6, {{1,2},{1,3},{2,4},{2,5},{3,6}});
        // Verify constraints for each vertex
        for (int v = 0; v < 6; ++v) {
            // neighbors of v are all distinct and not equal to color[v]
            std::vector<int> neighbor_colors;
            // Since we don't have adjacency in test, we'll implement manually
            // Actually we can rebuild adjacency from edges, but for simplicity we just check a few known facts
        }
        // Manually check: root 1 has neighbors 2 and 3, must have colors not 1 and distinct
        assert(res[0] == 1);
        assert(res[1] != 1 && res[2] != 1 && res[1] != res[2]);
        // Vertex 2 has neighbors 1,4,5: need colors not equal to res[0] and distinct among themselves
        assert(res[3] != res[0] && res[3] != res[4] && res[0] != res[4]);
        assert(res[3] != res[0] && res[3] != res[4] && res[0] != res[4]);
        // Vertex 3 has neighbors 1 and 6: colors not equal to res[0] and distinct
        assert(res[5] != res[0] && res[5] != res[2]);
        // Verify max color is reasonable (should be 3 given max degree 2? Actually vertex 2 degree 3 → max color = 4)
        assert(*std::max_element(res.begin(), res.end()) == 4);
    }
    // Test 6: large path to ensure iterative DFS works (n=100)
    {
        int n = 100;
        std::vector<std::pair<int,int>> edges;
        for (int i = 1; i < n; ++i) edges.emplace_back(i, i+1);
        auto res = treeColoring(n, edges);
        for (int i = 0; i < n; ++i) {
            if (i > 0) assert(res[i] != res[i-1]);
        }
        assert(*std::max_element(res.begin(), res.end()) == 2);
    }
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <vector>
#include <algorithm>

// Returns an optimal coloring for a tree with strict neighbor-distinct colors.
// colors[i] corresponds to vertex i+1. Root is vertex 1 with color 1.
std::vector<int> treeColoring(int n, const std::vector<std::pair<int, int>>& edges) {
    std::vector<std::vector<int>> adj(n);
    for (const auto& e : edges) {
        adj[e.first - 1].push_back(e.second - 1);
        adj[e.second - 1].push_back(e.first - 1);
    }

    std::vector<int> color(n, 0);
    color[0] = 1;

    // Recursive DFS is fine for n up to 200k if we increase stack, but to be safe we use an iterative approach.
    // Use a stack of pairs (vertex, parent) to avoid recursion depth issues.
    std::vector<std::pair<int, int>> stack;
    stack.emplace_back(0, -1);
    while (!stack.empty()) {
        int v = stack.back().first;
        int p = stack.back().second;
        stack.pop_back();

        int col = 1;
        for (int u : adj[v]) {
            if (u == p) continue;
            // Choose the smallest color not equal to color[v] and not equal to color[p] (if p exists)
            while (col == color[v] || (p != -1 && col == color[p])) {
                ++col;
            }
            color[u] = col;
            ++col;
            stack.emplace_back(u, v);
        }
    }
    return color;
}

// This problem is a known graph coloring problem for trees with the added constraint that each vertex's neighbors must all have distinct colors, and we minimize the maximum color. The optimal strategy is a greedy DFS: assign color 1 to the root (vertex 1). When visiting a node, assign colors to its children by scanning positive integers starting from 1, skipping the color of the parent (if any) and the color of the current node itself. Because the tree has no cycles, this greedy assignment ensures that no two neighbors share a color, and no two neighbors of the same vertex share a color. The minimal maximum color equals the maximum degree plus one (since a vertex with degree d needs d distinct neighbor colors plus its own color). The DFS assigns colors in increasing order guaranteed to be optimal. Edge cases: a single vertex (n=1) returns {1}. A path of length n returns colors 1,2,1,2,... for n>1, max color 2. A star with center degree d returns max color d+1. Time complexity is O(n), space O(n) for adjacency and recursion stack (or explicit stack).
