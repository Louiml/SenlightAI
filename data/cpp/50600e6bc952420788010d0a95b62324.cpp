// Write a C++ function that, given the coordinates of two axis-aligned rectangles (specified by their bottom-left and top-right corners), computes the area of the first rectangle that remains after removing the area of the second rectangle, assuming the second rectangle is entirely inside the first (including boundaries). Coordinates are integers in the range [-1000, 1000]. The function should take eight integers (x1, y1, x2, y2 for the first rectangle and x3, y3, x4, y4 for the second rectangle) and return the remaining area as an integer. If the second rectangle touches or coincides with the edges or corners of the first, the removal still applies only to the interior of the first rectangle (i.e., the boundary of the first rectangle is not removed). If the removable area is empty, return 0.
#include <cassert>

int main() {
    // Basic case: large rectangle minus smaller centered rectangle
    assert(remainingArea(0, 0, 10, 10, 2, 2, 8, 8) == 100 - 36);

    // Second rectangle touches edges (no interior removal, but area still subtracted)
    assert(remainingArea(0, 0, 10, 10, 0, 0, 10, 10) == 0);

    // Second rectangle completely covers first (same area)
    assert(remainingArea(0, 0, 5, 5, 0, 0, 5, 5) == 0);

    // Second rectangle is a line (zero width) -> no area removed
    assert(remainingArea(0, 0, 10, 10, 5, 0, 5, 10) == 100);

    // Second rectangle is a point (zero width and height)
    assert(remainingArea(0, 0, 10, 10, 7, 7, 7, 7) == 100);

    // Coordinates near the boundary range [-1000, 1000]
    assert(remainingArea(-1000, -1000, 1000, 1000, -500, -500, 500, 500) == 4000000 - 1000000);

    // Unordered coordinates for first rectangle
    assert(remainingArea(10, 10, 0, 0, 2, 2, 8, 8) == 100 - 36);

    // Unordered coordinates for second rectangle
    assert(remainingArea(0, 0, 10, 10, 8, 8, 2, 2) == 100 - 36);

    // Second rectangle touching bottom edge
    assert(remainingArea(0, 0, 10, 10, 2, 0, 8, 5) == 100 - 30);

    // Second rectangle touching left edge
    assert(remainingArea(0, 0, 10, 10, 0, 2, 8, 8) == 100 - 48);

    // Single unit rectangle, second is same
    assert(remainingArea(1, 1, 2, 2, 1, 1, 2, 2) == 0);
}
#include <algorithm>

// Compute the area of the first rectangle after removing the interior of the second rectangle.
// Assumes the second rectangle is entirely inside the first (including boundaries).
// If coordinates are unordered, they are normalized to ensure positive width/height.
int remainingArea(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    // Normalize first rectangle
    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);
    // Normalize second rectangle
    if (x3 > x4) std::swap(x3, x4);
    if (y3 > y4) std::swap(y3, y4);

    // Area of first rectangle
    int area_first = (x2 - x1) * (y2 - y1);
    // Area of second rectangle (fully inside, so intersection = second rectangle)
    int area_second = (x4 - x3) * (y4 - y3);

    // Remaining area cannot be negative
    return std::max(0, area_first - area_second);
}
// The original code uses a 2D grid to mark cells covered by the first rectangle, then unmarks cells strictly inside the second rectangle (using exclusive bounds for deletion). The remaining area is computed from the bounding box of marked cells. For a standalone function, we can avoid the grid and compute the area directly. The key insight is that the remaining shape is a rectilinear polygon formed by subtracting a smaller rectangle from a larger one. Since the second rectangle is guaranteed to be inside the first, the remaining area is simply `area_first - area_intersection`, where intersection is the second rectangle itself (because it's fully inside). However, the deletion in the original code only removes the *interior* of the second rectangle, leaving its boundary intact, but since the boundary has zero area, the area removed is exactly the area of the second rectangle. Thus, the answer is `(x2 - x1) * (y2 - y1) - (x4 - x3) * (y4 - y3)`. Edge cases: if the second rectangle has zero width or height (degenerate), the removed area is 0, and the result is simply the first rectangle's area. Also, if coordinates are given in any order? The problem implies bottom-left and top-right, so assume x1 < x2, y1 < y2, and similarly for the second. If not, we should normalize by swapping to ensure correct positive dimensions. Time complexity O(1), space O(1).
