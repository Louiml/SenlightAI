// Write a standalone C++ function named `computeWheelSuspensionForce` that, given a chassis mass, the suspension rest length, the current suspension length, the suspension stiffness, the compression damping coefficient, the relaxation damping coefficient, and the suspension relative velocity (positive when the wheel is moving in the direction of compression), returns the suspension force according to the spring-damper model used in the provided raycast vehicle code. The force must follow this logic: first compute a spring force as `stiffness * (restLength - currentLength) / max(0.1, contactNormalDotWheelDirection)` where `contactNormalDotWheelDirection` is provided as an additional parameter; then subtract a damper force where the damping coefficient is the compression damping if the relative velocity is negative (compression), otherwise the relaxation damping; finally, if the resulting force is negative, clamp it to zero. The function must handle the edge case where the current length is greater than the rest length, and must use the clipped inverse contact dot suspension (provided as an input parameter) exactly as the code does when the denominator is too small. The function should be pure, deterministic, and take all inputs as `double` parameters, returning a `double`. It should be named descriptively and be self-contained with only necessary headers (e.g., `<algorithm>` if needed).
The solution replicates the core of the `updateSuspension` method from the `btRaycastVehicle` class, isolating the computation of the suspension force for a single wheel. The algorithm is straightforward: compute the spring force by multiplying the suspension stiffness by the difference between the rest length and the current length, and then multiplying by the clipped inverse contact dot suspension (which is `1/denominator` or `1/0.1` if the denominator is too small). The damper force subtracts from the spring force, using the compression damping coefficient when the suspension relative velocity is negative (i.e., the suspension is compressing) and the relaxation damping coefficient otherwise. Finally, the total force is multiplied by the chassis mass, and if the result is negative (which can happen if the suspension is extended beyond rest length and the damper is acting), it is clamped to zero. The main edge cases include: (1) when the denominator for the inverse contact dot suspension is small or negative, the code forces the inverse to `1/0.1`, but the provided parameter already accounts for that, so we just use it; (2) when the current length exceeds the rest length, the spring force becomes negative (pulling down), but the clamping ensures no negative forces are applied. The complexity is O(1) in both time and space, as it is a simple arithmetic computation.
#include <algorithm> // for std::max

/**
 * Compute the suspension force for a single wheel using a spring-damper model.
 *
 * @param chassisMass               The mass of the chassis (scalar).
 * @param suspensionRestLength      The rest length of the suspension spring.
 * @param currentSuspensionLength   The current length of the suspension spring.
 * @param suspensionStiffness       The spring stiffness coefficient.
 * @param dampingCompression        Damping coefficient when the suspension is compressing (relative velocity < 0).
 * @param dampingRelaxation         Damping coefficient when the suspension is relaxing (relative velocity >= 0).
 * @param suspensionRelativeVelocity The relative velocity of the suspension (positive = extension, negative = compression).
 * @param clippedInvContactDotSuspension The clipped inverse of (contactNormal · wheelDirection), always >= 0.1.
 * @return The computed suspension force, clamped to be non-negative.
 */
double computeWheelSuspensionForce(
    double chassisMass,
    double suspensionRestLength,
    double currentSuspensionLength,
    double suspensionStiffness,
    double dampingCompression,
    double dampingRelaxation,
    double suspensionRelativeVelocity,
    double clippedInvContactDotSuspension) {

    // Spring force: stiffness * lengthDiff * clippedInvContactDotSuspension
    double lengthDiff = suspensionRestLength - currentSuspensionLength;
    double force = suspensionStiffness * lengthDiff * clippedInvContactDotSuspension;

    // Damper force: subtract damping * relative velocity, choosing the coefficient
    // based on the sign of the relative velocity.
    double damping = (suspensionRelativeVelocity < 0.0) ? dampingCompression : dampingRelaxation;
    force -= damping * suspensionRelativeVelocity;

    // Scale by chassis mass and clamp to zero (no negative suspension forces).
    force *= chassisMass;
    if (force < 0.0) {
        force = 0.0;
    }

    return force;
}
#include <cassert>
#include <cmath>

// The solution function is declared here for the test; in a real project, it would be in a header.
double computeWheelSuspensionForce(
    double chassisMass,
    double suspensionRestLength,
    double currentSuspensionLength,
    double suspensionStiffness,
    double dampingCompression,
    double dampingRelaxation,
    double suspensionRelativeVelocity,
    double clippedInvContactDotSuspension);

int main() {
    // Test 1: Basic compression case
    // rest=0.5, current=0.4, stiffness=100, dampingComp=10, dampingRelax=5, relVel=-2, inv=5
    // Spring: 100 * (0.1) * 5 = 50
    // Damper: -10 * (-2) = +20 (subtracted -> 50 - 20 = 30)
    // Force: 30 * 2 = 60
    double force1 = computeWheelSuspensionForce(2.0, 0.5, 0.4, 100.0, 10.0, 5.0, -2.0, 5.0);
    assert(std::abs(force1 - 60.0) < 1e-9);

    // Test 2: Extension case (relative velocity positive -> relaxation damping)
    // rest=0.5, current=0.6, stiffness=100, dampingComp=10, dampingRelax=5, relVel=1, inv=5
    // Spring: 100 * (-0.1) * 5 = -50
    // Damper: -5 * 1 = -5 (subtracted -> -50 - 5 = -55)
    // Force: -55 * 1 = -55 -> clamped to 0
    double force2 = computeWheelSuspensionForce(1.0, 0.5, 0.6, 100.0, 10.0, 5.0, 1.0, 5.0);
    assert(std::abs(force2 - 0.0) < 1e-9);

    // Test 3: Zero relative velocity, current equals rest length
    // Spring: 0, damper: 0 -> force 0
    double force3 = computeWheelSuspensionForce(1.0, 0.5, 0.5, 100.0, 10.0, 5.0, 0.0, 5.0);
    assert(std::abs(force3 - 0.0) < 1e-9);

    // Test 4: Compression with high damping, but still positive force
    // rest=0.5, current=0.4, stiffness=10, dampingComp=100, relVel=-0.1, inv=5
    // Spring: 10 * 0.1 * 5 = 5
    // Damper: -100 * (-0.1) = +10 (subtracted -> 5 - 10 = -5)
    // Force: -5 * 1 = -5 -> clamp to 0
    double force4 = computeWheelSuspensionForce(1.0, 0.5, 0.4, 10.0, 100.0, 5.0, -0.1, 5.0);
    assert(std::abs(force4 - 0.0) < 1e-9);

    // Test 5: Realistic scenario from the code's logic with clippedInvContactDotSuspension = 10 (approx 1/0.1)
    // rest=0.3, current=0.2, stiffness=200, dampingComp=20, dampingRelax=8, relVel=-3, inv=10
    // Spring: 200 * 0.1 * 10 = 200
    // Damper: -20 * (-3) = 60 (subtracted -> 200 - 60 = 140)
    // Force: 140 * 1.5 = 210
    double force5 = computeWheelSuspensionForce(1.5, 0.3, 0.2, 200.0, 20.0, 8.0, -3.0, 10.0);
    assert(std::abs(force5 - 210.0) < 1e-9);

    return 0;
}
