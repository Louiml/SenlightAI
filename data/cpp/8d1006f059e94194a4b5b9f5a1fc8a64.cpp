Given a set of transactions where each transaction has a unique identifier (a 64-bit unsigned integer), a size in bytes (a positive integer), and a fee in satoshis (a non-negative integer), along with a set of parent-child dependencies (a parent transaction must be confirmed before any of its children), write a C++ function that computes, for each transaction, the total size and total fee of all its descendants (including itself). A descendant of a transaction is any transaction reachable by following child links from that transaction. The function should accept a vector of transaction IDs, a map from transaction ID to a struct containing its size and fee, and a vector of pairs representing directed edges (parent ID, child ID). The function should return a map from transaction ID to a struct containing the computed descendant size and fee. Input is guaranteed to be acyclic (a valid DAG). If a transaction ID appears in the edge list but not in the provided transaction map, ignore that edge. The resulting map should only contain entries for IDs that appear in the provided transaction map, with the descendant aggregates computed using only IDs that appear in the transaction map.

// The core problem is computing descendant aggregates in a directed acyclic graph (DAG). The naive approach would be to perform a DFS from each node, but that would be \(O(V(V+E))\) and could fail on large inputs. Instead, we can use dynamic programming with memoization (or a topological sort) to compute the descendant aggregate for each node. Since the graph is a DAG, we can process nodes in reverse topological order (children before parents). For each node, we first recursively compute (or look up) the descendant aggregates of all its children, then combine them. However, the problem requires the aggregate to include the node itself and all reachable descendants, but not double-count nodes that are reachable through multiple paths. A simple sum of children’s descendant aggregates would overcount shared descendants. To handle that, we need to collect the set of all descendant IDs (including self) for each node, then sum their sizes and fees. But storing full sets for every node could be memory intensive. A cleaner approach is to use a DFS with memoization that returns both the aggregate size/fee and a set of descendant IDs. Since the graph is a DAG, we can safely memoize the set of descendants for each node. After computing for all nodes, we can deduplicate by using an unordered_set per node (or a more compact structure). For simplicity and correctness, we use memoized DFS returning a set of IDs. Then we iterate over all IDs in the transaction map, compute the aggregate from the set (including self), and store it in the result map. The time complexity is \(O((V+E) \cdot \alpha)\) where \(\alpha\) is the average cost of set operations (typically \(O(\log V)\) or \(O(1)\) average with unordered_set), but the total work across all nodes is bounded by the sum of descendant counts, which could be \(O(V^2)\) in the worst case (e.g., a chain). However, because we memoize, each node's descendant set is computed once, and when we combine child sets, we merge them, which could be expensive. A more efficient approach is to use bitmasks or a global topological order and prefix sums, but that is complex. For the scope of this task, memoized DFS with unordered_set is acceptable and clearly correct. Edge cases: a transaction with no children has only itself as descendant; cycles are not present; edges referencing missing IDs are ignored; the result map must contain every ID from the provided map.

#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>

struct TxInfo {
    uint64_t size;
    uint64_t fee;
};

// Result structure: descendant aggregate including self
struct AggregateInfo {
    uint64_t totalSize;
    uint64_t totalFee;
};

// Internal helper: recursive function with memoization
static void collectDescendants(
    uint64_t txid,
    const std::unordered_map<uint64_t, TxInfo>& txMap,
    const std::unordered_map<uint64_t, std::vector<uint64_t>>& children,
    std::unordered_map<uint64_t, std::unordered_set<uint64_t>>& memo,
    std::unordered_set<uint64_t>& resultSet)
{
    // If already computed, return cached result
    auto it = memo.find(txid);
    if (it != memo.end()) {
        resultSet.insert(it->second.begin(), it->second.end());
        return;
    }

    // Start with self
    std::unordered_set<uint64_t> localSet;
    localSet.insert(txid);

    // Process children recursively
    auto childIt = children.find(txid);
    if (childIt != children.end()) {
        for (uint64_t childId : childIt->second) {
            // If child not in txMap, ignore (shouldn't happen due to preprocessing)
            if (txMap.find(childId) == txMap.end()) continue;
            std::unordered_set<uint64_t> childSet;
            collectDescendants(childId, txMap, children, memo, childSet);
            localSet.insert(childSet.begin(), childSet.end());
        }
    }

    // Memoize and copy to result
    memo[txid] = localSet;
    resultSet.insert(localSet.begin(), localSet.end());
}

// Main solution function: compute descendant aggregates for all transactions
std::unordered_map<uint64_t, AggregateInfo> computeDescendantAggregates(
    const std::vector<uint64_t>& txids,
    const std::unordered_map<uint64_t, TxInfo>& txMap,
    const std::vector<std::pair<uint64_t, uint64_t>>& edges)
{
    // Build children adjacency list, filtering out edges with missing IDs
    std::unordered_map<uint64_t, std::vector<uint64_t>> children;
    for (const auto& edge : edges) {
        uint64_t parent = edge.first;
        uint64_t child = edge.second;
        // Only keep edge if both parent and child exist in txMap
        if (txMap.find(parent) != txMap.end() && txMap.find(child) != txMap.end()) {
            children[parent].push_back(child);
        }
    }

    // Memoization map: txid -> set of descendant IDs (including self)
    std::unordered_map<uint64_t, std::unordered_set<uint64_t>> memo;

    // Compute descendant set for each transaction
    std::unordered_map<uint64_t, std::unordered_set<uint64_t>> descendantSets;
    for (uint64_t txid : txids) {
        std::unordered_set<uint64_t> resultSet;
        collectDescendants(txid, txMap, children, memo, resultSet);
        descendantSets[txid] = std::move(resultSet);
    }

    // Build aggregate map
    std::unordered_map<uint64_t, AggregateInfo> result;
    for (const auto& entry : txMap) {
        uint64_t txid = entry.first;
        const auto& dsIt = descendantSets.find(txid);
        if (dsIt == descendantSets.end()) continue; // defensive, should not happen

        uint64_t totalSize = 0;
        uint64_t totalFee = 0;
        for (uint64_t descId : dsIt->second) {
            const auto& infoIt = txMap.find(descId);
            if (infoIt != txMap.end()) {
                totalSize += infoIt->second.size;
                totalFee += infoIt->second.fee;
            }
        }
        result[txid] = {totalSize, totalFee};
    }

    return result;
}

#include <cassert>
#include <iostream>
#include <unordered_map>
#include <vector>

// Assume the solution function and structs are declared/defined above

int main() {
    // Test 1: Simple chain: A -> B -> C
    std::vector<uint64_t> txids1 = {1, 2, 3};
    std::unordered_map<uint64_t, TxInfo> txMap1 = {
        {1, {10, 100}},
        {2, {20, 200}},
        {3, {30, 300}}
    };
    std::vector<std::pair<uint64_t, uint64_t>> edges1 = {{1, 2}, {2, 3}};
    auto res1 = computeDescendantAggregates(txids1, txMap1, edges1);
    assert(res1[1].totalSize == 60 && res1[1].totalFee == 600);
    assert(res1[2].totalSize == 50 && res1[2].totalFee == 500);
    assert(res1[3].totalSize == 30 && res1[3].totalFee == 300);

    // Test 2: Diamond: A -> B, A -> C, B -> D, C -> D
    std::vector<uint64_t> txids2 = {10, 20, 30, 40};
    std::unordered_map<uint64_t, TxInfo> txMap2 = {
        {10, {1, 1}},
        {20, {2, 2}},
        {30, {4, 4}},
        {40, {8, 8}}
    };
    std::vector<std::pair<uint64_t, uint64_t>> edges2 = {{10, 20}, {10, 30}, {20, 40}, {30, 40}};
    auto res2 = computeDescendantAggregates(txids2, txMap2, edges2);
    // Descendants of 10: {10,20,30,40} sizes: 1+2+4+8=15, fees: 1+2+4+8=15
    assert(res2[10].totalSize == 15 && res2[10].totalFee == 15);
    // Descendants of 20: {20,40} sizes: 2+8=10, fees: 2+8=10
    assert(res2[20].totalSize == 10 && res2[20].totalFee == 10);
    // Descendants of 30: {30,40} sizes: 4+8=12, fees: 4+8=12
    assert(res2[30].totalSize == 12 && res2[30].totalFee == 12);
    // Descendants of 40: {40} sizes: 8, fees: 8
    assert(res2[40].totalSize == 8 && res2[40].totalFee == 8);

    // Test 3: Isolated nodes, no edges
    std::vector<uint64_t> txids3 = {5, 6};
    std::unordered_map<uint64_t, TxInfo> txMap3 = {{5, {50, 500}}, {6, {60, 600}}};
    std::vector<std::pair<uint64_t, uint64_t>> edges3;
    auto res3 = computeDescendantAggregates(txids3, txMap3, edges3);
    assert(res3[5].totalSize == 50 && res3[5].totalFee == 500);
    assert(res3[6].totalSize == 60 && res3[6].totalFee == 600);

    // Test 4: Edge referencing missing child is ignored
    std::vector<uint64_t> txids4 = {1, 2};
    std::unordered_map<uint64_t, TxInfo> txMap4 = {{1, {10, 100}}, {2, {20, 200}}};
    std::vector<std::pair<uint64_t, uint64_t>> edges4 = {{1, 99}, {2, 3}}; // both missing child IDs
    auto res4 = computeDescendantAggregates(txids4, txMap4, edges4);
    assert(res4[1].totalSize == 10 && res4[1].totalFee == 100);
    assert(res4[2].totalSize == 20 && res4[2].totalFee == 200);

    // Test 5: Multiple parents and disjoint subtrees
    std::vector<uint64_t> txids5 = {1, 2, 3, 4, 5};
    std::unordered_map<uint64_t, TxInfo> txMap5 = {
        {1, {10, 10}},
        {2, {20, 20}},
        {3, {30, 30}},
        {4, {40, 40}},
        {5, {50, 50}}
    };
    std::vector<std::pair<uint64_t, uint64_t>> edges5 = {{1, 2}, {2, 4}, {1, 3}, {3, 4}, {3, 5}};
    auto res5 = computeDescendantAggregates(txids5, txMap5, edges5);
    // Descendants of 1: {1,2,3,4,5} sizes: 10+20+30+40+50=150, fees same
    assert(res5[1].totalSize == 150 && res5[1].totalFee == 150);
    // Descendants of 2: {2,4} sizes: 20+40=60
    assert(res5[2].totalSize == 60 && res5[2].totalFee == 60);
    // Descendants of 3: {3,4,5} sizes: 30+40+50=120
    assert(res5[3].totalSize == 120 && res5[3].totalFee == 120);
    // Descendants of 4: {4} sizes: 40
    assert(res5[4].totalSize == 40 && res5[4].totalFee == 40);
    // Descendants of 5: {5} sizes: 50
    assert(res5[5].totalSize == 50 && res5[5].totalFee == 50);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
