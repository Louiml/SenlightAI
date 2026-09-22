// Write a C++ function `bool graphIsColorableWithThreeColors(const std::vector<std::pair<int,int>>& edges, int vertexCount)` that determines whether an undirected graph with `vertexCount` vertices (numbered 0 to vertexCount-1) can be properly colored using at most 3 colors, where proper means no two adjacent vertices share the same color. The graph is simple (no self-loops, no duplicate edges), but may be disconnected. The function should return `true` if such a coloring exists, and `false` otherwise. The input edges are given as pairs of vertex indices. You may assume `vertexCount >= 0` and all vertex indices in edges are within `[0, vertexCount-1]`.

#include <cassert>
#include <vector>
#include <utility>

// Declaration of the solution function (include the solution code above in the same file)
bool graphIsColorableWithThreeColors(const std::vector<std::pair<int,int>>& edges, int vertexCount);

int main() {
    // Empty graph with 5 vertices -> trivially colorable
    assert(graphIsColorableWithThreeColors({}, 5) == true);

    // Single vertex -> colorable
    assert(graphIsColorableWithThreeColors({}, 1) == true);

    // Triangle (K3) -> colorable with 3 colors
    assert(graphIsColorableWithThreeColors({{0,1},{1,2},{2,0}}, 3) == true);

    // K4 (complete graph on 4 vertices) -> not 3-colorable
    assert(graphIsColorableWithThreeColors({{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}}, 4) == false);

    // Path of length 4 -> colorable
    assert(graphIsColorableWithThreeColors({{0,1},{1,2},{2,3}}, 4) == true);

    // Cycle of length 5 -> colorable with 3 colors
    assert(graphIsColorableWithThreeColors({{0,1},{1,2},{2,3},{3,4},{4,0}}, 5) == true);

    // Disconnected: a triangle plus an isolated vertex -> colorable
    assert(graphIsColorableWithThreeColors({{0,1},{1,2},{2,0}}, 4) == true);

    // Two disjoint triangles -> colorable (each 3-colorable)
    assert(graphIsColorableWithThreeColors({{0,1},{1,2},{2,0},{3,4},{4,5},{5,3}}, 6) == true);

    // Odd cycle of length 7 -> colorable
    assert(graphIsColorableWithThreeColors({{0,1},{1,2},{2,3},{3,4},{4,5},{5,6},{6,0}}, 7) == true);

    // Petersen graph (10 vertices) is not 3-colorable (chromatic number 3? Actually Petersen is 3-colorable, but a known non-3-colorable small graph is the Grötzsch graph. Let's test a graph with a subgraph K4: K4 plus an extra vertex connected to all four)
    std::vector<std::pair<int,int>> edges = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3},{0,4},{1,4},{2,4},{3,4}};
    // This is K5? Actually added vertex 4 connected to 0-3, but not all to each other? It includes K4 and a vertex adjacent to all four, making it K5 (since 4 is also adjacent to 0,1,2,3) but not edge (4,anything else)? Actually K5 needs all edges among 5 vertices; here we have all edges among 0-3 and edges from 4 to all 0-3, so it is K5, which requires 5 colors → not 3-colorable
    assert(graphIsColorableWithThreeColors(edges, 5) == false);

    return 0;
}

#include <vector>
#include <array>

// Determine whether an undirected graph can be properly colored with at most 3 colors.
// edges: list of undirected edges as pairs of vertex indices.
// vertexCount: number of vertices, indices 0..vertexCount-1.
// Returns true if a 3-coloring exists.
bool graphIsColorableWithThreeColors(const std::vector<std::pair<int,int>>& edges, int vertexCount) {
    // Build adjacency list
    std::vector<std::vector<int>> adj(vertexCount);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    // colors[i] will store the assigned color (0,1,2) for vertex i, -1 means uncolored
    std::vector<int> colors(vertexCount, -1);

    // Recursive backtracking function
    // Returns true if a valid coloring can be completed starting from vertex pos.
    // We assume vertices before pos are already colored.
    std::function<bool(int)> backtrack = [&](int pos) -> bool {
        if (pos == vertexCount) {
            // All vertices colored successfully
            return true;
        }

        // Try each of the three colors
        for (int color = 0; color < 3; ++color) {
            bool valid = true;
            // Check all neighbors already colored
            for (int neighbor : adj[pos]) {
                if (colors[neighbor] == color) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                colors[pos] = color;
                if (backtrack(pos + 1)) {
                    return true;
                }
                // backtrack: reset color
                colors[pos] = -1;
            }
        }
        return false;
    };

    // Start the search from vertex 0
    return backtrack(0);
}

// The problem is equivalent to checking whether the graph's chromatic number is at most 3, which is NP-complete in general, but for small vertex counts (as in a typical exercise) a backtracking search works perfectly. The algorithm: create an adjacency list from the edge list. Then assign colors (0,1,2) to vertices one by one using depth-first search with backtracking. For each vertex, try each of the three colors; a color is valid if no already-colored neighbor has that color. If a valid coloring is found, return `true`. If all possibilities are exhausted for a vertex, backtrack and try a different color for a previously placed vertex. Important edge cases: (1) empty graph (no edges) is always colorable (return true immediately), (2) graph with a single vertex is trivially colorable, (3) a graph that is a complete graph on 4 vertices (K4) is not 3-colorable and the algorithm will correctly return `false` after exploring all assignments, (4) disconnected components are handled automatically because we iterate over vertices in order, and unconstrained components can be colored greedily when reached. Time complexity: worst-case O(3^V) due to backtracking, but with pruning (skipping invalid colors) it is much faster in practice; space complexity O(V + E) for adjacency list and O(V) for the color array and recursion stack.
