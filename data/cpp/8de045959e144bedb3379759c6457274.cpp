/*
Write a C++ function `int totalCoveredArea(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)` that receives coordinates of two axis-aligned rectangles on a 2D plane. Each rectangle is defined by its bottom-left corner `(x1, y1)` and top-right corner `(x2, y2)`, with the guarantee that `x1 < x2` and `y1 < y2` for each rectangle. The function must return the total area covered by the union of the two rectangles, i.e., the sum of their individual areas minus the area of any overlapping region. Overlap occurs only if the rectangles intersect in both the x and y dimensions; if they only touch at edges or corners, the overlap area is zero. All coordinates and computed areas fit within a 32-bit signed integer range, but intermediate computations of width and height may be negative if there is no overlap—handle that by treating negative overlap width or height as zero. The function should be pure (no side effects), use `const` references for inputs (though inputs are passed by value here, so no need), and involve no floating-point arithmetic.
*/

#include <algorithm> // for std::max, std::min

// Compute the total area covered by the union of two axis-aligned rectangles.
// Each rectangle is given by its bottom-left (x1, y1) and top-right (x2, y2).
// Assumes x1 < x2 and y1 < y2 for each rectangle.
// Returns the sum of the two areas minus the area of their overlapping region.
int totalCoveredArea(int ax1, int ay1, int ax2, int ay2,
                     int bx1, int by1, int bx2, int by2) {
    // Area of each rectangle (guaranteed non-negative because x1<x2 and y1<y2)
    const int areaA = (ax2 - ax1) * (ay2 - ay1);
    const int areaB = (bx2 - bx1) * (by2 - by1);

    // Overlap width and height; if negative, there is no overlap.
    const int overlapWidth = std::min(ax2, bx2) - std::max(ax1, bx1);
    const int overlapHeight = std::min(ay2, by2) - std::max(ay1, by1);

    // Clamp negative values to zero to represent no overlap.
    const int safeWidth = std::max(0, overlapWidth);
    const int safeHeight = std::max(0, overlapHeight);

    const int overlapArea = safeWidth * safeHeight;

    return areaA + areaB - overlapArea;
}

#include <cassert>

int main() {
    // Non-overlapping rectangles
    assert(totalCoveredArea(0, 0, 1, 1, 2, 2, 3, 3) == 2);

    // Fully overlapping (identical rectangles)
    assert(totalCoveredArea(0, 0, 2, 2, 0, 0, 2, 2) == 4);

    // Partial overlap in both dimensions
    assert(totalCoveredArea(-3, -3, 3, 3, -1, -1, 1, 1) == 36); // 36 + 4 - 4 = 36

    // Touching along an edge (no area overlap)
    assert(totalCoveredArea(0, 0, 2, 2, 2, 0, 4, 2) == 8);

    // Touching at a corner (no area overlap)
    assert(totalCoveredArea(0, 0, 1, 1, 1, 1, 2, 2) == 2);

    // One rectangle completely inside the other
    assert(totalCoveredArea(0, 0, 10, 10, 2, 2, 3, 3) == 100);

    // Large gap in both axes
    assert(totalCoveredArea(-5, -5, -4, -4, 5, 5, 6, 6) == 2);

    // Single zero-area? (not allowed by spec, but testing robustness)
    // assert(totalCoveredArea(0,0,0,0,1,1,2,2) == 0); // Would fail due to x1==x2, but we ignore

    // Asymmetric overlap
    assert(totalCoveredArea(1, 1, 5, 5, 3, 2, 7, 6) == 16 + 16 - 8); // 24

    // Negative coordinates and partial overlap
    assert(totalCoveredArea(-4, -2, 2, 4, -1, -3, 3, 1) == 24 + 16 - 4); // 36
}

// The solution computes the area of the first rectangle as `(ax2 - ax1) * (ay2 - ay1)` and the second as `(bx2 - bx1) * (by2 - by1)`. The overlapping rectangle, if it exists, is defined by the maximum of the left edges (`max(ax1, bx1)`) as its left, the minimum of the right edges (`min(ax2, bx2)`) as its right, the maximum of the bottom edges (`max(ay1, by1)`) as its bottom, and the minimum of the top edges (`min(ay2, by2)`) as its top. The overlap width is `min(ax2, bx2) - max(ax1, bx1)` and overlap height is `min(ay2, by2) - max(ay1, by1)`. If either is negative, there is no overlap, so we clamp both to zero using `max(0, width)` and `max(0, height)`. The overlap area is the product of these clamped values. The total covered area is `area1 + area2 - overlapArea`. Edge cases include rectangles that touch along an edge (overlap width or height is zero, product zero), rectangles that share only a corner (both width and height zero), and rectangles far apart (negative width or height, clamped to zero). Time complexity is O(1) and space complexity is O(1), as only a few integer variables are used.
