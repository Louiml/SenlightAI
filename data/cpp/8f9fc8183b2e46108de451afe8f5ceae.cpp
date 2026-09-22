Write a C++ function that takes two `Eigen::Quaternion<double>` objects `a` and `b` and a double interpolation parameter `t` (interpreted as the fraction of the path from `a` to `b`, where `t=0` returns `a` and `t=1` returns `b`). The function must perform spherical linear interpolation (slerp) using the closed-form formula: compute the dot product `d = a.dot(b)`; if `d` is negative, flip the sign of `b` (i.e., use `-b`) to take the shorter arc; let `theta = acos(|d|)`, and if `theta` is near zero (say, `theta < 1e-10`), return `a` unchanged to avoid division by zero; otherwise compute `scale0 = sin((1-t)*theta)/sin(theta)` and `scale1 = sin(t*theta)/sin(theta)`, and return the normalized quaternion formed by `scale0*a.coeffs() + scale1*b.coeffs()` (using the flipped `b` if needed). Ensure the function is robust for edge cases like identical quaternions, opposite quaternions, and values of `t` outside the closed interval `[0,1]` (still extrapolate gracefully). The function must be named `slerp_manual` and should not rely on Eigen’s built-in `slerp`. Your implementation must include the necessary `#include <Eigen/Geometry>` and must compile standalone with a test harness.

// The core algorithm follows the standard quaternion slerp formulation. First, compute the dot product `d = a.dot(b)`. If `d` is negative, replace `b` with `-b` (negate all coefficients) to ensure the interpolation takes the shorter great-circle arc, then set `d = -d`. Let `theta = acos(d)`, which is in `[0, π]`. If `theta` is exceedingly small (or `d` is very close to 1), the two quaternions are nearly identical; in this case, using the formula would involve `sin(theta) ≈ 0`, causing division by zero. To handle this, if `theta < 1e-10`, return `a` directly (or alternatively, use linear interpolation). For normal cases, compute the two sine ratios: `scale0 = sin((1 - t) * theta) / sin(theta)` and `scale1 = sin(t * theta) / sin(theta)`. Then compute the weighted sum of coeffs and normalize the resulting quaternion (although mathematically the result should already be unit norm, numerical errors are reduced by normalizing). This approach works for any real `t`, including outside `[0,1]`, and for opposite quaternions (where `d = -1`, leading to `theta = π`, and the formula remains valid because `sin(π) = 0`? Actually, if `abs(d)=1` exactly, `theta = 0` or `π`, and `sin(theta)=0`. But after flipping, `abs(d)` becomes 1 only if `a` and `b` are identical or opposite. For identical, `theta≈0` handled above. For opposite, after flipping `b`, `d = -(-1) = 1`, so it reduces to the identical case and returns `a`. The edge case where `a` and `b` are exactly opposite but after flipping they become identical, so returning `a` is correct since both `b` and `-b` represent the same rotation, and any interpolation is ambiguous; choosing `a` is acceptable.) The time complexity is O(1) arithmetic operations (a few trig functions and vector operations), and space complexity is O(1) beyond the inputs. The implementation should use `Eigen::Quaterniond` (or `Eigen::Quaternion<double>`) and its `coeffs()` method, which returns a 4D vector in the order `(x, y, z, w)`. The solution must correctly handle `const` correctness: both input quaternions are taken by const reference, and the return value is a quaternion by value.

#include <Eigen/Geometry>
#include <cmath>

// Perform spherical linear interpolation between two quaternions a and b.
// t=0 gives a, t=1 gives b, other t values interpolate or extrapolate.
// Handles the shorter arc by flipping b if needed, and avoids division by zero.
Eigen::Quaternion<double> slerp_manual(const Eigen::Quaternion<double>& a,
                                       const Eigen::Quaternion<double>& b,
                                       double t) {
    // Compute dot product
    double d = a.dot(b);

    // If the dot product is negative, flip b to take the shorter arc
    Eigen::Quaternion<double> b_eff = b;
    if (d < 0.0) {
        b_eff.coeffs() = -b.coeffs();
        d = -d;
    }

    // Clamp d to [-1, 1] to avoid numerical issues
    if (d > 1.0) d = 1.0;
    if (d < -1.0) d = -1.0;

    double theta = std::acos(d); // angle between quaternions, in [0, pi]

    // Handle near-zero angle (identical quaternions) to avoid division by zero
    if (theta < 1e-10) {
        return a;
    }

    double sinTheta = std::sin(theta);

    // Compute the two scale factors
    double scale0 = std::sin((1.0 - t) * theta) / sinTheta;
    double scale1 = std::sin(t * theta) / sinTheta;

    // Combine and normalize
    Eigen::Quaternion<double> result;
    result.coeffs() = scale0 * a.coeffs() + scale1 * b_eff.coeffs();
    result.normalize();

    return result;
}

#include <cassert>
#include <cmath>
#include <Eigen/Geometry>

// The solution function is declared here (or include the header where it's defined)
Eigen::Quaternion<double> slerp_manual(const Eigen::Quaternion<double>& a,
                                       const Eigen::Quaternion<double>& b,
                                       double t);

int main() {
    // Test 1: t=0 gives a exactly
    Eigen::Quaternion<double> a(1.0, 0.0, 0.0, 0.0); // w=1, x=0,y=0,z=0 (identity)
    Eigen::Quaternion<double> b(0.0, 1.0, 0.0, 0.0); // 90-degree rotation
    Eigen::Quaternion<double> r = slerp_manual(a, b, 0.0);
    assert((r.coeffs() - a.coeffs()).norm() < 1e-12);

    // Test 2: t=1 gives b (or -b) exactly after normalization
    r = slerp_manual(a, b, 1.0);
    assert(std::abs(r.dot(b)) > 0.999999);

    // Test 3: midpoint of identity and 90-degree rotation is 45-degree rotation
    r = slerp_manual(a, b, 0.5);
    // Expected quaternion: cos(22.5°) + sin(22.5°)*axis(0,0,1)
    double expected_w = std::cos(22.5 * M_PI / 180.0);
    double expected_z = std::sin(22.5 * M_PI / 180.0);
    assert(std::abs(r.w() - expected_w) < 1e-6);
    assert(std::abs(r.z() - expected_z) < 1e-6);
    assert(std::abs(r.x()) < 1e-6 && std::abs(r.y()) < 1e-6);

    // Test 4: identical quaternions (theta ≈ 0) returns a
    Eigen::Quaternion<double> same(0.70710678, 0.0, 0.0, 0.70710678);
    r = slerp_manual(same, same, 0.3);
    assert((r.coeffs() - same.coeffs()).norm() < 1e-12);

    // Test 5: opposite quaternions (a and -a) – after flipping, becomes identical
    Eigen::Quaternion<double> opp(-0.70710678, 0.0, 0.0, -0.70710678);
    r = slerp_manual(same, opp, 0.7);
    // Both represent same rotation, result should be close to a
    assert((r.coeffs() - same.coeffs()).norm() < 1e-6 || (r.coeffs() + same.coeffs()).norm() < 1e-6);

    // Test 6: extrapolation outside [0,1] still produces a unit quaternion
    r = slerp_manual(a, b, 1.5);
    assert(std::abs(r.norm() - 1.0) < 1e-12);

    // Test 7: symmetric property: slerp(a,b,0.3) should be close to slerp(b,a,0.7)
    Eigen::Quaternion<double> r1 = slerp_manual(a, b, 0.3);
    Eigen::Quaternion<double> r2 = slerp_manual(b, a, 0.7);
    double dot = std::abs(r1.dot(r2));
    assert(dot > 0.999999);

    // Test 8: continuity: t=ε gives near a
    r = slerp_manual(a, b, 1e-6);
    assert((r.coeffs() - a.coeffs()).norm() < 1e-5);

    // Test 9: non-normalized input quaternions are handled by normalizing output
    Eigen::Quaternion<double> non_unit_a(2.0, 0.0, 0.0, 0.0);
    Eigen::Quaternion<double> non_unit_b(0.0, 3.0, 0.0, 0.0);
    r = slerp_manual(non_unit_a, non_unit_b, 0.5);
    assert(std::abs(r.norm() - 1.0) < 1e-12);

    // Test 10: known numerical example: 180-degree rotation about X
    Eigen::Quaternion<double> q1(1.0, 0.0, 0.0, 0.0); // identity
    Eigen::Quaternion<double> q2(0.0, 1.0, 0.0, 0.0); // 180° about X
    r = slerp_manual(q1, q2, 0.5);
    // Interpolation should be 90° about X: (0, sqrt(0.5), 0, 0)
    assert(std::abs(r.x()) < 1e-6);
    assert(std::abs(r.y() - sqrt(0.5)) < 1e-6);
    assert(std::abs(r.z()) < 1e-6);
    assert(std::abs(r.w() - sqrt(0.5)) < 1e-6);

    return 0;
}
