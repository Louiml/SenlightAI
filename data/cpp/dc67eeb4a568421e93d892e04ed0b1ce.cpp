Write a C++ function `rotationMatrixFromRodrigues` that takes a 3-element vector of floating-point values representing a rotation in Rodrigues form (rotation axis scaled by rotation angle in radians) and returns a 3×3 rotation matrix as a `std::array<std::array<double, 3>, 3>`. If the input vector is the zero vector (norm ≈ 0), return the identity matrix. The function must handle negative components correctly, compute the cosine and sine of the angle, and apply the standard Rodrigues rotation formula: R = I + sin(θ)·[v]ₓ + (1−cos(θ))·[v]ₓ², where v is the unit rotation axis. Ensure the result is a valid rotation matrix (orthonormal with determinant +1). The function must be `const`-correct (accept the input by const reference) and use only standard library facilities.

#include <cassert>
#include <cmath>
#include <array>

// Declaration from the solution (must be included or defined above).
std::array<std::array<double, 3>, 3> rotationMatrixFromRodrigues(
    const std::array<double, 3>& rodrigues);

bool isClose(double a, double b, double eps = 1e-6) {
    return std::fabs(a - b) < eps;
}

int main() {
    // Zero vector returns identity.
    auto I = rotationMatrixFromRodrigues({0.0, 0.0, 0.0});
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            assert(isClose(I[i][j], (i == j) ? 1.0 : 0.0));

    // 90-degree rotation about Z axis: (0, 0, pi/2)
    auto Rz = rotationMatrixFromRodrigues({0.0, 0.0, M_PI/2.0});
    assert(isClose(Rz[0][0], 0.0));
    assert(isClose(Rz[0][1], -1.0));
    assert(isClose(Rz[1][0], 1.0));
    assert(isClose(Rz[1][1], 0.0));
    assert(isClose(Rz[2][2], 1.0));

    // 180-degree rotation about X axis: (pi, 0, 0)
    auto Rx = rotationMatrixFromRodrigues({M_PI, 0.0, 0.0});
    assert(isClose(Rx[0][0], 1.0));
    assert(isClose(Rx[1][1], -1.0));
    assert(isClose(Rx[2][2], -1.0));

    // Rotation by 2*pi*2/3 about axis (1,1,1) normalized: should yield a known matrix.
    // Axis (1,1,1)/sqrt(3), angle = 4*pi/3.
    double axis_len = std::sqrt(3.0);
    auto R120 = rotationMatrixFromRodrigues({4*M_PI/3.0/axis_len, 4*M_PI/3.0/axis_len, 4*M_PI/3.0/axis_len});
    // Known result: a cyclic permutation matrix (0,0,1; 1,0,0; 0,1,0) for 120° about (1,1,1).
    assert(isClose(R120[0][2], 1.0));
    assert(isClose(R120[1][0], 1.0));
    assert(isClose(R120[2][1], 1.0));
    // Other entries should be ~0.
    assert(isClose(R120[0][0], 0.0));
    assert(isClose(R120[1][1], 0.0));
    assert(isClose(R120[2][2], 0.0));

    // Orthonormality check for an arbitrary rotation: R^T * R should be identity.
    auto Rarb = rotationMatrixFromRodrigues({0.3, -0.7, 0.2});
    auto RtR = std::array<std::array<double, 3>, 3>{{ {0,0,0},{0,0,0},{0,0,0} }};
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                RtR[i][j] += Rarb[k][i] * Rarb[k][j]; // R^T * R
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            assert(isClose(RtR[i][j], (i == j) ? 1.0 : 0.0));

    return 0;
}

#include <array>
#include <cmath>
#include <stdexcept>

// Convert a Rodrigues rotation vector (axis * angle) to a 3x3 rotation matrix.
// Throws std::invalid_argument if the input vector does not have exactly 3 elements.
std::array<std::array<double, 3>, 3> rotationMatrixFromRodrigues(
    const std::array<double, 3>& rodrigues) {
    const double rx = rodrigues[0];
    const double ry = rodrigues[1];
    const double rz = rodrigues[2];

    const double angle_sq = rx*rx + ry*ry + rz*rz;
    const double angle = std::sqrt(angle_sq);

    // Identity matrix by default.
    std::array<std::array<double, 3>, 3> R = {{
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    }};

    // Zero rotation vector means no rotation.
    if (angle < 1e-9) {
        return R;
    }

    // Unit axis.
    const double inv_angle = 1.0 / angle;
    const double kx = rx * inv_angle;
    const double ky = ry * inv_angle;
    const double kz = rz * inv_angle;

    const double c = std::cos(angle);
    const double s = std::sin(angle);
    const double one_minus_c = 1.0 - c;

    // Rodrigues formula: R = I + sin(angle)*K + (1-cos(angle))*K^2
    // where K is the skew-symmetric matrix of the unit axis.
    R[0][0] = c + kx*kx * one_minus_c;
    R[0][1] = kx*ky * one_minus_c - kz * s;
    R[0][2] = ky * s + kx*kz * one_minus_c;

    R[1][0] = kz * s + kx*ky * one_minus_c;
    R[1][1] = c + ky*ky * one_minus_c;
    R[1][2] = -kx * s + ky*kz * one_minus_c;

    R[2][0] = -ky * s + kx*kz * one_minus_c;
    R[2][1] = kx * s + ky*kz * one_minus_c;
    R[2][2] = c + kz*kz * one_minus_c;

    return R;
}

// The core algorithm is the Rodrigues rotation formula, which converts a rotation axis-angle representation into a rotation matrix. Given a vector `r = (rx, ry, rz)`, the rotation angle is `a = sqrt(rx² + ry² + rz²)`. If `a` is zero (or extremely close to zero within a tolerance like 1e-9), there is no rotation, so the identity matrix is returned. Otherwise, the unit axis is `(rx/a, ry/a, rz/a)`. The rotation matrix is computed as: `R = I + sin(a)·K + (1−cos(a))·K²`, where `K` is the skew-symmetric matrix of the unit axis. Expanding this yields the explicit 3×3 entries. Edge cases include the zero vector (no rotation), the vector already being a unit vector (so `a` equals the angle), and very small angles where `cos` and `sin` are well-defined. The result must be numerically stable; using `std::cos` and `std::sin` from `<cmath>` is sufficient. Time complexity is O(1), and space complexity is O(1) since only a fixed 3×3 matrix is returned. No external dependencies are required.
