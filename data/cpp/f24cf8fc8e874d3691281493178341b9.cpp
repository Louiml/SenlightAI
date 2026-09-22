Write a C++ function that, given a vector of unsigned integers representing vertex IDs and a vector of pairs representing undirected edges (each pair contains two vertex IDs), returns a new vector containing only the vertex IDs that have an odd degree (i.e., an odd number of incident edges). The function should handle duplicate edges (i.e., the same pair appearing multiple times should count each occurrence separately), self-loops (an edge connecting a vertex to itself should count as two incident edges for that vertex, since an undirected loop contributes degree 2), and vertices that appear in the edge list but have zero degree should not appear in the output. The output vector should be sorted in ascending order. If no vertices have odd degree, return an empty vector.

// The solution requires counting the degree of each vertex. For each edge (u, v) in the input, we increment the degree of u by 1 and the degree of v by 1. If u == v (self-loop), this effectively adds 2 to that vertex's degree since we increment twice. After processing all edges, we collect all vertices that have an odd degree. To handle potential large vertex IDs, we can use a hash map (e.g., `std::unordered_map<int, int>`) to store degrees, but since the task specifies unsigned integers as vertex IDs and we don't know the maximum ID, we could also use a `std::map` for sorted output later. After counting, we iterate over the map and collect keys where the degree modulo 2 equals 1. Finally, we sort the collected vertex IDs (if using `unordered_map`) or directly use a `std::set` for sorted output, or sort the vector at the end. Time complexity is O(E + V log V) where E is the number of edges and V is the number of distinct vertices with odd degree (if we sort). Space complexity is O(V) for storing degrees. Edge cases include: empty edge list (returns empty), self-loops (handle by incrementing twice), duplicate edges (count each occurrence separately), and vertices that only appear in edges with even degree (not included).

#include <vector>
#include <unordered_map>
#include <algorithm>

// Return sorted vector of vertex IDs with odd degree given edges list.
std::vector<unsigned int> oddDegreeVertices(
    const std::vector<unsigned int>& vertexIds,
    const std::vector<std::pair<unsigned int, unsigned int>>& edges
) {
    // We actually don't need vertexIds for counting, but keep it for potential validation.
    // Count degrees for all vertices appearing in edges.
    std::unordered_map<unsigned int, int> degree;
    for (const auto& e : edges) {
        degree[e.first]++;  // increment for first endpoint
        degree[e.second]++; // increment for second endpoint
        // If self-loop, this adds 2 to that vertex, which is correct for undirected loop.
    }
    
    std::vector<unsigned int> result;
    for (const auto& kv : degree) {
        if (kv.second % 2 == 1) {
            result.push_back(kv.first);
        }
    }
    
    // Sort ascending
    std::sort(result.begin(), result.end());
    return result;
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Empty graph
    {
        std::vector<unsigned int> vertices;
        std::vector<std::pair<unsigned int, unsigned int>> edges;
        assert(oddDegreeVertices(vertices, edges).empty());
    }
    
    // Simple path: 0-1, 1-2, 2-3 -> odd degrees at 0 and 3
    {
        std::vector<unsigned int> vertices = {0,1,2,3};
        std::vector<std::pair<unsigned int, unsigned int>> edges = {{0,1},{1,2},{2,3}};
        auto result = oddDegreeVertices(vertices, edges);
        std::vector<unsigned int> expected = {0,3};
        assert(result == expected);
    }
    
    // Self-loop: vertex 5 has loop, also edge 1-2
    // degrees: 1:1, 2:1, 5:2 (loop contributes 2) -> odd: 1,2
    {
        std::vector<unsigned int> vertices = {1,2,5};
        std::vector<std::pair<unsigned int, unsigned int>> edges = {{1,2},{5,5}};
        auto result = oddDegreeVertices(vertices, edges);
        std::vector<unsigned int> expected = {1,2};
        assert(result == expected);
    }
    
    // Duplicate edges count twice: edge (1,2) twice -> both get degree 2 (even), no odd vertices
    {
        std::vector<unsigned int> vertices = {1,2};
        std::vector<std::pair<unsigned int, unsigned int>> edges = {{1,2},{1,2}};
        assert(oddDegreeVertices(vertices, edges).empty());
    }
    
    // Larger test with duplicate and multiple odd vertices
    {
        std::vector<unsigned int> vertices = {10,20,30,40};
        std::vector<std::pair<unsigned int, unsigned int>> edges = {{10,20},{20,30},{30,10},{40,40},{10,20}};
        // degrees: 10:3 (odd), 20:3 (odd), 30:2 (even), 40:2 (even)
        auto result = oddDegreeVertices(vertices, edges);
        std::vector<unsigned int> expected = {10,20};
        assert(result == expected);
    }
    
    // Vertex only appears in even total, no odd vertices -> empty
    {
        std::vector<unsigned int> vertices = {7};
        std::vector<std::pair<unsigned int, unsigned int>> edges = {{7,7},{7,7}}; // each loop adds 2, total 4, even
        assert(oddDegreeVertices(vertices, edges).empty());
    }
    
    // Unsorted input IDs, valid vertex list includes isolated vertices not in edges
    {
        std::vector<unsigned int> vertices = {100, 1, 50};
        std::vector<std::pair<unsigned int, unsigned int>> edges = {{100,1},{1,100}}; // both degree 2, even
        assert(oddDegreeVertices(vertices, edges).empty());
    }
    
    return 0;
}
