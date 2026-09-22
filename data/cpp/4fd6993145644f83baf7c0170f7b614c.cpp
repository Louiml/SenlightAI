/*
Write a C++ function that processes a simplified representation of a pose-graph optimization problem, where a graph is stored as a vector of edges, each connecting two vertex IDs. The function must "anonymize" the graph by removing one endpoint from every edge that connects two vertices with IDs differing by more than 1, specifically removing the endpoint that corresponds to the vertex with the **larger ID** (if both endpoints are non-zero and distinct). For edges connecting consecutive IDs (difference ≤ 1) or where either endpoint is zero or both endpoints are the same, the edge must remain unchanged. The function should return the resulting vector of edges after processing all edges. Vertices are identified by positive integer IDs; an ID of 0 means "removed" or "null" and should never be considered for removal. The input vector may contain duplicate edges, and the order of edges in the output must match the original order.
*/
#include <vector>
#include <cstdlib> // for std::abs on int
#include <utility> // for std::pair

// Represents an edge in the graph: first vertex ID, second vertex ID.
using GraphEdge = std::pair<int, int>;

// Given a list of edges, return a new list where every edge that connects
// two distinct non-zero vertices with IDs differing by more than 1 has the
// endpoint with the larger ID set to 0. All other edges are unchanged.
std::vector<GraphEdge> anonymizeGraph(const std::vector<GraphEdge>& edges) {
    std::vector<GraphEdge> result;
    result.reserve(edges.size());

    for (const GraphEdge& e : edges) {
        int from = e.first;
        int to = e.second;

        // Check the conditions that permit anonymization:
        // Both endpoints are non-zero, endpoints are different,
        // and the absolute difference is greater than 1.
        if (from != 0 && to != 0 && from != to && std::abs(from - to) > 1) {
            // Remove the endpoint with the larger ID.
            if (from > to) {
                from = 0;
            } else {
                to = 0;
            }
        }

        result.emplace_back(from, to);
    }

    return result;
}
#include <cassert>
#include <vector>
#include <utility>

using GraphEdge = std::pair<int, int>;

// Declaration of the function under test.
std::vector<GraphEdge> anonymizeGraph(const std::vector<GraphEdge>& edges);

int main() {
    // Case 1: Edge with large difference, larger ID on 'from' side.
    {
        std::vector<GraphEdge> input = {{10, 2}};
        std::vector<GraphEdge> expected = {{0, 2}};
        assert(anonymizeGraph(input) == expected);
    }

    // Case 2: Edge with large difference, larger ID on 'to' side.
    {
        std::vector<GraphEdge> input = {{3, 8}};
        std::vector<GraphEdge> expected = {{3, 0}};
        assert(anonymizeGraph(input) == expected);
    }

    // Case 3: Consecutive IDs (difference = 1) -> unchanged.
    {
        std::vector<GraphEdge> input = {{4, 5}};
        std::vector<GraphEdge> expected = {{4, 5}};
        assert(anonymizeGraph(input) == expected);
    }

    // Case 4: Same ID -> unchanged.
    {
        std::vector<GraphEdge> input = {{7, 7}};
        std::vector<GraphEdge> expected = {{7, 7}};
        assert(anonymizeGraph(input) == expected);
    }

    // Case 5: One endpoint already zero -> unchanged.
    {
        std::vector<GraphEdge> input = {{0, 9}};
        std::vector<GraphEdge> expected = {{0, 9}};
        assert(anonymizeGraph(input) == expected);
    }

    // Case 6: Mixed edges, ensure order and content are preserved.
    {
        std::vector<GraphEdge> input = {{1, 5}, {2, 3}, {0, 4}, {6, 6}, {7, 2}};
        std::vector<GraphEdge> expected = {{0, 5}, {2, 3}, {0, 4}, {6, 6}, {0, 2}};
        assert(anonymizeGraph(input) == expected);
    }

    // Case 7: Empty input.
    {
        std::vector<GraphEdge> input = {};
        std::vector<GraphEdge> expected = {};
        assert(anonymizeGraph(input) == expected);
    }

    // Case 8: Multiple edges with same large difference pattern.
    {
        std::vector<GraphEdge> input = {{99, 1}, {50, 100}, {12, 4}};
        std::vector<GraphEdge> expected = {{0, 1}, {50, 0}, {0, 4}};
        assert(anonymizeGraph(input) == expected);
    }

    // Case 9: Negative IDs? Task says positive only, but we can test if any.
    // This case is not specified, but we check that behavior is consistent:
    // difference uses absolute, larger ID is the more positive one.
    {
        std::vector<GraphEdge> input = {{-5, 2}};
        // |−5−2| = 7 > 1, endpoints distinct and non-zero (since we
        // treat zero only as literal 0, negative IDs are non-zero).
        // larger ID is 2, so set 'to' to 0.
        std::vector<GraphEdge> expected = {{-5, 0}};
        assert(anonymizeGraph(input) == expected);
    }

    return 0;
}
// The main algorithm is a straightforward linear scan over the list of edges. For each edge represented as a pair `(from, to)` of vertex IDs, we check three conditions before any modification: both endpoints must be non-zero, the endpoints must be different from each other, and the absolute difference `|from - to|` must be greater than 1. If all conditions hold, we remove the endpoint that has the larger ID by setting that endpoint to 0. If both endpoints are non-zero and distinct but the difference is ≤ 1, or if either endpoint is zero, or if both endpoints are equal, we leave the edge unchanged. No additional data structures are needed beyond the output vector. Edge cases include: edges with ID 0 on one side (skip), edges with both IDs equal (skip), and cases where the larger ID appears on either the `from` or `to` side (handled by comparing the two values). The algorithm runs in O(n) time for n edges and uses O(n) auxiliary space for the output vector (which is a copy of the input vector if we return a new vector, or O(1) if we modify in place, but here we return a new vector for clarity). Space complexity is O(n) because we store the result.
