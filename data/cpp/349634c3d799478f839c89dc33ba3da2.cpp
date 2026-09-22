You are given an undirected graph with `n` vertices (numbered 1 to n) and `m` edges. Write a C++ function that takes `n` and a vector of edges (each edge as a pair of 1-based endpoints) and returns the size of the largest matching in the graph. A matching is a set of edges where no two edges share a common vertex. The graph may contain multiple edges between the same pair of vertices and self-loops; self-loops cannot be used in a matching, and duplicate edges behave like a single edge for matching purposes. The function must handle both connected and disconnected graphs, and be efficient for `n` up to 10,000 and `m` up to 50,000.

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test 1: Empty graph
    assert(maximumMatchingSize(5, {}) == 0);

    // Test 2: Single edge
    assert(maximumMatchingSize(2, {{1,2}}) == 1);

    // Test 3: Path of 3 vertices
    assert(maximumMatchingSize(3, {{1,2}, {2,3}}) == 1);

    // Test 4: Triangle (odd cycle) - maximum matching size 1
    assert(maximumMatchingSize(3, {{1,2}, {2,3}, {1,3}}) == 1);

    // Test 5: Square (even cycle) - maximum matching size 2
    assert(maximumMatchingSize(4, {{1,2}, {2,3}, {3,4}, {4,1}}) == 2);

    // Test 6: Star with center 1 and 3 leaves - max matching 1
    assert(maximumMatchingSize(4, {{1,2}, {1,3}, {1,4}}) == 1);

    // Test 7: Disconnected edges
    assert(maximumMatchingSize(6, {{1,2}, {3,4}, {5,6}}) == 3);

    // Test 8: Self-loops ignored and duplicate edges
    assert(maximumMatchingSize(4, {{1,1}, {1,2}, {2,1}, {3,4}}) == 2);

    // Test 9: Complete graph K5 - max matching size 2 (floor(5/2))
    assert(maximumMatchingSize(5, {{1,2},{1,3},{1,4},{1,5},{2,3},{2,4},{2,5},{3,4},{3,5},{4,5}}) == 2);

    // Test 10: Complex graph where blossom is needed (e.g., a pentagon with a tail)
    // Pentagon 1-2-3-4-5-1 plus edge 3-6 and 6-7
    assert(maximumMatchingSize(7, {{1,2},{2,3},{3,4},{4,5},{5,1},{3,6},{6,7}}) == 3);

    return 0;
}

#include <vector>
#include <queue>
#include <algorithm>

class BlossomMatching {
    int n;
    std::vector<std::vector<int>> adj;
    std::vector<int> match, p, base, used, blossom;
    std::queue<int> q;
    int timestamp;

public:
    BlossomMatching(int n) : n(n), adj(n), match(n, -1), p(n, -1), base(n), used(n, 0), blossom(n, -1), timestamp(0) {}

    void addEdge(int u, int v) {
        if (u == v) return; // self-loop ignored
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int maxMatching() {
        int result = 0;
        for (int i = 0; i < n; ++i) {
            if (match[i] == -1) {
                result += bfsFindAugmenting(i) ? 1 : 0;
            }
        }
        return result;
    }

private:
    int lca(int a, int b) {
        static std::vector<int> used_lca;
        used_lca.assign(n, 0);
        ++timestamp;
        while (true) {
            a = base[a];
            used_lca[a] = timestamp;
            if (match[a] == -1) break;
            a = p[match[a]];
        }
        while (true) {
            b = base[b];
            if (used_lca[b] == timestamp) return b;
            b = p[match[b]];
        }
    }

    void markPath(int v, int b, int child) {
        while (base[v] != b) {
            blossom[base[v]] = blossom[base[match[v]]] = child;
            p[v] = child;
            child = match[v];
            v = p[match[v]];
        }
    }

    void contractBlossom(int a, int b) {
        int c = lca(a, b);
        static int blossom_id = 0;
        ++blossom_id;
        // Reset blossom markers for this step
        std::fill(blossom.begin(), blossom.end(), -1);
        markPath(a, c, b);
        markPath(b, c, a);
        for (int v = 0; v < n; ++v) {
            if (blossom[base[v]] == blossom_id) {
                base[v] = c;
                if (used[v] == 0) {
                    used[v] = 1;
                    q.push(v);
                }
            }
        }
    }

    bool bfsFindAugmenting(int root) {
        std::fill(p.begin(), p.end(), -1);
        std::fill(base.begin(), base.end(), 0);
        for (int i = 0; i < n; ++i) base[i] = i;
        std::fill(used.begin(), used.end(), 0);
        std::queue<int>().swap(q);
        used[root] = 1;
        q.push(root);

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int u : adj[v]) {
                if (base[v] == base[u] || match[v] == u) continue;
                if (u == root || (match[u] != -1 && p[match[u]] != -1)) {
                    contractBlossom(v, u);
                } else if (p[u] == -1) {
                    p[u] = v;
                    if (match[u] == -1) {
                        // augmenting path found
                        while (u != -1) {
                            int pv = p[u];
                            int nv = match[pv];
                            match[u] = pv;
                            match[pv] = u;
                            u = nv;
                        }
                        return true;
                    }
                    used[match[u]] = 1;
                    q.push(match[u]);
                }
            }
        }
        return false;
    }
};

// Main solution function: returns size of maximum matching in an undirected graph.
int maximumMatchingSize(int n, const std::vector<std::pair<int, int>>& edges) {
    BlossomMatching bm(n);
    for (const auto& e : edges) {
        bm.addEdge(e.first - 1, e.second - 1); // convert to 0-based
    }
    return bm.maxMatching();
}

// The core problem is finding a maximum cardinality matching in a general undirected graph (not necessarily bipartite). The standard polynomial-time algorithm for this is Edmonds' blossom algorithm, which handles odd-length cycles (blossoms) by contracting them and then recursing. The algorithm works by repeatedly finding augmenting paths via BFS/DFS with a special structure for blossoms.  
// We implement a known efficient version of Edmonds' algorithm using BFS and a union-find (disjoint set) structure for blossom contraction, tracking matching partners and labels.  
// - Initialize all vertices as unmatched.  
// - For each vertex not yet matched, attempt to find an augmenting path. If found, alternate matched/unmatched edges along the path to increase matching size by 1.  
// - The algorithm maintains `match[v]` = partner or -1, `p[v]` = parent in BFS tree, `base[v]` = base of blossom, `used[v]` = BFS state.  
// - Edge case: self-loops are ignored, duplicate edges are processed once (we can store as a set or just process them normally since the algorithm treats them as parallel edges; but they don’t affect the answer beyond the first).  
// Time complexity: O(n^3) worst-case for the blossom algorithm (often much faster in practice). Space: O(n + m) for adjacency and auxiliary arrays.  
// After computing the maximum matching size `max_match`, the complement `n - max_match` is not needed here because the task returns the size of the largest matching directly.
