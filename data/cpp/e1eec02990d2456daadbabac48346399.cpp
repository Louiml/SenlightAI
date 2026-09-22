Write a C++ function `float computeFrictionImpulse(float invMassA, float invMassB, float invIA, float invIB, float rx, float ry, float vx, float vy, float w, float maxForce, float dt)` that computes the magnitude of the linear friction impulse applied by a simplified friction joint model. The joint connects a static anchor point (body A fixed at origin with zero velocity) to a moving body B at the given relative point `(rx, ry)` from B's center of mass. B has mass properties `invMassB` and `invIB`, while A contributes `invMassA` and `invIA` (which may be zero if A is static or infinite mass). The relative velocity at the anchor point is given by `(vx, vy)` (the velocity of point B relative to the fixed point, already including the `cross(w, r)` term). The angular velocity is `w`. The maximum force is `maxForce` and the time step is `dt`. The function must return the magnitude of the final linear impulse (after clamping) that would be applied in one velocity-solving iteration, using the same effective mass matrix and clamping logic as the Box2D friction joint (only the linear part, ignoring angular torque). Handle the edge case where the effective mass matrix is singular (zero determinant) by returning 0.0.

The solution mirrors the linear friction part of `b2FrictionJoint::SolveVelocityConstraints`. First, compute the 2x2 effective mass matrix `K` using the formula:
- `K.ex.x = invMassA + invMassB + invIA * ry * ry + invIB * ry * ry`
- `K.ex.y = -invIA * rx * ry - invIB * rx * ry`
- `K.ey.x = K.ex.y`
- `K.ey.y = invMassA + invMassB + invIA * rx * rx + invIB * rx * rx`
The inverse of this matrix is the linear mass matrix `m_linearMass`. Since the relative velocity `Cdot` is given directly as `(vx, vy)`, the unclamped impulse is `-m_linearMass * Cdot`. Then accumulate this into a current impulse (initially zero) and clamp its magnitude to `maxImpulse = dt * maxForce`. The returned value is the magnitude of the final clamped impulse. Edge cases: if the determinant of `K` is zero (e.g., all masses and inertias are zero, or a degenerate configuration), the inverse does not exist; in such cases, return 0.0 to indicate no impulse can be computed. Also, if `maxForce` is zero or `dt` is zero, the clamping will result in zero impulse. Time complexity is O(1) with constant space.

#include <cmath>
#include <algorithm>

// Compute the magnitude of the linear friction impulse for a simplified 2D friction joint.
// Parameters:
//   invMassA, invMassB : inverse masses of bodies A and B (A may be static, then 0)
//   invIA, invIB       : inverse rotational inertias of A and B
//   rx, ry             : relative position of the anchor point from body B's center of mass
//   vx, vy             : relative linear velocity at the anchor point (already includes cross(w, r))
//   w                  : angular velocity (not used for linear impulse, but kept for signature completeness)
//   maxForce           : maximum allowable force
//   dt                 : time step
// Returns the magnitude of the final clamped linear impulse (>=0).
// If the effective mass matrix is singular, returns 0.0.
float computeFrictionImpulse(float invMassA, float invMassB, float invIA, float invIB,
                             float rx, float ry, float vx, float vy, float w,
                             float maxForce, float dt) {
    // Effective mass matrix K as described.
    // K = [ mA+mB+iA*ry^2+iB*ry^2, -iA*rx*ry - iB*rx*ry ]
    //     [ same off-diagonal,       mA+mB+iA*rx^2+iB*rx^2 ]
    float k11 = invMassA + invMassB + invIA * ry * ry + invIB * ry * ry;
    float k12 = -invIA * rx * ry - invIB * rx * ry;
    float k22 = invMassA + invMassB + invIA * rx * rx + invIB * rx * rx;

    // Compute determinant; if zero, matrix is singular.
    float det = k11 * k22 - k12 * k12;
    if (std::fabs(det) < 1e-12f) {
        return 0.0f;
    }

    // Inverse of K (symmetric).
    float invDet = 1.0f / det;
    // m_linearMass = [[k22, -k12], [-k12, k11]] * invDet
    float m11 = k22 * invDet;
    float m12 = -k12 * invDet;
    float m22 = k11 * invDet;

    // Cdot = (vx, vy)
    // Unclamped impulse = -m_linearMass * Cdot
    float impulseX = -(m11 * vx + m12 * vy);
    float impulseY = -(m12 * vx + m22 * vy);

    // Clamping: initial impulse is zero, so the new impulse is simply the computed one.
    // But we must clamp its magnitude to maxImpulse.
    float maxImpulse = dt * maxForce;
    float magnitude = std::sqrt(impulseX * impulseX + impulseY * impulseY);

    if (magnitude > maxImpulse && magnitude > 1e-12f) {
        float scale = maxImpulse / magnitude;
        impulseX *= scale;
        impulseY *= scale;
        magnitude = maxImpulse;
    }

    // Return the magnitude of the final impulse.
    return std::fabs(magnitude);
}

#include <cassert>
#include <cmath>

// The solution function is declared here (as if included from the solution file).
float computeFrictionImpulse(float invMassA, float invMassB, float invIA, float invIB,
                             float rx, float ry, float vx, float vy, float w,
                             float maxForce, float dt);

int main() {
    // Simple case: both bodies have mass 1, inertia 1, no offsets, velocity along x.
    // K = [2, 0; 0, 2], inverse = [0.5, 0; 0, 0.5]
    // Unclamped impulse = -0.5 * 1 = -0.5 in x, magnitude 0.5.
    // maxImpulse = 1*1 = 1, no clamp.
    float result = computeFrictionImpulse(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    assert(std::fabs(result - 0.5f) < 1e-6f);

    // Clamping: velocity large, maxForce small, dt=1 => maxImpulse=0.1, should clamp to 0.1.
    result = computeFrictionImpulse(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0f, 0.0f, 0.1f, 1.0f);
    assert(std::fabs(result - 0.1f) < 1e-6f);

    // Case with offset: rx=1, ry=0, body B only (A static: invMassA=0, invIA=0), B has invMass=1, invI=1.
    // K = [1+0+0+1*0 =1, -1*1*0=0; 0, 1+1*1*1=2] => inverse diag [1, 0.5]
    // Velocity (1,0): impulse = -[1,0] = (-1,0), magnitude 1. Clamp with maxImpulse=5 => unchanged.
    result = computeFrictionImpulse(0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f, 1.0f);
    assert(std::fabs(result - 1.0f) < 1e-6f);

    // Singular matrix: all masses and inertias zero -> determinant zero, return 0.
    result = computeFrictionImpulse(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 5.0f, 5.0f, 0.0f, 10.0f, 1.0f);
    assert(result == 0.0f);

    // Direction test: velocity (0, -2), K = diag(2,2) => impulse = (0, 1), magnitude 1.
    result = computeFrictionImpulse(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -2.0f, 0.0f, 3.0f, 1.0f);
    assert(std::fabs(result - 1.0f) < 1e-6f);

    // Off-diagonal effect: rx=1, ry=1, both masses 1, inertias 1.
    // K = [1+1+1*1+1*1=4, -1*1*1 -1*1*1 = -2; -2, 4]
    // det = 16-4=12, inv = (1/12)*[4,2;2,4] = [1/3, 1/6; 1/6, 1/3]
    // Velocity (6,0) => impulse = -(1/3*6 + 1/6*0) = -2, y=-(1/6*6)= -1, magnitude sqrt(5) ≈ 2.236.
    // maxImpulse = 10, no clamp.
    result = computeFrictionImpulse(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 6.0f, 0.0f, 0.0f, 10.0f, 1.0f);
    assert(std::fabs(result - std::sqrt(5.0f)) < 1e-5f);

    // Zero maxForce: always returns 0.
    result = computeFrictionImpulse(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    assert(result == 0.0f);

    // Zero dt: maxImpulse = 0, returns 0.
    result = computeFrictionImpulse(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    assert(result == 0.0f);

    return 0;
}
