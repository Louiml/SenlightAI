Write a C++ function named `minimumSnowmanClimb` that takes a 2D grid of non-negative integers (`vector<vector<int>>`), where `0` represents empty space, `2` represents a snowman (start), `3` represents a jewel (goal), and any other positive integer represents a piece of terrain. Consecutive non-zero cells on the same row form a single "block" (a contiguous segment). Each block is considered a node. Two blocks are connected by an edge if their horizontal intervals overlap (i.e., they share at least one column). The cost of that edge is the absolute difference in their row indices (heights). The goal is to start at the block containing the snowman and reach the block containing the jewel, minimizing the **maximum** edge cost along the path (i.e., the bottleneck cost). The function should return that minimum possible bottleneck value as an integer. If no path exists, return a large value like `-1` (or assume the grid guarantees a path). The grid may have multiple rows and columns, and blocks on the same row are separated by zeros.
#include <cassert>
#include <vector>
using namespace std;

int main() {
    // Test 1: Simple two-row overlapping blocks
    vector<vector<int>> g1 = {
        {2, 0, 3},
        {1, 1, 1}
    };
    assert(minimumSnowmanClimb(g1) == 1); // cost from row0 to row1 is 1

    // Test 2: Same block contains both start and goal
    vector<vector<int>> g2 = {
        {2, 3, 0},
        {0, 0, 0}
    };
    assert(minimumSnowmanClimb(g2) == 0);

    // Test 3: Need to go through multiple blocks with increasing cost
    vector<vector<int>> g3 = {
        {2, 0, 0},
        {1, 1, 0},
        {0, 0, 3}
    };
    // Blocks: row0 col0 (start), row1 col0-1, row2 col2 (goal) - no direct overlap between row1 and row2? row1 xmax=1, row2 xmin=2 -> no overlap. Actually no path. But if we add another column...
    // Let's make a proper path: row0 col0-1, row1 col1-2, row2 col2-3
    vector<vector<int>> g3b = {
        {2, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 3}
    };
    // Blocks: row0 col0-1, row1 col1-2, row2 col2-3. Overlaps: row0 & row1 (col1), row1 & row2 (col2). Costs: row0-row1=1, row1-row2=1. max=1
    assert(minimumSnowmanClimb(g3b) == 1);

    // Test 4: Multiple paths, choose lower bottleneck
    vector<vector<int>> g4 = {
        {2, 0, 0, 0},
        {5, 5, 0, 0},
        {0, 0, 1, 3}
    };
    // Blocks: row0 col0 (start), row1 col0-1, row2 col2-3 (goal). Overlap? row1 xmax=1, row2 xmin=2 -> no. So not connected. Add another block.
    vector<vector<int>> g4b = {
        {2, 0, 0, 0},
        {5, 5, 1, 0},
        {0, 0, 1, 3}
    };
    // Blocks: row0 col0 (start), row1 col0-1 (cost 5), row1 col2 (cost 1), row2 col2-3 (goal). Overlaps: start-row1col0-1 (cost5), row1col0-1 - row1col2? intervals [0,1] and [2,2] do not overlap. So not connected. Actually we need overlapping.
    // Simpler: Direct overlap with high cost vs low cost detour
    vector<vector<int>> g4c = {
        {2, 0, 0},
        {1, 1, 0},
        {0, 1, 3}
    };
    // Blocks: row0 col0 (start), row1 col0-1, row2 col1-2 (goal). Overlaps: row0-row1 (col0), row1-row2 (col1). Costs: 1,1 -> bottleneck 1
    assert(minimumSnowmanClimb(g4c) == 1);

    // Test 5: No path (should return -1)
    vector<vector<int>> g5 = {
        {2, 0, 0},
        {0, 0, 0},
        {0, 0, 3}
    };
    assert(minimumSnowmanClimb(g5) == -1);

    // Test 6: Larger grid with multiple blocks on same row separated by zeros
    vector<vector<int>> g6 = {
        {2, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 1, 3}
    };
    // Blocks: row0 col0 (start), row1 col0-1, row2 col2-3 (goal). Overlap? row1 xmax=1, row2 xmin=2 -> no. So no path. Actually not connected.
    // Let's adjust to have a path:
    vector<vector<int>> g6b = {
        {2, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 1, 3}
    };
    // Blocks: row0 col0 (start), row1 col0-2, row2 col2-3 (goal). Overlaps: start-row1 (col0), row1-row2 (col2). Costs: 1,1 -> bottleneck 1
    assert(minimumSnowmanClimb(g6b) == 1);

    // Test 7: High cost edge must be avoided via longer path
    vector<vector<int>> g7 = {
        {2, 0, 0, 0},
        {100, 0, 1, 1},
        {0, 0, 1, 3}
    };
    // Blocks: row0 col0 (start), row1 col0 (cost100), row1 col2-3 (goal? no goal), row2 col2-3 (goal). Let's build properly:
    // Row0: {2,0,0,0} -> block col0
    // Row1: {100,0,1,1} -> block col0, block col2-3
    // Row2: {0,0,1,3} -> block col2-3
    // Overlaps: start (row0 col0) with row1 col0 (cost100), row1 col2-3 with row2 col2-3 (cost1), start with row1 col2-3? no overlap (col0 vs col2-3). So only path is start->row1col0 (cost100) then row1col0 to row1col2-3? no overlap (col0 vs col2-3). So no path. 
    // To test bottleneck avoidance, need a low-cost detour:
    vector<vector<int>> g7b = {
        {2, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 3}
    };
    // Path: start(row0 col0)->row1 col0-1 (cost1)->row2 col1-2 (cost1)->row3 col2-3 (cost1). bottleneck=1.
    assert(minimumSnowmanClimb(g7b) == 1);

    // Test 8: Grid with single row
    vector<vector<int>> g8 = { {2, 0, 3} };
    // Blocks: row0 col0, row0 col2. No overlap. So no path -> -1
    assert(minimumSnowmanClimb(g8) == -1);
    // But if they are adjacent without zero:
    vector<vector<int>> g8b = { {2, 3, 0} };
    // Same block contains both. -> 0
    assert(minimumSnowmanClimb(g8b) == 0);

    // Test 9: More complex overlapping multiple paths
    vector<vector<int>> g9 = {
        {2, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 1, 1, 3}
    };
    // Blocks: row0 col0-1, row1 col0-2, row2 col1-3 (goal at col3). Overlaps: row0-row1 (col0-1) cost1, row1-row2 (col1-2) cost1. bottleneck=1
    assert(minimumSnowmanClimb(g9) == 1);

    // Test 10: Bottleneck must be higher than 1 due to gap
    vector<vector<int>> g10 = {
        {2, 0, 0},
        {5, 5, 0},
        {0, 5, 3}
    };
    // Blocks: row0 col0 (start), row1 col0-1, row2 col1-2 (goal). Overlaps: start-row1 (col0) cost5, row1-row2 (col1) cost5. bottleneck=5
    assert(minimumSnowmanClimb(g10) == 5);

    return 0;
}
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

using namespace std;

// Return the minimum possible maximum edge cost from the block containing 2 to the block containing 3.
int minimumSnowmanClimb(const vector<vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) return -1;
    int H = grid.size();
    int W = grid[0].size();

    // Build blocks (nodes) as contiguous horizontal segments of non-zero cells.
    struct Node {
        int h;   // row index
        int xmin;
        int xmax;
    };
    vector<Node> nodes;
    int start = -1, goal = -1;

    for (int i = 0; i < H; ++i) {
        int j = 0;
        while (j < W) {
            if (grid[i][j] != 0) {
                // Start a new block at column j
                int xmin = j;
                int xmax = j;
                // Extend to the right while consecutive non-zero
                while (j + 1 < W && grid[i][j + 1] != 0) {
                    ++j;
                    xmax = j;
                }
                // Now block is from xmin to xmax inclusive
                int idx = nodes.size();
                nodes.push_back({i, xmin, xmax});
                // Check if this block contains start or goal
                for (int col = xmin; col <= xmax; ++col) {
                    if (grid[i][col] == 2) start = idx;
                    else if (grid[i][col] == 3) goal = idx;
                }
                ++j; // move past the block
            } else {
                ++j;
            }
        }
    }

    if (start == -1 || goal == -1) return -1;
    if (start == goal) return 0;

    // Build adjacency list
    int N = nodes.size();
    vector<vector<pair<int,int>>> adj(N); // {neighbor, cost}
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            const Node& a = nodes[i];
            const Node& b = nodes[j];
            // Check if intervals overlap
            if (a.xmax < b.xmin || b.xmax < a.xmin) continue;
            int cost = abs(a.h - b.h);
            adj[i].push_back({j, cost});
            adj[j].push_back({i, cost});
        }
    }

    // Dijkstra-style minimax (bottleneck) search
    const int INF = 1e9;
    vector<int> dist(N, INF);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq; // {current_bottleneck, node}
    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;
        if (u == goal) return d;
        for (const auto& [v, w] : adj[u]) {
            int new_d = max(d, w);
            if (new_d < dist[v]) {
                dist[v] = new_d;
                pq.push({new_d, v});
            }
        }
    }
    return -1; // unreachable
}
// The problem is a classic minimax (bottleneck) shortest path problem. We model each contiguous horizontal segment of non-zero cells as a node. For each pair of nodes (blocks), if their column intervals overlap (i.e., `n1.xmax < n2.xmin` or `n2.xmax < n1.xmin` are false), then they are adjacent, and the edge weight is the absolute difference in their row indices (`abs(h1 - h2)`). We then run a modified Dijkstra (or BFS-like relaxation) where instead of summing costs, we track the maximum edge cost encountered so far. We initialize the start node's cost to 0 and all others to infinity (or a large number). For each neighbor, the new cost is `max(current_cost, edge_weight)`. If this is less than the neighbor's stored cost, we update and push it into a priority queue (or a simple queue in the spirit of the snippet, but a priority queue gives better performance; since edge weights are non-negative, a simple queue with repeated relaxations also works because we are only minimizing the maximum, which has a monotonic property). We continue until the goal node is reached or the queue empties. If the queue empties without reaching the goal, the return value would be the sentinel (but in the problem, a path is guaranteed). The grid can be up to, say, 1000 rows and columns, with at most `rows * columns` nodes but in practice much fewer because blocks merge consecutive cells. Time complexity is O(N + E log N) with a priority queue where N is number of blocks and E is number of edges, and E can be up to O(N^2) in worst case. Space is O(N + E). Edge cases include: a single cell grid where snowman and jewel are the same block (cost 0), blocks on the same row that are adjacent but separated by zeros (they have no overlap of columns, so no edge within that row), and blocks on different rows that have overlapping column ranges.
