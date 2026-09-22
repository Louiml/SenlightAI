/*
Write a C++ function `computeStateAndReward` that takes six double parameters: the current distance between two robots (`dist`), the current bearing angle (`phi`), a set distance (`set_dist`), a set angle (`set_ang`), a distance threshold (`thresh_dist`), and an angle threshold (`thresh_ang`). The function must return a `std::pair<int, int>` where the first element is the state index computed as `state_dist * 3 + state_ang`, and the second element is the reward value. The state_dist is 0 if `dist` is less than or equal to `set_dist - thresh_dist`, 1 if it lies strictly between `set_dist - thresh_dist` and `set_dist + thresh_dist`, and 2 otherwise. Similarly, state_ang is 0 if `phi` is less than or equal to `set_ang - thresh_ang`, 1 if strictly between `set_ang - thresh_ang` and `set_ang + thresh_ang`, and 2 otherwise. The reward is 2 if both `dist` and `phi` are within their respective threshold bounds (i.e., state_dist==1 and state_ang==1), 1 if exactly one of them is within bounds, and -1 if neither is within bounds. Angles may be outside the range [-π, π] and should be normalized before computing state_ang, but the normalization must not affect the reward computation (which uses the original supplied `phi` and thresholds). Assume all thresholds are positive. Ensure the function is `const`-correct and handles edge cases with values exactly at thresholds (e.g., `dist == set_dist - thresh_dist` should result in state_dist=0, not 1, and if `phi` is exactly at a boundary, it is considered outside the inner range, so state_ang would be 0 or 2, not 1). Provide a reference solution and test examples.
*/

#include <utility>
#include <cmath>
#include <algorithm>

// Compute the discrete state index and reward based on distance and bearing.
// Returns a pair {state, reward} where state = state_dist * 3 + state_ang.
std::pair<int, int> computeStateAndReward(
    double dist, double phi,
    double set_dist, double set_ang,
    double thresh_dist, double thresh_ang) {

    // Normalize phi to [-π, π] for state computation.
    double normalized_phi = phi;
    while (normalized_phi > M_PI)
        normalized_phi -= 2.0 * M_PI;
    while (normalized_phi < -M_PI)
        normalized_phi += 2.0 * M_PI;

    // Distance state.
    int state_dist = 0;
    if (dist > set_dist - thresh_dist && dist < set_dist + thresh_dist)
        state_dist = 1;
    else if (dist >= set_dist + thresh_dist)
        state_dist = 2;
    // else state_dist remains 0 (dist <= set_dist - thresh_dist)

    // Angle state based on normalized phi.
    int state_ang = 0;
    if (normalized_phi > set_ang - thresh_ang && normalized_phi < set_ang + thresh_ang)
        state_ang = 1;
    else if (normalized_phi >= set_ang + thresh_ang)
        state_ang = 2;
    // else state_ang remains 0 (normalized_phi <= set_ang - thresh_ang)

    int state = state_dist * 3 + state_ang;

    // Reward: 2 if both within bounds, 1 if exactly one, -1 otherwise.
    bool dist_ok = (dist > set_dist - thresh_dist && dist < set_dist + thresh_dist);
    bool ang_ok = (normalized_phi > set_ang - thresh_ang && normalized_phi < set_ang + thresh_ang);
    int reward;
    if (dist_ok && ang_ok)
        reward = 2;
    else if (dist_ok || ang_ok)
        reward = 1;
    else
        reward = -1;

    return {state, reward};
}

#include <cassert>
#include <cmath>
#include <utility>

// Declare the function (in actual test, include the solution header).
std::pair<int, int> computeStateAndReward(double, double, double, double, double, double);

int main() {
    // Exact center: distance 1.5, angle 0, set 1.5 and 0 with thresholds 0.5 and π/6.
    auto result = computeStateAndReward(1.5, 0.0, 1.5, 0.0, 0.5, M_PI/6);
    assert(result.first == 4);  // state_dist=1, state_ang=1 => 1*3+1=4
    assert(result.second == 2);

    // Distance too far, angle ok.
    result = computeStateAndReward(2.0, 0.0, 1.5, 0.0, 0.5, M_PI/6);
    assert(result.first == 7);  // state_dist=2, state_ang=1 => 2*3+1=7
    assert(result.second == 1);

    // Distance too close, angle ok.
    result = computeStateAndReward(0.9, 0.0, 1.5, 0.0, 0.5, M_PI/6);
    assert(result.first == 1);  // state_dist=0, state_ang=1 => 0*3+1=1
    assert(result.second == 1);

    // Distance ok, angle too large.
    result = computeStateAndReward(1.5, 1.0, 1.5, 0.0, 0.5, M_PI/6);
    assert(result.first == 5);  // state_dist=1, state_ang=2 => 1*3+2=5
    assert(result.second == 1);

    // Both out of bounds.
    result = computeStateAndReward(2.5, 1.0, 1.5, 0.0, 0.5, M_PI/6);
    assert(result.first == 8);  // state_dist=2, state_ang=2 => 2*3+2=8
    assert(result.second == -1);

    // Boundary: dist exactly at set_dist - thresh_dist => state_dist=0, and phi exactly at set_ang - thresh_ang => state_ang=0.
    result = computeStateAndReward(1.0, -M_PI/6, 1.5, 0.0, 0.5, M_PI/6);
    assert(result.first == 0);  // 0*3+0=0
    assert(result.second == -1);  // both not strictly inside

    // Boundary: dist exactly at set_dist + thresh_dist => state_dist=2, phi exactly at set_ang + thresh_ang => state_ang=2.
    result = computeStateAndReward(2.0, M_PI/6, 1.5, 0.0, 0.5, M_PI/6);
    assert(result.first == 8);  // 2*3+2=8
    assert(result.second == -1);

    // Angle normalization: phi = 2π + 0.1 should normalize to 0.1, which may be within bounds.
    result = computeStateAndReward(1.5, 2*M_PI + 0.1, 1.5, 0.0, 0.5, M_PI/6);
    // normalized_phi = 0.1, which is within (-π/6, π/6) => state_ang=1, state_dist=1 => state=4.
    assert(result.first == 4);
    assert(result.second == 2);

    return 0;
}

// The problem is a straightforward discretization of a continuous two-dimensional space into a 3×3 grid, combined with a reward rule based on whether each dimension is within a target interval. The main steps are: (1) Compute the discrete state indices for distance and angle using inclusive/exclusive boundary rules exactly as specified; (2) Determine the reward by checking whether `dist` is strictly greater than `set_dist - thresh_dist` AND strictly less than `set_dist + thresh_dist` for the distance component, and similarly for the angle component using the original `phi` (without normalization, because the threshold comparison is based on the raw angle; however, the problem statement says to normalize phi for computing state_ang, so we must normalize phi into [-π, π] first, then apply the boundary checks to the normalized value; this could potentially change the state if the original phi is e.g., 2π+ε, but that is acceptable per the task). For reward, the specification says "the reward is 2 if both dist and phi are within their respective threshold bounds", so we use the normalized phi for that check as well, since the distance check uses raw dist and the angle check should use the normalized phi to stay consistent. Edge cases occur at exact boundary points: for state_dist, `dist <= set_dist - thresh_dist` gives 0, and `dist >= set_dist + thresh_dist` gives 2, but if `dist == set_dist + thresh_dist` it is not strictly less, so it goes to 2; similarly for angle. Time complexity is O(1) and space complexity is O(1). The function is pure and deterministic, so it can be tested with assertions directly.
