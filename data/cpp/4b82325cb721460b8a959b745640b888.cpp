/*
Write a standalone C++ function that simulates a simplified 2D friction joint between two rigid bodies. Given the two bodies' inverse masses, inverse inertias, local anchor offsets in world coordinates (rA and rB), and their current linear velocities (vA, vB) and angular velocities (wA, wB), the function must apply a velocity-level constraint solve that limits both the linear and angular relative motion according to specified maximum force and maximum torque values (both in Newton units, applied over a time step h). The function should return the resulting linear and angular velocities after one iteration of the solver, respecting the clamping of accumulated impulses. Use a point-to-point constraint for the linear part (so relative linear velocity at the anchor is reduced) and a pure angular constraint, with the effective mass matrices computed from the provided inertia and mass data. The output should be a struct containing the updated vA, wA, vB, wB, and the accumulated linear and angular impulses after the solve. Edge cases include zero mass/inertia values (which make the effective mass singular — handle by skipping that component's impulse update) and zero maxForce/maxTorque (which should result in no velocity changes). The function must be `const`-correct and take all inputs as scalar/vector values using a simple custom 2D vector class with `+`, `-`, `*`, `cross`, `dot`, and `lengthSquared` operations.
*/
#include <cmath>
#include <limits>

// Minimal 2D vector for the task.
struct Vec2 {
    double x, y;
    Vec2(double x_ = 0.0, double y_ = 0.0) : x(x_), y(y_) {}
    Vec2 operator+(const Vec2& o) const { return Vec2(x+o.x, y+o.y); }
    Vec2 operator-(const Vec2& o) const { return Vec2(x-o.x, y-o.y); }
    Vec2 operator*(double s) const { return Vec2(x*s, y*s); }
    double dot(const Vec2& o) const { return x*o.x + y*o.y; }
    double cross(const Vec2& o) const { return x*o.y - y*o.x; }
    double lengthSquared() const { return x*x + y*y; }
};

// Cross product of scalar (w) with vector (v) returns vector (w * (-v.y, v.x)).
inline Vec2 cross(double w, const Vec2& v) {
    return Vec2(-w * v.y, w * v.x);
}

// Solve one iteration of a friction joint.
struct FrictionJointResult {
    Vec2 vA, vB;
    double wA, wB;
    Vec2 linearImpulse;
    double angularImpulse;
};

FrictionJointResult solveFrictionJoint(
    double invMassA, double invMassB,
    double invIA, double invIB,
    const Vec2& rA, const Vec2& rB,
    Vec2 vA, double wA,
    Vec2 vB, double wB,
    double maxForce, double maxTorque,
    double h,
    Vec2 linearImpulse = Vec2(0.0, 0.0),
    double angularImpulse = 0.0) {

    // Compute effective linear mass matrix (2x2).
    double mA = invMassA, mB = invMassB;
    double iA = invIA, iB = invIB;
    double k11 = mA + mB + iA * rA.y * rA.y + iB * rB.y * rB.y;
    double k12 = -iA * rA.x * rA.y - iB * rB.x * rB.y;
    double k22 = mA + mB + iA * rA.x * rA.x + iB * rB.x * rB.x;
    double det = k11 * k22 - k12 * k12;

    // Linear effective mass inverse (if singular, unable to solve linear part).
    bool hasLinearMass = (std::abs(det) > 1e-12);
    double invDet = hasLinearMass ? 1.0 / det : 0.0;
    // m_linearMass = K^{-1}
    double lm11 = hasLinearMass ? k22 * invDet : 0.0;
    double lm12 = hasLinearMass ? -k12 * invDet : 0.0;
    double lm22 = hasLinearMass ? k11 * invDet : 0.0;

    // Angular effective mass.
    double angularMass = iA + iB;
    bool hasAngularMass = (angularMass > 1e-12);
    double invAngularMass = hasAngularMass ? 1.0 / angularMass : 0.0;

    // Solve angular friction (only if both maxTorque > 0 and has angular mass).
    if (hasAngularMass && maxTorque > 0.0) {
        double Cdot = wB - wA;
        double impulse = -invAngularMass * Cdot;
        double oldImpulse = angularImpulse;
        double maxImpulse = h * maxTorque;
        angularImpulse = std::max(-maxImpulse, std::min(maxImpulse, angularImpulse + impulse));
        impulse = angularImpulse - oldImpulse;
        wA -= iA * impulse;
        wB += iB * impulse;
    }

    // Solve linear friction (only if maxForce > 0 and has linear mass).
    if (hasLinearMass && maxForce > 0.0) {
        Vec2 Cdot = vB + cross(wB, rB) - vA - cross(wA, rA);
        // impulse = -m_linearMass * Cdot
        Vec2 impulse(-(lm11 * Cdot.x + lm12 * Cdot.y),
                     -(lm12 * Cdot.x + lm22 * Cdot.y));
        Vec2 oldImpulse = linearImpulse;
        linearImpulse = linearImpulse + impulse;

        double maxImpulse = h * maxForce;
        if (linearImpulse.lengthSquared() > maxImpulse * maxImpulse) {
            double len = std::sqrt(linearImpulse.lengthSquared());
            linearImpulse = linearImpulse * (maxImpulse / len);
        }

        impulse = linearImpulse - oldImpulse;

        vA = vA - mA * impulse;
        wA = wA - iA * cross(rA, impulse);
        vB = vB + mB * impulse;
        wB = wB + iB * cross(rB, impulse);
    }

    return {vA, vB, wA, wB, linearImpulse, angularImpulse};
}
#include <cassert>
#include <cmath>

// Include the solution (Vec2, cross, solveFrictionJoint) here.

int main() {
    // Case 1: No movement when maxForce and maxTorque are zero.
    {
        FrictionJointResult r = solveFrictionJoint(
            1.0, 1.0, 1.0, 1.0,
            Vec2(0.5, 0.0), Vec2(-0.5, 0.0),
            Vec2(2.0, 0.0), 1.0,
            Vec2(0.0, 0.0), -1.0,
            0.0, 0.0, 0.1);
        assert(r.vA.x == 2.0 && r.vA.y == 0.0);
        assert(r.wA == 1.0);
        assert(r.vB.x == 0.0 && r.vB.y == 0.0);
        assert(r.wB == -1.0);
        assert(r.linearImpulse.lengthSquared() == 0.0);
        assert(r.angularImpulse == 0.0);
    }

    // Case 2: Unlimited force/torque brings relative motion to zero (within tolerance).
    {
        // Equal masses/inertias, symmetric anchors.
        FrictionJointResult r = solveFrictionJoint(
            1.0, 1.0, 1.0, 1.0,
            Vec2(0.0, 0.0), Vec2(0.0, 0.0),
            Vec2(5.0, 0.0), 0.0,
            Vec2(0.0, 0.0), 0.0,
            1e9, 1e9, 0.1); // effectively unlimited
        // Linear relative velocity becomes zero: vA == vB.
        assert(std::abs(r.vA.x - r.vB.x) < 1e-6);
        assert(std::abs(r.vA.y - r.vB.y) < 1e-6);
        // Angular relative velocity becomes zero: wA == wB.
        assert(std::abs(r.wA - r.wB) < 1e-6);
    }

    // Case 3: Clamping with small maxForce.
    {
        // Two bodies approaching each other along x at anchor.
        FrictionJointResult r = solveFrictionJoint(
            1.0, 1.0, 0.0, 0.0, // no inertia
            Vec2(0.0, 0.0), Vec2(0.0, 0.0),
            Vec2(1.0, 0.0), 0.0,
            Vec2(-1.0, 0.0), 0.0,
            1.0, 0.0, 0.1); // maxForce=1, h=0.1 -> maxImpulse=0.1
        // Linear impulse should be clamped to length 0.1.
        double impulseLen = std::sqrt(r.linearImpulse.lengthSquared());
        assert(std::abs(impulseLen - 0.1) < 1e-6);
        // The impulse should reduce relative velocity but not fully stop it.
        double relV = (r.vB.x + r.vA.x) / 2.0; // since symmetric, both change equally? Actually vA and vB are updates.
        // Here both have mass 1, so vA increases by impulse, vB decreases by impulse.
        assert(std::abs(r.vA.x - (1.0 + 0.1)) < 1e-6);
        assert(std::abs(r.vB.x - (-1.0 - 0.1)) < 1e-6);
    }

    // Case 4: Static body (invMass=0, invI=0) should not move.
    {
        FrictionJointResult r = solveFrictionJoint(
            0.0, 1.0, 0.0, 1.0,
            Vec2(0.0, 0.0), Vec2(0.5, 0.0),
            Vec2(0.0, 0.0), 0.0,
            Vec2(0.0, 0.0), 2.0,
            10.0, 10.0, 0.1);
        // Body A stays static.
        assert(r.vA.x == 0.0 && r.vA.y == 0.0 && r.wA == 0.0);
        // Body B is slowed down but not fully stopped (since maxImpulse limited? actually maxImpulse=1, but angular clamp only if maxTorque >0 and hasAngularMass; here invIA=0 but invIB=1 so angularMass=1, maxTorque=10 so maxImpulse=1.0 at h=0.1).
        // We just ensure B's velocity changed from original.
        assert(r.wB < 2.0 && r.wB > 1.0); // original wB=2, clamp reduces it.
    }

    // Case 5: Zero inverse mass on both (degenerate) should not crash.
    {
        FrictionJointResult r = solveFrictionJoint(
            0.0, 0.0, 0.0, 0.0,
            Vec2(0.0, 0.0), Vec2(0.0, 0.0),
            Vec2(3.0, 4.0), 5.0,
            Vec2(-1.0, 2.0), -3.0,
            100.0, 100.0, 0.5);
        // No masses, so no impulses possible; velocities unchanged.
        assert(r.vA.x == 3.0 && r.vA.y == 4.0 && r.wA == 5.0);
        assert(r.vB.x == -1.0 && r.vB.y == 2.0 && r.wB == -3.0);
    }

    return 0;
}
// The solution follows the structure of Box2D's friction joint velocity solver. First, compute the effective mass matrices: for the linear part, form a 2x2 matrix K = [ mA+mB+iA*rA.y^2+iB*rB.y^2, -iA*rA.x*rA.y - iB*rB.x*rB.y; same symmetric, mA+mB+iA*rA.x^2+iB*rB.x^2 ] and invert it. For the angular part, the effective mass is just 1/(iA+iB). Then, if either mass is zero or the corresponding maximum force/torque is zero, skip that component's impulse update. For the angular part, compute Cdot = wB - wA, impulse = -angularMass * Cdot, clamp the accumulated angular impulse to ±h*maxTorque, and apply the delta to the angular velocities. For the linear part, compute Cdot = vB + cross(wB, rB) - vA - cross(wA, rA), impulse = -linearMass * Cdot, add to accumulated linear impulse, clamp its length to h*maxForce, and apply the delta to both linear and angular velocities. Edge cases include when either inverse mass/inertia is zero (e.g., static body) which makes the effective mass matrix singular — check for finite entries and skip if not invertible. The algorithm runs in O(1) time and O(1) space, as it solves only one velocity iteration without position correction.
