Write a C++ function `int shortestPathToVisitAllNodes(const std::vector<std::vector<int>>& graph)` that, given an undirected graph represented as an adjacency list where `graph[i]` contains the list of nodes directly connected to node `i`, returns the length of the shortest path that visits every node at least once. You may start at any node and end at any node. Edges are unweighted (each edge counts as length 1). The graph is connected and contains at least 1 and at most 12 nodes. The length is defined as the number of edges traversed. If no such path exists (which should not happen for a connected graph), return -1. The function should be efficient and use a bitmask to track visited nodes.

#include <cassert>
#include <vector>

int main() {
    // Test 1: Single node graph
    std::vector<std::vector<int>> g1 = {{}};
    assert(shortestPathToVisitAllNodes(g1) == 0);

    // Test 2: Two nodes connected
    std::vector<std::vector<int>> g2 = {{1}, {0}};
    assert(shortestPathToVisitAllNodes(g2) == 1);

    // Test 3: Three nodes in a line (0-1-2)
    std::vector<std::vector<int>> g3 = {{1}, {0,2}, {1}};
    assert(shortestPathToVisitAllNodes(g3) == 2);

    // Test 4: Three nodes in a triangle
    std::vector<std::vector<int>> g4 = {{1,2}, {0,2}, {0,1}};
    assert(shortestPathToVisitAllNodes(g4) == 2);

    // Test 5: Four nodes in a line (0-1-2-3)
    std::vector<std::vector<int>> g5 = {{1}, {0,2}, {1,3}, {2}};
    assert(shortestPathToVisitAllNodes(g5) == 3);

    // Test 6: Four nodes in a cycle (0-1-2-3-0)
    std::vector<std::vector<int>> g6 = {{1,3}, {0,2}, {1,3}, {2,0}};
    assert(shortestPathToVisitAllNodes(g6) == 3);

    // Test 7: Star graph with center 0 and leaves 1,2,3
    std::vector<std::vector<int>> g7 = {{1,2,3}, {0}, {0}, {0}};
    assert(shortestPathToVisitAllNodes(g7) == 3);  // 0->1->0->2->0->3 is 6 edges? Actually shortest is 0->1->0->2->0->3 = 5? Let's check: 1->0->2->0->3 = 4 edges, but better: 1->0->2->0->3 is 4? Actually from 1 to 0 (1 edge), to 2 (1), to 0 (1), to 3 (1) = 4 edges. But 0->1->0->2->0->3 = 5. So assert should be 4.

    // Correct test 7: star graph shortest is 4 (e.g., start at leaf 1: 1-0-2-0-3)
    std::vector<std::vector<int>> g7b = {{1,2,3}, {0}, {0}, {0}};
    assert(shortestPathToVisitAllNodes(g7b) == 4);

    // Test 8: Complete graph on 4 nodes (each connects to all others)
    std::vector<std::vector<int>> g8 = {{1,2,3}, {0,2,3}, {0,1,3}, {0,1,2}};
    assert(shortestPathToVisitAllNodes(g8) == 3);  // e.g., 0->1->2->3

    // Test 9: Graph with 5 nodes in a path
    std::vector<std::vector<int>> g9 = {{1}, {0,2}, {1,3}, {2,4}, {3}};
    assert(shortestPathToVisitAllNodes(g9) == 4);

    // Test 10: Graph with 2 nodes but one isolated? Not allowed since connected, but test with n=2
    std::vector<std::vector<int>> g10 = {{1}, {0}};
    assert(shortestPathToVisitAllNodes(g10) == 1);

    return 0;
}

#include <vector>
#include <queue>
#include <set>
#include <utility>

// Returns the length of the shortest path (number of edges) that visits all nodes at least once.
// The graph is undirected and connected; node count is at most 12.
int shortestPathToVisitAllNodes(const std::vector<std::vector<int>>& graph) {
    const int n = static_cast<int>(graph.size());
    if (n == 0) return 0;
    
    // State: (current node, visited mask, path length in edges)
    struct State {
        int node;
        int mask;
        int edges;
        State(int n, int m, int e) : node(n), mask(m), edges(e) {}
    };
    
    std::queue<State> q;
    std::set<std::pair<int, int>> visited;
    
    const int allMask = (1 << n) - 1;
    
    // Initialize BFS from every possible starting node.
    for (int i = 0; i < n; ++i) {
        int mask = 1 << i;
        q.emplace(i, mask, 0);  // edges = 0 initially
        visited.insert({i, mask});
    }
    
    while (!q.empty()) {
        State curr = q.front();
        q.pop();
        
        if (curr.mask == allMask) {
            return curr.edges;
        }
        
        for (int neighbor : graph[curr.node]) {
            int newMask = curr.mask | (1 << neighbor);
            if (visited.find({neighbor, newMask}) == visited.end()) {
                visited.insert({neighbor, newMask});
                q.emplace(neighbor, newMask, curr.edges + 1);
            }
        }
    }
    
    return -1;  // Should never be reached for a connected graph
}

// This problem is the classic "shortest path visiting all nodes" problem, often solved using BFS with state compression. Since visiting all nodes is required and each node can be visited multiple times, a simple BFS on nodes is insufficient. Instead, we treat each state as a pair `(current_node, visited_mask)` where `visited_mask` is a bitmask with the i-th bit set if node `i` has been visited at least once. The BFS starts from every possible starting node with its corresponding mask (only that bit set). The cost for each initial state is 1 (representing one node visited with zero edges). We use a `set` or `unordered_set` to avoid revisiting the same `(node, mask)` pair. When we pop a state, if its mask equals `(1 << n) - 1` (all bits set), we return `cost - 1` because the cost counted nodes, not edges, and the initial node had cost 1 for 0 edges. Otherwise, for each neighbor, we compute the new mask by OR-ing the neighbor's bit, and if that `(neighbor, new_mask)` hasn't been visited, we push it with cost+1. The BFS guarantees the first time we reach the all-visited mask is the shortest number of edges because BFS explores states in non-decreasing cost. Edge cases: when `n == 1`, the BFS returns `0` immediately because the initial mask equals `all`. Also, since the graph is connected, a solution always exists, but returning -1 is a safety net. Time complexity is O(2^n * n * avg_degree) because there are at most n * 2^n states, and each state processes its adjacency list. Space complexity is O(n * 2^n) for the visited set and queue.
