/*
Write a C++ function `bool isKColorable(const vector<vector<int>>& adjacencyMatrix, int k, vector<int>& coloring, vector<pair<int,int>>& exploredNodes)` that determines whether an undirected graph, represented by an `n x n` adjacency matrix (1-indexed, with `n` vertices), can be colored using at most `k` colors (colors numbered from 1 to `k`) such that no two adjacent vertices share the same color. The graph is simple (no self-loops, at most one edge between any two vertices), and the matrix is symmetric. The function must use recursive backtracking in the same style as the given snippet: at each step, assign a color to the next vertex, check validity with previously assigned neighbors, and if a valid assignment is found, recurse to the next vertex. The function should also record, in the vector `exploredNodes`, every call `(vertexIndex, colorAttempted)` in the order they are tried, and if a valid `k`-coloring exists, it should fill `coloring[1..n]` with the assigned colors (index 0 unused) and return `true`; otherwise return `false` and leave `coloring` unchanged. Handle the edge case where `k==0` or `n==0` by returning `false` immediately. The function must be `const`-correct and use only standard library headers.
*/

#include <vector>
#include <utility>

// Helper: check if assigning color c to vertex v is valid with respect to already colored neighbors (those with smaller index).
bool isValidColor(const std::vector<std::vector<int>>& adj, const std::vector<int>& color, int v, int c) {
    for (int u = 1; u < v; ++u) {
        if (adj[v][u] != 0 && color[u] == c) {
            return false;
        }
    }
    return true;
}

// Recursive backtracking function. Returns true if a valid coloring exists for vertices from v to n.
bool backtrack(const std::vector<std::vector<int>>& adj, int n, int k, int v, std::vector<int>& color, std::vector<std::pair<int,int>>& explored) {
    for (int c = 1; c <= k; ++c) {
        explored.emplace_back(v, c);
        if (isValidColor(adj, color, v, c)) {
            color[v] = c;
            if (v == n) {
                return true;
            }
            if (backtrack(adj, n, k, v + 1, color, explored)) {
                return true;
            }
            // Backtrack: uncolor current vertex
            color[v] = 0;
        }
    }
    return false;
}

// Main function: determine if graph is k-colorable and record attempts.
// adjacencyMatrix is 1-indexed (size (n+1)x(n+1)).
// coloring is expected to be a vector of size n+1, index 0 unused.
// exploredNodes will be filled with all (vertex, color) attempts in order.
bool isKColorable(const std::vector<std::vector<int>>& adjacencyMatrix, int k, std::vector<int>& coloring, std::vector<std::pair<int,int>>& exploredNodes) {
    int n = adjacencyMatrix.size() - 1; // number of vertices (since we use 1-indexing)
    if (n == 0 || k == 0) {
        return false;
    }
    coloring.assign(n + 1, 0); // 0 means uncolored
    exploredNodes.clear();
    return backtrack(adjacencyMatrix, n, k, 1, coloring, exploredNodes);
}

#include <cassert>
#include <vector>
#include <utility>

// Assume isKColorable is defined above.

int main() {
    // Test 1: Triangle graph (3 vertices, all connected) - needs 3 colors.
    {
        std::vector<std::vector<int>> adj = {
            {0,0,0,0},
            {0,0,1,1},
            {0,1,0,1},
            {0,1,1,0}
        };
        std::vector<int> coloring;
        std::vector<std::pair<int,int>> explored;
        assert(isKColorable(adj, 2, coloring, explored) == false);
        assert(coloring[0] == 0 && coloring[1] == 0 && coloring[2] == 0 && coloring[3] == 0); // unchanged on failure
    }
    // Test 2: Same triangle with k=3 - should succeed and color all three differently.
    {
        std::vector<std::vector<int>> adj = {
            {0,0,0,0},
            {0,0,1,1},
            {0,1,0,1},
            {0,1,1,0}
        };
        std::vector<int> coloring;
        std::vector<std::pair<int,int>> explored;
        assert(isKColorable(adj, 3, coloring, explored) == true);
        // Check that no adjacent vertices share the same color
        assert(coloring[1] != coloring[2] && coloring[1] != coloring[3] && coloring[2] != coloring[3]);
        // Check that explored contains at least some entries
        assert(!explored.empty());
    }
    // Test 3: Single vertex with k=1 - trivially colorable.
    {
        std::vector<std::vector<int>> adj = {
            {0,0},
            {0,0}
        };
        std::vector<int> coloring;
        std::vector<std::pair<int,int>> explored;
        assert(isKColorable(adj, 1, coloring, explored) == true);
        assert(coloring[1] == 1);
    }
    // Test 4: No vertices (n=0) - should fail immediately.
    {
        std::vector<std::vector<int>> adj = {{0}};
        std::vector<int> coloring;
        std::vector<std::pair<int,int>> explored;
        assert(isKColorable(adj, 1, coloring, explored) == false);
    }
    // Test 5: k=0 - should fail regardless of graph.
    {
        std::vector<std::vector<int>> adj = {
            {0,0,0},
            {0,0,1},
            {0,1,0}
        };
        std::vector<int> coloring;
        std::vector<std::pair<int,int>> explored;
        assert(isKColorable(adj, 0, coloring, explored) == false);
    }
    // Test 6: Disconnected graph (two separate edges) - k=2 should be possible.
    {
        // Vertices 1-2 connected, 3-4 connected, no cross edges.
        std::vector<std::vector<int>> adj = {
            {0,0,0,0,0},
            {0,0,1,0,0},
            {0,1,0,0,0},
            {0,0,0,0,1},
            {0,0,0,1,0}
        };
        std::vector<int> coloring;
        std::vector<std::pair<int,int>> explored;
        assert(isKColorable(adj, 2, coloring, explored) == true);
        assert(coloring[1] != coloring[2]);
        assert(coloring[3] != coloring[4]);
    }
    // Test 7: Empty graph (no edges) with n=4, k=1 - all can be same color.
    {
        std::vector<std::vector<int>> adj = {
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,0,0,0,0}
        };
        std::vector<int> coloring;
        std::vector<std::pair<int,int>> explored;
        assert(isKColorable(adj, 1, coloring, explored) == true);
        for (int i = 1; i <= 4; ++i) assert(coloring[i] == 1);
    }
    // Test 8: Check explored list order and content for a simple chain (1-2-3) with k=1 (impossible).
    {
        std::vector<std::vector<int>> adj = {
            {0,0,0,0},
            {0,0,1,0},
            {0,1,0,1},
            {0,0,1,0}
        };
        std::vector<int> coloring;
        std::vector<std::pair<int,int>> explored;
        assert(isKColorable(adj, 1, coloring, explored) == false);
        // Expect attempts: (1,1), (2,1) fails because neighbor 1 has color 1, (3,1) fails because neighbor 2 has color 1.
        // Because we record before validity check, we still record all (v,1) attempts.
        assert(explored.size() == 3);
        assert(explored[0] == std::make_pair(1,1));
        assert(explored[1] == std::make_pair(2,1));
        assert(explored[2] == std::make_pair(3,1));
    }

    return 0;
}

// The solution follows the exact backtracking pattern from the provided snippet. We start at vertex 1 and attempt each color from 1 to `k`. For each attempt, we call `isValid` to check whether any previously colored neighbor (vertices with index less than the current) already uses that color; if valid, we assign it and if it’s the last vertex, we succeed. Otherwise, we recursively try to color the next vertex with all possible colors. If none of the recursive calls return `true`, we backtrack by resetting the current vertex's color to 0 (uncolored) and try the next color. We record every `(vertex, color)` attempt in `exploredNodes`. Key edge cases: if `k==0` or `n==0`, no coloring is possible; if the graph is disconnected, the algorithm still works because it colors vertices in index order. The time complexity is `O(k^n)` in the worst case (exponential), but with pruning via validity checks it is typically much faster on sparse graphs. Space complexity is `O(n)` for the recursive call stack plus `O(n)` for the coloring array and `O(n)` for the explored list (which stores at most `n*k` entries, so `O(nk)` in the worst case). The function is implemented recursively with a helper that performs the depth-first search.
