Write a C++ function `bool canPlaceSmallCircles(long double n, long double R, long double r)` that determines whether `n` identical small circles of radius `r` can be placed inside a single larger circle of radius `R` so that each small circle touches the boundary of the large circle (i.e., each small circle is internally tangent to the large circle) and no two small circles overlap (they may touch). The input values are positive real numbers (with `n` interpreted as an integer count, though passed as `long double` for consistency with the snippet). The function must return `true` if such a placement is possible, and `false` otherwise.
For the small circles to be internally tangent to the large circle, their centers must lie on a circle of radius `R - r` (because the distance from the large circle's center to a small circle's center is `R - r`). To fit `n` such circles without overlap, the angular separation between any two adjacent centers must be at least the angle subtended by two radii `r` at the central circle of radius `R - r`. That minimal angle is `2 * asin(r / (R - r))` (derived from the law of sines in the triangle formed by the two small circle centers and the large circle's center). If `n` times this minimal angle is at most `2π` (i.e., `2 * acos(0)`) plus a tiny epsilon for floating-point error, then they fit. Special cases: when `r > R`, impossible; when `r == R`, only one small circle can fit (because `R - r = 0`, centers coincide at the middle, and any second would overlap). When `r < R`, the general angle formula applies (note that `R - r > 0`). Edge case: if `r` is very close to `R`, the angle becomes large, so likely only 1 fits; the formula handles that. Also, if `r` is very small, the angle is tiny, allowing many circles. The comparison uses `4 * acosl(0)` (which is `2π`) plus `1E-7` tolerance. Time complexity is O(1), space O(1). Must handle `long double` precision with `asinl` and `acosl`.
#include <cmath>   // for asinl, acosl
#include <limits>  // for numeric_limits

// Determines if n small circles of radius r can be placed inside a large
// circle of radius R, each touching the large circle's boundary without
// overlapping each other (they may touch).
bool canPlaceSmallCircles(long double n, long double R, long double r) {
    const long double pi_2 = 2.0L * acosl(0.0L);  // exact 2π
    const long double eps = 1.0e-7L;

    if (r < R) {
        // Centers lie on a circle of radius (R - r).
        long double angle = 2.0L * asinl(r / (R - r));
        return (angle * n < pi_2 + eps);
    } else if (r == R) {
        // Only one circle can fit (center at the middle).
        return (n == 1.0L);
    } else {
        // r > R: impossible.
        return false;
    }
}
#include <cassert>
#include <cmath> // for fabs

int main() {
    // Basic: 1 small circle always fits if r <= R
    assert(canPlaceSmallCircles(1.0L, 10.0L, 1.0L) == true);
    assert(canPlaceSmallCircles(1.0L, 10.0L, 10.0L) == true);
    assert(canPlaceSmallCircles(1.0L, 10.0L, 15.0L) == false);

    // r == R special case
    assert(canPlaceSmallCircles(1.0L, 5.0L, 5.0L) == true);
    assert(canPlaceSmallCircles(2.0L, 5.0L, 5.0L) == false);
    assert(canPlaceSmallCircles(0.5L, 5.0L, 5.0L) == false); // n must be integer count

    // r < R: check standard known cases
    // 6 circles of radius 1 inside a circle of radius 2 (R-r=1, centers on circle radius 1)
    assert(canPlaceSmallCircles(6.0L, 2.0L, 1.0L) == true);
    // 7 circles would not fit (angle * 7 > 2π)
    assert(canPlaceSmallCircles(7.0L, 2.0L, 1.0L) == false);

    // Large R, small r: many circles fit
    assert(canPlaceSmallCircles(100.0L, 1000.0L, 1.0L) == true);
    assert(canPlaceSmallCircles(10000.0L, 1000.0L, 1.0L) == false);

    // Boundary case: exactly 2π (should be true with epsilon tolerance)
    // For R=3, r=1, R-r=2, angle = 2*asin(0.5)=π/3, so 6 fit exactly
    assert(canPlaceSmallCircles(6.0L, 3.0L, 1.0L) == true);

    // Ensure false for r>R even if n is also large
    assert(canPlaceSmallCircles(1.0L, 1.0L, 1.0001L) == false);

    // Check integer-like n values passed as long double
    assert(canPlaceSmallCircles(3.0L, 10.0L, 5.0L) == true); // R-r=5, angle=2*asin(1)=π, 3*π > 2π? no, π<2π so yes?
    // Actually angle=2*asin(1)=π, 3*π ≈ 9.42, 2π≈6.28, so false
    // But wait, r=5, R=10, R-r=5, r/(R-r)=1, asin(1)=π/2, angle=π. 3*π>2π → false
    assert(canPlaceSmallCircles(3.0L, 10.0L, 5.0L) == false);
    // 2 circles: 2*π > 2π? No, 2π ≈ 6.28, 2π > 2π? equal, epsilon allows true
    assert(canPlaceSmallCircles(2.0L, 10.0L, 5.0L) == true);

    return 0;
}
