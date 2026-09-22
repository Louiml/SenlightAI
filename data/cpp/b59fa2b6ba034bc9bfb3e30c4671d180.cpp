// Write a C++ function `countDistinctEdges` that takes two vectors: a vector of `Vertex` objects and a vector of `Edge` objects, where each `Vertex` stores a fixed-length 3-character code (as a null-terminated char array) and each `Edge` connects two vertices. The function should remove duplicate edges from the input edge list (keeping only the first occurrence of each unique edge as defined by the provided operators), sort the remaining edges in ascending order (using the provided `<` operator), and return the total number of edges that were removed due to duplication. Assume the input vectors are already filled and ordered arbitrarily; you may rely on the existing struct definitions and operators, but you must implement the function within the given context. Note that an edge `(A, B)` is considered equal to another only if both starting vertices match and both finishing vertices match (using the `==` operator on `Vertex`). The function should not modify the original input vectors; it should create a local copy of the edge list, process it, and return the count of duplicates removed.
// The solution approach involves first copying the input edge list into a local vector to avoid modifying the caller's data. Next, sort this copy using the existing `Edge::operator<` which orders edges lexicographically by start vertex then finish vertex. After sorting, iterate through the sorted list once to count duplicates: since sorting groups identical edges together, we can compare each edge with the previous one; if they are equal (using `Edge::operator==`), increment a duplicate counter; otherwise, move to the next unique edge. The total removed count is the number of duplicates found. The function returns this count, and the original vectors remain unchanged. Time complexity is \(O(n \log n)\) due to sorting, where \(n\) is the number of edges in the input. Space complexity is \(O(n)\) for the temporary copy of the edge list. Edge cases include an empty edge list (returns 0), a list with all unique edges (returns 0), and a list where edges appear multiple times scattered across the input (handled correctly by sorting). The provided `copy` for `Vertex` uses `std::copy` and the `set` method.
#include <vector>
#include <algorithm>

// Count how many duplicate edges are in the given edge list.
// Does not modify the input; returns the number of edges removed if duplicates were eliminated.
int countDistinctEdges(const std::vector<Vertex>& /* vertex_list */, const std::vector<Edge>& edge_list) {
    if (edge_list.empty()) {
        return 0;
    }

    // Create a local copy to work with, so the original is unchanged.
    std::vector<Edge> local_edges = edge_list;
    std::sort(local_edges.begin(), local_edges.end());

    int duplicate_count = 0;
    for (std::size_t i = 1; i < local_edges.size(); ++i) {
        if (local_edges[i] == local_edges[i - 1]) {
            ++duplicate_count;
        }
    }

    return duplicate_count;
}
#include <cassert>
#include <vector>
#include <string>

// Re-define the required structs for testing (same as in the problem).
constexpr int V_SIZE = 3;
struct Vertex {
    char index[V_SIZE + 1] = {};
    bool operator==(Vertex& v2) { return std::strcmp(index, v2.index) == 0; }
    bool operator<(Vertex& v2) { return std::strcmp(index, v2.index) < 0; }
    void set(std::string& vs) { std::copy(vs.begin(), vs.end(), index); }
    friend std::ostream& operator<<(std::ostream& os, const Vertex& v) { os << v.index << " "; return os; }
};
struct Edge {
    Vertex start{};
    Vertex finish{};
    bool operator<(Edge& edge2) {
        if (start < edge2.start) return true;
        if (edge2.start < start) return false;
        return finish < edge2.finish;
    }
    bool operator==(Edge& edge2) { return start == edge2.start && finish == edge2.finish; }
    friend std::ostream& operator<<(std::ostream& os, const Edge& edge) { os << edge.start << "-> " << edge.finish << std::endl; return os; }
};

// Include the solution function (copied here for completeness).
#include "solution.h"

int main() {
    // Helper lambda to create a vertex from a 3-char code.
    auto makeVertex = [](const std::string& code) {
        Vertex v;
        v.set(const_cast<std::string&>(code));
        return v;
    };

    // Empty edge list -> no duplicates
    std::vector<Vertex> v_empty;
    std::vector<Edge> e_empty;
    assert(countDistinctEdges(v_empty, e_empty) == 0);

    // Single edge -> no duplicates
    std::vector<Edge> e_one = {{makeVertex("ABC"), makeVertex("DEF")}};
    assert(countDistinctEdges(v_empty, e_one) == 0);

    // Two identical edges -> one duplicate
    std::vector<Edge> e_two_dup = {
        {makeVertex("ABC"), makeVertex("DEF")},
        {makeVertex("ABC"), makeVertex("DEF")}
    };
    assert(countDistinctEdges(v_empty, e_two_dup) == 1);

    // Three edges with two unique and one duplicate
    std::vector<Edge> e_mixed = {
        {makeVertex("AAA"), makeVertex("BBB")},
        {makeVertex("CCC"), makeVertex("DDD")},
        {makeVertex("AAA"), makeVertex("BBB")} // duplicate
    };
    assert(countDistinctEdges(v_empty, e_mixed) == 1);

    // All unique edges -> zero duplicates
    std::vector<Edge> e_all_unique = {
        {makeVertex("AAA"), makeVertex("BBB")},
        {makeVertex("AAA"), makeVertex("CCC")},
        {makeVertex("BBB"), makeVertex("AAA")}
    };
    assert(countDistinctEdges(v_empty, e_all_unique) == 0);

    // Multiple duplicates of same edge
    std::vector<Edge> e_many_dup = {
        {makeVertex("XYZ"), makeVertex("UVW")},
        {makeVertex("XYZ"), makeVertex("UVW")},
        {makeVertex("XYZ"), makeVertex("UVW")},
        {makeVertex("PQR"), makeVertex("STU")}
    };
    assert(countDistinctEdges(v_empty, e_many_dup) == 2); // two extras

    // Ensure input vectors are not modified
    std::vector<Edge> original = e_two_dup;
    countDistinctEdges(v_empty, e_two_dup);
    assert(e_two_dup == original); // if `==` were defined for vector; fallback check
    // Simpler check: compare sizes
    assert(e_two_dup.size() == 2);

    return 0;
}
