// Write a C++ function named `traceWithDepthLimit` that simulates the core logic of a Whitted-style ray tracer's recursive `trace_ray` function. Given a `std::vector<double>` representing hit distances (`t` values) for a ray against objects (where `t > 0` indicates a hit, and `t <= 0` indicates no hit), a `std::vector<size_t>` representing material IDs for the corresponding objects (same size as the distance vector), a `double` background color intensity (0.0 to 1.0), a `size_t` maximum recursion depth, and a `size_t` current depth, the function must return the resulting color intensity as a `double`. The function should follow these rules: if the current depth exceeds the maximum depth, return `0.0` (black). Otherwise, find the smallest positive `t` value (closest hit). If no positive `t` exists, return the background intensity. If a hit exists, check if the corresponding material ID is `0` (non-reflective) — if so, return a color intensity of `1.0` (white). If the material ID is `1` (reflective), recursively call `traceWithDepthLimit` with the same distance and material vectors, same background, same max depth, but with the current depth incremented by 1, and return that recursive result's intensity multiplied by `0.5` (simulating reflection attenuation). Ensure the function is `const`-correct (it does not modify inputs), uses no global state, and handles edge cases like empty vectors, all non-positive `t` values, and duplicate positive `t` values.
The solution must iterate through the two parallel vectors to find the minimum positive `t` value. If none exists, return the background. If found, determine the material ID at that index. A key edge case: if multiple objects share the same minimum positive `t`, we must pick the first one encountered (since the original ray tracer's `hit_objects` typically returns the first hit). The recursive step only occurs when the material ID is `1`; for non-reflective materials (ID 0), we return `1.0` immediately without recursion. The base case for recursion is when `current_depth > max_depth`, returning `0.0`. The function must handle empty vectors by treating them as no hits. Time complexity is `O(n)` for the initial closest-hit search, and each recursive call does another `O(n)` search, so worst-case is `O(n * max_depth)` when the ray keeps reflecting. Space complexity is `O(max_depth)` due to the call stack. Important correctness: the recursive call should still respect the max depth check—if the current depth already equals max, the recursive call will have depth = max+1 and return 0.0, so the final result becomes 0.0 * 0.5 = 0.0, matching the original logic where depth > max returns black. Also, the material ID might be out of range if the vectors are malformed; we assume they are same-sized and IDs are 0 or 1 based on the spec.
#include <vector>
#include <algorithm>
#include <cstddef>

// Simulates Whitted tracer's trace_ray with a depth limit.
// distances[i] > 0 means hit at distance distances[i] for object i.
// materialIDs[i] is 0 for non-reflective, 1 for reflective.
// Returns color intensity in [0,1].
double traceWithDepthLimit(
    const std::vector<double>& distances,
    const std::vector<size_t>& materialIDs,
    double background,
    size_t max_depth,
    size_t current_depth)
{
    // If recursion depth exceeds max, return black.
    if (current_depth > max_depth) {
        return 0.0;
    }

    // Find closest positive hit.
    double min_t = -1.0; // sentinel: no hit yet
    size_t hit_index = 0;
    bool found = false;
    for (size_t i = 0; i < distances.size(); ++i) {
        double t = distances[i];
        if (t > 0.0) {
            if (!found || t < min_t) {
                min_t = t;
                hit_index = i;
                found = true;
            }
        }
    }

    // If no hit, return background.
    if (!found) {
        return background;
    }

    // Check material at the hit.
    size_t mat = materialIDs[hit_index];
    if (mat == 0) {
        // Non-reflective: return white.
        return 1.0;
    } else {
        // Reflective: recurse with depth+1, attenuate by 0.5.
        double reflected = traceWithDepthLimit(
            distances, materialIDs, background,
            max_depth, current_depth + 1);
        return 0.5 * reflected;
    }
}
#include <cassert>
#include <vector>
#include <cstddef>

// Solver function declaration (assumed from solution).
double traceWithDepthLimit(
    const std::vector<double>& distances,
    const std::vector<size_t>& materialIDs,
    double background,
    size_t max_depth,
    size_t current_depth);

int main() {
    // Case 1: no hits -> background.
    std::vector<double> d1 = {-1.0, 0.0, -2.0};
    std::vector<size_t> m1 = {0, 0, 0};
    assert(traceWithDepthLimit(d1, m1, 0.3, 3, 0) == 0.3);

    // Case 2: non-reflective hit -> white.
    std::vector<double> d2 = {2.0, 1.0, 3.0};
    std::vector<size_t> m2 = {0, 0, 0};
    assert(traceWithDepthLimit(d2, m2, 0.0, 3, 0) == 1.0);

    // Case 3: reflective hit, depth 0, max 1 -> 0.5 * (depth1: non-reflective ->1.0) = 0.5
    std::vector<double> d3 = {1.0};
    std::vector<size_t> m3 = {1};
    assert(traceWithDepthLimit(d3, m3, 0.0, 1, 0) == 0.5);

    // Case 4: reflective hit, depth 0, max 0 -> recursive call depth1 > max0 -> 0.0, so 0.5*0=0.
    assert(traceWithDepthLimit(d3, m3, 0.0, 0, 0) == 0.0);

    // Case 5: two reflections with max depth 2: depth0 -> 0.5 * (depth1 -> 0.5 * (depth2 -> 1.0)) = 0.25
    // But depth2 is non-reflective? Here all are reflective, and max=2. depth0 hit reflective -> call depth1 (max2 ok) -> hit reflective -> call depth2 (max2 ok) -> hit reflective -> call depth3 ( >2 ) -> returns 0.0, so depth2 returns 0.0, depth1 returns 0.0, depth0 returns 0.0.
    // To get 0.25, need a non-reflective at the end. Let's craft: distances {1.0} with materials {1} and max=2 -> depth0 recursive depth1 -> depth1 hit same object (reflective) -> recursive depth2 -> depth2 hit same object -> recursive depth3 -> returns 0, so depth2 returns 0, depth1 returns 0, depth0 returns 0.
    // So test that: 
    assert(traceWithDepthLimit(d3, m3, 0.0, 2, 0) == 0.0);

    // Case 6: closest hit is non-reflective even if later reflective is farther.
    std::vector<double> d6 = {1.0, 2.0};
    std::vector<size_t> m6 = {0, 1};
    assert(traceWithDepthLimit(d6, m6, 0.0, 3, 0) == 1.0);

    // Case 7: closest hit is reflective, but second hit non-reflective at larger t doesn't matter.
    std::vector<double> d7 = {1.0, 2.0};
    std::vector<size_t> m7 = {1, 0};
    // At depth0: reflective -> call depth1 (max3) -> still same closest hit (1.0) reflective -> call depth2 -> same -> call depth3 (max3 ok) -> call depth4 ( >3 ) -> returns 0.0, so each gives 0.5*prev, final = 0.0.
    assert(traceWithDepthLimit(d7, m7, 0.0, 3, 0) == 0.0);

    // Case 8: empty vectors -> no hit -> background.
    std::vector<double> d8;
    std::vector<size_t> m8;
    assert(traceWithDepthLimit(d8, m8, 0.7, 5, 0) == 0.7);

    // Case 9: duplicate closest t, first is non-reflective, second reflective -> first wins.
    std::vector<double> d9 = {1.0, 1.0};
    std::vector<size_t> m9 = {0, 1};
    assert(traceWithDepthLimit(d9, m9, 0.0, 3, 0) == 1.0);

    // Case 10: background returned when depth exceeds max even if hit exists.
    std::vector<double> d10 = {1.0};
    std::vector<size_t> m10 = {0}; // non-reflective but depth > max.
    // current_depth = 5, max = 3 -> returns 0.0, not 1.0.
    assert(traceWithDepthLimit(d10, m10, 0.5, 3, 5) == 0.0);

    return 0;
}
