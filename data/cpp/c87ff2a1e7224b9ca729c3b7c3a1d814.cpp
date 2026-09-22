// Write a C++ function named `areCollinear` that takes three 2D points, each defined by float coordinates `(x1, y1)`, `(x2, y2)`, and `(x3, y3)`, and returns a boolean value indicating whether the three points are collinear (lie on a straight line) or not. The function should handle the special cases where the denominator might be zero (i.e., vertical line segments) by checking equality of x-coordinates properly, and it should also account for potential floating-point precision issues by using a small tolerance threshold (e.g., `1e-6`) instead of exact equality. The function should be self-contained, with no user input/output inside it, and should be suitable for use in a larger program. The function signature must be: `bool areCollinear(float x1, float y1, float x2, float y2, float x3, float y3);`

// The standard approach is to compare the slopes of two line segments formed by the three points: slope between point 1 and 2, and slope between point 2 and 3. If the slopes are equal, the points are collinear. However, using slope `(y2-y1)/(x2-x1)` fails when any denominator is zero (vertical lines). To avoid division, use the cross-product (determinant) method: points are collinear if the area of the triangle they form is zero: `(x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1) == 0`. Since floating-point arithmetic may produce tiny non-zero values for collinear points due to rounding, compare the absolute value of this expression against a small tolerance like `1e-6`. This works for all cases including vertical lines, duplicate points, and horizontal lines. Edge cases: if all three points are identical, the triangle area is zero and they are collinear; if two points are identical and the third is different, the formula also gives zero (since the vector from the identical points is zero), which is correct because any line through the duplicate point and the third point contains all. Time complexity: O(1) constant time; space complexity: O(1) constant space.

#include <cmath>

// Determine whether three points are collinear using the determinant (cross product) method.
// Returns true if the area of the triangle formed by the three points is (near) zero.
bool areCollinear(float x1, float y1, float x2, float y2, float x3, float y3) {
    // Compute the cross product (determinant) of vectors (x2-x1, y2-y1) and (x3-x1, y3-y1)
    float cross = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
    // Use a small tolerance to account for floating-point precision errors.
    return std::abs(cross) < 1e-6f;
}

#include <cassert>
#include <cmath>

int main() {
    // Simple horizontal line
    assert(areCollinear(0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 0.0f) == true);
    // Simple vertical line
    assert(areCollinear(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f) == true);
    // Diagonal line
    assert(areCollinear(1.0f, 1.0f, 2.0f, 2.0f, 3.0f, 3.0f) == true);
    // Non-collinear points
    assert(areCollinear(0.0f, 0.0f, 1.0f, 1.0f, 2.0f, 0.0f) == false);
    // Two identical points
    assert(areCollinear(5.0f, 5.0f, 5.0f, 5.0f, 7.0f, 8.0f) == true);
    // All identical points
    assert(areCollinear(2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f) == true);
    // Near-collinear within tolerance
    assert(areCollinear(0.0f, 0.0f, 1.0f, 1.0f, 2.0f, 2.0000005f) == true);
    // Clearly non-collinear with large separation
    assert(areCollinear(0.0f, 0.0f, 1.0f, 1.0f, 2.0f, 3.0f) == false);
    // Horizontal line with negative coordinates
    assert(areCollinear(-1.0f, -2.0f, 0.0f, -2.0f, 3.0f, -2.0f) == true);
    // Vertical line with negative coordinates
    assert(areCollinear(-1.0f, -2.0f, -1.0f, 0.0f, -1.0f, 5.0f) == true);
}
