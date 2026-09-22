You are given a directed graph with `n` nodes (numbered 0 to n-1) and `m` directed edges. Write a C++ function `vector<vector<int>> generateScenario(int n, vector<pair<int,int>> edges, int& scenarioNumber)` that processes multiple test cases (like the original code does) and returns a data structure containing the "scenario" output: for each node, its topological "rank" (level in a topological ordering where nodes with in-degree 0 are level 1, their immediate successors when all predecessors are processed become level 2, etc.), and the nodes at each level sorted in ascending order. The function should simulate the original algorithm exactly: process nodes with in-degree 0 in stack order (LIFO), decrement in-degrees of successors, and assign ranks when a node's in-degree becomes 0. The output should be a vector of vectors, where `result[i]` (for i starting at 1) contains the sorted list of nodes with rank i, and `result[0]` is empty. Ignore nodes that never get a rank (if the graph has cycles). The function must return the result and also output (via `scenarioNumber` reference) the header `"Scenario #" + to_string(scenarioNumber) + ":"` for each test case, but you will only implement the core logic to return the vector of vectors; the test code will handle printing. The function should handle multiple test cases by being called for each case with an incrementing scenario number. The edges are given as pairs `(from, to)` meaning `from -> to`? Actually in the original, `y` is predecessor, `x` is successor, so edge is `x -> y`? Let's clarify: The original reads `cin>>y>>x; adj[x].push_back(y);` so edge goes from `x` to `y` (x is source, y is target). In your function, you'll receive edges as vector of pairs `(source, target)` where source points to target. Ensure you build adjacency correctly and compute in-degrees based on targets. Return the vector of vectors.

#include <cassert>
#include <vector>
#include <utility>

std::vector<std::vector<int>> buildScenarioRanks(int n, const std::vector<std::pair<int,int>>& edges);

int main() {
    // Test 1: Simple chain 0->1->2
    std::vector<std::pair<int,int>> edges1 = {{0,1},{1,2}};
    std::vector<std::vector<int>> res1 = buildScenarioRanks(3, edges1);
    assert(res1[1] == std::vector<int>({0}));
    assert(res1[2] == std::vector<int>({1}));
    assert(res1[3] == std::vector<int>({2}));
    assert(res1[0].empty());

    // Test 2: Two independent sources and a merge
    std::vector<std::pair<int,int>> edges2 = {{0,2},{1,2}};
    std::vector<std::vector<int>> res2 = buildScenarioRanks(3, edges2);
    assert(res2[1] == std::vector<int>({0,1})); // sorted ascending
    assert(res2[2] == std::vector<int>({2}));

    // Test 3: Cycle 0->1,1->0, plus disconnected node 2
    std::vector<std::pair<int,int>> edges3 = {{0,1},{1,0}};
    std::vector<std::vector<int>> res3 = buildScenarioRanks(3, edges3);
    // Node 2 has no incoming edges, rank 1
    assert(res3[1] == std::vector<int>({2}));
    assert(res3[2].empty());

    // Test 4: Self-loop on 0, plus edge 1->2
    std::vector<std::pair<int,int>> edges4 = {{0,0},{1,2}};
    std::vector<std::vector<int>> res4 = buildScenarioRanks(3, edges4);
    assert(res4[1] == std::vector<int>({1})); // node 0 has in-degree 1 from self, node 1 has 0
    assert(res4[2] == std::vector<int>({2}));

    // Test 5: Multiple edges and duplicates, nodes 0,1,2,3
    std::vector<std::pair<int,int>> edges5 = {{0,2},{0,2},{1,2},{2,3}};
    std::vector<std::vector<int>> res5 = buildScenarioRanks(4, edges5);
    assert(res5[1] == std::vector<int>({0,1}));
    assert(res5[2] == std::vector<int>({2}));
    assert(res5[3] == std::vector<int>({3}));

    // Test 6: Empty graph
    std::vector<std::vector<int>> res6 = buildScenarioRanks(0, {});
    assert(res6.size() == 1);
    assert(res6[0].empty());

    // Test 7: Single node with no edges
    std::vector<std::vector<int>> res7 = buildScenarioRanks(1, {});
    assert(res7[1] == std::vector<int>({0}));
    assert(res7[0].empty());

    // Test 8: Disconnected nodes
    std::vector<std::vector<int>> res8 = buildScenarioRanks(4, {});
    assert(res8[1] == std::vector<int>({0,1,2,3}));

    return 0;
}

#include <vector>
#include <stack>
#include <set>

// Given number of nodes n and directed edges as pairs (source, target),
// return a vector where result[i] (i>=1) contains sorted nodes with topological rank i.
// Nodes involved in cycles are omitted. result[0] is empty.
std::vector<std::vector<int>> buildScenarioRanks(int n, const std::vector<std::pair<int,int>>& edges) {
    std::vector<std::vector<int>> adj(n);
    std::vector<int> indeg(n, 0);
    
    for (const auto& e : edges) {
        int src = e.first;
        int dst = e.second;
        adj[src].push_back(dst);
        ++indeg[dst];
    }
    
    std::vector<std::vector<int>> result(n + 1);  // result[0] unused, index up to n
    std::vector<int> rank(n, 0);
    std::stack<int> st;
    
    for (int i = 0; i < n; ++i) {
        if (indeg[i] == 0) {
            st.push(i);
            rank[i] = 1;
            result[1].push_back(i);
        }
    }
    
    while (!st.empty()) {
        int x = st.top();
        st.pop();
        
        for (int u : adj[x]) {
            if (--indeg[u] == 0) {
                st.push(u);
                rank[u] = rank[x] + 1;
                result[rank[u]].push_back(u);
            }
        }
    }
    
    // Although we used push_back in processing order, the original used set for sorted output.
    // To match exactly, sort each non-empty level.
    for (int i = 1; i <= n; ++i) {
        std::sort(result[i].begin(), result[i].end());
    }
    
    return result;
}

// The algorithm is a modified Kahn's topological sort using a stack instead of a queue, which changes the order of processing and thus the assignment of ranks. Key steps:  
// 1. Build adjacency list from each source to its list of targets. Compute in-degree of each node (number of incoming edges).  
// 2. Initialize a stack with all nodes having in-degree 0. For each such node, set its rank to 1 and insert it into the set for level 1.  
// 3. While the stack is not empty, pop the top node `x`. For each target `u` of `x` (i.e., for each edge `x -> u`), decrement `indeg[u]`. If `indeg[u]` becomes 0, then push `u` onto the stack, set its rank to `rank[x] + 1`, and insert it into the set for that rank.  
// 4. After processing all nodes, the sets contain nodes at each level, sorted naturally because we use `set` (which is sorted ascending). However, the original code prints nodes in ascending order within each level via the set iteration.  
// 5. Handle cycles: nodes in a cycle will never have in-degree 0 after processing, so they remain unranked and are simply not included in any level. The output only includes levels that have at least one node.  
// 6. Edge cases: empty graph (n=0) returns empty vector; multiple edges or self-loops are handled naturally by in-degree counting; nodes with no incoming edges are all rank 1.  
// Time complexity: Each edge is processed once, and each node is pushed/popped once, so O(n+m). Using `set` for each level takes O(log k) per insertion, but total insertions are O(n), so worst-case O(n log n). Space complexity: O(n+m) for adjacency, in-degree, rank arrays, and the result sets.
