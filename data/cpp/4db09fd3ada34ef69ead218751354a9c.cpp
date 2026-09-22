In a sales distribution network, each member (identified by a unique integer from 0 to N-1) has exactly one immediate supplier, represented by a parent index, except for the root supplier whose parent is -1. The root sells at a base price P, and each subsequent level in the chain increases the price by a fixed percentage r (e.g., 1.80 means 1.8%). Given N, the base price P, the percentage increase r, and an array of parent indices, write a C++ function that computes the highest price among all members and the count of members who achieve that highest price. The price at a member is P × (1+r/100)^(depth), where depth is the number of edges from the root to that member. The output must be the maximum price printed with exactly two decimal places followed by a space and the count of members at that maximum depth. The parent array is guaranteed to form a valid tree (no cycles, exactly one root).
// The problem requires finding the maximum depth from the root in a tree where each node points to its parent. We can compute depths efficiently without recursion to avoid stack overflow for large N. The algorithm processes each node i: if its depth is already known, skip; otherwise, traverse upward through ancestors while tracking the current path length. If we hit a node whose depth is already known, we can compute the remaining depth by adding the known depth plus one for each step taken so far. If we hit -1 (the root's parent), we have walked to the root and know the total depth. After determining the depth for the starting node, we walk back along the same path, assigning decreasing depth values to all unvisited ancestors, since each step closer to the root reduces depth by one. This ensures each node is processed once. We track the maximum depth seen and count how many nodes have that depth. Finally, compute the price as P multiplied by (1+r/100) exactly maxDepth times. Edge cases: N=1 (only root, depth 0), r can be zero (price stays P), all nodes might be direct children of root (depth 1). Time complexity is O(N) because each edge is traversed at most twice (once upward, once back) and each node gets a depth value exactly once. Space complexity is O(N) for the depth and parent arrays.
#include <vector>
#include <cmath>

// Computes the maximum price and the count of members at that maximum depth.
// Parameters:
//   parents - vector of size N, where parents[i] is the supplier index (or -1 for root)
//   basePrice - base price P at the root
//   percentIncrease - annual percentage increase r (e.g., 1.80 means 1.8%)
// Returns:
//   a pair where first is the maximum price (rounded to 2 decimals by the caller)
//   and second is the count of members achieving that maximum depth.
std::pair<double, int> maxPriceAndCount(const std::vector<int>& parents, double basePrice, double percentIncrease) {
    const int N = static_cast<int>(parents.size());
    std::vector<int> depth(N, -1); // -1 means unknown; otherwise stores exact depth from root
    int maxDepth = 0;
    int maxDepthCount = 0;

    for (int i = 0; i < N; ++i) {
        if (depth[i] != -1) {
            continue;
        }

        // Walk up from node i until we find a known depth or reach root's parent.
        int current = i;
        int currentDepth = 0;
        while (true) {
            if (parents[current] == -1) {
                // Reached root, its depth is 0.
                depth[current] = 0;
                break;
            }
            if (depth[parents[current]] != -1) {
                // Parent's depth is known, so current's depth = parent's depth + 1.
                depth[current] = depth[parents[current]] + 1;
                break;
            }
            // Continue upward.
            ++currentDepth;
            current = parents[current];
        }

        // Now depth[current] is known for the node we stopped at.
        // currentDepth holds the number of steps we took from i to reach 'current'.
        int fullDepth = depth[current] + currentDepth;

        // Assign depths for all nodes on the path from i up to 'current'.
        int node = i;
        int relDepth = currentDepth; // relative distance from i
        while (true) {
            if (depth[node] != -1) {
                break; // already set (the stopping node) or previously processed
            }
            depth[node] = fullDepth - relDepth;
            --relDepth;
            if (node == current) {
                break;
            }
            node = parents[node];
        }

        // Update max depth and count if this node's depth is new or ties.
        int d = depth[i];
        if (d > maxDepth) {
            maxDepth = d;
            maxDepthCount = 1;
        } else if (d == maxDepth) {
            ++maxDepthCount;
        }
    }

    // Actually, the loop above only updates count for i, but other nodes might also share maxDepth.
    // However, we only track based on i; but we need to count all nodes with depth == maxDepth.
    // So we should recompute the count after all depths are known:
    maxDepthCount = 0;
    for (int d : depth) {
        if (d == maxDepth) {
            ++maxDepthCount;
        }
    }

    // Compute maximum price = basePrice * (1 + r/100)^maxDepth
    double factor = 1.0 + percentIncrease / 100.0;
    double price = basePrice * std::pow(factor, maxDepth);
    return {price, maxDepthCount};
}
#include <cassert>
#include <cmath>
#include <vector>

// The solution function is assumed to be defined above (in the same translation unit).
// We'll redeclare it for clarity.
std::pair<double, int> maxPriceAndCount(const std::vector<int>& parents, double basePrice, double percentIncrease);

int main() {
    // Sample from the problem statement
    std::vector<int> parents1 = {1, 5, 4, 4, -1, 4, 5, 3, 6};
    auto res1 = maxPriceAndCount(parents1, 1.80, 1.00);
    assert(std::abs(res1.first - 1.85) < 1e-4);
    assert(res1.second == 2);

    // Single node (root only)
    std::vector<int> parents2 = {-1};
    auto res2 = maxPriceAndCount(parents2, 10.0, 5.0);
    assert(std::abs(res2.first - 10.0) < 1e-4);
    assert(res2.second == 1);

    // Linear chain: 0->1->2->3 (root is 3)
    std::vector<int> parents3 = {1, 2, 3, -1};
    auto res3 = maxPriceAndCount(parents3, 100.0, 0.0); // 0% increase
    assert(std::abs(res3.first - 100.0) < 1e-4); // depth 3 but 0% => same price
    assert(res3.second == 1);

    // Two children of root at depth 1
    std::vector<int> parents4 = {-1, 0, 0};
    auto res4 = maxPriceAndCount(parents4, 50.0, 100.0);
    assert(std::abs(res4.first - 100.0) < 1e-4); // 50 * (1+1) = 100
    assert(res4.second == 2);

    // All nodes at same depth (root's direct children)
    std::vector<int> parents5 = {-1, 0, 0, 0};
    auto res5 = maxPriceAndCount(parents5, 2.0, 50.0);
    assert(std::abs(res5.first - 3.0) < 1e-4); // 2 * 1.5
    assert(res5.second == 3);

    return 0;
}
