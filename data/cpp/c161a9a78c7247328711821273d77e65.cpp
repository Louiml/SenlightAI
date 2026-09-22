// Write a C++ function `long long largestComponentAfterMerges(int n, const std::vector<std::pair<int,int>>& edges, const std::vector<std::pair<int,int>>& queries)` where `n` nodes are initially disconnected and labeled `1` through `n`. The function must process each query `(a, b)`: first merge the components containing `a` and `b` (if they are already in the same component, do nothing), then immediately return the size of the largest connected component **after** applying all merges from all queries in order. The final answer is the maximum component size observed **after every individual query** (i.e., after each merge operation, record the current maximum component size, and return the largest of all such recorded values). The graph is undirected, edges may be duplicated or self-loops (which are ignored), and queries may reference nodes that are already connected. Return 0 if there are no queries.
// The core data structure is a Disjoint Set Union (DSU) with path compression and union by size (or rank). Initially, each node is its own component, so the maximum component size is 1 if `n >= 1` and there is at least one query; otherwise it is 0. For each query, we call `merge(a, b)` on the DSU. If the two nodes are already in the same component, the merge is a no-op and the maximum component size does not change. Otherwise, the DSU merges the smaller component into the larger one (or we can choose any order), and we update the current maximum size by taking the max of the old maximum and the size of the new merged component. Since union by size ensures each merge at most doubles the size of the smaller set, the amortized time per operation is nearly O(α(n)) (inverse Ackermann). We process `q` queries, so total time is O((n + q) α(n)) ≈ O(n + q). Space is O(n) for the DSU parent and size arrays. Edge cases: `n` can be 0 (then return 0), self-loops and duplicate edges are irrelevant because DSU merges are idempotent, and queries with equal `a` and `b` are no-ops.
#include <vector>
#include <numeric>
#include <algorithm>

class DSU {
public:
    std::vector<int> parent;
    std::vector<int> size;

    DSU(int n) : parent(n + 1), size(n + 1, 1) {
        for (int i = 1; i <= n; ++i) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb) return;
        if (size[ra] < size[rb]) std::swap(ra, rb);
        parent[rb] = ra;
        size[ra] += size[rb];
    }

    int componentSize(int x) const {
        return size[find(x)];
    }

    int maxComponentSize() const {
        int max_size = 0;
        for (int i = 1; i < (int)size.size(); ++i) {
            if (parent[i] == i) {
                max_size = std::max(max_size, size[i]);
            }
        }
        return max_size;
    }
};

// Process all merges from queries and return the largest component size
// observed immediately after each individual query (maximum over all queries).
long long largestComponentAfterMerges(int n, const std::vector<std::pair<int,int>>& edges, const std::vector<std::pair<int,int>>& queries) {
    if (n <= 0 || queries.empty()) return 0;

    DSU dsu(n);

    // First, handle all initial edges (they are all merged at the start)
    for (const auto& e : edges) {
        if (e.first >= 1 && e.second >= 1 && e.first <= n && e.second <= n) {
            dsu.unite(e.first, e.second);
        }
    }

    long long current_max = dsu.maxComponentSize();
    long long global_max = current_max;

    // Process queries in order
    for (const auto& q : queries) {
        if (q.first >= 1 && q.second >= 1 && q.first <= n && q.second <= n) {
            dsu.unite(q.first, q.second);
            // After merge, recompute max quickly by checking the new component's size
            // and the previous global max (since merging only increases sizes).
            int new_size = dsu.componentSize(q.first);
            current_max = std::max(current_max, (long long)new_size);
            global_max = std::max(global_max, current_max);
        }
    }

    return global_max;
}
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Simple case: 1 query merging two nodes
    {
        int n = 2;
        std::vector<std::pair<int,int>> edges;
        std::vector<std::pair<int,int>> queries = {{1,2}};
        assert(largestComponentAfterMerges(n, edges, queries) == 2);
    }

    // No queries -> returns 0
    {
        int n = 5;
        std::vector<std::pair<int,int>> edges;
        std::vector<std::pair<int,int>> queries;
        assert(largestComponentAfterMerges(n, edges, queries) == 0);
    }

    // Already connected via initial edges, queries are no-ops
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{1,2}, {2,3}};
        std::vector<std::pair<int,int>> queries = {{1,3}, {1,1}};
        assert(largestComponentAfterMerges(n, edges, queries) == 3);
    }

    // Multiple merges building up size progressively
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges;
        std::vector<std::pair<int,int>> queries = {{1,2}, {3,4}, {2,3}};
        // after 1st q: max=2
        // after 2nd q: max=2
        // after 3rd q: max=4
        assert(largestComponentAfterMerges(n, edges, queries) == 4);
    }

    // Duplicate merges and self-loops
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{1,1}, {2,2}};
        std::vector<std::pair<int,int>> queries = {{1,2}, {2,1}, {3,3}};
        assert(largestComponentAfterMerges(n, edges, queries) == 2);
    }

    // Larger chain: each merge connects a new node
    {
        int n = 5;
        std::vector<std::pair<int,int>> edges;
        std::vector<std::pair<int,int>> queries = {{1,2}, {2,3}, {3,4}, {4,5}};
        // after each merge, the size grows: 2,3,4,5 -> final max=5
        assert(largestComponentAfterMerges(n, edges, queries) == 5);
    }

    // Out-of-range nodes should be ignored (or no effect)
    {
        int n = 2;
        std::vector<std::pair<int,int>> edges = {{0,3}};
        std::vector<std::pair<int,int>> queries = {{1,2}, {2,5}};
        // only merge 1 and 2 -> size 2
        assert(largestComponentAfterMerges(n, edges, queries) == 2);
    }

    return 0;
}
