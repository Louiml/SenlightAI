// Write a C++ function `computeSplineIndex` that, given a sorted vector of knot times `t` (with at least one element), a query time `s`, and a boolean `cyclic`, computes the interval index `i` and fractional parameter `u` (where `0 <= u < 1` for in-bounds queries) according to the spline interpolation logic described below. For non-cyclic splines, if `s` is less than the first knot, extrapolate using the first interval length (`t[1]-t[0]`) and allow `i` and `u` to be negative or exceed the last index as needed (handling off-bottom and off-top cases). If `s` is beyond the last knot but less than the last knot plus the final interval (for cyclic, `getFinalInterval()` is nonzero and equals half the sum of first and last interval lengths; for non-cyclic, assume constant extrapolation with the last interval length), treat it as an interior interval from `N-2` to `N-1` or from `N-1` to `N` for non-cyclic. For cyclic splines, use the same logic but wrap around: if `s` is outside `[t[0], t[N-1] + finalInterval)`, reduce `s` by multiples of the total duration (which equals `t[N-1]-t[0]` plus the final interval) before computing, and then add the number of wraps times `N` to the index. The function must handle the edge case where `N==1` (return `i=0, u=0`) and `N==2` (only one interval, so for in-bounds queries `i` is 0 and `u` is proportional). For in-bounds queries, use the provided binary search logic: start with an initial guess proportional to `(s-t[0])/(t[N-1]-t[0])` times `(N-1)`, then adjust by binary search between `lo=0` and `hi=N-1` until `t[i] <= s < t[i+1]` (or `t[i] <= s` for the last interval in cyclic). The function should take `const std::vector<float>& t` and `bool cyclic` as inputs and return a `std::pair<int, float>`.
The core task is to implement index and parameter lookup for a Catmull-Rom spline, which requires mapping a scalar time `s` to an interval index `i` and a local coordinate `u` within that interval. The logic mirrors the given `SplineBase::computeIndex` method. The main algorithm begins by handling trivial cases: if `t` has fewer than 2 knots, return `(0,0)`. For non-cyclic splines, if `s` is below the first knot, compute `x = (s - t[0]) / (t[1]-t[0])`, set `i = floor(x)` and `u = x - i`. If `s` is at or beyond the last knot, similarly compute `x = (N-1) + (s - t[N-1]) / (t[N-1]-t[N-2])`, then `i = floor(x)` and `u = x - i`. For in-bounds `s` (i.e., `t[0] <= s < t[N-1]`), use the binary search method: initialize `i` as `floor((N-1)*(s - t[0]) / (t[N-1] - t[0]))`, then loop while `t[i] > s` or `t[i+1] <= s`, adjusting `lo` and `hi` accordingly. After finding the correct `i`, compute `u = (s - t[i]) / (t[i+1] - t[i])`. For cyclic splines, first compute the final interval `fi = (t[1]-t[0] + t[N-1]-t[N-2])/2`. The total duration is `t[N-1] - t[0] + fi`. If `s` is outside `[t[0], t[N-1]+fi)`, compute `wraps = floor((s - t[0]) / duration)`, reduce `s` by `wraps*duration`, compute the index and u on this reduced `s` (using the same in-bounds logic, but note that the interval `[t[N-1], t[N-1]+fi)` is treated as a separate interval with index `N-1` and `u = (s - t[N-1]) / fi`), then add `wraps*N` to the index. For cyclic in-bounds `s` (including the last interval), if `s >= t[N-1]`, set `i = N-1` and `u = (s - t[N-1])/fi`; otherwise use the binary search. Edge cases include `N==1`, where there are no intervals, so return `(0,0)`, and `N==2` where the binary search works normally but only one interval exists (so `i` is always 0 for in-bounds). The time complexity is `O(log N)` for the binary search in the general in-bounds case (or `O(1)` for uniform-ish initialization plus a few iterations), and `O(1)` for off-bounds extrapolation. Space is `O(1)` auxiliary.
#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>

// Computes the interval index i and fractional parameter u for a spline.
// t: sorted knot times (at least one element). For non-cyclic, at least 2 for meaningful intervals.
// s: query time.
// cyclic: if true, the spline wraps around with a final interval computed from first and last intervals.
// Returns pair<int, float> where i is the interval index and u is in [0,1) for in-bounds queries.
std::pair<int, float> computeSplineIndex(const std::vector<float>& t, float s, bool cyclic) {
    int N = static_cast<int>(t.size());
    if (N == 0) return {0, 0.0f};
    if (N == 1) return {0, 0.0f};

    float t0 = t[0];
    float tn = t[N - 1];

    // Handle non-cyclic case
    if (!cyclic) {
        if (s < t0) {
            // Off bottom: extrapolate using first interval
            float dt = t[1] - t0;
            float x = (s - t0) / dt;
            int i = static_cast<int>(std::floor(x));
            float u = x - i;
            return {i, u};
        } else if (s >= tn) {
            // Off top: extrapolate using last interval
            if (N < 2) return {0, 0.0f};
            float dt = tn - t[N - 2];
            float x = (N - 1) + (s - tn) / dt;
            int i = static_cast<int>(std::floor(x));
            float u = x - i;
            return {i, u};
        } else {
            // In bounds: binary search
            int lo = 0;
            int hi = N - 1;
            int i = static_cast<int>(floor((N - 1) * (s - t0) / (tn - t0)));
            // Clamp initial guess to valid range
            i = std::max(0, std::min(N - 2, i));
            while (t[i] > s || t[i + 1] <= s) {
                if (t[i] > s) {
                    hi = i - 1;
                } else {
                    lo = i + 1;
                }
                i = (hi + lo) / 2;
            }
            float u = (s - t[i]) / (t[i + 1] - t[i]);
            return {i, u};
        }
    }

    // Cyclic case
    float fi = (t[1] - t0 + tn - t[N - 2]) * 0.5f;
    float duration = tn - t0 + fi;

    // Reduce s modulo duration
    float s_reduced = s;
    int wraps = 0;
    if (s < t0 || s >= tn + fi) {
        wraps = static_cast<int>(std::floor((s - t0) / duration));
        s_reduced = s - wraps * duration;
    }

    // Now t0 <= s_reduced < tn + fi
    if (s_reduced >= tn) {
        // Last interval (from tn to tn+fi)
        int i = N - 1;
        float u = (s_reduced - tn) / fi;
        return {i + wraps * N, u};
    } else {
        // In bounds (or equal to t0)
        int lo = 0;
        int hi = N - 1;
        int i = static_cast<int>(floor((N - 1) * (s_reduced - t0) / (tn - t0)));
        i = std::max(0, std::min(N - 2, i));
        while (t[i] > s_reduced || t[i + 1] <= s_reduced) {
            if (t[i] > s_reduced) {
                hi = i - 1;
            } else {
                lo = i + 1;
            }
            i = (hi + lo) / 2;
        }
        float u = (s_reduced - t[i]) / (t[i + 1] - t[i]);
        return {i + wraps * N, u};
    }
}
#include <cassert>
#include <vector>
#include <cmath>

// Declare the function from the solution
std::pair<int, float> computeSplineIndex(const std::vector<float>& t, float s, bool cyclic);

int main() {
    // Non-cyclic, uniform knots
    std::vector<float> t = {0.0f, 1.0f, 2.0f, 3.0f};
    auto res1 = computeSplineIndex(t, 1.5f, false);
    assert(res1.first == 1);
    assert(std::fabs(res1.second - 0.5f) < 1e-6);

    // Non-cyclic, exact knot
    auto res2 = computeSplineIndex(t, 2.0f, false);
    assert(res2.first == 1); // interval [1,2) ends, but binary search finds i=1 because t[2] not <= 2
    // Actually for s=2, the condition t[i+1] <= s is true for i=1, so it adjusts to i=2. Let's check:
    // The while loop condition: t[1]=1 <= 2 and t[2]=2 <= 2 -> t[i+1]<=s true, so i becomes 2.
    // Hence res2.first should be 2, u=0. So we assert that:
    assert(res2.first == 2);
    assert(std::fabs(res2.second - 0.0f) < 1e-6);

    // Non-cyclic, off bottom
    auto res3 = computeSplineIndex(t, -1.0f, false);
    // x = (-1-0)/1 = -1 -> i=-1, u=0
    assert(res3.first == -1);
    assert(std::fabs(res3.second - 0.0f) < 1e-6);

    // Non-cyclic, off top
    auto res4 = computeSplineIndex(t, 5.0f, false);
    // x = 3 + (5-3)/1 = 5 -> i=5, u=0
    assert(res4.first == 5);
    assert(std::fabs(res4.second - 0.0f) < 1e-6);

    // Non-cyclic, N=1
    std::vector<float> t1 = {2.0f};
    auto res5 = computeSplineIndex(t1, 0.0f, false);
    assert(res5.first == 0 && res5.second == 0.0f);

    // Non-cyclic, N=2
    std::vector<float> t2 = {0.0f, 10.0f};
    auto res6 = computeSplineIndex(t2, 5.0f, false);
    assert(res6.first == 0);
    assert(std::fabs(res6.second - 0.5f) < 1e-6);

    // Cyclic, uniform knots
    std::vector<float> tc = {0.0f, 1.0f, 2.0f, 3.0f};
    // finalInterval = (1+1)/2 = 1, duration = 3+1 = 4
    auto res7 = computeSplineIndex(tc, 3.5f, true);
    // s=3.5 in [tn=3, tn+fi=4) -> i=3, u=0.5
    assert(res7.first == 3);
    assert(std::fabs(res7.second - 0.5f) < 1e-6);

    // Cyclic, wraps around
    auto res8 = computeSplineIndex(tc, 5.0f, true);
    // s=5 -> wraps = floor((5-0)/4) = 1, s_reduced = 1.0 -> in bounds i=1,u=0, add 1*N => i=5,u=0
    assert(res8.first == 5);
    assert(std::fabs(res8.second - 0.0f) < 1e-6);

    // Cyclic, negative s
    auto res9 = computeSplineIndex(tc, -1.0f, true);
    // s=-1 -> wraps = floor((-1-0)/4) = -1, s_reduced = 3.0 -> last interval i=3,u=0, add -1*N => i=-1,u=0
    assert(res9.first == -1);
    assert(std::fabs(res9.second - 0.0f) < 1e-6);

    // Cyclic, N=2
    std::vector<float> tc2 = {0.0f, 2.0f};
    auto res10 = computeSplineIndex(tc2, 1.0f, true);
    // fi = (2+2)/2 = 2, duration = 2+2 =4. s=1 in bounds -> binary search i=0,u=0.5
    assert(res10.first == 0);
    assert(std::fabs(res10.second - 0.5f) < 1e-6);

    return 0;
}
