Given a weighted undirected graph with `n` vertices (numbered `0` to `n-1`) and a list of edges where each edge is `[u, v, weight]`, write a C++ function `vector<bool> connectedWithinLimit(int n, vector<vector<int>>& edges, vector<vector<int>>& queries)` that, for each query `[start, end, limit]`, returns `true` if there exists a path between `start` and `end` using only edges with weight strictly less than `limit`, and `false` otherwise. The function must process the queries offline using a minimum-spanning-tree-like approach (Kruskal's algorithm with union-find) to avoid redundant work. Assume the graph may be disconnected, edges may be duplicated, and queries may contain the same pair multiple times with different limits. The input sizes satisfy `1 <= n <= 1000`, `0 <= edges.size() <= 10000`, and `0 <= queries.size() <= 1000`. Edge weights and limits are positive integers. The function should return a vector of booleans in the same order as the queries.
// The core issue is that for each query we need to know connectivity in a subgraph containing only edges with weight less than the query limit. Processing each query independently would be inefficient (O(Q * E) at best with BFS). Instead, we observe that if we sort all edges by weight ascending, then as the query limit increases, we add more and more edges to a growing "available" set. This suggests an offline processing strategy: sort the queries by limit ascending, maintain a union-find structure that represents which vertices are connected using only edges added so far (all with weight less than the current query's limit), and for each query in order, add all edges whose weight is strictly less than the query limit before answering the query. Since edges are added monotonically as limits increase, each edge is processed at most once across all queries, giving O(E log E + Q log Q + (E+Q) α(n)) time where α is the inverse Ackermann function (near constant). The space is O(n + E + Q) for the parent array, edge list, and query answer vector. We must be careful with strict inequality: edges with weight exactly equal to the limit are not included. Edge cases: if start and end are already connected by edges added earlier, answer is true; if they are the same vertex, they are trivially connected even with no edges (union-find root same). Duplicate edges don't affect correctness because union-find skips redundant connections. Disconnected components are handled naturally.
#include <vector>
#include <algorithm>

using namespace std;

class UnionFind {
private:
    vector<int> parent;
    vector<int> rank;
public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]); // path compression
        return parent[x];
    }

    void unite(int x, int y) {
        int rx = find(x);
        int ry = find(y);
        if (rx == ry) return;
        if (rank[rx] < rank[ry]) swap(rx, ry);
        parent[ry] = rx;
        if (rank[rx] == rank[ry]) rank[rx]++;
    }

    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};

// For each query {start, end, limit}, returns true if there is a path
// using only edges with weight strictly less than limit.
vector<bool> connectedWithinLimit(int n, vector<vector<int>>& edges, vector<vector<int>>& queries) {
    // Sort edges by weight ascending.
    sort(edges.begin(), edges.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[2] < b[2];
    });

    // Add original index to each query and sort by limit ascending.
    vector<vector<int>> indexedQueries;
    indexedQueries.reserve(queries.size());
    for (int i = 0; i < (int)queries.size(); ++i) {
        indexedQueries.push_back({queries[i][0], queries[i][1], queries[i][2], i});
    }
    sort(indexedQueries.begin(), indexedQueries.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[2] < b[2]; });

    UnionFind uf(n);
    vector<bool> answer(queries.size(), false);
    int edgeIdx = 0;

    for (const auto& q : indexedQueries) {
        int start = q[0];
        int end = q[1];
        int limit = q[2];
        int originalIdx = q[3];

        // Add all edges with weight strictly less than limit.
        while (edgeIdx < (int)edges.size() && edges[edgeIdx][2] < limit) {
            uf.unite(edges[edgeIdx][0], edges[edgeIdx][1]);
            ++edgeIdx;
        }

        answer[originalIdx] = uf.connected(start, end);
    }

    return answer;
}
#include <cassert>
#include <vector>
using namespace std;

int main() {
    // Basic cases
    {
        int n = 4;
        vector<vector<int>> edges = {{0,1,2}, {1,2,3}, {2,3,4}};
        vector<vector<int>> queries = {{0,3,5}, {0,3,4}, {0,2,3}, {0,2,4}};
        vector<bool> expected = {true, false, false, true};
        assert(connectedWithinLimit(n, edges, queries) == expected);
    }

    // All edges heavier than limit, and self-loop query
    {
        int n = 3;
        vector<vector<int>> edges = {{0,1,10}, {1,2,10}};
        vector<vector<int>> queries = {{0,2,5}, {0,0,1}};
        vector<bool> expected = {false, true};
        assert(connectedWithinLimit(n, edges, queries) == expected);
    }

    // Disconnected graph, duplicate edges
    {
        int n = 5;
        vector<vector<int>> edges = {{0,1,1}, {1,0,1}, {2,3,2}, {2,3,2}};
        vector<vector<int>> queries = {{0,1,2}, {2,3,3}, {0,2,5}, {4,4,1}};
        vector<bool> expected = {true, true, false, true};
        assert(connectedWithinLimit(n, edges, queries) == expected);
    }

    // Strict inequality: edge weight equals limit should not be included
    {
        int n = 2;
        vector<vector<int>> edges = {{0,1,5}};
        vector<vector<int>> queries = {{0,1,5}, {0,1,6}};
        vector<bool> expected = {false, true};
        assert(connectedWithinLimit(n, edges, queries) == expected);
    }

    // Many queries with increasing limits
    {
        int n = 3;
        vector<vector<int>> edges = {{0,1,3}, {1,2,5}};
        vector<vector<int>> queries = {{0,2,4}, {0,2,5}, {0,2,6}, {0,2,100}};
        vector<bool> expected = {false, true, true, true};
        assert(connectedWithinLimit(n, edges, queries) == expected);
    }

    // No edges at all, n=1
    {
        int n = 1;
        vector<vector<int>> edges;
        vector<vector<int>> queries = {{0,0,1}};
        vector<bool> expected = {true};
        assert(connectedWithinLimit(n, edges, queries) == expected);
    }

    // Equivalent limit values across queries, edges sorted correctly
    {
        int n = 4;
        vector<vector<int>> edges = {{3,2,1}, {1,3,2}, {0,2,3}};
        vector<vector<int>> queries = {{0,1,3}, {2,3,3}, {0,3,4}};
        vector<bool> expected = {true, true, true};
        assert(connectedWithinLimit(n, edges, queries) == expected);
    }

    return 0;
}
