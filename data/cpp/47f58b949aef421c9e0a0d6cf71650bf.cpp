/*
Write a C++ function named `triangleRadiusDifference` that takes two `double` parameters — the length of a triangle's opposite side (relative to a corner) and the triangle's corner angle in degrees — and returns the difference between the triangle's circumradius (outside radius) and inradius (inside radius). The function must perform the following steps: (1) validate that the opposite side is positive and the angle is strictly between 0 and 180 degrees, returning `-1.0` for invalid inputs; (2) compute the hypotenuse using the sine rule: `hypotenuse = opposite / sin(angleRadians)`; (3) compute the adjacent side using the Pythagorean theorem: `adjacent = sqrt(hypotenuse² - opposite²)`; (4) compute the inradius as `(opposite + adjacent - hypotenuse) / 2` (valid for a right triangle); (5) compute the circumradius as `hypotenuse / 2`; and (6) return the absolute difference between circumradius and inradius. The function must be `const`-correct, use `cmath` functions, and handle edge cases where the inputs produce a degenerate triangle (e.g., angle near 0 or 180) gracefully without division by zero.
*/

#include <cmath>
#include <cstdlib>

// Returns the absolute difference between circumradius and inradius
// of a right triangle given the opposite side (relative to a corner)
// and the corner angle in degrees.
// Returns -1.0 for invalid inputs (non-positive side, angle not in (0,180)).
double triangleRadiusDifference(double oppositeSide, double cornerAngleDegrees) {
    // Validate inputs
    if (oppositeSide <= 0.0 || cornerAngleDegrees <= 0.0 || cornerAngleDegrees >= 180.0) {
        return -1.0;
    }

    // Convert angle to radians
    const double pi = 3.14159265358979323846;
    double angleRadians = cornerAngleDegrees * pi / 180.0;

    // Compute hypotenuse using sine rule: opposite / sin(angle)
    double sinAngle = std::sin(angleRadians);
    // Guard against division by zero (angle near 0 or 180 already validated)
    if (std::abs(sinAngle) < 1e-12) {
        return -1.0;
    }
    double hypotenuse = oppositeSide / sinAngle;

    // Adjacent side via Pythagorean theorem: sqrt(h² - o²)
    double adjacent = std::sqrt(hypotenuse * hypotenuse - oppositeSide * oppositeSide);

    // Inradius for a right triangle: (leg1 + leg2 - hypotenuse) / 2
    double inradius = (oppositeSide + adjacent - hypotenuse) / 2.0;

    // Circumradius for a right triangle: hypotenuse / 2
    double circumradius = hypotenuse / 2.0;

    // Return absolute difference
    return std::abs(circumradius - inradius);
}

#include <cassert>
#include <cmath>

int main() {
    // Valid: 3-4-5 right triangle, angle opposite 3 is arcsin(3/5) ≈ 36.87°
    double angle = std::asin(3.0/5.0) * 180.0 / 3.14159265358979323846;
    double result = triangleRadiusDifference(3.0, angle);
    // For 3-4-5: circumradius = 2.5, inradius = (3+4-5)/2 = 1, difference = 1.5
    assert(std::abs(result - 1.5) < 1e-9);

    // Valid: isosceles right triangle, leg = 1, angle = 45°, hypotenuse = sqrt(2)
    double angle45 = 45.0;
    double result2 = triangleRadiusDifference(1.0, angle45);
    // circumradius = sqrt(2)/2 ≈ 0.7071, inradius = (1 + 1 - sqrt(2))/2 ≈ 0.2929, difference ≈ 0.4142
    assert(std::abs(result2 - 0.414213562373) < 1e-9);

    // Valid: angle = 90°, opposite = hypotenuse (since sin90=1), adjacent = 0 (degenerate)
    // Inradius = (opp + 0 - opp)/2 = 0, circumradius = opp/2, difference = opp/2
    double result3 = triangleRadiusDifference(5.0, 90.0);
    assert(std::abs(result3 - 2.5) < 1e-9);

    // Invalid: negative side
    assert(triangleRadiusDifference(-1.0, 45.0) == -1.0);

    // Invalid: zero side
    assert(triangleRadiusDifference(0.0, 45.0) == -1.0);

    // Invalid: angle 0
    assert(triangleRadiusDifference(1.0, 0.0) == -1.0);

    // Invalid: angle 180
    assert(triangleRadiusDifference(1.0, 180.0) == -1.0);

    // Invalid: angle negative
    assert(triangleRadiusDifference(1.0, -10.0) == -1.0);

    // Invalid: angle > 180
    assert(triangleRadiusDifference(1.0, 200.0) == -1.0);

    // Valid: very small angle, should return positive value (not -1)
    assert(triangleRadiusDifference(1.0, 0.001) > 0.0);

    return 0;
}

// The solution involves basic right‑triangle trigonometry. Given an opposite side and the angle at that corner, the triangle is assumed to be right‑angled, with the opposite side adjacent to the given angle and the right angle at the other end. The sine rule gives the hypotenuse as `opposite / sin(angle)`. Then the adjacent side is derived from the Pythagorean theorem. For a right triangle, the inradius formula is `(sum of legs - hypotenuse)/2`, which derives from the area formula `Area = r * semiperimeter` and `Area = (leg1 * leg2)/2`. The circumradius of a right triangle is always half the hypotenuse. The difference is then `|hypotenuse/2 - inradius|`. Edge cases: if the opposite side is zero or negative, or the angle is not in (0,180), return `-1.0` to indicate an error. Also, if the angle is exactly 0 or 180 degrees, `sin(angle) = 0`, which would cause division by zero; the validation catches this. If the angle is 90 degrees, the triangle is isosceles right, and the formulas still work (adjacent equals opposite). For very small angles (e.g., 0.001°), the hypotenuse becomes huge, but the calculations remain numerically stable. Time complexity is O(1), space complexity O(1).
