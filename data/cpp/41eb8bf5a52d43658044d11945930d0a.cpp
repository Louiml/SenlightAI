/*
Write a C++ function `bool canPartitionIntoTwoColors(int n, const std::vector<std::pair<int,int>>& edges, std::vector<int>& colors)` that determines whether an undirected graph with `n` vertices (numbered 1 to `n`) can be colored with exactly two colors (1 and 2) such that no two adjacent vertices share the same color. The function should return `true` if such a coloring exists, and in that case fill the `colors` vector (indexed 0 to `n-1`) with the assigned color (1 or 2) for each vertex. If no valid coloring exists, return `false` and leave the `colors` vector unchanged. The graph may be disconnected, may contain isolated vertices, and may have parallel edges or self-loops (self-loops make coloring impossible). The input edges are given as 1-based vertex pairs.
*/
#include <vector>
#include <stack>
#include <cassert>

/**
 * Determines if the graph is bipartite (two-colorable) and fills colors.
 * Vertices are numbered 1..n. Colors are 1 and 2.
 * Returns true if possible and updates colors (0-indexed). Otherwise returns false.
 */
bool canPartitionIntoTwoColors(int n, const std::vector<std::pair<int,int>>& edges, std::vector<int>& colors) {
    // Build adjacency list (0-indexed)
    std::vector<std::vector<int>> adj(n);
    for (const auto& [u, v] : edges) {
        // Self-loop directly makes coloring impossible
        if (u == v) return false;
        adj[u-1].push_back(v-1);
        adj[v-1].push_back(u-1);
    }
    
    // Colors: 0 = unassigned, 1 and 2 are colors
    std::vector<int> col(n, 0);
    std::vector<bool> visited(n, false);
    
    // Iterative DFS over all components
    std::stack<int> stk;
    for (int start = 0; start < n; ++start) {
        if (visited[start]) continue;
        stk.push(start);
        visited[start] = true;
        col[start] = 1;  // assign first color
        
        while (!stk.empty()) {
            int u = stk.top();
            stk.pop();
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    col[v] = (col[u] == 1) ? 2 : 1;
                    stk.push(v);
                } else {
                    if (col[v] == col[u]) {
                        return false; // conflict
                    }
                }
            }
        }
    }
    
    // Final verification over all edges
    for (const auto& [u, v] : edges) {
        if (col[u-1] == col[v-1]) return false;
    }
    
    colors = col; // copy result
    return true;
}
#include <cassert>
#include <vector>
#include <iostream>

int main() {
    // Disconnected bipartite graph: two edges, no common vertices
    std::vector<int> colors1;
    assert(canPartitionIntoTwoColors(4, {{1,2}, {3,4}}, colors1) == true);
    assert(colors1[0] == 1 && colors1[1] == 2 && colors1[2] == 1 && colors1[3] == 2);
    
    // Odd cycle (not bipartite)
    std::vector<int> colors2;
    assert(canPartitionIntoTwoColors(3, {{1,2}, {2,3}, {3,1}}, colors2) == false);
    
    // Even cycle (bipartite)
    std::vector<int> colors3;
    assert(canPartitionIntoTwoColors(4, {{1,2}, {2,3}, {3,4}, {4,1}}, colors3) == true);
    assert(colors3[0] == 1 && colors3[1] == 2 && colors3[2] == 1 && colors3[3] == 2);
    
    // Isolated vertices and a single edge
    std::vector<int> colors4;
    assert(canPartitionIntoTwoColors(5, {{2,5}}, colors4) == true);
    assert(colors4[1] != colors4[4]);
    
    // Self-loop (impossible)
    std::vector<int> colors5;
    assert(canPartitionIntoTwoColors(2, {{1,1}}, colors5) == false);
    
    // Parallel edges (should still be valid)
    std::vector<int> colors6;
    assert(canPartitionIntoTwoColors(2, {{1,2}, {2,1}}, colors6) == true);
    assert(colors6[0] != colors6[1]);
    
    // Empty graph with n=3
    std::vector<int> colors7;
    assert(canPartitionIntoTwoColors(3, {}, colors7) == true);
    assert(colors7[0] == 1 && colors7[1] == 1 && colors7[2] == 1);
    
    // Disconnected with an edge inside component, plus an isolated vertex
    std::vector<int> colors8;
    assert(canPartitionIntoTwoColors(6, {{1,2}, {2,3}}, colors8) == true);
    assert(colors8[0] != colors8[1] && colors8[1] != colors8[2] && colors8[0] == colors8[2]);
    
    // Two disjoint odd cycles (both non-bipartite)
    std::vector<int> colors9;
    assert(canPartitionIntoTwoColors(6, {{1,2}, {2,3}, {3,1}, {4,5}, {5,6}, {6,4}}, colors9) == false);
    
    // Triangle plus an isolated vertex (non-bipartite)
    std::vector<int> colors10;
    assert(canPartitionIntoTwoColors(4, {{1,2}, {2,3}, {3,1}}, colors10) == false);
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The problem is a classic bipartiteness check, solvable via graph traversal (DFS or BFS) with two-coloring. Since the graph may be disconnected, we must iterate over all vertices and perform a traversal from each unvisited vertex, assigning alternating colors (e.g., 1 then 2) along edges. A conflict occurs if an edge connects two vertices with the same assigned color. Self-loops always cause a conflict because a vertex would be adjacent to itself, requiring a color different from itself (impossible). Parallel edges do not affect correctness; they just repeat the same adjacency check. After traversal, we must verify every edge: if any edge has endpoints with equal colors, return `false`. The algorithm runs in \(O(n + m)\) time, where \(m\) is the number of edges, because each vertex and edge is processed once. Auxiliary space is \(O(n)\) for the adjacency list and visited array, excluding the output vector.
//
// Implementation note: Build an adjacency list from the edges (converting 1-based to 0-based). Use an iterative or recursive DFS. To avoid recursion depth issues, an iterative stack is safer. For each unvisited vertex, push it with an initial color; pop, assign color, and push neighbors with opposite color if unvisited. After processing all vertices, iterate through all edges and check colors. If any conflict, return `false`.
