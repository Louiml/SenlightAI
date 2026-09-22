// You are given a rooted tree with `n` nodes, labeled from `1` to `n`, where node `1` is the root. Each node `i` has a positive integer energy value `S[i]`. Each non-root node `i` (for `i >= 2`) has a parent `p[i]` and an edge length `L[i]` (the distance from node `i` to its parent). For each node `u`, starting from `u` and moving upward toward the root, the node consumes its own energy `S[u]` to traverse edges: it can move from a node `v` to its parent `J0[v]` as long as there is enough remaining energy to cover the edge length `L[v]`. When it can no longer move upward (either because it reaches the root or the next edge is longer than remaining energy), it stops at that node and leaves a mark. Write a C++ function `std::vector<int> countMarks(int n, const std::vector<long long>& S, const std::vector<int>& parent, const std::vector<long long>& edgeLen)` that returns, for each node `1..n`, the number of marks that land on that node. Note that a node's own energy is consumed only when moving from that node upward, and each node's journey is independent. Edge lengths and energies are positive and fit in 64-bit integers. The tree may be deep (up to 200,000 nodes), so your solution must be efficient.

#include <cassert>
#include <vector>

// Include the solution function declaration or implementation here (as above)

int main() {
    // Test 1: Simple chain 1-2-3 with lengths 1,1 and energy all 1
    {
        int n = 3;
        std::vector<long long> S = {1, 1, 1};
        std::vector<int> parent = {0, 1, 2}; // parent[1] unused, parent[2]=1, parent[3]=2
        std::vector<long long> edgeLen = {0, 1, 1}; // edgeLen[1] unused, edgeLen[2]=1, edgeLen[3]=1
        std::vector<int> result = countMarks(n, S, parent, edgeLen);
        // Node 1: node1 can't move, node2 can move to 1, node3 can move to 2 (not 1 because would need 2 energy)
        // So ends: node1 gets 1, node2 gets 1, node3 gets 1
        // Subtree sums: node1 = 3, node2 = 2, node3 = 1
        assert(result.size() == 3);
        assert(result[0] == 3); // node1
        assert(result[1] == 2); // node2
        assert(result[2] == 1); // node3
    }

    // Test 2: Root has large energy, all others reach root
    {
        int n = 4;
        std::vector<long long> S = {100, 1, 1, 1};
        std::vector<int> parent = {0, 1, 1, 2}; // 2→1, 3→1, 4→2
        std::vector<long long> edgeLen = {0, 1, 1, 1};
        std::vector<int> result = countMarks(n, S, parent, edgeLen);
        // All non-root can reach root because edge lengths are 1 and energy is 1
        // Node1 ends at 1, node2 ends at 1, node3 ends at 1, node4 moves to 2 then 1 (needs 2 energy, but has 1 so stops at 2)
        // Actually node4: energy=1, edge to parent 2 is length 1, so moves to node2, remaining=0, stops at 2
        // So ends: node1 gets 3 (1,2,3), node2 gets 1 (4)
        // Subtree sums: node1 = 4, node2 = 1, node3 = 1, node4 = 1
        assert(result[0] == 4);
        assert(result[1] == 1); // node2
        assert(result[2] == 1); // node3
        assert(result[3] == 1); // node4
    }

    // Test 3: Star tree with root and 3 children, each child has large energy
    {
        int n = 4;
        std::vector<long long> S = {0, 10, 10, 10};
        std::vector<int> parent = {0, 1, 1, 1};
        std::vector<long long> edgeLen = {0, 5, 5, 5};
        std::vector<int> result = countMarks(n, S, parent, edgeLen);
        // Each child can reach root (energy 10 ≥ 5). Root stops at root.
        // Ends: root gets 1+3=4, children get 0
        // Subtree sums: root=4, children=0 each
        assert(result[0] == 4);
        assert(result[1] == 0);
        assert(result[2] == 0);
        assert(result[3] == 0);
    }

    // Test 4: Single node
    {
        int n = 1;
        std::vector<long long> S = {5};
        std::vector<int> parent = {0};
        std::vector<long long> edgeLen = {0};
        std::vector<int> result = countMarks(n, S, parent, edgeLen);
        assert(result.size() == 1);
        assert(result[0] == 1);
    }

    // Test 5: Long chain, energy insufficient to cross all edges
    {
        int n = 5;
        std::vector<long long> S = {0, 2, 2, 2, 2};
        std::vector<int> parent = {0, 1, 2, 3, 4};
        std::vector<long long> edgeLen = {0, 1, 1, 1, 1};
        std::vector<int> result = countMarks(n, S, parent, edgeLen);
        // Node1: ends at 1
        // Node2: moves to 1 (cost 1) ends at 1
        // Node3: moves to 1? cost from 3 to 1 is 2, but energy=2, so yes reaches 1
        // Node4: moves to 2 (cost 1), then to 1 (cost 1) total 2, reaches 1
        // Node5: moves to 3 (cost1), then to 2 (cost1) total 2, stops at 2
        // Ends: node1 gets 1,2,3,4 = 4; node2 gets 5 = 1; others 0
        // Subtree sums: node1=5, node2=1, node3=0, node4=0, node5=0
        assert(result[0] == 5);
        assert(result[1] == 1);
        assert(result[2] == 0);
        assert(result[3] == 0);
        assert(result[4] == 0);
    }

    return 0;
}

#include <vector>
#include <algorithm>
#include <cstdint>

// Given a rooted tree (root=1), for each node u we spend its energy S[u] to move upward as far as possible.
// Return a vector R where R[i] = number of distinct starting nodes whose upward journey passes through or ends at node i.
std::vector<int> countMarks(int n, const std::vector<long long>& S, const std::vector<int>& parent, const std::vector<long long>& edgeLen) {
    const int LOG = 20; // since n <= 200,000, log2(200,000) < 18, but use 20 for safety
    std::vector<std::vector<int>> up(LOG, std::vector<int>(n + 1, 0));
    std::vector<std::vector<long long>> dist(LOG, std::vector<long long>(n + 1, 0));

    // Initialize level 0: direct parent and edge length
    for (int i = 1; i <= n; ++i) {
        if (i == 1) {
            // Root has no parent; we'll never move beyond it
            up[0][i] = i;
            dist[0][i] = 0;
        } else {
            up[0][i] = parent[i];
            dist[0][i] = edgeLen[i];
        }
    }

    // Build binary lifting tables
    for (int k = 1; k < LOG; ++k) {
        for (int v = 1; v <= n; ++v) {
            int mid = up[k-1][v];
            up[k][v] = up[k-1][mid];
            dist[k][v] = dist[k-1][v] + dist[k-1][mid];
        }
    }

    // cnt[i] = number of starting nodes that end exactly at i
    std::vector<int> cnt(n + 1, 0);

    // For each node, simulate upward movement
    for (int u = 1; u <= n; ++u) {
        long long remaining = S[u];
        int current = u;
        // Greedily jump using the largest possible power of two
        for (int k = LOG - 1; k >= 0; --k) {
            if (dist[k][current] <= remaining) {
                remaining -= dist[k][current];
                current = up[k][current];
            }
        }
        // current is the node where the journey stops
        cnt[current]++;
    }

    // Build children lists
    std::vector<std::vector<int>> children(n + 1);
    for (int i = 2; i <= n; ++i) {
        children[parent[i]].push_back(i);
    }

    // R[u] = cnt[u] + sum of R[child] (subtree sum)
    std::vector<int> R(n + 1, 0);

    // Iterative DFS to avoid recursion depth issues (using post-order)
    std::vector<int> order;
    order.reserve(n);
    std::vector<int> stack = {1};
    std::vector<int> parentStack(n + 1, 0);
    parentStack[1] = 0;
    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        order.push_back(u);
        for (int v : children[u]) {
            parentStack[v] = u;
            stack.push_back(v);
        }
    }
    // Reverse order: children before parents
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        int u = *it;
        R[u] = cnt[u];
        for (int v : children[u]) {
            R[u] += R[v];
        }
    }

    // Return vector index 1..n (we ignore index 0)
    R.erase(R.begin()); // now R[0] corresponds to original node 1, etc.
    return R;
}

// We need to simulate for each node `u` the highest ancestor reachable by walking upward as far as possible given the cumulative edge lengths. This is a classic "jump up while the sum of edge lengths ≤ S[u]" problem. We can precompute binary lifting tables `J[k][v]` (the `2^k`-th ancestor) and `L[k][v]` (the total edge length from `v` up to `J[k][v]`). For each node `u`, we want to find the highest ancestor `a` such that the sum of edge lengths from `u` to `a` is ≤ `S[u]`. We do this by greedily jumping upward using the largest `k` where `L[k][current] ≤ remaining_energy`; subtract that length and move to `J[k][current]`. After we cannot move further, we increment a counter for that final node. After processing all nodes, we have an array `cnt[node]` giving how many journeys end at each node. Then we compute the subtree sums: for each node, the number of marks that pass through it (including those that end at it) is the sum of `cnt` over its subtree (including itself). This can be done with a DFS: `R[u] = cnt[u] + sum(R[child])`. Edge cases: root has no parent, so its journey always stops at root (set its edge length to a huge value or handle separately). Also, if `S[u]` is very large, it can reach the root. Time complexity is O(n log n) for preprocessing and O(n log n) for all queries, plus O(n) for DFS. Space O(n log n) for tables.
