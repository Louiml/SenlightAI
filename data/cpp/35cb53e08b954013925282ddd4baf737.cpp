/*
Write a C++ function named `poiseuilleVelocityComponent` that, given a point in 2D space (as a `std::pair<double,double>` or a small struct), a 2D unit vector for the tube axis, a 2D center point on the axis, a tube radius, a maximum velocity (positive scalar), and a velocity component index (0 for x, 1 for y), returns the corresponding velocity component at that point according to the Poiseuille flow profile: \( v(r) = V_{max} \cdot (1 - r^2/R^2) \) directed along the tube axis, where \( r \) is the perpendicular distance from the point to the tube axis. The function must compute the perpendicular radius using vector projection, handle the case where the point lies exactly on the axis (r=0) yielding maximum velocity, and correctly handle negative components. The function should be pure (no side effects) and const-correct where applicable. Assume inputs are valid (axis is unit length, radius positive, component index 0 or 1).
*/

#include <cmath>
#include <utility>

// Compute a velocity component of Poiseuille flow in a tube.
// point: 2D point coordinates (x, y)
// axis: unit vector along tube axis (ux, uy)
// center: point on the tube axis (cx, cy)
// tubeRadius: radius of the tube (positive)
// maxVel: maximum velocity at the axis (positive)
// comp: 0 for x-component, 1 for y-component
double poiseuilleVelocityComponent(
    const std::pair<double,double>& point,
    const std::pair<double,double>& axis,
    const std::pair<double,double>& center,
    double tubeRadius,
    double maxVel,
    int comp)
{
    // Vector from center to point
    double dx = point.first - center.first;
    double dy = point.second - center.second;

    // Project point onto axis: t = dot(d, axis)
    double t = dx * axis.first + dy * axis.second;

    // Closest point on axis
    double closestX = center.first + t * axis.first;
    double closestY = center.second + t * axis.second;

    // Perpendicular vector from axis to point
    double perpX = point.first - closestX;
    double perpY = point.second - closestY;

    // Radius = length of perpendicular vector
    double radius = std::sqrt(perpX * perpX + perpY * perpY);

    // Velocity magnitude
    double magnitude = maxVel * (1.0 - (radius * radius) / (tubeRadius * tubeRadius));

    // Velocity vector = axis * magnitude
    double vx = axis.first * magnitude;
    double vy = axis.second * magnitude;

    // Return requested component
    return (comp == 0) ? vx : vy;
}

#include <cassert>
#include <cmath>
#include <utility>

// Declaration of the tested function (from solution)
double poiseuilleVelocityComponent(
    const std::pair<double,double>& point,
    const std::pair<double,double>& axis,
    const std::pair<double,double>& center,
    double tubeRadius,
    double maxVel,
    int comp);

int main() {
    // Axis along x-direction, center at origin, radius 2, maxVel 5
    std::pair<double,double> axis = {1.0, 0.0};
    std::pair<double,double> center = {0.0, 0.0};
    double R = 2.0;
    double Vmax = 5.0;

    // Point on axis -> max velocity in x direction
    double v1 = poiseuilleVelocityComponent({0.0, 0.0}, axis, center, R, Vmax, 0);
    assert(std::abs(v1 - 5.0) < 1e-12);

    // Point at distance 1 from axis -> velocity = Vmax*(1 - 1/4) = 3.75
    double v2 = poiseuilleVelocityComponent({0.0, 1.0}, axis, center, R, Vmax, 0);
    assert(std::abs(v2 - 3.75) < 1e-12);

    // Point at distance 2 (on wall) -> velocity = 0
    double v3 = poiseuilleVelocityComponent({0.0, 2.0}, axis, center, R, Vmax, 0);
    assert(std::abs(v3) < 1e-12);

    // Point off-axis in negative y direction, x-component unchanged
    double v4 = poiseuilleVelocityComponent({2.0, -0.5}, axis, center, R, Vmax, 0);
    double expected4 = Vmax * (1 - 0.25 / 4.0); // r=0.5
    assert(std::abs(v4 - expected4) < 1e-12);

    // Y-component at non-axis point should be zero because axis is horizontal
    double v5 = poiseuilleVelocityComponent({1.0, 1.0}, axis, center, R, Vmax, 1);
    assert(std::abs(v5) < 1e-12);

    // Axis along diagnal (1,1)/sqrt(2), check both components
    std::pair<double,double> axisDiag = {1.0/std::sqrt(2.0), 1.0/std::sqrt(2.0)};
    std::pair<double,double> centerDiag = {1.0, 1.0};
    // Point at (1,1) is on axis -> max velocity
    double v6x = poiseuilleVelocityComponent({1.0, 1.0}, axisDiag, centerDiag, R, Vmax, 0);
    double v6y = poiseuilleVelocityComponent({1.0, 1.0}, axisDiag, centerDiag, R, Vmax, 1);
    double expected6 = Vmax / std::sqrt(2.0); // component of max velocity
    assert(std::abs(v6x - expected6) < 1e-12);
    assert(std::abs(v6y - expected6) < 1e-12);

    // Point at perpendicular distance 1 from diagonal axis
    // Point (2,1): vector from center (1,1) is (1,0), projection on axis = 1/sqrt(2)
    // closest point = center + 0.7071*axis = (1.5,1.5), radius = sqrt(0.25+0.25)=0.7071
    double perpDist = std::sqrt( (0.5)*(0.5) + (0.5)*(0.5) ); // distance from (2,1) to (1.5,1.5)
    double expectedMag = Vmax * (1 - perpDist*perpDist / (R*R));
    double v7x = poiseuilleVelocityComponent({2.0, 1.0}, axisDiag, centerDiag, R, Vmax, 0);
    double v7y = poiseuilleVelocityComponent({2.0, 1.0}, axisDiag, centerDiag, R, Vmax, 1);
    double expectedComp = expectedMag / std::sqrt(2.0);
    assert(std::abs(v7x - expectedComp) < 1e-12);
    assert(std::abs(v7y - expectedComp) < 1e-12);

    return 0;
}

// The solution computes the perpendicular distance from a given point to a line defined by a center point and a unit axis vector. This is done by projecting the vector from the center to the point onto the axis, finding the closest point on the axis, then computing the Euclidean distance between the original point and that closest point. Mathematically, given center C, axis unit vector u, and point P, the vector d = P - C; the projection length t = dot(d, u); the closest point Q = C + t*u; then the perpendicular vector is P - Q; its length is r. The velocity magnitude is Vmax * (1 - r^2 / R^2). The velocity vector is axis * magnitude. Return the chosen component (x if index 0, y if index 1). Edge case: when r = 0, magnitude = Vmax. When r = R, magnitude = 0. When r > R (outside tube), the formula yields negative magnitude, but that is physically invalid; however, the task does not specify to clamp, so we return the computed value (which could be negative). Complexity: O(1) time and space.
