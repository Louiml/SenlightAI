Write a C++ function `int longestCycle(const std::vector<int>& successor)` that takes a vector of size `V` where each element at index `i` gives the node that node `i` points to (a directed functional graph; `-1` means no outgoing edge). The function should compute the maximum cycle length among all directed cycles in the graph. If there are no cycles, return `-1`. Each node has exactly one outgoing edge unless it is `-1` (no edge). The graph may have nodes with no outgoing edges, and multiple disjoint cycles are possible. The function must use a DFS-based approach with careful use of recursion and state to detect cycles and measure their lengths without revisiting nodes in a way that causes incorrect counts.
// The graph is a functional graph where each node has at most one outgoing edge. A cycle exists if following edges from some starting node eventually returns to a previously visited node in the same DFS path. The provided snippet uses DFS with a visited array that is reset after returning from each DFS call (backtracking), and it tracks the distance from the starting node. When a back edge to the starting node is found, the total distance (current depth + 1) is recorded as a candidate cycle length. However, the snippet resets `vis` after each DFS, which correctly handles cases where a cycle is entered from outside, but it does not always detect cycles that are not reachable from the starting node in a single DFS? Actually, because the outer loop calls DFS from every node, every distinct cycle will be found at least once from each node on that cycle. The key is that when DFS from a node on a cycle, following the cycle will eventually return to the source, and that distance is the cycle length. For nodes not on cycles, no back edge to source is found, so no dist is updated. The algorithm works for all nodes. Edge cases: (1) A self-loop: node i points to itself, distance is 1. (2) A cycle of length L, the DFS from a node on the cycle gives distance L. (3) If no cycle, all dist remain 0 initially, but since we return -1 for no cycles, we must handle that. The snippet uses `dist` initialized to 0 and `ans = INT_MIN`, then if no cycle, ans stays INT_MIN, which would print INT_MIN—but we should adjust. For our function, we’ll track maximum cycle length found. Time complexity: O(V * (E_per_node)) but since each node has out-degree at most 1, each DFS from a node walks along a path of length up to V, and we do this for V starting nodes, worst-case O(V^2). Space: O(V) for visited array and recursion stack depth up to V. To improve, we could use three-color DFS (0=unvisited,1=in progress,2=done) to detect cycles in O(V) time total. I’ll implement the simpler O(V^2) method to match the given snippet’s approach, but we can also provide a faster O(V) method as an alternative. However, the task says “inspired by a given code snippet” – we should follow the same DFS-with-backtracking approach for clarity, but we can mention the complexity. For the reference solution, we'll use the backtracking DFS exactly as in the snippet, but adapted to a standalone function.
#include <vector>
#include <algorithm>
#include <climits>

// Compute the maximum directed cycle length in a functional graph.
// successor[i] is the node that i points to, or -1 if none.
// Returns -1 if there are no cycles.
int longestCycle(const std::vector<int>& successor) {
    int V = successor.size();
    std::vector<bool> visited(V, false);
    std::vector<int> cycleDist(V, 0); // stores max cycle length found so far

    // Recursive DFS with backtracking
    std::function<void(int, int, int)> dfs = [&](int node, int depth, int source) {
        visited[node] = true;
        int next = successor[node];
        if (next != -1) {
            if (!visited[next]) {
                dfs(next, depth + 1, source);
            } else if (next == source) {
                // Found a cycle back to the source of this DFS
                cycleDist[source] = std::max(cycleDist[source], depth + 1);
            }
        }
        visited[node] = false; // backtrack
    };

    // Run DFS from every node
    for (int i = 0; i < V; ++i) {
        dfs(i, 0, i);
    }

    int ans = -1;
    for (int i = 0; i < V; ++i) {
        if (cycleDist[i] > 0) {
            ans = std::max(ans, cycleDist[i]);
        }
    }
    return ans;
}
#include <cassert>
#include <vector>

// Function declared here (or include the header)
int longestCycle(const std::vector<int>& successor);

int main() {
    // No edges: no cycle
    assert(longestCycle({-1, -1, -1}) == -1);
    // Self-loop at node 0
    assert(longestCycle({0, -1, -1}) == 1);
    // Simple cycle 0->1->0
    assert(longestCycle({1, 0, -1}) == 2);
    // Cycle of length 3
    assert(longestCycle({1, 2, 0}) == 3);
    // Two cycles: 0->1->0 (length 2) and 2->3->4->2 (length 3) => max 3
    assert(longestCycle({1, 0, 3, 4, 2}) == 3);
    // Node pointing to itself combined with a longer chain
    assert(longestCycle({0, 2, -1, 4, 3, -1}) == 2);
    // Single node with no edge
    assert(longestCycle({-1}) == -1);
    // All nodes form one big cycle
    assert(longestCycle({1, 2, 3, 4, 0}) == 5);
    // Nodes leading into a cycle but not on it
    assert(longestCycle({1, 2, 3, 1, -1}) == 3); // cycle 1-2-3-1
    return 0;
}
