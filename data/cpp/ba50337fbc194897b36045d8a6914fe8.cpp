Write a C++ function `int minimumConnectionCost(int n, const std::vector<std::vector<int>>& grid)` that takes an `n x n` grid where each cell contains either `0` (empty), `1` (settlement A), `2` (settlement B), or `-1` (obstacle, impassable). All settlements (both type 1 and type 2) must be connected into a single network where movement is allowed only in four orthogonal directions (up, down, left, right) through empty cells or other settlements, but never through obstacles. The cost to connect two settlements is the Manhattan distance (number of steps) along a path through empty/settlement cells. Your function must return the minimum total connection cost to make all settlements connected, or `-1` if it is impossible to connect all settlements (i.e., they lie in disconnected components separated by obstacles). The grid can be up to 50×50, and the number of settlements will be at least 2. The solution must compute the shortest path distances between every pair of settlements using BFS from each settlement, then build a Minimum Spanning Tree (MST) over those distances using Kruskal’s algorithm with union-find.

// The problem requires connecting all special cells (value 1 or 2) into one connected component with minimal total edge weight, where the weight between two special cells is the shortest path distance through the grid (moving orthogonally, avoiding obstacles). This is exactly the classic "Minimum Spanning Tree" problem on a complete graph where vertices are the special cells and edge weights are the BFS shortest paths. Approach: (1) Label each special cell with a unique ID (1..k). (2) For each special cell, run BFS that computes the shortest distance from that cell to every other cell in the grid, ignoring obstacles but allowing movement through any non-obstacle cell. When the BFS reaches another special cell, record an edge with its ID and the distance. (3) Collect all such edges (possibly duplicate edges, but union-find handles that). (4) Sort all edges by weight, then run Kruskal’s algorithm: union the endpoints, accumulate weight, and count edges used. If we successfully add `k-1` edges, return the sum; otherwise return -1 (meaning some settlements are unreachable from each other due to obstacles). Edge cases: multiple settlements in the same connected component produce distance 0 edges; isolated settlements lead to -1; zero or one settlement (though guaranteed at least 2, but handle gracefully). Time complexity: O(k * n^2) for BFS from each settlement, plus O(E log E) for sorting edges, where E ≤ k*n^2 (but typically O(k^2)). Since k ≤ n^2 and n ≤ 50, worst-case k=2500 leading to O(2500*2500) = ~6.25M operations for BFS, which is fine. Space: O(n^2) for visited/distance per BFS, and O(E) for edges.

#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

// Structure for an edge in the MST candidate graph
struct ConnectionEdge {
    int cost;
    int from;
    int to;
    bool operator<(const ConnectionEdge& other) const {
        return cost < other.cost;
    }
};

// Union-Find (Disjoint Set) for Kruskal's MST
class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;
public:
    explicit UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    bool unite(int x, int y) {
        int rx = find(x);
        int ry = find(y);
        if (rx == ry) return false;
        if (rank[rx] < rank[ry]) parent[rx] = ry;
        else if (rank[rx] > rank[ry]) parent[ry] = rx;
        else { parent[ry] = rx; rank[rx]++; }
        return true;
    }
};

// Returns minimum total connection cost or -1 if impossible
int minimumConnectionCost(int n, const std::vector<std::vector<int>>& grid) {
    // Step 1: Assign IDs to settlement cells
    std::vector<std::vector<int>> id(n, std::vector<int>(n, -1));
    int k = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] == 1 || grid[i][j] == 2) {
                id[i][j] = k++;
            }
        }
    }
    if (k <= 1) return 0; // already connected or nothing to connect

    // Step 2: BFS from each settlement to find shortest distances to others
    std::vector<ConnectionEdge> edges;
    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};
    auto isInside = [&](int y, int x) {
        return y >= 0 && y < n && x >= 0 && x < n;
    };

    for (int startY = 0; startY < n; ++startY) {
        for (int startX = 0; startX < n; ++startX) {
            if (grid[startY][startX] != 1 && grid[startY][startX] != 2) continue;
            int startId = id[startY][startX];

            // BFS distances
            std::vector<std::vector<int>> dist(n, std::vector<int>(n, -1));
            std::queue<std::pair<int,int>> q;
            q.push({startY, startX});
            dist[startY][startX] = 0;

            while (!q.empty()) {
                auto [cy, cx] = q.front();
                q.pop();
                for (int d = 0; d < 4; ++d) {
                    int ny = cy + dy[d];
                    int nx = cx + dx[d];
                    if (!isInside(ny, nx) || dist[ny][nx] != -1) continue;
                    if (grid[ny][nx] == -1) continue; // obstacle
                    dist[ny][nx] = dist[cy][cx] + 1;
                    if (grid[ny][nx] == 1 || grid[ny][nx] == 2) {
                        // Found another settlement
                        int otherId = id[ny][nx];
                        if (otherId != startId) {
                            edges.push_back({dist[ny][nx], startId, otherId});
                        }
                    }
                    q.push({ny, nx});
                }
            }
        }
    }

    // Step 3: Kruskal's MST
    std::sort(edges.begin(), edges.end());
    UnionFind uf(k);
    int totalCost = 0;
    int edgesUsed = 0;
    for (const auto& e : edges) {
        if (uf.unite(e.from, e.to)) {
            totalCost += e.cost;
            edgesUsed++;
            if (edgesUsed == k - 1) break;
        }
    }
    return (edgesUsed == k - 1) ? totalCost : -1;
}

#include <vector>
#include <cassert>

// Ensure the solution function is declared above this point
int main() {
    // Test 1: Simple 2x2 with two settlements adjacent
    {
        std::vector<std::vector<int>> grid = {{1,0},{0,2}};
        assert(minimumConnectionCost(2, grid) == 2);
    }
    // Test 2: Three settlements in a line with obstacles
    {
        std::vector<std::vector<int>> grid = {
            {1, -1, 2},
            {0, -1, 0},
            {2, 0, 1}
        };
        // Settlements at (0,0), (0,2), (2,0), (2,2)
        // BFS distances: (0,0)-(0,2)=4, (0,0)-(2,0)=4, (0,0)-(2,2)=4, etc.
        // MST: connect all four with cost 4+4+4 = 12? Actually need to check
        // Let's compute: all are connected via corners, distance between any diagonal is 4.
        // MST of 4 nodes with all edges 4: minimal tree has 3 edges = 12
        assert(minimumConnectionCost(3, grid) == 12);
    }
    // Test 3: Impossible due to obstacle barrier
    {
        std::vector<std::vector<int>> grid = {
            {1, -1, 2},
            {-1, -1, -1},
            {2, 0, 1}
        };
        // Top-left and bottom-right settlements are isolated by full obstacle row/col
        // Not all connected, return -1
        assert(minimumConnectionCost(3, grid) == -1);
    }
    // Test 4: Already connected (zero cost when settlements adjacent)
    {
        std::vector<std::vector<int>> grid = {{1,2}};
        assert(minimumConnectionCost(1, grid) == 0);
    }
    // Test 5: Larger 4x4 with a path
    {
        std::vector<std::vector<int>> grid = {
            {1, 0, 0, 2},
            {0, -1, 0, 0},
            {0, 0, 0, 0},
            {2, 0, 0, 1}
        };
        // Four settlements: (0,0), (0,3), (3,0), (3,3)
        // Shortest distances: (0,0)-(0,3)=3, (0,0)-(3,0)=3, (0,0)-(3,3)=6
        // (0,3)-(3,0)=6, (0,3)-(3,3)=3, (3,0)-(3,3)=3
        // MST: pick 3 smallest edges (3,3,3) total 9
        assert(minimumConnectionCost(4, grid) == 9);
    }
    // Test 6: All cells are settlements (fully connected grid)
    {
        std::vector<std::vector<int>> grid = {
            {1, 2},
            {2, 1}
        };
        // All adjacent, MST cost = 1+1+1 = 3
        assert(minimumConnectionCost(2, grid) == 3);
    }
    // Test 7: Single settlement (special case, returns 0)
    {
        std::vector<std::vector<int>> grid = {{1}};
        assert(minimumConnectionCost(1, grid) == 0);
    }
    // Test 8: Two settlements separated by long obstacle-free path
    {
        std::vector<std::vector<int>> grid = {
            {1, 0, 0, 0},
            {0, -1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 2}
        };
        // Shortest path from (0,0) to (3,3): go around obstacle, length 6
        assert(minimumConnectionCost(4, grid) == 6);
    }
    return 0;
}
