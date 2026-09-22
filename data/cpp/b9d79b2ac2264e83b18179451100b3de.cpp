// Write a C++ function that models a simplified 2D linear constraint solver for two rigid bodies, analogous to the linear portion of a Box2D motor joint. Given two bodies each with position, velocity, mass, and rotational inertia, and given a fixed relative offset (the "anchor offset" between the bodies), a maximum force limit, and a correction factor, the function must compute and return the linear impulse that should be applied to both bodies to drive their relative velocity toward a desired target while respecting the force limit. The function should take as input: body A's inverse mass, inverse inertia, position, velocity, and the anchor offset; body B's inverse mass, inverse inertia, position, and velocity; the desired linear offset (the target relative position between the anchor points); the maximum linear force; the correction factor (between 0 and 1); and the time step. It should return a `std::pair<double, double>` representing the final linear impulse components (x, y) after applying the velocity constraint and clamping to the maximum impulse. The solver must account for the rotational coupling caused by the offset of the anchor points from the body centers of mass, and must handle the case where the effective mass matrix is singular by returning a zero impulse.

// The task is to implement the linear part of a motor joint velocity constraint solver as done in Box2D. The main algorithm:
// 1. Compute the anchor point positions relative to each body's center of mass: `rA = rotate(qA, linearOffset - localCenterA)` and `rB = rotate(qB, -localCenterB)`. Here `localCenterA` and `localCenterB` are the body's local centers of mass (for simplicity, we can assume they are zero if the task doesn't specify otherwise, but to be general we include them). The rotation `qA` and `qB` are from the current angles of the bodies. Since the task only mentions positions and velocities, we need to include angles as well. The statement says "position" but to compute the rotation, we need the angle. I will assume the function takes the angle as a separate parameter for each body, or the position is a 2D vector and we also have an angle. I'll define a simple `Body` struct inside the solution with fields for position (x,y), angle, velocity (vx,vy), angular velocity, inverse mass, and inverse inertia. The function will take two such `Body` references, plus the desired linear offset (a 2D vector) between the anchors, the max force, correction factor, and time step `dt`. The function will return `std::pair<double,double>` for the impulse.
//
// 2. Compute the effective mass matrix `K` (2x2) which is the sum of contributions from both bodies: for each body, the linear mass term plus the rotational inertia times the cross product of the anchor offset. Specifically, for a body with inverse mass `m`, inverse inertia `i`, and anchor offset `r = (rx, ry)`, the contribution to K is:
//    - K[0][0] += m + i * ry^2
//    - K[0][1] += -i * rx * ry
//    - K[1][1] += m + i * rx^2
//    The off-diagonal terms are symmetric.
//    If K is singular (determinant near zero), we cannot invert; in that case, the solver should return zero impulse. To check, compute determinant = K[0][0]*K[1][1] - K[0][1]*K[1][0]. If abs(det) < 1e-12, return {0.0, 0.0}.
//
// 3. Compute the current relative velocity at the anchor points: `Cdot = vB + cross(wB, rB) - vA - cross(wA, rA)`, where `cross(w, r)` in 2D is `(-w*ry, w*rx)`.
//
// 4. Compute the position error: `m_linearError = (cB + rB) - (cA + rA) - (desiredOffset)` where the desired offset is the target relative position between the two anchor points. Actually in Box2D, `m_linearError = cB + rB - cA - rA - linearOffset` where `linearOffset` is the target offset between the anchor points. The correction term is `inv_h * correctionFactor * m_linearError`, where `inv_h = 1/dt`.
//
// 5. The desired velocity correction is `Cdot' = Cdot + inv_h * correctionFactor * m_linearError`. The impulse before clamping is `impulse = -K_inv * Cdot'`. Then we add this to the accumulated impulse from previous steps (but since this function is called only once per step, we can treat the accumulated impulse as zero at start; however, the problem statement says return the final linear impulse, so we compute the incremental impulse and return it directly). For simplicity, we treat the initial accumulated impulse as zero. So the impulse to apply is `-K_inv * Cdot'`.
//
// 6. Clamp the impulse magnitude to `maxImpulse = dt * maxForce`. If the squared length of the impulse exceeds `maxImpulse^2`, scale it down to maxImpulse. Return the final impulse.
//
// 7. Important edge cases: singular matrix (det ~ 0) returns zero; if the time step is zero or negative, we should handle gracefully (return zero); if maxForce is zero, the impulse should be zero. Also, if the correction factor is outside [0,1], we can ignore or assert; we'll clamp internally.
//
// Time complexity is O(1) constant time, space complexity O(1).
//
// The solution will define a `Body` struct and a free function `solveLinearMotorImpulse` that takes the required parameters. We must be careful with the const-correctness: the function should take const references to bodies but we don't modify them; it returns the impulse only. The bodies' angles are used to compute rotation matrices. I'll include the necessary headers: `<utility>`, `<cmath>`, `<algorithm>`, `<cassert>`.

#include <utility>
#include <cmath>
#include <algorithm>

// Simple 2D body state for the motor joint linear solver.
struct Body {
    double x, y;        // position
    double angle;       // orientation in radians
    double vx, vy;      // linear velocity
    double w;           // angular velocity
    double invMass;     // inverse mass
    double invI;        // inverse rotational inertia
    // Optionally, local center of mass offset. For simplicity, assume zero.
};

// Helper: 2D cross product of a scalar angular velocity with a position vector: cross(w, r) = (-w*ry, w*rx)
static std::pair<double, double> cross(double w, double rx, double ry) {
    return std::make_pair(-w * ry, w * rx);
}

// Helper: rotate a vector by an angle (2D rotation)
static std::pair<double, double> rotate(double angle, double vx, double vy) {
    double c = std::cos(angle);
    double s = std::sin(angle);
    return std::make_pair(c * vx - s * vy, s * vx + c * vy);
}

// Solve the linear part of a motor joint constraint.
// Returns the final linear impulse (px, py) to apply to both bodies (opposite signs).
// The desired linear offset is the target relative position of B's anchor relative to A's anchor.
std::pair<double, double> solveLinearMotorImpulse(
    const Body& A, const Body& B,
    double desiredOffsetX, double desiredOffsetY,
    double maxForce, double correctionFactor, double dt)
{
    // Guard for invalid time step or zero max force
    if (dt <= 0.0 || maxForce <= 0.0) {
        return std::make_pair(0.0, 0.0);
    }

    // Assume local centers of mass are at the body origins (0,0)
    // Then rA = rotate(A.angle, desiredOffsetX, desiredOffsetY) (since linearOffset is in A's local frame)
    // Actually in Box2D, linearOffset is the offset from A's origin to B's origin in A's local frame.
    // But here we treat desiredOffset as the target relative vector from A's anchor to B's anchor in world space?
    // The snippet uses m_linearOffset = bodyA->GetLocalPoint(xB) where xB is B's position.
    // We simplify: the desired offset is the target world-space difference between B and A positions.
    // Then rA = rotation of (desiredOffset - localCenterA) but localCenterA = 0, so rA = rotate(A.angle, desiredOffsetX, desiredOffsetY).
    // But careful: In Box2D, m_rA = b2Mul(qA, m_linearOffset - m_localCenterA) and m_rB = b2Mul(qB, -m_localCenterB).
    // Since we assume local centers are zero, m_rA = rotate(A.angle, linearOffset) and m_rB = (0,0).
    // However, the linear error is computed as cB + m_rB - cA - m_rA where m_rA is the anchor offset from A's body to the anchor point.
    // We need to interpret the problem: The bodies have positions (x,y) and we want to maintain a fixed relative offset between their anchor points.
    // Let's define anchorA = A.pos + rotate(A.angle, aLocalOffset) and anchorB = B.pos + rotate(B.angle, bLocalOffset).
    // For simplicity, set local anchors to zero, so anchorA = A.pos, anchorB = B.pos.
    // Then desired offset is the target vector from A.pos to B.pos.
    // So the linear error = (B.pos - A.pos) - desiredOffset.
    // And the relative velocity at the anchors: Cdot = vB - vA (since anchors coincide with body positions).
    // This simplifies but at the cost of ignoring rotational coupling.
    // To keep the task interesting and matching the snippet, we include the offset from the center of mass.
    // Let's define local anchors: For simplicity, we set both local anchors to (0,0), but then the rotational coupling disappears.
    // To exercise the full linear solver, we allow each body to have a local anchor offset.
    // For clarity, we'll add parameters for local anchor offsets? The problem statement says "fixed relative offset" which I interpret as the target world-space distance between the two anchor points.
    // I'll implement a version where the local anchors are at the body centers (so rA = rB = 0), but that trivializes.
    // A better interpretation: The parameters will include the local anchor offset for each body.
    // Since the task is self-contained, I will modify the function signature to include local anchor offsets.
    // However, the problem statement explicitly says "given a fixed relative offset (the 'anchor offset' between the bodies)".
    // I'll take the interpretation: The desired offset is the target relative vector from A's anchor to B's anchor.
    // The anchor offsets from each body's center are given as two additional parameters.
    // To keep it manageable, I'll include them as parameters.

    // For this solution, I'll assume local anchors are at zero, but then the mass matrix is diagonal with invMassA+invMassB.
    // To make the problem more interesting, I'll add local anchor offsets as extra parameters.
    // Since the problem statement is ambiguous, I will produce a solution that includes the general case.
    // To avoid overcomplicating, I'll define the function to take the local anchor offsets as well.
    // But to fit the given structure, I'll make the function take them as arguments.
    // Let's redesign: The function signature will be:
    // solveLinearMotorImpulse(A, B, localAnchorAX, localAnchorAY, localAnchorBX, localAnchorBY,
    //                         desiredOffsetX, desiredOffsetY, maxForce, correctionFactor, dt)
    // I'll implement that.

    // The rest of the analysis and solution follows that.
}

// Since the above is incomplete, I'll write a proper solution now.

Given the ambiguity, I will now finalize the solution with a clear design: The function takes two `Body` structs, each with an associated local anchor offset (stored within the Body struct for simplicity), plus the desired world-space offset between anchors, max force, correction factor, and dt. The function computes the impulse exactly as in Box2D but only the linear part, returning the final accumulated linear impulse.

I'll write the final solution:

#include <utility>
#include <cmath>
#include <algorithm>

struct Body {
    double x, y;        // position
    double angle;       // orientation
    double vx, vy;      // linear velocity
    double w;           // angular velocity
    double invMass;     // inverse mass
    double invI;        // inverse rotational inertia
    double localAnchorX, localAnchorY; // local anchor offset from body center
};

// Helper: 2D cross product of angular velocity w and position r: cross(w, r) = (-w*ry, w*rx)
static std::pair<double, double> cross(double w, double rx, double ry) {
    return std::make_pair(-w * ry, w * rx);
}

// Rotate a vector by angle
static void rotate(double angle, double vx, double vy, double& outX, double& outY) {
    double c = std::cos(angle);
    double s = std::sin(angle);
    outX = c * vx - s * vy;
    outY = s * vx + c * vy;
}

// Solve linear part of motor joint. Returns final linear impulse (px, py).
std::pair<double, double> solveLinearMotorImpulse(
    const Body& A, const Body& B,
    double desiredOffsetX, double desiredOffsetY,
    double maxForce, double correctionFactor, double dt)
{
    // Guard invalid inputs
    if (dt <= 0.0 || maxForce <= 0.0) {
        return std::make_pair(0.0, 0.0);
    }

    // Compute anchor offsets relative to each body's center of mass in world frame
    double rAX, rAY;
    rotate(A.angle, A.localAnchorX, A.localAnchorY, rAX, rAY);
    double rBX, rBY;
    rotate(B.angle, B.localAnchorX, B.localAnchorY, rBX, rBY);

    // Effective mass matrix K (2x2)
    // K[0][0] = invMassA + invMassB + invIA * rAY^2 + invIB * rBY^2
    // K[0][1] = -invIA * rAX * rAY - invIB * rBX * rBY
    // K[1][0] = same as K[0][1]
    // K[1][1] = invMassA + invMassB + invIA * rAX^2 + invIB * rBX^2
    double k00 = A.invMass + B.invMass + A.invI * rAY * rAY + B.invI * rBY * rBY;
    double k01 = -A.invI * rAX * rAY - B.invI * rBX * rBY;
    double k10 = k01;
    double k11 = A.invMass + B.invMass + A.invI * rAX * rAX + B.invI * rBX * rBX;

    double det = k00 * k11 - k01 * k10;
    if (std::abs(det) < 1e-12) {
        return std::make_pair(0.0, 0.0); // singular, cannot solve
    }

    // Invert K
    double invDet = 1.0 / det;
    double invK00 = invDet * k11;
    double invK01 = -invDet * k01;
    double invK10 = invK01;
    double invK11 = invDet * k00;

    // Compute current relative velocity at anchors
    // Cdot = vB + cross(wB, rB) - vA - cross(wA, rA)
    auto crossA = cross(A.w, rAX, rAY);
    auto crossB = cross(B.w, rBX, rBY);
    double cdotX = B.vx + crossB.first - A.vx - crossA.first;
    double cdotY = B.vy + crossB.second - A.vy - crossA.second;

    // Position error: desired offset is target relative vector from A's anchor to B's anchor.
    // Actual relative vector = (B.pos + rB) - (A.pos + rA)
    double actualRelX = (B.x + rBX) - (A.x + rAX);
    double actualRelY = (B.y + rBY) - (A.y + rAY);
    double linErrX = actualRelX - desiredOffsetX;
    double linErrY = actualRelY - desiredOffsetY;

    // Correction term
    double inv_h = 1.0 / dt;
    double biasX = inv_h * correctionFactor * linErrX;
    double biasY = inv_h * correctionFactor * linErrY;

    // Cdot' = Cdot + bias
    double cdotPrimeX = cdotX + biasX;
    double cdotPrimeY = cdotY + biasY;

    // Impulse before clamping: impulse = -K_inv * Cdot'
    double impulseX = -(invK00 * cdotPrimeX + invK01 * cdotPrimeY);
    double impulseY = -(invK10 * cdotPrimeX + invK11 * cdotPrimeY);

    // Clamp to max impulse magnitude
    double maxImpulse = dt * maxForce;
    double lenSq = impulseX * impulseX + impulseY * impulseY;
    if (lenSq > maxImpulse * maxImpulse) {
        double scale = maxImpulse / std::sqrt(lenSq);
        impulseX *= scale;
        impulseY *= scale;
    }

    return std::make_pair(impulseX, impulseY);
}

#include <cassert>
#include <cmath>
#include <utility>

int main() {
    // Test 1: Zero dt -> zero impulse
    Body a{0,0,0, 0,0,0, 1.0, 1.0, 0,0};
    Body b{0,0,0, 0,0,0, 1.0, 1.0, 0,0};
    auto p = solveLinearMotorImpulse(a, b, 0,0, 10.0, 0.5, 0.0);
    assert(p.first == 0.0 && p.second == 0.0);

    // Test 2: Zero max force -> zero impulse
    p = solveLinearMotorImpulse(a, b, 0,0, 0.0, 0.5, 0.1);
    assert(p.first == 0.0 && p.second == 0.0);

    // Test 3: Identical bodies, no offset, no velocity, desired offset (0,0) -> zero impulse
    p = solveLinearMotorImpulse(a, b, 0,0, 100.0, 1.0, 0.1);
    assert(std::abs(p.first) < 1e-12 && std::abs(p.second) < 1e-12);

    // Test 4: Desired offset not met, correction factor 1, no velocity -> impulse towards offset
    // A at (0,0), B at (1,0), desired offset (0,0) -> wants B to come to A, so impulse should pull B left
    Body a2{0,0,0, 0,0,0, 1.0, 1.0, 0,0};
    Body b2{1,0,0, 0,0,0, 1.0, 1.0, 0,0};
    p = solveLinearMotorImpulse(a2, b2, 0,0, 1000.0, 1.0, 0.1);
    // Impulse direction should be negative X (pull B left, push A right)
    assert(p.first < 0.0);
    assert(std::abs(p.second) < 1e-12);

    // Test 5: Clamping: large error, limited maxForce, impulse magnitude <= maxImpulse
    Body a3{0,0,0, 0,0,0, 0.5, 2.0, 0,0};
    Body b3{100,100,0, 0,0,0, 0.5, 2.0, 0,0};
    double maxForce = 10.0;
    double dt = 0.016;
    p = solveLinearMotorImpulse(a3, b3, 0,0, maxForce, 1.0, dt);
    double maxImpulse = dt * maxForce; // 0.16
    double len = std::sqrt(p.first*p.first + p.second*p.second);
    assert(len <= maxImpulse + 1e-9);

    // Test 6: Singular mass matrix (e.g., both invMass and invI zero) -> zero impulse
    Body a4{0,0,0, 0,0,0, 0.0, 0.0, 0,0};
    Body b4{0,0,0, 0,0,0, 0.0, 0.0, 0,0};
    p = solveLinearMotorImpulse(a4, b4, 1,0, 10.0, 1.0, 0.1);
    assert(p.first == 0.0 && p.second == 0.0);

    // Test 7: Symmetry: swapping bodies but with reversed desired offset should give opposite impulse
    Body A{0,0,0, 0,0,0, 2.0, 3.0, 0,0};
    Body B{2,1,0, 0,0,0, 4.0, 5.0, 0,0};
    auto p1 = solveLinearMotorImpulse(A, B, 1,1, 50.0, 1.0, 0.02);
    // Now swap: B as A, A as B, desired offset becomes - (1,1)
    auto p2 = solveLinearMotorImpulse(B, A, -1,-1, 50.0, 1.0, 0.02);
    assert(std::abs(p1.first + p2.first) < 1e-9);
    assert(std::abs(p1.second + p2.second) < 1e-9);

    return 0;
}

The test file includes the solution function directly (in practice, the solution would be included via header). For brevity in the test section, I assume the solution function is already defined above. The tests cover edge cases: zero dt, zero force, zero error, positive error, clamping, singular matrix, and symmetry. All assertions should pass.

Explanation of the symmetry test: If we swap bodies and negate the desired offset, the physical situation is identical but with roles reversed, so the impulse applied to the first body should be the negative of the impulse applied to the swapped first body (which is the original second body). This holds because the impulse vector for A equals -impulse for B due to Newton's third law.

The solution function is independent and self-contained, with no main function. The test section provides a complete main function with assertions.
