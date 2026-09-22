// You are given a network of `n` nodes (numbered 1 to `n`) and `m` hyperedges, where each hyperedge connects exactly `k` distinct nodes. A hyperedge allows you to travel directly between any two nodes that are both part of that hyperedge. The cost to move from one node to another using a single hyperedge is 1 (so traversing a hyperedge costs 1 regardless of which two nodes you move between). Write a C++ function `int minHyperedgeHops(int n, int k, int m, const std::vector<std::vector<int>>& nodeToHyperedges, const std::vector<std::vector<int>>& hyperedgeToNodes)` that returns the minimum number of hyperedge hops needed to go from node 1 to node `n`. If node `n` is unreachable, return `-1`. The input vectors are already preprocessed: `nodeToHyperedges[i]` (for 1-indexed node `i`) lists the indices (1 to `m`) of hyperedges that contain node `i`; `hyperedgeToNodes[j]` (for 1-indexed hyperedge `j`) lists the nodes (1 to `n`) that belong to that hyperedge. All hyperedges contain exactly `k` distinct nodes, but you may assume that the structures are consistent and complete. The function should not modify the input vectors.
The problem is a shortest-path problem on a bipartite graph where nodes are connected via hyperedges. The key observation is that entering a hyperedge from any of its nodes and leaving it to another node costs exactly 1 hop. Therefore, we can model this as: from a node, you can go to any hyperedge that contains it (cost 0 to enter the hyperedge conceptually), and from a hyperedge, you can go to any node in it (cost 1 to leave it and land on a new node). That is essentially a 0-1 BFS, but since all transitions that change the node count cost 1, we can treat hyperedges as intermediate vertices. We use Dijkstra's algorithm on a graph with `n` nodes and `m` hyperedge vertices, where edges have weight 1 when moving from a node to a hyperedge or vice versa, but since the cost is symmetric, we only count going through a hyperedge as one hop. Actually, a simpler approach: start at node 1 with distance 0. For each node popped, look at all hyperedges containing it; if that hyperedge has not been processed yet, mark it processed and for each node in that hyperedge, if its distance is greater than current distance + 1, update and push. This directly counts each hyperedge usage as 1 hop. This is exactly the logic in the provided snippet. Edge cases: if `n` is 1, answer is 0 (already at destination). If there is no path, return -1. Time complexity is O((n·average incidents + m·k) log n) due to priority queue pushes, and space O(n+m+sum of degrees). Dijkstra is correct because all edge weights are positive (1). Since we never revisit a hyperedge (marked visited when first popped), each hyperedge is processed once. The initial distance for node 1 is 0; others are infinity. The answer is `C[n]` after Dijkstra, or -1 if unreachable.
#include <queue>
#include <vector>
#include <climits>

// Returns the minimum number of hyperedge hops from node 1 to node n.
// nodeToHyperedges[i] (1-indexed) lists hyperedge indices (1..m) containing node i.
// hyperedgeToNodes[j] (1-indexed) lists node indices (1..n) in hyperedge j.
int minHyperedgeHops(int n, int m,
                     const std::vector<std::vector<int>>& nodeToHyperedges,
                     const std::vector<std::vector<int>>& hyperedgeToNodes) {
    if (n == 1) return 0;

    const int INF = INT_MAX / 2;
    std::vector<int> dist(n + 1, INF);
    std::vector<bool> hyperedgeUsed(m + 1, false);
    dist[1] = 0;

    // Min-heap of (cost, node)
    using State = std::pair<int, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    pq.push({0, 1});

    while (!pq.empty()) {
        auto [cost, node] = pq.top();
        pq.pop();

        if (cost > dist[node]) continue;
        if (node == n) break;

        for (int hyper : nodeToHyperedges[node]) {
            if (hyperedgeUsed[hyper]) continue;
            hyperedgeUsed[hyper] = true;

            for (int neighbor : hyperedgeToNodes[hyper]) {
                int newCost = cost + 1;
                if (newCost < dist[neighbor]) {
                    dist[neighbor] = newCost;
                    pq.push({newCost, neighbor});
                }
            }
        }
    }

    return (dist[n] == INF) ? -1 : dist[n];
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple chain of 3 nodes via two hyperedges
    // n=3, k=2, m=2
    // Hyperedge 1: nodes {1,2}; Hyperedge 2: nodes {2,3}
    {
        int n = 3, m = 2;
        std::vector<std::vector<int>> nodeToHyperedges(4);
        nodeToHyperedges[1] = {1};
        nodeToHyperedges[2] = {1,2};
        nodeToHyperedges[3] = {2};
        std::vector<std::vector<int>> hyperedgeToNodes(3);
        hyperedgeToNodes[1] = {1,2};
        hyperedgeToNodes[2] = {2,3};
        assert(minHyperedgeHops(n, m, nodeToHyperedges, hyperedgeToNodes) == 2);
    }

    // Test 2: Direct edge connecting node 1 and n
    {
        int n = 5, m = 1;
        std::vector<std::vector<int>> nodeToHyperedges(6);
        nodeToHyperedges[1] = {1};
        nodeToHyperedges[5] = {1};
        std::vector<std::vector<int>> hyperedgeToNodes(2);
        hyperedgeToNodes[1] = {1,5};
        assert(minHyperedgeHops(n, m, nodeToHyperedges, hyperedgeToNodes) == 1);
    }

    // Test 3: Unreachable target
    {
        int n = 4, m = 1;
        std::vector<std::vector<int>> nodeToHyperedges(5);
        nodeToHyperedges[1] = {1};
        nodeToHyperedges[2] = {1};
        // Node 4 not connected to any hyperedge
        std::vector<std::vector<int>> hyperedgeToNodes(2);
        hyperedgeToNodes[1] = {1,2};
        assert(minHyperedgeHops(n, m, nodeToHyperedges, hyperedgeToNodes) == -1);
    }

    // Test 4: n == 1
    {
        int n = 1, m = 0;
        std::vector<std::vector<int>> nodeToHyperedges(2);
        std::vector<std::vector<int>> hyperedgeToNodes(1);
        assert(minHyperedgeHops(n, m, nodeToHyperedges, hyperedgeToNodes) == 0);
    }

    // Test 5: Larger network with multiple routes, choose shortest
    {
        int n = 6, m = 3;
        // Hyperedge 1: {1,2,3}
        // Hyperedge 2: {3,4,5}
        // Hyperedge 3: {5,6}
        std::vector<std::vector<int>> nodeToHyperedges(7);
        nodeToHyperedges[1] = {1};
        nodeToHyperedges[2] = {1};
        nodeToHyperedges[3] = {1,2};
        nodeToHyperedges[4] = {2};
        nodeToHyperedges[5] = {2,3};
        nodeToHyperedges[6] = {3};
        std::vector<std::vector<int>> hyperedgeToNodes(4);
        hyperedgeToNodes[1] = {1,2,3};
        hyperedgeToNodes[2] = {3,4,5};
        hyperedgeToNodes[3] = {5,6};
        assert(minHyperedgeHops(n, m, nodeToHyperedges, hyperedgeToNodes) == 3);
    }

    // Test 6: Multiple nodes in same hyperedge, direct hop from any to any
    {
        int n = 4, m = 1;
        std::vector<std::vector<int>> nodeToHyperedges(5);
        nodeToHyperedges[1] = {1};
        nodeToHyperedges[4] = {1};
        // Also include nodes 2 and 3 in the same hyperedge
        nodeToHyperedges[2] = {1};
        nodeToHyperedges[3] = {1};
        std::vector<std::vector<int>> hyperedgeToNodes(2);
        hyperedgeToNodes[1] = {1,2,3,4};
        assert(minHyperedgeHops(n, m, nodeToHyperedges, hyperedgeToNodes) == 1);
    }

    return 0;
}
