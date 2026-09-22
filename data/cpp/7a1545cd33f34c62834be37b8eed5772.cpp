// Write a standalone C++ function named `countElementaryCycles` that takes a directed graph represented as an adjacency list (using `std::vector<std::vector<int>>`, where the outer vector index is the source vertex and the inner vector holds the destination vertices of outgoing edges) and returns the number of elementary cycles (simple cycles with no repeated vertices except the starting/ending vertex) of length at least 3 in the graph. The function must handle graphs with vertices numbered from 0 to n-1, may contain self-loops and parallel edges (which should be ignored for cycle counting—treat each edge as a distinct possibility but a cycle consisting of a self-loop is length 1 and should not be counted; parallel edges between two distinct vertices form a cycle of length 2 and should also not be counted), and must correctly count each distinct elementary cycle exactly once regardless of the starting vertex or traversal direction. The function signature must be `int countElementaryCycles(const std::vector<std::vector<int>>& adj)`. The algorithm must use a depth-first search with a visited stack, and must produce correct results for graphs with up to 15 vertices (i.e., small enough for exponential worst-case time but within typical limits). Provide a reference solution and test code.

// The solution uses a recursive depth-first search (DFS) that explores all simple paths. Since the graph is small (≤15 vertices), we can use a bitmask to track visited vertices, and a path stack to reconstruct cycles. The main approach: for each starting vertex `v` (from 0 to n-1), we begin a DFS from `v` only considering neighbors with index greater than `v` to break symmetry and ensure each cycle is counted once (this avoids counting rotations and reversals). During DFS, we maintain a `visited` bitmask and the current path as a vector of vertices. When we return to the starting vertex `v` from a neighbor, we check if the path length (number of vertices in the path before adding the neighbor) is at least 2 (so total cycle length ≥3), and if so, increment the count. Since we only go to neighbors with index greater than the start, we avoid duplicates. Self-loops (edge to itself) generate a cycle of length 1, so we skip immediately. Parallel edges: when exploring from a vertex, we may encounter the same destination multiple times; but since we are counting cycles, we must be careful not to double-count the same cycle multiple times. The DFS processes each distinct edge leading to an unvisited vertex as part of a path; when we return to the start, we count once per cycle occurrence from that path. For parallel edges that form a 2-cycle (v->u and u->v, also with parallel edges), since we only start from smaller index and go to larger neighbors, a 2-cycle is only detected when we are at the start `v` and neighbor `u` and then back to `v`; but the path length of the existing path is just one vertex (v), so we require path length >=2 before adding neighbor; thus 2-cycles are excluded. Also, because we only allow neighbors with index > start, we never traverse from `u` back to `v` if u > v? Actually if u > v, then when we are at u, its neighbors with index > u are considered, so v (which is smaller) is not considered; thus a 2-cycle is not counted. For longer cycles, each cycle has a unique smallest vertex; starting from that smallest vertex and only following neighbors with index greater than the smallest vertex, we traverse the cycle in one specific direction (the direction that goes through increasing indices first). However, a cycle can have multiple smallest vertices? No, a unique smallest vertex. But we must also ensure that we don't traverse the cycle in both directions. Since we only follow neighbors with index > smallest vertex, the path is forced to go from the smallest vertex to its larger neighbor, etc., and eventually returns to the smallest vertex, so direction is unique. So each cycle is counted exactly once. Edge cases: self-loops (ignore), 2-cycles (ignored by requiring path length >=2 before returning to start), multiple edges (each edge is considered separately, but the DFS path is based on vertices; if there are parallel edges, they might cause multiple identical cycles? Actually a cycle is defined by sequence of vertices, so parallel edges between the same pair of vertices lead to the same vertex sequence; we would count that cycle multiple times if we process each edge separately. But since we traverse vertices, from a vertex we have a list of neighbors; if there are duplicate neighbors (parallel edges), the DFS will branch into multiple identical recursive calls, leading to duplicate cycles. To avoid that, we must deduplicate neighbors (unique vertices) before recursing. In the solution, we can use a `std::set` or sort and unique on the neighbor list, or use a visited check per neighbor per call? Actually when exploring from a vertex, we loop over its adjacency list; if there are duplicate target vertices, we would call DFS multiple times with same target, leading to duplicate cycles. So we must deduplicate by visiting each distinct neighbor only once. We can do this by copying the adjacency list for that vertex, sorting and removing duplicates, or using a `std::unordered_set` on the fly. In the solution, we'll use a local `std::set<int>` for each function call to deduplicate neighbors. Time complexity: worst-case exponential, O(V!) for complete graph, but V ≤15 so acceptable. Space: O(V) for recursion stack and path, plus O(V) for visited bitmask.

#include <vector>
#include <algorithm>
#include <unordered_set>

// Count elementary cycles (simple cycles of length >= 3) in a directed graph.
// Graph represented as adjacency list: adj[v] = list of destinations from v.
// Vertices are 0..n-1. Self-loops and 2-cycles are ignored.
// Each cycle is counted once, regardless of starting vertex or direction.
int countElementaryCycles(const std::vector<std::vector<int>>& adj) {
    const int n = static_cast<int>(adj.size());
    if (n < 3) return 0;

    int cycle_count = 0;

    // DFS recursion: start is the smallest vertex in the cycle (the origin).
    // path: current sequence of vertices (excluding the start, which is not stored explicitly).
    // visited: bitmask of vertices already on the path (excluding start).
    // current: last vertex in the path (we are about to expand from it).
    auto dfs = [&](int start, int current, unsigned int visited, const std::vector<int>& path, auto&& self) -> void {
        // For each neighbor of 'current' that is greater than 'start' (to break symmetry)
        // and not already visited, try to extend the path.
        // Use a set to avoid processing parallel edges to the same vertex (deduplicate).
        std::unordered_set<int> unique_neighbors;
        for (int nb : adj[current]) {
            if (nb > start && nb != current) { // skip self-loop and vertices <= start
                unique_neighbors.insert(nb);
            }
        }

        for (int nb : unique_neighbors) {
            if (nb == start) {
                // We have returned to the start, forming a cycle.
                // Path length must be at least 2 (so total cycle length >= 3).
                // path.size() is number of vertices after start, so we need path.size() >= 2.
                if (path.size() >= 2) {
                    ++cycle_count;
                }
            } else if (!(visited & (1u << nb))) {
                // Extend path.
                std::vector<int> new_path = path;
                new_path.push_back(nb);
                self(start, nb, visited | (1u << nb), new_path, self);
            }
        }
    };

    for (int start = 0; start < n; ++start) {
        // For each start vertex, we only consider neighbors with index > start.
        // The initial path is empty (just start).
        // Use a visited bitmask with 'start' already marked? Not needed because we never go back to start except to close a cycle.
        // We'll mark start as visited in the sense that we don't revisit it via 'visited' mask? Actually we never check 'start' in visited; we treat it specially when we see nb == start.
        // For the DFS call, visited bitmask should not include 'start' itself, because we allow returning to it.
        // We'll pass visited = 0 initially.
        std::vector<int> empty_path;
        dfs(start, start, 0u, empty_path, dfs);
    }

    return cycle_count;
}

#include <cassert>
#include <vector>

// Declaration from solution (not needed if compiled with the solution file)
int countElementaryCycles(const std::vector<std::vector<int>>& adj);

int main() {
    // Test 1: Empty graph
    std::vector<std::vector<int>> g1;
    assert(countElementaryCycles(g1) == 0);

    // Test 2: Single vertex with self-loop -> no cycles of length >=3
    std::vector<std::vector<int>> g2 = {{0}};
    assert(countElementaryCycles(g2) == 0);

    // Test 3: Two vertices with a 2-cycle (edges 0->1 and 1->0) -> no cycles
    std::vector<std::vector<int>> g3 = {{1}, {0}};
    assert(countElementaryCycles(g3) == 0);

    // Test 4: Simple triangle 0->1, 1->2, 2->0
    std::vector<std::vector<int>> g4 = {{1}, {2}, {0}};
    assert(countElementaryCycles(g4) == 1);

    // Test 5: Triangle with parallel edges (0->1 twice, 1->2, 2->0) still 1 cycle
    std::vector<std::vector<int>> g5 = {{1,1}, {2}, {0}};
    assert(countElementaryCycles(g5) == 1);

    // Test 6: Complete graph on 4 vertices: number of cycles of length 3 = 4, length 4 = 3 (or 6? Let's compute: each triangle from C(4,3)=4; each 4-cycle has 4 vertices, number of distinct cycles = (4-1)!/2 = 3? Actually total cycles of length 4 in directed complete graph: Each permutation of 4 vertices gives a cycle, but rotations and reversals reduce count: (4-1)!/2 = 3. So total = 4+3=7.
    std::vector<std::vector<int>> g6(4);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i != j) g6[i].push_back(j);
        }
    }
    assert(countElementaryCycles(g6) == 7);

    // Test 7: Disconnected graph with a triangle and a 2-cycle
    std::vector<std::vector<int>> g7 = {
        {1},       // 0 -> 1
        {2},       // 1 -> 2
        {0},       // 2 -> 0
        {4},       // 3 -> 4
        {3}        // 4 -> 3
    };
    assert(countElementaryCycles(g7) == 1);

    // Test 8: Graph with a cycle of length 4 that is not a simple path? e.g., 0->1,1->2,2->3,3->0, plus 0->2 (chord) -> includes cycles of length 3 and 4
    std::vector<std::vector<int>> g8 = {
        {1,2}, // 0 -> 1, 2
        {2},   // 1 -> 2
        {3},   // 2 -> 3
        {0}    // 3 -> 0
    };
    // Cycles: 0-1-2-0 (length 3), 0-2-3-0 (length 3), 0-1-2-3-0 (length 4) total 3.
    assert(countElementaryCycles(g8) == 3);

    // Test 9: Cycle with self-loop inside: 0->0, 0->1, 1->0 (but 1->0 gives 2-cycle, self-loop ignored) -> only 2-cycle, so 0 cycles of length >=3
    std::vector<std::vector<int>> g9 = {{0,1}, {0}};
    assert(countElementaryCycles(g9) == 0);

    // Test 10: Larger cycle: 5 vertices in a directed cycle
    std::vector<std::vector<int>> g10(5);
    for (int i = 0; i < 5; ++i) {
        g10[i].push_back((i+1)%5);
    }
    assert(countElementaryCycles(g10) == 1);

    return 0;
}
