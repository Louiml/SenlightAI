Given a directed graph represented as an adjacency matrix (vector of vectors of ints, where 1 indicates an edge from row i to column j), write a C++ function that performs a depth-first search (DFS) traversal starting from vertex 0 and continuing with the lowest-index unvisited vertex until all vertices are visited. The function must return a vector of integers containing the vertices in the exact order they are first visited (i.e., the discovery order) during the DFS. The graph may be disconnected, self-loops (diagonal 1s) are allowed, and the matrix is guaranteed square with at least one vertex. The DFS must use a recursive approach with an explicit color array (0 = white/unvisited, 1 = gray/in-progress, 2 = black/finished) and must only traverse edges where the matrix entry is 1. The function should not output anything; it should only build and return the order vector.

The solution uses a standard recursive DFS with three-color marking. For each vertex not yet visited (color 0), we start a DFS visit: mark it gray (color=1), add it to the order vector, then iterate through all possible neighbors (columns of that row). If an edge exists (matrix value 1) and the neighbor is white, we recursively visit it. After exploring all neighbors, we mark the vertex black (color=2). The main function iterates through vertices in increasing index order, so for disconnected components, the next unvisited lowest-index vertex becomes the new root. Self-loops (i==i with matrix[i][i]==1) are harmless because the vertex is already gray when we encounter it, so we skip it due to color check. Since each vertex is visited exactly once, time complexity is O(V^2) because we scan all V neighbors for each of V vertices. Space complexity is O(V) for recursion stack (in worst case O(V) depth) plus O(V) for the color and result vectors.

#include <vector>

// Perform DFS on an adjacency matrix and return vertices in discovery order.
// matrix[r][c] == 1 indicates a directed edge from r to c.
std::vector<int> dfsDiscoveryOrder(const std::vector<std::vector<int>>& matrix) {
    int n = matrix.size();
    std::vector<int> color(n, 0); // 0=white, 1=gray, 2=black
    std::vector<int> order;
    order.reserve(n);

    // Recursive lambda or helper function; we use a lambda with self-reference.
    std::function<void(int)> dfsVisit = [&](int node) {
        color[node] = 1;           // mark gray
        order.push_back(node);     // add to discovery order

        for (int neighbor = 0; neighbor < n; ++neighbor) {
            if (matrix[node][neighbor] == 1 && color[neighbor] == 0) {
                dfsVisit(neighbor);
            }
        }
        color[node] = 2;           // mark black
    };

    // Start from vertex 0, then continue with next unvisited in index order.
    for (int i = 0; i < n; ++i) {
        if (color[i] == 0) {
            dfsVisit(i);
        }
    }

    return order;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Graph from snippet (undirected edges given in matrix, but we treat as directed).
    // Matrix: 0->1,0->2,1->0,1->1,1->3,1->4,2->0,2->2,2->3,3->1,3->2,3->4,4->3
    std::vector<std::vector<int>> g1 = {
        {0,1,1,0,0},
        {1,1,0,1,1},
        {1,0,1,1,0},
        {0,1,1,0,1},
        {0,0,0,1,0}
    };
    // Expected DFS from 0: visits 0, then 1 (neighbor), then 3 (neighbor of 1), then 2 (neighbor of 3), then 4 (neighbor of 1? Actually after 3 finishes, back to 1, then 4). So order: 0,1,3,2,4.
    assert(dfsDiscoveryOrder(g1) == std::vector<int>({0,1,3,2,4}));

    // Test 2: Disconnected graph: vertex 0 isolated, vertices 1 and 2 connected.
    std::vector<std::vector<int>> g2 = {
        {0,0,0},
        {0,0,1},
        {0,1,0}
    };
    // Start from 0 (isolated), then 1, then 2. Order: 0,1,2.
    assert(dfsDiscoveryOrder(g2) == std::vector<int>({0,1,2}));

    // Test 3: Self-loop only.
    std::vector<std::vector<int>> g3 = {{1}};
    assert(dfsDiscoveryOrder(g3) == std::vector<int>({0}));

    // Test 4: Complete directed graph (edges from each to each), but no self-loops.
    std::vector<std::vector<int>> g4 = {
        {0,1,1},
        {1,0,1},
        {1,1,0}
    };
    // From 0: visits 1 (first neighbor), then from 1 visits 2 (first white neighbor). Order: 0,1,2.
    assert(dfsDiscoveryOrder(g4) == std::vector<int>({0,1,2}));

    // Test 5: Chain 0->1->2, each forward.
    std::vector<std::vector<int>> g5 = {
        {0,1,0},
        {0,0,1},
        {0,0,0}
    };
    assert(dfsDiscoveryOrder(g5) == std::vector<int>({0,1,2}));

    // Test 6: Backward direction only.
    std::vector<std::vector<int>> g6 = {
        {0,0,0},
        {1,0,0},
        {1,1,0}
    };
    // From 0: no outgoing edges, so visit 1 next, then from 1 edge to 0 already visited, so visit 2 next. Order: 0,1,2.
    assert(dfsDiscoveryOrder(g6) == std::vector<int>({0,1,2}));

    // Test 7: Single vertex with no edges.
    std::vector<std::vector<int>> g7 = {{0}};
    assert(dfsDiscoveryOrder(g7) == std::vector<int>({0}));

    // Test 8: Two vertices with only edge from 1 to 0.
    std::vector<std::vector<int>> g8 = {
        {0,0},
        {1,0}
    };
    // From 0 isolated, then 1. Order: 0,1.
    assert(dfsDiscoveryOrder(g8) == std::vector<int>({0,1}));

    // Test 9: Larger graph with two components.
    std::vector<std::vector<int>> g9 = {
        {0,1,0,0},
        {0,0,0,0},
        {0,0,0,1},
        {1,0,0,0}
    };
    // From 0: visits 1. Then 2: visits 3. Order: 0,1,2,3.
    assert(dfsDiscoveryOrder(g9) == std::vector<int>({0,1,2,3}));

    // Test 10: Graph with back edge causing revisit attempt but should not add again.
    std::vector<std::vector<int>> g10 = {
        {0,1,0},
        {0,0,1},
        {1,0,0}
    };
    // From 0: visits 1, then 2, then from 2 edge to 0 already gray, so not added. Order: 0,1,2.
    assert(dfsDiscoveryOrder(g10) == std::vector<int>({0,1,2}));

    return 0;
}
