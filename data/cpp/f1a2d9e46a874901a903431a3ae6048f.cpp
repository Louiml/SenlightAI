Write a C++ function named `doesIntersect` that takes four floating-point parameters representing two intervals on the real line: `a1`, `b1` for the first interval `[a1, b1]` and `a2`, `b2` for the second interval `[a2, b2]`. The function must return `true` if the two closed intervals share at least one point, and `false` otherwise. Intervals are inclusive, meaning endpoints belong to the interval. You must handle cases where the intervals are given in any order (i.e., `a1` may be greater or less than `b1`), but you may assume the parameters are finite real numbers. The solution should be robust and avoid floating-point equality pitfalls by using a clear mathematical condition.
The two closed intervals `[min(a1,b1), max(a1,b1)]` and `[min(a2,b2), max(a2,b2)]` intersect if and only if the start of each interval is less than or equal to the end of the other interval. Formally, after normalizing each interval so that its lower endpoint is `L1 = min(a1,b1)`, `U1 = max(a1,b1)`, `L2 = min(a2,b2)`, `U2 = max(a2,b2)`, the intersection is non-empty when `L1 <= U2` and `L2 <= U1`. This condition covers all cases including touching at a single endpoint (equality), fully overlapping intervals, and containment. Edge cases: identical intervals, degenerate intervals where a equals b (point intervals), and intervals with reversed order of parameters. No special handling is needed for floating-point equality because the condition uses `<=` which correctly treats touching endpoints as intersecting. Time complexity is O(1) and space complexity is O(1).
#include <algorithm>

// Determine if two closed intervals [a1,b1] and [a2,b2] overlap.
// The intervals are inclusive of their endpoints, and parameters may be in any order.
bool doesIntersect(float a1, float b1, float a2, float b2) {
    // Normalize each interval so that first value <= second value.
    const float L1 = std::min(a1, b1);
    const float U1 = std::max(a1, b1);
    const float L2 = std::min(a2, b2);
    const float U2 = std::max(a2, b2);

    // Two intervals intersect if neither starts after the other ends.
    return (L1 <= U2) && (L2 <= U1);
}
#include <cassert>

// Free function declared in solution; include the header or definition here.
bool doesIntersect(float, float, float, float);

int main() {
    // Basic cases: overlapping, touching, disjoint
    assert(doesIntersect(0.0f, 5.0f, 3.0f, 8.0f) == true);
    assert(doesIntersect(0.0f, 5.0f, 5.0f, 8.0f) == true);   // touch at 5
    assert(doesIntersect(0.0f, 5.0f, 6.0f, 8.0f) == false);

    // Intervals given in reversed order of endpoints
    assert(doesIntersect(5.0f, 0.0f, 8.0f, 3.0f) == true);   // [0,5] and [3,8]
    assert(doesIntersect(5.0f, 0.0f, 9.0f, 6.0f) == false);  // [0,5] and [6,9]

    // Degenerate intervals (points)
    assert(doesIntersect(2.0f, 2.0f, 2.0f, 2.0f) == true);   // same point
    assert(doesIntersect(2.0f, 2.0f, 3.0f, 3.0f) == false);  // different points

    // Fully contained intervals
    assert(doesIntersect(-10.0f, 10.0f, -2.0f, 2.0f) == true);
    assert(doesIntersect(-2.0f, 2.0f, -10.0f, 10.0f) == true);

    // Negative values and mixed signs
    assert(doesIntersect(-5.0f, -1.0f, -4.0f, 0.0f) == true);
    assert(doesIntersect(-5.0f, -1.0f, 0.0f, 2.0f) == false);

    // Large and small magnitudes
    assert(doesIntersect(1e20f, 2e20f, 1.5e20f, 3e20f) == true);
    assert(doesIntersect(-1e-20f, 1e-20f, 2e-20f, 3e-20f) == false);
}
