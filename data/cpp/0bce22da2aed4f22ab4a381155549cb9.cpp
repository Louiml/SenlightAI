/*
Write a C++ function `scanlineOverlapLength` that takes two sorted vectors of scanline intersection events (where each event has an `x` coordinate and a signed `direction` representing the change in winding number at that point), a query interval `[xFrom, xTo)`, and a fill rule (nonzero = 1, odd = 2, positive = 3, negative = 4). The function must compute the total length within the interval where both scanlines are simultaneously "inside" according to the given fill rule. The input vectors are already sorted by `x` and contain no duplicate `x` coordinates. The function must return the overlap length as a `double`, and must handle cases where the interval is empty, where either vector has no intersections, and where the fill rule produces inside states that change at each intersection.
*/

#include <vector>
#include <algorithm>

struct ScanlineIntersection {
    double x;
    int direction; // signed change in winding number
};

// Interpret accumulated direction into a boolean inside state based on fill rule.
// Rule 1=nonzero, 2=odd, 3=positive, 4=negative.
static bool applyFillRule(int accumulatedDirection, int fillRule) {
    switch (fillRule) {
        case 1: return accumulatedDirection != 0;
        case 2: return (accumulatedDirection & 1) != 0;
        case 3: return accumulatedDirection > 0;
        case 4: return accumulatedDirection < 0;
        default: return false;
    }
}

// Compute the total length within [xFrom, xTo) where both scanlines are inside,
// given sorted intersection lists and a fill rule.
double scanlineOverlapLength(
    const std::vector<ScanlineIntersection>& a,
    const std::vector<ScanlineIntersection>& b,
    double xFrom,
    double xTo,
    int fillRule
) {
    if (xTo <= xFrom) return 0.0;

    double total = 0.0;
    bool aInside = false, bInside = false;
    std::size_t i = 0, j = 0;
    int aDirection = 0, bDirection = 0;

    // Advance both pointers past all intersections before xFrom.
    while (i < a.size() && a[i].x < xFrom) {
        aDirection += a[i].direction;
        ++i;
    }
    while (j < b.size() && b[j].x < xFrom) {
        bDirection += b[j].direction;
        ++j;
    }
    aInside = applyFillRule(aDirection, fillRule);
    bInside = applyFillRule(bDirection, fillRule);

    double currentX = xFrom;
    // Sweep through the union of remaining intersection points.
    while (i < a.size() || j < b.size()) {
        double nextAX = (i < a.size()) ? a[i].x : xTo;
        double nextBX = (j < b.size()) ? b[j].x : xTo;
        double nextX = std::min(nextAX, nextBX);
        if (nextX >= xTo) break; // no more intersections inside the interval

        // Accumulate overlap from currentX to nextX.
        if (aInside == bInside) {
            total += (nextX - currentX);
        }

        // Process the intersection(s) at nextX.
        if (i < a.size() && a[i].x == nextX) {
            aDirection += a[i].direction;
            ++i;
            aInside = applyFillRule(aDirection, fillRule);
        }
        if (j < b.size() && b[j].x == nextX) {
            bDirection += b[j].direction;
            ++j;
            bInside = applyFillRule(bDirection, fillRule);
        }
        currentX = nextX;
    }

    // Add the final segment from last intersection to xTo.
    if (aInside == bInside && currentX < xTo) {
        total += (xTo - currentX);
    }
    return total;
}

#include <cassert>
#include <vector>

int main() {
    using S = ScanlineIntersection;
    // Nonzero fill rule: both inside after odd number of crossings? No, nonzero = not zero.
    // Case 1: Both scanlines inside in the same sub-interval.
    std::vector<S> a = {{1.0, +1}, {3.0, -1}}; // inside [1,3)
    std::vector<S> b = {{0.0, +1}, {2.0, -1}, {4.0, +1}, {5.0, -1}}; // inside [0,2) and [4,5)
    // Overlap inside both: [1,2) length 1.0 and [4,5) is not inside a, so total = 1.0
    assert(scanlineOverlapLength(a, b, 0.0, 5.0, 1) == 1.0);

    // Odd fill rule: inside when accumulated direction is odd.
    std::vector<S> c = {{0.0, +1}, {2.0, +1}, {4.0, -1}}; // inside [0,2) and [4,?]? direction: 1 (odd) [0,2), then 2 (even) [2,4), then 1 (odd) [4,∞)
    // For b (inside [0,2) and [4,5) under odd? b accumulation: +1 at 0 (odd) [0,2), 0 at 2 (even) [2,4), +1 at 4 (odd) [4,5)
    // Overlap odd inside both: [0,2) and [4,5) → length 2 + 1 = 3
    assert(scanlineOverlapLength(c, b, 0.0, 5.0, 2) == 3.0);

    // Positive fill rule: inside only when accumulated direction > 0.
    // a: +1 then -1 → inside only [1,?) actually for positive: direction=1 (positive) [1,3), then 0 (not positive) after 3.
    std::vector<S> d = {{2.0, +2}, {4.0, -2}}; // direction +2 (positive) [2,4), then 0 after 4.
    // b with positive: +1 at 0 (positive) [0,2), 0 at 2 (not positive) [2,4), +1 at 4 (positive) [4,5)
    // Overlap positive inside both: [2,?] a inside [2,4) and b inside [0,2) and [4,5) → no overlap in [2,4) because b not inside there. So 0.
    assert(scanlineOverlapLength(d, b, 0.0, 5.0, 3) == 0.0);

    // Negative fill rule: inside when accumulated direction < 0.
    std::vector<S> e = {{1.0, -1}}; // direction -1 (negative) [1,∞) until? Actually after 1, direction = -1 → negative.
    // Overlap with b under negative: b with directions? For negative rule, b direction after 0 is 1 (not negative), so bInside false always? Actually b at 0 +1 → 1 (positive), at 2 → 0, at 4 → +1, at 5 → 0. Never negative. So overlap is 0.
    assert(scanlineOverlapLength(e, b, 0.0, 5.0, 4) == 0.0);

    // Empty interval.
    assert(scanlineOverlapLength(a, b, 3.0, 2.0, 1) == 0.0);

    // No intersections in one or both lists.
    std::vector<S> empty;
    assert(scanlineOverlapLength(a, empty, 0.0, 5.0, 1) == 0.0); // bInside always false, a inside [1,3) → overlap only where both false? Actually if both outside then aInside==bInside true in regions where a is outside. So overlap = outside regions of a in [1,3)? Wait a inside [1,3), outside elsewhere. bInside always false. Overlap = length where aInside == false (since bInside false) → all outside a: [0,1) + [3,5) = 1+2=3? Let's compute: aInside true [1,3), false elsewhere. So overlap (both false) = [0,1) length 1 and [3,5) length 2 → total 3. But check: from 0 to 1 aInside false, b false → both equal → accumulate. From 1 to 3 a true, b false → not equal. From 3 to 5 a false, b false → accumulate. So total 1+2=3.
    assert(scanlineOverlapLength(a, empty, 0.0, 5.0, 1) == 3.0);

    // Interval completely inside a single intersection range.
    std::vector<S> f = {{0.5, +1}, {1.5, -1}};
    std::vector<S> g = {{0.0, +1}, {2.0, -1}};
    // Overlap in [0.5,1.5) and [0,2) → both inside [0.5,1.5) length 1.0, but also outside? Actually both inside only [0.5,1.5) because f inside only that, g inside [0,2). Overlap = 1.0.
    assert(scanlineOverlapLength(f, g, 0.0, 3.0, 1) == 1.0);

    // Exact boundary: xTo equals an intersection.
    std::vector<S> h = {{0.0, +1}, {2.0, -1}};
    std::vector<S> i = {{0.0, +1}, {2.0, -1}};
    // Both inside [0,2) → overlap length 2.0.
    assert(scanlineOverlapLength(h, i, 0.0, 2.0, 1) == 2.0);

    // Start after all intersections.
    assert(scanlineOverlapLength(h, i, 5.0, 10.0, 1) == 5.0); // both outside → overlap full interval.

    // Mixed direction changes.
    std::vector<S> j = {{1.0, +2}, {3.0, -1}, {4.0, -1}}; // direction: 2 (nonzero) then 1, then 0 after 4.
    std::vector<S> k = {{0.0, +1}, {2.0, +1}, {5.0, -2}}; // direction: 1, 2, then 0 after 5.
    // Nonzero: j inside [1,4), k inside [0,5) → overlap [1,4) length 3.
    assert(scanlineOverlapLength(j, k, 0.0, 6.0, 1) == 3.0);

    return 0;
}

// The core idea is a two-pointer sweep over the sorted intersection lists. We maintain the current inside state for each scanline, initialized as `false`. The algorithm advances through the ordered union of all intersection points that lie within `[xFrom, xTo)`. At each step, we compute the next `x` value among the two current pointers (or `xTo` if a list is exhausted). Between the previous point and this next point, the inside states are constant, so we can accumulate the overlap length whenever both states are equal (both inside or both outside). Then we toggle the states at the current intersection point by adding the `direction` and applying the fill rule: nonzero means state = (accumulated direction != 0), odd means accumulated direction is odd, positive means >0, negative means <0. We must first skip all intersections before `xFrom` to correctly initialize the states. Edge cases: empty interval returns 0; if either list is empty, its state remains `false` throughout; if the interval is partially or fully outside the range of intersections, the remaining portion contributes based on the final states. Time complexity is O(a + b) where a and b are the sizes of the two vectors, and space complexity is O(1).
