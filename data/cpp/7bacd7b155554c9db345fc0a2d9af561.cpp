// Write a standalone C++ function that, given a vector of non-negative floating-point cost values representing a robot path (where a value of 0.0 means no cost, values in the range (0, 1) represent small repulsive penalties, and values ≥ 1.0 represent obstacles or collisions), simulates a simplified obstacle critic. Specifically, the function must compute a "cost score" for the entire path: it should sum squared penalties for every value in the range [1.0, 2.0) (representing near-collision but not lethal), add a large fixed penalty (e.g., 1000.0) if any value is ≥ 2.0 (lethal collision), and ignore all values below 1.0. The final score is the sum of contributions across all elements, returned as a float. The function must handle an empty vector by returning 0.0, must treat values ≥ 2.0 as immediate collision (once found, the entire path fails, and the function may return early with the collision penalty), and must correctly process duplicate values and floating-point edge cases (e.g., exactly 1.0 and exactly 2.0).
The problem is straightforward: iterate through the vector, and for each element:
- If element < 1.0: skip (no cost).
- If element >= 2.0: return the collision constant immediately (since a collision means the whole path is penalized; no need to continue).
- Otherwise (1.0 ≤ element < 2.0): compute a penalty as (element - 1.0)^2 and add it to an accumulator.

Important edge cases:
- Empty vector: return 0.0.
- Values exactly 1.0: penalty (0)^2 = 0, so contributes nothing.
- Values exactly 2.0: treated as collision, return collision constant.
- NaN or infinity: not specified, but we assume finite non-negative inputs; we could handle NaN as a collision but simplicity suggests we ignore that. For robustness, we can treat any non-finite as a collision (if element >= 2.0 includes infinity). We'll use a simple comparison.

Time complexity: O(n) in the worst case, but if a collision is found early, it returns in O(1) average. Space complexity: O(1) auxiliary.
#include <vector>
#include <cmath>

// Simulates an obstacle critic for a vector of cost values.
// Returns 0.0 for empty input, else:
// - For each value in [1.0, 2.0): adds (value - 1.0)^2 to the total.
// - If any value >= 2.0: immediately returns 1000.0 (collision penalty).
// Values < 1.0 are ignored.
float obstacleCriticScore(const std::vector<float>& costs) {
    constexpr float collision_penalty = 1000.0f;
    float total = 0.0f;

    for (float cost : costs) {
        if (cost >= 2.0f) {
            return collision_penalty;
        }
        if (cost >= 1.0f) {
            float diff = cost - 1.0f;
            total += diff * diff;
        }
    }
    return total;
}
#include <cassert>
#include <vector>

// Declaration of the solution function (should be included from the solution)
float obstacleCriticScore(const std::vector<float>& costs);

int main() {
    // Empty vector
    assert(obstacleCriticScore({}) == 0.0f);

    // All free space (costs < 1.0)
    assert(obstacleCriticScore({0.0f, 0.5f, 0.99f}) == 0.0f);

    // Exactly 1.0 contributes zero
    assert(obstacleCriticScore({1.0f}) == 0.0f);

    // Single near-obstacle: (1.5-1)^2 = 0.25
    assert(std::abs(obstacleCriticScore({1.5f}) - 0.25f) < 1e-5f);

    // Multiple near-obstacles: (1.1-1)^2 + (1.2-1)^2 = 0.01+0.04=0.05
    assert(std::abs(obstacleCriticScore({1.1f, 1.2f}) - 0.05f) < 1e-5f);

    // Just below collision: 1.9999, sum = (0.9999)^2 ~ 0.9998
    float near_collision = obstacleCriticScore({1.9999f});
    assert(std::abs(near_collision - 0.9998f) < 1e-3f);

    // Collision at exactly 2.0
    assert(obstacleCriticScore({2.0f}) == 1000.0f);

    // Collision after some near-obstacles (should return collision only)
    assert(obstacleCriticScore({1.5f, 2.0f, 1.1f}) == 1000.0f);

    // Large collision value
    assert(obstacleCriticScore({5.0f}) == 1000.0f);

    // Mixed: free, near, free, near
    assert(std::abs(obstacleCriticScore({0.0f, 1.5f, 0.8f, 1.0f}) - 0.25f) < 1e-5f);

    return 0;
}
