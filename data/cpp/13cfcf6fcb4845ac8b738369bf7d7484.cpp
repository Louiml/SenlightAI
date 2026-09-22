Write a C++ function `double computeSteeringAngle(const double pos_cur[2], const double vehicle_v[2], const double pos_target[2])` that computes the signed angle (in radians) needed to steer a vehicle from its current position and velocity toward a target position in a 2D plane. The angle is defined as the signed rotation from the vehicle’s current heading (normalized velocity vector) to the desired heading vector (from current position to target). The angle should be in the interval `[-π, π]`, where positive means a left turn and negative means a right turn. If the vehicle’s speed is too small (norm < 1e-6) or the target is coincident with the current position (distance < 1e-6), return `0.0` because no meaningful steering is needed. The function must be robust to floating‑point inaccuracies (e.g., clamp the dot product to [-1,1] before calling `acos`). Use `const` for input arrays.

#include <cassert>
#include <cmath>

int main() {
    const double tol = 1e-9;

    // Case 1: Moving east, target north → left turn +90° (π/2)
    double pos1[2] = {0.0, 0.0};
    double vel1[2] = {1.0, 0.0};
    double tgt1[2] = {0.0, 1.0};
    assert(std::abs(computeSteeringAngle(pos1, vel1, tgt1) - M_PI_2) < tol);

    // Case 2: Moving east, target south → right turn -90° (-π/2)
    double tgt2[2] = {0.0, -1.0};
    assert(std::abs(computeSteeringAngle(pos1, vel1, tgt2) + M_PI_2) < tol);

    // Case 3: Already heading directly to target → angle 0
    double tgt3[2] = {5.0, 0.0};
    assert(std::abs(computeSteeringAngle(pos1, vel1, tgt3)) < tol);

    // Case 4: Head-on opposite direction → angle π
    double tgt4[2] = {-5.0, 0.0};
    assert(std::abs(std::abs(computeSteeringAngle(pos1, vel1, tgt4)) - M_PI) < tol);

    // Case 5: Zero velocity → returns 0
    double vel0[2] = {0.0, 0.0};
    assert(computeSteeringAngle(pos1, vel0, tgt3) == 0.0);

    // Case 6: Target equals current position → returns 0
    double pos2[2] = {3.0, -2.0};
    double tgt_same[2] = {3.0, -2.0};
    assert(computeSteeringAngle(pos2, vel1, tgt_same) == 0.0);

    // Case 7: Non‑axis aligned, e.g., velocity (1,1) heading (0,1) → angle +45° (π/4)
    double vel7[2] = {1.0, 1.0};
    double tgt7[2] = {0.0, 1.0};
    double expected = M_PI / 4.0;
    assert(std::abs(computeSteeringAngle(pos1, vel7, tgt7) - expected) < tol);

    // Case 8: Velocity (1,1) heading (1,0) → angle -45° (-π/4)
    double tgt8[2] = {1.0, 0.0};
    assert(std::abs(computeSteeringAngle(pos1, vel7, tgt8) + expected) < tol);
}

#include <cmath>
#include <algorithm>

// Compute the signed steering angle (radians) from current velocity toward target.
// Positive angle = left turn, negative = right turn. Returns 0.0 if undefined.
double computeSteeringAngle(const double pos_cur[2], const double vehicle_v[2], const double pos_target[2]) {
    const double tol = 1e-6;

    // Heading vector from current to target (x, y)
    const double hdg_x = pos_target[0] - pos_cur[0];
    const double hdg_y = pos_target[1] - pos_cur[1];

    // Compute norms
    const double v_norm = std::sqrt(vehicle_v[0] * vehicle_v[0] + vehicle_v[1] * vehicle_v[1]);
    const double hdg_norm = std::sqrt(hdg_x * hdg_x + hdg_y * hdg_y);

    // If speed or distance is negligible, no meaningful angle
    if (v_norm < tol || hdg_norm < tol) {
        return 0.0;
    }

    // Unit vectors
    const double v_unit[2] = { vehicle_v[0] / v_norm, vehicle_v[1] / v_norm };
    const double hdg_unit[2] = { hdg_x / hdg_norm, hdg_y / hdg_norm };

    // Dot product and clamp to avoid acos domain errors
    double dot_val = v_unit[0] * hdg_unit[0] + v_unit[1] * hdg_unit[1];
    dot_val = std::max(-1.0, std::min(1.0, dot_val));
    const double angle = std::acos(dot_val); // in [0, π]

    // 2D cross product to determine sign
    const double cross_val = v_unit[0] * hdg_unit[1] - v_unit[1] * hdg_unit[0];

    // Positive cross → left turn (angle positive), negative → right turn (negative)
    return (cross_val < 0.0) ? -angle : angle;
}

// The algorithm implements the pure‑pursuit method for 2D navigation, which turns the vehicle directly toward the target. First, compute the heading vector `H = pos_target - pos_cur` (note: use indices `[0]` and `[1]` correctly for x and y coordinates). Then compute the Euclidean norms of the velocity vector `V` and heading vector `H`. If either norm is below a tolerance (say `1e-6`), return `0.0` to avoid division by zero or undefined angles. Normalize both vectors to unit vectors `v_unit` and `hdg_unit`. The unsigned angle between them is `angle = acos(clamp(dot(v_unit, hdg_unit), -1.0, 1.0))`. To decide the sign (left positive, right negative), compute the 2D scalar cross product `cross = v_unit[0]*hdg_unit[1] - v_unit[1]*hdg_unit[0]`. If the cross product is negative, the vehicle must turn right, so multiply the angle by `-1.0`; otherwise, keep it positive. This yields a signed angle in `[-π, π]`. Edge cases: zero velocity, target at the current position, and near‑parallel or anti‑parallel vectors are handled by the tolerance check and the clamping. Time complexity is O(1) and space complexity is O(1) because only a fixed number of local variables are used.
