Write a C++ function that, given two rigid-body objects represented by their mass, inverse inertia, position, angle, and the local anchor offsets relative to each body's center of mass, computes the velocity impulses required to satisfy a point-to-point (revolute) joint constraint over a single velocity-solving step. The function should take the two bodies' current velocities (linear and angular), the step time delta, and the already-computed joint mass matrix (as a 2×2 symmetric matrix) and relative anchor vectors, and return the 2D linear impulse that must be applied to body A (and its opposite to body B) to enforce the constraint that the anchor points coincide after the step. Use the standard Box2D-style formulation where the velocity constraint is \(Cdot = v_B + \omega_B \times r_B - v_A - \omega_A \times r_A\), the effective mass matrix \(K\) is given, and the impulse is \(P = -K^{-1} Cdot\). The function must handle the degenerate case where the mass matrix is singular (e.g., when both bodies have infinite mass) by returning a zero impulse. The function should not modify any input structures except through a reference output parameter for the impulse; it should be `const`-correct regarding the input velocities and mass matrix.
The solution approach follows the Box2D revolute joint velocity constraint solver. Given the relative anchor vectors \(r_A\) and \(r_B\) (from each body's center of mass to the joint anchor, in world coordinates), and the current linear and angular velocities \((v_A, \omega_A)\) and \((v_B, \omega_B)\), we compute the velocity error at the anchor: \(Cdot = v_B + \omega_B \times r_B - v_A - \omega_A \times r_A\). The goal is to find an impulse \(P\) applied to body A (and \(-P\) to body B) that drives \(Cdot\) to zero. The effective mass matrix \(K\) (2×2) already includes the inverse masses and inverse inertias and the cross-coupling terms; it is symmetric. Since \(Cdot = K P\) when considering the impulse's effect on velocities, solving for \(P\) gives \(P = -K^{-1} Cdot\). To invert the 2×2 matrix, we compute the determinant; if it is zero (singular), we cannot solve uniquely, so we return a zero impulse. This handles the degenerate case of both bodies having infinite mass (all masses and inertias zero), which makes \(K\) zero. The main algorithm is: (1) compute \(Cdot\) using cross products, (2) compute determinant of \(K\), (3) if determinant is near zero (use a small epsilon like 1e-12), return zero; else compute the inverse and solve for \(P\). Time complexity is \(O(1)\) and space complexity \(O(1)\). Edge cases include singular matrix, zero-length anchors, and extreme velocities; the epsilon threshold prevents division by zero.
#include <cmath>
#include <cstddef>

// Simple 2D vector structure
struct Vec2 {
    double x, y;
    Vec2() : x(0), y(0) {}
    Vec2(double x_, double y_) : x(x_), y(y_) {}
};

// 2x2 matrix stored column-major for convenience
struct Mat22 {
    Vec2 ex, ey; // column vectors
    Mat22() : ex(0,0), ey(0,0) {}
    Mat22(const Vec2& ex_, const Vec2& ey_) : ex(ex_), ey(ey_) {}
};

// Cross product of a scalar and a vector: s * (v.x, v.y) -> (-s * v.y, s * v.x)
inline Vec2 crossScalarVec(double s, const Vec2& v) {
    return Vec2(-s * v.y, s * v.x);
}

// Cross product of two vectors returns scalar (z-component)
inline double crossVecVec(const Vec2& a, const Vec2& b) {
    return a.x * b.y - a.y * b.x;
}

// Solve 2x2 linear system K * x = b, returns true if solvable (det non-zero)
bool solve22(const Mat22& K, const Vec2& b, Vec2& x) {
    double det = K.ex.x * K.ey.y - K.ex.y * K.ey.x;
    const double eps = 1e-12;
    if (std::fabs(det) < eps) {
        return false;
    }
    double invDet = 1.0 / det;
    x.x = invDet * ( K.ey.y * b.x - K.ex.y * b.y);
    x.y = invDet * (-K.ey.x * b.x + K.ex.x * b.y);
    return true;
}

// Compute revolute joint point-to-point velocity impulse
// Parameters:
//   vA, wA: linear and angular velocity of body A
//   vB, wB: linear and angular velocity of body B
//   rA, rB: world-space anchor offsets from center of mass to joint anchor for A and B
//   K: effective mass matrix (2x2 symmetric)
//   impulse: output impulse applied to body A (opposite to body B)
void computeRevoluteImpulse(const Vec2& vA, double wA,
                            const Vec2& vB, double wB,
                            const Vec2& rA, const Vec2& rB,
                            const Mat22& K,
                            Vec2& impulse) {
    // Velocity difference at the anchor point
    // Cdot = vB + wB x rB - vA - wA x rA
    Vec2 termB = vB + crossScalarVec(wB, rB);
    Vec2 termA = vA + crossScalarVec(wA, rA);
    Vec2 Cdot(termB.x - termA.x, termB.y - termA.y);

    // Solve K * P = -Cdot
    Vec2 rhs(-Cdot.x, -Cdot.y);
    if (!solve22(K, rhs, impulse)) {
        // Singular matrix (e.g., infinite masses) => no unique solution, zero impulse
        impulse = Vec2(0.0, 0.0);
    }
}
#include <cassert>
#include <cmath>

int main() {
    // Helper to compare doubles with tolerance
    auto close = [](double a, double b) { return std::fabs(a - b) < 1e-9; };

    // Test 1: Simple case where both bodies have mass 1, inertia 1, anchors at (1,0) and (0,1)
    // K = [ mA+mB+rAy^2*iA+rBy^2*iB,  -rAy*rAx*iA-rBy*rBx*iB ]
    //     [ -rAy*rAx*iA-rBy*rBx*iB,  mA+mB+rAx^2*iA+rBx^2*iB ]
    Vec2 rA(1,0), rB(0,1);
    Mat22 K(Vec2(1+1+0*0*1+1*1*1, -0*1*1-1*0*1),
            Vec2(-0*1*1-1*0*1, 1+1+1*1*1+0*0*1));
    // K = [[2+1, 0], [0, 2+1]] = [[3,0],[0,3]]
    K.ex.x = 3; K.ex.y = 0; K.ey.x = 0; K.ey.y = 3;
    Vec2 vA(0,0), vB(1,0);
    double wA = 0, wB = 0;
    Vec2 impulse;
    computeRevoluteImpulse(vA, wA, vB, wB, rA, rB, K, impulse);
    // Cdot = (1,0) - (0,0) = (1,0); P = -K^-1 * Cdot = -(1/3,0)
    assert(close(impulse.x, -1.0/3.0));
    assert(close(impulse.y, 0.0));

    // Test 2: Singular matrix (both bodies have infinite mass => K = 0)
    Mat22 Kzero;
    impulse = Vec2(5,5);
    computeRevoluteImpulse(Vec2(0,0), 0, Vec2(2,3), 0, rA, rB, Kzero, impulse);
    assert(close(impulse.x, 0.0));
    assert(close(impulse.y, 0.0));

    // Test 3: Angular velocities produce non-zero Cdot
    // rA = (1,0), rB = (0,1), wA=2, wB=3, vA=vB=0
    // Cdot = (wB x rB) - (wA x rA) = (-3,0) - (0,2) = (-3,-2)
    // K = identity for simplicity
    Mat22 Kidentity(Vec2(1,0), Vec2(0,1));
    computeRevoluteImpulse(Vec2(0,0), 2.0, Vec2(0,0), 3.0, Vec2(1,0), Vec2(0,1), Kidentity, impulse);
    assert(close(impulse.x, 3.0)); // P = -(-3) = 3
    assert(close(impulse.y, 2.0)); // P = -(-2) = 2

    // Test 4: Symmetric off-diagonal terms in K
    // K = [[2,1],[1,2]], Cdot = (1,0)
    Mat22 K2(Vec2(2,1), Vec2(1,2));
    computeRevoluteImpulse(Vec2(0,0), 0, Vec2(1,0), 0, rA, rB, K2, impulse);
    // Solve [[2,1],[1,2]] * P = (-1,0) => P = (-2/3, 1/3)
    assert(close(impulse.x, -2.0/3.0));
    assert(close(impulse.y, 1.0/3.0));

    // Test 5: Zero relative motion => zero impulse
    computeRevoluteImpulse(Vec2(3,4), 1.0, Vec2(3,4), 1.0, rA, rB, Kidentity, impulse);
    assert(close(impulse.x, 0.0));
    assert(close(impulse.y, 0.0));

    return 0;
}
