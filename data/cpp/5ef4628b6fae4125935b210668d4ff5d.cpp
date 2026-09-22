// Given a directed cyclic graph represented by a set of nodes and weighted directed edges, write a C++ function that computes a Hamiltonian path (a path visiting every node exactly once, starting at the designated entry node) that maximizes the sum of edge weights along the path. The function should return the ordered list of node IDs in the optimal path and the maximum total weight achievable. The graph is specified by an entry node ID, a list of node IDs, and a list of directed edges with associated positive weights. If multiple paths achieve the same maximum weight, any one of them is acceptable. Ensure the algorithm is correct for graphs with up to 20 nodes (though a general implementation is fine) and handles cases where nodes may be disconnected or the graph contains self-loops, which should be ignored. The output path must begin with the entry node.

#include <cassert>
#include <iostream>

// Assume the solution function is defined above.

int main() {
    // Test 1: Simple chain 0->1->2, all weights 1.
    {
        std::vector<int> nodes = {0, 1, 2};
        std::vector<WeightedEdge> edges = {{0,1,1}, {1,2,1}, {0,2,5}};
        auto result = maxHamiltonianPath(0, nodes, edges);
        std::vector<int> expected = {0, 1, 2};
        assert(result.first == expected);
        assert(result.second == 2); // 0->1 (1) + 1->2 (1) = 2; 0->2 would skip node 1.
    }

    // Test 2: Single node.
    {
        std::vector<int> nodes = {42};
        std::vector<WeightedEdge> edges = {};
        auto result = maxHamiltonianPath(42, nodes, edges);
        std::vector<int> expected = {42};
        assert(result.first == expected);
        assert(result.second == 0);
    }

    // Test 3: Disconnected graph, must still visit all nodes.
    {
        std::vector<int> nodes = {0, 1, 2};
        std::vector<WeightedEdge> edges = {{0,1,10}, {2,0,3}}; // 2 is separate but reachable via 0 only if we visit 2 later.
        auto result = maxHamiltonianPath(0, nodes, edges);
        // Possible optimal: 0->1 (10), then 1->2 (0) = 10; or 0->2 (0), 2->0 self-loop ignored, 0->1 (10) = 10; both valid.
        assert(result.first.size() == 3);
        assert(result.second == 10);
    }

    // Test 4: Graph where choosing a heavier edge earlier results in worse overall.
    {
        std::vector<int> nodes = {0, 1, 2};
        std::vector<WeightedEdge> edges = {{0,1,100}, {1,2,1}, {0,2,99}};
        auto result = maxHamiltonianPath(0, nodes, edges);
        // Path 0->2->1: 99 + 0 (no edge 2->1) = 99; Path 0->1->2: 100 + 1 = 101.
        std::vector<int> expected = {0, 1, 2};
        assert(result.first == expected);
        assert(result.second == 101);
    }

    // Test 5: Self-loops ignored.
    {
        std::vector<int> nodes = {0, 1};
        std::vector<WeightedEdge> edges = {{0,0,1000}, {0,1,5}, {1,1,1000}};
        auto result = maxHamiltonianPath(0, nodes, edges);
        std::vector<int> expected = {0, 1};
        assert(result.first == expected);
        assert(result.second == 5);
    }

    // Test 6: Multiple edges between same pair, take maximum weight.
    {
        std::vector<int> nodes = {0, 1};
        std::vector<WeightedEdge> edges = {{0,1,3}, {0,1,7}};
        auto result = maxHamiltonianPath(0, nodes, edges);
        assert(result.second == 7);
    }

    // Test 7: Four nodes with a cycle, ensure optimal path found.
    {
        std::vector<int> nodes = {0, 1, 2, 3};
        std::vector<WeightedEdge> edges = {{0,1,10}, {1,2,10}, {2,3,10}, {3,0,10}, {0,2,1}};
        auto result = maxHamiltonianPath(0, nodes, edges);
        // Optimal: 0->1->2->3 (30) or 0->3->2->1 (30 via 3->0 is irrelevant).
        // Both weights 30; path is valid.
        assert(result.second == 30);
        assert(result.first.size() == 4);
    }

    // Test 8: Node with no outgoing edges must be visited last.
    {
        std::vector<int> nodes = {0, 1, 2};
        std::vector<WeightedEdge> edges = {{0,1,5}, {1,0,5}}; // 2 has no edges.
        auto result = maxHamiltonianPath(0, nodes, edges);
        assert(result.second == 5); // Must visit 2 with zero edge.
        assert(result.first.back() == 2);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <vector>
#include <algorithm>
#include <cstdint>
#include <limits>
#include <cassert>

// Represents a directed edge with a positive weight.
struct WeightedEdge {
    int from;
    int to;
    int weight;
};

/**
 * Finds a maximum-weight Hamiltonian path starting at entryNode.
 *
 * @param entryNode  The node where the path must start.
 * @param nodes      All node IDs in the graph (must be non-empty).
 * @param edges      Directed, weighted edges (self-loops are ignored).
 * @return A pair: (path as a sequence of node IDs, total weight of the path).
 */
std::pair<std::vector<int>, int> maxHamiltonianPath(
    int entryNode,
    const std::vector<int>& nodes,
    const std::vector<WeightedEdge>& edges) {

    const int n = static_cast<int>(nodes.size());
    assert(n > 0);

    // Map node IDs to indices 0..n-1 for DP arrays.
    std::vector<int> indexOf(nodes.size());
    for (int i = 0; i < n; ++i) {
        indexOf[i] = i;
    }
    // Since node IDs may be arbitrary, we need a lookup table.
    // Use a simple array assuming node IDs are small, but we can use a map for generality.
    // For simplicity, we assume node IDs are from 0..n-1 (or we can adjust).
    // We'll create a mapping using a vector of size = maxNodeId+1.
    int maxNodeId = entryNode;
    for (int v : nodes) maxNodeId = std::max(maxNodeId, v);
    std::vector<int> nodeToIndex(maxNodeId + 1, -1);
    for (int i = 0; i < n; ++i) {
        nodeToIndex[nodes[i]] = i;
    }

    // Weight matrix: weight[i][j] = max edge weight from i to j (ignoring self-loops).
    std::vector<std::vector<int>> weight(n, std::vector<int>(n, 0));
    for (const auto& e : edges) {
        if (e.from == e.to) continue; // ignore self-loops
        int u = nodeToIndex[e.from];
        int v = nodeToIndex[e.to];
        if (u != -1 && v != -1) {
            weight[u][v] = std::max(weight[u][v], e.weight);
        }
    }

    int startIdx = nodeToIndex[entryNode];
    assert(startIdx != -1);

    // DP[set][last] = max weight of a path covering exactly the set 'set' and ending at node 'last'.
    // We use -1 to indicate unreachable state.
    const int numSets = 1 << n;
    std::vector<std::vector<int>> dp(numSets, std::vector<int>(n, -1));
    dp[1 << startIdx][startIdx] = 0;

    // Best state tracking.
    int bestSet = 0;
    int bestLast = -1;
    int bestWeight = -1;

    // Iterate over all subsets.
    for (int set = 0; set < numSets; ++set) {
        for (int last = 0; last < n; ++last) {
            if (dp[set][last] < 0) continue; // state not reachable

            // Try extending with any unvisited node.
            for (int next = 0; next < n; ++next) {
                if (set & (1 << next)) continue; // already visited

                int newSet = set | (1 << next);
                int newWeight = dp[set][last] + weight[last][next];
                if (newWeight > dp[newSet][next]) {
                    dp[newSet][next] = newWeight;
                }
            }

            // If this state covers all nodes, consider it as a candidate best.
            if (set == numSets - 1) {
                if (dp[set][last] > bestWeight) {
                    bestWeight = dp[set][last];
                    bestSet = set;
                    bestLast = last;
                }
            }
        }
    }

    // If no full path was found (shouldn't happen because we can always
    // visit all nodes with zero-weight edges), handle fallback.
    if (bestLast == -1) {
        // A path always exists because any order of nodes is valid with zero weights.
        // Find any state that covers all nodes.
        for (int last = 0; last < n; ++last) {
            if (dp[numSets - 1][last] >= 0) {
                if (dp[numSets - 1][last] > bestWeight) {
                    bestWeight = dp[numSets - 1][last];
                    bestLast = last;
                    bestSet = numSets - 1;
                }
            }
        }
        // If still not found, set to start (shouldn't happen for n>=1).
        if (bestLast == -1) {
            bestSet = 1 << startIdx;
            bestLast = startIdx;
            bestWeight = 0;
        }
    }

    // Reconstruct the path.
    std::vector<int> path;
    int set = bestSet;
    int last = bestLast;
    path.push_back(nodes[last]);
    set &= ~(1 << last);

    while (set != 0) {
        // Find the predecessor that gave the optimal weight.
        int bestPred = -1;
        int bestPredWeight = -1;
        for (int pred = 0; pred < n; ++pred) {
            if (!(set & (1 << pred))) continue;
            if (dp[set][pred] < 0) continue;
            int candidate = dp[set][pred] + weight[pred][last];
            if (candidate == dp[bestSet][last]) { // note: bestSet is not updated; use current dp value
                // We need to be careful: dp[set|bit][last] is the value we want.
                // Actually we are reconstructing from the final state, so we know the final value.
                // Let's compute based on the recorded predecessor.
            }
            if (dp[set][pred] + weight[pred][last] == bestWeight) {
                bestPred = pred;
                bestPredWeight = dp[set][pred];
                break;
            }
        }
        // Since we don't store predecessor, we need to search for the pred that
        // satisfies dp[set][pred] + weight[pred][last] == bestWeight.
        // We'll loop until found.
        if (bestPred == -1) {
            for (int pred = 0; pred < n; ++pred) {
                if (!(set & (1 << pred))) continue;
                if (dp[set][pred] < 0) continue;
                if (dp[set][pred] + weight[pred][last] == bestWeight) {
                    bestPred = pred;
                    break;
                }
            }
        }
        // If still not found, pick any node with an edge; else choose any.
        if (bestPred == -1) {
            // No edge, so zero weight transition; choose any unvisited.
            for (int pred = 0; pred < n; ++pred) {
                if (set & (1 << pred)) {
                    if (dp[set][pred] >= 0) {
                        bestPred = pred;
                        break;
                    }
                }
            }
        }
        // Fallback in case of unexpected issues.
        if (bestPred == -1) {
            // Just pick first unvisited.
            for (int pred = 0; pred < n; ++pred) {
                if (set & (1 << pred)) {
                    bestPred = pred;
                    break;
                }
            }
        }
        path.push_back(nodes[bestPred]);
        bestWeight -= weight[bestPred][last];
        last = bestPred;
        set &= ~(1 << last);
    }

    // The path is built in reverse; reverse it.
    std::reverse(path.begin(), path.end());

    // The total weight is stored in bestWeight, but we have decremented it; save original.
    // Recompute total weight from the path for accuracy.
    int totalWeight = 0;
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        int u = nodeToIndex[path[i]];
        int v = nodeToIndex[path[i+1]];
        totalWeight += weight[u][v];
    }

    return {path, totalWeight};
}

// The problem is the maximum-weight Hamiltonian path problem, constrained to start at a given entry node. The optimal approach is dynamic programming over subsets of nodes, a classic O(2^n * n^2) algorithm. We represent the state as (set of visited nodes, last node in the path) and store the maximum total weight achievable for that state. Initialize the state containing only the entry node with zero weight. Then for each state, we try to extend the path to any node not yet visited, updating the DP table. The answer is the state that covers all nodes with the maximum weight, and we reconstruct the path by tracing back from that state. Self-loops are ignored because they do not help progress toward visiting new nodes. Graphs with disconnected components still have a solution because we can visit disconnected nodes in any order with zero-weight transitions between components. The time complexity is O(2^n * n^2) due to iterating over all subsets and all last/new node pairs, and the space complexity is O(2^n * n) for the DP table plus O(n) for reconstruction. Edge cases include n=1 (path is just the entry node) and nodes with no outgoing edges (they are appended at the end).
