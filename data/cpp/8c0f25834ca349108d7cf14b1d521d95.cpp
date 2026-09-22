// Write a standalone C++ function that simulates the core scoring logic of a DWA (Dynamic Window Approach) local planner: given a robot’s current pose (x, y, yaw), a set of candidate velocity samples (linear x, linear y, angular z), and a set of weighted cost functions (path bias, goal bias, obstacle bias, and twirling bias), compute a total score for each candidate trajectory. The function should take a list of candidate velocity triples, a target goal (x, y, yaw), and weight parameters, and return the index of the best (lowest-cost) candidate. Candidates that move into an obstacle (simulated by a simple circular obstacle check with a fixed obstacle position and radius) or that have any cost component equal to a sentinel “invalid” value (e.g., -1) must be discarded; if no candidate is valid, return -1. For scoring, use the following formulas per candidate: path cost = Euclidean distance from the endpoint of a short straight-line simulation (using the given simulation time, default 1.0 second) to the nearest point on a straight path from the robot’s start to the goal; goal cost = Euclidean distance from the endpoint to the goal; obstacle cost = 1.0 if the endpoint or any intermediate point (sampled every 0.1 seconds) is within the obstacle radius, else 0.0; twirling cost = absolute value of angular velocity. Total cost = path_bias * path_cost + goal_bias * goal_cost + obstacle_bias * obstacle_cost + twirling_bias * twirling_cost. The function must handle edge cases: empty candidate list, duplicate candidates, and candidates that produce identical total costs (in which case choose the first encountered). The implementation must be self-contained, using only standard C++ libraries, and must not rely on ROS or any external dependencies.

// The solution simulates each candidate velocity triple over a fixed time horizon (sim_time, default 1.0) to compute the robot’s endpoint using simple kinematics: x’ = x + vx * sim_time, y’ = y + vy * sim_time, yaw’ = yaw + vth * sim_time. For obstacle avoidance, we sample the trajectory at small time steps (dt = 0.1) from start to end and check whether each sampled point is within the circular obstacle radius (given a fixed obstacle center, e.g., (5.0, 5.0) with radius 1.5). If any sampled point is inside, that candidate is marked invalid. Path cost is the perpendicular distance from the endpoint to the straight line segment from (initial_x, initial_y) to (goal_x, goal_y); this can be computed via the standard point-to-segment distance formula. Goal cost is the Euclidean distance from endpoint to goal. Twirling cost is the absolute angular velocity. Each candidate’s total cost is a weighted sum; candidates with any component equal to the sentinel -1 (or with obstacle cost causing invalid) are discarded. We iterate through the list, track the minimum total cost and its index, and handle ties by keeping the first occurrence (using strict `<` for comparison). Time complexity is O(n * (sim_time/dt)) per candidate where n is the number of candidates, so overall O(n * k) with k = sim_time/dt; space is O(1) beyond input storage. Edge cases: empty list returns -1; if all candidates invalid, return -1; negative bias weights are allowed but should be used as given.

#include <vector>
#include <cmath>
#include <limits>

// Compute Euclidean distance between two 2D points
inline double distance2D(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

// Compute distance from a point to a line segment (from p1 to p2)
double pointToSegmentDistance(double px, double py, double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    double length_sq = dx * dx + dy * dy;
    if (length_sq == 0.0) {
        return distance2D(px, py, x1, y1);
    }
    double t = ((px - x1) * dx + (py - y1) * dy) / length_sq;
    t = std::max(0.0, std::min(1.0, t));
    double proj_x = x1 + t * dx;
    double proj_y = y1 + t * dy;
    return distance2D(px, py, proj_x, proj_y);
}

// Return index of best (lowest total cost) velocity candidate, or -1 if no valid candidate.
// Candidates: vector of triples (vx, vy, vth) in order.
// start_x, start_y, start_yaw: initial robot pose.
// goal_x, goal_y, goal_yaw: target pose (yaw used only for path alignment but not necessary for this simplified cost).
// weights: path_bias, goal_bias, obstacle_bias, twirling_bias.
// obstacle_x, obstacle_y, obstacle_radius: circular obstacle. sim_time: simulation horizon.
int findBestDwaTrajectory(
    const std::vector<std::tuple<double,double,double>>& candidates,
    double start_x, double start_y, double start_yaw,
    double goal_x, double goal_y, double goal_yaw,
    double path_bias, double goal_bias, double obstacle_bias, double twirling_bias,
    double obstacle_x, double obstacle_y, double obstacle_radius,
    double sim_time = 1.0)
{
    const double dt = 0.1;
    const double invalid_sentinel = -1.0;
    double best_cost = std::numeric_limits<double>::infinity();
    int best_index = -1;

    for (size_t i = 0; i < candidates.size(); ++i) {
        double vx, vy, vth;
        std::tie(vx, vy, vth) = candidates[i];

        // Compute endpoint using simple kinematics
        double end_x = start_x + vx * sim_time;
        double end_y = start_y + vy * sim_time;
        double end_yaw = start_yaw + vth * sim_time;

        // Check obstacle collision by sampling along the trajectory
        bool obstacle_hit = false;
        double steps = std::ceil(sim_time / dt);
        for (int s = 0; s <= steps; ++s) {
            double t = std::min(s * dt, sim_time);
            double sample_x = start_x + vx * t;
            double sample_y = start_y + vy * t;
            if (distance2D(sample_x, sample_y, obstacle_x, obstacle_y) < obstacle_radius) {
                obstacle_hit = true;
                break;
            }
        }
        if (obstacle_hit) continue; // discard this candidate

        // Compute cost components
        double path_cost = pointToSegmentDistance(end_x, end_y, start_x, start_y, goal_x, goal_y);
        double goal_cost = distance2D(end_x, end_y, goal_x, goal_y);
        double obstacle_cost = 0.0; // already validated
        double twirling_cost = std::fabs(vth);

        // Check for invalid components (sentinel)
        if (path_cost < 0.0 || goal_cost < 0.0 || twirling_cost < 0.0) continue;

        double total = path_bias * path_cost + goal_bias * goal_cost + obstacle_bias * obstacle_cost + twirling_bias * twirling_cost;

        // Keep first if tie (strict less than)
        if (total < best_cost) {
            best_cost = total;
            best_index = static_cast<int>(i);
        }
    }
    return best_index;
}

#include <cassert>
#include <tuple>
#include <vector>

// The solution function is assumed to be included above.
int main() {
    // Simple case: two candidates, one clearly better (goes toward goal, no obstacle)
    {
        std::vector<std::tuple<double,double,double>> candidates = {
            {0.0, 0.0, 0.0},   // stays in place
            {1.0, 0.0, 0.0}    // moves toward goal at (10,0)
        };
        int best = findBestDwaTrajectory(candidates, 0,0,0, 10,0,0, 1.0, 1.0, 1.0, 0.1, 5.0, 0.0, 1.5);
        assert(best == 1); // moving forward should be better
    }

    // Obstacle avoidance: candidate that goes through obstacle should be discarded
    {
        std::vector<std::tuple<double,double,double>> candidates = {
            {2.0, 0.0, 0.0},  // goes straight through obstacle at (5,0)
            {0.5, 1.0, 0.0}   // moves around
        };
        int best = findBestDwaTrajectory(candidates, 0,0,0, 10,0,0, 1.0, 1.0, 10.0, 0.0, 5.0, 0.0, 1.5);
        assert(best == 1);
    }

    // All candidates invalid (all hit obstacle)
    {
        std::vector<std::tuple<double,double,double>> candidates = {
            {5.0, 0.0, 0.0},
            {5.0, 1.0, 0.0}
        };
        int best = findBestDwaTrajectory(candidates, 0,0,0, 10,0,0, 1.0, 1.0, 10.0, 0.0, 5.0, 0.0, 1.5);
        assert(best == -1);
    }

    // Empty candidate list
    {
        std::vector<std::tuple<double,double,double>> candidates;
        int best = findBestDwaTrajectory(candidates, 0,0,0, 10,0,0, 1.0, 1.0, 1.0, 0.0, 5.0, 0.0, 1.5);
        assert(best == -1);
    }

    // Tie-breaking: identical costs, first should be chosen
    {
        std::vector<std::tuple<double,double,double>> candidates = {
            {1.0, 0.0, 0.0},
            {1.0, 0.0, 0.0}
        };
        int best = findBestDwaTrajectory(candidates, 0,0,0, 10,0,0, 1.0, 1.0, 1.0, 0.0, 5.0, 0.0, 1.5);
        assert(best == 0);
    }

    // Twirling bias: prefer non-spinning candidate even if other costs similar
    {
        std::vector<std::tuple<double,double,double>> candidates = {
            {1.0, 0.0, 2.0},  // spins
            {1.0, 0.0, 0.0}   // no spin
        };
        int best = findBestDwaTrajectory(candidates, 0,0,0, 10,0,0, 0.0, 0.0, 0.0, 10.0, 5.0, 0.0, 1.5);
        assert(best == 1);
    }

    // Goal bias: prefer candidate that ends closer to goal
    {
        std::vector<std::tuple<double,double,double>> candidates = {
            {1.0, 0.0, 0.0},  // ends at (1,0)
            {3.0, 0.0, 0.0}   // ends at (3,0) closer to goal at (5,0)
        };
        int best = findBestDwaTrajectory(candidates, 0,0,0, 5,0,0, 0.0, 1.0, 0.0, 0.0, 5.0, 0.0, 1.5);
        assert(best == 1);
    }

    // Short simulation time: no collision with obstacle at distance 5, but endpoint beyond obstacle
    {
        std::vector<std::tuple<double,double,double>> candidates = {
            {3.0, 0.0, 0.0}  // with sim_time=1, ends at (3,0) still before obstacle, but path samples may cross? actually with dt 0.1, samples go to 3.0, all before 5.0
        };
        int best = findBestDwaTrajectory(candidates, 0,0,0, 10,0,0, 1.0, 1.0, 1.0, 0.0, 5.0, 0.0, 1.5, 1.0);
        assert(best == 0);
    }

    return 0;
}
