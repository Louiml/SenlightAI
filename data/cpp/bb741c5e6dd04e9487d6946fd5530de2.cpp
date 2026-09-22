Write a C++ function `int computeMSTWeight(const vector<vector<int>>& adjMatrix)` that takes a symmetric square matrix representing an undirected weighted graph (where `adjMatrix[i][j] > 0` indicates an edge with that weight between vertices `i` and `j`, and `0` indicates no edge) and returns the total weight of the Minimum Spanning Tree (MST) using Prim's algorithm starting from vertex 0. The graph is guaranteed to be connected, and edge weights are positive integers. If the graph has only one vertex, the MST weight is 0.

// The solution uses Prim's algorithm with a simple array-based approach (since the task focuses on understanding the algorithm rather than optimizing for large graphs). Initialize three arrays: `inMST` (boolean) to track which vertices are already added, `key` (integer) to store the minimum edge weight connecting each vertex to the current MST, and `parent` (integer) to record the MST structure (though not needed for weight computation). Start by setting `key[0] = 0` and all others to `INT_MAX`. Then, in each of the `V` iterations, select the vertex `u` not yet in the MST with the smallest `key` value, add it to the MST, add `key[u]` to the total weight, and update the `key` values of all its neighbors `v` not in the MST: if `adjMatrix[u][v] > 0` and `adjMatrix[u][v] < key[v]`, set `key[v] = adjMatrix[u][v]`. Edge cases: a single-vertex graph returns 0; since the graph is connected, each iteration will find a valid vertex (no need to check for disconnected components). Time complexity is \(O(V^2)\) due to the nested loop for finding the minimum key and updating neighbors; space complexity is \(O(V)\) for the auxiliary arrays.

#include <vector>
#include <climits>

// Compute the total weight of the Minimum Spanning Tree using Prim's algorithm.
// The input adjMatrix is symmetric, with adjMatrix[i][j] > 0 representing an edge.
// The graph is guaranteed to be connected and undirected.
int computeMSTWeight(const std::vector<std::vector<int>>& adjMatrix) {
    int numVertices = adjMatrix.size();
    if (numVertices == 0) return 0;

    std::vector<bool> inMST(numVertices, false);
    std::vector<int> key(numVertices, INT_MAX);
    key[0] = 0;  // Start from vertex 0

    int totalWeight = 0;

    for (int count = 0; count < numVertices; ++count) {
        // Find the vertex with the smallest key that is not yet in the MST
        int u = -1;
        for (int v = 0; v < numVertices; ++v) {
            if (!inMST[v] && (u == -1 || key[v] < key[u])) {
                u = v;
            }
        }

        inMST[u] = true;
        totalWeight += key[u];

        // Update key values for neighbors of u
        for (int v = 0; v < numVertices; ++v) {
            if (adjMatrix[u][v] > 0 && !inMST[v] && adjMatrix[u][v] < key[v]) {
                key[v] = adjMatrix[u][v];
            }
        }
    }

    return totalWeight;
}

#include <cassert>
#include <vector>

// The solution function is declared here (or included from the header).
int computeMSTWeight(const std::vector<std::vector<int>>& adjMatrix);

int main() {
    // Test 1: Single vertex -> MST weight = 0
    std::vector<std::vector<int>> g1 = {{0}};
    assert(computeMSTWeight(g1) == 0);

    // Test 2: Two vertices with edge weight 5
    std::vector<std::vector<int>> g2 = {{0, 5}, {5, 0}};
    assert(computeMSTWeight(g2) == 5);

    // Test 3: Triangle with weights 1, 2, 3 -> MST should pick 1 and 2 = 3
    std::vector<std::vector<int>> g3 = {
        {0, 1, 2},
        {1, 0, 3},
        {2, 3, 0}
    };
    assert(computeMSTWeight(g3) == 3);

    // Test 4: Square (cycle of 4) with weights: edges 0-1=4, 1-2=2, 2-3=3, 3-0=1, and diagonal 0-2=10, 1-3=10
    // MST: choose 3-0 (1), 1-2 (2), 2-3 (3) [or 0-3, 1-2, and 0-1? Let's compute: edges available
    // Actually a square with diagonals is complete graph. For vertices 0,1,2,3:
    // Edges: 0-1=4, 1-2=2, 2-3=3, 3-0=1, 0-2=10, 1-3=10.
    // MST picks 3-0(1), 1-2(2), and 2-3(3) = 6. Or 3-0, 0-1, 1-2 = 4+2+1=7. So best is 1+2+3=6.
    std::vector<std::vector<int>> g4 = {
        {0, 4, 10, 1},
        {4, 0, 2, 10},
        {10, 2, 0, 3},
        {1, 10, 3, 0}
    };
    assert(computeMSTWeight(g4) == 6);

    // Test 5: Line graph: 0-1=7, 1-2=3, 2-3=1
    std::vector<std::vector<int>> g5 = {
        {0, 7, 0, 0},
        {7, 0, 3, 0},
        {0, 3, 0, 1},
        {0, 0, 1, 0}
    };
    assert(computeMSTWeight(g5) == 11); // 7+3+1

    // Test 6: Complete graph with all weights equal 5 (V=4) -> MST weight = 5*(4-1) = 15
    std::vector<std::vector<int>> g6 = {
        {0,5,5,5},
        {5,0,5,5},
        {5,5,0,5},
        {5,5,5,0}
    };
    assert(computeMSTWeight(g6) == 15);

    // Test 7: Star graph: center 0 connected to 1,2,3 with weights 2,4,6
    std::vector<std::vector<int>> g7 = {
        {0,2,4,6},
        {2,0,0,0},
        {4,0,0,0},
        {6,0,0,0}
    };
    assert(computeMSTWeight(g7) == 12);

    return 0;
}
