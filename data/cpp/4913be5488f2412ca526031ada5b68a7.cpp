Write a C++ function `computePageRankContributions` that, given a vector of directed edges represented as pairs of integer node IDs (src, dst), returns a `std::map<int, double>` mapping each unique node ID (both sources and destinations) to its PageRank contribution value. The contribution for each node is defined as follows: for each directed edge (src, dst), the source node `src` contributes an amount equal to `0.85 / out_degree(src)` to its own score (i.e., the contribution is added to `src`'s entry in the map). Nodes with zero out-degree still appear in the map with contribution 0.0. All nodes (both sources and destinations) must appear in the result map, even if they have no outgoing edges. The function should not modify the input vector.

#include <cassert>
#include <map>
#include <vector>
#include <utility>
#include <cmath>

int main() {
    // Test 1: Simple chain 1->2, 2->3
    std::vector<std::pair<int,int>> edges1 = {{1,2},{2,3}};
    auto result1 = computePageRankContributions(edges1);
    assert(result1.size() == 3);
    assert(std::fabs(result1[1] - 0.85) < 1e-9);
    assert(std::fabs(result1[2] - 0.85) < 1e-9);
    assert(std::fabs(result1[3] - 0.0) < 1e-9);
    
    // Test 2: Two edges from same source
    std::vector<std::pair<int,int>> edges2 = {{1,2},{1,3}};
    auto result2 = computePageRankContributions(edges2);
    assert(result2.size() == 3);
    // 0.85 / 2 = 0.425, twice
    assert(std::fabs(result2[1] - 0.85) < 1e-9);
    assert(std::fabs(result2[2] - 0.0) < 1e-9);
    assert(std::fabs(result2[3] - 0.0) < 1e-9);
    
    // Test 3: Node with no outgoing edges appears with zero
    std::vector<std::pair<int,int>> edges3 = {{1,2},{3,4}};
    auto result3 = computePageRankContributions(edges3);
    assert(result3.size() == 4);
    assert(std::fabs(result3[1] - 0.85) < 1e-9);
    assert(std::fabs(result3[2] - 0.0) < 1e-9);
    assert(std::fabs(result3[3] - 0.85) < 1e-9);
    assert(std::fabs(result3[4] - 0.0) < 1e-9);
    
    // Test 4: Self-loop
    std::vector<std::pair<int,int>> edges4 = {{5,5}};
    auto result4 = computePageRankContributions(edges4);
    assert(result4.size() == 1);
    assert(std::fabs(result4[5] - 0.85) < 1e-9);
    
    // Test 5: Empty input (no nodes)
    std::vector<std::pair<int,int>> edges5;
    auto result5 = computePageRankContributions(edges5);
    assert(result5.empty());
    
    // Test 6: Duplicate edges
    std::vector<std::pair<int,int>> edges6 = {{1,2},{1,2}};
    auto result6 = computePageRankContributions(edges6);
    assert(result6.size() == 2);
    // out_degree(1) = 2, each contributes 0.425, total 0.85
    assert(std::fabs(result6[1] - 0.85) < 1e-9);
    assert(std::fabs(result6[2] - 0.0) < 1e-9);
    
    return 0;
}

#include <map>
#include <vector>
#include <utility>

// Compute PageRank contributions for each node based on its out-degree.
// For each directed edge (src, dst), src contributes 0.85 / out_degree(src) to its own score.
// All nodes (sources and destinations) appear in the result, even with zero contribution.
std::map<int, double> computePageRankContributions(
    const std::vector<std::pair<int, int>>& edges) {
    
    std::map<int, int> out_degree;
    
    // First pass: count out-degrees for all source nodes.
    for (const auto& edge : edges) {
        out_degree[edge.first]++;
    }
    
    // Ensure all destination nodes appear in the degree map (with default 0).
    for (const auto& edge : edges) {
        if (out_degree.find(edge.second) == out_degree.end()) {
            out_degree[edge.second] = 0;
        }
    }
    
    std::map<int, double> contributions;
    
    // Second pass: accumulate contributions for each source node.
    for (const auto& edge : edges) {
        int src = edge.first;
        if (out_degree[src] > 0) {
            contributions[src] += 0.85 / out_degree[src];
        }
    }
    
    // Ensure every node present in out_degree map has an entry in contributions.
    for (const auto& entry : out_degree) {
        if (contributions.find(entry.first) == contributions.end()) {
            contributions[entry.first] = 0.0;
        }
    }
    
    return contributions;
}

// The algorithm processes each directed edge and accumulates contributions. First, we build a degree map: for each node that appears as a source, increment its out-degree counter. Then, for each edge (src, dst), we compute `0.85 / out_degree(src)` and add that value to the contribution map for `src`. This requires two passes over the edges: one to compute degrees, one to accumulate contributions. Edge cases include: (1) a node with out-degree zero—it contributes nothing, but still must appear in the result map with value 0.0; (2) nodes that appear only as destinations—they have out-degree 0 and contribution 0.0; (3) duplicate edges—they are counted separately, so out-degree counts each occurrence, and each occurrence adds its own contribution; (4) self-loops (src == dst) are handled naturally since we only update `src`. Time complexity is O(E) for E edges, and space complexity is O(V) where V is the number of unique nodes, since we store degree and contribution maps.
