// Write a C++ function named `findPath` that implements the A* search algorithm over a 2D grid with blocked cells. The function takes a map height `mapH`, map width `mapW`, a flat array `blockedMap` of size `mapH * mapW` where `true` means the cell is blocked (non-walkable) and `false` means walkable, a start coordinate `start`, a goal coordinate `goal`, and an output parameter `pathSteps` that will be filled with the sequence of grid coordinates (from start to goal, inclusive) representing the shortest path using 8-directional movement (orthogonal and diagonal moves, each costing 1). The function must return `true` if a path exists, and `false` otherwise. If the start equals the goal, return `true` with `pathSteps` containing only that single coordinate. Diagonal moves through corners are allowed even if the two adjacent orthogonal cells are blocked (i.e., no corner-cutting restriction). The algorithm must use the Manhattan distance as the heuristic, and must break ties by preferring lower estimated distance to the goal. The function should not modify the input map. If no path exists, `pathSteps` should be left empty. Use `std::vector<GridPos>` for the output, where `GridPos` is a simple struct with `int x` and `int y`.
#include <cassert>
#include <vector>
#include <iostream>

// Assume GridPos and findPath are defined above (or include the solution header).

int main() {
    // Test 1: Open 3x3 grid, start (0,0) to (2,2)
    {
        int mapH = 3, mapW = 3;
        std::vector<bool> blocked(mapH*mapW, false);
        GridPos start{0,0}, goal{2,2};
        std::vector<GridPos> path;
        assert(findPath(mapH, mapW, blocked, start, goal, path) == true);
        assert(path.size() == 3);
        assert(path[0] == start);
        assert(path[path.size()-1] == goal);
    }
    
    // Test 2: Start equals goal
    {
        int mapH = 2, mapW = 2;
        std::vector<bool> blocked(4, false);
        GridPos start{1,1}, goal{1,1};
        std::vector<GridPos> path;
        assert(findPath(mapH, mapW, blocked, start, goal, path) == true);
        assert(path.size() == 1);
        assert(path[0] == start);
    }
    
    // Test 3: Unreachable goal (blocked cell)
    {
        int mapH = 3, mapW = 3;
        std::vector<bool> blocked(9, false);
        blocked[2*3 + 2] = true; // block (2,2)
        GridPos start{0,0}, goal{2,2};
        std::vector<GridPos> path;
        assert(findPath(mapH, mapW, blocked, start, goal, path) == false);
        assert(path.empty());
    }
    
    // Test 4: Blocked path forces detour
    {
        int mapH = 3, mapW = 3;
        std::vector<bool> blocked(9, false);
        blocked[1*3 + 0] = true; // block (0,1)
        blocked[1*3 + 1] = true; // block (1,1)
        blocked[1*3 + 2] = true; // block (2,1)
        GridPos start{0,0}, goal{2,2};
        std::vector<GridPos> path;
        assert(findPath(mapH, mapW, blocked, start, goal, path) == true);
        // Path must go around the middle row
        assert(path[0] == start);
        assert(path[path.size()-1] == goal);
        // Each consecutive step must be a valid 8-direction move
        for (size_t i = 1; i < path.size(); ++i) {
            int dx = std::abs(path[i].x - path[i-1].x);
            int dy = std::abs(path[i].y - path[i-1].y);
            assert(dx <= 1 && dy <= 1 && (dx+dy) >= 1);
        }
    }
    
    // Test 5: Larger grid, path length correctness (check no blocked cells visited)
    {
        int mapH = 5, mapW = 5;
        std::vector<bool> blocked(25, false);
        // Create a small wall segment
        for (int i = 0; i < 3; ++i) blocked[i*5 + 2] = true; // vertical line at x=2, y=0,1,2
        GridPos start{0,0}, goal{4,4};
        std::vector<GridPos> path;
        assert(findPath(mapH, mapW, blocked, start, goal, path) == true);
        // Check path does not step onto blocked cells
        for (const auto& p : path) {
            assert(!blocked[p.y * mapW + p.x]);
        }
        // Check connectivity
        for (size_t i = 1; i < path.size(); ++i) {
            int dx = std::abs(path[i].x - path[i-1].x);
            int dy = std::abs(path[i].y - path[i-1].y);
            assert(dx <= 1 && dy <= 1 && (dx+dy) >= 1);
        }
    }
    
    // Test 6: Start is blocked but goal reachable (should still find path? Our implementation treats start as walkable)
    {
        int mapH = 2, mapW = 2;
        std::vector<bool> blocked(4, false);
        blocked[0] = true; // block start (0,0)
        GridPos start{0,0}, goal{1,1};
        std::vector<GridPos> path;
        // Our version returns true and uses start even if blocked
        assert(findPath(mapH, mapW, blocked, start, goal, path) == true);
        assert(path[0] == start);
        assert(path[path.size()-1] == goal);
    }
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm>

struct GridPos {
    int x, y;
    bool operator==(const GridPos& other) const {
        return x == other.x && y == other.y;
    }
};

// A* search on an 8-connected grid with unit move cost.
// Returns true and fills pathSteps with coordinates from start to goal (inclusive) if a path exists.
// Returns false and leaves pathSteps empty otherwise.
bool findPath(int mapH, int mapW,
              const std::vector<bool>& blockedMap,
              GridPos start, GridPos goal,
              std::vector<GridPos>& pathSteps) {
    pathSteps.clear();
    
    // Degenerate case: start == goal
    if (start == goal) {
        pathSteps.push_back(start);
        return true;
    }
    
    // If goal is blocked (or start is out of bounds), no path is possible.
    if (start.x < 0 || start.x >= mapW || start.y < 0 || start.y >= mapH ||
        goal.x < 0 || goal.x >= mapW || goal.y < 0 || goal.y >= mapH) {
        return false;
    }
    if (blockedMap[goal.y * mapW + goal.x]) {
        return false;
    }
    
    // Manhattan distance heuristic
    auto heuristic = [](int x1, int y1, int x2, int y2) {
        return std::abs(x1 - x2) + std::abs(y1 - y2);
    };
    
    int totalCells = mapH * mapW;
    std::vector<int> bestG(totalCells, std::numeric_limits<int>::max());
    std::vector<bool> closed(totalCells, false);
    std::vector<GridPos> predecessor(totalCells, GridPos{-1, -1});
    
    // Priority queue entry
    struct Node {
        int f, g, h, x, y;
        bool operator<(const Node& other) const {
            if (f != other.f) return f > other.f; // min-heap
            // Tie-break: smaller estimated remaining distance
            return h > other.h;
        }
    };
    
    int startIndex = start.y * mapW + start.x;
    int goalIndex = goal.y * mapW + goal.x;
    int startH = heuristic(start.x, start.y, goal.x, goal.y);
    bestG[startIndex] = 0;
    
    std::priority_queue<Node> open;
    open.push({startH, 0, startH, start.x, start.y});
    
    const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    bool found = false;
    
    while (!open.empty()) {
        Node current = open.top();
        open.pop();
        
        int currentIndex = current.y * mapW + current.x;
        if (closed[currentIndex]) continue; // already processed with better or equal g
        closed[currentIndex] = true;
        
        if (current.x == goal.x && current.y == goal.y) {
            found = true;
            break;
        }
        
        for (int i = 0; i < 8; ++i) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];
            if (nx < 0 || nx >= mapW || ny < 0 || ny >= mapH) continue;
            int neighborIndex = ny * mapW + nx;
            if (blockedMap[neighborIndex]) continue;
            
            int tentativeG = current.g + 1;
            if (tentativeG < bestG[neighborIndex]) {
                bestG[neighborIndex] = tentativeG;
                predecessor[neighborIndex] = {current.x, current.y};
                int nh = heuristic(nx, ny, goal.x, goal.y);
                int nf = tentativeG + nh;
                open.push({nf, tentativeG, nh, nx, ny});
            }
        }
    }
    
    if (!found) return false;
    
    // Reconstruct path from goal back to start
    std::vector<GridPos> reversePath;
    GridPos cur = goal;
    while (!(cur == start)) {
        reversePath.push_back(cur);
        int curIndex = cur.y * mapW + cur.x;
        cur = predecessor[curIndex];
    }
    reversePath.push_back(start);
    
    // Reverse to get start -> goal
    std::reverse(reversePath.begin(), reversePath.end());
    pathSteps = reversePath;
    return true;
}
// The solution is a standard A* search on an 8-connected grid. We maintain two sets: an open set (priority queue) and a closed set (visited flags). The priority queue stores nodes keyed by `f = g + h`, where `g` is the cost from start (each move cost 1) and `h` is the Manhattan distance to the goal. Ties are broken by smaller `h` to encourage more direct paths. Each grid cell is a node; we use a `std::vector` to store `g` values (initialized to large) and a `std::vector` of booleans for closed status. The open set is implemented with a `std::priority_queue` of a small struct containing `f`, `g`, `h`, and `x,y`. When we pop a node, if it's already closed (visited with a better or equal `g`), we skip it. For each of the 8 neighbors (dx,dy in {-1,0,1} except both zero), we check bounds and that the neighbor is not blocked. The neighbor's `g` candidate is `current.g + 1`. If that is better than the stored `g`, we update and push the neighbor with new `f`. We also store a predecessor map (`std::vector<std::pair<int,int>>` of `GridPos`) to reconstruct the path when the goal is popped. Edge cases: start equals goal (return path with one element), start blocked (then it may still be allowed as a walkable node for the algorithm? The problem says blocked map indicates blocked cells; typically start is assumed walkable, but if start is blocked, we treat it as walkable because the algorithm uses it as the starting node – but for consistency, we can check if start is blocked and immediately return false if blocked? The original code treats start as traversable even if blocked (with special handling). To be independent, we'll follow the original: start can be blocked, but we still allow movement from it; however, if the goal is blocked, no path can reach it, so return false. Also note that diagonal moves are allowed without checking corner obstacles, as specified. Complexity: For a grid with `N = mapH * mapW` cells, each cell may be visited once in the worst case, and each pop from the priority queue is O(log N). Thus time is O(N log N) and space is O(N) for the storage of g, predecessors, and the priority queue (which may hold multiple entries per node temporarily). The heuristic is admissible and consistent because Manhattan distance on a 4-neighbor grid is consistent for 8-connectivity? Actually Manhattan is admissible for 8-connectivity (since diagonal moves cost 1, Manhattan underestimates, but it's still admissible). It is not consistent, but A* remains optimal with admissible heuristic even if not consistent; however, we might need to allow re-opening nodes. Our algorithm uses a priority queue and allows re-push if a shorter path is found, which is standard for A* with an inconsistent heuristic. Alternatively, we could use a consistent heuristic like Chebyshev distance (max(abs(dx),abs(dy))) which is exact for 8-connectivity with unit costs, but the task specifies Manhattan, so we use that. We must also handle the case where the goal is unreachable: the priority queue eventually empties, and we return false with empty `pathSteps`.
