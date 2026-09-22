/*
Implement a standalone C++ function that simulates the core Dubins-turn trajectory prediction logic from the provided behavior code. Given an ownship position `(osx, osy)`, ownship heading `osh` in degrees (0-360, with 0=north, 90=east, etc.), a target position `(cnx, cny)`, and a fixed turn radius `turn_radius`, the function must compute and return the predicted path length (in meters) of a Dubins-like path that first drives straight until the target lies exactly on the turning circle, then follows the circle's arc (with a 5-degree step) until the target is directly reachable (i.e., the angle to the next arc point is no longer inside the circle on the correct turning side). The path must include the final straight segment to the target. The function should not post any messages or use any external library beyond `<cmath>` and `<cstddef>`; it should return a `double` representing the total path length. Handle edge cases: if the target is already inside the turning circle at the start, the function should still return the total path length as if the vehicle first moved straight until the target is on the circle (following the same loop logic). If the loop exceeds 1000 steps, break and return the accumulated length. For the arc-step loop, the angle-step is fixed at 5 degrees (in radians). The path length is the sum of all straight segments: the initial straight segment (if any), each arc step segment (between consecutive predicted points), and the final straight segment from the last arc point to the target. Use the same normalization for heading as in the snippet: `heading = pmod(450 - osh, 360) * M_PI / 180`, where `pmod` returns a non-negative modulo for doubles. Do not include any input/output; just the function.
*/

#include <cmath>
#include <cstddef>

// Helper for positive modulo for doubles.
static double pmod(double a, double b) {
    double r = std::fmod(a, b);
    if (r < 0) {
        r += b;
    }
    return r;
}

// Predicts a Dubins-like path from (osx,osy) with heading osh (degrees) to target (cnx,cny)
// using a fixed turn radius. Returns the total path length in meters.
double predictedDubinsPathLength(double osx, double osy, double osh,
                                 double cnx, double cny,
                                 double turn_radius) {
    const double M_PI_LOCAL = 3.14159265358979323846;
    const double step_size = 0.5;          // forward straight segment step (meters)
    const double ang_step = 5.0 * M_PI_LOCAL / 180.0; // arc step in radians

    // Normalize ownship heading to radians (0=north, clockwise positive).
    double heading = pmod(450.0 - osh, 360.0) * M_PI_LOCAL / 180.0;

    // Difference between heading and heading to target.
    double dx = cnx - osx;
    double dy = cny - osy;
    double psi = pmod(std::atan2(dy, dx) - heading + M_PI_LOCAL, 2 * M_PI_LOCAL) - M_PI_LOCAL;

    // Determine turning direction and angle to rotation center.
    double ang_to_cor = heading;
    double cw = -1.0;
    if (psi < 0) {
        ang_to_cor -= M_PI_LOCAL / 2.0;
        cw = -1.0;
    } else {
        ang_to_cor += M_PI_LOCAL / 2.0;
        cw = 1.0;
    }

    // Current position and center of rotation.
    double current_x = osx;
    double current_y = osy;
    double center_x = current_x + turn_radius * std::cos(ang_to_cor);
    double center_y = current_y + turn_radius * std::sin(ang_to_cor);

    double total_length = 0.0;

    // Move forward until the target is no longer strictly inside the turning circle.
    bool moved_forward = false;
    while (std::hypot(cny - center_y, cnx - center_x) < turn_radius) {
        current_x += step_size * std::cos(heading);
        current_y += step_size * std::sin(heading);
        center_x = current_x + turn_radius * std::cos(ang_to_cor);
        center_y = current_y + turn_radius * std::sin(ang_to_cor);
        moved_forward = true;
    }

    // If we moved forward, accumulate that straight segment.
    if (moved_forward) {
        total_length += std::hypot(current_y - osy, current_x - osx);
    }

    // Now follow the arc until we have a straight shot to the target.
    int steps = 1;
    while (true) {
        double nang = ang_to_cor + M_PI_LOCAL + (cw * steps * ang_step);
        double next_x = center_x + turn_radius * std::cos(nang);
        double next_y = center_y + turn_radius * std::sin(nang);

        // Vectors from current point to target and to next proposed arc point.
        double dv_x = cnx - current_x;
        double dv_y = cny - current_y;
        double du_x = next_x - current_x;
        double du_y = next_y - current_y;

        double ang_to_trg = std::atan2(dv_y, dv_x);
        double ang_to_np = std::atan2(du_y, du_x);
        double phi = ang_to_trg - ang_to_np;
        phi = pmod(phi + M_PI_LOCAL, 2 * M_PI_LOCAL) - M_PI_LOCAL;

        // Stop if the next point would overshoot the target direction or too many steps.
        if (cw * phi + ang_step < 0 || steps > 1000) {
            break;
        }

        // Move to the next arc point and add the segment length.
        total_length += std::hypot(next_y - current_y, next_x - current_x);
        current_x = next_x;
        current_y = next_y;
        steps++;
    }

    // Final straight segment to the target.
    total_length += std::hypot(cny - current_y, cnx - current_x);
    return total_length;
}

#include <cassert>
#include <cmath>

// Declaration of the function under test.
double predictedDubinsPathLength(double osx, double osy, double osh,
                                 double cnx, double cny,
                                 double turn_radius);

int main() {
    // Exact straight-line case: no turn needed, but the algorithm still does an arc.
    // Instead, test a simple case: ownship at (0,0), heading east (90 deg), target ahead straight on same heading.
    // With turn_radius very large, the path should be close to the direct distance.
    double dist1 = predictedDubinsPathLength(0,0, 90, 10,0, 1000.0);
    // The path should be at least 10 (direct) and not huge; with a huge radius the arc is shallow, but due to 5-degree steps the path may be slightly longer.
    assert(dist1 > 9.9 && dist1 < 20.0);

    // Target exactly at ownship? Distance should be 0.
    double dist2 = predictedDubinsPathLength(5,5, 45, 5,5, 3.0);
    assert(std::fabs(dist2) < 1e-9);

    // Target directly north (heading north = 0 deg). Turn radius small.
    // Place ownship at (0,0), heading 0, target at (0,1). The algorithm will turn (since heading to target is north, psi=0, so turns left/right).
    // The path length should be at least 1 (direct distance) and probably slightly more.
    double dist3 = predictedDubinsPathLength(0,0, 0, 0, 1, 3.0);
    assert(dist3 >= 1.0 && dist3 < 10.0);

    // Target behind ownship: heading north, target at (0,-1). Must turn around, path longer than 1.
    double dist4 = predictedDubinsPathLength(0,0, 0, 0, -1, 3.0);
    assert(dist4 > 1.0 && dist4 < 30.0);

    // Symmetry: ownship at (0,0), heading east, target at (10,0) vs (10,0) with heading east but turn radius 1.
    // Both should be similar, but due to discrete steps, just check they are positive and reasonable.
    double dist5a = predictedDubinsPathLength(0,0, 90, 10,0, 1.0);
    double dist5b = predictedDubinsPathLength(0,0, 90, 10,0, 1.0);
    assert(dist5a == dist5b);
    assert(dist5a >= 10.0 && dist5a < 20.0);

    // Test that a very small turn radius and a far target does not get stuck in the 1000-step loop.
    double dist6 = predictedDubinsPathLength(0,0, 90, 1000,0, 0.1);
    assert(dist6 > 1000.0 && dist6 < 2000.0);

    // Test with negative coordinates and non-quadrant headings.
    double dist7 = predictedDubinsPathLength(-3,-4, 210, 5, 7, 5.0);
    assert(dist7 > 0.0);

    // Verify that the path length is not negative or NaN.
    assert(std::isfinite(dist7));

    return 0;
}

// The algorithm replicates the snippet's `getTaskBid` logic. First, compute the heading in radians using the provided formula. Compute the difference angle `psi` between the heading and the vector to the target, normalized to `[-pi, pi]`. Based on the sign of `psi`, decide the turning direction (`cw = -1` for clockwise, `+1` for counterclockwise) and set the angle to the center of rotation as `heading ± pi/2`. The center of rotation is initially at `(osx + turn_radius*cos(ang_to_cor), osy + turn_radius*sin(ang_to_cor))`. Then, while the target is strictly inside the turning circle (distance from center to target < turn_radius), move the ownship forward along the current heading by `step_size = 0.5` meters, updating the center of rotation accordingly. After this loop, if the ownship moved at least half a step, add the new point to the trajectory. Then iterate over arc steps: for each step, compute the next point on the circle using `ang_to_cor + pi + cw*steps*5deg` (since the starting arc angle is opposite to the center direction). Compute the angle difference `phi` between the vector from current point to target and the vector from current point to next arc point. If `cw*phi + ang_step_size < 0` or `steps > 1000`, break; otherwise, move current point to next arc point, accumulate the straight-line distance between consecutive points, and increment `steps`. Finally, add the distance from the last current point to the target. The total path length is the sum of all straight segments. Edge cases: if the target is exactly on the turning circle at the start (distance == radius), the while loop does not execute, so no initial straight segment; if the target is inside, the loop moves forward until it's just outside or equal. The arc loop terminates when the next point would overshoot the target direction or after 1000 steps, which prevents infinite loops. Time complexity is O(steps) where steps is at most 1000 plus the forward-step loop (which could be many iterations if the target is far inside, but at most distance/0.5). Space complexity is O(1) since we only track current position and accumulated length.
