Write a C++ function `int maxEvenComponents(const std::vector<long long>& nodeValues, const std::vector<std::pair<int, int>>& edges)` that takes a tree with `n` nodes (where `nodeValues[i]` is the value of node `i`, and `edges` lists the bidirectional connections between 0-indexed nodes) and returns the maximum number of components you can split the tree into by removing edges, such that each component has an even sum of node values. If it is impossible to split the tree into components all having even sums (i.e., the total sum of all node values is odd), return -1. Note that you may remove zero or more edges; if the whole tree itself has an even total sum, it counts as one valid component. The function must handle up to 2×10^5 nodes, with node values fitting in a signed 64-bit integer.
// The problem is a classic tree DP with parity constraints. Since the sum of all component sums must equal the total tree sum, if the total sum is odd, no valid partition exists, so return -1. Otherwise, we can always split the tree into components of even sum by removing edges where a subtree sum is even. The strategy: root the tree at any node (say node 0), and perform a DFS. For each node `u`, compute the sum of all node values in its subtree. If a child subtree sum is even, then we can cut the edge to that child, forming one valid even component, and increment the answer. If the subtree sum is odd, we keep it attached to the parent, because an odd subtree cannot be a standalone even component (and we cannot combine it with another odd subtree in this single DFS since we are only looking at immediate subtrees). After processing all children, return the total sum of the current subtree to the parent. The answer is the number of cut edges (where child subtree sum is even) plus 1 for the final component containing the root (which will have even total sum because the total sum is even and all cut subtrees are even). Edge cases: single-node tree with even value → answer is 1; single-node tree with odd value → total sum odd → -1; tree with total even but a child subtree sum odd — that odd subtree remains attached and combined with other parts to eventually form an even component. Time complexity is O(n) and space complexity is O(n) for adjacency list and recursion stack.
#include <vector>
#include <functional>

// Returns the maximum number of even-sum components after cutting edges,
// or -1 if the total sum is odd.
int maxEvenComponents(const std::vector<long long>& nodeValues,
                      const std::vector<std::pair<int, int>>& edges) {
    int n = static_cast<int>(nodeValues.size());
    
    // Build adjacency list
    std::vector<std::vector<int>> adj(n);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }
    
    // If total sum is odd, impossible
    long long totalSum = 0;
    for (long long v : nodeValues) {
        totalSum += v;
    }
    if (totalSum % 2 != 0) return -1;
    
    int componentCount = 0; // counts cut edges that form even components
    
    // DFS returns sum of subtree rooted at u
    std::function<long long(int, int)> dfs = [&](int u, int parent) -> long long {
        long long subtreeSum = nodeValues[u];
        for (int v : adj[u]) {
            if (v == parent) continue;
            long long childSum = dfs(v, u);
            // If child subtree sum is even, we can cut this edge
            if (childSum % 2 == 0) {
                componentCount++;
            } else {
                subtreeSum += childSum; // odd child stays attached
            }
        }
        return subtreeSum;
    };
    
    long long rootSum = dfs(0, -1);
    // rootSum is totalSum, which is even, so the root component is valid
    return componentCount + 1;
}
#include <cassert>
#include <vector>
#include <utility>

// Assume the solution function is defined above or included here.

int main() {
    // Test 1: Single node, even value -> 1 component
    {
        std::vector<long long> values = {4};
        std::vector<std::pair<int, int>> edges;
        assert(maxEvenComponents(values, edges) == 1);
    }
    
    // Test 2: Single node, odd value -> total odd -> -1
    {
        std::vector<long long> values = {3};
        std::vector<std::pair<int, int>> edges;
        assert(maxEvenComponents(values, edges) == -1);
    }
    
    // Test 3: Two nodes both odd, total even -> can cut edge? Subtrees: leaf odd -> stays, root + leaf = even -> 1 comp
    {
        std::vector<long long> values = {1, 3};
        std::vector<std::pair<int, int>> edges = {{0, 1}};
        assert(maxEvenComponents(values, edges) == 1);
    }
    
    // Test 4: Two nodes both even, total even -> cut leaf (even) -> 2 comps
    {
        std::vector<long long> values = {2, 4};
        std::vector<std::pair<int, int>> edges = {{0, 1}};
        assert(maxEvenComponents(values, edges) == 2);
    }
    
    // Test 5: Star: center even, leaves odd each, 3 leaves -> total even? 2 + 1+1+1 = 5 odd -> -1
    {
        std::vector<long long> values = {2, 1, 1, 1};
        std::vector<std::pair<int, int>> edges = {{0,1},{0,2},{0,3}};
        assert(maxEvenComponents(values, edges) == -1);
    }
    
    // Test 6: Star: center even, 2 odd leaves and 2 even leaves -> total even, even leaves cut (2), center+2 odd = even -> 1 more => 3
    {
        std::vector<long long> values = {10, 1, 1, 2, 4};
        std::vector<std::pair<int, int>> edges = {{0,1},{0,2},{0,3},{0,4}};
        assert(maxEvenComponents(values, edges) == 3);
    }
    
    // Test 7: Chain 1-3-2-4: total even, subtree at node3 (value 2) even -> cut, subtree at node1 (3+2+4=9 odd) stays, root=1+9=10 even -> 2 comps
    {
        std::vector<long long> values = {1, 3, 2, 4};
        std::vector<std::pair<int, int>> edges = {{0,1},{1,2},{2,3}};
        assert(maxEvenComponents(values, edges) == 2);
    }
    
    // Test 8: Chain 2-2-2-2: all even, each leaf subtree even -> cut 3 edges -> 4 comps
    {
        std::vector<long long> values = {2, 2, 2, 2};
        std::vector<std::pair<int, int>> edges = {{0,1},{1,2},{2,3}};
        assert(maxEvenComponents(values, edges) == 4);
    }
    
    // Test 9: Larger tree with mixed parities
    // Tree: 0(1) - 1(2) - 2(3) - 3(4) and 0 also connected to 4(5) and 5(6)
    // Total = 1+2+3+4+5+6=21 odd -> -1
    {
        std::vector<long long> values = {1, 2, 3, 4, 5, 6};
        std::vector<std::pair<int, int>> edges = {{0,1},{1,2},{2,3},{0,4},{0,5}};
        assert(maxEvenComponents(values, edges) == -1);
    }
    
    // Test 10: Empty graph (n=0) is not allowed by constraints, but test n=2 with total even and one even leaf
    {
        std::vector<long long> values = {0, 2};
        std::vector<std::pair<int, int>> edges = {{0,1}};
        assert(maxEvenComponents(values, edges) == 2);
    }
    
    return 0;
}
