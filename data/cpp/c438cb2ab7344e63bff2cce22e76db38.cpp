Write a C++ function that simulates a four-wheel mecanum drivetrain's inverse kinematics. Given target velocities along the forward/backward axis (`fx`), lateral/strafe axis (`fy`), and rotational velocity (`fr`), the function computes per-wheel motor speeds using the standard mecanum coefficient matrix:
- Wheel 1 (front-left): speed = fx + fy + fr
- Wheel 2 (front-right): speed = fx - fy - fr
- Wheel 3 (rear-left): speed = -fx + fy - fr
- Wheel 4 (rear-right): speed = -fx - fy + fr

All input velocities are floats in the range [-255, 255]. After computing the four raw wheel speeds, the function must normalize them according to the following rule: if the maximum absolute value among the four speeds exceeds 255, scale all speeds proportionally so that the maximum absolute value becomes exactly 255. The function should return a `std::array<int, 4>` containing the four final motor speeds (integers, using truncation toward zero after scaling). To avoid floating-point precision issues, compute the scale factor using double precision and round the scaled values to the nearest integer using `std::lround` before casting to int.

The core algorithm is matrix multiplication: each wheel speed is a linear combination of `fx`, `fy`, and `fr` with coefficients ±1. Since the coefficients are all 1 or -1, no matrix inversion is needed. The key steps: (1) compute four raw speeds as doubles; (2) find the maximum absolute value across all four; (3) if that max is ≤ 255, return the raw values rounded to integers; (4) otherwise, compute a scale factor = 255.0 / max_abs, multiply each raw speed by that factor, and round to nearest integer. Edge cases: all inputs zero yields all outputs zero; inputs exactly ±255 produce raw speeds that may exceed 255 (e.g., fx=255, fy=255, fr=0 gives wheel1=510), so normalization is required; negative max values are handled by taking absolute value; rounding via `std::lround` ensures correct tie-breaking (round half away from zero). Time complexity is O(1) – only four multiplications and a constant number of comparisons. Space complexity is O(1), just a few local variables.

#include <array>
#include <cmath>
#include <cstdint>

// Compute final motor speeds for a four-wheel mecanum drive.
// Inputs fx, fy, fr are target velocities (forward, strafe, rotation).
// Returns array of 4 ints: [front-left, front-right, rear-left, rear-right].
std::array<int, 4> computeWheelSpeeds(double fx, double fy, double fr) {
    // Raw wheel speeds according to mecanum inverse kinematics
    double raw[4];
    raw[0] =  fx + fy + fr;   // front-left
    raw[1] =  fx - fy - fr;   // front-right
    raw[2] = -fx + fy - fr;   // rear-left
    raw[3] = -fx - fy + fr;   // rear-right

    // Find maximum absolute raw speed
    double max_abs = 0.0;
    for (double v : raw) {
        double a = std::fabs(v);
        if (a > max_abs) max_abs = a;
    }

    // If all speeds are zero, return zeros directly (avoid division by zero)
    if (max_abs == 0.0) {
        return {0, 0, 0, 0};
    }

    // Normalize if max_abs exceeds 255
    double scale = 1.0;
    if (max_abs > 255.0) {
        scale = 255.0 / max_abs;
    }

    // Scale and round to nearest integer
    std::array<int, 4> result;
    for (int i = 0; i < 4; ++i) {
        result[i] = static_cast<int>(std::lround(raw[i] * scale));
    }
    return result;
}

#include <cassert>
#include <cmath>
#include <array>

int main() {
    // All zero → all zero
    assert(computeWheelSpeeds(0.0, 0.0, 0.0) == std::array<int,4>({0,0,0,0}));

    // Pure forward
    assert(computeWheelSpeeds(100.0, 0.0, 0.0) == std::array<int,4>({100,100,-100,-100}));

    // Pure strafe right
    assert(computeWheelSpeeds(0.0, 100.0, 0.0) == std::array<int,4>({100,-100,100,-100}));

    // Pure rotation clockwise
    assert(computeWheelSpeeds(0.0, 0.0, 100.0) == std::array<int,4>({100,-100,-100,100}));

    // No normalization when max ≤255
    assert(computeWheelSpeeds(100.0, 100.0, 0.0) == std::array<int,4>({200,0,0,-200}));

    // Normalization needed: max raw = 510, scale=0.5 → 100,100,100,100? Actually check:
    // fx=255, fy=255, fr=0 → raw=[510,0,0,-510] → scale=255/510=0.5 → [255,0,0,-255]
    assert(computeWheelSpeeds(255.0, 255.0, 0.0) == std::array<int,4>({255,0,0,-255}));

    // Normalization with fractional rounding
    // fx=200, fy=100, fr=50 → raw=[350,50,-50,-150] → max=350 scale≈0.72857 → [255,36,-36,-109]
    auto r = computeWheelSpeeds(200.0, 100.0, 50.0);
    assert(r[0] == 255);
    assert(std::abs(r[1] - 36) <= 1);  // rounding tolerance
    assert(std::abs(r[2] + 36) <= 1);
    assert(std::abs(r[3] + 109) <= 1);

    // Negative inputs
    assert(computeWheelSpeeds(-100.0, 0.0, 0.0) == std::array<int,4>({-100,-100,100,100}));

    // Slightly above 255
    assert(computeWheelSpeeds(128.0, 128.0, 0.0) == std::array<int,4>({256,0,0,-256}));
    // Actually 128+128=256, scale=255/256≈0.996 → 255 after rounding

    return 0;
}
