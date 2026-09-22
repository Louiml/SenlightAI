// Write a standalone C++ function named `segmentsIntersect` that determines whether two 2D line segments intersect. The function should accept eight doubles representing the coordinates of the two segments: `(x1, y1, x2, y2)` for the first segment and `(x3, y3, x4, y4)` for the second segment. It must return a `bool`: `true` if the segments intersect at any point (including endpoints and collinear overlapping cases), `false` otherwise. Implement the algorithm using orientation tests and on-segment checks without relying on any external geometry libraries. Use exact arithmetic with `double` and handle edge cases such as collinear segments that overlap only partially, segments that just touch at an endpoint, and nearly degenerate cases. The function must be self-contained, including all necessary headers, and should not include a `main` function.
// The solution uses the standard computational geometry approach: for two segments `AB` and `CD`, compute four orientation values using the cross product to determine the sign of the turn from one point to another. Define `orient(P, Q, R)` as the sign of `(Q.x - P.x)*(R.y - P.y) - (Q.y - P.y)*(R.x - P.x)`. Positive indicates counterclockwise, negative clockwise, zero collinear.
//
// For the general case, the segments intersect if the endpoints of each segment lie on opposite sides of the other segment. That is, `orient(A, B, C)` and `orient(A, B, D)` have opposite signs, and `orient(C, D, A)` and `orient(C, D, B)` have opposite signs. When any orientation is zero, the point is collinear with the segment, and we must check if the point lies within the segment's bounding box (both x and y coordinates between the segment's endpoints, inclusive). Overlap occurs if any collinear endpoint is on the other segment, or if the segments are collinear and their projections overlap. The bounding-box check handles this.
//
// Time complexity is O(1) because we only perform constant arithmetic operations. Space complexity is O(1) aside from temporary variables. Edge cases include: segments that share exactly one endpoint (orientation zero for that endpoint, bounding-box check passes), fully overlapping collinear segments, partially overlapping collinear segments, and cases where segments are vertical/horizontal (bounding-box check still works). The code uses `std::min`/`std::max` and works with `double` precision; for robustness in a production setting one might use epsilon comparisons, but the task allows simple equality.
#include <algorithm>
#include <cmath>

/**
 * Determine if two 2D line segments intersect.
 * @param x1,y1,x2,y2  First segment endpoints.
 * @param x3,y3,x4,y4  Second segment endpoints.
 * @return true if segments intersect (including touching at endpoints or overlapping), false otherwise.
 */
bool segmentsIntersect(double x1, double y1, double x2, double y2,
                       double x3, double y3, double x4, double y4) {
    // Orientation test: returns sign of cross product (Q-P) x (R-P)
    // >0 counterclockwise, <0 clockwise, =0 collinear
    auto orient = [](double px, double py, double qx, double qy, double rx, double ry) -> int {
        double value = (qx - px) * (ry - py) - (qy - py) * (rx - px);
        if (value > 0.0) return 1;
        if (value < 0.0) return -1;
        return 0;
    };

    // Check if point P lies on segment MN (assuming collinear), using bounding box
    auto onSegment = [](double mx, double my, double nx, double ny, double px, double py) -> bool {
        return (px >= std::min(mx, nx) && px <= std::max(mx, nx) &&
                py >= std::min(my, ny) && py <= std::max(my, ny));
    };

    int o1 = orient(x1, y1, x2, y2, x3, y3);
    int o2 = orient(x1, y1, x2, y2, x4, y4);
    int o3 = orient(x3, y3, x4, y4, x1, y1);
    int o4 = orient(x3, y3, x4, y4, x2, y2);

    // General case: opposite signs for each segment
    if (o1 != o2 && o3 != o4) return true;

    // Special cases: collinear and on-segment
    if (o1 == 0 && onSegment(x1, y1, x2, y2, x3, y3)) return true;
    if (o2 == 0 && onSegment(x1, y1, x2, y2, x4, y4)) return true;
    if (o3 == 0 && onSegment(x3, y3, x4, y4, x1, y1)) return true;
    if (o4 == 0 && onSegment(x3, y3, x4, y4, x2, y2)) return true;

    return false;
}
#include <cassert>
#include <cmath>

// Assume the solution function is defined above
int main() {
    // Basic intersection
    assert(segmentsIntersect(0,0, 2,2, 0,2, 2,0) == true);
    // No intersection
    assert(segmentsIntersect(0,0, 1,1, 2,2, 3,3) == false);
    // Sharing an endpoint
    assert(segmentsIntersect(0,0, 1,1, 1,1, 2,0) == true);
    // Collinear overlapping
    assert(segmentsIntersect(0,0, 2,2, 1,1, 3,3) == true);
    // Collinear non-overlapping
    assert(segmentsIntersect(0,0, 1,1, 2,2, 3,3) == false);
    // Vertical and horizontal crossing
    assert(segmentsIntersect(0,-1, 0,1, -1,0, 1,0) == true);
    // Touching at just one point (non-collinear)
    assert(segmentsIntersect(0,0, 1,1, 1,1, 2,0) == true);
    // Parallel non-intersecting
    assert(segmentsIntersect(0,0, 1,0, 0,1, 1,1) == false);
    // Degenerate segment (point) on another segment
    assert(segmentsIntersect(0,0, 2,2, 1,1, 1,1) == true);
    // Degenerate point not on segment
    assert(segmentsIntersect(0,0, 2,2, 3,3, 3,3) == false);
    return 0;
}
