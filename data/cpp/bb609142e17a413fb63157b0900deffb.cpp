You are given an undirected, unweighted graph represented as an 8×8 adjacency matrix `A`, where the vertices are numbered 1 through 7 (row/column index 0 is unused and always zero). The graph has 7 vertices and edges as defined in the provided snippet (vertex 1 connects to 2,3,4; vertex 2 to 1,3; vertex 3 to 1,2,4,5; vertex 4 to 1,3,5; vertex 5 to 3,4,6,7; vertex 6 to 5; vertex 7 to 5). Write a C++ function `std::vector<int> bfsTraversal(int startVertex, const std::array<std::array<int, 8>, 8>& adjMatrix)` that performs a Breadth‑First Search (BFS) starting from `startVertex` (which will be between 1 and 7 inclusive) and returns the order in which vertices are visited. The order must follow the standard BFS rule: visit the starting vertex first, then process vertices in the order they were discovered (queue order), and for each vertex, consider its neighbors in increasing index order (1 through 7). The function must not use any global or static state (besides the constant matrix passed in) and must be reusable for multiple calls with different start vertices. The returned vector should contain exactly the visited vertex numbers (integers from 1 to 7, no duplicates). If the graph is connected (which it is here), the vector will have length 7. Ensure your function is `const` correct (the adjacency matrix is passed as a const reference). Do not include a `main` function in your solution; your code will be tested separately.
#include <array>
#include <cassert>
#include <vector>

// The solution function declaration (as above).
std::vector<int> bfsTraversal(int startVertex, const std::array<std::array<int, 8>, 8>& adjMatrix);

int main() {
    // Adjacency matrix from the problem statement (0 index unused, 1..7 vertices).
    const std::array<std::array<int, 8>, 8> A = {{
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 0, 0, 0},
        {0, 1, 0, 1, 0, 0, 0, 0},
        {0, 1, 1, 0, 1, 1, 0, 0},
        {0, 1, 0, 1, 0, 1, 0, 0},
        {0, 0, 0, 1, 1, 0, 1, 1},
        {0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0}
    }};

    // Test starting from vertex 1.
    std::vector<int> bfs1 = bfsTraversal(1, A);
    assert(bfs1 == std::vector<int>({1, 2, 3, 4, 5, 6, 7}));

    // Test starting from vertex 4.
    std::vector<int> bfs4 = bfsTraversal(4, A);
    assert(bfs4 == std::vector<int>({4, 1, 3, 5, 2, 6, 7}));

    // Test starting from vertex 7.
    std::vector<int> bfs7 = bfsTraversal(7, A);
    assert(bfs7 == std::vector<int>({7, 5, 3, 4, 6, 1, 2}));

    // Test starting from vertex 2 (already partially covered, but check length and uniqueness).
    std::vector<int> bfs2 = bfsTraversal(2, A);
    assert(bfs2.size() == 7);

    // Verify all visited vertices are exactly the set {1..7}.
    std::vector<int> sortedBfs2 = bfs2;
    std::sort(sortedBfs2.begin(), sortedBfs2.end());
    assert(sortedBfs2 == std::vector<int>({1, 2, 3, 4, 5, 6, 7}));

    return 0;
}
#include <array>
#include <queue>
#include <vector>

// Perform BFS on a fixed 8x8 adjacency matrix starting from startVertex.
// Returns the order of visited vertices (1-indexed).
std::vector<int> bfsTraversal(int startVertex, const std::array<std::array<int, 8>, 8>& adjMatrix) {
    std::vector<int> result;
    std::vector<int> visited(8, 0);
    std::queue<int> q;

    visited[startVertex] = 1;
    result.push_back(startVertex);
    q.push(startVertex);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 1; v < 8; ++v) {
            if (adjMatrix[u][v] == 1 && visited[v] == 0) {
                visited[v] = 1;
                result.push_back(v);
                q.push(v);
            }
        }
    }

    return result;
}
// The solution uses a standard BFS approach with a queue. The algorithm:  
// 1. Initialize a vector `visited` of size 8 (indices 0–7) all set to 0, and a queue `q`.  
// 2. Mark `startVertex` as visited, push it into the queue, and add it to the result vector.  
// 3. While the queue is not empty, pop the front vertex `u`.  
// 4. Iterate `v` from 1 to 7 inclusive. For each neighbor `v` such that `adjMatrix[u][v] == 1` and `visited[v] == 0`, mark `v` as visited, push `v` into the queue, and append `v` to the result.  
// 5. Continue until the queue is empty. Since the graph is connected, all 7 vertices will be visited.  
//
// Edge cases:  
// - The start vertex is always valid (1–7).  
// - The adjacency matrix has zeros on the diagonal and in row/column 0, so those are ignored naturally.  
// - Because we process neighbors in increasing index order, the BFS order is deterministic.  
// - If the graph were disconnected (not the case here), the function would return only the reachable component; the task guarantees connectedness but the function is still correct for any input matrix.  
//
// Time complexity: O(V + E) = O(8 + 7) ≈ O(1) constant time for this fixed size, but in general for an adjacency matrix it is O(V^2) because for each vertex we scan all V columns. Here V = 8, so it is essentially constant. Space complexity: O(V) for the visited array and queue, plus O(V) for the result vector.
