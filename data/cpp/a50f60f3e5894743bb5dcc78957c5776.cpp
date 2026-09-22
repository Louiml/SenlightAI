Given a weighted undirected graph with `n` vertices (labeled 1 to n) and `m` edges, write a C++ function `int countEssentialEdges(int n, const std::vector<std::tuple<int, int, int>>& edges)` that returns the number of edges that are **not** part of any maximum spanning tree (MST variant that maximizes total weight). In other words, count the edges that are **excluded** from every maximum spanning tree of the graph. The graph is connected. The edges are given as tuples `(weight, u, v)` with 0-based vertex indices (0 ≤ u, v < n) and positive integer weights. If there are multiple edges or self-loops, they should be treated as separate edges. The function must be self-contained and avoid modifying the input vector. For example, if a graph has 3 vertices and edges: (5,0,1), (5,1,2), (4,0,2), the maximum spanning tree picks both weight-5 edges, so the weight-4 edge is not in any maximum spanning tree, so the answer is 1. If the graph has two parallel edges of equal maximum weight, both are essential for different trees but only one can appear in a given tree; however, each edge is included in some maximum spanning tree, so neither is excluded, hence answer 0. Count edges that are **never** selected in any maximum spanning tree.

The key idea is to process edges in descending order of weight, but to determine for each weight class which edges are "essential" for maintaining connectivity in a maximum spanning forest. For a given weight `w`, all edges of weight `w` are candidates. Build the DSU components that have been formed by heavier edges. Among the edges of weight `w`, we need to know which ones can be safely omitted while still creating a forest that achieves maximum total weight. A known property: an edge is in some MST (or maximum spanning tree) if and only if it is not the maximum-weight edge in any cycle where all other edges are strictly heavier. Here, we are counting edges that are **never** in any maximum spanning tree, which are exactly those edges that are the unique maximum-weight edge on some cycle formed by edges of the same or heavier weight. In the algorithm: group edges by weight (descending). For each weight group, first count how many distinct connected components are spanned by all edges in the group (using DSU before merging). Then for each edge in the group, if its endpoints already belong to the same component before merging, that edge cannot be added without creating a cycle among equal-weight edges, meaning it is not essential—but we still need to decide if it can appear in some maximum spanning tree. The trick from Kruskal’s algorithm with equal weights: within a weight class, edges that connect different components are all equally valid and every such edge is included in *some* maximum spanning tree, because you can always choose any spanning forest among them; if you have `k` components to merge and `e` edges that connect them, the number of edges that must be chosen is `k-1` but every such edge is part of some maximum spanning tree. The only edges that are never chosen are those that, even when all edges of that weight are considered, do not reduce the number of connected components—i.e., they are internal to a component after merging all edges of that weight. The provided code counts `ans` as the number of edges that are **not** used in the maximum spanning tree (i.e., edges that are not in the chosen greedy set). However, that code counts edges that are in the group but after merging all edges of that weight, they are skipped because they form a cycle; but it also counts edges that were already connected by heavier edges (these are definitely not in any maximum spanning tree). The correct interpretation for "not in any maximum spanning tree" is an edge that is never part of any MST. That is exactly the count of edges that are *not* selected in the greedy Kruskal algorithm when processing weights descending, **and** are not needed to maintain connectivity in any equivalent maximum spanning tree. The most efficient method: sort edges descending, process equal-weight groups. For a group, first compute the set of components that are connected by all edges in the group (by temporarily merging them into a copy of DSU). Then, for each edge, if it connects two different components in the DSU of heavier edges, it is a candidate; but if it connects vertices that are already in the same component after considering all heavier edges **and** all other edges of the same weight, then that edge is never chosen. The standard algorithm: for each weight group, use DSU to find, for each edge, whether it joins two different components. Let `cnt` be the number of successful unions we can do with all edges of this weight (i.e., number of edges that will be selected in some maximum spanning tree). But to count edges that are *not* in any maximum spanning tree, we count edges that are not selected by the greedy algorithm, which is exactly the total number of edges in the group minus the number of edges that are actually selected (i.e., that join different components in the DSU after we have processed heavier edges). However, care: if two edges of same weight are parallel between the same components, only one is selected, but the other could be selected in an alternative maximum spanning tree. Actually, if there are two parallel edges of same weight between two components, both are in *some* maximum spanning tree: you can choose one or the other. So the number of edges not in any maximum spanning tree is exactly the number of edges that, after merging all heavier edges, have endpoints in the same connected component **and** cannot be used to connect distinct components even if we ignore all other edges of the same weight. The provided code counts `ans` as the number of edges that are not used in its greedy selection, but that is not correct for the "never in any" property. Instead, we need to count edges that are **redundant**: they do not reduce the number of connected components when considered with all edges of equal weight. The known solution: process weights descending. For each weight, let `total` be the number of edges of that weight. Then, create a temporary DSU copy (or use a "check" pointer). For each edge, if in the temporary DSU (which already includes all heavier edges) the two endpoints are in the same component, then that edge is definitely not in any maximum spanning tree (because heavier edges already connect them, and adding this edge would form a cycle with strictly heavier edges, so it can be replaced). Count such edges. Then for the remaining edges that connect different temporary components, we need to merge them into the main DSU. However, among these remaining edges, some might not be in any maximum spanning tree if they are only usable for cycles with other edges of the same weight. But if an edge connects two different components in the temporary DSU, then it is part of *some* maximum spanning tree: you can always include it and still maintain a forest because it connects two distinct components. Even if there are multiple parallel edges, each of them is in some maximum spanning tree by choosing that one. So the correct algorithm: For each weight group, first pre-check each edge: if `find(u) == find(v)` in the main DSU (which already contains all heavier edges), then this edge is definitely not in any maximum spanning tree, because it would create a cycle with heavier edges. Increase answer by 1. For the remaining edges (where endpoints are in different main DSU components), they will all be included in *some* maximum spanning tree, regardless of how many we choose. So we don't increase answer for them. Then merge them all into the main DSU (in any order, but use the main DSU; after merging, some edges that originally had different components may become internal, but that doesn't matter because we already decided they are in some tree). Thus, we only count edges that are obviously excluded due to heavier connections. Time complexity: O(n + m log m) for sorting, plus O(m α(n)) for DSU operations. Space: O(n + m).

#include <bits/stdc++.h>
using namespace std;

// Counts edges that are not part of any maximum spanning tree.
// edges: tuples of (weight, u, v) with 0-based vertices.
int countNeverInMaxSpanningTree(int n, const vector<tuple<int, int, int>>& edges) {
    vector<tuple<int, int, int>> e = edges; // copy to sort
    sort(e.begin(), e.end(), greater<tuple<int, int, int>>());

    vector<int> parent(n);
    iota(parent.begin(), parent.end(), 0);
    function<int(int)> find = [&](int x) -> int {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    };
    auto unite = [&](int a, int b) {
        a = find(a);
        b = find(b);
        if (a != b) parent[a] = b;
    };

    int answer = 0;
    int m = e.size();
    int i = 0;
    while (i < m) {
        int j = i;
        int weight = get<0>(e[i]);
        // Process all edges of this weight.
        vector<tuple<int, int, int>> group;
        while (j < m && get<0>(e[j]) == weight) {
            group.push_back(e[j]);
            ++j;
        }
        // First pass: mark edges that connect vertices already in the same component
        // using only heavier edges. Such edges are never in any maximum spanning tree.
        for (const auto& edge : group) {
            int u = get<1>(edge), v = get<2>(edge);
            if (find(u) == find(v)) {
                ++answer;
            }
        }
        // Second pass: merge all edges of this weight into DSU.
        // All remaining edges (that connected different components) are in some
        // maximum spanning tree, so we don't count them.
        for (const auto& edge : group) {
            int u = get<1>(edge), v = get<2>(edge);
            unite(u, v);
        }
        i = j;
    }
    return answer;
}

#include <cassert>
#include <vector>
#include <tuple>
using namespace std;

int countNeverInMaxSpanningTree(int n, const vector<tuple<int,int,int>>& edges); // from solution

int main() {
    // Simple triangle: two heavy edges, one light edge not in any max spanning tree.
    {
        vector<tuple<int,int,int>> edges = {{5,0,1},{5,1,2},{4,0,2}};
        assert(countNeverInMaxSpanningTree(3, edges) == 1);
    }
    // Two parallel equal-weight edges between two vertices: both can appear in some tree.
    {
        vector<tuple<int,int,int>> edges = {{3,0,1},{3,0,1}};
        assert(countNeverInMaxSpanningTree(2, edges) == 0);
    }
    // Heavier edge already connects the pair; a lighter parallel edge is never used.
    {
        vector<tuple<int,int,int>> edges = {{10,0,1},{5,0,1}};
        assert(countNeverInMaxSpanningTree(2, edges) == 1);
    }
    // All edges equal weight, forming a cycle of 4 vertices. No edge is excluded.
    {
        vector<tuple<int,int,int>> edges = {{1,0,1},{1,1,2},{1,2,3},{1,3,0}};
        assert(countNeverInMaxSpanningTree(4, edges) == 0);
    }
    // Weight groups: weight 7 connects 0-1 and 1-2; weight 5 connects 2-3 and 3-0.
    // The two weight-5 edges form a cycle with weight-7 path, but each weight-5 edge is in some max spanning tree? 
    // Actually: max spanning tree has 0-1 (7), 1-2 (7), and one of 2-3 or 3-0 (5) => the other weight-5 is never used.
    {
        vector<tuple<int,int,int>> edges = {{7,0,1},{7,1,2},{5,2,3},{5,3,0}};
        assert(countNeverInMaxSpanningTree(4, edges) == 1);
    }
    // Self-loop is never part of any spanning tree.
    {
        vector<tuple<int,int,int>> edges = {{2,0,0},{1,0,1},{1,1,2}};
        assert(countNeverInMaxSpanningTree(3, edges) == 1);
    }
    // Complete graph with all equal weight: no edge is excluded.
    {
        vector<tuple<int,int,int>> edges;
        for (int i = 0; i < 4; ++i)
            for (int j = i+1; j < 4; ++j)
                edges.emplace_back(1, i, j);
        assert(countNeverInMaxSpanningTree(4, edges) == 0);
    }
    // Graph with two weight-10 edges and three weight-9 edges forming a cycle.
    // Weight-10 edges are always in; among weight-9, exactly one is excluded.
    {
        vector<tuple<int,int,int>> edges = {
            {10,0,1},{10,1,2},
            {9,2,3},{9,3,0},{9,0,2}
        };
        // Max spanning tree: take both 10s, then one of the 9s (either 2-3 or 3-0) but not 0-2 (since 0-2 already connected via 0-1-2). So one 9 is excluded.
        assert(countNeverInMaxSpanningTree(4, edges) == 1);
    }
    // Heavier path between two vertices makes all lighter parallel edges excluded.
    {
        vector<tuple<int,int,int>> edges = {{100,0,1},{100,0,1},{50,0,1}};
        assert(countNeverInMaxSpanningTree(2, edges) == 1);
    }
    // Large isolated vertex not connected? But graph is connected by assumption; still test.
    {
        vector<tuple<int,int,int>> edges = {{1,0,1}}; // only 2 vertices, 1 edge
        assert(countNeverInMaxSpanningTree(2, edges) == 0);
    }
    return 0;
}
