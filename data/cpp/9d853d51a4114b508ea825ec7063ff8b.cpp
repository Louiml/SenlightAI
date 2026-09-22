/*
Write a C++ function `std::unordered_set<int> findLoopTails(const std::vector<std::pair<int, int>>& edges, int numVertices, int loopTail)` that, given a directed graph represented as a list of directed edges (where each edge is a pair `(source, target)` with vertex indices in `[0, numVertices)`), determines all vertices from which the specified `loopTail` vertex is reachable. A vertex `u` is considered "reachable to the tail" if there exists a directed path from `u` to `loopTail`. The function should return an unordered set containing all such vertices, including the `loopTail` itself (since a zero-length path counts). The graph may contain self-loops, multiple edges between the same pair, and cycles (including cycles involving `loopTail`). Vertices that are not present in any edge are still valid vertices and should be included in the result if they equal `loopTail` (i.e., the loopTail itself is always in the result). The graph is directed; reverse traversal is needed. Assume `numVertices > 0` and `loopTail` is in `[0, numVertices)`. The function must not modify the input vectors.
*/
#include <vector>
#include <unordered_set>
#include <stack>

// Given a directed graph as a list of edges (source, target) with vertices 0..numVertices-1,
// return all vertices from which the given 'loopTail' vertex is reachable.
// The returned set includes 'loopTail' itself (zero-length path).
std::unordered_set<int> findLoopTails(
    const std::vector<std::pair<int, int>>& edges,
    int numVertices,
    int loopTail)
{
    // Build reverse adjacency list: for each original edge (u, v), add v -> u.
    std::vector<std::vector<int>> reverseAdj(numVertices);
    for (const auto& e : edges) {
        int u = e.first;
        int v = e.second;
        // Validate vertex indices? Assume valid per specification.
        reverseAdj[v].push_back(u); // reverse direction
    }

    // DFS from loopTail on the reverse graph.
    std::vector<bool> visited(numVertices, false);
    std::stack<int> stack;
    stack.push(loopTail);
    visited[loopTail] = true;

    while (!stack.empty()) {
        int current = stack.top();
        stack.pop();
        for (int neighbor : reverseAdj[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                stack.push(neighbor);
            }
        }
    }

    // Collect all visited vertices.
    std::unordered_set<int> result;
    for (int v = 0; v < numVertices; ++v) {
        if (visited[v]) {
            result.insert(v);
        }
    }
    return result;
}
#include <cassert>
#include <unordered_set>
#include <vector>

// Declaration (assume the solution function is defined above).
std::unordered_set<int> findLoopTails(
    const std::vector<std::pair<int, int>>& edges,
    int numVertices,
    int loopTail);

int main() {
    // Test 1: Simple chain 0->1->2->3, loopTail=3
    {
        std::vector<std::pair<int, int>> edges = {{0,1},{1,2},{2,3}};
        auto result = findLoopTails(edges, 4, 3);
        std::unordered_set<int> expected = {0,1,2,3};
        assert(result == expected);
    }
    // Test 2: Disconnected nodes, loopTail=5 (isolated)
    {
        std::vector<std::pair<int, int>> edges = {{0,1},{1,2}};
        auto result = findLoopTails(edges, 6, 5);
        std::unordered_set<int> expected = {5}; // only itself reachable
        assert(result == expected);
    }
    // Test 3: Cycle with loopTail on the cycle
    {
        std::vector<std::pair<int, int>> edges = {{0,1},{1,2},{2,0},{2,3}};
        auto result = findLoopTails(edges, 4, 2);
        std::unordered_set<int> expected = {0,1,2}; // 3 cannot reach 2
        assert(result == expected);
    }
    // Test 4: Self-loop and multiple edges
    {
        std::vector<std::pair<int, int>> edges = {{0,0},{0,0},{1,0},{2,1},{3,2},{3,3}};
        auto result = findLoopTails(edges, 4, 0);
        std::unordered_set<int> expected = {0,1,2,3}; // all can reach 0 (3->2->1->0)
        assert(result == expected);
    }
    // Test 5: Single vertex
    {
        std::vector<std::pair<int, int>> edges = {};
        auto result = findLoopTails(edges, 1, 0);
        std::unordered_set<int> expected = {0};
        assert(result == expected);
    }
    // Test 6: loopTail not reachable from any other
    {
        std::vector<std::pair<int, int>> edges = {{0,1},{1,2}};
        auto result = findLoopTails(edges, 3, 2);
        std::unordered_set<int> expected = {2}; // only itself
        assert(result == expected);
    }
    return 0;
}
// The problem asks for all vertices that can reach the given `loopTail`. This is equivalent to performing a depth‑first search (DFS) on the *reverse graph*, starting from `loopTail`. In the reverse graph, every edge `(u, v)` becomes `(v, u)`. A DFS starting from `loopTail` in the reverse graph will mark exactly those vertices from which `loopTail` is reachable in the original graph, because if there is a path `u → … → loopTail` in the original graph, then in the reverse graph there is a path `loopTail → … → u`. The DFS must handle cycles correctly (using a visited array to avoid infinite loops). We can implement the DFS either recursively or iteratively; an iterative stack is safer to avoid stack overflow for large graphs, but recursion is acceptable for typical test sizes. Edge cases: (1) `loopTail` itself is always included because the zero‑length path reaches itself; (2) vertices with no outgoing edges in the reverse graph but that are still reachable (e.g., a direct predecessor) are handled; (3) disconnected vertices not reachable to `loopTail` are excluded; (4) multi‑edges and self‑loops do not affect reachability; (5) if a vertex is isolated and not equal to `loopTail`, it is excluded. Time complexity: Building an adjacency list for the reverse graph takes O(E) time (where E is the number of edges). DFS visits each edge at most once, so total time is O(V + E). Space complexity: O(V) for the visited array and O(V + E) for the reverse adjacency list.
