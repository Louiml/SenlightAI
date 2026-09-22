// Write a C++ function that computes the maximum squared distance between any two points in a 3D particle system, given an array of `N` particles (each with `x`, `y`, `z` coordinates stored in separate parallel arrays) and returns this value as a `double`. The function must handle `N >= 2`, must not modify the input arrays, and must use only `O(1)` extra space (excluding the input arrays themselves). The coordinates are arbitrary real numbers (including negative values), and the squared distance between two points \((x_i,y_i,z_i)\) and \((x_j,y_j,z_j)\) is defined as \((x_i-x_j)^2 + (y_i-y_j)^2 + (z_i-z_j)^2\). The result should be exact within floating-point precision.
#include <cassert>
#include <cmath>

// Declaration of the function under test (assume it is defined above)
double maxSquaredDistance(const double* x, const double* y, const double* z, std::size_t N);

int main() {
    // Two points: distance squared = 1^2 + 2^2 + 3^2 = 14
    double x1[] = {0.0, 1.0};
    double y1[] = {0.0, 2.0};
    double z1[] = {0.0, 3.0};
    assert(maxSquaredDistance(x1, y1, z1, 2) == 14.0);

    // Three points: farthest are (0,0,0) and (3,4,5) -> 9+16+25 = 50
    double x2[] = {0.0, 1.0, 3.0};
    double y2[] = {0.0, 1.0, 4.0};
    double z2[] = {0.0, 1.0, 5.0};
    assert(maxSquaredDistance(x2, y2, z2, 3) == 50.0);

    // Duplicate points: distance 0
    double x3[] = {1.0, 1.0, 1.0};
    double y3[] = {2.0, 2.0, 2.0};
    double z3[] = {3.0, 3.0, 3.0};
    assert(maxSquaredDistance(x3, y3, z3, 3) == 0.0);

    // Negative coordinates: (-1,-1,-1) and (2,2,2) -> 9+9+9 = 27
    double x4[] = {-1.0, 2.0, 0.5};
    double y4[] = {-1.0, 2.0, 0.5};
    double z4[] = {-1.0, 2.0, 0.5};
    assert(maxSquaredDistance(x4, y4, z4, 3) == 27.0);

    // Large values, approximate comparison due to floating point
    double x5[] = {1e10, -1e10};
    double y5[] = {0.0, 0.0};
    double z5[] = {0.0, 0.0};
    double expected = 4e20; // (2e10)^2 = 4e20
    double got = maxSquaredDistance(x5, y5, z5, 2);
    assert(std::fabs(got - expected) < 1e-6 * expected);

    // N == 2 with mixed signs: (3,4,5) and (-2,-6,0) -> dx=5, dy=10, dz=5 -> 25+100+25=150
    double x6[] = {3.0, -2.0};
    double y6[] = {4.0, -6.0};
    double z6[] = {5.0, 0.0};
    assert(maxSquaredDistance(x6, y6, z6, 2) == 150.0);

    return 0;
}
#include <cstddef>
#include <cmath>
#include <algorithm>

// Compute the maximum squared Euclidean distance among N 3D points.
// Points are given as three parallel arrays for x, y, z coordinates.
// The input arrays are not modified. N must be at least 2.
double maxSquaredDistance(const double* x, const double* y, const double* z, std::size_t N) {
    if (N < 2) {
        return 0.0; // not enough points; by convention return 0
    }
    double maxDistSq = 0.0;
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = i + 1; j < N; ++j) {
            double dx = x[i] - x[j];
            double dy = y[i] - y[j];
            double dz = z[i] - z[j];
            double distSq = dx*dx + dy*dy + dz*dz;
            maxDistSq = std::max(maxDistSq, distSq);
        }
    }
    return maxDistSq;
}
// The straightforward approach is to compare every pair of points with a double nested loop, computing the squared distance for each pair and keeping track of the maximum. This requires \(O(N^2)\) time and \(O(1)\) auxiliary space. Since the problem asks for the maximum squared distance, we do not need to take square roots, which avoids unnecessary floating-point errors and is slightly faster. Edge cases include negative coordinates (handled naturally by squaring), duplicate points (distance zero), and large coordinates that may cause overflow if using `float` – hence we use `double`. We also assume `N >= 2` to guarantee at least one pair exists. The function should be `const` correct: take the arrays as pointers to `const double` and the count as a `size_t` or `int`. For very large `N` (e.g., millions), the \(O(N^2)\) loop may be slow, but that is acceptable given the task specification; no special optimization is required.
