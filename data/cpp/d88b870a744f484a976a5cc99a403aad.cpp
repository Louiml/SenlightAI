// You are given a directed graph with `n` nodes labeled from `0` to `n-1`, represented by an array `edges` of length `n` where `edges[i]` is the destination of the directed edge from node `i`, or `-1` if node `i` has no outgoing edge. Write a C++ function `int closestMeetingNode(const std::vector<int>& edges, int node1, int node2)` that returns the node with the smallest index that is reachable from both `node1` and `node2` (following directed edges) and minimizes the maximum distance from `node1` and `node2` to that node. If no such common reachable node exists, return `-1`. Distances are measured as the number of edges traversed. If multiple nodes achieve the same minimal maximum distance, return the one with the smallest index. The graph may contain cycles, and a node is considered reachable from itself (distance 0). The input indices are valid (0 ≤ node1, node2 < n), and `n` ≥ 1.
// The solution performs a depth-first search (DFS) from each starting node to record the set of nodes reachable from that node and their distances. Since each node has at most one outgoing edge (the graph is a functional graph, but possibly disconnected), a simple recursive or iterative DFS works without needing to handle complex branching—but the standard DFS handles it fine. For `node1`, we compute a distance map (or vector indexed by node) storing the distance from `node1` to each reachable node; similarly for `node2`. Then we iterate over all nodes `0` to `n-1` that appear in both reachable sets. For each such node, compute `max(dist1[node], dist2[node])`, and keep the node with the smallest such value; if tie, pick the smaller node index. Edge cases: a node may be unreachable from either starting node—skip it; if no common node, return `-1`. Because each node has out-degree ≤ 1, DFS visits each node at most once per traversal, so time complexity is O(n) for both traversals plus O(n) for scanning, total O(n). Space is O(n) for visited flags and distance arrays, plus recursion stack depth up to O(n) in the worst case (if the graph is a long chain). The implementation uses iterative or recursive DFS; here we use recursive with a helper, but must ensure no infinite loop in cycles—visited flags prevent that. The distance is the depth in DFS; because each node has exactly one outgoing edge (or none), DFS correctly computes the shortest (and only) path from the source to any reachable node.
#include <vector>
#include <algorithm>
#include <climits>

// Return the node with smallest index that is reachable from both node1 and node2,
// minimizing the maximum distance from either start. Return -1 if none exists.
int closestMeetingNode(const std::vector<int>& edges, int node1, int node2) {
    int n = static_cast<int>(edges.size());
    
    // Perform DFS from a start node and fill distances; distances initialized to -1 for unreachable.
    auto dfs_distances = [&](int start) {
        std::vector<int> dist(n, -1);
        std::vector<bool> visited(n, false);
        int current = start;
        int d = 0;
        while (current != -1 && !visited[current]) {
            visited[current] = true;
            dist[current] = d++;
            current = edges[current];
        }
        return dist;
    };
    
    std::vector<int> dist1 = dfs_distances(node1);
    std::vector<int> dist2 = dfs_distances(node2);
    
    int best_node = -1;
    int best_max_dist = INT_MAX;
    
    for (int i = 0; i < n; ++i) {
        if (dist1[i] != -1 && dist2[i] != -1) {
            int current_max = std::max(dist1[i], dist2[i]);
            if (current_max < best_max_dist ||
                (current_max == best_max_dist && (best_node == -1 || i < best_node))) {
                best_max_dist = current_max;
                best_node = i;
            }
        }
    }
    return best_node;
}
#include <cassert>
#include <vector>

// Function declaration (definition is provided above in actual compilation)
int closestMeetingNode(const std::vector<int>& edges, int node1, int node2);

int main() {
    // Example 1: Simple chain 0->1->2, node1=0, node2=2. Common reachable: 2 (0 reaches 0,1,2; 2 reaches 2). Max distance: max(2,0)=2.
    assert(closestMeetingNode({1,2,-1}, 0, 2) == 2);
    
    // Example 2: Separate components, no common node.
    assert(closestMeetingNode({1,-1,-1}, 0, 2) == -1);
    
    // Example 3: Both start at same node.
    assert(closestMeetingNode({-1,-1}, 0, 0) == 0);
    
    // Example 4: Cycle 0->1->0, node1=0, node2=1. Common nodes: 0 and 1. For 0: max(0,1)=1; for 1: max(1,0)=1. Tie, pick smaller index 0.
    assert(closestMeetingNode({1,0}, 0, 1) == 0);
    
    // Example 5: Multiple common nodes with different distances, ensure min max distance chosen.
    // 0->1->2->3, 4->2; node1=0, node2=4. Common: 2 (dist 2 from 0, 1 from 4 => max=2) and 3 (dist 3,2 => max=3). Pick 2.
    assert(closestMeetingNode({1,2,3,-1,2}, 0, 4) == 2);
    
    // Example 6: Self-loop and disconnected.
    assert(closestMeetingNode({0,-1}, 0, 1) == -1);
    
    // Example 7: Node1 reaches node2, node2 reaches node1 (two-node cycle). node1=0, node2=1. Both nodes common: 0 max(0,1)=1; 1 max(1,0)=1 → pick 0.
    assert(closestMeetingNode({1,0,-1}, 0, 1) == 0);
    
    // Example 8: Longer chain with a tie in max but different index.
    // 0->1->2->3, node1=0, node2=3 → common: 3 (max(3,0)=3) only. return 3.
    assert(closestMeetingNode({1,2,3,-1}, 0, 3) == 3);
    
    // Example 9: Single node graph.
    assert(closestMeetingNode({-1}, 0, 0) == 0);
    
    // Example 10: Node1 unreachable to node2's component, but node2 reaches node1.
    // 0->1, 2->0; node1=0, node2=2. Common node 0 (max(0,1)=1) and 1 (max(1,2)=2) → pick 0.
    assert(closestMeetingNode({1,-1,0}, 0, 2) == 0);
    
    return 0;
}
