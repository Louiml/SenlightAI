Given an undirected connected graph with `n` vertices labeled from 1 to `n` and `m` weighted edges, write a C++ function `long long minimumPossibleSpanningTreeWeight(long long n, long long m, long long k, const vector<tuple<long long, long long, long long>>& edges)` that computes the minimum number of edges whose weights must be reduced to 1 (from their original weight, which is always ≥ 1) so that the total weight of the graph’s minimum spanning tree (MST) becomes at most `k`. If it is impossible to achieve this even after reducing all edges to 1, return `-1`. The function must find the MST of the original graph, then greedily reduce the heaviest edges of that MST (each reduction setting the edge weight to 1, saving `originalWeight - 1`) until the MST total weight ≤ `k`. The graph is guaranteed to be connected, but may contain multiple edges between the same vertices. All weights are positive integers, and `m ≥ n-1`.
// The key insight is that reducing an edge that is not in the original MST cannot help lower the MST total weight below what we achieve by reducing edges already in the MST, because any spanning tree must contain at least `n-1` edges and the MST is the minimal possible total. However, after reducing an edge in the MST, a different edge might become part of a new MST, but reducing edges outside the original MST never helps because the MST is the absolute minimum total for any spanning tree; reducing a non-MST edge still leaves the original MST edges unchanged, and the MST remains the original one (since its total is lower or equal). Therefore, we only need to consider the edges in the original MST. Algorithm: Build the MST using Kruskal’s algorithm (sort edges by weight, use union-find). If the MST total cost is already ≤ `k`, return 0 (no reductions needed). Otherwise, store the MST edge weights in a max-heap (priority queue). Repeatedly pop the largest weight `w`, reduce it to 1, thus subtracting `(w-1)` from the total cost, and increment a counter. Stop when total cost ≤ `k`. If we run out of edges (i.e., after reducing all MST edges to 1) and still total > `k`, return -1. Edge cases: `n=1` (MST has 0 edges, total cost 0, always ≤ `k` for any non-negative `k`; return 0). Duplicate edges are handled by union-find. Weights are `long long` to avoid overflow. Time complexity: O(m log m) for sorting edges, O(m α(n)) for union-find operations, and O(n log n) for heap operations. Space complexity: O(m + n) for edges, union-find arrays, and heap.
#include <bits/stdc++.h>
using namespace std;

// Computes the minimum number of edge-weight reductions (to 1) needed so that
// the MST total weight ≤ k. Returns -1 if impossible even after reducing all edges.
long long minimumPossibleSpanningTreeWeight(
    long long n, long long m, long long k,
    const vector<tuple<long long, long long, long long>>& edges)
{
    // Union-Find structure
    vector<long long> parent(n + 1), rank(n + 1, 0);
    iota(parent.begin(), parent.end(), 0);

    function<long long(long long)> find = [&](long long x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    };

    auto unite = [&](long long a, long long b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (rank[a] < rank[b]) swap(a, b);
        parent[b] = a;
        if (rank[a] == rank[b]) rank[a]++;
        return true;
    };

    // Sort edges by weight for Kruskal's algorithm
    vector<tuple<long long, long long, long long>> sortedEdges = edges;
    sort(sortedEdges.begin(), sortedEdges.end(),
         [](const auto& a, const auto& b) { return get<2>(a) < get<2>(b); });

    long long mstCost = 0;
    long long edgesUsed = 0;
    priority_queue<long long> maxHeap; // stores MST edge weights

    for (const auto& [u, v, w] : sortedEdges) {
        if (unite(u, v)) {
            mstCost += w;
            maxHeap.push(w);
            edgesUsed++;
            if (edgesUsed == n - 1) break;
        }
    }

    // If graph is connected (guaranteed), edgesUsed == n-1.
    // If n==1, MST cost is 0 and no edges needed.
    if (n == 1) return 0;
    if (edgesUsed < n - 1) return -1; // should not happen per problem, but safe

    long long reductions = 0;
    long long currentCost = mstCost;
    if (currentCost <= k) return 0;

    while (!maxHeap.empty()) {
        long long w = maxHeap.top();
        maxHeap.pop();
        currentCost = currentCost - w + 1; // reduce this edge to 1
        reductions++;
        if (currentCost <= k) return reductions;
    }

    return -1; // even after reducing all edges, cost > k
}
#include <cassert>
#include <vector>
#include <tuple>
using namespace std;

// Declare the function (already defined above)
long long minimumPossibleSpanningTreeWeight(
    long long n, long long m, long long k,
    const vector<tuple<long long, long long, long long>>& edges);

int main() {
    // Test 1: Simple triangle, k large enough, no reduction needed
    {
        vector<tuple<long long, long long, long long>> edges = {
            {1,2,5}, {2,3,6}, {1,3,7}
        };
        assert(minimumPossibleSpanningTreeWeight(3, 3, 12, edges) == 0);
    }

    // Test 2: Need one reduction
    {
        vector<tuple<long long, long long, long long>> edges = {
            {1,2,10}, {2,3,10}, {1,3,1}
        };
        // MST: edges (1,3)=1, (1,2)=10 total 11; k=3 → reduce 10 to 1 → total 2 ≤3, one reduction
        assert(minimumPossibleSpanningTreeWeight(3, 3, 3, edges) == 1);
    }

    // Test 3: Impossible even after reducing all
    {
        vector<tuple<long long, long long, long long>> edges = {
            {1,2,100}, {2,3,100}, {1,3,100}
        };
        // MST cost 200, reduce both edges → 2, but k=1 → impossible
        assert(minimumPossibleSpanningTreeWeight(3, 3, 1, edges) == -1);
    }

    // Test 4: Single vertex
    {
        vector<tuple<long long, long long, long long>> edges = {};
        assert(minimumPossibleSpanningTreeWeight(1, 0, 0, edges) == 0);
    }

    // Test 5: Already satisfies after reducing two edges, but one might be enough
    {
        vector<tuple<long long, long long, long long>> edges = {
            {1,2,5}, {2,3,5}, {1,3,1}, {3,4,5}
        };
        // MST: (1,3)=1, (1,2)=5, (3,4)=5 total 11; k=4 → reduce 5 to 1 → total 7, not enough; reduce second 5 → total 3 ≤4 → 2 reductions
        assert(minimumPossibleSpanningTreeWeight(4, 4, 4, edges) == 2);
    }

    // Test 6: Large k, zero reductions
    {
        vector<tuple<long long, long long, long long>> edges = {
            {1,2,3}, {2,3,4}, {3,4,5}, {1,4,100}
        };
        assert(minimumPossibleSpanningTreeWeight(4, 4, 15, edges) == 0);
    }

    // Test 7: Multiple duplicate edges, need reduction
    {
        vector<tuple<long long, long long, long long>> edges = {
            {1,2,8}, {1,2,8}, {2,3,8}, {1,3,1}
        };
        // MST picks one of (1,2)=8 and (1,3)=1 total 9; k=2 → reduce 8 to 1 → total 2 → 1 reduction
        assert(minimumPossibleSpanningTreeWeight(3, 4, 2, edges) == 1);
    }

    // Test 8: Disconnected? Not per problem, but test connectivity check: n=3, only one edge
    {
        vector<tuple<long long, long long, long long>> edges = {{1,2,5}};
        assert(minimumPossibleSpanningTreeWeight(3, 1, 0, edges) == -1);
    }

    return 0;
}
