/*
You are given a 2D grid of size `w` × `h` (1 ≤ w, h ≤ 50), where each cell can be either empty or blocked. Some empty cells are designated as "portals" with a specific destination cell (also empty) and a travel time cost. From any non-portal empty cell, you may move to an adjacent (up/down/left/right) empty cell in 1 time unit, but you cannot pass through blocked cells. From a portal cell, you must take the portal to its designated destination, paying the given time cost (the portal is one-way and can be used any number of times). Your start is the top-left cell (0,0) and your goal is the bottom-right cell (w-1,h-1). Write a C++ function `int shortestPathTime(int w, int h, const vector<vector<int>>& blocked, const vector<tuple<int,int,int,int,int>>& portals)` that returns the shortest possible time to reach the goal, or `-1` if impossible. However, there is a complication: if the graph contains a negative-weight cycle reachable from the start (making the shortest path undefined as it can be arbitrarily small), the function should instead return `-2` to indicate "Never" (i.e., you can wait forever and get arbitrarily small cost). The portal times are given in the tuple as (x, y, destX, destY, time), where (x,y) is the portal cell and time can be any integer (including negative). Blocked cells are given as a list of coordinates (x,y) that are forbidden. Portals may lead to any empty cell, including the goal, and multiple portals cannot occupy the same cell. If no path exists, return -1 (Impossible). The grid coordinates are 0-indexed.
*/

#include <vector>
#include <tuple>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

int shortestPathTime(int w, int h, const vector<vector<int>>& blocked, const vector<tuple<int,int,int,int,int>>& portals) {
    const int INF = numeric_limits<int>::max() / 4;
    int n = w * h;
    vector<vector<pair<int,int>>> adj(n); // adjacency list: (neighbor, weight)

    // Build a set of blocked coordintes for quick lookup
    vector<vector<bool>> isBlocked(w, vector<bool>(h, false));
    for (const auto& b : blocked) {
        if (b.size() == 2) isBlocked[b[0]][b[1]] = true;
    }

    // Create a map from portal cell index to destination and time
    vector<int> portalDest(n, -1);
    vector<int> portalTime(n, 0);
    for (const auto& p : portals) {
        int x, y, dx, dy, t;
        tie(x, y, dx, dy, t) = p;
        int u = x * h + y;
        int v = dx * h + dy;
        portalDest[u] = v;
        portalTime[u] = t;
    }

    // Build graph edges
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            int u = i * h + j;
            if (isBlocked[i][j]) continue;
            if (portalDest[u] != -1) {
                // Portal edge
                adj[u].push_back({portalDest[u], portalTime[u]});
            } else {
                // Normal moves
                for (int k = 0; k < 4; ++k) {
                    int ni = i + dr[k];
                    int nj = j + dc[k];
                    if (ni >= 0 && ni < w && nj >= 0 && nj < h && !isBlocked[ni][nj]) {
                        int v = ni * h + nj;
                        adj[u].push_back({v, 1});
                    }
                }
            }
        }
    }

    int src = 0; // (0,0)
    int des = (w-1) * h + (h-1);

    // If start or goal is blocked, impossible
    if (isBlocked[0][0] || isBlocked[w-1][h-1]) return -1;

    // Bellman-Ford with negative cycle detection
    vector<int> dist(n, INF);
    dist[src] = 0;

    // Relax all edges n-1 times
    for (int iter = 0; iter < n-1; ++iter) {
        bool updated = false;
        for (int u = 0; u < n; ++u) {
            if (dist[u] == INF) continue;
            for (const auto& edge : adj[u]) {
                int v = edge.first;
                int wgt = edge.second;
                if (dist[v] > dist[u] + wgt) {
                    dist[v] = dist[u] + wgt;
                    updated = true;
                }
            }
        }
        if (!updated) break;
    }

    // Check for reachable negative cycle: try one more relaxation pass
    bool hasNegativeCycle = false;
    for (int u = 0; u < n; ++u) {
        if (dist[u] == INF) continue;
        for (const auto& edge : adj[u]) {
            int v = edge.first;
            int wgt = edge.second;
            if (dist[v] > dist[u] + wgt) {
                hasNegativeCycle = true;
                break;
            }
        }
        if (hasNegativeCycle) break;
    }

    if (hasNegativeCycle) return -2; // Never
    if (dist[des] == INF) return -1; // Impossible
    return dist[des];
}

#include <cassert>
#include <vector>
#include <tuple>
using namespace std;

// Assume the solution function is already defined above

int main() {
    // Test 1: Simple open grid 2x2, no blocked, no portals
    {
        int w=2,h=2;
        vector<vector<int>> blocked;
        vector<tuple<int,int,int,int,int>> portals;
        assert(shortestPathTime(w,h,blocked,portals) == 2);
    }

    // Test 2: Blocked middle, no path
    {
        int w=3,h=3;
        vector<vector<int>> blocked = {{1,1}};
        vector<tuple<int,int,int,int,int>> portals;
        // Start at (0,0) to get (2,2) impossible because center blocked? Actually can go around
        // Let's block entire row to make impossible
        blocked = {{1,0},{1,1},{1,2}};
        assert(shortestPathTime(w,h,blocked,portals) == -1);
    }

    // Test 3: Portal with negative time creates negative cycle
    {
        int w=2,h=2;
        vector<vector<int>> blocked;
        vector<tuple<int,int,int,int,int>> portals;
        portals.push_back(make_tuple(0,0,0,0,-5)); // self-loop negative on start
        assert(shortestPathTime(w,h,blocked,portals) == -2);
    }

    // Test 4: Portal with positive time accelerates path
    {
        int w=3,h=3;
        vector<vector<int>> blocked = {{1,1}};
        vector<tuple<int,int,int,int,int>> portals;
        // Portal from (0,0) to (2,2) with time 1
        portals.push_back(make_tuple(0,0,2,2,1));
        assert(shortestPathTime(w,h,blocked,portals) == 1);
    }

    // Test 5: Longer path with regular moves and one portal
    {
        int w=4,h=4;
        vector<vector<int>> blocked;
        vector<tuple<int,int,int,int,int>> portals;
        // Portal from (0,0) to (3,3) with time 5
        portals.push_back(make_tuple(0,0,3,3,5));
        // Regular path length = 6 (moves) vs portal 5
        assert(shortestPathTime(w,h,blocked,portals) == 5);
    }

    // Test 6: Goal unreachable due to blocked path despite portal
    {
        int w=3,h=3;
        vector<vector<int>> blocked = {{0,1},{1,1},{2,1}};
        vector<tuple<int,int,int,int,int>> portals;
        portals.push_back(make_tuple(0,0,2,2,1)); // portal but cannot reach portal? Actually portal is at start, so can go.
        // But goal is reachable via portal.
        // Let's block start? Not allowed. So let's block all cells except start and goal? That's not possible with portal.
        // Better: block all adjacent to start so only portal works, but portal leads directly to goal.
        // So it's reachable.
        // To make impossible, block both start and goal? Not allowed.
        // Let's block all possible paths, including portal destination? But portal destination is goal.
        // So it's always reachable if portal exists from start.
        // Let's just check a case where start has no portal and all neighbors blocked, and goal blocked.
        blocked = {{0,1},{1,0},{2,2}}; // goal (2,2) blocked
        portals.clear();
        assert(shortestPathTime(w,h,blocked,portals) == -1);
    }

    // Test 7: Negative portal but no negative cycle
    {
        int w=3,h=3;
        vector<vector<int>> blocked;
        vector<tuple<int,int,int,int,int>> portals;
        // From (0,0) to (1,1) with time -2, then from (1,1) to (2,2) with time 3
        portals.push_back(make_tuple(0,0,1,1,-2));
        portals.push_back(make_tuple(1,1,2,2,3));
        // Path cost = 1 (because -2 + 3 = 1)
        assert(shortestPathTime(w,h,blocked,portals) == 1);
    }

    // Test 8: Portal with negative edge but no cycle, shortest path uses it
    {
        int w=2,h=2;
        vector<vector<int>> blocked;
        vector<tuple<int,int,int,int,int>> portals;
        // From (0,0) to (1,1) with time -1
        portals.push_back(make_tuple(0,0,1,1,-1));
        assert(shortestPathTime(w,h,blocked,portals) == -1);
    }

    // Test 9: Large grid but simple to ensure no crash
    {
        int w=50,h=50;
        vector<vector<int>> blocked;
        vector<tuple<int,int,int,int,int>> portals;
        int result = shortestPathTime(w,h,blocked,portals);
        assert(result == 98);
    }

    // Test 10: Portal with negative cycle not reachable from start
    {
        int w=3,h=3;
        vector<vector<int>> blocked;
        vector<tuple<int,int,int,int,int>> portals;
        // Negative cycle at cell (2,2) (self-loop -1), but not reachable from start
        portals.push_back(make_tuple(2,2,2,2,-1));
        // Path from start to goal exists normally, cost 4 (for 3x3)
        assert(shortestPathTime(w,h,blocked,portals) == 4);
    }

    return 0;
}

// The problem is a single-source shortest path (SSSP) on a graph where nodes are all empty cells. Edges: from each non-portal cell to its 4 adjacent empty cells with weight 1; from each portal cell to its destination with weight equal to the portal time (which may be negative). The graph may contain negative-weight edges, so we use the Bellman-Ford algorithm (or SPFA with cycle detection) to detect negative cycles reachable from the source. If a negative cycle is reachable, then the distance to the goal can be made arbitrarily small, so we return -2 (Never). If the goal is unreachable, return -1 (Impossible). Otherwise return the shortest distance. Edge cases: the start or goal may be blocked (then impossible). Self-loop or duplicate destinations are allowed. Negative portal times can create negative cycles. Time complexity: O(V*E) where V=w*h ≤ 2500 and E ≤ 4*V + number_of_portals + V (since each cell has at most 4 edges, plus portals); in worst case ~10,000 edges, so Bellman-Ford runs in ~25 million operations, acceptable. The space complexity is O(V+E).
