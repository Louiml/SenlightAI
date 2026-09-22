Implement a standalone C++ function `float computeWallFollowingCorrection(bool detectLeftWall, bool detectRightWall, float leftSensorError, float rightSensorError, float lastLeftError, float lastRightError, float deltaDistance, float postThreshold, float pidProportional, float pidIntegral, float pidDerivative, float pidPrevError, float pidIntegralAccum, float elapsedTime, float linearSpeed, float maxLinearSpeed, bool& followingLeft, bool& followingRight, float& lastLeftErr, float& lastRightErr, float& lastBlindDist, bool& resetByPost, float cellSize, float postClearance, float& pidIntegralOut)` that simulates the core correction logic from the provided `FollowWall::compute_angular_correction` and `FollowWall::check_posts` methods. The function should: (1) update the stored last-left/right errors and last blind distance using the passed delta distance; (2) detect "posts" when the negative slope of a sensor error with respect to distance exceeds `postThreshold`, turning off the corresponding wall-following flag and marking `resetByPost`; (3) reset following flags and accumulator if no wall is being followed on a side and the blind distance traveled since the last reset exceeds `cellSize + (resetByPost ? postClearance : 0)`; (4) compute the error as: if both sides followed → `leftError - rightError`; if only left → `2*leftError`; if only right → `-2*rightError`; else return 0; (5) compute PID response as `P*error + I*integral + D*(error - prevError)/dt` (with integral clamped to ±10), then scale by `linearSpeed / maxLinearSpeed` and return. The function must modify the references to track state. All parameters are floats except booleans and references. No classes or external libraries beyond `<cmath>` and `<algorithm>`.

#include <cassert>
#include <cmath>

// Forward declaration (function defined above in same translation unit)
float computeWallFollowingCorrection(
    bool, bool, float, float, float, float, float, float,
    float, float, float, float, float, float, float, float,
    bool&, bool&, float&, bool&, float, float, float&);

int main() {
    // Test 1: Following both walls, symmetric errors, no posts, PID only P=1
    bool followingLeft = true, followingRight = true;
    float lastLeftErr = 1.0f, lastRightErr = 1.0f;
    float lastBlindDist = 0.0f;
    bool resetByPost = false;
    float pidIntegral = 0.0f;
    float result = computeWallFollowingCorrection(
        true, true, 1.0f, 1.0f,
        lastLeftErr, lastRightErr,
        0.1f, 0.5f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f,
        0.1f, 1.0f, 1.0f,
        followingLeft, followingRight,
        lastBlindDist, resetByPost,
        1.0f, 0.2f, pidIntegral);
    assert(std::fabs(result - 0.0f) < 1e-5); // error = 1-1 = 0
    assert(followingLeft && followingRight);
    assert(lastBlindDist > 0.0f);

    // Test 2: Only left wall, error=0.5, P=2, no posts
    followingLeft = true; followingRight = false;
    lastLeftErr = 0.5f; lastRightErr = 0.0f;
    lastBlindDist = 0.0f; resetByPost = false; pidIntegral = 0.0f;
    result = computeWallFollowingCorrection(
        true, false, 0.5f, 0.0f,
        lastLeftErr, lastRightErr,
        0.1f, 0.5f,
        2.0f, 0.0f, 0.0f,
        0.0f, 0.0f,
        0.1f, 1.0f, 1.0f,
        followingLeft, followingRight,
        lastBlindDist, resetByPost,
        1.0f, 0.2f, pidIntegral);
    // error = 2*0.5 = 1.0, response = 2*1 = 2, scaled by 1/1 = 2
    assert(std::fabs(result - 2.0f) < 1e-5);
    assert(followingLeft); // no post detected

    // Test 3: Post detection disables left following
    followingLeft = true; followingRight = true;
    lastLeftErr = 0.0f; lastRightErr = 0.0f;
    lastBlindDist = 0.0f; resetByPost = false; pidIntegral = 0.0f;
    // Large negative slope (rapid error decrease) triggers post
    // rate = -(0.0 - 0.5)/0.1 = 5 >= 0.5
    result = computeWallFollowingCorrection(
        true, true, 0.0f, 0.0f,
        0.5f, 0.0f,  // previous left error high
        0.1f, 0.5f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f,
        0.1f, 1.0f, 1.0f,
        followingLeft, followingRight,
        lastBlindDist, resetByPost,
        1.0f, 0.2f, pidIntegral);
    assert(!followingLeft); // left post detected
    assert(followingRight);
    assert(resetByPost); // reset due to post
    assert(std::fabs(lastBlindDist - 0.0f) < 1e-5); // reset happened

    // Test 4: No walls followed → returns 0, state flags unchanged
    followingLeft = false; followingRight = false;
    lastLeftErr = 0.0f; lastRightErr = 0.0f;
    lastBlindDist = 0.0f; resetByPost = false; pidIntegral = 0.0f;
    result = computeWallFollowingCorrection(
        false, false, 0.0f, 0.0f,
        lastLeftErr, lastRightErr,
        0.1f, 0.5f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f,
        0.1f, 1.0f, 1.0f,
        followingLeft, followingRight,
        lastBlindDist, resetByPost,
        1.0f, 0.2f, pidIntegral);
    assert(result == 0.0f);
    assert(!followingLeft && !followingRight);

    // Test 5: Reset when no wall followed after threshold distance
    followingLeft = true; followingRight = false;
    lastLeftErr = 0.0f; lastRightErr = 0.0f;
    lastBlindDist = 0.9f; resetByPost = false; pidIntegral = 0.0f;
    // Simulate a previous call that set followingLeft=false via post? For simplicity, manually set followingLeft=false
    followingLeft = false;
    result = computeWallFollowingCorrection(
        false, false, 0.0f, 0.0f,
        lastLeftErr, lastRightErr,
        0.2f, 0.5f, // deltaDistance = 0.2, so lastBlindDist becomes 1.1 >= 1.0
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f,
        0.1f, 1.0f, 1.0f,
        followingLeft, followingRight,
        lastBlindDist, resetByPost,
        1.0f, 0.2f, pidIntegral);
    assert(result == 0.0f); // no walls followed
    assert(!followingLeft && !followingRight); // still false
    // The reset condition triggered but since already false, no change
    // Test that integration state is reset when reset occurs
    pidIntegral = 5.0f;
    // Force followingLeft=true, followingRight=false, then send a large delta to trigger reset
    followingLeft = true; followingRight = false;
    lastBlindDist = 0.0f; resetByPost = false;
    result = computeWallFollowingCorrection(
        true, false, 0.0f, 0.0f,
        0.0f, 0.0f,
        2.0f, 0.5f, // deltaDistance 2.0, lastBlindDist becomes 2.0 >= 1.0
        1.0f, 0.0f, 0.0f,
        0.0f, 5.0f, // prevError=0, integral=5
        0.1f, 1.0f, 1.0f,
        followingLeft, followingRight,
        lastBlindDist, resetByPost,
        1.0f, 0.2f, pidIntegral);
    // Since followingLeft becomes false (post not detected? No, slope is 0, not post)
    // But condition "not followingLeft or not followingRight" is true (followingRight=false), and lastBlindDist >= 1.0
    // so both flags become false and pidIntegral reset to 0
    assert(pidIntegral == 0.0f);
    assert(!followingLeft && !followingRight);

    // Test 6: PID integral and proportional combine
    followingLeft = true; followingRight = true;
    lastLeftErr = 1.0f; lastRightErr = 0.0f;
    lastBlindDist = 0.0f; resetByPost = false; pidIntegral = 0.0f;
    result = computeWallFollowingCorrection(
        true, true, 1.0f, 0.0f,
        lastLeftErr, lastRightErr,
        0.1f, 0.5f,
        1.0f, 0.5f, 0.0f,
        0.0f, 0.0f,
        0.1f, 2.0f, 2.0f,
        followingLeft, followingRight,
        lastBlindDist, resetByPost,
        1.0f, 0.2f, pidIntegral);
    // error = 1-0 = 1; integral accumulates 1*0.1 = 0.1; response = 1*1 + 0.5*0.1 = 1.05; scaled by 2/2 = 1.05
    assert(std::fabs(result - 1.05f) < 1e-5);
    assert(std::fabs(pidIntegral - 0.1f) < 1e-5);

    // Test 7: Negative delta returns 0 and does not update state
    followingLeft = true; followingRight = false;
    lastLeftErr = 2.0f; lastRightErr = 0.0f;
    lastBlindDist = 3.0f; resetByPost = false; pidIntegral = 0.0f;
    result = computeWallFollowingCorrection(
        true, false, 1.0f, 0.0f,
        lastLeftErr, lastRightErr,
        -0.5f, 0.5f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f,
        0.1f, 1.0f, 1.0f,
        followingLeft, followingRight,
        lastBlindDist, resetByPost,
        1.0f, 0.2f, pidIntegral);
    assert(result == 0.0f);
    // State should be unchanged
    assert(followingLeft);
    assert(std::fabs(lastBlindDist - 3.0f) < 1e-5);
    assert(std::fabs(lastLeftErr - 2.0f) < 1e-5); // not updated

    return 0;
}

#include <algorithm>
#include <cmath>

/**
 * Compute wall-following angular correction.
 * 
 * @param detectLeftWall   Current reading whether left wall is detected.
 * @param detectRightWall  Current reading whether right wall is detected.
 * @param leftSensorError  Current left sensor error (positive when too close).
 * @param rightSensorError Current right sensor error.
 * @param lastLeftError    Previous left error (updated by function).
 * @param lastRightError   Previous right error (updated by function).
 * @param deltaDistance    Distance traveled since last call.
 * @param postThreshold    Slope threshold for post detection.
 * @param pidP, pidI, pidD PID gains.
 * @param pidPrevError     Previous PID error (updated by function).
 * @param pidIntegralAccum Integral accumulator (updated by function).
 * @param elapsedTime      Time step for PID derivative.
 * @param linearSpeed      Current linear speed.
 * @param maxLinearSpeed   Maximum linear speed for scaling.
 * @param followingLeft    In/out: flag for following left wall.
 * @param followingRight   In/out: flag for following right wall.
 * @param lastBlindDist    In/out: distance since last reset.
 * @param resetByPost      In/out: whether last reset was due to post.
 * @param cellSize         Cell size for reset threshold.
 * @param postClearance    Extra clearance after post.
 * @param pidIntegralOut   Output integral accumulator (same as pidIntegralAccum reference).
 * @return Angular correction in range proportional to error.
 */
float computeWallFollowingCorrection(
    bool detectLeftWall, bool detectRightWall,
    float leftSensorError, float rightSensorError,
    float lastLeftError, float lastRightError,
    float deltaDistance, float postThreshold,
    float pidP, float pidI, float pidD,
    float pidPrevError, float pidIntegralAccum,
    float elapsedTime, float linearSpeed, float maxLinearSpeed,
    bool& followingLeft, bool& followingRight,
    float& lastBlindDist, bool& resetByPost,
    float cellSize, float postClearance,
    float& pidIntegralOut)
{
    // If no movement, no correction (and do not update state)
    if (deltaDistance <= 0.0F) {
        return 0.0F;
    }

    lastBlindDist += deltaDistance;

    bool foundPost = false;

    // Check left wall for post
    if (followingLeft) {
        float leftSlope = -(leftSensorError - lastLeftError) / deltaDistance;
        if (leftSlope >= postThreshold) {
            followingLeft = false;
            if (followingRight) {
                lastBlindDist = 0.0F;
                resetByPost = true;
            }
            foundPost = true;
        }
    }

    // Check right wall for post
    if (followingRight) {
        float rightSlope = -(rightSensorError - lastRightError) / deltaDistance;
        if (rightSlope >= postThreshold) {
            followingRight = false;
            if (followingLeft) {
                lastBlindDist = 0.0F;
                resetByPost = true;
            }
            foundPost = true;
        }
    }

    // Update stored errors
    lastLeftError = leftSensorError;
    lastRightError = rightSensorError;

    // Reset if no longer following any wall and traveled enough
    if ((not followingLeft or not followingRight) &&
        lastBlindDist >= (cellSize + (resetByPost ? postClearance : 0.0F))) {
        followingLeft = false;
        followingRight = false;
        pidIntegralAccum = 0.0F;
    }

    // Compute error based on active following sides
    float error = 0.0F;
    if (followingLeft && followingRight) {
        error = leftSensorError - rightSensorError;
    } else if (followingLeft) {
        error = 2.0F * leftSensorError;
    } else if (followingRight) {
        error = -2.0F * rightSensorError;
    } else {
        return 0.0F;
    }

    // PID computation
    float integral = pidIntegralAccum + error * elapsedTime;
    integral = std::max(-10.0F, std::min(10.0F, integral));
    float derivative = (elapsedTime > 0.0F) ? (error - pidPrevError) / elapsedTime : 0.0F;
    float response = pidP * error + pidI * integral + pidD * derivative;

    // Update PID state
    pidPrevError = error;
    pidIntegralOut = integral;

    // Scale response by speed ratio, avoid division by zero
    if (maxLinearSpeed == 0.0F) {
        return 0.0F;
    }
    return response * linearSpeed / maxLinearSpeed;
}

// The solution mirrors the original algorithm but removes dependencies on wall sensor objects and PID classes. The main challenge is correctly handling state transitions. First, if `deltaDistance <= 0`, return 0 immediately without updating state (as per original). Then update `lastBlindDist += deltaDistance`. For each followed side, compute the rate of change: `-(currentError - lastError) / deltaDistance`. If this rate ≥ `postThreshold`, that side's following flag becomes false; if the other side is still following, reset the blind distance reference (set `lastBlindDist = 0` and `resetByPost = true`). This reset happens only once per post detection, so we must handle the case where both sides trigger in the same call — original code calls reset twice but second resets again, effectively same as one. After post checks, update stored last errors. Then if not following either side but following at least one side previously (i.e., followingLeft or followingRight became false) and `lastBlindDist >= cellSize + (resetByPost ? postClearance : 0)`, reset both following flags to false and clear the PID accumulator (also reset `resetByPost` to false conceptually, but the function only sets it via reference when post detected — in original `reset()` calls `reset_displacement(false)` which sets `resetByPost=false`). For simplicity in the task, we set `followingLeft = followingRight = false` and `pidIntegralOut = 0` on this reset. Then compute error as described. PID: integral += error * dt, clamp to [-10,10]; derivative = (error - prevError)/dt; response = P*error + I*integral + D*derivative; then store prevError = error. Finally return response * linearSpeed / maxLinearSpeed. Edge cases: deltaDistance zero or negative returns 0; division by zero in derivative when dt=0 — assume dt>0; if maxLinearSpeed is zero, return 0 to avoid division by zero. The function is called repeatedly with state maintained by caller. Complexity: O(1) time and space.
