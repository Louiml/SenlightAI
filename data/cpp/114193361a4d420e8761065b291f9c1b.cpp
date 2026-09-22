Write a C++ function that simulates the core numerical computation of a point-to-point constraint solver used in physics engines: given two 3D anchor points `pivotA` and `pivotB` (in world space), their relative position vectors from their respective body centers `relA` and `relB`, the inverse masses (`invMassA`, `invMassB`) and inverse inertia tensors (as symmetric 3x3 matrices diagonal or full), and a time step `dt`, compute the accumulated impulse correction vector needed to reduce the positional error between the anchors to zero (with optional clamping) using a simplified Jacobian-free iterative projection method. The function should return a `std::array<double,3>` representing the impulse to apply (positive for body A, negative for body B) and must handle the degenerate case where both inverse masses are zero (static bodies) by returning a zero vector.

// The task simplifies the Bullet physics `btPoint2PointConstraint` by focusing on the scalar impulse computation per axis rather than full rigid-body dynamics. The main idea for each of the three Cartesian axes independently: compute the positional error `depth = pivotB[i] - pivotA[i]` (or its negation, but consistent), then compute the effective mass `jacDiagABInv = 1 / (invMassA + invMassB + invInertiaA[i] + invInertiaB[i])` (treating the inertia tensor as diagonal for simplicity; for full matrices, take the corresponding diagonal entry). The impulse delta is `deltaImpulse = -depth * (tau/dt) * jacDiagABInv` where `tau` is a stiffness factor (e.g., 0.2). Clamp each axis impulse to a range `[-impulseClamp, impulseClamp]` if provided. Accumulate the total impulse per axis. Edge cases: zero effective mass (both bodies static) → return zero impulse; large errors may require multiple iterations, but the function does a single pass and returns the immediate correction; negative errors handled by sign. Time complexity O(1), space O(1).

#include <array>
#include <algorithm>
#include <cmath>
#include <stdexcept>

// Compute the impulse correction for a point-to-point constraint given anchor points,
// relative positions (not actually used in simplified model but kept for interface compatibility),
// inverse masses, inverse inertia diagonal entries, time step, stiffness tau, and optional clamp.
// Returns a 3D impulse vector (positive applies to body A, negative to body B). Handles static bodies.
std::array<double,3> computePointConstraintImpulse(
    const std::array<double,3>& pivotA,
    const std::array<double,3>& pivotB,
    const std::array<double,3>& relA,   // unused in this simplified version
    const std::array<double,3>& relB,   // unused
    double invMassA,
    double invMassB,
    const std::array<double,3>& invInertiaDiagA, // diagonal of inverse inertia tensor
    const std::array<double,3>& invInertiaDiagB,
    double dt,
    double tau = 0.2,
    double impulseClamp = -1.0) { // negative means no clamp
    if (dt <= 0.0) {
        throw std::invalid_argument("Time step must be positive");
    }
    std::array<double,3> impulse{0.0, 0.0, 0.0};
    for (int i = 0; i < 3; ++i) {
        double effectiveMassInv = invMassA + invMassB + invInertiaDiagA[i] + invInertiaDiagB[i];
        if (effectiveMassInv == 0.0) {
            continue; // both bodies static, no impulse needed
        }
        double jacDiagABInv = 1.0 / effectiveMassInv;
        double depth = pivotB[i] - pivotA[i]; // error: how far B is from A
        double deltaImpulse = -depth * (tau / dt) * jacDiagABInv;
        if (impulseClamp > 0.0) {
            deltaImpulse = std::clamp(deltaImpulse, -impulseClamp, impulseClamp);
        }
        impulse[i] = deltaImpulse;
    }
    return impulse;
}

#include <cassert>
#include <cmath>
#include <array>

int main() {
    // Basic case: error along x-axis, both bodies have mass 1, no inertia
    auto impulse1 = computePointConstraintImpulse(
        {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0},
        1.0, 1.0, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0},
        1.0, 0.2);
    assert(std::abs(impulse1[0] - (-0.1)) < 1e-9); // -0.2 * 1 * (1/(1+1)) = -0.1
    assert(std::abs(impulse1[1]) < 1e-9);
    assert(std::abs(impulse1[2]) < 1e-9);

    // Static bodies: both inv masses and inertias zero -> zero impulse
    auto impulse2 = computePointConstraintImpulse(
        {0.0, 0.0, 0.0}, {5.0, 5.0, 5.0},
        {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0},
        0.0, 0.0, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0},
        1.0);
    for (double v : impulse2) {
        assert(v == 0.0);
    }

    // Clamping: large error, small clamp
    auto impulse3 = computePointConstraintImpulse(
        {0.0, 0.0, 0.0}, {100.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0},
        1.0, 1.0, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0},
        1.0, 0.2, 5.0);
    assert(std::abs(impulse3[0] - (-5.0)) < 1e-9);

    // Asymmetric masses: heavier body B (inv mass smaller) -> less correction
    auto impulse4 = computePointConstraintImpulse(
        {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0},
        1.0, 0.5, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0},
        1.0, 0.2);
    // effectiveMassInv = 1.5, jacDiagABInv = 2/3, depth=1, delta = -0.2*1*(2/3) = -0.13333...
    assert(std::abs(impulse4[0] - (-0.1333333333)) < 1e-9);

    // Negative error (B is behind A)
    auto impulse5 = computePointConstraintImpulse(
        {0.0, 0.0, 0.0}, {-1.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0},
        1.0, 1.0, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0},
        1.0, 0.2);
    assert(std::abs(impulse5[0] - 0.1) < 1e-9); // positive impulse, pulls A toward B

    // Inertia influence: add inertia on A only, same mass
    auto impulse6 = computePointConstraintImpulse(
        {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0},
        1.0, 1.0, {1.0, 0.0, 0.0}, {0.0, 0.0, 0.0},
        1.0, 0.2);
    // effectiveMassInv = 1+1+1+0 = 3, jacDiagABInv = 1/3, delta = -0.2*1*(1/3) = -0.066666...
    assert(std::abs(impulse6[0] - (-0.0666666667)) < 1e-9);

    // Small time step: larger correction (since dt in denominator)
    auto impulse7 = computePointConstraintImpulse(
        {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0},
        1.0, 1.0, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0},
        0.1, 0.2); // delta = -0.2 * (1/0.1) * (1/2) = -1.0
    assert(std::abs(impulse7[0] - (-1.0)) < 1e-9);

    // Test that invalid dt throws
    bool threw = false;
    try {
        computePointConstraintImpulse(
            {0,0,0}, {1,0,0}, {0,0,0}, {0,0,0},
            1,1,{0,0,0},{0,0,0}, 0.0);
    } catch (...) {
        threw = true;
    }
    assert(threw);

    // Test with clamp that does not trigger: clamp larger than needed
    auto impulse8 = computePointConstraintImpulse(
        {0,0,0}, {1,0,0}, {0,0,0}, {0,0,0},
        1,1,{0,0,0},{0,0,0}, 1.0, 0.2, 100.0);
    assert(std::abs(impulse8[0] - (-0.1)) < 1e-9);

    return 0;
}
