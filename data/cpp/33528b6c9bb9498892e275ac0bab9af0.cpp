Given a set of axis-aligned rectangles in 2D space, each represented by its bottom-left corner (x, y) and its width and height (all floating-point values), write a C++ function `float totalOverlapArea(const std::vector<Rect>& rects)` that returns the total area of the union of all rectangles (i.e., the area covered by at least one rectangle). Rectangles may overlap arbitrarily, may be empty (width or height ≤ 0), and may be degenerate (zero area). The function should be efficient for up to 100 rectangles. Input rectangles are guaranteed to have finite, non-NaN coordinates.
The problem requires computing the area of the union of axis-aligned rectangles. A straightforward pairwise inclusion-exclusion is exponential and infeasible. The standard approach is **sweep line + coordinate compression**: 

1. **Collect all unique x-coordinates** from the left and right edges of every non-empty rectangle (width > 0 and height > 0). Sort these x-values. The vertical strips between consecutive x-values have constant overlap status.
2. Similarly, for each strip (between consecutive x-values), we need to know which y-intervals are covered. A naive method would, for each strip, gather all y-intervals from rectangles that span that strip, then compute the total covered length of y using a merge/union of intervals. Summing `(x[i+1] - x[i]) * coveredYLength` gives the union area.
3. **Optimization**: Instead of processing each strip independently (which would be O(n^2) with n rectangles), we can use a **sweep over x**: sort events (x-start, x-end) and maintain a set of active rectangles. However, for simplicity and given constraints (≤100 rectangles, ≤200 unique x-values), we can process each vertical strip directly:
   - For each consecutive pair of x-coordinates, compute the midpoint.
   - For each rectangle that contains this midpoint in its x-range, collect its y-interval [y, y+height].
   - Merge these y-intervals (sort by start, combine overlapping). If the total length of merged intervals is `len`, add `(x[i+1]-x[i]) * len` to the result.
   - Complexity: Let `m` be the number of unique x-coordinates (≤200 for 100 rects). For each of `m-1` strips, we scan all `n` rectangles and sort up to `n` intervals, so O(m * n log n) ≈ 200 * 100 * 7 ≈ 140k operations, which is trivial. Space: O(m + n).

**Edge cases**: 
- Empty or non-positive dimensions → ignore.
- Rectangles that touch only at edges (no area) contribute 0; the algorithm naturally handles because strip width is 0 if x-values equal, and y-length 0 if intervals only touch.
- Duplicate rectangles or fully contained ones: interval merging handles them naturally.
- Floating point: Use exact comparisons for coordinates? Since inputs are floats, we treat them as given; equal floats compare exactly. In practice, coordinates are assumed to be exact representable numbers (e.g., integers or decimals with same precision). For robustness, we use a small epsilon when checking containment/overlap? Not necessary for correct math, but for typical task, assume exact values. We’ll use strict `<` and `>` for midpoint containment to avoid boundary double-counting (midpoint is strictly between x[i] and x[i+1]).

Time complexity: O(m * n log n) where m ≤ 2n. Space O(m + n).
#include <vector>
#include <algorithm>
#include <cstddef>

struct Rect {
    double x;      // bottom-left x
    double y;      // bottom-left y
    double width;
    double height;
};

// Compute the total area of the union of axis-aligned rectangles.
// Rectangles with width <= 0 or height <= 0 are ignored.
double totalOverlapArea(const std::vector<Rect>& rects) {
    // Collect all unique x-coordinates (left and right edges).
    std::vector<double> xs;
    for (const auto& r : rects) {
        if (r.width > 0.0 && r.height > 0.0) {
            xs.push_back(r.x);
            xs.push_back(r.x + r.width);
        }
    }
    if (xs.empty()) return 0.0;

    std::sort(xs.begin(), xs.end());
    xs.erase(std::unique(xs.begin(), xs.end()), xs.end());

    double totalArea = 0.0;

    // Iterate over vertical strips between consecutive x-coordinates.
    for (std::size_t i = 0; i + 1 < xs.size(); ++i) {
        double xLeft = xs[i];
        double xRight = xs[i + 1];
        if (xRight <= xLeft) continue;  // safety
        double xMid = 0.5 * (xLeft + xRight);

        // Collect y-intervals from rectangles that span this strip.
        std::vector<std::pair<double, double>> intervals;
        for (const auto& r : rects) {
            if (r.width <= 0.0 || r.height <= 0.0) continue;
            if (r.x < xMid && xMid < r.x + r.width) {
                intervals.emplace_back(r.y, r.y + r.height);
            }
        }

        if (intervals.empty()) continue;

        // Sort intervals by start coordinate.
        std::sort(intervals.begin(), intervals.end());

        // Merge overlapping or touching intervals.
        double coveredLength = 0.0;
        double curStart = intervals[0].first;
        double curEnd = intervals[0].second;
        for (std::size_t j = 1; j < intervals.size(); ++j) {
            if (intervals[j].first <= curEnd) {
                // Overlap or touch, extend current interval.
                if (intervals[j].second > curEnd) {
                    curEnd = intervals[j].second;
                }
            } else {
                // Non-overlapping, finalize previous interval.
                coveredLength += (curEnd - curStart);
                curStart = intervals[j].first;
                curEnd = intervals[j].second;
            }
        }
        coveredLength += (curEnd - curStart);

        totalArea += (xRight - xLeft) * coveredLength;
    }

    return totalArea;
}
#include <cassert>
#include <cmath>
#include <vector>

struct Rect;  // defined in solution (include it here for test)

int main() {
    // Single rectangle
    {
        std::vector<Rect> rects = {{0.0, 0.0, 2.0, 3.0}};
        assert(std::fabs(totalOverlapArea(rects) - 6.0) < 1e-9);
    }
    // Two overlapping rectangles (area = 1*2 + 1*2 - 0.5*0.5 = 3.75)
    {
        std::vector<Rect> rects = {{0.0, 0.0, 1.0, 2.0}, {0.5, 0.5, 1.5, 1.0}};
        assert(std::fabs(totalOverlapArea(rects) - (2.0 + 1.5 - 0.25)) < 1e-9);
    }
    // Non-overlapping rectangles
    {
        std::vector<Rect> rects = {{0.0, 0.0, 1.0, 1.0}, {2.0, 2.0, 1.0, 1.0}};
        assert(std::fabs(totalOverlapArea(rects) - 2.0) < 1e-9);
    }
    // Empty and degenerate rectangles ignored
    {
        std::vector<Rect> rects = {{0.0, 0.0, 0.0, 1.0}, {0.0, 0.0, -1.0, 2.0}, {1.0, 1.0, 2.0, 2.0}};
        assert(std::fabs(totalOverlapArea(rects) - 4.0) < 1e-9);
    }
    // Touching at edges (no area overlap)
    {
        std::vector<Rect> rects = {{0.0, 0.0, 1.0, 1.0}, {1.0, 0.0, 1.0, 1.0}};
        assert(std::fabs(totalOverlapArea(rects) - 2.0) < 1e-9);
    }
    // One rectangle contains another
    {
        std::vector<Rect> rects = {{0.0, 0.0, 10.0, 10.0}, {2.0, 2.0, 3.0, 3.0}};
        assert(std::fabs(totalOverlapArea(rects) - 100.0) < 1e-9);
    }
    // Negative coordinates
    {
        std::vector<Rect> rects = {{-2.0, -1.0, 4.0, 2.0}, {-1.0, -2.0, 2.0, 4.0}};
        // union area = total of both (8 + 8) - overlap (2*1=2) = 14
        assert(std::fabs(totalOverlapArea(rects) - 14.0) < 1e-9);
    }
    return 0;
}
