// Given a connected undirected weighted graph with `n` vertices, `m` edges, and `s` special "source" vertices, write a C++ function `vector<bool> answerQueries(const vector<vector<pair<int,int>>>& adj, const vector<int>& sources, const vector<EdgeInput>& edges, const vector<QueryInput>& queries)` that for each query `(a, b, limit)` returns `true` if after constructing a graph using only edges whose "modified cost" is ≤ `limit`, vertices `a` and `b` are connected. The modified cost of an edge `(u,v)` is computed as the original edge weight plus the shortest distance from any source to `u` plus the shortest distance from any source to `v`. The graph is undirected, all weights are non‑negative, and source distances are computed using Dijkstra with multiple starting points (all sources have distance 0). The function must return answers in the original query order.

#include <cassert>
#include <vector>
#include <string>

// Forward declaration of the solution function
std::vector<bool> answerQueries(
    const std::vector<std::vector<std::pair<int,int>>>& adj,
    const std::vector<int>& sources,
    const std::vector<EdgeInput>& edges,
    const std::vector<QueryInput>& queries);

int main() {
    // Test 1: Simple graph with one source
    // Vertices: 1 (source), 2, 3. Edges: 1-2 weight 1, 2-3 weight 2
    // Query: can 1 and 3 be connected with limit 3? Modified costs: edge 1-2: 1+0+1=2, edge 2-3: 2+1+2=5
    std::vector<std::vector<std::pair<int,int>>> adj1(4);
    adj1[1].push_back({2,1});
    adj1[2].push_back({1,1});
    adj1[2].push_back({3,2});
    adj1[3].push_back({2,2});
    std::vector<int> sources1 = {1};
    std::vector<EdgeInput> edges1 = {{1,2,1}, {2,3,2}};
    std::vector<QueryInput> queries1 = {{1,3,3,0}, {1,3,5,1}};
    auto res1 = answerQueries(adj1, sources1, edges1, queries1);
    assert(res1.size() == 2);
    assert(res1[0] == false); // limit 3: no edge qualifies? edge 1-2 has key 2 qualifies, but 2-3 key 5 not, so 1 and 3 not connected
    assert(res1[1] == true);  // limit 5: both edges qualify, connected

    // Test 2: Multiple sources
    // Vertices 1 and 3 are sources, edge 1-2 weight 10, 2-3 weight 10, 1-3 weight 100
    // distances: dist[1]=0, dist[3]=0, dist[2]=10 (from either source)
    // Modified keys: edge 1-2: 10+0+10=20, 2-3: 10+10+0=20, 1-3: 100+0+0=100
    std::vector<std::vector<std::pair<int,int>>> adj2(4);
    adj2[1].push_back({2,10});
    adj2[2].push_back({1,10});
    adj2[2].push_back({3,10});
    adj2[3].push_back({2,10});
    adj2[1].push_back({3,100});
    adj2[3].push_back({1,100});
    std::vector<int> sources2 = {1,3};
    std::vector<EdgeInput> edges2 = {{1,2,10}, {2,3,10}, {1,3,100}};
    std::vector<QueryInput> queries2 = {{1,3,50,0}, {1,2,25,1}};
    auto res2 = answerQueries(adj2, sources2, edges2, queries2);
    assert(res2.size() == 2);
    assert(res2[0] == true);  // limit 50: edges with key ≤50: both 1-2 and 2-3 (20 each) connect 1 and 3
    assert(res2[1] == true);  // limit 25: edge 1-2 qualifies (key 20) connects 1 and 2

    // Test 3: Single vertex, no edges
    std::vector<std::vector<std::pair<int,int>>> adj3(2);
    std::vector<int> sources3 = {1};
    std::vector<EdgeInput> edges3;
    std::vector<QueryInput> queries3 = {{1,1,0,0}};
    auto res3 = answerQueries(adj3, sources3, edges3, queries3);
    assert(res3.size() == 1);
    assert(res3[0] == true); // same vertex always connected

    // Test 4: Edge case where no source can reach a vertex (disconnected graph)
    // Graph: 1 source, 2 isolated, edge between 2 and 3 weight 1
    // dist for 2 and 3 are INF, but key for edge 2-3 becomes INF+INF+1 = large, so with any finite limit, not connected
    std::vector<std::vector<std::pair<int,int>>> adj4(4);
    adj4[2].push_back({3,1});
    adj4[3].push_back({2,1});
    std::vector<int> sources4 = {1};
    std::vector<EdgeInput> edges4 = {{2,3,1}};
    std::vector<QueryInput> queries4 = {{2,3,5,0}};
    auto res4 = answerQueries(adj4, sources4, edges4, queries4);
    assert(res4.size() == 1);
    assert(res4[0] == false); // key = 1 + INF + INF > 5, edge not added

    return 0;
}

#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

struct EdgeInput {
    int u, v, w;
};

struct QueryInput {
    int a, b, limit;
    int id;
};

// Disjoint Set Union with path compression and union by rank
class DSU {
public:
    explicit DSU(int n) : parent(n+1), rank(n+1, 0) {
        for (int i = 1; i <= n; ++i) parent[i] = i;
    }
    
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    
    void unite(int x, int y) {
        int rx = find(x);
        int ry = find(y);
        if (rx == ry) return;
        if (rank[rx] < rank[ry]) parent[rx] = ry;
        else if (rank[rx] > rank[ry]) parent[ry] = rx;
        else {
            parent[ry] = rx;
            rank[rx]++;
        }
    }
    
private:
    std::vector<int> parent;
    std::vector<int> rank;
};

// Main solution function
std::vector<bool> answerQueries(
    const std::vector<std::vector<std::pair<int,int>>>& adj,
    const std::vector<int>& sources,
    const std::vector<EdgeInput>& edges,
    const std::vector<QueryInput>& queries)
{
    int n = static_cast<int>(adj.size()) - 1; // vertices are 1..n
    const long long INF = std::numeric_limits<long long>::max() / 4;
    
    // Multi-source Dijkstra: distances from nearest source
    std::vector<long long> dist(n+1, INF);
    using P = std::pair<long long, int>; // (distance, vertex)
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    
    for (int src : sources) {
        dist[src] = 0;
        pq.push({0, src});
    }
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue;
        for (const auto& [v, w] : adj[u]) {
            if (dist[v] > d + w) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }
    
    // Build modified edge costs
    std::vector<std::pair<long long, std::pair<int,int>>> modifiedEdges; // (key, {u,v})
    modifiedEdges.reserve(edges.size());
    for (const auto& e : edges) {
        long long key = e.w + dist[e.u] + dist[e.v];
        modifiedEdges.push_back({key, {e.u, e.v}});
    }
    std::sort(modifiedEdges.begin(), modifiedEdges.end());
    
    // Sort queries by limit
    std::vector<QueryInput> sortedQueries = queries;
    std::sort(sortedQueries.begin(), sortedQueries.end(),
              [](const QueryInput& a, const QueryInput& b) { return a.limit < b.limit; });
    
    DSU dsu(n);
    std::vector<bool> answers(queries.size(), false);
    int edgeIdx = 0;
    int m = static_cast<int>(modifiedEdges.size());
    
    for (const auto& q : sortedQueries) {
        while (edgeIdx < m && modifiedEdges[edgeIdx].first <= q.limit) {
            dsu.unite(modifiedEdges[edgeIdx].second.first, modifiedEdges[edgeIdx].second.second);
            edgeIdx++;
        }
        answers[q.id] = (dsu.find(q.a) == dsu.find(q.b));
    }
    
    return answers;
}

// The problem reduces to: compute, for each vertex, its distance to the nearest source using multi‑source Dijkstra (initializing the priority queue with all sources at distance 0). Then for each edge `(u,v,w)`, define `key = w + dist[u] + dist[v]`. For each query with limit `L`, we need to know if `a` and `b` are connected when we only include edges with `key ≤ L`. This is a classic offline technique: sort edges by `key` ascending, sort queries by `limit` ascending, and use a Disjoint Set Union (DSU) to progressively add edges whose `key` is ≤ current query's limit. For each query in sorted order, after adding all qualifying edges, check if `find(a) == find(b)`. Because queries are processed offline, we can store answers by the original query index. Edge cases: there may be multiple sources; all distances from sources are computed correctly via Dijkstra (no negative weights); a vertex that is itself a source has distance 0; if no path from any source to a vertex exists (graph may be disconnected), then `dist` for that vertex remains "infinite" – but since the problem statement says graph is connected, we can assume all vertices are reachable, but we can still handle it by using a large sentinel. Time complexity: Dijkstra is O((n+m) log n), sorting edges O(m log m), sorting queries O(q log q), DSU operations nearly O((m+q) α(n)). Overall O((n+m) log n + (m+q) log max(m,q)). Space complexity: O(n+m+q).
