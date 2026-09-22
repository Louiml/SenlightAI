// Write a standalone C++ function that, given a vector of integers representing the DFS discovery times (DFI) of nodes in a graph, returns a vector of integers representing the "lowpoint" values. The lowpoint of a node is defined as the minimum DFI reachable from that node via zero or more tree edges (DFS child edges) followed by at most one back edge (or the node's own DFI if no such path exists). Assume the input vector `dfi` is already computed for a connected undirected graph with `n` nodes (nodes numbered 0 to n-1), and that `dfi` is a permutation of 0..n-1. You are also given an adjacency list `adj` where `adj[v]` is a vector of neighbor node IDs (undirected edges, no self-loops). The function must compute lowpoints for all nodes using a DFS traversal that follows the order of nodes as given by increasing DFI. That is, start DFS from the node with DFI 0, and when exploring from a node, iterate its neighbors in the order they appear in the adjacency list, skipping those already visited (which are treated as back edges). The output vector `low` should have `low[v]` = minimum DFI reachable. Must handle disconnected graphs: for each unvisited node (in increasing DFI order), start a new DFS root with lowpoint equal to its own DFI. The function signature: `std::vector<int> computeLowPoints(const std::vector<int>& dfi, const std::vector<std::vector<int>>& adj)`. The function must be const-correct, use only standard headers, and have linear time complexity in terms of total edges and nodes.
// The key insight is that a lowpoint computation is a standard DFS-based algorithm. We need to perform a DFS traversal where the order of visiting nodes is dictated by the given `dfi` array. We treat `dfi` as a mapping from node ID to discovery time. We maintain a `visited` boolean array (or use DFI comparison) to separate tree edges from back edges. The algorithm: iterate over nodes in order of increasing DFI (i.e., from 0 to n-1 in the dfi array's index space? Actually we need to map dfi values back to node IDs). Since `dfi` is a permutation, we can create an array `nodeFromDFI` where `nodeFromDFI[dfi[v]] = v`. Then iterate `d` from 0 to n-1, take node `v = nodeFromDFI[d]`, if not visited, start a DFS from it. During DFS, for each neighbor `w` of `v`: if `dfi[w]` is greater than `dfi[v]` and not visited, then it's a tree child; recurse, and update `low[v] = min(low[v], low[w])`. If `dfi[w] < dfi[v]` and `w` is not the parent (to avoid treating the parent edge as a back edge), then it's a back edge to an ancestor (since graph is undirected and we are doing DFS in DFI order), so update `low[v] = min(low[v], dfi[w])`. If `dfi[w]` is greater than `dfi[v]` but already visited (cross edge), we ignore it because in undirected DFS with DFI ordering, such edges are either forward edges (but we already processed) or back edges from descendant? Actually careful: In undirected graph, any edge to a visited node with higher DFI is either a back edge from a descendant (already handled) or a forward edge that we skipped; but since we iterate neighbors in order, we might encounter a neighbor that has already been visited via another path; but since the graph is undirected, if neighbor has higher DFI and is visited, it must be a descendant (because if it had lower DFI, it would be an ancestor). So we can simply treat all neighbors with `dfi[w] > dfi[v]` as children (if not visited) or skip if visited (since they are already visited via another route, and their lowpoint is already computed? But careful: In DFS, when you encounter a visited node with higher DFI, it means it's a back edge from a descendant to current node in the DFS tree? Actually no: In undirected DFS, the only edges are tree edges and back edges to ancestors. A neighbor with higher DFI that is already visited must be a descendant that was visited via a different path? But that would create a cycle and it would have been visited earlier? The standard algorithm for lowpoint (for articulation points) uses exactly this: if neighbor is not parent and `dfi[w] < dfi[v]`, then it's a back edge; if `dfi[w] > dfi[v]`, it's a tree edge (child). Because in DFS of undirected graph, there are no cross edges; every edge connects an ancestor and a descendant in the DFS tree. So we don't need a visited check for higher DFI; we can just treat all neighbors with higher DFI as children (and recursively call them if not yet processed). But to avoid infinite recursion, we need to know which nodes have been fully processed. Since we iterate nodes in increasing DFI, we can ensure that when we start DFS from a node, all nodes with lower DFI have already been processed (or we might start from a new root). Actually the simplest approach: Perform a standard iterative or recursive DFS using a `visited` boolean array. When at node `v`, for each neighbor `w`: if `dfi[w] < dfi[v]` and `w != parent`, then update low[v] with dfi[w]; if `dfi[w] > dfi[v]` and not visited, recurse; if `dfi[w] > dfi[v]` and visited, ignore (or update low[v] with low[w]? But that would be incorrect because low[w] is already finalized, but since it's a descendant not yet finished? Actually in simple lowpoint calculation, you only use children's low and back edges to ancestors. So we can do: if w is unvisited, recurse and then low[v] = min(low[v], low[w]); else if w != parent and dfi[w] < dfi[v], low[v] = min(low[v], dfi[w]); else if w != parent and dfi[w] > dfi[v] and visited, we can ignore because it's a descendant already visited through another path? But in undirected graph, that cannot happen if we start DFS from the node with smallest DFI and only traverse in increasing DFI order? Actually if we follow adjacency order, it's possible to encounter a visited neighbor with higher DFI that is not a child but a back edge from a descendant? Let's think: In a standard DFS, when we are at v, we visit all unvisited neighbors. If a neighbor w has higher DFI and is already visited, that means w was visited via some other path, and since w has higher DFI, that path must have started from a descendant of v? But if w is already visited, then there is already an edge from some ancestor to w; but w's DFI is higher, so it must be a descendant of v in the DFS tree? Actually in undirected DFS, the DFS tree has the property that every non-tree edge connects a node to an ancestor. So when you are at v, any neighbor w with dfi[w] > dfi[v] that is already visited must be a descendant of v (since it was visited after v). But if it's already visited, it means it was visited via some other child of v, but that's fine; it's still a descendant, and the edge (v,w) is a back edge from w to v. However, in lowpoint calculation, we don't need to process that edge because the lowpoint of v will get contributions from the child that leads to w already. So we can safely ignore visited neighbors with higher DFI. For correctness, we just need to avoid recursion loops. The simplest robust implementation: Use recursion with a `visited` array. For each node in order of increasing DFI (i.e., iterate d from 0 to n-1, v=nodeFromDFI[d]), if not visited, call a helper function that performs DFS. In helper: set visited[v]=true, low[v]=dfi[v]; then for each neighbor w: if w==parent continue; if !visited[w] and dfi[w] > dfi[v] (or simply if !visited[w] because in undirected graph if not visited, it's a child; but we must ensure we only go to nodes with higher DFI to respect the given DFI ordering? Actually the given DFI ordering is a valid DFS order from some traversal, but we need to reconstruct the DFS tree based on adjacency order. The standard fact: Given a DFI permutation that is consistent with a DFS, we can determine tree edges by scanning adjacency lists in order: an edge to an unvisited node is a tree edge (that node gets the next DFI). But here we already have DFI, so we need to ensure that we only follow edges to nodes with higher DFI as tree edges, because that matches the original DFS. So we should do: if dfi[w] < dfi[v] and w!=parent, it's a back edge; if dfi[w] > dfi[v] and !visited[w], it's a tree edge and recurse; if dfi[w] > dfi[v] and visited[w], it's a back edge from a descendant? Actually in a valid DFS, when you are at v, all neighbors with higher DFI that are already visited must be descendants visited via a different path, but they are not the parent, and since they are descendants, the edge is a back edge from that descendant to v. But the lowpoint of v should consider the lowpoint of that descendant via the child that reaches it, not via this direct edge. So we can ignore it. However, to be safe and adhere to the definition (lowpoint is min DFI reachable via tree edges then at most one back edge), we can still consider that direct edge as a back edge to v? But v is an ancestor, so that edge is a back edge from the descendant to v, which doesn't help lower v's lowpoint because v's lowpoint is at most dfi[v] anyway. So ignoring is fine. The edge case: parallel edges? The problem statement says no self-loops, but may have parallel edges? It says "undirected edges", implies no multi-edges? Not specified, but we can handle: if there is a parallel edge to parent, that is a back edge to parent, so we treat w==parent but that parallel edge should be considered as a back edge (since parent has lower DFI). So we need to be careful: when w==parent and there is another edge to parent, we should treat it as a back edge. But our adjacency list has simple edges; if multiple edges exist, we might need to distinguish. Since not specified, assume simple graph (no multi-edges). Also handle isolated nodes: lowpoint = its own DFI. Disconnected graphs: each component's root has lowpoint = its own DFI. The DFS order: we must iterate over nodes in increasing DFI order and start DFS from unvisited nodes. That ensures we get a DFS forest consistent with the given DFI. Time complexity: O(n + m) because each edge is examined twice (once from each endpoint) and recursion stack O(n). Space: O(n) for visited, low, and nodeFromDFI.
#include <vector>
#include <algorithm>

// Compute lowpoint values for each node given a valid DFS discovery time (DFI) ordering.
// dfi[v] is the discovery time of node v (0..n-1), a permutation.
// adj[v] is the list of neighbors of v (undirected, no self-loops).
// Returns a vector low where low[v] = minimum DFI reachable via tree edges then at most one back edge.
std::vector<int> computeLowPoints(
    const std::vector<int>& dfi,
    const std::vector<std::vector<int>>& adj
) {
    const int n = static_cast<int>(dfi.size());
    // Map DFI back to node ID.
    std::vector<int> nodeFromDFI(n);
    for (int v = 0; v < n; ++v) {
        nodeFromDFI[dfi[v]] = v;
    }

    std::vector<int> low(n);
    std::vector<bool> visited(n, false);

    // Recursive lambda for DFS traversal.
    // We pass the parent node to avoid treating the parent edge as a back edge.
    std::function<void(int, int)> dfs = [&](int v, int parent) {
        visited[v] = true;
        low[v] = dfi[v]; // at worst, reach itself
        for (int w : adj[v]) {
            if (w == parent) continue;
            if (dfi[w] < dfi[v]) {
                // Back edge to an ancestor.
                low[v] = std::min(low[v], dfi[w]);
            } else if (!visited[w]) {
                // Tree edge to a child (since dfi[w] > dfi[v] and unvisited).
                dfs(w, v);
                low[v] = std::min(low[v], low[w]);
            }
            // If dfi[w] > dfi[v] and visited, ignore (descendant via other path).
        }
    };

    // Process nodes in increasing DFI order to cover all components.
    for (int d = 0; d < n; ++d) {
        int v = nodeFromDFI[d];
        if (!visited[v]) {
            dfs(v, -1);
        }
    }

    return low;
}
#include <cassert>
#include <vector>
#include <functional>

// Include the solution function here (or link it) -- for brevity, assume it's above.

int main() {
    // Test 1: Simple path 0-1-2 with DFI = {0,1,2}
    std::vector<int> dfi1 = {0, 1, 2};
    std::vector<std::vector<int>> adj1 = {{1}, {0,2}, {1}};
    std::vector<int> low1 = computeLowPoints(dfi1, adj1);
    assert((low1 == std::vector<int>{0, 0, 0}));

    // Test 2: Cycle 0-1-2-0, DFI = {0,1,2}
    // Node 0: low=0, node1 has back edge to 0, low=0; node2 back to 0, low=0
    std::vector<int> dfi2 = {0, 1, 2};
    std::vector<std::vector<int>> adj2 = {{1,2}, {0,2}, {0,1}};
    std::vector<int> low2 = computeLowPoints(dfi2, adj2);
    assert((low2 == std::vector<int>{0, 0, 0}));

    // Test 3: Tree with branches: 0-1, 0-2 where 0 has DFI 0, 1 has DFI1, 2 has DFI2
    // Lowpoints: all nodes reach once ancestor (0) via tree, so all 0
    std::vector<int> dfi3 = {0, 1, 2};
    std::vector<std::vector<int>> adj3 = {{1,2}, {0}, {0}};
    std::vector<int> low3 = computeLowPoints(dfi3, adj3);
    assert((low3 == std::vector<int>{0, 0, 0}));

    // Test 4: Disconnected: node0 isolated, node1-2 edge, DFI given as {0,1,2} for nodes 0,1,2
    std::vector<int> dfi4 = {0, 1, 2};
    std::vector<std::vector<int>> adj4 = {{}, {2}, {1}};
    std::vector<int> low4 = computeLowPoints(dfi4, adj4);
    // node0 low=0, node1 low=1 (itself), node2 low=1 (back to node1)
    assert((low4 == std::vector<int>{0, 1, 1}));

    // Test 5: More complex: DFI = [0,1,2,3] for nodes 0,1,2,3; edges: 0-1,1-2,2-0,1-3
    // Node3 leaf attached to 1. Low: node0=0, node1 min(dfi0=0, child low0=0, child low3=3) ->0
    // node2 back to 0 ->0, node3 back to parent 1? Actually node3 only edge to 1, so low=dfi[3]=3
    std::vector<int> dfi5 = {0, 1, 2, 3};
    std::vector<std::vector<int>> adj5 = {{1,2}, {0,2,3}, {0,1}, {1}};
    std::vector<int> low5 = computeLowPoints(dfi5, adj5);
    assert((low5 == std::vector<int>{0, 0, 0, 3}));

    // Test 6: Node with no back edges: star with center 0 (DFI0), leaves 1,2,3 (DFI1,2,3)
    // Leaves have low = their own DFI because only tree edge to parent (no back edge).
    std::vector<int> dfi6 = {0, 1, 2, 3};
    std::vector<std::vector<int>> adj6 = {{1,2,3}, {0}, {0}, {0}};
    std::vector<int> low6 = computeLowPoints(dfi6, adj6);
    assert((low6 == std::vector<int>{0, 1, 2, 3}));

    // Test 7: Single node
    std::vector<int> dfi7 = {0};
    std::vector<std::vector<int>> adj7 = {{}};
    std::vector<int> low7 = computeLowPoints(dfi7, adj7);
    assert((low7 == std::vector<int>{0}));

    // Test 8: Parallel edge? Not in spec, but ensure no crash: two nodes with two parallel edges
    // dfi = {0,1}, adj[0]={1,1}, adj[1]={0,0} (two parallel edges)
    std::vector<int> dfi8 = {0, 1};
    std::vector<std::vector<int>> adj8 = {{1,1}, {0,0}};
    std::vector<int> low8 = computeLowPoints(dfi8, adj8);
    // Node0 low 0, node1 has back edge to parent (0) via parallel edge, so low=0
    assert((low8 == std::vector<int>{0, 0}));
}
