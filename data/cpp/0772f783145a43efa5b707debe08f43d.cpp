// Given a connected undirected tree with `n` vertices numbered from 1 to `n`, and a set of `k` initially occupied vertices (each provided as input), write a C++ function `removeRedundantEdges` that simulates a multi-source BFS starting from all occupied vertices simultaneously. In this BFS, each vertex is labeled with the distance (in edges) to the nearest initially occupied vertex, with ties broken arbitrarily but consistently by the BFS order. During the BFS, if an edge connects two vertices that are both already visited, and the edge is not the tree edge used to discover the current vertex, then that edge is considered "redundant" and must be reported. The function takes the number of vertices `n`, the vector of initial occupied vertices `occupied`, and a vector of edges (each edge given as a pair of endpoints), and returns a vector of the indices (1-based, in the order they were provided in the input) of all redundant edges. If no edge is redundant, return an empty vector. The BFS discovery order must follow a standard queue: start by pushing all occupied vertices in the order they appear in `occupied` (with each having a parent of 0), then process the queue in FIFO order, exploring neighbors in the order they appear in the adjacency list, which should be built by iterating over the provided edge list in order.

// The problem is essentially a multi-source BFS on a tree (graph with no cycles) but with the twist that redundant edges are detected when a BFS tries to traverse an edge to an already visited vertex that is not the parent of the current vertex. In a tree, the total number of edges is `n-1`. A standard BFS from multiple sources will visit every vertex exactly once, and the BFS tree will contain exactly `n-1` edges. However, the input graph is a tree, so there are exactly `n-1` edges total. Therefore, every edge must be either a tree edge (used for discovery) or a redundant edge. In fact, the BFS will always discover every vertex exactly once, and during the processing of each vertex's neighbors, the edge to the parent will be skipped because the neighbor is already visited and equals the parent. For any other already visited neighbor (which cannot happen in a tree unless the graph had a cycle, because a tree has no cycles), but the original code's logic treats any already visited neighbor that is not the parent as redundant. Since the given graph is a tree, no cycles exist, so in a correct tree, the set of redundant edges should be empty. However, in the provided snippet, the code uses a map keyed by `(min(u,v)*1000000 + max(u,v))` to retrieve edge indices, and it counts redundant edges when `visited[i]` is true and `i != y` (where `y` is the parent of the current vertex). To replicate the exact behavior, we must follow the same logic: for each vertex popped from the queue, for each neighbor, if not visited, mark visited and push, else if visited and neighbor is not the parent (i.e., the vertex that caused the current vertex to be discovered), treat that edge as redundant and record its index. We must store edge indices in a map keyed by the unordered pair. The main algorithm is: build adjacency lists with edge index information, initialize visited array (0 means unvisited, 1 for initially occupied, and later set to a positive distance value), push all occupied vertices with parent 0 into a queue, process BFS. When a neighbor is already visited and is not the parent, record the edge index. Since the graph is a tree, this should not happen, but the function must handle it as per the snippet. The time complexity is O(n + k) because each vertex and edge is processed once; space complexity is O(n) for visited, adjacency, and queue.

#include <vector>
#include <queue>
#include <unordered_map>
#include <utility>
#include <algorithm>

/**
 * Simulates multi-source BFS on a tree and returns indices of redundant edges.
 *
 * @param n Number of vertices (1-indexed).
 * @param occupied Vector of initially occupied vertex labels (1..n).
 * @param edges Vector of pairs (u,v) representing tree edges, 1-indexed.
 * @return Vector of 1-based edge indices that are redundant (empty if none).
 */
std::vector<int> removeRedundantEdges(int n, const std::vector<int>& occupied, const std::vector<std::pair<int,int>>& edges) {
    // Build adjacency: each entry is a pair (neighbor, edge_index)
    std::vector<std::vector<std::pair<int,int>>> adj(n + 1);
    for (int idx = 0; idx < (int)edges.size(); ++idx) {
        int u = edges[idx].first;
        int v = edges[idx].second;
        adj[u].push_back({v, idx + 1});
        adj[v].push_back({u, idx + 1});
    }

    // visited[v] = 0 if unvisited, otherwise stores BFS distance (1 for initial occupied)
    std::vector<int> visited(n + 1, 0);
    // queue of (vertex, parent_vertex); parent 0 means no parent (initial source)
    std::queue<std::pair<int,int>> qu;

    // Initialize BFS with all occupied vertices in given order
    for (int v : occupied) {
        if (visited[v] == 0) {
            visited[v] = 1;
            qu.push({v, 0});
        }
    }

    std::vector<int> redundant;
    while (!qu.empty()) {
        int x = qu.front().first;
        int parent = qu.front().second;
        qu.pop();

        // Explore neighbors in adjacency order
        for (const auto& [neighbor, edge_id] : adj[x]) {
            if (visited[neighbor] == 0) {
                visited[neighbor] = visited[x] + 1;
                qu.push({neighbor, x});
            } else if (neighbor != parent) {
                // Already visited and not the parent -> redundant edge
                redundant.push_back(edge_id);
            }
        }
    }

    return redundant;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above; here we test it.

int main() {
    // Test 1: Simple tree with one occupied vertex, no redundant edges.
    {
        std::vector<int> occupied = {1};
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4}};
        auto res = removeRedundantEdges(4, occupied, edges);
        assert(res.empty());
    }

    // Test 2: Two occupied vertices, tree is a straight line, should be no redundant.
    {
        std::vector<int> occupied = {2, 3};
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4}};
        auto res = removeRedundantEdges(4, occupied, edges);
        assert(res.empty());
    }

    // Test 3: All vertices occupied, every edge connects two initial sources -> all edges are redundant.
    {
        std::vector<int> occupied = {1, 2, 3};
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3}};
        auto res = removeRedundantEdges(3, occupied, edges);
        // BFS starts with all vertices visited, then when processing vertex 1, edge to 2 is redundant (parent=0, neighbor=2).
        // When processing vertex 2, edge to 1 is redundant (parent=0, neighbor=1) and edge to 3 is redundant.
        // When processing vertex 3, edge to 2 is redundant.
        // The order depends on queue order: push 1, 2, 3. Pop 1 -> neighbor 2 visited, not parent -> record edge 1.
        // Pop 2 -> neighbor 1 visited, not parent -> record edge 1 again? Wait, we need to avoid duplicates? The snippet records duplicates.
        // Actually the snippet's logic would record each redundant edge once per adjacency direction, so duplicates appear.
        // To match the reference exactly, we should expect duplicates. But our test should reflect this.
        // For simplicity, let's just assert that the size is 4? Actually processing:
        // Pop 1 (parent 0): neighbor 2 visited -> record edge 1.
        // Pop 2 (parent 0): neighbors: 1 visited -> record edge 1 again; 3 visited -> record edge 2.
        // Pop 3 (parent 0): neighbor 2 visited -> record edge 2.
        // So redundant = [1,1,2,2] (order as recorded). But edges indices: 1 and 2 provided. So expect 4 entries.
        auto res = removeRedundantEdges(3, occupied, edges);
        assert(res.size() == 4);
        assert(res[0] == 1 && res[1] == 1 && res[2] == 2 && res[3] == 2);
    }

    // Test 4: Single vertex tree (no edges) with one occupied vertex.
    {
        std::vector<int> occupied = {1};
        std::vector<std::pair<int,int>> edges;
        auto res = removeRedundantEdges(1, occupied, edges);
        assert(res.empty());
    }

    // Test 5: Tree where a vertex has multiple initial sources? Not possible because occupied set is unique.
    // Test with duplicates in occupied: we should avoid duplicates by using visited check.
    {
        std::vector<int> occupied = {1, 1};
        std::vector<std::pair<int,int>> edges = {{1,2}};
        auto res = removeRedundantEdges(2, occupied, edges);
        // Only one initial push, no redundant edges.
        assert(res.empty());
    }

    // Test 6: A more complex tree where a redundant edge might appear? In a tree, never. But test star with center occupied.
    {
        std::vector<int> occupied = {1};
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{1,4}};
        auto res = removeRedundantEdges(4, occupied, edges);
        assert(res.empty());
    }

    // Test 7: Star with leaves occupied, center not occupied -> redundant edge? No, BFS will visit center from first leaf, then others will see center visited but parent is not leaf? Actually each leaf's neighbor is center, and center is visited by first leaf, so for second leaf, neighbor center is visited but parent for that leaf is 0, so it records redundant. So expect 2 redundant edges.
    {
        std::vector<int> occupied = {2, 3, 4};
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{1,4}};
        auto res = removeRedundantEdges(4, occupied, edges);
        // BFS: push 2,3,4. Pop 2 -> neighbor 1 unvisited -> mark visited, push (1,2). Pop 3 -> neighbor 1 already visited, parent is 0 -> redundant edge (1,3) index 2. Pop 4 -> neighbor 1 already visited, parent 0 -> redundant edge (1,4) index 3. Pop 1 -> neighbors: 2 (visited, parent is 2 -> not redundant), 3 (visited, parent is 2? no, parent is 2 but neighbor 3 is not parent -> redundant edge (1,3) index 2 again), 4 (visited, not parent -> redundant edge index 3).
        // So redundant = [2,3,2,3] in that order.
        auto res2 = removeRedundantEdges(4, occupied, edges);
        assert(res2.size() == 4);
        assert(res2[0] == 2 && res2[1] == 3 && res2[2] == 2 && res2[3] == 3);
    }

    // Test 8: Duplicate edge indices? Not needed.

    return 0;
}
