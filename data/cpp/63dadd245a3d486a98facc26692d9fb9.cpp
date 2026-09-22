/*
Write a C++ function that takes three floating-point numbers `a`, `b`, and `d` representing the coordinates of a 2D point and a rotation angle in degrees. The function should return a `std::pair<double, double>` containing the coordinates of the point after rotating it counterclockwise around the origin by `d` degrees. The rotation must be mathematically exact using the standard rotation matrix formula: `new_x = a*cos(theta) - b*sin(theta)` and `new_y = a*sin(theta) + b*cos(theta)`, where `theta` is `d` converted to radians. The function must return results with full double precision without any unnecessary rounding. Assume that the input values are finite real numbers, and the angle can be any real value, including negative or greater than 360 degrees. The function should be standalone, independent of any main program, and should not print anything.
*/
#include <utility>
#include <cmath>

// Rotate point (a, b) counterclockwise by d degrees around the origin.
// Returns a pair (new_x, new_y) with full double precision.
std::pair<double, double> rotatePoint(double a, double b, double d) {
    const double PI = std::acos(-1.0);
    const double theta = d * PI / 180.0;
    const double cosTheta = std::cos(theta);
    const double sinTheta = std::sin(theta);

    const double newX = a * cosTheta - b * sinTheta;
    const double newY = a * sinTheta + b * cosTheta;

    return std::make_pair(newX, newY);
}
int main() {
    // Zero rotation should return original coordinates
    auto p1 = rotatePoint(3.0, 4.0, 0.0);
    assert(std::abs(p1.first - 3.0) < 1e-9);
    assert(std::abs(p1.second - 4.0) < 1e-9);

    // 90 degrees counterclockwise: (1,0) -> (0,1)
    auto p2 = rotatePoint(1.0, 0.0, 90.0);
    assert(std::abs(p2.first - 0.0) < 1e-9);
    assert(std::abs(p2.second - 1.0) < 1e-9);

    // 180 degrees: (2,-1) -> (-2,1)
    auto p3 = rotatePoint(2.0, -1.0, 180.0);
    assert(std::abs(p3.first - (-2.0)) < 1e-9);
    assert(std::abs(p3.second - 1.0) < 1e-9);

    // Negative angle: -90 degrees: (0,5) -> (5,0)
    auto p4 = rotatePoint(0.0, 5.0, -90.0);
    assert(std::abs(p4.first - 5.0) < 1e-9);
    assert(std::abs(p4.second - 0.0) < 1e-9);

    // Angle 360 degrees returns original: (3.5,-2.25) unchanged
    auto p5 = rotatePoint(3.5, -2.25, 360.0);
    assert(std::abs(p5.first - 3.5) < 1e-9);
    assert(std::abs(p5.second - (-2.25)) < 1e-9);

    // 45 degrees: (1,1) -> (0, sqrt(2))
    auto p6 = rotatePoint(1.0, 1.0, 45.0);
    assert(std::abs(p6.first - 0.0) < 1e-9);
    assert(std::abs(p6.second - std::sqrt(2.0)) < 1e-9);

    // Check with a non-trivial case: (3,4) rotated 30 degrees
    // Exact values: x = 3*cos30 - 4*sin30 = 3*(√3/2) - 4*(0.5) = (3√3/2 - 2)
    // y = 3*sin30 + 4*cos30 = 3*(0.5) + 4*(√3/2) = (1.5 + 2√3)
    auto p7 = rotatePoint(3.0, 4.0, 30.0);
    double expectedX = 3.0 * std::sqrt(3.0)/2.0 - 2.0;
    double expectedY = 1.5 + 2.0 * std::sqrt(3.0);
    assert(std::abs(p7.first - expectedX) < 1e-9);
    assert(std::abs(p7.second - expectedY) < 1e-9);

    // Using negative coordinates: (-1, -2) rotated 270 deg -> (-2,1)
    auto p8 = rotatePoint(-1.0, -2.0, 270.0);
    assert(std::abs(p8.first - (-2.0)) < 1e-9);
    assert(std::abs(p8.second - 1.0) < 1e-9);

    return 0;
}
// The solution directly implements the 2D rotation matrix. Given a point `(a, b)` and a counterclockwise rotation angle `θ` (in degrees), the new coordinates are `(a cos θ - b sin θ, a sin θ + b cos θ)`. The angle must be converted from degrees to radians by multiplying by `π/180`. Use `M_PI` from `<cmath>` (or `std::numbers::pi` if using C++20). Important edge cases: (1) the angle may be zero, in which case the output should exactly equal the input; (2) negative angles simply rotate clockwise but the formula still works; (3) angles larger than 360 degrees are valid due to periodicity of trigonometric functions; (4) values like `a=0` or `b=0` produce normal results. The function should compute using `std::cos` and `std::sin` with a `const double` for the conversion factor. Time complexity is O(1), space complexity O(1). No special error handling is needed for finite inputs; the only subtlety is ensuring the constant for π is available. For portability, define `const double PI = std::acos(-1.0);` or use `M_PI` if available, but to be self-contained, use `std::acos(-1.0)`.
