// Write a C++ function `connectedComponents` that, given an undirected graph with vertices numbered from 1 to `n` and a list of edges, preprocesses the graph and then answers a series of queries. Each query asks whether two given vertices `a` and `b` are in the same connected component. The function should take as parameters: `n` (number of vertices), `edgeList` (a vector of pairs of integers representing edges), and `queries` (a vector of pairs of integers). It should return a vector of strings, where each string is `"Yes"` if the two vertices of the corresponding query are connected, and `"No"` otherwise. The graph is undirected, may have multiple edges or self-loops, and the queries may repeat pairs or include vertices with no edges. Your function must be efficient for large graphs and many queries.
// The problem reduces to finding connected components of an undirected graph, which can be done efficiently with a disjoint-set union (DSU) data structure. Initialize a DSU with `parent[i] = i` and `size[i] = 1` for all vertices from 1 to `n`. For each edge `(u, v)` in the edge list, call the union operation to merge the sets containing `u` and `v`. After processing all edges, for each query `(a, b)`, find the root (representative) of `a` and the root of `b` using path compression. If the roots are equal, return `"Yes"`; otherwise, return `"No"`.  
// Edge cases include vertices with no edges (each is its own component), self-loops (union does nothing), multiple edges between the same vertices (union is idempotent), and queries that ask about the same vertex twice (always "Yes"). Union by size ensures the DSU tree remains shallow, and path compression keeps subsequent finds near constant time.  
// Time complexity: O((m + q) · α(n)) where α is the inverse Ackermann function (nearly constant), so effectively O(m + q). Space complexity: O(n) for the parent and size arrays.
#include <vector>
#include <string>

// Disjoint-set union with path compression and union by size.
std::vector<std::string> connectedComponents(
    int n,
    const std::vector<std::pair<int, int>>& edgeList,
    const std::vector<std::pair<int, int>>& queries
) {
    // Parent and size arrays, 1-indexed.
    std::vector<int> parent(n + 1);
    std::vector<int> size(n + 1, 1);
    for (int i = 0; i <= n; ++i) {
        parent[i] = i;
    }

    // Find with path compression.
    auto find = [&](auto&& self, int x) -> int {
        if (parent[x] != x) {
            parent[x] = self(self, parent[x]);
        }
        return parent[x];
    };

    // Union by size.
    auto unite = [&](int a, int b) {
        int ra = find(find, a);
        int rb = find(find, b);
        if (ra == rb) return;
        if (size[ra] < size[rb]) {
            parent[ra] = rb;
            size[rb] += size[ra];
        } else {
            parent[rb] = ra;
            size[ra] += size[rb];
        }
    };

    // Build components.
    for (const auto& edge : edgeList) {
        unite(edge.first, edge.second);
    }

    // Answer queries.
    std::vector<std::string> result;
    result.reserve(queries.size());
    for (const auto& query : queries) {
        int ra = find(find, query.first);
        int rb = find(find, query.second);
        result.push_back(ra == rb ? "Yes" : "No");
    }

    return result;
}
#include <cassert>
#include <vector>
#include <string>
#include <utility>

// Include the solution function here or link it.

int main() {
    // Test 1: Simple connected graph
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3}};
        std::vector<std::pair<int,int>> queries = {{1,3},{1,4},{4,4}};
        auto res = connectedComponents(4, edges, queries);
        assert(res == std::vector<std::string>({"Yes","No","Yes"}));
    }

    // Test 2: No edges, all isolated
    {
        std::vector<std::pair<int,int>> edges;
        std::vector<std::pair<int,int>> queries = {{1,2},{2,2}};
        auto res = connectedComponents(5, edges, queries);
        assert(res == std::vector<std::string>({"No","Yes"}));
    }

    // Test 3: Self-loops and multiple edges
    {
        std::vector<std::pair<int,int>> edges = {{1,1},{2,3},{3,2}};
        std::vector<std::pair<int,int>> queries = {{1,1},{2,3},{1,2}};
        auto res = connectedComponents(3, edges, queries);
        assert(res == std::vector<std::string>({"Yes","Yes","No"}));
    }

    // Test 4: Disjoint components
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{3,4}};
        std::vector<std::pair<int,int>> queries = {{2,1},{4,3},{2,3}};
        auto res = connectedComponents(4, edges, queries);
        assert(res == std::vector<std::string>({"Yes","Yes","No"}));
    }

    // Test 5: Large single component
    {
        std::vector<std::pair<int,int>> edges;
        for (int i = 1; i < 1000; ++i) edges.push_back({i, i+1});
        std::vector<std::pair<int,int>> queries = {{1,1000},{500,750}};
        auto res = connectedComponents(1000, edges, queries);
        assert(res == std::vector<std::string>({"Yes","Yes"}));
    }

    return 0;
}
