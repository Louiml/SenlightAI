// Given an undirected graph with `n` vertices (numbered 1 to n) and `m` edges, write a C++ function `solveMaxCut(int n, const vector<pair<int, int>>& edges)` that attempts to find a maximum cut using a local search heuristic. Start with all vertices uncolored (0). Repeatedly scan all vertices; for each vertex, compute the difference `d = (# neighbors with different color) - (# neighbors with same color)`. If `d < 0`, flip the vertex's color (to 1 if it was 0, and vice versa), add `d` to the current cut size, and continue scanning until no vertex can be improved. After the process stabilizes, if the final cut size (number of crossing edges) satisfies `2 * cut <= m`, return a vector containing the indices of vertices colored 1 (in increasing order). Otherwise, return an empty vector to indicate “Impossible.” The function must not modify the input graph; it should work for graphs with up to 100 vertices and any number of edges, including disconnected graphs and self‑loops (self‑loops contribute 0 to any cut; they are given as `u == v` and should be ignored for flipping decisions). The output vector must be sorted in increasing order.
#include <cassert>
#include <vector>

int main() {
    // Case 1: Single edge, trivial
    auto r1 = solveMaxCut(2, {{1,2}});
    // cut=1, m=1, 2*1<=1? no, so empty
    assert(r1.empty());

    // Case 2: Two edges forming a path 1-2-3
    auto r2 = solveMaxCut(3, {{1,2},{2,3}});
    // The heuristic finds max cut of 2 crossing edges; 2*2<=2? no, empty
    assert(r2.empty());

    // Case 3: Triangle, one edge can be cut at most? m=3, max cut=2, 2*2<=3? no
    auto r3 = solveMaxCut(3, {{1,2},{2,3},{3,1}});
    assert(r3.empty());

    // Case 4: Two disjoint edges, m=2, each edge can be cut individually
    // Heuristic will flip one vertex per edge to get cut=2, 2*2<=2? no
    auto r4 = solveMaxCut(4, {{1,2},{3,4}});
    assert(r4.empty());

    // Case 5: Four-cycle (square), m=4, max cut=4 (all edges cross)
    // Heuristic can achieve cut=4, 2*4<=4? no, so empty
    auto r5 = solveMaxCut(4, {{1,2},{2,3},{3,4},{4,1}});
    assert(r5.empty());

    // Case 6: Single isolated vertex, no edges, m=0, cut=0, 2*0<=0 true
    // No vertices colored 1, result empty
    auto r6 = solveMaxCut(1, {});
    assert(r6.empty());

    // Case 7: Two vertices with two parallel edges? We allow multiple edges.
    // m=2, heuristic flips one vertex to cut=2, 2*2<=2? no
    auto r7 = solveMaxCut(2, {{1,2},{1,2}});
    assert(r7.empty());

    // Case 8: Self-loop only, m=1 but self-loop never crosses, cut stays 1? 
    // Actually we ignore self-loops in adjacency, but m counts them.
    // Heuristic: cut=1, 2*1<=1? no -> empty
    auto r8 = solveMaxCut(2, {{1,1}});
    assert(r8.empty());

    // Case 9: Star with 3 leaves, m=3, max cut=3 (center opposite leaves)
    // Heuristic can achieve cut=3, 2*3<=3? no
    auto r9 = solveMaxCut(4, {{1,2},{1,3},{1,4}});
    assert(r9.empty());

    // Case 10: Bipartite complete? Testing known scenario where 2*cut > m?
    // For a single edge, m=1, max cut=1, 2*1<=1 false. For m=0, true but empty result
    // To get a non-empty result, we need 2*cut <= m, which implies cut <= m/2,
    // but max cut for connected graphs is often > m/2. Only when m=0 or specific
    // cases with few cuts. For example, graph with vertex 1 isolated and no edges? m=0, cut=0, but no colored vertices -> empty.
    // So we just test a case where cut=0 and m=0 yields empty. That's covered.

    return 0;
}
#include <vector>
#include <algorithm>

// Performs local search Max Cut. Returns vertex indices colored 1 if cut is
// at least half the edges, otherwise an empty vector.
std::vector<int> solveMaxCut(int n, const std::vector<std::pair<int, int>>& edges) {
    // Build adjacency lists, ignoring self-loops.
    std::vector<std::vector<int>> adj(n + 1); // 1-indexed
    int m = edges.size();
    for (const auto& [u, v] : edges) {
        if (u == v) continue; // self-loop never crosses
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // colors: 0 or 1; initially all 0
    std::vector<int> color(n + 1, 0);
    int cut = m; // initially all edges are uncut (same color)

    bool improved;
    do {
        improved = false;
        for (int i = 1; i <= n; ++i) {
            int d = 0;
            for (int v : adj[i]) {
                if (color[i] == color[v]) d -= 1;
                else d += 1;
            }
            if (d < 0) {
                color[i] ^= 1; // flip
                cut += d; // cut increases by (negative d)
                improved = true;
            }
        }
    } while (improved);

    if (2 * cut <= m) {
        std::vector<int> result;
        for (int i = 1; i <= n; ++i) {
            if (color[i] == 1) result.push_back(i);
        }
        return result;
    }
    return {};
}
// The algorithm implements a simple local search for the Max Cut problem (also known as the “flip” heuristic). Initialize all vertex colors to 0. The current cut size is initially `m` (all edges are monochromatic, so none cross). Then, in a loop, we examine each vertex. For vertex `i`, we compute `d = (#neighbors with color != color[i]) - (#neighbors with color == color[i])`. If `d < 0`, flipping the vertex will increase the cut size by `d` (since `d` is negative, adding it makes the cut larger). We flip the vertex, add `d` to the cut size, and set a flag to continue scanning. The loop repeats until a full pass of all vertices produces no improvement. Each flip strictly increases the cut size by at least 1 (since `d` is an integer and negative), and the cut size is bounded above by `m`, so the loop terminates in at most `m` flips. Edge cases: isolated vertices (degree 0) have `d=0`, so they never flip. Self‑loops are ignored when computing `d` because they never cross; they can be skipped in the adjacency list (or simply handled by not counting them). After stabilisation, we check if `2 * cut <= m`. If so, we collect all vertices with color 1, sort them (they are already processed in order, but we ensure order by building the vector in a for‑loop anyway), and return it. Otherwise, we return an empty vector. Time complexity: each full pass of vertices costs `O(n + total_degree)` (which is `O(n + m)`), and there are at most `m` flips, so worst‑case `O(m(n+m))`. For the constraints (n ≤ 100, m arbitrary but practically small), this is fine. Space complexity is `O(n + m)` for adjacency lists and colors.
