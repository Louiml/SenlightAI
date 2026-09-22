// Write a C++ function `bool isUniqueMinimumSpanningSubset(const std::vector<int>& edgeWeights, int u, int v, int weight)` that, given a graph’s edge list and a query consisting of a subset of edges (represented by the three parallel arrays `edgeWeights`, `u`, `v`), returns `true` if **every edge in the subset is a bridge** in the graph formed by considering only the edges of the same weight as the query edge. In other words, for each edge in the query, when you look at the subgraph containing all edges of that same weight (and only those edges, ignoring all other weights), the edge must be a bridge (i.e., removing it increases the number of connected components within that subgraph). The function must process many queries efficiently by precomputing, for each edge, whether it is a bridge among its same-weight edges. The input graph has `n` vertices (1-indexed) and `m` edges; each edge has a distinct ID from 1 to `m`, with endpoints `u[i]`, `v[i]`, and weight `w[i]`. Queries are groups of edge IDs; the function should return `true` for a query only if all edges in that group are bridges in their respective same-weight subgraphs, and additionally no two edges in the group have the same unordered pair of endpoints when considering their same-weight “component representatives”. The function will be called with the complete edge information (precomputed in global arrays) and a list of edge IDs in the query.

#include <bits/stdc++.h>
using namespace std;

// Declaration of the solution function (as defined above)
bool isUniqueMinimumSpanningSubset(const vector<int>& u, const vector<int>& v, const vector<int>& w, const vector<int>& queryEdges);

int main() {
    // Example graph with 4 vertices and 5 edges (1-indexed)
    // Edges: (1,2,w=1), (2,3,w=1), (3,4,w=2), (1,4,w=2), (1,3,w=3)
    vector<int> u = {0, 1, 2, 3, 1, 1};
    vector<int> v = {0, 2, 3, 4, 4, 3};
    vector<int> w = {0, 1, 1, 2, 2, 3};

    // Query 1: edges 1 and 3 (weights 1 and 2). Both are bridges in their same-weight subgraphs.
    vector<int> q1 = {1, 3};
    assert(isUniqueMinimumSpanningSubset(u, v, w, q1) == true);

    // Query 2: edges 1 and 2 (both weight 1). Edge 1 is a bridge, edge 2 is a bridge too? 
    // In weight-1 subgraph, edges 1(1-2) and 2(2-3) are both bridges because removing either disconnects the weight-1 graph (components: 1-2-3 connects all three, but each edge is a bridge). 
    // But their help pairs: edge1 help = (1,2) after DSU of smaller weights (none) -> (1,2); edge2 help = (1,3) because after union of edge1, we have component 1-2 and 3 separate -> max(1,3),min(1,3) = (3,1) sorted (1,3). No duplicate. So true.
    vector<int> q2 = {1, 2};
    assert(isUniqueMinimumSpanningSubset(u, v, w, q2) == true);

    // Query 3: edges 3 and 4 (both weight 2). 
    // Weight-2 subgraph has edges (3-4) and (1-4). They form a path 1-4-3, so each is a bridge. 
    // Edge3 help: after processing weight 1, DSU roots: 1,2,3 are connected? Actually weight1 edges union 1-2-3, so component {1,2,3} root 1, and vertex4 root4. Edge3 (3-4): roots 3 and 4 are different (1 and 4) -> help (4,1). Edge4 (1-4): roots 1 and 4 -> help (4,1) same! 
    // Duplicate help pair, so query is false because they connect the same pair of components in the MST construction.
    vector<int> q3 = {3, 4};
    assert(isUniqueMinimumSpanningSubset(u, v, w, q3) == false);

    // Query 4: edge 5 (weight 3). In weight-3 subgraph, only edge (1-3) connects components {1,2,3} and {4}? Actually after weight2, all vertices 1-4 are in one component, so edge5's endpoints are already connected, so ok[5] is false.
    vector<int> q4 = {5};
    assert(isUniqueMinimumSpanningSubset(u, v, w, q4) == false);

    // Query 5: empty query (should be true vacuously)
    vector<int> q5 = {};
    assert(isUniqueMinimumSpanningSubset(u, v, w, q5) == true);

    // Query 6: edge 1 alone is a bridge.
    vector<int> q6 = {1};
    assert(isUniqueMinimumSpanningSubset(u, v, w, q6) == true);

    cout << "All tests passed!" << endl;
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 500900;
int parentDSU[MAXN];
int tmpParent[MAXN];

// Find with path compression for the main DSU
int findMain(int x) {
    return parentDSU[x] == x ? x : parentDSU[x] = findMain(parentDSU[x]);
}

// Union for main DSU
void unionMain(int a, int b) {
    a = findMain(a);
    b = findMain(b);
    if (a != b) parentDSU[a] = b;
}

// Find with path compression for the temporary DSU used in queries
int findTmp(int x) {
    return tmpParent[x] == x ? x : tmpParent[x] = findTmp(tmpParent[x]);
}

// Union for temporary DSU
void unionTmp(int a, int b) {
    a = findTmp(a);
    b = findTmp(b);
    if (a != b) tmpParent[a] = b;
}

// The solution function: given the edge endpoints, weights, and a query list of edge IDs,
// returns true if all edges in the query are bridges in their same-weight subgraphs,
// and no two edges connect the same pair of components.
bool isUniqueMinimumSpanningSubset(
    const vector<int>& u,
    const vector<int>& v,
    const vector<int>& w,
    const vector<int>& queryEdges
) {
    static vector<int> edgesByWeight[MAXN];
    static bool ok[MAXN];
    static pair<int,int> help[MAXN];
    static bool computed = false;
    static int m = u.size() - 1; // edges are 1-indexed

    if (!computed) {
        // Initialize main DSU
        for (int i = 0; i < MAXN; ++i) parentDSU[i] = i;
        // Group edges by weight
        for (int i = 1; i <= m; ++i) {
            edgesByWeight[w[i]].push_back(i);
        }
        // Process weights in increasing order
        for (int wt = 1; wt < MAXN; ++wt) {
            // First pass: check if each edge is a bridge in same-weight subgraph
            for (int eid : edgesByWeight[wt]) {
                int a = findMain(u[eid]);
                int b = findMain(v[eid]);
                if (a != b) ok[eid] = true;
                else ok[eid] = false;
                help[eid] = make_pair(max(a,b), min(a,b));
            }
            // Second pass: union all edges of this weight
            for (int eid : edgesByWeight[wt]) {
                unionMain(u[eid], v[eid]);
            }
        }
        computed = true;
    }

    // Check query: all edges must be bridges, and no duplicate help pairs
    set<pair<int,int>> seenPairs;
    // Initialize temporary DSU for all vertices involved in query edges
    for (int eid : queryEdges) {
        tmpParent[u[eid]] = u[eid];
        tmpParent[v[eid]] = v[eid];
    }
    for (int eid : queryEdges) {
        if (!ok[eid]) return false;
        if (!seenPairs.insert(help[eid]).second) return false;
        int a = findTmp(u[eid]);
        int b = findTmp(v[eid]);
        if (a == b) return false;
        unionTmp(u[eid], v[eid]);
    }
    return true;
}

// The core idea is to process edges grouped by weight. For each weight, we first attempt to add all edges of that weight to a union-find structure that already contains all edges of smaller weights (i.e., the DSU after processing all weights less than the current one). Before actually merging, we check for each edge whether its two endpoints’ current DSU roots differ. If they differ, the edge is a bridge in the same-weight subgraph because it connects two distinct components; if the roots are the same, the edge is not a bridge because it lies inside a cycle formed by other edges of the same weight. After checking all edges of a weight, we merge them all into the DSU for the next weight. This is a classic technique for building a minimum spanning forest and identifying bridges within each weight class. We store for each edge: (1) `ok[edge]` = whether it is a bridge; (2) `help[edge]` = an unordered pair of the DSU roots (sorted) at the time of the check. For a query group of edges, we must verify that all edges have `ok=1`. Additionally, we must ensure that no two edges in the query share the same `help` pair (meaning they would connect the same two components in the MST construction and thus be redundant). For each query, we temporarily initialize a DSU for the vertices involved in the query edges, then iterate through the query edges and try to union their endpoints; if any union attempt finds the endpoints already connected, or if the set of `help` pairs has duplicate, the query is invalid. Time complexity: Processing all edges once per weight group takes O(m α(n)) for DSU operations. Each query takes O(k α(n)) where k is the number of edges in the query, plus O(k log k) if using a set to detect duplicate `help` pairs. Space complexity is O(n + m).
