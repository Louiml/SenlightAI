/*
Design a C++ function that computes the accumulated hinge angle for a simplified hinge constraint system. Given two 3D rotation matrices (representing the orientations of two rigid bodies A and B), a hinge axis in body A's local frame, and a previous accumulated angle, the function should compute the current hinge angle between the two bodies around the shared hinge axis, then update the accumulated angle by taking the shortest angular path from the previous accumulated angle to the current angle, with a tolerance threshold of 0.3 radians — if the absolute difference between the current and accumulated angle exceeds this threshold, the accumulated angle snaps to the current angle; otherwise it adjusts by the shortest angular distance. The function must handle angle wrapping so that all computed angles remain within (-π, π] and the accumulated angle is continuous (no jumps larger than π). The hinge axis is the Z-axis (third column) of the frame A rotation matrix, and the reference frames are defined by the first two columns of frame A and the second column of frame B. The function should work purely with 3x3 rotation matrices (using a simple matrix representation) and use standard math library functions; no external physics library is allowed. The signature should be: `double accumulatedHingeAngle(const double rotA[3][3], const double rotB[3][3], double previousAccumulatedAngle)`.
*/
#include <cmath>
#include <algorithm>

// Compute the accumulated hinge angle for two rotation matrices.
// rotA and rotB are 3x3 rotation matrices stored in row-major order (rot[i][j]).
// The hinge axis is assumed to be the Z-axis (third column) of rotA.
// previousAccumulatedAngle is the last accumulated angle (in radians).
// Returns the updated accumulated angle with shortest-path continuity and a tolerance of 0.3.
double accumulatedHingeAngle(const double rotA[3][3], const double rotB[3][3], double previousAccumulatedAngle) {
    // Extract reference axes from rotA (columns 0 and 1) and swing axis from rotB (column 1)
    double refAxis0[3] = {rotA[0][0], rotA[1][0], rotA[2][0]};
    double refAxis1[3] = {rotA[0][1], rotA[1][1], rotA[2][1]};
    double swingAxis[3] = {rotB[0][1], rotB[1][1], rotB[2][1]};

    // Compute dot products
    double dot0 = swingAxis[0] * refAxis0[0] + swingAxis[1] * refAxis0[1] + swingAxis[2] * refAxis0[2];
    double dot1 = swingAxis[0] * refAxis1[0] + swingAxis[1] * refAxis1[1] + swingAxis[2] * refAxis1[2];

    // Current hinge angle in (-π, π]
    double currentAngle = std::atan2(dot0, dot1);

    // Compute shortest angular distance from previous to current, wrapped to (-π, π]
    double twoPi = 2.0 * M_PI;
    double diff = std::fmod(currentAngle - previousAccumulatedAngle + M_PI, twoPi);
    if (diff <= -M_PI) {
        diff += twoPi;  // handles the edge case where fmod returns exactly -π
    }
    diff -= M_PI;  // now diff is in (-π, π]

    // Tolerance threshold
    const double tol = 0.3;
    if (std::fabs(diff) > tol) {
        // Snap to current angle
        return currentAngle;
    } else {
        // Smoothly update by shortest path
        return previousAccumulatedAngle + diff;
    }
}
#include <cassert>
#include <cmath>
#include <iostream>

// The solution function (repeated here for standalone compilation)
double accumulatedHingeAngle(const double rotA[3][3], const double rotB[3][3], double previousAccumulatedAngle) {
    double refAxis0[3] = {rotA[0][0], rotA[1][0], rotA[2][0]};
    double refAxis1[3] = {rotA[0][1], rotA[1][1], rotA[2][1]};
    double swingAxis[3] = {rotB[0][1], rotB[1][1], rotB[2][1]};

    double dot0 = swingAxis[0] * refAxis0[0] + swingAxis[1] * refAxis0[1] + swingAxis[2] * refAxis0[2];
    double dot1 = swingAxis[0] * refAxis1[0] + swingAxis[1] * refAxis1[1] + swingAxis[2] * refAxis1[2];

    double currentAngle = std::atan2(dot0, dot1);

    double twoPi = 2.0 * M_PI;
    double diff = std::fmod(currentAngle - previousAccumulatedAngle + M_PI, twoPi);
    if (diff <= -M_PI) {
        diff += twoPi;
    }
    diff -= M_PI;

    const double tol = 0.3;
    if (std::fabs(diff) > tol) {
        return currentAngle;
    } else {
        return previousAccumulatedAngle + diff;
    }
}

int main() {
    // Identity matrices: both bodies aligned, hinge angle should be 0
    double identityA[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
    double identityB[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
    assert(std::fabs(accumulatedHingeAngle(identityA, identityB, 0.0) - 0.0) < 1e-9);
    assert(std::fabs(accumulatedHingeAngle(identityA, identityB, 1.0) - 1.0) < 1e-9); // small diff, stays

    // Rotate body B by 90 degrees around Z axis relative to A: rotB = rotZ(π/2)
    double rotZ90[3][3] = {{0,-1,0},{1,0,0},{0,0,1}};
    double angle = accumulatedHingeAngle(identityA, rotZ90, 0.0);
    assert(std::fabs(angle - M_PI/2) < 1e-9); // current angle ≈ π/2, snap because diff > 0.3

    // Large jump: previous 0, current near π (rotate 170 degrees). snap to current
    double rotZ170[3][3] = {{std::cos(170*M_PI/180), -std::sin(170*M_PI/180), 0},
                            {std::sin(170*M_PI/180), std::cos(170*M_PI/180), 0},
                            {0,0,1}};
    double current170 = std::atan2(rotZ170[1][0], rotZ170[0][0]); // should be 170° in radians
    double acc = accumulatedHingeAngle(identityA, rotZ170, 0.0);
    // Diff is ~2.967 rad (>0.3), so snap to current (which is ~2.967, but within (-π,π] it's 2.967)
    assert(std::fabs(acc - current170) < 1e-9);

    // Wrap-around: previous angle near π, current near -π (small actual diff)
    double rotZ179[3][3] = {{std::cos(179*M_PI/180), -std::sin(179*M_PI/180), 0},
                            {std::sin(179*M_PI/180), std::cos(179*M_PI/180), 0},
                            {0,0,1}};
    double current179 = std::atan2(rotZ179[1][0], rotZ179[0][0]); // ~3.124
    // Simulate previous = current - 0.1 (so diff is 0.1, within tolerance)
    double prev = current179 - 0.1;
    double acc2 = accumulatedHingeAngle(identityA, rotZ179, prev);
    assert(std::fabs(acc2 - current179) < 1e-9); // should update to prev+0.1 = current179

    // Edge: diff exactly -π (previous = current + π), should wrap correctly
    // current = 0, previous = π → diff = -π (abs=π >0.3) so snap to 0
    double acc3 = accumulatedHingeAngle(identityA, identityB, M_PI);
    assert(std::fabs(acc3 - 0.0) < 1e-9);

    // Test non-trivial rotation in 3D (not pure Z): pick rotA rotated about X, rotB about X+Z
    double rotAX[3][3] = {{1,0,0},{0,std::cos(0.2),-std::sin(0.2)},{0,std::sin(0.2),std::cos(0.2)}};
    double rotB1[3][3] = {{0.9, -0.1, 0.1}, {0.1, 0.9, -0.2}, {0.1, 0.2, 0.9}}; // arbitrary rotation
    // Just ensure it returns a finite number and no assertion failure
    double result = accumulatedHingeAngle(rotAX, rotB1, 0.0);
    assert(std::isfinite(result));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The task requires implementing a continuous hinge angle accumulator based on the standard hinge constraint formulation. The core algorithm: (1) Extract the hinge axis from rotA's third column (assuming the hinge is along frame A's Z-axis). (2) compute the current hinge angle using atan2 of the dot products: the hinge angle is the signed angle between the projections of the second column of rotB onto the X and Y axes of frame A. Specifically, compute refAxis0 = column0 of rotA, refAxis1 = column1 of rotA, swingAxis = column1 of rotB. Then angle = atan2(dot(swingAxis, refAxis0), dot(swingAxis, refAxis1)). This gives the hinge angle in the range (-π, π] as atan2 returns that. (3) To update the accumulated angle with shortest-path continuity, normalize the current angle to be within (-π, π] (already guaranteed by atan2). Compute the difference between the current angle and previous accumulated angle, but wrap this difference to (-π, π] using the standard formula: d = fmod(current - prev + π, 2π) - π; if d <= -π then d += 2π (to handle exactly -π). Then apply the tolerance: if abs(d) > 0.3 radians, the accumulated angle snaps to current; otherwise accumulated = prev + d. The resulting accumulated angle should remain within a reasonable range; no further wrapping is needed for the returned value, but the logic ensures each step's change is at most π. Edge cases: when previous angle is near the boundary (e.g., prev = 3.1, current = -3.1), the shortest path is correct (difference of about 0.08). The tolerance check uses the absolute value of the wrapped difference. Time complexity O(1), space O(1), using only a handful of scalar operations and trig. Matrix columns are assumed to be stored as rot[i][j] where i is row, j is column. The function is const-correct (takes pointers to const arrays).
