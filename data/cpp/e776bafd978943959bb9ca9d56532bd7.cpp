Write a C++ function `findOrder` that takes the number of vertices `N` and a vector of directed edges `edges` (each edge is a pair `(from, to)`, with vertex labels from 1 to N) and returns a `std::vector<int>` representing a valid topological ordering of the graph. If the graph contains a cycle (i.e., no valid topological order exists), return an empty vector. The function must handle the edge cases of `N = 0` (return empty) and graphs with no edges (return any ordering, e.g., 1 to N). The input edges may be unsorted, and there may be duplicate edges (which should be ignored in terms of degree counting to avoid over-counting). The solution must be self-contained, not depend on any external libraries other than the standard library, and the function signature must be exactly `std::vector<int> findOrder(int N, const std::vector<std::pair<int,int>>& edges)`.

// The problem is a classic topological sort on a directed graph. Since the graph may have cycles, we need to detect them. The approach uses Kahn’s algorithm with an in-degree array: first compute in-degree for each vertex (ignoring duplicate edges by either using a set or simply adding only if not already present, but since in-degree should only count unique predecessors, a simpler method is to use a boolean or count duplicates correctly—actually for topological sort, duplicate edges don't affect correctness if we treat them as one edge, because they increase in-degree unnecessarily. So we should deduplicate edges. One way is to store adjacency as a set of neighbors, or we can just count in-degree from unique edges by using a set of pairs). Then, initialize a queue with all vertices having in-degree 0. Repeatedly pop a vertex, add it to the result, and for each unique neighbor, decrement its in-degree; if it reaches 0, push it to the queue. If the result size equals N, we have a valid ordering; otherwise, there is a cycle and we return an empty vector. Edge cases: N=0 returns empty vector; if no edges, in-degree all zero, queue holds all vertices in order 1..N, so result is [1..N]. Time complexity is O(N + E) after deduplication, but with deduplication using a set of edges it becomes O(E log E) if we sort or use unordered_set; a simpler approach is to use adjacency lists of vectors and a separate set for each adjacency to avoid duplicates, but that increases overhead. For simplicity and given typical constraints, we can deduplicate by sorting edges or using a set of pairs. Space complexity is O(N + E). For correctness, we should note that the input may have duplicate edges, so we must deduplicate them for in-degree computation to avoid artificially high in-degrees that never reach zero.

#include <vector>
#include <queue>
#include <set>
#include <algorithm>

// Returns a topological ordering of the graph with vertices 1..N.
// If the graph contains a cycle, returns an empty vector.
std::vector<int> findOrder(int N, const std::vector<std::pair<int,int>>& edges) {
    if (N == 0) return {};
    
    // Deduplicate edges using a set of pairs
    std::set<std::pair<int,int>> unique_edges(edges.begin(), edges.end());
    
    // Build adjacency list and compute in-degrees
    std::vector<std::vector<int>> adj(N + 1);
    std::vector<int> in_degree(N + 1, 0);
    for (const auto& edge : unique_edges) {
        int from = edge.first;
        int to = edge.second;
        adj[from].push_back(to);
        in_degree[to]++;
    }
    
    // Queue for vertices with in-degree 0
    std::queue<int> q;
    for (int i = 1; i <= N; ++i) {
        if (in_degree[i] == 0) {
            q.push(i);
        }
    }
    
    std::vector<int> result;
    result.reserve(N);
    
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        result.push_back(node);
        for (int neighbor : adj[node]) {
            if (--in_degree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }
    
    // If result size is less than N, there's a cycle
    if (static_cast<int>(result.size()) != N) {
        return {};
    }
    return result;
}

#include <cassert>
#include <vector>
#include <utility>

// Include the solution function here (as defined above)

int main() {
    // Test 1: Simple linear graph 1->2->3
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3}};
        std::vector<int> result = findOrder(3, edges);
        assert(result.size() == 3);
        // Must be exactly [1,2,3] because it's the only valid order
        assert(result[0] == 1 && result[1] == 2 && result[2] == 3);
    }
    
    // Test 2: Graph with no edges – any order is valid, but we expect [1,2,3]
    {
        std::vector<std::pair<int,int>> edges;
        std::vector<int> result = findOrder(3, edges);
        assert(result.size() == 3);
        assert(result[0] == 1 && result[1] == 2 && result[2] == 3);
    }
    
    // Test 3: Graph with a cycle – should return empty
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,1}};
        std::vector<int> result = findOrder(3, edges);
        assert(result.empty());
    }
    
    // Test 4: Graph with duplicate edges – should not affect result
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{1,2},{2,3},{2,3}};
        std::vector<int> result = findOrder(3, edges);
        assert(result.size() == 3);
        assert(result[0] == 1 && result[1] == 2 && result[2] == 3);
    }
    
    // Test 5: Graph with multiple valid orders – check that result is a valid topological order
    {
        std::vector<std::pair<int,int>> edges = {{1,3},{2,3}};
        std::vector<int> result = findOrder(3, edges);
        assert(result.size() == 3);
        // We need to verify order constraints: 1 must come before 3, 2 must come before 3
        int pos1 = -1, pos2 = -1, pos3 = -1;
        for (int i = 0; i < 3; ++i) {
            if (result[i] == 1) pos1 = i;
            if (result[i] == 2) pos2 = i;
            if (result[i] == 3) pos3 = i;
        }
        assert(pos1 != -1 && pos2 != -1 && pos3 != -1);
        assert(pos1 < pos3 && pos2 < pos3);
    }
    
    // Test 6: N=0 – should return empty
    {
        std::vector<std::pair<int,int>> edges;
        std::vector<int> result = findOrder(0, edges);
        assert(result.empty());
    }
    
    // Test 7: Graph with cycle not involving all vertices – should still return empty
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,1},{3,4}};
        std::vector<int> result = findOrder(4, edges);
        assert(result.empty());
    }
    
    // Test 8: Large N with no edges – order is 1..N
    {
        std::vector<std::pair<int,int>> edges;
        std::vector<int> result = findOrder(5, edges);
        assert(result.size() == 5);
        for (int i = 0; i < 5; ++i) {
            assert(result[i] == i+1);
        }
    }
    
    return 0;
}
