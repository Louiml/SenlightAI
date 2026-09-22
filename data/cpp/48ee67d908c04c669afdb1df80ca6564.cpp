/*
Write a C++ function `std::vector<int> stableTopologicalOrder(int N, const std::vector<std::pair<int,int>>& edges)` that, given a directed graph with vertices labeled 1 through N and a list of directed edges (a,b) meaning “a must come before b”, returns a topological ordering of all vertices such that among all valid topological orderings, the output is lexicographically smallest (i.e., the sequence is as small as possible when compared element-by-element). If no topological ordering exists (i.e., the graph contains a cycle), return an empty vector. The function must handle duplicate edges (they should be ignored), self-loops (which cause a cycle), and isolated vertices. The input will have at most 100,000 vertices and 200,000 edges.
*/
#include <vector>
#include <queue>
#include <set>
#include <utility>

// Returns a lexicographically smallest topological ordering of vertices 1..N.
// Edges are given as pairs (a,b) meaning a must come before b.
// Returns an empty vector if a cycle exists or N == 0.
std::vector<int> stableTopologicalOrder(int N, const std::vector<std::pair<int,int>>& edges) {
    if (N <= 0) return {};

    // Deduplicate edges to avoid double-counting indegrees
    std::set<std::pair<int,int>> uniqueEdges;
    for (const auto& e : edges) {
        if (e.first >= 1 && e.first <= N && e.second >= 1 && e.second <= N) {
            uniqueEdges.insert(e);
        }
    }

    // Build adjacency list and indegree array (1-indexed)
    std::vector<std::vector<int>> graph(N + 1);
    std::vector<int> indegrees(N + 1, 0);
    for (const auto& e : uniqueEdges) {
        graph[e.first].push_back(e.second);
        indegrees[e.second]++;
    }

    // Min-heap to always pick the smallest available vertex
    std::priority_queue<int, std::vector<int>, std::greater<int>> ready;
    for (int v = 1; v <= N; ++v) {
        if (indegrees[v] == 0) ready.push(v);
    }

    std::vector<int> order;
    order.reserve(N);

    while (!ready.empty()) {
        int current = ready.top();
        ready.pop();
        order.push_back(current);
        for (int neighbor : graph[current]) {
            indegrees[neighbor]--;
            if (indegrees[neighbor] == 0) ready.push(neighbor);
        }
    }

    if (static_cast<int>(order.size()) != N) return {}; // cycle detected
    return order;
}
#include <cassert>
#include <vector>
#include <utility>

// Function declaration (from solution)
std::vector<int> stableTopologicalOrder(int N, const std::vector<std::pair<int,int>>& edges);

int main() {
    // Basic chain: 1->2->3
    assert(stableTopologicalOrder(3, {{1,2},{2,3}}) == std::vector<int>({1,2,3}));

    // Lexicographically smallest among multiple valid: 3->1, 2->1, so possible orders: 2,3,1 or 3,2,1; smallest is 2,3,1
    assert(stableTopologicalOrder(3, {{3,1},{2,1}}) == std::vector<int>({2,3,1}));

    // Cycle detection
    assert(stableTopologicalOrder(3, {{1,2},{2,3},{3,1}}).empty());

    // Self-loop is a cycle
    assert(stableTopologicalOrder(2, {{1,1}}).empty());

    // Duplicate edges ignored
    assert(stableTopologicalOrder(3, {{1,2},{1,2},{2,3}}) == std::vector<int>({1,2,3}));

    // Isolated vertices: N=4 with edge 2->3, order: 1,2,3,4 (1 and 4 are ready initially, smallest first)
    assert(stableTopologicalOrder(4, {{2,3}}) == std::vector<int>({1,2,3,4}));

    // Disconnected components, might need to process smallest available
    assert(stableTopologicalOrder(5, {{5,1},{4,2}}) == std::vector<int>({3,4,5,2,1}));

    // No edges: all vertices isolated, return sorted order
    assert(stableTopologicalOrder(4, {}) == std::vector<int>({1,2,3,4}));

    // N=0 returns empty
    assert(stableTopologicalOrder(0, {}).empty());

    // Edge case: single vertex with no edges
    assert(stableTopologicalOrder(1, {}) == std::vector<int>({1}));

    // More complex: DAG where min-heap matters: 1->3, 2->3, 4->5, output: 1,2,4,3,5? Actually 4 available early, but 1,2 are smaller
    assert(stableTopologicalOrder(5, {{1,3},{2,3},{4,5}}) == std::vector<int>({1,2,4,3,5}));
}
// The core algorithm is Kahn’s algorithm (BFS-based topological sort) with a min-heap priority queue to always select the smallest available vertex (indegree 0) next, ensuring lexicographically smallest output. First, build an adjacency list and compute indegrees for each vertex, ignoring duplicate edges (can use a set or simply allow duplicates—duplicates only increment indegree multiple times, which is wrong; better to deduplicate edges by storing them in a set or checking when adding). To handle duplicates robustly, use a `std::set<std::pair<int,int>>` during input to filter duplicates before building the graph. Then initialize a priority queue with all vertices having indegree 0. While the queue is not empty, pop the smallest, append to result, and for each neighbor, decrement indegree; if it becomes 0, push it. If the result size equals N, return it; otherwise, a cycle exists and return an empty vector. Edge cases: N=0 returns empty; self-loop creates an indegree that never reaches 0; isolated vertices appear at the end if all others are processed (or can be processed from the start if their indegree is 0). Time complexity O((N+E) log N) due to the priority queue, and space O(N+E) for adjacency and indegree arrays.
