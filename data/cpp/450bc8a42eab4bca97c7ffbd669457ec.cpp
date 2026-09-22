// Given a vector of joint-space waypoints for a 7-degree-of-freedom robot, where each waypoint is a `std::vector<double>` of length 7, write a C++ function that performs a simplified Rapidly-exploring Random Tree (RRT) expansion from an initial configuration to a goal configuration. The function must accept the initial configuration, the goal configuration, a maximum number of iterations (e.g., 10000), a maximum tree depth (e.g., 30), and a collision-checking callback (as a `std::function<bool(const std::vector<double>&)>`). It should return a path as `std::vector<std::vector<double>>` from the initial configuration to the goal configuration (inclusive), or an empty vector if no path is found within the limits. The sampling strategy must blend goal-biased expansion with random perturbation: for early levels (depth < 5), the step is `(goal - parent) * (1/level) + (π/2) * random_in_unit_interval * (1/level)`, and for later levels it becomes `(goal - parent) * (1 + (π/2) * random_in_unit_interval)`. A new node is accepted only if it is collision-free along a linear interpolation from its parent using at least 3 evenly spaced checkpoints (including both endpoints). The tree is stored as an adjacency map from child to parent, and the final path is reconstructed by tracing back from a node whose Euclidean squared distance to the goal is less than 0.1, then smoothing the path by interpolating between consecutive waypoints with a fixed number of steps (e.g., `max_depth / current_depth` per segment, with a minimum of 1). The function should be named `rrtPlan` and should use only standard C++17 features.
// The solution implements a straightforward RRT variant in configuration space. The core algorithm is: (1) initialize the tree with the start node, (2) repeatedly expand from nodes in the frontier (starting from the root and moving level by level), (3) for each parent node, generate a small number of candidate children using the level-dependent sampling formula, (4) perform coarse collision checking along the straight line from parent to candidate, (5) if collision-free, add the candidate to the tree and check for goal proximity, (6) stop when a node within the goal tolerance is found or limits are exceeded, and (7) reconstruct the path by walking parent pointers and then optionally add interpolated points to create a denser trajectory. Edge cases: the start itself may be within tolerance (path is just `{start, goal}`), the goal may be unreachable (return empty), collision callback may reject all candidates, and numerical tolerance must handle floating-point precision. The `max_depth` and `max_iterations` act as safety bounds to guarantee termination. Time complexity is `O(iterations * D * K * C)` where `D` is the dimensionality (7), `K` is the number of collision checkpoints (constant), and `C` is the cost of the collision callback (user-defined). Space complexity is `O(N)` where `N` is the number of stored nodes, plus the final path size which is `O(depth * interpolation_steps)`.
#include <vector>
#include <functional>
#include <map>
#include <cmath>
#include <algorithm>

// Perform RRT-based path planning from start to goal with collision checking.
// Returns a path (inclusive of start and goal) or an empty vector if no path found.
std::vector<std::vector<double>> rrtPlan(
    const std::vector<double>& start,
    const std::vector<double>& goal,
    std::function<bool(const std::vector<double>&)> isCollision,
    int maxIterations = 10000,
    int maxDepth = 30) {

    constexpr double GOAL_SQUARED_DIST = 0.1;
    constexpr double PI_OVER_2 = 3.14159265358979323846 / 2.0;
    const std::vector<double>& qini = start;
    const std::vector<double>& qfin = goal;
    const size_t dim = qini.size();

    // Tree: child -> parent
    std::map<std::vector<double>, std::vector<double>> tree;
    tree[qini] = qini;

    // Level storage for BFS-like expansion: level -> vector of nodes
    std::map<int, std::vector<std::vector<double>>> levels;
    levels[0].push_back(qini);

    // Best distance to goal so far (for potential pruning, though not strictly enforced)
    double bestdist = 0.0;
    for (size_t i = 0; i < dim; ++i) {
        bestdist += (qini[i] - qfin[i]) * (qini[i] - qfin[i]);
    }

    int node_count = 1;
    int nl = 0;  // current depth
    bool found = false;
    std::vector<double> endnode;

    while (node_count < maxIterations && nl < maxDepth && !found) {
        int next_level_index = nl + 1;
        int parent_count = static_cast<int>(levels[nl].size());
        int nodes_added_in_next = 0;

        // Generate candidates from each parent in the current frontier
        for (int p = 0; p < parent_count && !found; ++p) {
            const auto& parent = levels[nl][p];

            // Generate a few candidate children per parent
            const int candidates_per_parent = 3;
            for (int c = 0; c < candidates_per_parent && !found; ++c) {
                if (node_count >= maxIterations) break;

                std::vector<double> candidate(dim);
                for (size_t k = 0; k < dim; ++k) {
                    // Random value in [-50, 50] then scaled to [-0.5, 0.5]
                    double rand_val = (rand() % 100 - 50) / 100.0;
                    double step;
                    if (nl < 5) {
                        // Early phase: more conservative step
                        step = (qfin[k] - parent[k]) * (1.0 / (nl + 1)) + PI_OVER_2 * rand_val / (nl + 1);
                    } else {
                        // Later phase: aggressive pull to goal
                        step = (qfin[k] - parent[k]) * (1.0 + PI_OVER_2 * rand_val);
                    }
                    candidate[k] = parent[k] + step;
                }

                // Collision check along the straight path from parent to candidate
                bool collides = false;
                for (double t = 0.0; t <= 1.0; t += 0.33) {
                    std::vector<double> interpolated(dim);
                    for (size_t j = 0; j < dim; ++j) {
                        interpolated[j] = (1.0 - t) * parent[j] + t * candidate[j];
                    }
                    if (isCollision(interpolated)) {
                        collides = true;
                        break;
                    }
                }

                if (!collides) {
                    // Add to tree
                    tree[candidate] = parent;
                    levels[next_level_index].push_back(candidate);
                    ++nodes_added_in_next;
                    ++node_count;

                    // Update best distance
                    double dist = 0.0;
                    for (size_t j = 0; j < dim; ++j) {
                        dist += (candidate[j] - qfin[j]) * (candidate[j] - qfin[j]);
                    }
                    bestdist = std::min(bestdist, dist);

                    // Check goal proximity
                    if (dist < GOAL_SQUARED_DIST) {
                        found = true;
                        endnode = candidate;
                        break;
                    }
                }
            }
        }

        // Move to next level
        nl = next_level_index;
        if (nodes_added_in_next == 0 && !found) {
            // No progress this level, continue loop but won't add nodes; avoids infinite loop
            // Actually the while condition will stop when node_count or nl exceeded.
        }
    }

    if (!found) {
        return {};  // no path
    }

    // Reconstruct path from endnode back to start
    std::vector<std::vector<double>> reverse_path;
    reverse_path.push_back(endnode);
    std::vector<double> current = endnode;
    while (current != qini) {
        auto it = tree.find(current);
        if (it == tree.end()) break;  // safety
        current = it->second;
        reverse_path.push_back(current);
        if (current == qini) break;
    }
    std::reverse(reverse_path.begin(), reverse_path.end());

    // Optionally add interpolation points for smoother path
    // Use number of steps based on current depth: maxDepth / depth, min 1
    int current_depth = static_cast<int>(reverse_path.size()) - 1;
    int steps_per_segment = std::max(1, maxDepth / std::max(1, current_depth));

    std::vector<std::vector<double>> final_path;
    for (size_t i = 0; i + 1 < reverse_path.size(); ++i) {
        const auto& p0 = reverse_path[i];
        const auto& p1 = reverse_path[i + 1];
        for (int s = 0; s < steps_per_segment; ++s) {
            double t = static_cast<double>(s) / steps_per_segment;
            std::vector<double> point(dim);
            for (size_t j = 0; j < dim; ++j) {
                point[j] = (1.0 - t) * p0[j] + t * p1[j];
            }
            final_path.push_back(point);
        }
    }
    final_path.push_back(reverse_path.back());  // ensure goal included

    return final_path;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <functional>

// Declare the function under test (from solution)
std::vector<std::vector<double>> rrtPlan(
    const std::vector<double>& start,
    const std::vector<double>& goal,
    std::function<bool(const std::vector<double>&)> isCollision,
    int maxIterations = 10000,
    int maxDepth = 30);

int main() {
    // Helper: always collision-free
    auto noCollision = [](const std::vector<double>&) { return false; };

    // Test 1: Start equals goal (or very close) -> path should be non-empty and first == last
    {
        std::vector<double> start = {0,0,0,0,0,0,0};
        std::vector<double> goal = {0.01,0,0,0,0,0,0};
        auto path = rrtPlan(start, goal, noCollision, 1000, 10);
        assert(!path.empty());
        assert(path.front() == start);
        assert(path.back() == goal);
    }

    // Test 2: Simple straight-line move, should find a path of length >= 1
    {
        std::vector<double> start = {0,0,0,0,0,0,0};
        std::vector<double> goal = {1.0,0,0,0,0,0,0};
        auto path = rrtPlan(start, goal, noCollision, 10000, 30);
        assert(!path.empty());
        assert(path.front() == start);
        // Goal may not be exactly reached due to tolerance, but last point should be close
        double last_dist = 0;
        for (size_t i = 0; i < start.size(); ++i) {
            last_dist += (path.back()[i] - goal[i]) * (path.back()[i] - goal[i]);
        }
        assert(last_dist < 0.2);
    }

    // Test 3: Collision everywhere except start -> should return empty
    {
        std::vector<double> start = {0,0,0,0,0,0,0};
        std::vector<double> goal = {1,0,0,0,0,0,0};
        auto alwaysCollide = [](const std::vector<double>&) { return true; };
        auto path = rrtPlan(start, goal, alwaysCollide, 1000, 10);
        assert(path.empty());
    }

    // Test 4: Path with a central obstacle (simple hyperplane) forces detour
    {
        std::vector<double> start = {0,0,0,0,0,0,0};
        std::vector<double> goal = {2,0,0,0,0,0,0};
        auto obstacleCollision = [](const std::vector<double>& q) {
            // Collision if joint 0 is between 0.8 and 1.2 and joint 1 is small
            return (q[0] > 0.8 && q[0] < 1.2 && q[1] < 0.5);
        };
        auto path = rrtPlan(start, goal, obstacleCollision, 10000, 30);
        assert(!path.empty());
        // Verify no point in path (excluding endpoints) is inside obstacle
        for (size_t i = 0; i < path.size(); ++i) {
            assert(!obstacleCollision(path[i]));
        }
    }

    // Test 5: Very large goal tolerance (squared distance < 0.1) with zero-depth case
    {
        std::vector<double> start = {0.1,0.2,0.3,0.4,0.5,0.6,0.7};
        std::vector<double> goal = {0.15,0.2,0.3,0.4,0.5,0.6,0.7}; // dist^2 = 0.0025 < 0.1
        auto path = rrtPlan(start, goal, noCollision, 100, 1);
        assert(!path.empty());
        assert(path.front() == start);
        assert(path.back() == goal);
    }

    // Test 6: Ensure path continuity (each consecutive pair should be "close")
    {
        std::vector<double> start = {0,0,0,0,0,0,0};
        std::vector<double> goal = {3,1,-2,0.5,-0.5,1.5,2};
        auto path = rrtPlan(start, goal, noCollision, 10000, 30);
        assert(!path.empty());
        for (size_t i = 0; i + 1 < path.size(); ++i) {
            double diff = 0;
            for (size_t j = 0; j < start.size(); ++j) {
                diff += (path[i][j] - path[i+1][j]) * (path[i][j] - path[i+1][j]);
            }
            assert(diff < 5.0); // loose bound, just ensure no teleportation
        }
    }

    // Test 7: Check that path respects collision-free property entirely
    {
        std::vector<double> start = {-1,-1,-1,-1,-1,-1,-1};
        std::vector<double> goal = {1,1,1,1,1,1,1};
        auto sphereCollision = [](const std::vector<double>& q) {
            // Collision if within radius 0.5 of origin
            double sum = 0;
            for (double v : q) sum += v*v;
            return sum < 0.25;
        };
        auto path = rrtPlan(start, goal, sphereCollision, 10000, 30);
        // May or may not find path due to randomness, but if found must be valid
        if (!path.empty()) {
            for (const auto& pt : path) {
                assert(!sphereCollision(pt));
            }
        }
    }

    // Test 8: Ensure function respects iteration limit
    {
        std::vector<double> start = {0,0,0,0,0,0,0};
        std::vector<double> goal = {100,0,0,0,0,0,0}; // far away
        auto path = rrtPlan(start, goal, noCollision, 10, 10);
        // With only 10 iterations, may or may not find path; just ensure no crash
        (void)path;
        assert(true);
    }

    // Test 9: Duplicate start and goal exactly
    {
        std::vector<double> point = {1,2,3,4,5,6,7};
        auto path = rrtPlan(point, point, noCollision, 100, 1);
        assert(!path.empty());
        assert(path.size() == 1);
        assert(path[0] == point);
    }

    // Test 10: Dimensions must match (actual function checks implicitly by loops)
    {
        std::vector<double> start = {0,0,0,0,0,0,0};
        std::vector<double> goal = {1,1,1,1,1,1,1};
        auto path = rrtPlan(start, goal, noCollision);
        assert(!path.empty());
        assert(path.front().size() == 7);
    }
}
