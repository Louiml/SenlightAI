Write a standalone C++ function that simulates the core physics of a repulsive potential field. Given a circular field's center location, radius, area of influence, and a particle's position and velocity, the function should compute the force vector applied to the particle when it is inside the influence area, following the same formula as the provided code: `force = 2 * (areaOfInfluence - distance) / (areaOfInfluence - radius)` applied along the unit vector pointing from the field center to the particle. The function must return the updated particle velocity after adding this force (in arbitrary units). Handle the case where the particle is exactly at the field center (distance zero) by returning the original velocity unchanged to avoid division by zero. The function must not modify the input objects; it should take all inputs by value or const reference and return a new velocity.

#include <cassert>
#include <cmath>

// Vec2D and function declaration (included from solution above, but re-declared here for completeness).
struct Vec2D { double x, y; };
Vec2D applyRepulsiveField(const Vec2D&, double, double, const Vec2D&, const Vec2D&);

int main() {
    // Case 1: Particle directly above center, inside influence.
    Vec2D center = {0.0, 0.0};
    Vec2D pos = {0.0, 5.0};
    Vec2D vel = {1.0, 1.0};
    double radius = 2.0, influence = 10.0;
    Vec2D result = applyRepulsiveField(center, radius, influence, pos, vel);
    // forceMag = 2*(10-5)/(10-2)=2*5/8=1.25, direction (0,1)
    assert(std::abs(result.x - 1.0) < 1e-9);
    assert(std::abs(result.y - (1.0 + 1.25)) < 1e-9);

    // Case 2: Outside influence area, unchanged.
    Vec2D posOut = {0.0, 11.0};
    Vec2D resultOut = applyRepulsiveField(center, radius, influence, posOut, vel);
    assert(resultOut.x == vel.x && resultOut.y == vel.y);

    // Case 3: Exactly at center, unchanged (avoid division by zero).
    Vec2D posCenter = {0.0, 0.0};
    Vec2D resultCenter = applyRepulsiveField(center, radius, influence, posCenter, vel);
    assert(resultCenter.x == vel.x && resultCenter.y == vel.y);

    // Case 4: On boundary (distance == influence), unchanged.
    Vec2D posBoundary = {0.0, 10.0};
    Vec2D resultBoundary = applyRepulsiveField(center, radius, influence, posBoundary, vel);
    assert(resultBoundary.x == vel.x && resultBoundary.y == vel.y);

    // Case 5: Invalid parameters (radius >= influence) -> unchanged.
    Vec2D posAny = {3.0, 4.0};
    Vec2D resultInvalid = applyRepulsiveField(center, 5.0, 5.0, posAny, vel);
    assert(resultInvalid.x == vel.x && resultInvalid.y == vel.y);

    // Case 6: Asymmetric position, check force direction and magnitude.
    Vec2D posDiag = {3.0, 4.0}; // distance = 5
    Vec2D resultDiag = applyRepulsiveField(center, 1.0, 10.0, posDiag, {0.0, 0.0});
    double expectedMag = 2.0 * (10.0 - 5.0) / (10.0 - 1.0); // = 10/9 ≈ 1.1111
    // Unit vector (0.6, 0.8)
    assert(std::abs(resultDiag.x - (0.6 * expectedMag)) < 1e-9);
    assert(std::abs(resultDiag.y - (0.8 * expectedMag)) < 1e-9);

    // Case 7: Negative coordinates still work.
    Vec2D centerNeg = {-10.0, -10.0};
    Vec2D posNeg = {-7.0, -6.0}; // dx=3, dy=4, dist=5
    Vec2D resultNeg = applyRepulsiveField(centerNeg, 2.0, 8.0, posNeg, {0.0, 0.0});
    // expectedMag = 2*(8-5)/(8-2)=6/6=1, unit vector (0.6,0.8)
    assert(std::abs(resultNeg.x - 0.6) < 1e-9);
    assert(std::abs(resultNeg.y - 0.8) < 1e-9);

    // Case 8: Zero velocity, particle just inside edge (dist=9.9, influence=10).
    Vec2D posEdge = {0.0, 9.9};
    Vec2D resultEdge = applyRepulsiveField(center, 1.0, 10.0, posEdge, {0.0, 0.0});
    double magEdge = 2.0 * (10.0 - 9.9) / (10.0 - 1.0); // 0.2/9 ≈ 0.02222
    assert(std::abs(resultEdge.y - magEdge) < 1e-9);

    return 0;
}

#include <cmath>
#include <utility>

// Struct to represent a 2D point/vector with double precision.
struct Vec2D {
    double x, y;
};

// Compute the updated velocity of a particle affected by a repulsive potential field.
// fieldCenter: center of the field
// fieldRadius: radius of the repulsive core (must be < influenceArea)
// influenceArea: outer radius of the influence zone
// particlePos: current position of the particle
// particleVel: current velocity of the particle
// Returns the new velocity after adding the repulsive force (if inside influence).
Vec2D applyRepulsiveField(const Vec2D& fieldCenter, double fieldRadius,
                          double influenceArea, const Vec2D& particlePos,
                          const Vec2D& particleVel) {
    // Guard against invalid parameters: radius must be non-negative and strictly less than influence.
    if (fieldRadius < 0.0 || influenceArea <= fieldRadius) {
        return particleVel;
    }
    
    // Displacement vector from field center to particle.
    double dx = particlePos.x - fieldCenter.x;
    double dy = particlePos.y - fieldCenter.y;
    double dist = std::sqrt(dx*dx + dy*dy);
    
    // If particle is outside influence area or exactly at center, no force applied.
    if (dist >= influenceArea || dist == 0.0) {
        return particleVel;
    }
    
    // Unit vector pointing from center to particle.
    double ux = dx / dist;
    double uy = dy / dist;
    
    // Magnitude of the force using the specified formula.
    double forceMag = 2.0 * (influenceArea - dist) / (influenceArea - fieldRadius);
    
    // Add force to the velocity.
    Vec2D result;
    result.x = particleVel.x + ux * forceMag;
    result.y = particleVel.y + uy * forceMag;
    return result;
}

// The solution requires computing the Euclidean distance between the field center and the particle position. If this distance is greater than or equal to the area of influence, the particle is unaffected, and the function returns the original velocity. If the distance is zero (particle exactly at center), we return the original velocity because the direction vector is undefined. Otherwise, compute the unit vector from center to particle by dividing the displacement by its length. The scalar force magnitude is derived from the given formula: `2 * (influence - dist) / (influence - radius)`. Since the formula assumes `radius < influence` (otherwise the denominator is zero or negative causing undefined behavior), we must guard against invalid input: if `radius >= influence`, return original velocity. The force is added to the original velocity to produce the result. Complexity is O(1) time and O(1) space, with only constant arithmetic and a square root operation.
