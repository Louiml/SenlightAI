// Write a standalone C++ function that determines whether a given axis-aligned rectangle (specified by its lower-left and upper-right corner coordinates as `double` values) is completely covered by another rectangle, meaning the first rectangle lies entirely inside the second rectangle or exactly on its border. The function should return `true` if the first rectangle is fully contained within or touches the boundary of the second rectangle, and `false` otherwise. Handle edge cases where rectangles touch exactly at edges or corners, and where they overlap partially or not at all. The function must be const-correct, take the rectangles as parameters using appropriate data structures, and be usable in a test harness without a `main` function in the solution file.
The solution defines a simple `Rectangle` struct with four `double` members: `minX`, `minY`, `maxX`, `maxY`, where `minX <= maxX` and `minY <= maxY`. The main algorithm checks four inequalities: the first rectangle is covered by the second if and only if its minimum x-coordinate is at least the second's minimum x-coordinate, its maximum x-coordinate is at most the second's maximum x-coordinate, and similarly for the y-coordinates. This ensures the first rectangle does not extend beyond the second in any direction, including touching boundaries (using `>=` and `<=`). Edge cases include identical rectangles (true), touching at a single corner (true), partial overlap where one side extends beyond (false), and disjoint rectangles (false). Time complexity is O(1) with constant space O(1), as only four comparisons are required.
#include <utility>

// Represents an axis-aligned rectangle with lower-left and upper-right corners.
struct Rectangle {
    double minX;
    double minY;
    double maxX;
    double maxY;
};

// Returns true if rectangle a is entirely inside or on the boundary of rectangle b.
bool isCoveredBy(const Rectangle& a, const Rectangle& b) {
    return a.minX >= b.minX &&
           a.maxX <= b.maxX &&
           a.minY >= b.minY &&
           a.maxY <= b.maxY;
}
#include <cassert>

int main() {
    // Basic containment
    Rectangle inner{1.0, 1.0, 2.0, 2.0};
    Rectangle outer{0.0, 0.0, 3.0, 3.0};
    assert(isCoveredBy(inner, outer) == true);

    // Identical rectangles
    Rectangle same{0.0, 0.0, 1.0, 1.0};
    assert(isCoveredBy(same, same) == true);

    // Touching at right edge
    Rectangle touchRight{1.0, 0.0, 2.0, 1.0};
    Rectangle outer2{0.0, 0.0, 2.0, 1.0};
    assert(isCoveredBy(touchRight, outer2) == true);

    // Touching only at a corner
    Rectangle corner{1.0, 1.0, 2.0, 2.0};
    Rectangle outer3{0.0, 0.0, 1.0, 1.0};
    assert(isCoveredBy(corner, outer3) == true); // only point (1,1) shared

    // Overlapping but extending beyond
    Rectangle overlap{0.5, 0.5, 2.5, 2.5};
    Rectangle outer4{0.0, 0.0, 2.0, 2.0};
    assert(isCoveredBy(overlap, outer4) == false);

    // Disjoint rectangles
    Rectangle disjoint{3.0, 3.0, 4.0, 4.0};
    Rectangle outer5{0.0, 0.0, 2.0, 2.0};
    assert(isCoveredBy(disjoint, outer5) == false);

    // Negative coordinates and touching on left/bottom
    Rectangle negInner{-2.0, -2.0, -1.0, -1.0};
    Rectangle negOuter{-3.0, -3.0, 0.0, 0.0};
    assert(isCoveredBy(negInner, negOuter) == true);

    // Inner exactly zero-width or zero-height (degenerate) but within outer
    Rectangle degenerateLine{1.0, 1.0, 1.0, 2.0}; // vertical line
    Rectangle outer6{0.0, 0.0, 3.0, 3.0};
    assert(isCoveredBy(degenerateLine, outer6) == true);

    // Inner extends beyond in y only
    Rectangle tall{0.0, -1.0, 1.0, 2.0};
    Rectangle outer7{0.0, 0.0, 1.0, 1.0};
    assert(isCoveredBy(tall, outer7) == false);

    return 0;
}
