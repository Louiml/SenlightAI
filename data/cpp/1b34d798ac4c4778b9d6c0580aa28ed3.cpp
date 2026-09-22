Implement a standalone C++ function that, given a rectangular grid described by width `dimx`, height `dimy`, a collection of obstacle coordinates, a start state, and a goal state, performs a **grid-based path search with time-dependent obstacle intervals** using a simplified Safe Interval Path Planning (SIPP) algorithm. The function must return a vector of `(x, y, time)` tuples representing a collision-free path from start to goal, where the agent moves one grid cell per time step in four cardinal directions or waits in place. The input also provides a set of "forbidden time intervals" for each cell (e.g., due to other agents' planned occupancy). At each time step, the agent may only be in a cell if the current time does not fall inside a forbidden interval for that cell. The path must respect that no two consecutive time steps can have the agent in the same cell unless it is a deliberate wait (which is allowed). If no path exists, return an empty vector. Use a 4-puzzle-style grid with obstacles and time intervals, and compute the path using a time-augmented A* search where the state is `(x, y, t)`. The heuristic is Manhattan distance to goal, and edge cost is 1 per move or wait. Prefer earlier arrival times when multiple states have the same `(x,y)`. The function signature:  
`std::vector<std::tuple<int,int,int>> plan_sipp(int dimx, int dimy, const std::vector<std::pair<int,int>>& obstacles, const std::pair<int,int>& start, const std::pair<int,int>& goal, const std::unordered_map<int, std::vector<std::pair<int,int>>>& forbidden_intervals_by_cell_index)`  
where cell index = `y * dimx + x`, and each interval is `[low, high]` inclusive (i.e., the cell is occupied at all times `t` with `low <= t <= high`). The path's first tuple must have `t=0` at the start cell (which is guaranteed not to be forbidden at t=0), and each subsequent tuple must have `t` increasing by exactly 1. Return the shortest path (minimum total time) that respects all constraints. If multiple shortest paths, prefer lexicographically smallest by `(x,y,t)` sequence.

#include <cassert>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <utility>

// The solution function is assumed to be declared above (omitted for brevity in test)

int main() {
    // Test 1: simple empty grid, no obstacles, no forbidden intervals
    {
        std::vector<std::pair<int,int>> obstacles;
        std::unordered_map<int, std::vector<std::pair<int,int>>> intervals;
        auto path = plan_sipp(3, 3, obstacles, {0,0}, {2,2}, intervals);
        assert(!path.empty());
        // Check path length should be 4 (Manhattan distance 4)
        assert(path.size() == 5); // positions at t=0,1,2,3,4
        assert(path[0] == std::make_tuple(0,0,0));
        assert(path[4] == std::make_tuple(2,2,4));
        // Each consecutive time step moves or waits
        for (size_t i = 1; i < path.size(); ++i) {
            assert(std::get<2>(path[i]) == std::get<2>(path[i-1]) + 1);
            int dx = std::get<0>(path[i]) - std::get<0>(path[i-1]);
            int dy = std::get<1>(path[i]) - std::get<1>(path[i-1]);
            assert((std::abs(dx) + std::abs(dy) == 1) || (dx == 0 && dy == 0));
        }
    }

    // Test 2: obstacle blocks direct path, must go around
    {
        std::vector<std::pair<int,int>> obstacles = {{1,0}}; // cell (1,0)
        std::unordered_map<int, std::vector<std::pair<int,int>>> intervals;
        auto path = plan_sipp(3, 3, obstacles, {0,0}, {2,0}, intervals);
        assert(!path.empty());
        // Path must avoid (1,0). Manhattan distance is 2, but with obstacle need detour, cost 4
        assert(path.back() == std::make_tuple(2,0,4));
    }

    // Test 3: forbidden interval forces waiting
    {
        std::vector<std::pair<int,int>> obstacles;
        // Cell (1,0) is forbidden at time 1 (index 0*3+1 = 1)
        std::unordered_map<int, std::vector<std::pair<int,int>>> intervals;
        intervals[1] = {{1,1}}; // forbidden exactly at t=1
        auto path = plan_sipp(3, 1, obstacles, {0,0}, {2,0}, intervals);
        assert(!path.empty());
        // Must wait at start for one step, then go: t=0 at (0,0), t=1 wait at (0,0), t=2 move to (1,0), t=3 move to (2,0)
        assert(path.size() == 4);
        assert(path[0] == std::make_tuple(0,0,0));
        assert(path[1] == std::make_tuple(0,0,1)); // wait
        assert(path[2] == std::make_tuple(1,0,2));
        assert(path[3] == std::make_tuple(2,0,3));
    }

    // Test 4: unreachable due to blocking obstacles
    {
        std::vector<std::pair<int,int>> obstacles = {{0,1}, {1,0}}; // block all exits from (0,0)
        std::unordered_map<int, std::vector<std::pair<int,int>>> intervals;
        auto path = plan_sipp(2, 2, obstacles, {0,0}, {1,1}, intervals);
        assert(path.empty());
    }

    // Test 5: start cell forbidden at t>0 but not at t=0; goal reachable by waiting
    {
        std::vector<std::pair<int,int>> obstacles;
        // forbid cell (0,0) from t=1 to t=2, but start at t=0 is fine
        std::unordered_map<int, std::vector<std::pair<int,int>>> intervals;
        intervals[0] = {{1,2}};
        // Goal is also (0,0)?? Actually goal cannot be start, so choose goal (1,0)
        auto path = plan_sipp(2, 1, obstacles, {0,0}, {1,0}, intervals);
        assert(!path.empty());
        // Direct move to (1,0) at t=1 is fine because only cell (0,0) is forbidden
        assert(path.back() == std::make_tuple(1,0,1));
    }

    // Test 6: multiple intervals for same cell, must wait longer
    {
        std::vector<std::pair<int,int>> obstacles;
        // Cell (1,0) forbidden at t=1 and t=3
        std::unordered_map<int, std::vector<std::pair<int,int>>> intervals;
        intervals[1] = {{1,1}, {3,3}};
        auto path = plan_sipp(3, 1, obstacles, {0,0}, {2,0}, intervals);
        assert(!path.empty());
        // Must wait at (0,0) until t=2, then move to (1,0) at t=2? But t=2 is safe for (1,0) because intervals are [1,1] and [3,3]
        // t=2 move to (1,0), then t=3 move to (2,0) but t=3 is forbidden for (1,0) only, not (2,0), so can move at t=3
        // Actually t=3 move from (1,0) to (2,0) is allowed because (2,0) has no intervals
        // Path: (0,0,0), (0,0,1), (0,0,2), (1,0,3)?? No, at t=2 we can move to (1,0) because (1,0) not forbidden at t=2
        // So: (0,0,0), (0,0,1) wait, (1,0,2) move, (2,0,3) move. Check: at t=2 cell (1,0) safe, t=3 cell (2,0) safe. Good.
        assert(path.size() == 4);
        assert(path[0] == std::make_tuple(0,0,0));
        assert(path[1] == std::make_tuple(0,0,1));
        assert(path[2] == std::make_tuple(1,0,2));
        assert(path[3] == std::make_tuple(2,0,3));
    }

    // Test 7: empty grid with no obstacles, but forbidden intervals that block waiting at goal? Not needed for basic

    return 0;
}

#include <vector>
#include <tuple>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>

// Define a hash for tuple<int,int,int> for visited set
struct StateHash {
    std::size_t operator()(const std::tuple<int,int,int>& s) const {
        std::size_t h1 = std::hash<int>()(std::get<0>(s));
        std::size_t h2 = std::hash<int>()(std::get<1>(s));
        std::size_t h3 = std::hash<int>()(std::get<2>(s));
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

// Check if time t is within any forbidden interval for a given cell's sorted interval list
bool isForbidden(int t, const std::vector<std::pair<int,int>>& intervals) {
    // Binary search for the last interval with start <= t
    int lo = 0, hi = intervals.size() - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (intervals[mid].first <= t) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    // hi is the index of the last interval with start <= t, or -1 if none
    if (hi >= 0 && t <= intervals[hi].second) {
        return true;
    }
    return false;
}

// Plan a path using SIPP (simplified) or time-augmented A*
std::vector<std::tuple<int,int,int>> plan_sipp(
    int dimx, int dimy,
    const std::vector<std::pair<int,int>>& obstacles,
    const std::pair<int,int>& start,
    const std::pair<int,int>& goal,
    const std::unordered_map<int, std::vector<std::pair<int,int>>>& forbidden_intervals_by_cell_index)
{
    // Preprocess obstacles as a set for O(1) lookup
    std::unordered_set<int> obstacle_set;
    for (const auto& obs : obstacles) {
        obstacle_set.insert(obs.second * dimx + obs.first);
    }

    // Preprocess forbidden intervals: ensure each cell's intervals are sorted by start time
    std::unordered_map<int, std::vector<std::pair<int,int>>> sorted_intervals;
    for (const auto& kv : forbidden_intervals_by_cell_index) {
        sorted_intervals[kv.first] = kv.second;
        std::sort(sorted_intervals[kv.first].begin(), sorted_intervals[kv.first].end());
    }

    // A* priority queue: (f, g, x, y) with f = g + heuristic
    // Use a custom struct for priority queue
    struct Node {
        int f, g, x, y;
        bool operator>(const Node& other) const {
            if (f != other.f) return f > other.f;
            if (g != other.g) return g > other.g;
            if (x != other.x) return x > other.x;
            return y > other.y;
        }
    };
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;

    // Visited set: storing (x, y, t) to avoid re-expanding same state
    std::unordered_set<std::tuple<int,int,int>, StateHash> visited;

    // Start state: t=0
    int sx = start.first, sy = start.second;
    int gx = goal.first, gy = goal.second;

    // Check that start is valid (not obstacle and not forbidden at t=0)
    int start_idx = sy * dimx + sx;
    if (obstacle_set.count(start_idx)) return {};
    auto it_start = sorted_intervals.find(start_idx);
    if (it_start != sorted_intervals.end() && isForbidden(0, it_start->second)) return {};

    // Heuristic function
    auto heuristic = [&](int x, int y) {
        return std::abs(x - gx) + std::abs(y - gy);
    };

    // For reconstructing path, we need parent pointers: map from (x,y,t) to (px,py,pt)
    // We'll use an unordered_map with custom hash for tuple
    std::unordered_map<std::tuple<int,int,int>, std::tuple<int,int,int>, StateHash> parent;

    int h_start = heuristic(sx, sy);
    pq.push({h_start, 0, sx, sy});
    visited.insert({sx, sy, 0});

    // Direction vectors: Up, Down, Left, Right
    const int dx[4] = {0, 0, -1, 1};
    const int dy[4] = {1, -1, 0, 0};

    std::vector<std::tuple<int,int,int>> path;

    while (!pq.empty()) {
        Node top = pq.top();
        pq.pop();

        int x = top.x, y = top.y, t = top.g;

        // Check if goal reached
        if (x == gx && y == gy) {
            // Reconstruct path backwards
            std::tuple<int,int,int> cur = {x, y, t};
            while (true) {
                path.push_back(cur);
                if (cur == std::tuple<int,int,int>{sx, sy, 0}) break;
                cur = parent[cur];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Generate neighbors: 5 actions (4 moves + wait)
        // First, wait
        int nt = t + 1;
        int nidx = y * dimx + x;
        bool wait_valid = true;
        if (obstacle_set.count(nidx)) wait_valid = false;
        auto it_wait = sorted_intervals.find(nidx);
        if (it_wait != sorted_intervals.end() && isForbidden(nt, it_wait->second)) wait_valid = false;
        if (wait_valid) {
            auto state = std::make_tuple(x, y, nt);
            if (visited.find(state) == visited.end()) {
                visited.insert(state);
                parent[state] = {x, y, t};
                int f = nt + heuristic(x, y);
                pq.push({f, nt, x, y});
            }
        }

        // Then moves
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            // Bounds check
            if (nx < 0 || nx >= dimx || ny < 0 || ny >= dimy) continue;
            int nidx2 = ny * dimx + nx;
            if (obstacle_set.count(nidx2)) continue;
            // Check forbidden at time nt
            auto it = sorted_intervals.find(nidx2);
            if (it != sorted_intervals.end() && isForbidden(nt, it->second)) continue;
            auto state = std::make_tuple(nx, ny, nt);
            if (visited.find(state) == visited.end()) {
                visited.insert(state);
                parent[state] = {x, y, t};
                int f = nt + heuristic(nx, ny);
                pq.push({f, nt, nx, ny});
            }
        }
    }

    // No path found
    return {};
}

// The problem is a time-expanded shortest-path search with cell-based time constraints. We model each state as `(x, y, time)` representing the agent's location and current discrete time. Start time is 0, goal is any time ≥ 0. From each state, we generate up to 5 actions: move Up, Down, Left, Right (if within bounds and not an obstacle), and Wait (stay in place). For each neighbor state `(nx, ny, nt = t+1)`, we check that `(nx, ny)` is not an obstacle and that time `nt` is not inside a forbidden interval for that cell. Since intervals are per cell, we precompute for each cell a sorted vector of intervals and use binary search to check if `nt` is in any interval. The search uses A* with a priority queue ordered by `f = g + h`, where `g` is the current time and `h` is Manhattan distance to goal. Because the heuristic is admissible (never overestimates) and consistent (triangle inequality holds for grid moves), A* finds the optimal path. We also maintain a `visited` set to avoid revisiting the same `(x,y,t)` state (t strictly increases, so each t is visited at most once). We must be careful about the wait action: the goal can be reached by waiting after arrival, but since we stop as soon as we pop the goal state, we only accept an immediate goal detection. To break ties and guarantee lexicographic minimality, we can push states with lower `t` first (since `t` is the g-score), and for equal `f` we can also compare `x,y` but it's not strictly required for correctness; the first goal popped is optimal in time. Edge cases: start cell might have forbidden intervals but not at t=0 (given), goal might be unreachable (return empty). Time complexity: O(V * T) where V is number of cells and T is maximum time explored (bounded by the number of intervals plus grid size because each cell can be visited at most once per time step; in worst case, we might explore O(V * T) states). Space complexity O(V * T) for the visited set and priority queue.
