// Given a list of odometry measurements from a differential-drive robot, write a C++ function `computeDeltas` that calculates and returns the change in global X position, change in global Y position, and change in heading angle (alpha) for a single update step. The function must accept the current delta distances from the left, right, and middle tracking wheels (in inches), the current robot width (distance between left and right wheels, in inches), the distance from the middle wheel to the robot center (in inches), and the robot's previous heading angle `prev_alpha` (in radians). Use the provided formulas: delta_alpha = (delta_left - delta_right) / (width), delta_Dly = 2 * sin(delta_alpha/2) * (right_wheel_distance + (delta_right / delta_alpha)) if delta_alpha is nonzero, otherwise delta_Dly = delta_right; delta_Dlx = 2 * sin(delta_alpha/2) * ((delta_middle / delta_alpha) + middle_offset) if nonzero, otherwise delta_Dlx = delta_middle; then compute global deltas: delta_globalX = (delta_Dlx * cos(delta_alpha/2 + prev_alpha)) + (delta_Dly * sin(delta_alpha/2 + prev_alpha)) and delta_globalY = (delta_Dly * cos(delta_alpha/2 + prev_alpha)) - (delta_Dlx * sin(delta_alpha/2 + prev_alpha)). The function should return a small struct `OdometryDelta` containing three `double` fields: `deltaX`, `deltaY`, and `deltaAlpha`. Handle the degenerate case where `delta_alpha` is zero (straight-line motion) by falling back to the linear approximations without dividing by zero. The inputs are guaranteed to be finite and non-negative for distances, with width and middle offset strictly positive.
The solution directly implements the provided kinematic formulas for differential-drive odometry with a middle tracking wheel. The key steps are: first compute `delta_alpha` from the difference in left/right wheel distances divided by the track width. Then compute local displacements `Dlx` and `Dly` using the arc-based formulas, being careful to avoid division by zero when `delta_alpha` is exactly zero (or extremely close within floating-point tolerance; for the test we treat exact zero as the special case, but in practice a small epsilon is advisable). When `delta_alpha` is non-zero, we compute the local displacements using the sine-of-half-angle formula that models the robot's circular arc. Then we rotate the local displacements into the global frame using the average heading `(prev_alpha + delta_alpha/2)`. The formulas for `delta_globalX` and `delta_globalY` are direct applications of the rotation matrix for the robot's local-to-global transformation. The function returns a struct with three values. Edge cases include: zero `delta_alpha` where the robot moves straight, so `Dly` equals the right wheel distance and `Dlx` equals the middle wheel distance; negative or zero distances (not expected but safe to handle); and large `prev_alpha` values (no issue since trigonometric functions handle any radians). Time complexity is O(1) with O(1) auxiliary space, as only a few arithmetic operations and standard math functions are used.
#include <cmath>
#include <limits>

// Structure to hold the computed odometry delta values.
struct OdometryDelta {
    double deltaX;
    double deltaY;
    double deltaAlpha;
};

/**
 * Compute the change in global X, global Y, and heading angle (alpha)
 * from wheel encoder delta distances and the previous heading.
 *
 * @param delta_left   Change in left tracking wheel distance (inches).
 * @param delta_right  Change in right tracking wheel distance (inches).
 * @param delta_middle Change in middle tracking wheel distance (inches).
 * @param width        Distance between left and right wheels (inches, >0).
 * @param middle_offset Distance from middle wheel to robot center (inches, >0).
 * @param prev_alpha   Previous heading angle in radians.
 * @return OdometryDelta with deltaX, deltaY, deltaAlpha.
 */
OdometryDelta computeDeltas(double delta_left, double delta_right,
                            double delta_middle, double width,
                            double middle_offset, double prev_alpha)
{
    // Compute change in heading using arclength difference / width.
    double delta_alpha = (delta_left - delta_right) / width;

    // Guard against zero delta_alpha to avoid division by zero.
    // Use a small epsilon to also catch near-zero values for numerical stability.
    const double eps = 1e-12;
    double Dly, Dlx;

    if (std::abs(delta_alpha) < eps) {
        // Straight-line motion: use linear distances.
        Dly = delta_right;
        Dlx = delta_middle;
    } else {
        // Arc motion: use the sine-of-half-angle formulas.
        double half_alpha = delta_alpha / 2.0;
        double sin_half = 2.0 * std::sin(half_alpha);
        Dly = sin_half * (width / 2.0 + (delta_right / delta_alpha));
        Dlx = sin_half * ((delta_middle / delta_alpha) + middle_offset);
    }

    // Average heading used for rotation.
    double average_alpha = prev_alpha + delta_alpha / 2.0;

    // Rotate local displacements to global frame.
    double cos_avg = std::cos(average_alpha);
    double sin_avg = std::sin(average_alpha);
    double delta_globalX = (Dlx * cos_avg) + (Dly * sin_avg);
    double delta_globalY = (Dly * cos_avg) - (Dlx * sin_avg);

    return {delta_globalX, delta_globalY, delta_alpha};
}
#include <cassert>
#include <cmath>
#include <iostream>

// Declaration of the function under test.
OdometryDelta computeDeltas(double, double, double, double, double, double);

int main() {
    // Test 1: Straight forward (delta_alpha = 0), no rotation.
    // Left and right equal, middle moves forward.
    auto d1 = computeDeltas(2.0, 2.0, 2.0, 10.0, 5.0, 0.0);
    assert(std::abs(d1.deltaX - 2.0) < 1e-9);
    assert(std::abs(d1.deltaY - 2.0) < 1e-9); // Wait, check formula: Dly = right = 2, Dlx = middle = 2, alpha=0, so deltaX = 2*cos(0)+2*sin(0)=2, deltaY = 2*cos(0)-2*sin(0)=2. That's right.
    assert(std::abs(d1.deltaAlpha - 0.0) < 1e-9);

    // Test 2: Pure rotation, no translation.
    // Left moves forward, right moves backward, middle stays.
    // width = 10, so delta_alpha = (2 - (-2))/10 = 0.4 rad.
    // Middle distance = 0 -> Dlx = 2*sin(0.2)*(0/0.4 + 5) = 2*sin(0.2)*5 ≈ 1.9867
    // Dly = 2*sin(0.2)*(5 + (-2/0.4)) = 2*sin(0.2)*(5 - 5) = 0
    // With prev_alpha = 0, deltaX = Dlx*cos(0.2)+0 = 1.9867*cos(0.2)≈1.947, deltaY = 0*cos(0.2)-Dlx*sin(0.2)≈ -1.9867*0.1987≈ -0.3947
    auto d2 = computeDeltas(2.0, -2.0, 0.0, 10.0, 5.0, 0.0);
    assert(std::abs(d2.deltaAlpha - 0.4) < 1e-9);
    double expected_Dlx = 2.0 * std::sin(0.2) * 5.0; // ≈ 1.98669
    double expected_Dly = 0.0;
    double expected_dX = expected_Dlx * std::cos(0.2) + expected_Dly * std::sin(0.2);
    double expected_dY = expected_Dly * std::cos(0.2) - expected_Dlx * std::sin(0.2);
    assert(std::abs(d2.deltaX - expected_dX) < 1e-6);
    assert(std::abs(d2.deltaY - expected_dY) < 1e-6);

    // Test 3: Straight motion with offset heading.
    auto d3 = computeDeltas(3.0, 3.0, 3.0, 12.0, 4.0, M_PI_2);
    assert(std::abs(d3.deltaAlpha - 0.0) < 1e-9);
    // Dly = 3, Dlx = 3, avg_alpha = pi/2
    // deltaX = 3*cos(pi/2)+3*sin(pi/2) = 0 + 3 = 3
    // deltaY = 3*cos(pi/2)-3*sin(pi/2) = 0 - 3 = -3
    assert(std::abs(d3.deltaX - 3.0) < 1e-9);
    assert(std::abs(d3.deltaY + 3.0) < 1e-9);

    // Test 4: Arc with previous heading nonzero.
    // delta_left=4, delta_right=1, middle=2, width=10, middle_offset=3, prev_alpha=0.5
    double width=10.0, mo=3.0, prev=0.5;
    double dL=4.0, dR=1.0, dM=2.0;
    auto d4 = computeDeltas(dL, dR, dM, width, mo, prev);
    double da = (dL - dR)/width; // 0.3
    double half = da/2.0; // 0.15
    double sh = 2.0*std::sin(half); // 2*sin(0.15)
    double Dly = sh * (width/2.0 + (dR/da));
    double Dlx = sh * ((dM/da) + mo);
    double avg = prev + da/2.0;
    double expX = Dlx*std::cos(avg) + Dly*std::sin(avg);
    double expY = Dly*std::cos(avg) - Dlx*std::sin(avg);
    assert(std::abs(d4.deltaX - expX) < 1e-9);
    assert(std::abs(d4.deltaY - expY) < 1e-9);
    assert(std::abs(d4.deltaAlpha - da) < 1e-9);

    // Test 5: Negative delta_alpha (turning right).
    // delta_left=1, delta_right=3, delta_middle=0, width=8, middle_offset=2, prev=0
    auto d5 = computeDeltas(1.0, 3.0, 0.0, 8.0, 2.0, 0.0);
    double da5 = (1.0-3.0)/8.0; // -0.25
    assert(std::abs(d5.deltaAlpha + 0.25) < 1e-9);
    double half5 = -0.125;
    double sh5 = 2.0*std::sin(half5); // negative
    double Dly5 = sh5 * (4.0 + (3.0/(-0.25))); // sh5*(4-12)=sh5*(-8)
    double Dlx5 = sh5 * ((0.0/(-0.25)) + 2.0); // sh5*2
    double avg5 = half5;
    double expX5 = Dlx5*std::cos(avg5) + Dly5*std::sin(avg5);
    double expY5 = Dly5*std::cos(avg5) - Dlx5*std::sin(avg5);
    assert(std::abs(d5.deltaX - expX5) < 1e-9);
    assert(std::abs(d5.deltaY - expY5) < 1e-9);

    // Test 6: All zeros -> no movement.
    auto d6 = computeDeltas(0.0, 0.0, 0.0, 10.0, 5.0, 1.0);
    assert(std::abs(d6.deltaX) < 1e-12);
    assert(std::abs(d6.deltaY) < 1e-12);
    assert(std::abs(d6.deltaAlpha) < 1e-12);

    // Test 7: Large width, tiny delta_alpha (should be handled as non-zero).
    auto d7 = computeDeltas(0.1, 0.0, 0.1, 10000.0, 10.0, 0.0);
    // delta_alpha = 0.00001, which is > epsilon 1e-12, so non-zero path.
    double da7 = 0.1/10000.0;
    // Dly = 2*sin(5e-6)*(5000 + 0/da7) ≈ 2*5e-6*5000 = 0.05
    // Dlx = 2*sin(5e-6)*(0.1/5e-6 + 10) ≈ 1e-5*(20000+10)=0.2001
    // Average alpha = 2.5e-6, approximately 0.
    // deltaX ≈ Dlx + Dly*2.5e-6 ≈ 0.2001
    // deltaY ≈ Dly - Dlx*2.5e-6 ≈ 0.05
    assert(std::abs(d7.deltaAlpha - da7) < 1e-12);
    assert(std::abs(d7.deltaX - 0.2001) < 1e-4);
    assert(std::abs(d7.deltaY - 0.05) < 1e-4);

    std::cout << "All tests passed." << std::endl;
    return 0;
}
