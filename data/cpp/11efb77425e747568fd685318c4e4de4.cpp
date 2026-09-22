/*
Write a C++ function named `allocationFromEdges` that takes a vector of directed edges (each edge as a `std::pair<int,int>` with `first` = tail, `second` = head), constructs a graph, and returns a `std::pair` of two vectors: (1) `adj` — an adjacency list where each node's neighbors are sorted in ascending order of their original IDs, and (2) a permutation `itx` mapping each original node ID to its new index (0-based) after sorting nodes by decreasing degree (ties broken by smaller original ID). The graph can have up to 10,000 distinct nodes (original IDs range from 0 to 9999). The function must handle isolated nodes (nodes with degree 0) that appear as tails or heads in edges, and must include all distinct nodes from the edge list. The result's adjacency list shall be indexed by new indices (0..n-1), and each neighbor list shall contain new indices sorted in ascending order of original IDs (i.e., we sort neighbors by original ID before mapping to new index, then output the mapped list). The function should be `const`-correct: it must not modify the input edges, and it should return by value (using move semantics for efficiency). The function signature is: `std::pair<std::vector<std::vector<int>>, std::vector<int>> allocationFromEdges(const std::vector<std::pair<int,int>>& edges);`. You may assume the edges list is non-empty, but it may contain duplicate edges (you must treat them as separate and count them for degree calculation, but adjacency lists should not deduplicate them — they should contain each neighbor occurrence as per input, but we will sort them based on neighbor's original ID and then map). For simplicity, in the adjacency list, you may include duplicate entries if the input has duplicate edges, but the final adjacency list must have each occurrence (so if an edge appears twice, the neighbor appears twice). However, to align with typical graph representation, we will keep duplicates as they are. The returned `itx` vector must have size equal to the maximum original ID plus one (up to 10000) with -1 for nodes not present, but we only care that for present nodes the mapping is correct; however, the returned vector must be sized 10000 (to match original code) and filled with -1 for absent nodes. The adjacency list `adj` must have size equal to the number of distinct nodes (n), not 10000. The task is to implement the core logic of the `Allocation::load` method without file I/O, focusing on building the degree-sorted mapping and adjacency list.
*/
#include <vector>
#include <utility>
#include <algorithm>
#include <map>

// Given a list of directed edges (tail, head), build a graph representation
// where nodes are sorted by decreasing degree (ties by smaller original ID).
// Returns adjacency list (indexed by new node indices, neighbors as new indices)
// and a mapping 'itx' from original node ID to new index (size 10000, -1 for absent).
std::pair<std::vector<std::vector<int>>, std::vector<int>>
allocationFromEdges(const std::vector<std::pair<int,int>>& edges) {
    // Fixed-size mapping as in original code
    const int MAX_ID = 10000;
    std::vector<int> itx(MAX_ID, -1);
    std::vector<int> originalIds;

    // Collect all distinct nodes from both ends of edges
    for (const auto& e : edges) {
        int u = e.first;
        int v = e.second;
        if (itx[u] == -1) {
            itx[u] = 0;  // temporary marker (not final index)
            originalIds.push_back(u);
        }
        if (itx[v] == -1) {
            itx[v] = 0;
            originalIds.push_back(v);
        }
    }

    // Number of distinct nodes
    int n = static_cast<int>(originalIds.size());

    // Compute degrees for each distinct node
    std::map<int,int> degree;  // original ID -> degree
    for (int id : originalIds) degree[id] = 0;
    for (const auto& e : edges) {
        degree[e.first]++;
        degree[e.second]++;
    }

    // Sort distinct original IDs by (degree descending, ID ascending)
    std::sort(originalIds.begin(), originalIds.end(),
        [&](int a, int b) {
            if (degree[a] != degree[b]) return degree[a] > degree[b];  // descending degree
            return a < b;  // tie-break by smaller ID
        });

    // Build mapping from original ID to new index
    for (int i = 0; i < n; ++i) {
        itx[originalIds[i]] = i;
    }

    // Build adjacency list with original neighbor IDs first (to allow sorting)
    std::vector<std::vector<int>> adjOriginal(n);
    for (const auto& e : edges) {
        int u = e.first;   // tail
        int v = e.second;  // head
        adjOriginal[itx[u]].push_back(v);  // store original neighbor ID
    }

    // For each adjacency list, sort by original neighbor ID (ascending)
    for (auto& list : adjOriginal) {
        std::sort(list.begin(), list.end());
    }

    // Map neighbor original IDs to new indices
    std::vector<std::vector<int>> adj(n);
    for (int i = 0; i < n; ++i) {
        adj[i].reserve(adjOriginal[i].size());
        for (int neighbor : adjOriginal[i]) {
            adj[i].push_back(itx[neighbor]);
        }
    }

    // Return adjacency list and the mapping (itx has size 10000)
    return {std::move(adj), std::move(itx)};
}
#include <cassert>
#include <vector>
#include <utility>

// Function prototype (as defined in solution)
std::pair<std::vector<std::vector<int>>, std::vector<int>>
allocationFromEdges(const std::vector<std::pair<int,int>>& edges);

int main() {
    // Test 1: Simple two-node graph with one edge 0->1
    {
        std::vector<std::pair<int,int>> edges = {{0,1}};
        auto result = allocationFromEdges(edges);
        // Distinct nodes: 0 and 1. Both degree 1, tie-break by ID => order 0,1
        // New mapping: 0->0, 1->1
        // Adjacency: node0 has neighbor 1, node1 has empty list
        assert(result.second[0] == 0);
        assert(result.second[1] == 1);
        assert(result.first.size() == 2);
        assert(result.first[0] == std::vector<int>{1});
        assert(result.first[1].empty());
    }

    // Test 2: Disconnected nodes, degrees differ
    {
        // Edges: 2->3 (both degree 1), and 7->8 (both degree 1)
        // Also add duplicate edge to give node 5 degree 2
        std::vector<std::pair<int,int>> edges = {{2,3}, {7,8}, {5,9}, {5,10}};
        auto result = allocationFromEdges(edges);
        // Distinct nodes: 2,3,5,7,8,9,10
        // Degrees: 5=2, others=1. So 5 gets index 0.
        // Others with degree 1 sorted by ID: 2,3,7,8,9,10 => indices 1..6
        assert(result.second[5] == 0);
        assert(result.second[2] == 1);
        assert(result.second[3] == 2);
        assert(result.second[7] == 3);
        assert(result.second[8] == 4);
        assert(result.second[9] == 5);
        assert(result.second[10] == 6);
        // Adjacency for node 5 (original 5) has neighbors 9 and 10 (in that order)
        assert(result.first[0] == std::vector<int>{5, 6}); // new indices of 9 and 10
        // Node 2 (index 1) has neighbor 3 (index 2)
        assert(result.first[1] == std::vector<int>{2});
        // Node 3 has empty list
        assert(result.first[2].empty());
    }

    // Test 3: Duplicate edges preserved, multiple occurrences
    {
        std::vector<std::pair<int,int>> edges = {{1,2}, {1,2}, {1,3}};
        auto result = allocationFromEdges(edges);
        // Nodes: 1 (degree 3), 2 (degree 2), 3 (degree 1)
        // Order: 1,2,3
        assert(result.second[1] == 0);
        assert(result.second[2] == 1);
        assert(result.second[3] == 2);
        // Adjacency of 1: neighbors sorted by original ID: 2,2,3 -> new indices 1,1,2
        assert(result.first[0] == std::vector<int>{1,1,2});
        // Adjacency of 2: empty (only appears as head)
        assert(result.first[1].empty());
        // Adjacency of 3: empty
        assert(result.first[2].empty());
    }

    // Test 4: Self-loop
    {
        std::vector<std::pair<int,int>> edges = {{4,4}};
        auto result = allocationFromEdges(edges);
        // One node, index 0
        assert(result.second[4] == 0);
        assert(result.first.size() == 1);
        // Self-loop: neighbor is 4 -> new index 0
        assert(result.first[0] == std::vector<int>{0});
    }

    // Test 5: Ensure absent nodes have -1 in itx
    {
        std::vector<std::pair<int,int>> edges = {{10,20}};
        auto result = allocationFromEdges(edges);
        assert(result.second[10] == 0);
        assert(result.second[20] == 1);
        assert(result.second[0] == -1);
        assert(result.second[9999] == -1);
        // itx size must be 10000
        assert(result.second.size() == 10000);
    }

    return 0;
}
// The solution mimics the original code's graph construction from an edge list. Steps: (1) Discover all distinct node IDs by iterating over edges, collect them into a set or vector. Use a fixed-size array `itx` of size 10000 initialized to -1. Maintain `xti` (original IDs) and `itx` mapping. In the original code, nodes are added as they appear as donors (tails) in the input, but here we must include heads as well because edges may have heads that are not donors? Actually in the original, only tails become nodes; heads are assumed to be among tails. But to be general, we should include all distinct nodes from both ends. The problem statement says "all distinct nodes from the edge list", so we collect both tails and heads. (2) For each distinct node, compute its degree as the count of edges incident to it (both as tail and head). (3) Sort the distinct nodes by decreasing degree; if tie, sort by increasing original ID (the original code uses `sortpair` which sorts by second (degree) descending, but it doesn't specify a tie-breaker; however, to be deterministic, we'll use the default sort which for equal degrees will preserve the order of insertion? Actually `std::sort` is not stable, so we need a custom comparator that breaks ties by original ID ascending). (4) Build new mapping: for each node in sorted order, assign new index. (5) Build adjacency list: for each edge, map tail and head to new indices, push the mapped head into the adjacency list of the mapped tail. But note: the original code pushes `adj[itx[edges[i][0]]].push_back(itx[edges[i][1]])` — that is, for directed edge (tail, head), it adds head to the adjacency of tail, not the reverse. So we follow that: for each edge (u,v), we add `itx[v]` to `adj[itx[u]]`. (6) Sort each adjacency list by the original ID of the neighbor before mapping? The problem statement says "sorted in ascending order of their original IDs" — but since we have already mapped, we need to sort neighbor original IDs, then map. So for each node, collect the original IDs of neighbors, sort them, then map to new indices. Also note: duplicates are kept, so sorting the original IDs with duplicates is fine. (7) The return pair: first is `adj`, second is `itx` (but `itx` must have size 10000, filled with -1 for absent nodes). However, we also need to map original IDs to new indices; `itx` serves that purpose, but its size is 10000. The problem says "returned `itx` vector must have size equal to the maximum original ID plus one (up to 10000)" — to be safe, we'll make it size 10000 as in original code. Complexity: Let V = number of distinct nodes (≤ 10000), E = number of edges. Time: O(V log V + E log max_deg) for sorting neighbor lists; but we can sort each neighbor list of size deg(v) in O(deg(v) log deg(v)), total O(E log E) in worst case (if all edges from one node). Overall O(E log E + V log V). Space: O(V + E + 10000) for the fixed array.
