/*
Write a C++ function `bfsLevels` that, given a directed graph represented as an edge list (vectors of source vertices, destination vertices, and a source vertex `start`), returns a vector of integers where each index `i` holds the shortest path distance (in number of edges) from `start` to vertex `i`. Vertices are numbered from 0 to `vertexCount-1`. If a vertex is unreachable from `start`, its distance should remain as `INT32_MAX`. The input graph may contain self-loops, parallel edges, and vertices with no outgoing or incoming edges. The function must process the graph using a level-by-level BFS (also known as a frontier‑based BFS) where in each level the algorithm expands only from vertices that were newly discovered in the previous level, and it must return the distance vector after the BFS completes.
*/
#include <vector>
#include <cstdint>
#include <limits>
#include <algorithm>

/**
 * Performs a level‑synchronous BFS on a directed graph and returns the shortest
 * distance (in edges) from `start` to every vertex.
 *
 * @param edgeSrc     Source vertex of each directed edge.
 * @param edgeDst     Destination vertex of each directed edge.
 * @param vertexCount Total number of vertices (labeled 0..vertexCount-1).
 * @param start       The source vertex from which distances are computed.
 * @return A vector `dist` where dist[v] = shortest distance from start to v,
 *         or INT32_MAX if v is unreachable.
 */
std::vector<int> bfsLevels(const std::vector<int>& edgeSrc,
                           const std::vector<int>& edgeDst,
                           int vertexCount,
                           int start) {
    // Distances initialized to "infinity"
    std::vector<int> dist(vertexCount, INT32_MAX);
    // Frontier: vertices whose neighbors will be examined in the next level
    std::vector<int> active(vertexCount, 0);
    
    dist[start] = 0;
    active[start] = 1;
    int activeCount = 1;
    int currentLevel = 1;  // next level distance to assign

    while (activeCount > 0) {
        std::vector<int> nextActive(vertexCount, 0);
        int nextCount = 0;

        // Examine all edges whose source is in the current frontier
        for (size_t i = 0; i < edgeSrc.size(); ++i) {
            int src = edgeSrc[i];
            if (active[src]) {  // only expand from active vertices
                int dst = edgeDst[i];
                if (dist[dst] == INT32_MAX) {
                    dist[dst] = currentLevel;
                    if (nextActive[dst] == 0) {
                        nextActive[dst] = 1;
                        ++nextCount;
                    }
                }
            }
        }

        // Move to next level
        active = std::move(nextActive);
        activeCount = nextCount;
        ++currentLevel;
    }

    return dist;
}
#include <cassert>
#include <vector>
#include <cstdint>

// Function declaration (mirror of the solution)
std::vector<int> bfsLevels(const std::vector<int>& edgeSrc,
                           const std::vector<int>& edgeDst,
                           int vertexCount,
                           int start);

int main() {
    // Test 1: Simple chain 0->1->2, start=0
    {
        std::vector<int> src = {0, 1};
        std::vector<int> dst = {1, 2};
        std::vector<int> result = bfsLevels(src, dst, 3, 0);
        assert(result[0] == 0);
        assert(result[1] == 1);
        assert(result[2] == 2);
        assert(result.size() == 3);
    }

    // Test 2: Disconnected vertex, start=0 in graph 0->1, vertex 2 isolated
    {
        std::vector<int> src = {0};
        std::vector<int> dst = {1};
        std::vector<int> result = bfsLevels(src, dst, 3, 0);
        assert(result[0] == 0);
        assert(result[1] == 1);
        assert(result[2] == INT32_MAX);
    }

    // Test 3: Self‑loop and parallel edges, start=0
    {
        std::vector<int> src = {0, 0, 0, 1};
        std::vector<int> dst = {0, 1, 1, 2};
        std::vector<int> result = bfsLevels(src, dst, 3, 0);
        assert(result[0] == 0);
        assert(result[1] == 1);
        assert(result[2] == 2);
    }

    // Test 4: Start vertex with no outgoing edges
    {
        std::vector<int> src = {1, 2};
        std::vector<int> dst = {2, 3};
        std::vector<int> result = bfsLevels(src, dst, 4, 0);
        assert(result[0] == 0);
        assert(result[1] == INT32_MAX);
        assert(result[2] == INT32_MAX);
        assert(result[3] == INT32_MAX);
    }

    // Test 5: Two disconnected components, start in one of them
    {
        // Component A: 0-1, Component B: 2-3 (directed 2->3)
        std::vector<int> src = {0, 2};
        std::vector<int> dst = {1, 3};
        std::vector<int> result = bfsLevels(src, dst, 4, 0);
        assert(result[0] == 0);
        assert(result[1] == 1);
        assert(result[2] == INT32_MAX);
        assert(result[3] == INT32_MAX);
    }

    // Test 6: Larger graph with branching
    {
        // 0->1, 0->2, 1->3, 2->3, 3->4
        std::vector<int> src = {0, 0, 1, 2, 3};
        std::vector<int> dst = {1, 2, 3, 3, 4};
        std::vector<int> result = bfsLevels(src, dst, 5, 0);
        assert(result[0] == 0);
        assert(result[1] == 1);
        assert(result[2] == 1);
        assert(result[3] == 2);
        assert(result[4] == 3);
    }

    // Test 7: Cycle, ensure distances are correct
    {
        // 0->1, 1->2, 2->0
        std::vector<int> src = {0, 1, 2};
        std::vector<int> dst = {1, 2, 0};
        std::vector<int> result = bfsLevels(src, dst, 3, 0);
        assert(result[0] == 0);
        assert(result[1] == 1);
        assert(result[2] == 2);
    }

    return 0;
}
// The solution uses a classic breadth‑first search over the graph, but implemented in a level‑synchronous manner to mirror the style of the FPGA code in the original snippet. We maintain:  
// - `distance[vertex]` = current known shortest distance from `start` (initialized to `INT32_MAX`).  
// - `active` vector marking which vertices are part of the current frontier (those discovered in the previous level).  
// - `activeCount` = number of vertices in the current frontier; the BFS continues while `activeCount > 0`.  
//
// In each iteration, we traverse only the edges whose source vertex is active. For each such edge `(src, dst)`, if `distance[dst]` is still `INT32_MAX`, we set it to `currentLevel` (where `currentLevel` starts at 1 for the first expansion from `start`) and mark `dst` as part of the next frontier. After processing all edges of the current frontier, we increment `currentLevel`, swap the frontiers, and recompute `activeCount`. This is exactly the outer loop structure from the FPGA engine: `divideGraphByEdge`, `MSGGenMerge`, `MSGApply`, and `Gather`. The algorithm correctly handles self‑loops (because checking `distance[dst]` prevents revisiting), parallel edges (they are just redundant but harmless), and vertices that are unreachable (they remain at `INT32_MAX`). The time complexity is \(O(V + E)\) where \(V\) is the number of vertices and \(E\) the number of edges, because each edge is examined at most once. Space complexity is \(O(V + E)\) for storing the edge lists and distance/active vectors.
