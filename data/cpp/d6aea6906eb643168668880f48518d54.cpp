/*
Given an array `edges` of length `n` where `edges[i]` points to the node that node `i` connects to (or `-1` if it has no outgoing edge), and two starting nodes `node1` and `node2`, write a C++ function `int closestMeetingNode(const std::vector<int>& edges, int node1, int node2)` that returns the node index where the sum of path lengths from `node1` and `node2` is minimized, prioritizing the smaller maximum distance first. If no node is reachable from both starting nodes, return `-1`. Nodes are labeled `0` to `n-1`. Each node has at most one outgoing edge, and paths follow these directed edges until a node with `-1` is reached or a previously visited node is encountered. When multiple nodes satisfy the smallest maximum distance condition, choose the node with the smallest index. Guarantee `0 <= node1, node2 < n`.
*/

#include <vector>
#include <climits>
#include <algorithm>

// Returns the node index reachable from both node1 and node2 with minimal max distance.
// If no common reachable node exists, returns -1.
int closestMeetingNode(const std::vector<int>& edges, int node1, int node2) {
    int n = static_cast<int>(edges.size());
    std::vector<bool> visited1(n, false);
    std::vector<bool> visited2(n, false);
    std::vector<int> dist1(n, 0);
    std::vector<int> dist2(n, 0);

    // Recursive DFS that records distances from a given start node.
    auto dfs = [&](int node, std::vector<bool>& visited, std::vector<int>& dist, auto&& self) -> void {
        visited[node] = true;
        int next = edges[node];
        if (next != -1 && !visited[next]) {
            dist[next] = dist[node] + 1;
            self(next, visited, dist, self);
        }
    };

    dfs(node1, visited1, dist1, dfs);
    dfs(node2, visited2, dist2, dfs);

    int answer = -1;
    int bestMaxDist = INT_MAX;
    for (int i = 0; i < n; ++i) {
        if (visited1[i] && visited2[i]) {
            int currentMax = std::max(dist1[i], dist2[i]);
            if (currentMax < bestMaxDist) {
                bestMaxDist = currentMax;
                answer = i;
            }
            // If equal, keep the smaller index because we iterate ascending.
        }
    }
    return answer;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple path meeting
    std::vector<int> edges1 = {1, 2, -1};
    assert(closestMeetingNode(edges1, 0, 2) == 2);

    // Test 2: No common reachable node
    std::vector<int> edges2 = {1, -1, 3, -1};
    assert(closestMeetingNode(edges2, 0, 2) == -1);

    // Test 3: Same starting node
    std::vector<int> edges3 = {1, 2, -1};
    assert(closestMeetingNode(edges3, 1, 1) == 1);

    // Test 4: Cycle with multiple meeting options, choose smallest max distance then smallest index
    std::vector<int> edges4 = {1, 2, 0, 4, 3};
    // From 0: path 0->1->2->0..., visited: 0,1,2, distances: 0,1,2
    // From 3: path 3->4->3..., visited: 3,4, distances: 0,1
    // Common: none -> -1
    assert(closestMeetingNode(edges4, 0, 3) == -1);

    // Test 5: Both reach a common node at different distances
    std::vector<int> edges5 = {2, 2, -1, 2};
    // From 0: 0->2 (dist 1), visited 0,2
    // From 1: 1->2 (dist 1), visited 1,2
    // Common: 2, max(1,1)=1 -> answer 2
    assert(closestMeetingNode(edges5, 0, 1) == 2);

    // Test 6: Multiple common nodes with distinct max distances
    std::vector<int> edges6 = {1, 2, 3, -1, 1};
    // From 0: 0->1->2->3, dist: 0,1,2,3
    // From 4: 4->1->2->3, dist: 0,1,2,3
    // Common: 1 (max(1,1)=1), 2 (max(2,2)=2), 3 (max(3,3)=3) -> answer 1
    assert(closestMeetingNode(edges6, 0, 4) == 1);

    // Test 7: Tie in max distance choose smaller index
    std::vector<int> edges7 = {1, 0, 3, 2};
    // From 0 and 2: cycle between 0-1 and 2-3, no common -> -1
    // But test scenario where common with tie: adjust
    std::vector<int> edges7b = {1, 0, 1, -1};
    // From 0: 0->1->0... visited 0,1 dist 0,1
    // From 2: 2->1->0... visited 2,1,0 dist 0,1,2
    // Common: 0 (max(0,2)=2), 1 (max(1,1)=1) -> answer 1
    assert(closestMeetingNode(edges7b, 0, 2) == 1);

    // Test 8: Node with -1 self-loop not possible; ensure handles -1 from start
    std::vector<int> edges8 = {-1, 0};
    // From 0: visited 0, no next
    // From 1: 1->0, visited 1,0
    // Common: 0, max(0,1)=1 -> answer 0
    assert(closestMeetingNode(edges8, 0, 1) == 0);

    return 0;
}

// The solution performs two Depth-First Searches (DFS) starting from `node1` and `node2` separately. For each start, we mark visited nodes and store the distance from the start to each reached node in a `path` vector. The DFS stops when the next node is `-1` or has already been visited. After both traversals complete, we iterate over all nodes `0` to `n-1`. For every node reachable from both starting nodes (i.e., visited in both DFS runs), we compute `max(dist1[i], dist2[i])` and keep track of the minimal such value. If there is a tie, the smaller node index wins because we iterate in increasing order and only update when strictly larger than the current candidate; we first initialize `ans = -1` and `best = INT_MAX`. This correctly handles cycles because visited arrays prevent infinite loops. The time complexity is O(n) as each node is visited at most once per DFS, and the space complexity is O(n) for visited and distance arrays plus the recursion stack (worst-case O(n) in a chain). Edge cases: both starts may be the same node, unreachable nodes from either start, and paths that terminate early.
