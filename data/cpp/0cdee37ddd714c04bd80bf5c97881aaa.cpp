/*
Given a polygonal mesh represented by a simple adjacency structure (vertices with 3D coordinates identified by integer IDs, and undirected edges connecting pairs of vertex IDs), write a C++ function `std::vector<int> GetBoundarySpanEdgeIDs(const std::vector<int>& vertexSpan, const std::vector<std::pair<int,int>>& edges)` that takes a list of vertex IDs forming an open or closed polyline (the vertex span) and a list of all undirected edges in the mesh, and returns the list of edge IDs (indices into the `edges` vector) that connect consecutive vertices in the vertex span, in the same order. If any consecutive pair of vertices does not correspond to an existing edge, return an empty vector. The function should treat edges as undirected, so either ordering of the vertex pair is acceptable.
*/

#include <vector>
#include <unordered_map>

// Given a list of vertex IDs forming a polyline (vertexSpan) and a list of undirected edges
// (each edge is a pair of vertex IDs), return the indices of edges in 'edges' that connect
// consecutive vertices in the span, in order. Return an empty vector if any pair is not an edge.
std::vector<int> GetBoundarySpanEdgeIDs(const std::vector<int>& vertexSpan,
                                        const std::vector<std::pair<int,int>>& edges) {
    // Build a lookup from a normalized unordered pair of vertex IDs to the edge index.
    std::unordered_map<int, std::unordered_map<int, int>> edgeLookup;
    for (int i = 0; i < static_cast<int>(edges.size()); ++i) {
        int a = edges[i].first;
        int b = edges[i].second;
        // Normalize so the smaller vertex ID is the key in the outer map.
        if (a > b) std::swap(a, b);
        edgeLookup[a][b] = i;
    }

    std::vector<int> result;
    int n = static_cast<int>(vertexSpan.size());
    if (n < 2) return result;  // Need at least two vertices to form an edge.

    for (int i = 0; i < n - 1; ++i) {
        int a = vertexSpan[i];
        int b = vertexSpan[i + 1];
        // Normalize for lookup.
        if (a > b) std::swap(a, b);
        auto outerIt = edgeLookup.find(a);
        if (outerIt == edgeLookup.end()) return {};
        auto innerIt = outerIt->second.find(b);
        if (innerIt == outerIt->second.end()) return {};
        result.push_back(innerIt->second);
    }
    return result;
}

#include <cassert>
#include <vector>

// The solution function is declared above (or included from a header).
int main() {
    // Simple triangle: vertices 0,1,2 and edges (0-1), (1-2), (2-0)
    std::vector<std::pair<int,int>> edges1 = {{0,1}, {1,2}, {2,0}};
    assert(GetBoundarySpanEdgeIDs({0,1,2}, edges1) == std::vector<int>({0,1}));
    assert(GetBoundarySpanEdgeIDs({2,1,0}, edges1) == std::vector<int>({1,0}));
    assert(GetBoundarySpanEdgeIDs({1,2}, edges1) == std::vector<int>({1}));

    // Single vertex span -> no edges, empty result.
    assert(GetBoundarySpanEdgeIDs({5}, edges1).empty());

    // Span with a missing edge (0-2 is present, but 0-3 is not).
    assert(GetBoundarySpanEdgeIDs({0,3}, edges1).empty());

    // Duplicate edges in the mesh: two edges between 0 and 1.
    std::vector<std::pair<int,int>> edges2 = {{0,1}, {0,1}, {1,2}};
    assert(GetBoundarySpanEdgeIDs({0,1,2}, edges2) == std::vector<int>({0,2}));

    // Larger mesh: quadrilateral with a diagonal.
    std::vector<std::pair<int,int>> edges3 = {{0,1}, {1,2}, {2,3}, {3,0}, {0,2}};
    assert(GetBoundarySpanEdgeIDs({3,2,0}, edges3) == std::vector<int>({2,4}));

    // Edges provided in different order.
    std::vector<std::pair<int,int>> edges4 = {{5,6}, {4,5}, {6,7}};
    assert(GetBoundarySpanEdgeIDs({6,5,4}, edges4) == std::vector<int>({0,1}));
}

// The solution iterates through the `vertexSpan` list, considering each consecutive pair `(v[i], v[i+1])`. For each pair, it searches the `edges` vector for an edge whose endpoints match the pair in either order. The search can be done using a hash map keyed by a pair of vertex IDs (normalized so the smaller ID comes first) for efficient lookup. If any pair is not found, the function returns an empty vector. Otherwise, it returns a vector of edge indices in the order they appear in the vertex span. Edge cases: an empty or single-vertex span should return an empty vector (since there are no edges to form). Duplicate edges (if present in the mesh) are acceptable—the first match is returned. The time complexity is \(O(n + m)\) where \(n\) is the number of vertices in the span and \(m\) is the total number of edges, because building the hash map takes \(O(m)\) and each lookup takes \(O(1)\). Space complexity is \(O(m)\) for the hash map.
