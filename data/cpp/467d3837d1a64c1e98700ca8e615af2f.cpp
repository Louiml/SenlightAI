// Write a C++ function that, given a vector of integers representing vertex IDs and a graph stored as an adjacency list (using `vector<vector<int>>` where `graph[node]` is the list of out-neighbors), computes a new order of the vertices such that for each vertex in the input vector, its relative rank in the output order is determined by sorting the input vertices by their out-degree in descending order (ties broken by ascending vertex ID). The function must return a vector of length equal to the input vector's size, where the element at position `i` is the rank (0-based) assigned to the vertex `input[i]` in that sorted order. If a vertex in the input appears multiple times, it must appear multiple times in the output with the same rank. Assume the graph has at least as many vertices as the maximum vertex ID in the input (vertex IDs are 0-indexed). The function should be named `rankVerticesByOutDegree` and should take two parameters: `const vector<vector<int>>& graph` and `const vector<int>& input`. It must not modify the graph or the input vector.

// The solution involves extracting the out-degree of each vertex from the adjacency list, then creating a vector of pairs `(out-degree, vertex ID)` for only the vertices listed in the input vector. Sort this vector of pairs using a custom comparator: descending by out-degree, and if equal, ascending by vertex ID. After sorting, assign ranks from 0 to `n-1` (where `n` is the input size) in the sorted order. Then, for each original vertex in the input, map it back to its assigned rank by consulting a dictionary (e.g., `unordered_map<int, int>`) that stores the rank for each unique vertex. Edge cases: the input may contain duplicates; the same vertex will receive the same rank regardless of how many times it appears. Vertices not in the input are ignored entirely; the function only ranks the given vertices. The graph may have vertices with zero out-degree, which are sorted to the end (since descending, they get the highest ranks). Complexity: Let `n = input.size()` and `V` be the total number of vertices in the graph. Creating the pairs takes `O(n)` time, sorting takes `O(n log n)`, and building the mapping takes `O(n)` time. Total time is `O(n log n)`, with `O(n)` auxiliary space for the pairs, sorted vector, and mapping.

#include <vector>
#include <algorithm>
#include <unordered_map>

// Given an adjacency list graph and a list of vertex IDs, return a vector
// of ranks (0-based) for the vertices in input, sorted by out-degree descending,
// then by vertex ID ascending. Duplicates in input get the same rank.
std::vector<int> rankVerticesByOutDegree(
    const std::vector<std::vector<int>>& graph,
    const std::vector<int>& input) {
    // Build a vector of pairs (out-degree, vertex ID) only for vertices in input.
    std::vector<std::pair<int, int>> degreeVertex;
    degreeVertex.reserve(input.size());
    for (int v : input) {
        degreeVertex.emplace_back(static_cast<int>(graph[v].size()), v);
    }
    
    // Sort: descending out-degree, then ascending vertex ID.
    std::sort(degreeVertex.begin(), degreeVertex.end(),
        [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            if (a.first != b.first) return a.first > b.first;
            return a.second < b.second;
        });
    
    // Assign rank to each unique vertex.
    std::unordered_map<int, int> vertexRank;
    for (size_t i = 0; i < degreeVertex.size(); ++i) {
        int vertex = degreeVertex[i].second;
        // If duplicate, the first occurrence sets the rank, others ignore.
        if (vertexRank.find(vertex) == vertexRank.end()) {
            vertexRank[vertex] = static_cast<int>(i);
        }
    }
    
    // Build output ranks for each vertex in input order.
    std::vector<int> result;
    result.reserve(input.size());
    for (int v : input) {
        result.push_back(vertexRank[v]);
    }
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Graph: 0->1,2 ; 1->2 ; 2->1 ; 3->0,1,2 ; 4->empty
    std::vector<std::vector<int>> graph = {
        {1, 2},
        {2},
        {1},
        {0, 1, 2},
        {}
    };

    // Test 1: Basic ordering by degree
    std::vector<int> input1 = {0, 1, 2, 3, 4};
    std::vector<int> ranks1 = rankVerticesByOutDegree(graph, input1);
    // degrees: 3:3, 0:2, 1:1, 2:1, 4:0 -> sorted order: 3,0,1,2,4; ranks: 3->0, 0->1, 1->2, 2->3, 4->4
    assert(ranks1 == std::vector<int>({1, 2, 3, 0, 4}));

    // Test 2: Duplicate vertices receive same rank
    std::vector<int> input2 = {3, 3, 0, 3};
    std::vector<int> ranks2 = rankVerticesByOutDegree(graph, input2);
    // Only vertices 3 and 0 appear; sorted: 3 (degree 3) then 0 (degree 2); ranks: 3->0, 0->1
    assert(ranks2 == std::vector<int>({0, 0, 1, 0}));

    // Test 3: Single vertex
    std::vector<int> input3 = {4};
    std::vector<int> ranks3 = rankVerticesByOutDegree(graph, input3);
    assert(ranks3 == std::vector<int>({0}));

    // Test 4: Tie-breaking by ascending vertex ID
    std::vector<int> input4 = {2, 1, 4, 0};
    // degrees: 1:1, 2:1, 0:2, 4:0 -> sorted: 0 (2), 1 (1), 2 (1), 4 (0); ranks: 0->0, 1->1, 2->2, 4->3
    std::vector<int> ranks4 = rankVerticesByOutDegree(graph, input4);
    assert(ranks4 == std::vector<int>({2, 1, 3, 0}));

    // Test 5: Empty input
    std::vector<int> input5 = {};
    std::vector<int> ranks5 = rankVerticesByOutDegree(graph, input5);
    assert(ranks5.empty());
}
