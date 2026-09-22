/*
Given a 2D differential-drive robot simulation, write a standalone C++ function `bool navigateTo(float& x, float& y, float& theta, float targetX, float targetY, float targetThetaDegrees, float maxSpeed, float dt)` that simulates one time step of the robot as it navigates toward a target pose. The robot state `(x, y, theta)` is the front orientation in radians, where `theta` is measured counterclockwise from the positive x-axis within `[-π, π]`. The robot first rotates in place to face the target point (using the shortest angular path), then moves straight toward it, and finally rotates in place to the target orientation (represented in degrees, convert to radians; if `targetThetaDegrees` is `1000`, skip final rotation). The function updates `x`, `y`, `theta` after time `dt` seconds and returns `true` if the robot has arrived at the target pose (within 0.1 units in position and 0.01 radians in orientation), else `false`. Movement obeys simple kinematics: rotational speed is capped at `0.8` rad/s (use a proportional controller with gain `4.0`), and linear speed is capped at `maxSpeed` (use `maxSpeed * 0.9` when within 10 units of the target, otherwise `maxSpeed`). The robot stops once arrival is detected.
*/
#include <cmath>
#include <algorithm>

// Constants
const double kPI = 3.14159265358979323846;
const double kPosTolerance = 0.1;
const double kAngTolerance = 0.01;
const double kMaxSpin = 0.8;
const double kSpinGain = 4.0;
const double kLinearGain = 1.0; // not strictly needed; we just use max speed
const double kSlowDist = 10.0;
const double kSlowFactor = 0.9;

// Helper: wrap angle to [-pi, pi]
double wrapAngle(double a) {
    while (a > kPI) a -= 2.0 * kPI;
    while (a < -kPI) a += 2.0 * kPI;
    return a;
}

// Main navigation step function
bool navigateTo(float& x, float& y, float& theta, float targetX, float targetY, float targetThetaDegrees, float maxSpeed, float dt) {
    // Convert target orientation to radians; if 1000, use a flag to skip final spin
    bool skipFinalSpin = (targetThetaDegrees == 1000.0f);
    double targetTheta = skipFinalSpin ? theta : targetThetaDegrees * kPI / 180.0;

    // Current pose
    double curX = x, curY = y, curTheta = theta;

    // Pose error
    double dx = targetX - curX;
    double dy = targetY - curY;
    double dist = std::sqrt(dx*dx + dy*dy);
    double targetAngleToPoint = std::atan2(dy, dx);
    double angleErrToPoint = wrapAngle(targetAngleToPoint - curTheta);

    // Final orientation error (only if not skipping)
    double angleErrFinal = skipFinalSpin ? 0.0 : wrapAngle(targetTheta - curTheta);

    // Check arrival
    bool posArrived = (dist <= kPosTolerance);
    bool angArrived = (std::fabs(angleErrFinal) <= kAngTolerance || skipFinalSpin);
    if (posArrived && angArrived) {
        return true; // already at target
    }

    // ---- Phase decision ----
    // Phase 1: rotate to face target point (only if target point is not reached)
    if (!posArrived && std::fabs(angleErrToPoint) > kAngTolerance) {
        // Rotational speed (proportional + cap)
        double spin = std::clamp(kSpinGain * angleErrToPoint, -kMaxSpin, kMaxSpin);
        curTheta += spin * dt;
        curTheta = wrapAngle(curTheta);
    }
    // Phase 2: move toward target point (if not yet there and facing it)
    else if (!posArrived && std::fabs(angleErrToPoint) <= kAngTolerance) {
        // Linear speed: slower when near target
        double speed = (dist <= kSlowDist) ? maxSpeed * kSlowFactor : maxSpeed;
        // Direction is forward along current theta
        curX += speed * std::cos(curTheta) * dt;
        curY += speed * std::sin(curTheta) * dt;
        // Recompute errors after moving
        dx = targetX - curX;
        dy = targetY - curY;
        dist = std::sqrt(dx*dx + dy*dy);
        if (dist <= kPosTolerance) {
            posArrived = true;
        }
    }
    // Phase 3: final rotation (only if position reached and orientation not yet)
    else if (posArrived && !skipFinalSpin && std::fabs(angleErrFinal) > kAngTolerance) {
        double spin = std::clamp(kSpinGain * angleErrFinal, -kMaxSpin, kMaxSpin);
        curTheta += spin * dt;
        curTheta = wrapAngle(curTheta);
        // Recompute final error
        angleErrFinal = wrapAngle(targetTheta - curTheta);
    }

    // Update output state
    x = static_cast<float>(curX);
    y = static_cast<float>(curY);
    theta = static_cast<float>(curTheta);

    // Recompute arrival after this step
    dx = targetX - x;
    dy = targetY - y;
    dist = std::sqrt(dx*dx + dy*dy);
    double finalErr = skipFinalSpin ? 0.0 : wrapAngle(targetTheta - theta);
    return (dist <= kPosTolerance) && (std::fabs(finalErr) <= kAngTolerance || skipFinalSpin);
}
#include <cassert>
#include <cmath>

int main() {
    // Test 1: no movement needed, already at target (skip orientation via 1000)
    {
        float x=0, y=0, theta=0;
        bool arrived = navigateTo(x, y, theta, 0, 0, 1000, 10, 0.1);
        assert(arrived == true);
        assert(std::fabs(x-0) < 1e-6 && std::fabs(y-0) < 1e-6);
    }

    // Test 2: already at target orientation (no movement)
    {
        float x=0, y=0, theta=0;
        bool arrived = navigateTo(x, y, theta, 0, 0, 0, 10, 0.1);
        assert(arrived == true);
    }

    // Test 3: rotate 90 degrees to face point (2,0) from (0,0), theta=0
    {
        float x=0, y=0, theta=0;
        bool arrived = navigateTo(x, y, theta, 2, 0, 1000, 10, 0.1);
        // After one step, should have rotated toward 0 rad (already facing), so move forward
        // Since theta=0 and target x=2, it moves straight, so position changes
        assert(std::fabs(x - 1.0) < 1e-3); // moved at max speed 10 * 0.1 = 1 unit
        assert(std::fabs(y - 0) < 1e-3);
        assert(std::fabs(theta - 0) < 1e-3);
    }

    // Test 4: rotate to face point (0,1) from (0,0), theta=0 (needs +90°)
    {
        float x=0, y=0, theta=0;
        bool arrived = navigateTo(x, y, theta, 0, 1, 1000, 10, 0.1);
        // Should rotate at max spin 0.8 rad/s * 0.1s = 0.08 rad, not yet facing
        assert(std::fabs(theta - 0.08) < 1e-3);
        assert(std::fabs(x - 0) < 1e-3);
        assert(std::fabs(y - 0) < 1e-3);
        assert(arrived == false);
    }

    // Test 5: final spin after reaching point
    {
        float x=0, y=0, theta=0;
        // Force arrival by setting target to current position, but target orientation 90°
        // First step: position already reached, so rotate
        bool arrived = navigateTo(x, y, theta, 0, 0, 90, 10, 0.1);
        assert(arrived == false); // not yet at 90°
        assert(std::fabs(theta - 0.08) < 1e-3); // rotated 0.08 rad
        // Run many steps until arrival
        for (int i=0; i<1000 && !arrived; ++i) {
            arrived = navigateTo(x, y, theta, 0, 0, 90, 10, 0.1);
        }
        assert(arrived == true);
        assert(std::fabs(theta - kPI/2) < 0.011);
    }

    // Test 6: slow down when near target (distance < 10)
    {
        float x=0, y=0, theta=0;
        // Place target at (1,0) so distance=1 (<10)
        // Rotate step first (already facing), then move slowly
        // First call: face 0 already, move with 0.9*maxSpeed = 9 * 0.1 = 0.9
        bool arrived = navigateTo(x, y, theta, 1, 0, 1000, 10, 0.1);
        assert(std::fabs(x - 0.9) < 1e-3);
    }

    // Test 7: rapid call until final destination
    {
        float x=0, y=0, theta=0;
        bool arrived = false;
        for (int i=0; i<10000 && !arrived; ++i) {
            arrived = navigateTo(x, y, theta, 10, 0, 0, 5, 0.05);
        }
        assert(arrived == true);
        assert(std::fabs(x-10) < 0.1);
        assert(std::fabs(y-0) < 0.1);
        assert(std::fabs(theta-0) < 0.01);
    }

    // Test 8: went backward? Should never go negative x when target is positive
    {
        float x=0, y=0, theta=0;
        for (int i=0; i<100; ++i) {
            navigateTo(x, y, theta, 5, 0, 1000, 3, 0.02);
            assert(x >= -1e-6); // never moves backward
        }
    }

    // Test 9: orientation wrap checking from near ±π
    {
        float x=0, y=0, theta=kPI-0.05; // nearly 180°
        // Target point at (0,0) from (0,0) but target orientation -90°
        bool arrived = navigateTo(x, y, theta, 0, 0, -90, 5, 0.01);
        // Should rotate toward -90° (i.e., -π/2). From near π, shortest path is negative direction
        assert(theta < kPI-0.04); // theta decreased
    }

    // Test 10: exact arrival tolerance
    {
        float x=0, y=0, theta=0;
        // Target at (0.05, 0) is within tolerance 0.1, so arrive immediately
        bool arrived = navigateTo(x, y, theta, 0.05, 0, 0, 5, 0.1);
        assert(arrived == true);
        // Position unchanged
        assert(std::fabs(x) < 1e-6);
    }

    return 0;
}
// The solution models three phases: (1) rotate to face the target point, (2) move straight toward it, (3) rotate to the final orientation. For each phase, compute the error (angle difference or distance) and apply a proportional controller with speed caps. For angular control, use the shortest signed difference `delta = atan2(sin(target - current), cos(target - current))`. For linear control, simply move along the current forward direction. Important edge cases: when the target is already reached (or very close) and no final rotation is required, stop immediately. When the robot is exactly at the target point but final orientation is required, skip linear movement and go straight to final spin. Also handle the case where the robot is already at the target position and orientation (within tolerance). The algorithm runs in constant time per call; every update is O(1). Space is O(1). The main challenge is correctly converting between radians and degrees and ensuring angular wrapping.
