Given a number of cities `n` (numbered 1 to n) and a list of `n-1` roads, where each road connects two cities and has a weight, write a C++ function `vector<int> countReachablePairs(int n, vector<vector<int>> edges, int q, vector<int>& queries)` that answers `q` queries. For each query value `x`, consider only the roads with weight **strictly less than or equal to `x`**. In the graph formed by these selected roads, count the total number of unordered pairs of cities `(u, v)` such that `u != v` and there is a path between them (i.e., they are in the same connected component). The function must return a vector of answers in the same order as the queries. The input edges are given as `[u, v, weight]` (1-indexed cities), and all weights and queries are positive integers. If no road satisfies a query, the answer is 0.

#include <cassert>
#include <vector>

// The solution function is declared above.
int main() {
    // Test 1: Simple tree of 3 nodes, edges (1-2, w=1), (2-3, w=2)
    {
        int n = 3;
        std::vector<std::vector<int>> edges = {{1,2,1},{2,3,2}};
        int q = 4;
        std::vector<int> queries = {0,1,2,3};
        std::vector<int> result = countReachablePairs(n, edges, q, queries);
        std::vector<int> expected = {0,1,3,3}; // for x>=2 all pairs connected
        assert(result == expected);
    }
    // Test 2: Disconnected graph
    {
        int n = 4;
        std::vector<std::vector<int>> edges = {{1,2,5},{3,4,5}};
        int q = 1;
        std::vector<int> queries = {5};
        std::vector<int> result = countReachablePairs(n, edges, q, queries);
        // Two components of size 2 each -> 1 pair each = 2 total
        assert(result == std::vector<int>{2});
    }
    // Test 3: Single node (n=1, no edges)
    {
        int n = 1;
        std::vector<std::vector<int>> edges; // empty (n-1 = 0)
        int q = 2;
        std::vector<int> queries = {1, 100};
        std::vector<int> result = countReachablePairs(n, edges, q, queries);
        assert(result == std::vector<int>(2, 0));
    }
    // Test 4: Query order is preserved and intermediate queries work
    {
        int n = 5;
        std::vector<std::vector<int>> edges = {{1,2,1},{2,3,1},{3,4,2},{4,5,2}};
        int q = 3;
        std::vector<int> queries = {2,1,3};
        std::vector<int> result = countReachablePairs(n, edges, q, queries);
        // For x=1: edges with weight<=1 connect 1-2-3, that's 3 nodes -> 3 pairs
        // For x=2: all edges connect all 5 nodes -> 10 pairs
        // For x=3: still all 5 -> 10 pairs
        std::vector<int> expected = {10,3,10};
        assert(result == expected);
    }
    // Test 5: Duplicate weights and fully connected graph
    {
        int n = 4;
        std::vector<std::vector<int>> edges = {{1,2,3},{2,3,3},{3,4,3}};
        int q = 2;
        std::vector<int> queries = {2,3};
        std::vector<int> result = countReachablePairs(n, edges, q, queries);
        assert(result == std::vector<int>{0,6}); // for x=2 no edges, x=3 all pairs
    }
    // Test 6: Larger components merge
    {
        int n = 6;
        std::vector<std::vector<int>> edges = {{1,2,1},{2,3,1},{4,5,1},{5,6,1},{3,4,2}};
        int q = 2;
        std::vector<int> queries = {1,2};
        std::vector<int> result = countReachablePairs(n, edges, q, queries);
        // x=1: two components of size 3 each -> 2*C(3,2)=6
        // x=2: all 6 connected -> C(6,2)=15
        assert(result == std::vector<int>{6,15});
    }
    // Test 7: Edge case with negative/zero weights? Not needed but check zero limit.
    {
        int n = 2;
        std::vector<std::vector<int>> edges = {{1,2,5}};
        int q = 3;
        std::vector<int> queries = {0,5,10};
        std::vector<int> result = countReachablePairs(n, edges, q, queries);
        std::vector<int> expected = {0,1,1};
        assert(result == expected);
    }
    // Test 8: Sorted queries but unsorted input edges
    {
        int n = 3;
        std::vector<std::vector<int>> edges = {{2,3,1},{1,2,2}}; // swapped order
        int q = 1;
        std::vector<int> queries = {2};
        std::vector<int> result = countReachablePairs(n, edges, q, queries);
        assert(result == std::vector<int>{3});
    }
    return 0;
}

#include <vector>
#include <algorithm>
#include <utility>

// Disjoint-set union with path compression and union by size.
static int findRoot(std::vector<std::pair<int,int>>& parent, int x) {
    if (parent[x].first != x) {
        parent[x].first = findRoot(parent, parent[x].first);
    }
    return parent[x].first;
}

// Compute the number of unordered pairs (u,v) with u<v that are connected
// after adding all edges with weight <= queries[i].
std::vector<int> countReachablePairs(int n, std::vector<std::vector<int>> edges,
                                     int q, std::vector<int> queries) {
    // Pair each query with its original index.
    std::vector<std::pair<int,int>> queryWithIndex(q);
    for (int i = 0; i < q; ++i) {
        queryWithIndex[i] = {queries[i], i};
    }
    std::sort(queryWithIndex.begin(), queryWithIndex.end());

    // Sort edges by weight (assume edges are [u, v, weight]).
    // To sort by weight, we reorder the columns so weight is first.
    for (auto& e : edges) {
        std::swap(e[0], e[2]); // now e[0] = weight, e[1]=u, e[2]=v
    }
    std::sort(edges.begin(), edges.end());

    // Initialize DSU: each node is its own parent, size = 1.
    std::vector<std::pair<int,int>> parent(n);
    for (int i = 0; i < n; ++i) {
        parent[i] = {i, 1};
    }

    std::vector<int> answer(q);
    int edgeIdx = 0;
    int totalPairs = 0; // number of connected pairs so far

    for (int qi = 0; qi < q; ++qi) {
        int currentLimit = queryWithIndex[qi].first;
        // Add all edges with weight <= currentLimit.
        while (edgeIdx < n - 1 && edges[edgeIdx][0] <= currentLimit) {
            int u = edges[edgeIdx][1] - 1; // convert to 0-indexed
            int v = edges[edgeIdx][2] - 1;
            int ru = findRoot(parent, u);
            int rv = findRoot(parent, v);
            if (ru != rv) {
                int sizeU = parent[ru].second;
                int sizeV = parent[rv].second;
                // Remove old contributions.
                totalPairs -= (sizeU * (sizeU - 1)) / 2;
                totalPairs -= (sizeV * (sizeV - 1)) / 2;
                // Union by attaching rv's root to ru.
                parent[rv].first = ru;
                parent[ru].second += sizeV;
                // Add new contribution.
                int newSize = parent[ru].second;
                totalPairs += (newSize * (newSize - 1)) / 2;
            }
            ++edgeIdx;
        }
        answer[queryWithIndex[qi].second] = totalPairs;
    }
    return answer;
}

// The key is to process queries in increasing order and incrementally add roads, maintaining a disjoint-set union (DSU) structure. Start by sorting the edges by weight and the queries by their value, keeping track of each query's original index. For each query (from smallest to largest), add all edges whose weight ≤ query value. While adding an edge, union the two endpoints. Maintain a running total `last` that stores the number of valid pairs in the current graph. When merging two components of sizes `cu` and `cv`, the number of pairs contributed by these components before the merge is `C(cu,2) + C(cv,2)`, and after the merge it is `C(cu+cv,2)`. Update `last` by subtracting the old contributions and adding the new one. The DSU parent array stores both the parent and the component size (using the root's size). Path compression is applied for efficiency. Edge cases: when a query value is smaller than all edge weights, `last` remains 0; when the graph becomes fully connected, the answer is `C(n,2)` for all larger queries. The time complexity is `O((n + q) α(n))` after sorting (sorting edges `O(n log n)`, sorting queries `O(q log q)`), with `α` being the inverse Ackermann function (essentially constant). Space complexity is `O(n + q)`.
