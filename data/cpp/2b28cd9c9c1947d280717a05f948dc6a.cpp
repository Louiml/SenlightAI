You are given `N` cities (numbered 1 through N) and `M` undirected roads between pairs of cities. Write a C++ function that, given `N`, `M`, and a vector of road pairs `(a, b)`, returns a boolean indicating whether it is possible to decompose the set of roads into disjoint simple cycles covering every road exactly once. Equivalently, every city must have an even degree (each city must be incident to an even number of roads). The function should return `true` if such a decomposition exists, and `false` otherwise.
The problem reduces to checking whether every vertex in the undirected graph has an even degree. This is a classic necessary and sufficient condition for an undirected multigraph to have an Eulerian cycle covering every edge once; such a graph can always be decomposed into edge-disjoint cycles. The algorithm simply counts the degree of each city by incrementing a counter for each endpoint of every road. After processing all roads, we check if any degree is odd. If any odd degree is found, return `false`; otherwise, return `true`. Edge cases include graphs with zero roads (`M=0`), where all degrees are 0 (even), so the answer is `true`; isolated vertices (degree 0) also cause no issue. We can use a `vector<int>` of size `N+1` (0‑based could be used too) to store degrees. Time complexity is `O(N + M)` because we allocate a vector of size `N+1` and process `M` roads. Space complexity is `O(N)` for the degree array.
#include <vector>

// Checks whether all vertices have even degree, meaning the road set can be
// decomposed into disjoint cycles covering every road exactly once.
bool canDecomposeIntoCycles(int cityCount, int roadCount,
                            const std::vector<std::pair<int,int>>& roads) {
    // Degree array indexed by city number (1-based).
    std::vector<int> degree(cityCount + 1, 0);

    // Count the degree of each city.
    for (const auto& road : roads) {
        int a = road.first;
        int b = road.second;
        if (a < 1 || a > cityCount || b < 1 || b > cityCount) {
            // Malformed road; treat as invalid.
            return false;
        }
        ++degree[a];
        ++degree[b];
    }

    // A decomposition into cycles exists iff every degree is even.
    for (int city = 1; city <= cityCount; ++city) {
        if (degree[city] % 2 != 0) {
            return false;
        }
    }
    return true;
}
#include <cassert>
#include <vector>

// Declaration of the function being tested.
bool canDecomposeIntoCycles(int cityCount, int roadCount,
                            const std::vector<std::pair<int,int>>& roads);

int main() {
    // Example 1: 4 cities, 3 roads forming a triangle plus an extra edge? 
    // Actually 3 roads: (1,2), (2,3), (3,1) -> all degrees even -> true.
    std::vector<std::pair<int,int>> roads1 = {{1,2},{2,3},{3,1}};
    assert(canDecomposeIntoCycles(4, 3, roads1) == true);

    // Example 2: 4 cities, 4 roads forming a path with an extra edge? 
    // Roads: (1,2),(2,3),(3,4),(4,1) -> cycle, all even -> true.
    std::vector<std::pair<int,int>> roads2 = {{1,2},{2,3},{3,4},{4,1}};
    assert(canDecomposeIntoCycles(4, 4, roads2) == true);

    // Example 3: A star with center 1 and three leaves 2,3,4 -> center degree 3 (odd), leaves odd -> false.
    std::vector<std::pair<int,int>> roads3 = {{1,2},{1,3},{1,4}};
    assert(canDecomposeIntoCycles(4, 3, roads3) == false);

    // Example 4: No roads -> all degrees 0 (even) -> true.
    std::vector<std::pair<int,int>> roads4 = {};
    assert(canDecomposeIntoCycles(5, 0, roads4) == true);

    // Example 5: Two disjoint edges (1,2) and (3,4) -> each degree 1 (odd) -> false.
    std::vector<std::pair<int,int>> roads5 = {{1,2},{3,4}};
    assert(canDecomposeIntoCycles(4, 2, roads5) == false);

    // Example 6: A plus shape: edges (1,2),(2,3),(3,4),(4,1) and also (2,4)? 
    // That gives degrees: 1:2,2:3,3:2,4:3 -> odd at 2 and 4 -> false.
    std::vector<std::pair<int,int>> roads6 = {{1,2},{2,3},{3,4},{4,1},{2,4}};
    assert(canDecomposeIntoCycles(4, 5, roads6) == false);

    // Example 7: Two overlapping triangles sharing vertex 1: (1,2),(2,3),(3,1),(1,4),(4,5),(5,1)
    // Vertex 1 has degree 4, others degree 2 -> all even -> true.
    std::vector<std::pair<int,int>> roads7 = {{1,2},{2,3},{3,1},{1,4},{4,5},{5,1}};
    assert(canDecomposeIntoCycles(5, 6, roads7) == true);

    // Example 8: Isolated vertex plus a cycle: N=3, road (1,2) only? Actually needs at least 2.
    // Vertex 1 and 2 degree 1 (odd) -> false.
    std::vector<std::pair<int,int>> roads8 = {{1,2}};
    assert(canDecomposeIntoCycles(3, 1, roads8) == false);

    // Example 9: Self-loop? Not allowed in simple road pair, but if given (1,1) it increases degree by 2 each, but our code adds twice to same city -> becomes 2 (even) -> true.
    std::vector<std::pair<int,int>> roads9 = {{1,1}};
    assert(canDecomposeIntoCycles(1, 1, roads9) == true);

    // Example 10: Invalid road city number should fail.
    std::vector<std::pair<int,int>> roads10 = {{0,2},{2,1}};
    assert(canDecomposeIntoCycles(3, 2, roads10) == false);

    return 0;
}
