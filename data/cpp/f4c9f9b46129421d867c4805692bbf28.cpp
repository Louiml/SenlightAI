Write a C++ function named `graphTraversal` that accepts an undirected graph represented as an adjacency list (a `std::vector<std::vector<int>>`), a starting vertex `start`, and returns a `std::string` containing the BFS traversal order followed by a newline and then the DFS traversal order, each as space-separated integers. The graph is guaranteed to be connected and have vertices numbered from 0 to `n-1` where `n` is the size of the adjacency list. The traversal must start from the given `start` vertex and visit neighbors in the order they appear in each vertex's adjacency list (i.e., ascending order if the input is sorted). The returned string should have exactly one space between consecutive numbers and no trailing spaces, with the two traversal lines separated by `'\n'`. Your function should handle graphs where vertices may have self-loops or multiple edges, and must correctly reset visited flags between BFS and DFS.
// The solution requires two standard graph traversals: BFS uses a queue, DFS uses an explicit stack (to avoid recursion depth issues). We first initialize a boolean vector `visited` of size `n` to `false`. For BFS, push the start vertex, mark visited, and process the queue: pop a vertex, append its value to a result string, then for each neighbor in the adjacency list order, if not visited, mark and push. After BFS completes, we reset `visited` to all `false` and repeat a similar process for DFS using a stack. Edge cases: self-loops are handled because we check `visited` before pushing; multiple edges cause repeated neighbor checks but the visited flag ensures no duplicates in output. Since the graph is connected, all vertices are reachable from `start`. Time complexity is `O(V+E)` for each traversal, so `O(V+E)` overall; space complexity is `O(V)` for visited and the queue/stack.
#include <string>
#include <vector>
#include <queue>
#include <stack>

// Perform BFS and DFS traversals on an undirected graph represented as an adjacency list.
// Returns a string with BFS order, newline, then DFS order.
std::string graphTraversal(const std::vector<std::vector<int>>& adj, int start) {
    int n = static_cast<int>(adj.size());
    std::vector<bool> visited(n, false);

    // BFS using queue
    std::queue<int> q;
    q.push(start);
    visited[start] = true;
    std::string bfsResult;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (!bfsResult.empty()) bfsResult += " ";
        bfsResult += std::to_string(u);
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }

    // Reset visited for DFS
    std::fill(visited.begin(), visited.end(), false);

    // DFS using stack
    std::stack<int> stk;
    stk.push(start);
    visited[start] = true;
    std::string dfsResult;
    while (!stk.empty()) {
        int u = stk.top();
        stk.pop();
        if (!dfsResult.empty()) dfsResult += " ";
        dfsResult += std::to_string(u);
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                stk.push(v);
            }
        }
    }

    return bfsResult + "\n" + dfsResult;
}
#include <cassert>
#include <vector>
#include <string>

// Assume graphTraversal is defined as above.

int main() {
    // Simple chain: 0-1-2
    std::vector<std::vector<int>> g1 = {{1}, {0,2}, {1}};
    assert(graphTraversal(g1, 0) == "0 1 2\n0 1 2");

    // Star: center 0 connected to 1,2,3
    std::vector<std::vector<int>> g2 = {{1,2,3}, {0}, {0}, {0}};
    std::string r2 = graphTraversal(g2, 0);
    assert(r2 == "0 1 2 3\n0 3 2 1");

    // With self-loop and multiple edges (ignored by visited)
    std::vector<std::vector<int>> g3 = {{0,1,1}, {0,0}};
    std::string r3 = graphTraversal(g3, 0);
    assert(r3 == "0 1\n0 1");

    // Larger graph
    std::vector<std::vector<int>> g4 = {{2,1}, {3}, {0,3,4}, {1,2}, {2}};
    std::string r4 = graphTraversal(g4, 0);
    // BFS from 0: neighbors 2,1 -> then 3 (from 2) then 4 (from 2) – order: 0 2 1 3 4
    // DFS from 0: pop 0, push 2 and 1 (order in adj list), pop 1, push 3, pop 3, pop 2, push 4, pop 4
    assert(r4 == "0 2 1 3 4\n0 1 3 2 4");

    // Single node with self-loop
    std::vector<std::vector<int>> g5 = {{0}};
    assert(graphTraversal(g5, 0) == "0\n0");

    return 0;
}
