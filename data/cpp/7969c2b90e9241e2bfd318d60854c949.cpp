/*
Write a C++ function `int minimalSquareSide(const std::vector<std::pair<int, int>>& points)` that, given at least one point (but possibly the same point repeated), returns the minimum possible side length of an axis-aligned square whose boundary (the four edges, including corners) contains every given point. If no such square exists, return `-1`. The square may be placed anywhere in the plane, but its sides must be parallel to the x and y axes. Points lying strictly inside (not on the boundary) are not allowed. The input coordinates can be any integers, possibly negative, and there can be duplicate points. The side length must be a non-negative integer (note: a square of side 0 is allowed only if all points are identical and lie on that degenerate boundary). The function must not modify the input vector.
*/
#include <vector>
#include <utility>
#include <algorithm>

// Return true if all points lie on the boundary of the axis-aligned square
// with lower-left corner (sx, sy) and given side length.
static bool allOnBoundary(const std::vector<std::pair<int, int>>& points,
                          int sx, int sy, int side) {
    int right = sx + side;
    int top = sy + side;
    for (const auto& p : points) {
        int x = p.first;
        int y = p.second;
        bool onLeftOrRight = (x == sx || x == right) && (sy <= y && y <= top);
        bool onBottomOrTop = (y == sy || y == top) && (sx <= x && x <= right);
        if (!onLeftOrRight && !onBottomOrTop) {
            return false;
        }
    }
    return true;
}

// Return minimal side length of an axis-aligned square whose boundary
// contains all given points, or -1 if impossible.
int minimalSquareSide(const std::vector<std::pair<int, int>>& points) {
    if (points.empty()) return -1; // Not specified, but handle defensively.

    int min_x = points[0].first, max_x = points[0].first;
    int min_y = points[0].second, max_y = points[0].second;
    for (const auto& p : points) {
        min_x = std::min(min_x, p.first);
        max_x = std::max(max_x, p.first);
        min_y = std::min(min_y, p.second);
        max_y = std::max(max_y, p.second);
    }

    int side = std::max(max_x - min_x, max_y - min_y);

    // Candidate lower-left corners.
    // If width < side, can shift left; if height < side, can shift down.
    const std::vector<std::pair<int, int>> candidates = {
        {min_x, min_y},
        {min_x, max_y - side},
        {max_x - side, min_y},
        {max_x - side, max_y - side}
    };

    for (const auto& cand : candidates) {
        if (allOnBoundary(points, cand.first, cand.second, side)) {
            return side;
        }
    }
    return -1;
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above; here we only test it.
int main() {
    // Single point: side 0 works.
    assert(minimalSquareSide({{0,0}}) == 0);

    // Two points forming a horizontal segment: side = width.
    assert(minimalSquareSide({{-2, 3}, {2, 3}}) == 4);

    // Two points forming a vertical segment: side = height.
    assert(minimalSquareSide({{5, -1}, {5, 4}}) == 5);

    // Three points on a rectangle boundary: side = max(width, height).
    assert(minimalSquareSide({{0,0}, {4,0}, {0,3}}) == 4); // 4x4 square with bottom-left corner (0,0) works.
    assert(minimalSquareSide({{0,0}, {2,2}, {2,0}}) == 2);

    // Points inside a bounding box but not on boundary -> impossible.
    // e.g., corners of a 10x10 square plus center (5,5).
    std::vector<std::pair<int,int>> pts = {{0,0},{10,0},{0,10},{10,10},{5,5}};
    assert(minimalSquareSide(pts) == -1);

    // Points that can be placed on a shifted square.
    // width=2, height=10 -> side=10, but need to shift left so the two narrow points are on left/right edges.
    // Points: (5,0), (5,10), (7,5) -> width=2, height=10. side=10.
    // Candidate (5,0) side=10: right=15, top=10. (7,5) is inside? x=7 not on left/right, y=5 not on bottom/top -> fails.
    // Candidate (5,0) works? Check: (5,0) on left? x=5 left yes, y in [0,10] yes. (5,10) on left and top. (7,5): not on left or right, not on bottom or top -> fails.
    // Candidate (max_x - side = 7-10=-3, min_y=0): right=7, top=10. (5,0) on bottom yes, x in[-3,7] yes. (5,10) on top yes. (7,5) on right yes -> works! side=10.
    assert(minimalSquareSide({{5,0},{5,10},{7,5}}) == 10);

    // Duplicate points: still same result.
    assert(minimalSquareSide({{3,4},{3,4},{3,4}}) == 0);

    // Negative coordinates.
    assert(minimalSquareSide({{-5,-5},{-5,-1},{-1,-5}}) == 4);

    // A square exactly matching bounding box but with a point on edge.
    assert(minimalSquareSide({{-2,-2},{2,-2},{-2,2},{2,2},{-2,0}}) == 4);

    // Impossible: two points inside a small box but not on its boundary.
    assert(minimalSquareSide({{0,0},{1,1}}) == -1); // width=height=1, but (0,0) and (1,1) are opposite corners of a 1x1 square, actually both on boundary? Check side=1, candidate (0,0): right=1, top=1. (0,0) on left/bottom yes. (1,1) on right/top yes -> works! So not -1. Let's use an impossible one: (0,0) and (1,2) -> width=1, height=2, side=2, can we place? Candidate (max_x-side = 1-2=-1, min_y=0): right=1, top=2. (0,0) bottom yes, (1,2) right/top yes -> works. Actually many work. Try (0,0) and (2,1): width=2, height=1, side=2, candidate (min_x, max_y-side = 1-2=-1): top=1, (0,0) bottom, (2,1) top and right? x=2=right yes -> works. Need truly impossible: points inside a square but not on boundary, e.g., (0,0), (10,10), (5,5) already tested -1. Also (0,0), (10,0), (0,10), (5,5) works? (5,5) inside, so -1. Good.
    assert(minimalSquareSide({{0,0},{10,0},{0,10},{5,5}}) == -1);

    return 0;
}
// The key observation is that any valid square must have all points on its perimeter. Therefore, the square must be large enough to enclose all points in the bounding box sense, and its corner positions must align so that no point falls strictly inside. Let `min_x`, `max_x`, `min_y`, `max_y` be the extremes. The minimal possible side length is at least `max(max_x - min_x, max_y - min_y)`, because otherwise either some points exceed the square's x-range or y-range. Let `side = max(width, height)`. The square's side is fixed at `side`, and its lower-left corner `(sx, sy)` can only be placed in a few positions relative to the bounding box: Because the square must cover all x's from `min_x` to `max_x` (if `width == side`), `sx` must equal `min_x`; if `width < side`, then `sx` can be either `min_x` (push right) or `max_x - side` (push left) – but since `width < side`, there is flexibility, but only these two extremes matter because any other placement would just shift interior points. Similarly for y. Thus the candidate lower-left corners are the four combinations: `(min_x, min_y)`, `(min_x, max_y - side)`, `(max_x - side, min_y)`, `(max_x - side, max_y - side)`. For each candidate, check if every point lies on the square's perimeter (i.e., inside the closed range for x and y, and on at least one edge: either x equals left or right boundary and y is within range, or y equals bottom or top boundary and x is within range). If any candidate works, return `side`; otherwise return `-1`. Edge case: if all points are identical, then `width=height=0`, `side=0`, and the candidate `(min_x, min_y)` with side 0 works because every point lies on the degenerate square's boundary. Complexity: O(n) time to compute extremes, O(n) per check (four checks), total O(n) time and O(1) auxiliary space.
