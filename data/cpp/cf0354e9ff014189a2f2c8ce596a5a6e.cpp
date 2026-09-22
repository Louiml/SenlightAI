Write a standalone C++ function that computes a navigation path from a start cell to a goal cell on a 2D grid with obstacle costs, using a simplified Dijkstra-style wavefront propagation (without priority queue heuristics or A*). The function should take a 1D costmap (row-major order, with `COST_OBS = 255` for obstacles and values 0–254 for traversable cells, where lower is cheaper), grid dimensions `nx` and `ny`, start coordinates `(sx, sy)`, goal coordinates `(gx, gy)`, and an output buffer `plan` of floats (pairs `x,y`) with a max length `nplan`. It should return the number of points in the found path (or 0 if no path exists). The path should be computed by first performing a Dijkstra-like potential propagation from the goal (using the provided update rule: `pot = ta + hf` or quadratic interpolation based on two lowest neighbors), then tracing from the start down the potential gradient using the given `calcPath` logic (including oscillation detection, boundary handling, and interpolation). The solution must handle obstacles (cells with cost ≥ `COST_OBS` are not traversable), ensure the start and goal are valid, and return a path as a sequence of integer grid coordinates (no sub-pixel interpolation needed — output integer indices as floats). The function should be self-contained, not depend on ROS, and use only standard C++ libraries.

#include <cassert>
#include <vector>

int main() {
    // Simple 5x5 grid, all traversable (cost 50), start (0,0), goal (4,4)
    std::vector<int> costmap1(25, 50);
    std::vector<float> plan1;
    int len1 = computeNavPath(costmap1, 5, 5, 0, 0, 4, 4, plan1, 100);
    assert(len1 > 0);
    // Path should reach goal exactly at end
    assert(plan1[len1*2-2] == 4.0f && plan1[len1*2-1] == 4.0f);
    // Path should not contain obstacles (none here)

    // Grid with a wall (obstacle) blocking path
    std::vector<int> costmap2(25, 50);
    for (int i = 0; i < 5; ++i) costmap2[i*5 + 2] = 255;  // vertical wall at x=2
    std::vector<float> plan2;
    int len2 = computeNavPath(costmap2, 5, 5, 0, 0, 4, 4, plan2, 100);
    // There is a path around the wall (e.g., go to x=3 then down)
    assert(len2 > 0);
    assert(plan2[len2*2-2] == 4.0f && plan2[len2*2-1] == 4.0f);

    // Fully blocked (start enclosed by obstacles)
    std::vector<int> costmap3(25, 50);
    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            if (x == 0 && y == 0) continue;
            costmap3[y*5+x] = 255;
        }
    }
    // start (0,0) alone, goal (4,4) obstacle -> no path
    std::vector<float> plan3;
    int len3 = computeNavPath(costmap3, 5, 5, 0, 0, 4, 4, plan3, 100);
    assert(len3 == 0);

    // Start equals goal
    std::vector<int> costmap4(25, 50);
    std::vector<float> plan4;
    int len4 = computeNavPath(costmap4, 5, 5, 2, 2, 2, 2, plan4, 100);
    assert(len4 == 1);
    assert(plan4[0] == 2.0f && plan4[1] == 2.0f);

    // Start or goal on obstacle
    std::vector<int> costmap5(25, 50);
    costmap5[0] = 255;  // start (0,0) obstacle
    std::vector<float> plan5;
    int len5 = computeNavPath(costmap5, 5, 5, 0, 0, 4, 4, plan5, 100);
    assert(len5 == 0);

    // 1x1 grid
    std::vector<int> costmap6 = {50};
    std::vector<float> plan6;
    int len6 = computeNavPath(costmap6, 1, 1, 0, 0, 0, 0, plan6, 10);
    assert(len6 == 1);
    assert(plan6[0] == 0.0f && plan6[1] == 0.0f);

    return 0;
}

#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <limits>

// Constants matching the provided code
const int COST_OBS = 255;               // obstacle cost
const int COST_NEUTRAL = 50;            // neutral traversable cost
const float POT_HIGH = std::numeric_limits<float>::max() / 2.0f;

// Simplified nav function: Dijkstra wavefront propagation + gradient descent path
// Returns number of points written to plan (pairs x,y as floats), or 0 if no path.
int computeNavPath(const std::vector<int>& costmap, int nx, int ny,
                   int startX, int startY, int goalX, int goalY,
                   std::vector<float>& plan, int max_plan_size) {
    int ns = nx * ny;
    if (startX < 0 || startX >= nx || startY < 0 || startY >= ny ||
        goalX < 0 || goalX >= nx || goalY < 0 || goalY >= ny) return 0;

    int startIdx = startY * nx + startX;
    int goalIdx = goalY * nx + goalX;
    if (costmap[startIdx] >= COST_OBS || costmap[goalIdx] >= COST_OBS) return 0;

    // (1) Dijkstra-style potential propagation from goal
    std::vector<float> pot(ns, POT_HIGH);
    pot[goalIdx] = 0.0f;
    // Min-heap: pair (potential, index)
    std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<>> pq;
    pq.push({0.0f, goalIdx});

    // Direction offsets for 4-connected neighbors
    const int dx4[4] = {1, -1, 0, 0};
    const int dy4[4] = {0, 0, 1, -1};

    while (!pq.empty()) {
        auto [curPot, curIdx] = pq.top();
        pq.pop();
        if (curPot > pot[curIdx]) continue;  // stale entry

        int cx = curIdx % nx;
        int cy = curIdx / nx;

        for (int dir = 0; dir < 4; ++dir) {
            int nx2 = cx + dx4[dir];
            int ny2 = cy + dy4[dir];
            if (nx2 < 0 || nx2 >= nx || ny2 < 0 || ny2 >= ny) continue;
            int nIdx = ny2 * nx + nx2;
            if (costmap[nIdx] >= COST_OBS) continue;  // obstacle

            // Find current cell's potential and the neighbor's potential (before update)
            float cur_p = pot[curIdx];
            float neighbor_p = pot[nIdx];
            // For updateCell, need two lowest neighbors of the target (nIdx).
            // We'll compute new potential for nIdx using its own neighbors.
            // Gather up, down, left, right potentials of nIdx
            auto getNeighbor = [&](int dx, int dy) -> float {
                int ax = nx2 + dx;
                int ay = ny2 + dy;
                if (ax < 0 || ax >= nx || ay < 0 || ay >= ny) return POT_HIGH;
                int idx = ay * nx + ax;
                if (costmap[idx] >= COST_OBS) return POT_HIGH;
                return pot[idx];
            };
            float l = getNeighbor(-1, 0);
            float r = getNeighbor(1, 0);
            float u = getNeighbor(0, -1);
            float d = getNeighbor(0, 1);

            // Find two lowest among l,r,u,d (excluding POT_HIGH)
            float ta, tc;
            if (l < r) tc = l; else tc = r;
            if (u < d) ta = u; else ta = d;
            // Note: if both are POT_HIGH, then no feasible neighbor; skip

            float hf = static_cast<float>(costmap[nIdx]);
            float dc = tc - ta;
            if (dc < 0) { dc = -dc; ta = tc; }

            float new_pot;
            if (dc >= hf) {
                new_pot = ta + hf;
            } else {
                float ratio = dc / hf;
                float v = -0.2301f * ratio * ratio + 0.5307f * ratio + 0.7040f;
                new_pot = ta + hf * v;
            }

            if (new_pot < pot[nIdx]) {
                pot[nIdx] = new_pot;
                pq.push({new_pot, nIdx});
            }
        }
    }

    if (pot[startIdx] >= POT_HIGH) return 0;  // goal unreachable

    // (2) Trace path from start using gradient descent
    // We'll use integer coordinates and move to lowest-potential neighbor (8-connected)
    std::vector<float> pathX, pathY;
    pathX.reserve(max_plan_size);
    pathY.reserve(max_plan_size);

    int curX = startX, curY = startY;
    int curIdx = startIdx;
    int npath = 0;
    const int max_steps = max_plan_size;

    // Directions: 8-connected
    const int dx8[8] = {1, -1, 0, 0, 1, 1, -1, -1};
    const int dy8[8] = {0, 0, 1, -1, 1, -1, 1, -1};

    // To avoid infinite loops, track visited (or allow oscillation detection)
    std::vector<int> last2x(2, -1), last2y(2, -1);

    while (npath < max_steps) {
        // Check if near goal: if current potential below neutral, we are done
        if (pot[curIdx] < COST_NEUTRAL) {
            pathX.push_back(static_cast<float>(goalX));
            pathY.push_back(static_cast<float>(goalY));
            npath++;
            break;
        }

        // Record current position
        pathX.push_back(static_cast<float>(curX));
        pathY.push_back(static_cast<float>(curY));
        npath++;

        // Oscillation detection: if we revisited same (x,y) two steps ago
        if (npath > 2 && pathX[npath-1] == pathX[npath-3] && pathY[npath-1] == pathY[npath-3]) {
            // Force move to neighbor with lowest potential (excluding current)
            float bestPot = POT_HIGH;
            int bestIdx = -1;
            for (int d = 0; d < 8; ++d) {
                int nx2 = curX + dx8[d];
                int ny2 = curY + dy8[d];
                if (nx2 < 0 || nx2 >= nx || ny2 < 0 || ny2 >= ny) continue;
                int nIdx = ny2 * nx + nx2;
                if (costmap[nIdx] >= COST_OBS) continue;
                if (pot[nIdx] < bestPot) {
                    bestPot = pot[nIdx];
                    bestIdx = nIdx;
                }
            }
            if (bestIdx < 0 || bestPot >= POT_HIGH) {
                // stuck
                return 0;
            }
            curIdx = bestIdx;
            curX = curIdx % nx;
            curY = curIdx / nx;
            // We'll still continue; but to avoid infinite loop, maybe add a step counter
            // We'll rely on max_steps.
        } else {
            // Normal step: find neighbor with lowest potential (including current to avoid going up)
            float bestPot = pot[curIdx];
            int bestIdx = curIdx;
            for (int d = 0; d < 8; ++d) {
                int nx2 = curX + dx8[d];
                int ny2 = curY + dy8[d];
                if (nx2 < 0 || nx2 >= nx || ny2 < 0 || ny2 >= ny) continue;
                int nIdx = ny2 * nx + nx2;
                if (costmap[nIdx] >= COST_OBS) continue;
                if (pot[nIdx] < bestPot) {
                    bestPot = pot[nIdx];
                    bestIdx = nIdx;
                }
            }
            if (bestIdx == curIdx) {
                // no better neighbor
                // Try forcing to go to any lower potential neighbor including diagonals; if none, fail
                // But we already scanned; if bestIdx == curIdx, we are at local min but not goal (potential >= COST_NEUTRAL)
                // Attempt to move to neighbor with same potential? Not advisable.
                // Return failure? Or just stop.
                return 0;
            }
            curIdx = bestIdx;
            curX = curIdx % nx;
            curY = curIdx / nx;
        }

        // Safety: if we've exceeded max steps, fail
        if (npath >= max_steps) {
            return 0;
        }
    }

    // Copy to output plan
    plan.clear();
    int len = std::min(npath, max_plan_size);
    for (int i = 0; i < len; ++i) {
        plan.push_back(pathX[i]);
        plan.push_back(pathY[i]);
    }
    return len;
}

// The core problem is to compute a shortest-cost path on a 4-connected grid (no diagonal moves) where each cell has a traversal cost (costmap values). We implement a Dijkstra-style wavefront propagation: initialize the potential array to a large value (`POT_HIGH`), set the goal cell potential to 0, and then iteratively relax neighbors using the exact update rule from the provided code. The update rule, known as the "fast marching" or "optimal path" update, computes the new potential for a cell from its two lowest neighbors: if the cost difference between the two is large, use the single-neighbor update `ta + hf`; otherwise use a quadratic interpolation approximation `ta + hf * (-0.2301*(dc/hf)^2 + 0.5307*(dc/hf) + 0.7040)`. This ensures the potential approximates the true cost-to-go distance (with a small overestimation due to the quadratic approximation). To handle large grids efficiently, we can use a simple priority queue (min-heap) keyed on potential, which gives the same results as the buffered block approach but is simpler to implement. After propagation, we trace a path from start to goal: at each step, we examine the eight neighbors (including diagonals, though propagation was 4-connected) and move to the neighbor with the lowest potential, but we must avoid cells with `POT_HIGH` (unreached). This greedy gradient descent may get stuck in local minima or oscillate; the trace includes oscillation detection (if we revisit the same point after two steps) and boundary checks. The trace terminates when we reach the goal cell (potential < `COST_NEUTRAL`) or after a maximum number of steps. The output path is a sequence of grid cell indices (floats). Edge cases: obstacle boundaries (ensure we never move into a cell with cost ≥ `COST_OBS`), start or goal inside an obstacle (return 0), unreachable goal (return 0), and degenerate grids (1x1). Time complexity: propagation is O(ns log ns) with a heap (or O(ns) with the buffered approach), where ns = nx*ny; path tracing is O(path length), typically much less than ns. Space: O(ns) for potential and cost arrays.
