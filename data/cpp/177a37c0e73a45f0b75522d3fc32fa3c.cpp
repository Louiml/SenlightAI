// Write a C++ function `maximumFrogsSaved` that takes a rectangular lake grid of size `rows x cols` (both at least 1), a list of stone positions (each given as 1-indexed `(x, y)` coordinates), and returns the maximum number of frogs that can be saved. A frog can be placed on any land cell (non-stone cell). Two frogs are considered friends if their cells are orthogonally adjacent (up, down, left, right). The goal is to place frogs on as many land cells as possible, but no two friends can both be placed—meaning you must choose a set of land cells where no two chosen cells are adjacent. The output is the size of the largest such independent set in the grid graph of land cells. This is equivalent to the complement of maximum matching in a bipartite graph (since the grid is bipartite by checkerboard coloring). Return that integer.

// The lake grid with stones removed forms a bipartite graph where each land cell is a node and edges connect orthogonally adjacent land cells. Since the grid is bipartite (color cells by parity of row+col), the maximum independent set size in a bipartite graph equals total nodes minus maximum matching size (Kőnig's theorem). We build adjacency lists for each land cell, run standard Hopcroft–Karp or simple augmenting-path DFS for bipartite matching, and then compute `totalLand - maxMatching`. Edge cases: if no land cells, return 0; if the graph has no edges, max matching is 0 and independent set is all land. Complexity: For `N = rows*cols` cells, building the graph is O(N), DFS-based augmenting path matching runs in O(V * E) worst-case (here V ≤ 10000, E ≤ 4V), so overall O(N^2) in worst case but typically fast for small grids. Space is O(N) for adjacency and matching arrays.

#include <vector>
#include <cstring>
#include <algorithm>

// Returns maximum number of frogs that can be placed without adjacency.
// Stones are given as 1-indexed (x, y). Rows and cols ≥ 1.
int maximumFrogsSaved(int rows, int cols, const std::vector<std::pair<int, int>>& stones) {
    const int N = rows * cols;
    std::vector<bool> isStone(N, false);
    for (const auto& stone : stones) {
        int x = stone.first - 1;
        int y = stone.second - 1;
        if (x >= 0 && x < rows && y >= 0 && y < cols) {
            isStone[x * cols + y] = true;
        }
    }

    // Count land cells and assign each land cell an index (compressed).
    std::vector<int> nodeId(N, -1);
    int totalLand = 0;
    for (int i = 0; i < N; ++i) {
        if (!isStone[i]) {
            nodeId[i] = totalLand++;
        }
    }
    if (totalLand == 0) return 0;

    // Build adjacency between land cells (undirected, we only add from left side to right side based on parity).
    std::vector<std::vector<int>> adj(totalLand);
    const int dx[4] = {-1, 0, 1, 0};
    const int dy[4] = {0, 1, 0, -1};

    auto isValid = [&](int r, int c) {
        return r >= 0 && r < rows && c >= 0 && c < cols;
    };

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int pos = r * cols + c;
            if (isStone[pos]) continue;
            // Only add edge from even parity node to odd parity node.
            if ((r + c) % 2 == 0) {
                int u = nodeId[pos];
                for (int k = 0; k < 4; ++k) {
                    int nr = r + dx[k];
                    int nc = c + dy[k];
                    if (isValid(nr, nc) && !isStone[nr * cols + nc]) {
                        int v = nodeId[nr * cols + nc];
                        adj[u].push_back(v);
                    }
                }
            }
        }
    }

    // Maximum bipartite matching (simple DFS augmenting path).
    std::vector<int> rightMatch(totalLand, -1);
    std::vector<bool> visited;
    std::function<bool(int)> tryAugment = [&](int u) -> bool {
        if (visited[u]) return false;
        visited[u] = true;
        for (int v : adj[u]) {
            if (rightMatch[v] == -1 || tryAugment(rightMatch[v])) {
                rightMatch[v] = u;
                return true;
            }
        }
        return false;
    };

    int matching = 0;
    for (int u = 0; u < totalLand; ++u) {
        if (!adj[u].empty()) { // only need to try from left side (even parity nodes)
            visited.assign(totalLand, false);
            if (tryAugment(u)) {
                ++matching;
            }
        }
    }

    // Max independent set = total nodes - max matching (bipartite graph).
    return totalLand - matching;
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // 1x1 with no stones → one frog
    assert(maximumFrogsSaved(1, 1, {}) == 1);
    // 1x1 with stone → zero
    assert(maximumFrogsSaved(1, 1, {{1,1}}) == 0);
    // 2x2 no stones: can place 2 (diagonal)
    assert(maximumFrogsSaved(2, 2, {}) == 2);
    // 2x2 with one stone in (1,1): remaining 3 cells form a path of length 2 edges → max independent = 2
    assert(maximumFrogsSaved(2, 2, {{1,1}}) == 2);
    // 3x3 no stones: checkerboard max independent = 5
    assert(maximumFrogsSaved(3, 3, {}) == 5);
    // 3x3 with stones blocking all center and corners → only edges left? test 1x3 row
    assert(maximumFrogsSaved(1, 3, {}) == 2); // can place at ends
    assert(maximumFrogsSaved(1, 3, {{1,2}}) == 2); // only middle stone → two ends
    assert(maximumFrogsSaved(1, 4, {}) == 2); // path of 4 → max independent 2
    // 2x3 no stones: total 6, max matching = 3, independent = 3
    assert(maximumFrogsSaved(2, 3, {}) == 3);
    // Completely stone grid
    std::vector<std::pair<int,int>> allStones;
    for (int r = 1; r <= 2; ++r)
        for (int c = 1; c <= 2; ++c)
            allStones.push_back({r, c});
    assert(maximumFrogsSaved(2, 2, allStones) == 0);

    return 0;
}
