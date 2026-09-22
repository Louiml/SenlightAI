// Write a C++ function `buildMatrix` that takes an integer `k` and two vectors of pairs representing row constraints (`above`, `below`) and column constraints (`left`, `right`). The function should return a `k x k` matrix containing the numbers `1` through `k`, each placed exactly once, such that for every row constraint `(a, b)`, the number `a` appears in a row strictly above the row containing `b`, and for every column constraint `(a, b)`, `a` appears in a column strictly left of the column containing `b`. If no such matrix exists, return an empty `vector<vector<int>>`. The constraints are guaranteed to be non-cyclic individually (no cycles among row constraints and no cycles among column constraints separately), but the combined placement may still be impossible due to conflicting row/column positions. Assume `1 <= k <= 10`. The function must be self-contained, using only standard C++ libraries, and must not modify the input vectors.
The problem decomposes into two independent topological ordering problems: one for rows and one for columns. For the row constraints, build a directed graph where an edge from `a` to `b` means `a` must be above `b`. Since there are no cycles, we can perform a topological sort (e.g., using Kahn's algorithm or DFS) to assign each number a distinct row index. The same is done for column constraints to assign each number a distinct column index. Because `k` is small (≤10), we can also use a simpler approach: repeatedly pick a number with no unassigned predecessor in the constraint graph. However, the given snippet uses a DFS-based approach that tracks the maximum depth (score) to assign positions, but a standard topological sort is more straightforward. Edge cases: (1) If the graph has a cycle, return empty matrix. (2) If two numbers would occupy the same row or column (which cannot happen because topological sort of `k` nodes gives `k` distinct positions), but in theory if constraints are consistent, positions are unique. (3) If the topological order is not unique, any valid order works; we just assign row indices in the order of topological sort (e.g., first in order gets row 0, second gets row 1, etc.) and similarly for columns. After obtaining row and column indices for each number, place them in the matrix. Time complexity: O(k + E) for each topological sort, where E is the number of constraints (at most k^2). Space complexity: O(k + E) for adjacency lists and in-degree counts. Since k ≤ 10, this is trivial.
#include <vector>
#include <queue>
#include <algorithm>

// Build a k x k matrix with numbers 1..k such that rowConstraints (above, below)
// and colConstraints (left, right) are satisfied. Return empty matrix if impossible.
std::vector<std::vector<int>> buildMatrix(
    int k,
    const std::vector<std::pair<int,int>>& rowConstraints,
    const std::vector<std::pair<int,int>>& colConstraints) {
    
    // Helper to compute topological order of a directed graph with k nodes (0..k-1).
    auto topoOrder = [&](const std::vector<std::pair<int,int>>& edges) -> std::vector<int> {
        std::vector<int> indeg(k, 0);
        std::vector<std::vector<int>> adj(k);
        for (const auto& e : edges) {
            int u = e.first - 1; // convert to 0-based
            int v = e.second - 1;
            adj[u].push_back(v);
            indeg[v]++;
        }
        std::queue<int> q;
        for (int i = 0; i < k; ++i) if (indeg[i] == 0) q.push(i);
        std::vector<int> order;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (int v : adj[u]) {
                if (--indeg[v] == 0) q.push(v);
            }
        }
        if ((int)order.size() != k) return {}; // cycle detected
        return order;
    };

    std::vector<int> rowOrder = topoOrder(rowConstraints);
    if (rowOrder.empty()) return {};
    std::vector<int> colOrder = topoOrder(colConstraints);
    if (colOrder.empty()) return {};

    // Assign row and column indices based on order position.
    std::vector<int> rowIdx(k), colIdx(k);
    for (int i = 0; i < k; ++i) {
        rowIdx[rowOrder[i]] = i;
        colIdx[colOrder[i]] = i;
    }

    // Fill matrix.
    std::vector<std::vector<int>> ans(k, std::vector<int>(k, 0));
    for (int num = 0; num < k; ++num) {
        ans[rowIdx[num]][colIdx[num]] = num + 1;
    }
    return ans;
}
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Example 1: Simple 2x2 with constraints 1 above 2, 1 left 2.
    {
        int k = 2;
        std::vector<std::pair<int,int>> row = {{1,2}};
        std::vector<std::pair<int,int>> col = {{1,2}};
        auto res = buildMatrix(k, row, col);
        assert(!res.empty());
        // Check constraints manually: 1 must be above and left of 2.
        int r1, c1, r2, c2;
        for (int i = 0; i < k; ++i) for (int j = 0; j < k; ++j) {
            if (res[i][j] == 1) { r1 = i; c1 = j; }
            if (res[i][j] == 2) { r2 = i; c2 = j; }
        }
        assert(r1 < r2 && c1 < c2);
    }

    // Example 2: 3x3 with row constraints 1<2 and 2<3, col constraints 1<3.
    {
        int k = 3;
        std::vector<std::pair<int,int>> row = {{1,2},{2,3}};
        std::vector<std::pair<int,int>> col = {{1,3}};
        auto res = buildMatrix(k, row, col);
        assert(!res.empty());
        // Verify each number appears exactly once.
        int count[4] = {0};
        for (auto& v : res) for (int x : v) if (x != 0) count[x]++;
        for (int i = 1; i <= 3; ++i) assert(count[i] == 1);
    }

    // Example 3: Cycle in rows -> impossible.
    {
        int k = 2;
        std::vector<std::pair<int,int>> row = {{1,2},{2,1}};
        std::vector<std::pair<int,int>> col = {};
        auto res = buildMatrix(k, row, col);
        assert(res.empty());
    }

    // Example 4: No constraints -> any placement works, but matrix must be valid.
    {
        int k = 1;
        std::vector<std::pair<int,int>> row, col;
        auto res = buildMatrix(k, row, col);
        assert(res.size() == 1 && res[0][0] == 1);
    }

    // Example 5: Conflicting constraints that force same row/col? Not possible with topological sort, but test a valid case with multiple numbers.
    {
        int k = 3;
        std::vector<std::pair<int,int>> row = {{1,2}};
        std::vector<std::pair<int,int>> col = {{2,3}};
        auto res = buildMatrix(k, row, col);
        assert(!res.empty());
        // Check each number appears once.
        int count[4] = {0};
        for (auto& v : res) for (int x : v) if (x != 0) count[x]++;
        for (int i = 1; i <= 3; ++i) assert(count[i] == 1);
    }

    return 0;
}
