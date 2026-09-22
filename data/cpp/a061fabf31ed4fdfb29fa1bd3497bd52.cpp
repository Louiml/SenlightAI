Write a C++ function `bool segmentIntersectsRectangle(const double p1x, const double p1y, const double p2x, const double p2y, const double rectMinX, const double rectMinY, const double rectMaxX, const double rectMaxY)` that determines whether a line segment (defined by endpoints (p1x, p1y) and (p2x, p2y)) intersects a rectangle aligned with the coordinate axes, defined by its minimum and maximum x and y coordinates (rectMinX, rectMinY, rectMaxX, rectMaxY). The function should return `true` if any part of the segment lies inside or on the rectangle boundary, including cases where the segment is completely inside, touches the rectangle at a single point, or passes through it. The rectangle may have zero area (i.e., minX == maxX and/or minY == maxY), and the segment may be degenerate (both endpoints equal). You must handle horizontal, vertical, and diagonal segments correctly, including segments that are exactly on the rectangle boundary.

##
// The solution is based on the following geometric approach: a segment intersects an axis-aligned rectangle if and only if the projections of the segment onto the x-axis and y-axis both overlap with the corresponding projections of the rectangle, and additionally, the segment's line (extended infinitely) intersects the rectangle region. A simpler method is to:
//
// 1. Compute the range of x-values covered by the segment (minX = min(p1x, p2x), maxX = max(p1x, p2x)). Intersect this with the rectangle's x-range [rectMinX, rectMaxX]. If the intersection is empty, return false.
//
// 2. For the clipped x-range obtained, compute the corresponding y-values on the segment's line. If the segment is not vertical (dx ≠ 0), derive the line equation y = a*x + b; otherwise, the y-range of the segment is just [min(p1y,p2y), max(p1y,p2y)].
//
// 3. Clip the computed y-range with the rectangle's y-range [rectMinY, rectMaxY]. If the intersection is non-empty, the segment intersects the rectangle; otherwise false.
//
// This method correctly handles all cases:
// - Degenerate segments (p1 == p2) that may lie inside the rectangle.
// - Segments that touch the rectangle at one corner or edge.
// - Segments entirely inside the rectangle.
// - Horizontal, vertical, and diagonal segments.
// - Rectangles with zero width or height.
//
// Time complexity is O(1) since only a constant number of arithmetic operations and comparisons are performed. Space complexity is O(1).
//
// ##
#include <cmath>
#include <algorithm>

// Determine if a line segment from (p1x, p1y) to (p2x, p2y) intersects
// an axis-aligned rectangle defined by its minimum and maximum coordinates.
bool segmentIntersectsRectangle(
    double p1x, double p1y,
    double p2x, double p2y,
    double rectMinX, double rectMinY,
    double rectMaxX, double rectMaxY)
{
    // Ensure rectangle bounds are correctly ordered.
    if (rectMinX > rectMaxX) std::swap(rectMinX, rectMaxX);
    if (rectMinY > rectMaxY) std::swap(rectMinY, rectMaxY);

    // 1. Find the segment's x-range.
    double segMinX = std::min(p1x, p2x);
    double segMaxX = std::max(p1x, p2x);

    // Intersect with rectangle x-range.
    double clipMinX = std::max(segMinX, rectMinX);
    double clipMaxX = std::min(segMaxX, rectMaxX);

    if (clipMinX > clipMaxX) return false;

    // 2. Compute y-values on the segment line corresponding to clipMinX and clipMaxX.
    double segMinY, segMaxY;

    double dx = p2x - p1x;
    if (std::fabs(dx) > 1e-12) {
        // Non-vertical segment: y = a*x + b
        double a = (p2y - p1y) / dx;
        double b = p1y - a * p1x;
        segMinY = a * clipMinX + b;
        segMaxY = a * clipMaxX + b;
    } else {
        // Vertical segment: x is constant, y ranges from min to max endpoint.
        segMinY = std::min(p1y, p2y);
        segMaxY = std::max(p1y, p2y);
    }

    // Ensure y-range is ordered.
    if (segMinY > segMaxY) std::swap(segMinY, segMaxY);

    // Intersect with rectangle y-range.
    double clipMinY = std::max(segMinY, rectMinY);
    double clipMaxY = std::min(segMaxY, rectMaxY);

    return clipMinY <= clipMaxY;
}

##
#include <cassert>

int main() {
    // Basic intersection
    assert(segmentIntersectsRectangle(0, 0, 5, 5, 2, 2, 4, 4) == true);
    // Segment completely inside rectangle
    assert(segmentIntersectsRectangle(1, 1, 2, 2, 0, 0, 3, 3) == true);
    // Segment outside rectangle
    assert(segmentIntersectsRectangle(-2, -2, -1, -1, 0, 0, 3, 3) == false);
    // Segment touching rectangle edge
    assert(segmentIntersectsRectangle(0, 0, 2, 0, 1, -1, 3, 1) == true);
    // Segment touching rectangle corner
    assert(segmentIntersectsRectangle(0, 0, 1, 1, 1, 1, 2, 2) == true);
    // Horizontal segment along rectangle boundary
    assert(segmentIntersectsRectangle(0, 1, 3, 1, 1, 1, 2, 2) == true);
    // Vertical segment through rectangle
    assert(segmentIntersectsRectangle(1, -1, 1, 3, 0, 0, 2, 2) == true);
    // Degenerate segment (point) inside rectangle
    assert(segmentIntersectsRectangle(2, 2, 2, 2, 0, 0, 3, 3) == true);
    // Degenerate rectangle (point) pierced by segment
    assert(segmentIntersectsRectangle(-1, 0, 1, 0, 0, 0, 0, 0) == true);
    // Segment passing over rectangular region without crossing x overlap (anti-gravity case)
    assert(segmentIntersectsRectangle(0, 0, 0, 2, -1, 5, 1, 6) == false);
}
