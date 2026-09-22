Given a non-empty 2D matrix of integers, write a C++ function that returns the length of the longest strictly increasing path in the matrix. From each cell, you can move to one of the four adjacent cells (up, down, left, right) only if the value in the next cell is strictly greater than the current cell’s value. The path can start and end at any cell, and you cannot move diagonally. If the matrix has only one row or one column, the function must still work correctly. Return the maximum path length as an integer.
The problem is equivalent to finding the longest path in a directed acyclic graph (DAG) where each cell is a node and a directed edge exists from a cell to an adjacent cell with a larger value. Since each edge goes from a smaller value to a larger value, the graph has no cycles. We build the graph by iterating through every cell and adding edges to its four neighbors when the neighbor is strictly larger. Then we compute the topological order using Kahn’s algorithm: we initialize a queue with all nodes having in-degree zero (i.e., cells with no incoming edges, meaning no smaller neighbor points to them). We process the queue level by level: each level corresponds to one step in the path, so we increment a counter for each level processed. For each node popped, we push all its neighbors into the queue, but to ensure we process levels correctly (and avoid re-adding the same node multiple times in the same level), we can simply push all neighbors each time and use a visited or level-check. However, the simplest correct approach is to simulate a BFS-like level traversal: while the queue is not empty, record the queue size, increment the level counter, and process exactly that many nodes, pushing all their unvisited neighbors (or just all neighbors, but that could cause duplicates). To avoid duplicates and inefficiency, we can reduce the in-degree of each neighbor by 1 and push the neighbor only when its in-degree becomes zero — this is the standard topological sort approach that ensures each node is processed exactly once. The number of levels processed equals the longest path length because we peel off nodes in topological order, and the depth of the last level equals the maximum distance from any source. Edge cases: 1×1 matrix returns 1; all equal values produce no edges, but every node has in-degree 0, so the queue processes all nodes, level becomes 1. Time complexity is O(n*m) for building the graph plus O(n*m) for the topological sort, so O(n*m). Space complexity is O(n*m) for the adjacency list and in-degree array.
#include <vector>
#include <queue>
#include <array>

// Returns the length of the longest strictly increasing path in the matrix.
int longestIncreasingPath(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) return 0;
    const int rows = static_cast<int>(matrix.size());
    const int cols = static_cast<int>(matrix[0].size());
    const int total = rows * cols;

    // Directions: up, down, left, right
    constexpr std::array<int, 4> dx = {1, -1, 0, 0};
    constexpr std::array<int, 4> dy = {0, 0, 1, -1};

    // Build adjacency list and in-degree array
    std::vector<std::vector<int>> adj(total);
    std::vector<int> indegree(total, 0);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            const int node = i * cols + j;
            for (int k = 0; k < 4; ++k) {
                const int ni = i + dx[k];
                const int nj = j + dy[k];
                if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                    if (matrix[ni][nj] > matrix[i][j]) {
                        const int neighbor = ni * cols + nj;
                        adj[node].push_back(neighbor);
                        ++indegree[neighbor];
                    }
                }
            }
        }
    }

    // Kahn's algorithm for topological level BFS
    std::queue<int> q;
    for (int node = 0; node < total; ++node) {
        if (indegree[node] == 0) {
            q.push(node);
        }
    }

    int level = 0;
    while (!q.empty()) {
        const int sz = static_cast<int>(q.size());
        ++level;  // Each full queue layer corresponds to one path step
        for (int i = 0; i < sz; ++i) {
            const int cur = q.front();
            q.pop();
            for (const int next : adj[cur]) {
                if (--indegree[next] == 0) {
                    q.push(next);
                }
            }
        }
    }

    return level;
}
#include <cassert>
#include <vector>

int longestIncreasingPath(const std::vector<std::vector<int>>& matrix);

int main() {
    // Single cell
    assert(longestIncreasingPath({{5}}) == 1);

    // Simple 2x2 strictly increasing
    assert(longestIncreasingPath({{1, 2}, {3, 4}}) == 4);

    // Classic example from LeetCode
    assert(longestIncreasingPath({{9, 9, 4}, {6, 6, 8}, {2, 1, 1}}) == 4);

    // All equal values -> no increasing moves
    assert(longestIncreasingPath({{2, 2}, {2, 2}}) == 1);

    // Single row
    assert(longestIncreasingPath({{1, 3, 5, 2}}) == 3);

    // Single column decreasing and then increasing
    assert(longestIncreasingPath({{5}, {4}, {3}, {6}}) == 2);

    // Zigzag path: (0,0)=1 -> (0,1)=2 -> (1,1)=3 -> (1,0)=4 -> (2,0)=5
    assert(longestIncreasingPath({{1, 2}, {4, 3}, {5, 6}}) == 5);

    // Large steps: diagonal not allowed, but horizontal/vertical chains work
    assert(longestIncreasingPath({{1, 10}, {2, 11}}) == 4);

    // Ensure no crash and correct for 1x1 and small matrices
    assert(longestIncreasingPath({{7}}) == 1);
    return 0;
}
