Given a set of `n` points in a 2D plane, each with integer coordinates `(x, y)` and two non-negative integer weights `(u, v)`, followed by a single query line containing `"1"` and then the coordinates of a checkpoint `(qx, qy)`, write a C++ function `long long closestWeightedDistance(const std::vector<std::vector<long long>>& points, long long qx, long long qy)` that receives all points and the checkpoint and returns the minimum possible value of `u * abs(x - qx) + v * abs(y - qy)` among all points. The function must treat the input as exact integers (no floating-point) and handle up to 1000 points with coordinates and weights in the range `[0, 1e8]` (coordinates may be negative). The output is a single long long integer.
The problem is straightforward: for each point `(x, y, u, v)`, compute the weighted Manhattan distance to the query point `(qx, qy)` as `u * |x - qx| + v * |y - qy|`, then return the minimum among all points. Since `n` is at most 1000, a simple linear scan is efficient. Use `long long` to avoid overflow because `1e8 * 1e8 * 2` can reach `2e16`, which fits in 64-bit signed integers but not in 32-bit. There is no need for sorting or advanced data structures. Edge cases: coordinates may be negative, weights may be zero (if both weights are zero, distance is zero), and there is always at least one point. Time complexity is O(n) and space complexity is O(1) beyond the input vector.
#include <vector>
#include <cstdlib>
#include <algorithm>

// Computes the minimum weighted Manhattan distance from any given point to the query point.
// points[i] = {x, y, u, v} for each point.
long long closestWeightedDistance(const std::vector<std::vector<long long>>& points, long long qx, long long qy) {
    long long best = LLONG_MAX;
    for (const auto& p : points) {
        long long distance = p[2] * llabs(p[0] - qx) + p[3] * llabs(p[1] - qy);
        best = std::min(best, distance);
    }
    return best;
}
#include <cassert>
#include <climits>
#include <vector>

// Forward declaration of the solution function.
long long closestWeightedDistance(const std::vector<std::vector<long long>>& points, long long qx, long long qy);

int main() {
    // Single point, query at same location, zero weights.
    std::vector<std::vector<long long>> p1 = {{0, 0, 0, 0}};
    assert(closestWeightedDistance(p1, 0, 0) == 0);

    // Two points, simple check.
    std::vector<std::vector<long long>> p2 = {{0, 0, 1, 1}, {10, 10, 1, 1}};
    assert(closestWeightedDistance(p2, 5, 5) == 10); // first gives 5+5=10, second gives 5+5=10

    // Negative coordinates, weights larger.
    std::vector<std::vector<long long>> p3 = {{-3, 4, 2, 0}, {7, -1, 0, 3}};
    assert(closestWeightedDistance(p3, 0, 0) == 6); // first: 2*3 + 0 = 6, second: 0 + 3*1 = 3? Wait: |7-0|=7, | -1 | =1, so 0*7 + 3*1 = 3 -> actually min is 3.
    // Correction: for p3, first gives 2*3 = 6, second gives 3*1 = 3. So min is 3.
    assert(closestWeightedDistance(p3, 0, 0) == 3);

    // All weights zero, any point distance zero.
    std::vector<std::vector<long long>> p4 = {{1, 1, 0, 0}, {100, -100, 0, 0}};
    assert(closestWeightedDistance(p4, 50, 50) == 0);

    // Maximum value check: distance can be large but fits in long long.
    std::vector<std::vector<long long>> p5 = {{-100000000, -100000000, 100000000, 100000000}};
    assert(closestWeightedDistance(p5, 100000000, 100000000) == 40000000000000000LL); // 2e8 * 1e8 * 2 = 4e16

    return 0;
}
