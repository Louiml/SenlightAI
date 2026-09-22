Write a C++ function that, given the side lengths `a`, `b`, `c` of a triangle and a positive real value `k`, returns the length of a segment parallel to side `a` that cuts off a smaller similar triangle at the top of the original triangle, such that the ratio of the area of this smaller triangle to the area of the remaining trapezoid is exactly `k`. The function must compute the required scale factor for the smaller triangle’s sides via binary search, because the relationship is nonlinear. The input values are positive real numbers, and `k` is positive. Return the length of the segment (which is `a` times the scale factor) as a double, with precision at least `1e-7`. Handle the standard case where the triangle is valid (satisfies triangle inequality) and `k` is finite. The function must be self-contained and not rely on global state.

The problem is a classic geometry/binary-search task. Let the original triangle have sides `a`, `b`, `c`, with area `S`. A line parallel to side `a` (and thus similar to the original triangle) cuts off a smaller triangle at the top with scale factor `t` (0 < t < 1). Then the smaller triangle’s sides are `a*t`, `b*t`, `c*t`, and its area is `S1 = S * t^2` because area scales quadratically with linear dimensions. The remaining trapezoid has area `S - S1`. The required condition is `S1 / (S - S1) = k`. Substituting, we get `(S * t^2) / (S - S * t^2) = t^2 / (1 - t^2) = k`, which simplifies to `t^2 = k / (1 + k)`, so there is a closed-form solution `t = sqrt(k / (1 + k))`. However, the original snippet uses a binary search with an epsilon, which is also valid and demonstrates numerical methods. For our task, we implement the binary search approach because the problem statement explicitly says "via binary search". We set the search range `low = 0` (strictly positive, but we start at 0) and `high = 1`. In each iteration, we compute `mid = (low+high)/2`. Compute the area of the smaller triangle using Heron's formula with sides `a*mid`, `b*mid`, `c*mid`. Then compute the ratio `ratio = S1 / (S - S1)`. If `ratio` is greater than `k`, we need a smaller `mid`, so set `high = mid`; otherwise set `low = mid`. Continue until `high - low` is small (e.g., `1e-12`) or the ratio is within `1e-7` of `k`. The answer is `a * mid`. Edge cases: if `k` is 0 (but the problem says positive), then `t=0` and the segment length is 0, but we handle that fine. If `k` is huge, `t` approaches 1, so the search still works. The triangle must be valid; if not, Heron’s formula may produce NaN, but we assume valid input. The time complexity is O(log(1/epsilon)) iterations, typically around 60-80 iterations for `1e-12` precision, and each iteration does constant work. Space complexity is O(1). We use `fabs` for epsilon comparison and `double` for all calculations.

#include <cmath>
#include <algorithm>

// Compute the length of the segment parallel to side a that cuts off a smaller
// similar triangle such that the ratio of its area to the trapezoid area is k.
// Uses binary search on the scale factor t (0 < t < 1).
// Returns a * t, the required length.
double cuttingSegmentLength(double a, double b, double c, double k) {
    const double EPS = 1e-12;  // precision for binary search
    const double RATIO_EPS = 1e-7;  // acceptable error in ratio

    // Heron's formula for area of a triangle
    auto area = [](double x, double y, double z) -> double {
        double p = (x + y + z) * 0.5;
        return std::sqrt(p * (p - x) * (p - y) * (p - z));
    };

    double S = area(a, b, c);  // original area
    double low = 0.0;
    double high = 1.0;

    while (high - low > EPS) {
        double mid = (low + high) * 0.5;
        double S1 = area(a * mid, b * mid, c * mid);
        double ratio = S1 / (S - S1);
        if (std::abs(ratio - k) < RATIO_EPS) {
            return a * mid;
        }
        if (ratio > k) {
            high = mid;
        } else {
            low = mid;
        }
    }

    double t = (low + high) * 0.5;
    return a * t;
}

#include <cassert>
#include <cmath>

// Forward declaration of the solution function
double cuttingSegmentLength(double a, double b, double c, double k);

int main() {
    // Test 1: equilateral triangle side 1, k=1 (scale factor sqrt(0.5) = 0.7071...)
    double result1 = cuttingSegmentLength(1.0, 1.0, 1.0, 1.0);
    assert(std::abs(result1 - 0.7071067811865476) < 1e-6);

    // Test 2: 3-4-5 triangle, side a=3, k=3 → t = sqrt(3/4)=0.866025..., segment = 3*t
    double result2 = cuttingSegmentLength(3.0, 4.0, 5.0, 3.0);
    double expected2 = 3.0 * std::sqrt(3.0 / 4.0);
    assert(std::abs(result2 - expected2) < 1e-6);

    // Test 3: very small k = 0.01, segment should be a * sqrt(0.01/1.01) ≈ a * 0.0995
    double result3 = cuttingSegmentLength(5.0, 6.0, 7.0, 0.01);
    double expected3 = 5.0 * std::sqrt(0.01 / 1.01);
    assert(std::abs(result3 - expected3) < 1e-6);

    // Test 4: large k = 100, segment approaches a
    double result4 = cuttingSegmentLength(2.0, 3.0, 4.0, 100.0);
    double expected4 = 2.0 * std::sqrt(100.0 / 101.0);
    assert(std::abs(result4 - expected4) < 1e-6);

    // Test 5: degenerate check - a=0 is invalid, but we test k=0 gives 0 (though k should be positive)
    double result5 = cuttingSegmentLength(1.0, 1.0, 1.0, 0.0);
    assert(result5 == 0.0);

    // Test 6: right triangle 5-12-13, a=5, k=1
    double result6 = cuttingSegmentLength(5.0, 12.0, 13.0, 1.0);
    double expected6 = 5.0 * std::sqrt(0.5);
    assert(std::abs(result6 - expected6) < 1e-6);

    // Test 7: isosceles triangle a=6, b=5, c=5, k=2
    double result7 = cuttingSegmentLength(6.0, 5.0, 5.0, 2.0);
    double expected7 = 6.0 * std::sqrt(2.0 / 3.0);
    assert(std::abs(result7 - expected7) < 1e-6);

    // Test 8: k = 4, a=10, b=10, c=10
    double result8 = cuttingSegmentLength(10.0, 10.0, 10.0, 4.0);
    double expected8 = 10.0 * std::sqrt(4.0 / 5.0);
    assert(std::abs(result8 - expected8) < 1e-6);

    return 0;
}
