// Given an undirected tree with `n` nodes (numbered 1 to `n`) and a positive integer `k`, write a C++ function `bool isCenteredKTree(int n, int k, const std::vector<std::pair<int,int>>& edges)` that returns `true` if the tree is a "centered k-caterpillar": a tree where there exists a unique central node (root) whose distance from every leaf is exactly `k`, every non-leaf node has at least 3 neighbors that are closer to the center (i.e., degree pattern supporting a layered structure), and the center itself has degree at least 3. Equivalently, the tree must have exactly one center at layer `k` (where leaves are layer 0), every internal node at layer `i > 0` must have at least 3 children in layer `i-1`, and all edges must connect nodes whose layer numbers differ by exactly 1. If the tree has fewer than 4 nodes, return `false`. The function must handle arbitrary `n` (up to 100,000) and return `false` for any structure that does not satisfy the conditions.
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Simple path of 4 nodes: 1-2-3-4, k=2? Center would be 2 or 3, not unique with degree >=3 -> false
    assert(isCenteredKTree(4, 2, {{1,2},{2,3},{3,4}}) == false);
    
    // Star with center 1 and 4 leaves: n=5, k=1, center degree 4 -> true
    assert(isCenteredKTree(5, 1, {{1,2},{1,3},{1,4},{1,5}}) == true);
    
    // Same star but k=2 -> false
    assert(isCenteredKTree(5, 2, {{1,2},{1,3},{1,4},{1,5}}) == false);
    
    // n=3 triangle? Not a tree -> false (tree with 3 nodes is a path, not acceptable)
    assert(isCenteredKTree(3, 1, {{1,2},{1,3}}) == false);
    
    // Two-level tree: center 1, four children 2-5, each child has 3 leaf grandchildren.
    // n=1+4+12=17, k=2
    std::vector<std::pair<int,int>> edges;
    int node = 2;
    for (int c = 2; c <= 5; ++c) {
        edges.push_back({1, c});
        for (int j = 0; j < 3; ++j) {
            edges.push_back({c, node});
            node++;
        }
    }
    assert(isCenteredKTree(17, 2, edges) == true);
    
    // Same structure but k=1 -> false because leaves are at distance 2
    assert(isCenteredKTree(17, 1, edges) == false);
    
    // Root has only 2 children (degree 2) -> false even if layers exist
    // n=1+2+6=9, center 1, children 2,3 each with 3 leaves
    std::vector<std::pair<int,int>> edges2;
    node = 4;
    for (int c = 2; c <= 3; ++c) {
        edges2.push_back({1, c});
        for (int j = 0; j < 3; ++j) {
            edges2.push_back({c, node});
            node++;
        }
    }
    assert(isCenteredKTree(9, 2, edges2) == false);
    
    // Edge case: n=100000, large star with center 1 and 99999 leaves, k=1 -> true
    std::vector<std::pair<int,int>> bigEdges;
    for (int i = 2; i <= 100000; ++i) bigEdges.push_back({1, i});
    assert(isCenteredKTree(100000, 1, bigEdges) == true);
    
    return 0;
}
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>

bool isCenteredKTree(int n, int k, const std::vector<std::pair<int,int>>& edges) {
    if (n <= 3) return false;
    std::vector<std::vector<int>> adj(n + 1);
    std::vector<int> deg(n + 1, 0);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
        deg[e.first]++;
        deg[e.second]++;
    }
    
    std::vector<int> layer(n + 1, -1);
    std::vector<std::vector<int>> layers;
    std::queue<int> q;
    for (int i = 1; i <= n; ++i) {
        if (deg[i] == 1) {
            layer[i] = 0;
            q.push(i);
        }
    }
    
    std::vector<int> remaining = deg;
    while (!q.empty()) {
        int size = q.size();
        std::vector<int> current;
        for (int s = 0; s < size; ++s) {
            int u = q.front(); q.pop();
            current.push_back(u);
            for (int v : adj[u]) {
                if (layer[v] == -1) {
                    remaining[v]--;
                    if (remaining[v] == 1) {
                        layer[v] = layer[u] + 1;
                        q.push(v);
                    }
                }
            }
        }
        layers.push_back(current);
    }
    
    int mx = -1;
    for (int i = 0; i < (int)layers.size(); ++i) {
        if (!layers[i].empty()) mx = i;
    }
    if (mx != k) return false;
    if (layers[mx].size() != 1) return false;
    int root = layers[mx][0];
    if ((int)adj[root].size() <= 2) return false;
    
    // Verify all edges connect layers differing by 1
    for (int i = 1; i <= n; ++i) {
        for (int v : adj[i]) {
            if (layer[i] == -1 || layer[v] == -1) return false;
            if (std::abs(layer[i] - layer[v]) != 1) return false;
        }
    }
    
    // Verify every non-leaf node has at least 3 neighbors in the previous layer
    for (int i = 1; i <= n; ++i) {
        if (layer[i] > 1) {
            int cnt = 0;
            for (int v : adj[i]) {
                if (layer[v] == layer[i] - 1) cnt++;
            }
            if (cnt < 3) return false;
        }
    }
    
    return true;
}
// The solution builds a layered decomposition of the tree starting from all leaves (degree 1 nodes) at layer 0. We iteratively peel off the current layer: for each node in the current layer, we decrement the degree of its neighbors; if a neighbor's remaining degree becomes 1 and it is not already assigned a layer, assign it to the next layer. After the peeling process, we find the largest layer index `mx` that has a non-empty list of nodes. Conditions include: `mx` must equal exactly `k`, the top layer must contain exactly one node (the center), the center must have original degree at least 3, and for every node with layer > 1, the number of neighbors with layer one less must be at least 3. Additionally, every edge must connect nodes whose layer difference is exactly 1 (no equal layers or jumps). Time complexity is `O(n)` because each edge is processed a constant number of times (during peeling and later verification). Space complexity is `O(n)` for adjacency lists and layer arrays. Edge cases: small trees (`n <= 3`) are rejected; disconnected graphs are not possible since input is guaranteed a tree; the case where the peeling produces multiple centers or a mismatch with `k` is rejected.
