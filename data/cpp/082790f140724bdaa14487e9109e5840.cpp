Write a C++ function `void subtreeRanks(const std::vector<std::vector<int>>& children, const std::vector<int>& values, std::vector<std::array<long long,3>>& result)` that, given a rooted tree with root at node 1 (nodes numbered 1..n), a list of `children` (where `children[i]` is a vector of child node indices for node `i`), and a list of `values` (where `values[i]` is the original key value associated with node `i`, not necessarily unique), computes for each node `u` three quantities:  
- `result[u][0]`: number of nodes in the subtree of `u` (including `u` itself? No—see note) that have a *strictly smaller* value than `values[u]` and are *already visited* in a post-order DFS? Actually define precisely: In the original snippet, the DFS visits children first, then computes counts from the current Fenwick tree state (which contains all previously visited nodes from subtrees already fully processed) and then adds the current node to the tree. The counts are:  
  - `ans[u][0]` = number of nodes in the subtree of `u` that have values strictly less than `values[u]` (excluding `u` because `u` is not yet inserted).  
  - `ans[u][1]` = number of nodes in the subtree of `u` that have values equal to `values[u]` (excluding `u` itself).  
  - `ans[u][2]` = number of nodes in the subtree of `u` that have values strictly greater than `values[u]`.  
For leaf nodes, all three values are 0. The algorithm must process nodes in a DFS from root 1, using a Binary Indexed Tree (Fenwick tree) that stores counts of values encountered so far in the traversal. The function should assume the tree is connected and acyclic, and `children` lists are pre‑built. Return the results in the `result` vector which must be sized `n+1` (index 1..n). The input `values` are original integers; you may compress them to ranks. Note that the original snippet compresses values by sorting unique values and using lower_bound; you should do the same to handle up to 5e5 nodes. Provide the implementation as a standalone function; do not include `main`.

// The solution uses a Fenwick tree (Binary Indexed Tree) to maintain counts of values already processed during a DFS. The key idea is that when we finish processing all children of a node `u`, the Fenwick tree contains exactly the nodes in the subtrees of `u`'s children, because we insert each node only after processing its entire subtree. We then query the tree for three ranges: (1) values less than `values[u]`, (2) values exactly equal to `values[u]`, and (3) values greater than `values[u]`. To make equality queries possible, we compress the original values to ranks from 1..m where m is the number of distinct values; after compression, equal values map to the same rank. We perform queries before inserting `u` itself, so the counts exclude `u`. We then insert `u` with its rank. For each node, we compute:  
// - `less = getsum(rank-1)`  
// - `equal = getsum(rank) - getsum(rank-1)`  
// - `greater = getsum(total_count_so_far) - getsum(rank)` where total_count_so_far is the number of nodes processed so far (we can track it).  
// Actually, the original code uses `getsum(n)` where `n` is the number of nodes, because the Fenwick tree size is `n` and after compression all ranks are within 1..n. We can maintain the total processed count separately, but `getsum(n)` works because all ranks are ≤ n. We must be careful to store `a`, `b`, `c` before processing children, as in the original, to get the counts from the subtree only, because the Fenwick tree accumulates global counts. The original computes `a = getsum(rank-1)`, `b = getsum(rank)-a`, `c = getsum(n)-b-a` *before* recursing into children, then after children it subtracts the pre‑values to get the subtree-only counts. That is correct because the pre‑values are counts from previously visited subtrees (outside the current subtree). So we replicate exactly that logic. Edge cases: single node tree (all zeros), duplicate values (equal counts correctly computed), large n (use long long). Time complexity: O(n log n) for building the Fenwick tree operations (each node processed once, each query/update O(log n)) plus O(n log n) for compression sort and lower_bound. Space complexity: O(n) for the Fenwick tree, adjacency list, and result array.

#include <vector>
#include <algorithm>
#include <cstdint>
#include <array>

// Fenwick tree for counts, 1-indexed
class FenwickCount {
    std::vector<long long> bit;
    int n;
public:
    FenwickCount(int size) : n(size), bit(size + 1, 0) {}
    void add(int idx, long long delta) {
        for (; idx <= n; idx += idx & (-idx)) bit[idx] += delta;
    }
    long long sum(int idx) const {
        long long res = 0;
        for (; idx > 0; idx -= idx & (-idx)) res += bit[idx];
        return res;
    }
};

void subtreeRanks(const std::vector<std::vector<int>>& children,
                  const std::vector<int>& values,
                  std::vector<std::array<long long,3>>& result) {
    int n = (int)values.size() - 1; // nodes 1..n
    result.assign(n + 1, {0,0,0});
    if (n == 0) return;

    // Coordinate compression: rank values 1..m (m <= n)
    std::vector<int> sorted = values;
    std::sort(sorted.begin() + 1, sorted.end());
    std::vector<int> rank(n + 1);
    for (int i = 1; i <= n; ++i) {
        rank[i] = (int)(std::lower_bound(sorted.begin() + 1, sorted.end(), values[i]) - sorted.begin());
    }

    FenwickCount ft(n);

    // Recursive DFS
    std::function<void(int)> dfs = [&](int u) {
        // Counts in the whole tree before processing children
        long long a = ft.sum(rank[u] - 1);
        long long b = ft.sum(rank[u]) - a;
        long long c = ft.sum(n) - b - a;

        // Process children depth-first
        for (int v : children[u]) {
            dfs(v);
        }

        // After children, subtract pre-counts to get subtree-only contributions
        result[u][0] = ft.sum(rank[u] - 1) - a;
        result[u][1] = ft.sum(rank[u]) - b - ft.sum(rank[u] - 1);
        result[u][2] = ft.sum(n) - c - ft.sum(rank[u]);

        // Insert current node
        ft.add(rank[u], 1);
    };

    dfs(1);
}

#include <cassert>
#include <vector>
#include <array>
#include <functional>

// Include the solution function here or link it.

int main() {
    // Test 1: Single node
    {
        std::vector<std::vector<int>> children(2); // index 1 only
        std::vector<int> values = {0, 42};
        std::vector<std::array<long long,3>> result;
        subtreeRanks(children, values, result);
        assert(result[1] == std::array<long long,3>{0,0,0});
    }

    // Test 2: Simple star, root 1 with children 2,3,4
    {
        std::vector<std::vector<int>> children(5);
        children[1] = {2,3,4};
        std::vector<int> values = {0, 10, 20, 5, 10};
        std::vector<std::array<long long,3>> result;
        subtreeRanks(children, values, result);
        // Node 1: subtree has values [10,20,5,10] (including root, but root not counted). Values less than 10: 5 (1 node). Equal to 10: 10 (1 node, child 4). Greater: 20 (1 node). 
        assert(result[1][0] == 1);
        assert(result[1][1] == 1);
        assert(result[1][2] == 1);
        // Node 2 leaf: all zero
        assert(result[2] == std::array<long long,3>{0,0,0});
        // Node 3 leaf: all zero
        assert(result[3] == std::array<long long,3>{0,0,0});
        // Node 4 leaf: zero
        assert(result[4] == std::array<long long,3>{0,0,0});
    }

    // Test 3: Chain 1->2->3, values increasing 1,2,3
    {
        std::vector<std::vector<int>> children(4);
        children[1] = {2};
        children[2] = {3};
        std::vector<int> values = {0, 1, 2, 3};
        std::vector<std::array<long long,3>> result;
        subtreeRanks(children, values, result);
        // Node 3 leaf: 0,0,0
        assert(result[3] == std::array<long long,3>{0,0,0});
        // Node 2 subtree: values {2,3}. less:0, equal:0, greater:1 (3>2)
        assert(result[2][0] == 0);
        assert(result[2][1] == 0);
        assert(result[2][2] == 1);
        // Node 1 subtree: values {1,2,3}. less:0, equal:0, greater:2 (2,3)
        assert(result[1][0] == 0);
        assert(result[1][1] == 0);
        assert(result[1][2] == 2);
    }

    // Test 4: Chain with duplicate values: 1->2->3, values 5,5,5
    {
        std::vector<std::vector<int>> children(4);
        children[1] = {2};
        children[2] = {3};
        std::vector<int> values = {0, 5, 5, 5};
        std::vector<std::array<long long,3>> result;
        subtreeRanks(children, values, result);
        // Node 3 leaf: 0,0,0
        assert(result[3] == std::array<long long,3>{0,0,0});
        // Node 2 subtree: children {3} values {5,5}. less:0, equal:1 (child 3), greater:0
        assert(result[2][0] == 0);
        assert(result[2][1] == 1);
        assert(result[2][2] == 0);
        // Node 1 subtree: children {2,3} values {5,5,5}. less:0, equal:2, greater:0
        assert(result[1][0] == 0);
        assert(result[1][1] == 2);
        assert(result[1][2] == 0);
    }

    // Test 5: Deeper tree with mixed values
    {
        std::vector<std::vector<int>> children(7);
        children[1] = {2,3};
        children[2] = {4,5};
        children[3] = {6};
        std::vector<int> values = {0, 8, 3, 10, 1, 6, 14};
        std::vector<std::array<long long,3>> result;
        subtreeRanks(children, values, result);
        // Compute manually:
        // Node 4 leaf: 0,0,0
        // Node 5 leaf: 0,0,0
        // Node 6 leaf: 0,0,0
        // Node 2 subtree: values {3,1,6}. less than 3: 1 (node 4), equal:0, greater:1 (node 5)
        assert(result[2][0] == 1);
        assert(result[2][1] == 0);
        assert(result[2][2] == 1);
        // Node 3 subtree: values {10,14}. less:0, equal:0, greater:1 (node 6)
        assert(result[3][0] == 0);
        assert(result[3][1] == 0);
        assert(result[3][2] == 1);
        // Node 1 subtree: values {8,3,1,6,10,14}. less than 8: 3 (3,1,6), equal:0, greater:2 (10,14)
        assert(result[1][0] == 3);
        assert(result[1][1] == 0);
        assert(result[1][2] == 2);
    }

    return 0;
}
