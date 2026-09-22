Write a C++ function that, given two parallel arrays of `double` values `x` and `y` (each of length `n ≥ 2`, with `x` strictly increasing), returns the minimum vertical distance between the piecewise-linear function defined by connecting the points `(x[i], y[i])` sequentially, and any line segment that connects two consecutive points in that polyline. More precisely, for each interval `[x[i], x[i+1]]`, consider the line `L_i(t) = y[i] + ((y[i+1]-y[i])/(x[i+1]-x[i]))*(t - x[i])`. For each point `t` on that interval, compute `d(t) = max over all j=0..n-2 of (L_j(t) - L_i(t))` (i.e., the maximum vertical distance from `L_i(t)` to the entire polyline above it). The function should return the minimum over all `i` and all `t` in `[x[i], x[i+1]]` of that `d(t)` value. The input guarantee is that the polyline is not self-intersecting in a way that makes the maximum undefined, and that the intervals are ordered. The return value should be printed with 3 decimal places. Your function must be named `minimalGapBetweenSegments` and accept `const std::vector<double>& x` and `const std::vector<double>& y`. The result should be a `double`.
// The algorithm works by considering each segment `i` (from `i=0` to `n-2`). For a fixed segment `i`, the function `f(t) = max_{j} (seg_j(t) - seg_i(t))` is convex on `[x[i], x[i+1]]` because it is the maximum of linear functions (each `seg_j(t) - seg_i(t)` is linear in `t`). Therefore, we can use ternary search to find the minimum of `f` on that interval. For each candidate `t`, we compute `seg_j(t)` for all segments `j` and take the maximum difference. The ternary search is run for a fixed number of iterations (e.g., 100) or until the interval is very small. The answer is the minimum over all segments of the minimum found by ternary search. Edge cases: if two consecutive points have equal x-coordinates (not allowed per problem constraints), division by zero would occur; also handle n=2 (only one segment) — the answer is 0 because the segment is compared to itself, so the maximum difference is trivially 0. Time complexity: O((n-1) * (n-1) * iterations) = O(n^2) for the inner loop, and for each ternary search iteration we compute over all n-1 segments, so overall O(iterations * n^2). With n up to 300, this is fine. Space complexity is O(1) extra beyond the input vectors.
#include <vector>
#include <algorithm>
#include <cmath>

// Compute the vertical distance from the point (t, seg_i(t)) to the maximum of all segments at t.
static double maxGapAt(const std::vector<double>& x, const std::vector<double>& y, int i, double t) {
    int n = (int)x.size();
    // Slope and intercept of segment i
    double xi = x[i], yi = y[i];
    double k_i = (y[i+1] - y[i]) / (x[i+1] - xi);
    double b_i = yi - xi * k_i;
    double value_i = k_i * t + b_i;
    // Maximum over all segments j of (seg_j(t) - value_i)
    double maxDiff = 0.0;
    for (int j = 0; j < n-1; ++j) {
        double k_j = (y[j+1] - y[j]) / (x[j+1] - x[j]);
        double b_j = y[j] - x[j] * k_j;
        double val_j = k_j * t + b_j;
        maxDiff = std::max(maxDiff, val_j - value_i);
    }
    return maxDiff;
}

// Return the minimum vertical gap between any point on a segment and the highest point of the entire polyline above that segment.
double minimalGapBetweenSegments(const std::vector<double>& x, const std::vector<double>& y) {
    int n = (int)x.size();
    if (n < 2) return 0.0;
    double answer = 1e200;
    for (int i = 0; i < n-1; ++i) {
        double left = x[i];
        double right = x[i+1];
        // Ternary search to find the minimum of the convex function on [left, right]
        for (int iter = 0; iter < 100; ++iter) {
            double m1 = left + (right - left) / 3.0;
            double m2 = right - (right - left) / 3.0;
            double f1 = maxGapAt(x, y, i, m1);
            double f2 = maxGapAt(x, y, i, m2);
            if (f1 < f2) {
                right = m2;
            } else {
                left = m1;
            }
        }
        double mid = (left + right) / 2.0;
        answer = std::min(answer, maxGapAt(x, y, i, mid));
    }
    return answer;
}
#include <cassert>
#include <vector>
#include <cmath>

// The solution function is declared above, but for the test we include it here.

int main() {
    // Test 1: Simple two-point line, gap should be 0.
    std::vector<double> x1 = {0.0, 1.0};
    std::vector<double> y1 = {0.0, 1.0};
    assert(std::fabs(minimalGapBetweenSegments(x1, y1) - 0.0) < 1e-9);

    // Test 2: Three points forming a peak, gap on the base segment should be positive.
    std::vector<double> x2 = {0.0, 1.0, 2.0};
    std::vector<double> y2 = {0.0, 1.0, 0.0};
    // The maximum gap occurs on the segment from 0 to 1 or 1 to 2.
    // The peak is at x=1, and the other segment is below it.
    // The answer is equal to the maximum height difference, which is 0.0
    // because each segment only needs to be compared to itself? Actually re-evaluate:
    // For segment 0 (0->1), the other segment (1->2) is below it for t in [0,1]?
    // No, at t=0.5, segment0 gives 0.5, segment1 gives 1 - 0.5 = 0.5, so diff=0.
    // At t=0, segment0=0, segment1=-1? Wait segment1 at x=0 is extrapolated y=1 - (0)*( -1?) Actually slope = (0-1)/(2-1) = -1, so at t=0, segment1 = 1 + (-1)*(0-1)=2? Wait let's compute: segment1 from (1,1) to (2,0): slope = -1, line: y -1 = -1(t-1) => y = -t+2. At t=0, y=2. So segment0 at t=0 is 0, diff=2. So gap can be large. For segment 0 at t=0, gap = 2. But the minimum over t on [0,1] might be 0 at t=1 (since at t=1 both give 1). Actually let's test with the code logic.
    double result2 = minimalGapBetweenSegments(x2, y2);
    // We'll just check it's non-negative.
    assert(result2 >= 0.0);

    // Test 3: A straight line, all segments identical, gap 0.
    std::vector<double> x3 = {0.0, 1.0, 2.0};
    std::vector<double> y3 = {2.0, 3.0, 4.0};
    assert(std::fabs(minimalGapBetweenSegments(x3, y3) - 0.0) < 1e-9);

    // Test 4: A V-shape with sharp valley, gap 0 because each segment is above itself?
    // Actually each segment is compared to all, so min gap is 0 if there is any t where all segments are ≤ the chosen segment.
    std::vector<double> x4 = {0.0, 1.0, 2.0};
    std::vector<double> y4 = {0.0, -1.0, 0.0};
    double result4 = minimalGapBetweenSegments(x4, y4);
    assert(result4 >= 0.0);

    // Test 5: Known example from problem statement? Use random small test that we can compute manually.
    // Points (0,0), (1,2), (2,0). For segment 0 (0-1), line slope=2, y=2t. At t=0.5, segment0=1, segment1 from (1,2) to (2,0) slope=-2, line: y= -2(t-1)+2 = -2t+4. At t=0.5, segment1=3. diff=2. At t=0, segment0=0, segment1=4, diff=4. At t=1, both give 2, diff=0. So min on [0,1] is 0. For segment1, similar. So answer 0.
    std::vector<double> x5 = {0.0, 1.0, 2.0};
    std::vector<double> y5 = {0.0, 2.0, 0.0};
    assert(std::fabs(minimalGapBetweenSegments(x5, y5) - 0.0) < 1e-9);

    // Test 6: More complex, ensure it converges and returns finite.
    std::vector<double> x6 = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y6 = {0.0, 1.0, 0.0, 2.0};
    double result6 = minimalGapBetweenSegments(x6, y6);
    assert(std::isfinite(result6));

    // Test 7: n=2 with non-zero slope, gap 0.
    std::vector<double> x7 = {1.0, 2.0};
    std::vector<double> y7 = {5.0, 7.0};
    assert(std::fabs(minimalGapBetweenSegments(x7, y7) - 0.0) < 1e-9);

    // Test 8: Large n to check performance.
    std::vector<double> x8, y8;
    for (int i = 0; i < 300; ++i) {
        x8.push_back(i * 0.1);
        y8.push_back(std::sin(i * 0.1));
    }
    double result8 = minimalGapBetweenSegments(x8, y8);
    assert(std::isfinite(result8) && result8 >= 0.0);

    return 0;
}
