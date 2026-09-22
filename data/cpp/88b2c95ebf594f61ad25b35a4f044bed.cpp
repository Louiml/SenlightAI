// Write a C++ function `triangleArea` that takes six `double` values representing the coordinates of three points in the plane (`ax, ay, bx, by, cx, cy`) and returns a `double` value: the area of the triangle formed by these points, rounded to two decimal places, or `-1.0` if the points do not form a valid non-degenerate triangle (i.e., if any side length is zero, or the triangle inequality is violated). The function must handle floating-point inputs correctly, including very small or large coordinates, and must not rely on global state or external I/O. The area should be computed using Heron’s formula after verifying triangle validity. If the triangle is invalid, return `-1.0`; otherwise, return the area rounded to two decimal places (e.g., using `std::round(value * 100.0) / 100.0`).

// The approach is straightforward: compute the three side lengths using the Euclidean distance formula. Then check validity: a triangle is degenerate if any side length is exactly `0.0` (or effectively non-positive due to arithmetic, but here exact comparison is fine) or if the sum of any two sides is less than or equal to the third side. If invalid, return `-1.0`. Otherwise, apply Heron’s formula: `s = (a+b+c)/2`, area = `sqrt(s*(s-a)*(s-b)*(s-c))`. Because the inputs are `double`, we must ensure that the rounding to two decimal places is done correctly; using `std::round` is appropriate. The function is `const`-correct by taking parameters by value and not modifying them. Edge cases include duplicate points (side length zero), collinear points (sum of two sides equals the third, which violates the strict > condition), and extremely large coordinates that might cause overflow in Heron’s formula—but with typical inputs this is not a concern for a teaching task. Time complexity is O(1) since only constant arithmetic is performed; space complexity is O(1).

#include <cmath>
#include <algorithm>

// Computes the area of a triangle from three points.
// Returns -1.0 if the points do not form a valid non-degenerate triangle.
// Otherwise returns the area rounded to two decimal places.
double triangleArea(double ax, double ay, double bx, double by, double cx, double cy) {
    // Compute side lengths using Euclidean distance.
    double d1 = std::sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
    double d2 = std::sqrt((bx - cx) * (bx - cx) + (by - cy) * (by - cy));
    double d3 = std::sqrt((ax - cx) * (ax - cx) + (ay - cy) * (ay - cy));

    // Check for degenerate or zero-length sides.
    if (d1 <= 0.0 || d2 <= 0.0 || d3 <= 0.0) {
        return -1.0;
    }
    // Triangle inequality: sum of any two sides must be strictly greater than the third.
    if (d1 + d2 <= d3 || d1 + d3 <= d2 || d2 + d3 <= d1) {
        return -1.0;
    }

    // Heron's formula.
    double semiperimeter = (d1 + d2 + d3) / 2.0;
    double area = std::sqrt(semiperimeter * (semiperimeter - d1) * (semiperimeter - d2) * (semiperimeter - d3));

    // Round to two decimal places.
    return std::round(area * 100.0) / 100.0;
}

#include <cassert>
#include <cmath>

// Re-declare the function here for the test file (or include the header).
double triangleArea(double ax, double ay, double bx, double by, double cx, double cy);

int main() {
    // Right triangle with legs 3 and 4, area = 6.0
    assert(std::fabs(triangleArea(0.0, 0.0, 3.0, 0.0, 0.0, 4.0) - 6.0) < 1e-9);

    // Equilateral triangle with side 2, area = sqrt(3) ≈ 1.73205, rounded = 1.73
    assert(std::fabs(triangleArea(0.0, 0.0, 2.0, 0.0, 1.0, std::sqrt(3.0)) - 1.73) < 1e-9);

    // Points collinear (degenerate) → invalid
    assert(triangleArea(1.0, 1.0, 2.0, 2.0, 3.0, 3.0) == -1.0);

    // Duplicate points → invalid
    assert(triangleArea(0.0, 0.0, 0.0, 0.0, 1.0, 1.0) == -1.0);

    // Triangle inequality violated (e.g., side lengths 1,2,4) – use coordinates (0,0),(1,0),(0,4)
    assert(triangleArea(0.0, 0.0, 1.0, 0.0, 0.0, 4.0) == -1.0);

    // A small triangle with coordinates (0,0), (1,0), (0,1) area = 0.5 → rounded 0.50
    assert(std::fabs(triangleArea(0.0, 0.0, 1.0, 0.0, 0.0, 1.0) - 0.50) < 1e-9);

    return 0;
}
