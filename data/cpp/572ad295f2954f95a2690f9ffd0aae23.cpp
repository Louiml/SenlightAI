Write a C++ function named `rectangleUnionArea` that takes eight integer parameters representing the bottom-left and top-right corners of two axis-aligned rectangles: `(A, B, C, D)` for the first rectangle and `(E, F, G, H)` for the second, where `A < C`, `B < D`, `E < G`, and `F < H`. The function must return the total area covered by the union of the two rectangles (i.e., the sum of their individual areas minus the area of their intersection, if any). The rectangles may overlap partially, fully contain one another, or be completely disjoint. Coordinates can be negative, zero, or positive, and the result is guaranteed to fit within a 32-bit signed integer. The function must be standalone, const-correct (parameters and logic should not modify any input), and include no global state.
// The solution computes the union area as `area1 + area2 - overlapArea`. The individual areas are direct products of their side lengths: `(C - A) * (D - B)` and `(G - E) * (H - F)`. The overlap region, if it exists, is also an axis-aligned rectangle. Its left boundary is the maximum of the two left edges: `max(A, E)`. Its right boundary is the minimum of the two right edges: `min(C, G)`. If this left boundary is less than the right boundary, there is positive horizontal overlap width = `right - left`; otherwise, the width is zero. Similarly, the bottom boundary is `max(B, F)`, top is `min(D, H)`, and the vertical height is positive only if `top > bottom`. The overlap area is the product of the width and height if both are positive, otherwise zero. This avoids negative dimensions for disjoint rectangles. Edge cases include identical rectangles (overlap equals either rectangle), one rectangle fully inside the other (overlap = smaller area), and no overlap (width or height = 0). The algorithm runs in constant time `O(1)` with constant space `O(1)`.
#include <algorithm>

// Compute the total area covered by the union of two axis-aligned rectangles.
// Rectangles are defined by bottom-left (A, B) and top-right (C, D) for the first,
// and bottom-left (E, F) and top-right (G, H) for the second.
int rectangleUnionArea(int A, int B, int C, int D, int E, int F, int G, int H) {
    // Area of each rectangle individually
    int area1 = (C - A) * (D - B);
    int area2 = (G - E) * (H - F);

    // Compute overlap width; zero if no horizontal overlap
    int left = std::max(A, E);
    int right = std::min(C, G);
    int overlapWidth = (right > left) ? (right - left) : 0;

    // Compute overlap height; zero if no vertical overlap
    int bottom = std::max(B, F);
    int top = std::min(D, H);
    int overlapHeight = (top > bottom) ? (top - bottom) : 0;

    int overlapArea = overlapWidth * overlapHeight;

    return area1 + area2 - overlapArea;
}
#include <cassert>

int main() {
    // No overlap: rectangles far apart
    assert(rectangleUnionArea(0, 0, 2, 2, 3, 3, 5, 5) == 4 + 4);

    // Fully overlapping (identical rectangles)
    assert(rectangleUnionArea(1, 1, 4, 4, 1, 1, 4, 4) == 9);

    // One rectangle fully inside the other
    assert(rectangleUnionArea(0, 0, 10, 10, 2, 2, 5, 5) == 100);

    // Partial overlap: width=2, height=2 overlap area=4
    assert(rectangleUnionArea(0, 0, 4, 4, 2, 2, 6, 6) == 16 + 16 - 4);

    // Negative coordinates, partial overlap
    assert(rectangleUnionArea(-3, -3, 2, 2, -1, -1, 3, 3) == 25 + 16 - 9);

    // Edge touching but not overlapping (zero width)
    assert(rectangleUnionArea(0, 0, 2, 2, 2, 0, 4, 2) == 4 + 4);

    // Corner touching (zero height and width)
    assert(rectangleUnionArea(0, 0, 1, 1, 1, 1, 2, 2) == 1 + 1);

    // Large coordinates, non-overlapping
    assert(rectangleUnionArea(-100, -100, 0, 0, 1, 1, 101, 101) == 10000 + 10000);

    // One rectangle has zero area? But spec says valid, so skip. 
    // Overlap with negative dimensions not possible due to spec.

    // Horizontal and vertical both overlap but not fully
    assert(rectangleUnionArea(0, 0, 3, 3, 1, 2, 5, 6) == 9 + 8 - 2);
}
