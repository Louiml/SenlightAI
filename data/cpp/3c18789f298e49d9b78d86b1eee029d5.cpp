// Write a C++ function named `minimumDistanceToRectangle` that takes four integers `x`, `y`, `w`, and `h` representing a point `(x, y)` and an axis-aligned rectangle with its lower-left corner at `(0, 0)` and its upper-right corner at `(w, h)`. The function must return the minimum Euclidean distance (as an integer, using Manhattan-like axis-aligned distance) from the point to any side or corner of the rectangle. The distance is defined as the smallest of the absolute differences between the point's `x`-coordinate and the rectangle's left (0) and right (`w`) boundaries, and between the point's `y`-coordinate and the rectangle's bottom (0) and top (`h`) boundaries. The input values are positive integers with `x`, `y`, `w`, `h` all between 1 and 1000 inclusive. The function should return that minimum integer distance.

#include <cassert>

int minimumDistanceToRectangle(int x, int y, int w, int h);

int main() {
    // Point inside rectangle, nearest to left side
    assert(minimumDistanceToRectangle(3, 4, 10, 10) == 3);
    // Point exactly on bottom side
    assert(minimumDistanceToRectangle(5, 0, 10, 10) == 0);
    // Point outside to the right and above, nearest to right side
    assert(minimumDistanceToRectangle(12, 15, 10, 10) == 2);
    // Point outside above, nearest to top side
    assert(minimumDistanceToRectangle(5, 12, 10, 10) == 2);
    // Point at top-right corner (distance zero in both axes)
    assert(minimumDistanceToRectangle(10, 10, 10, 10) == 0);
    // Point outside to the left and below, nearest to left side
    assert(minimumDistanceToRectangle(-3, -4, 10, 10) == 3);
    // Point inside, nearest to top side
    assert(minimumDistanceToRectangle(8, 9, 10, 10) == 1);
    // Point outside to the left, x-distance smaller than y-distance
    assert(minimumDistanceToRectangle(-2, 8, 10, 10) == 2);
    // Point outside below, y-distance smaller than x-distance
    assert(minimumDistanceToRectangle(6, -7, 10, 10) == 7);
    // Point exactly in the middle, all sides equal distance (5 each)
    assert(minimumDistanceToRectangle(5, 5, 10, 10) == 5);
    return 0;
}

#include <algorithm>

// Return the minimum axis-aligned distance from point (x, y)
// to the rectangle with corners (0,0) and (w, h).
int minimumDistanceToRectangle(int x, int y, int w, int h) {
    int minDistance = std::min(std::abs(x - 0), std::abs(x - w));
    minDistance = std::min(minDistance, std::abs(y - 0));
    minDistance = std::min(minDistance, std::abs(y - h));
    return minDistance;
}

// The problem reduces to finding the shortest axis-aligned distance from the given point to the four sides of the rectangle. The rectangle spans from `x=0` to `x=w` and from `y=0` to `y=h`. For a point `(x, y)`, the horizontal distances to the left and right vertical sides are `abs(x - 0)` and `abs(x - w)`, respectively. Similarly, the vertical distances to the bottom and top horizontal sides are `abs(y - 0)` and `abs(y - h)`. The desired answer is the minimum among these four values. This works regardless of whether the point is inside or outside the rectangle; if inside, the distances are positive and represent the distance to the nearest side. Edge cases include the point lying exactly on a side (distance zero), or on a corner (both distances zero), and the minimum being correctly computed even when multiplicities occur. The time complexity is O(1) because only four constant-time operations are performed, and the space complexity is O(1) as only a few scalar variables are used. No loops or data structures are needed.
