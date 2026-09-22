/*
You are given a directed acyclic graph (DAG) represented by an integer `n` (the number of vertices, labeled from 0 to n-1, though not all labels may appear in the edges) and a vector of directed edges. Write a C++ function that returns the smallest set of vertices from which all nodes in the graph are reachable. The result should be a vector of integers sorted in ascending order. You may assume the graph contains no cycles, but some vertices may have no incident edges at all (isolated vertices). Your function must handle graphs where some vertices have zero out-degree, and it must return vertices that cover all nodes—specifically, the set of vertices with zero in-degree. Note that if a vertex has no incoming edges, it cannot be reached from any other vertex, so it must be included. For vertices with incoming edges but no outgoing edges, they are reachable from their predecessors and thus need not be included unless they also have no incoming edges. The input edges list may contain duplicates, but you should treat them as a single edge. The output must be a vector of vertex IDs in increasing order.
*/

#include <vector>

// Return the smallest set of vertices from which all nodes in a DAG are reachable.
// The graph has vertices 0..n-1; edges are pairs [from, to].
std::vector<int> findSmallestSetOfVertices(int n, const std::vector<std::vector<int>>& edges) {
    std::vector<int> in_degree(n, 0);
    
    // Count in-degrees. Duplicate edges are harmless: they only increase in-degree,
    // never turning a true source (in-degree 0) into non-source.
    for (const auto& edge : edges) {
        // edge[1] is the target vertex
        in_degree[edge[1]]++;
    }
    
    std::vector<int> result;
    for (int v = 0; v < n; ++v) {
        if (in_degree[v] == 0) {
            result.push_back(v);
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// Function declaration (as per solution)
std::vector<int> findSmallestSetOfVertices(int n, const std::vector<std::vector<int>>& edges);

int main() {
    // Example 1: simple chain 0->1->2, source is 0
    assert(findSmallestSetOfVertices(3, {{0,1},{1,2}}) == std::vector<int>({0}));
    
    // Example 2: two independent edges, sources 0 and 2
    assert(findSmallestSetOfVertices(4, {{0,1},{2,3}}) == std::vector<int>({0,2}));
    
    // Example 3: isolated vertices (0 has no edges, 1 has no edges), plus edge from 2 to 3
    assert(findSmallestSetOfVertices(4, {{2,3}}) == std::vector<int>({0,1,2}));
    
    // Example 4: duplicate edges, still sources are 0 and 1
    assert(findSmallestSetOfVertices(3, {{0,2},{0,2},{1,2}}) == std::vector<int>({0,1}));
    
    // Example 5: all vertices are isolated (no edges)
    assert(findSmallestSetOfVertices(3, {}) == std::vector<int>({0,1,2}));
    
    // Example 6: single vertex with no edges
    assert(findSmallestSetOfVertices(1, {}) == std::vector<int>({0}));
    
    // Example 7: vertex with in-degree but also an outgoing edge to a new vertex
    assert(findSmallestSetOfVertices(3, {{0,1},{1,2}}) == std::vector<int>({0}));
    
    // Example 8: larger DAG, sources are 0,2,4
    assert(findSmallestSetOfVertices(5, {{0,1},{0,3},{2,3},{4,3}}) == std::vector<int>({0,2,4}));
    
    return 0;
}

// The problem reduces to finding all vertices with zero in-degree (i.e., no incoming edges). In a DAG, every vertex is reachable from at least one zero-in-degree vertex, and no vertex with in-degree > 0 can be a source because it has at least one predecessor. Therefore, the minimal set of starting vertices is exactly the set of all zero-in-degree vertices. To compute this, we count the in-degree of each vertex that appears in any edge. We must also consider isolated vertices (those with no edges at all): they have in-degree 0 and must be included. Degrees are tracked in an array of size `n` (since vertices are labeled 0..n-1). After counting, iterate from 0 to n-1 and collect all indices with in-degree 0. Edge cases: if a vertex never appears in any edge, its in-degree remains 0 and is included; duplicate edges do not affect the count if we simply increment each time, but since duplicates represent the same edge, incrementing multiple times could artificially increase in-degree, so it is safe to either disregard duplicates (e.g., using a set of pairs) or, simpler, just increment each occurrence and then check only if in-degree == 0—duplicates only make in-degree larger, never zero, so a vertex that is truly a source will remain zero even with duplicates. Time complexity: O(n + E) where E is the number of edge entries (including duplicates), space O(n).
