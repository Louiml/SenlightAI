// Write a C++ function that simulates a simplified version of the "Move Until Touch with Position Targeting and Effort Limiting" algorithm. The function should take the following inputs: a target distance (in meters, can be negative to indicate pulling), a tool direction vector (3D), a task direction vector (3D), a force/torque vector (6 components: force x,y,z and torque x,y,z), a boolean indicating whether to use the task frame for movement, and parameters for force thresholds (directional force threshold along movement axis, non-directional force threshold for perpendicular axes, and torque threshold). The function should simulate one iteration of the control loop: it computes a new "virtual attractor" position by offsetting a current position (assumed to be at the origin) by the appropriate direction vector scaled by a fixed pull distance (0.01 meters), plus an additional offset from the task z-axis scaled by a keep-cutting distance (0.001 meters). The function should return an integer status code: 0 if no threshold is crossed, 1 if any force/torque threshold is crossed, 2 if the target distance has been reached (the dot product between the remaining vector to the goal and the movement direction is zero or past zero), and 3 if both threshold crossed and target reached (priority to threshold). The goal position is computed as the movement direction scaled by the target distance from the origin. The movement direction is the task vector if the boolean is true, otherwise the tool vector. Handle edge cases where the movement direction is zero (return -1). The function should not loop; it only performs one iteration and returns the status based on the current state.
#include <cassert>
#include <array>

int main() {
    // Case 1: no events, target not reached, no thresholds crossed
    assert(moveUntilTouchStatus(0.1, {1,0,0}, {0,1,0}, {0,0,0}, {0,0,0}, false, 12, 20, 2) == 0);

    // Case 2: target reached (target distance 0.1, movement along x, dot will be positive until we pass? Actually with positive target, reached when dot <= 0, but from origin to goal dot = 0.1*1=0.1 >0, so not reached yet)
    // To test target reached, we need to simulate that current position has moved, but function assumes current at origin. So to test target reached, use target distance 0 and any direction, dot = 0, which satisfies target reached for both positive and negative (dot <=0 and >=0 both true)
    assert(moveUntilTouchStatus(0.0, {1,0,0}, {0,1,0}, {0,0,0}, {0,0,0}, false, 12, 20, 2) == 2);

    // Case 3: effort limit crossed via torque z
    assert(moveUntilTouchStatus(0.1, {1,0,0}, {0,1,0}, {0,0,0}, {0,0,3}, false, 12, 20, 2) == 1);

    // Case 4: effort limit crossed via directional force (force along x with threshold 1)
    assert(moveUntilTouchStatus(0.1, {1,0,0}, {0,1,0}, {5,0,0}, {0,0,0}, false, 1, 20, 2) == 1);

    // Case 5: effort limit crossed via non-directional force (force along y with threshold 1)
    assert(moveUntilTouchStatus(0.1, {1,0,0}, {0,1,0}, {0,5,0}, {0,0,0}, false, 12, 1, 2) == 1);

    // Case 6: both crossed and target reached (target 0 and force crossed)
    assert(moveUntilTouchStatus(0.0, {1,0,0}, {0,1,0}, {0,5,0}, {0,0,0}, false, 12, 1, 2) == 3);

    // Case 7: use task frame, movement along y, no events
    assert(moveUntilTouchStatus(0.2, {1,0,0}, {0,1,0}, {0,0,0}, {0,0,0}, true, 12, 20, 2) == 0);

    // Case 8: invalid movement direction (zero vector)
    assert(moveUntilTouchStatus(0.1, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, false, 12, 20, 2) == -1);

    // Case 9: negative target distance, not reached yet (dot positive because movement direction positive, goal is negative, dot = -0.1*1 = -0.1 <0, for negative target we need dot >=0, so not reached)
    assert(moveUntilTouchStatus(-0.1, {1,0,0}, {0,1,0}, {0,0,0}, {0,0,0}, false, 12, 20, 2) == 0);

    // Case 10: negative target distance, reached (target 0? if target -0.0? Use very small negative target but dot will be negative still. Instead target = -0.0, but then targetDistance < 0 false? So we use target = -0.0? Let's use target = -0.0 as double -0.0 is not <0, so it triggers else branch. To test negative reached, use targetDistance = -0.0? Actually we need dot >=0, with targetDistance = -0.0, goal = 0, dot = 0, satisfies dot>=0, so targetReached true.
    assert(moveUntilTouchStatus(-0.0, {1,0,0}, {0,1,0}, {0,0,0}, {0,0,0}, false, 12, 20, 2) == 2);

    return 0;
}
#include <cmath>
#include <array>

// Simulate one iteration of the move-until-touch algorithm.
// Returns:
//   0 = no event
//   1 = effort limit crossed
//   2 = target reached
//   3 = effort crossed and target reached (priority to effort)
//  -1 = invalid movement direction (zero vector)
int moveUntilTouchStatus(
    double targetDistance,
    const std::array<double, 3>& toolVector,
    const std::array<double, 3>& taskVector,
    const std::array<double, 3>& force,
    const std::array<double, 3>& torque,
    bool useTaskFrame,
    double directionalForceThreshold,
    double nonDirectionalForceThreshold,
    double torqueThreshold,
    double pullDistance = 0.01,
    double keepCuttingDistance = 0.001
) {
    // Select movement direction
    std::array<double, 3> moveDir = useTaskFrame ? taskVector : toolVector;
    
    // Check for zero movement direction (magnitude ~0)
    double norm = std::sqrt(moveDir[0]*moveDir[0] + moveDir[1]*moveDir[1] + moveDir[2]*moveDir[2]);
    if (norm < 1e-12) return -1;

    // Goal position is moveDir * targetDistance from origin
    std::array<double, 3> goal = { moveDir[0]*targetDistance, moveDir[1]*targetDistance, moveDir[2]*targetDistance };

    // Compute virtual attractor position (not needed for status but demonstrates algorithm)
    // Current position is origin (0,0,0)
    std::array<double, 3> attractor = {0.0, 0.0, 0.0};
    double dirSign = (targetDistance > 0) ? 1.0 : -1.0;
    attractor[0] += dirSign * moveDir[0] * pullDistance + taskVector[0]*keepCuttingDistance;
    attractor[1] += dirSign * moveDir[1] * pullDistance + taskVector[1]*keepCuttingDistance;
    attractor[2] += dirSign * moveDir[2] * pullDistance + taskVector[2]*keepCuttingDistance;

    // Check effort limits
    bool effortCrossed = false;
    // Torque check (all three)
    if (std::abs(torque[0]) > torqueThreshold ||
        std::abs(torque[1]) > torqueThreshold ||
        std::abs(torque[2]) > torqueThreshold) {
        effortCrossed = true;
    }
    // Force check: along movement axis use directional threshold, others non-directional
    // We need to project force onto movement direction to find component along movement axis.
    // Simpler: since direction is normalized? Not normalized, but we can compare the force component along movement direction.
    // To keep faithful to original, original used force.z as directional (likely movement along z). Here we'll treat force component along moveDir as directional.
    double projection = (force[0]*moveDir[0] + force[1]*moveDir[1] + force[2]*moveDir[2]) / norm;
    if (std::abs(projection) > directionalForceThreshold) {
        effortCrossed = true;
    }
    // For perpendicular force, we check the magnitude of force minus its projection onto moveDir
    double perpForceX = force[0] - projection * (moveDir[0]/norm);
    double perpForceY = force[1] - projection * (moveDir[1]/norm);
    double perpForceZ = force[2] - projection * (moveDir[2]/norm);
    double perpMagnitude = std::sqrt(perpForceX*perpForceX + perpForceY*perpForceY + perpForceZ*perpForceZ);
    if (perpMagnitude > nonDirectionalForceThreshold) {
        effortCrossed = true;
    }

    // Check target reached
    // Remaining vector from current position (origin) to goal
    std::array<double, 3> remain = { goal[0], goal[1], goal[2] }; // since current is zero
    double dot = remain[0]*moveDir[0] + remain[1]*moveDir[1] + remain[2]*moveDir[2];
    bool targetReached;
    if (targetDistance < 0) {
        targetReached = dot >= 0;
    } else {
        targetReached = dot <= 0;
    }

    if (effortCrossed && targetReached) return 3;
    if (effortCrossed) return 1;
    if (targetReached) return 2;
    return 0;
}
// The solution computes the movement direction vector by selecting between the task and tool vectors based on the boolean input. If the magnitude of this direction is zero, we return -1 because no meaningful movement is possible. The goal position is the origin plus the movement direction multiplied by the target distance. The virtual attractor position is computed by taking the origin (current position) plus or minus the movement direction scaled by the pull distance (positive target means push, negative means pull), plus the task z-axis scaled by the keep-cutting distance (this represents an additional offset, typically for cutting). To determine threshold crossing, we check each force/torque component against its respective threshold: the force component along the movement axis is checked against the directional threshold, while the other two force components against the non-directional threshold, and all three torque components against the torque threshold. If any threshold is crossed, we set status to 1. To determine target reached, we compute the remaining vector from the current position (origin) to the goal, then take its dot product with the movement direction. If the target distance is positive, the target is reached when the dot product is less than or equal to zero (meaning we have gone past the perpendicular plane); if negative, when it is greater than or equal to zero. The final status is threshold first (1) if crossed, else target (2) if reached, else 0. Edge cases: zero target distance means we are already at the goal, so target is reached; zero movement direction returns -1; thresholds of zero would immediately be crossed if any force/torque component is non-zero. Time complexity is O(1) as only constant operations are performed. Space complexity is O(1) as we only use a few local variables.
