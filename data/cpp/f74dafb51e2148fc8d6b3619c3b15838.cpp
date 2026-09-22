/*
Write a C++ function that takes a 6-dimensional acceleration vector (linear and angular accelerations), a 6-dimensional velocity vector (linear and angular velocities), and a 3x3 rotation matrix, and computes the inverse dynamics force/torque vector for a simplified submerged rigid body. The function must implement the standard underwater vehicle dynamics equation: τ = M·a + C(ν)·ν + D(ν)·ν + g(η), where M is a constant 6x6 inertia matrix (including added mass), C(ν) is the Coriolis/centripetal matrix, D(ν) is a simplified linear-plus-quadratic damping matrix (diagonal with linear coefficient on linear velocities and quadratic coefficient on angular velocities), and g(η) is the gravity/buoyancy vector (assumed zero for this task). You must hardcode a specific constant inertia matrix (given in the solution) and implement the matrices algebraically. The function should return a 6-dimensional vector of forces and torques (fx, fy, fz, tx, ty, tz). Test with an identity rotation matrix, zero acceleration, and unit velocity vector, expecting a specific result you can verify manually.
*/
#include <array>
#include <cmath>

// Simplified underwater vehicle inverse dynamics.
// Returns 6D force/torque vector (fx, fy, fz, tx, ty, tz) given acceleration, velocity, and rotation matrix.
// The rotation matrix is not actually used in this simplified model (gravity assumed zero), but kept for interface completeness.
std::array<double, 6> inverse_dynamics(
    const std::array<double, 6>& acc,
    const std::array<double, 6>& vel,
    const std::array<std::array<double, 3>, 3>& rot)
{
    // Constant inertia matrix (including added mass) - hardcoded for a symmetric body.
    // Partitioned into 4 3x3 blocks M11, M12, M21, M22.
    const double M11[3][3] = {{25.0, 0.0, 0.0}, {0.0, 25.0, 0.0}, {0.0, 0.0, 25.0}};
    const double M12[3][3] = {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
    const double M21[3][3] = {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
    const double M22[3][3] = {{8.0, 0.0, 0.0}, {0.0, 8.0, 0.0}, {0.0, 0.0, 8.0}};

    // Damping coefficients.
    const double d_lin = 0.2;   // linear damping coefficient for linear velocities
    const double d_quad = 0.5;  // quadratic damping coefficient for angular velocities

    // Compute M * acc (6x6 matrix times 6x1 vector).
    std::array<double, 6> M_acc = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            M_acc[i] += M11[i][j] * acc[j] + M12[i][j] * acc[j + 3];
            M_acc[i + 3] += M21[i][j] * acc[j] + M22[i][j] * acc[j + 3];
        }
    }

    // Compute C(vel) * vel using the skew-symmetric matrix approach.
    // Define a helper for skew-symmetric matrix-vector product: cross_product(a, b) = a × b.
    auto cross = [](const std::array<double, 3>& a, const std::array<double, 3>& b) {
        return std::array<double, 3>{a[1]*b[2] - a[2]*b[1],
                                     a[2]*b[0] - a[0]*b[2],
                                     a[0]*b[1] - a[1]*b[0]};
    };

    // Compute a1 = M11 * vel_lin + M12 * vel_ang, and a2 = M21 * vel_lin + M22 * vel_ang.
    std::array<double, 3> lin_part = {0.0, 0.0, 0.0};
    std::array<double, 3> ang_part = {0.0, 0.0, 0.0};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            lin_part[i] += M11[i][j] * vel[j] + M12[i][j] * vel[j + 3];
            ang_part[i] += M21[i][j] * vel[j] + M22[i][j] * vel[j + 3];
        }
    }

    // Coriolis terms: C(vel) * vel = [ cross(lin_part, vel_ang); cross(lin_part, vel_lin) + cross(ang_part, vel_ang) ].
    // However, standard form gives: [ cross(lin_part, vel_ang); cross(lin_part, vel_lin) + cross(ang_part, vel_ang) ]?
    // Actually for our simplified model with M12=M21=0, lin_part = M11*vel_lin and ang_part = M22*vel_ang.
    // The correct Coriolis is: [ cross(M11*vel_lin, vel_ang); cross(M11*vel_lin, vel_lin) + cross(M22*vel_ang, vel_ang) ].
    // But cross(v,v)=0, so second term reduces to cross(M22*vel_ang, vel_ang) = 0 as well? Wait, cross(M22*vel_ang, vel_ang) is not necessarily zero because M22 scales components differently.
    // Standard formula: C(ν)ν = [ (M11 ν1 + M12 ν2) × ν2 ; (M11 ν1 + M12 ν2) × ν1 + (M21 ν1 + M22 ν2) × ν2 ].
    std::array<double, 3> vel_lin = {vel[0], vel[1], vel[2]};
    std::array<double, 3> vel_ang = {vel[3], vel[4], vel[5]};
    // Here a = M11*vel_lin + M12*vel_ang = lin_part (since M12=0) and b = M21*vel_lin + M22*vel_ang = ang_part.
    std::array<double, 3> coriolis_lin = cross(lin_part, vel_ang);
    std::array<double, 3> coriolis_ang = cross(lin_part, vel_lin) + cross(ang_part, vel_ang);

    // Damping vector: D(ν)ν = [d_lin * |ν_lin| * ν_lin ; d_quad * |ν_ang| * ν_ang].
    double lin_mag = std::sqrt(vel_lin[0]*vel_lin[0] + vel_lin[1]*vel_lin[1] + vel_lin[2]*vel_lin[2]);
    double ang_mag = std::sqrt(vel_ang[0]*vel_ang[0] + vel_ang[1]*vel_ang[1] + vel_ang[2]*vel_ang[2]);
    std::array<double, 6> damping;
    for (int i = 0; i < 3; ++i) {
        damping[i] = d_lin * lin_mag * vel_lin[i];
        damping[i + 3] = d_quad * ang_mag * vel_ang[i];
    }

    // Gravity/buoyancy term is zero for this model.
    // Assemble the final result tau = M_acc + Coriolis + damping.
    std::array<double, 6> tau;
    for (int i = 0; i < 3; ++i) {
        tau[i] = M_acc[i] + coriolis_lin[i] + damping[i];
        tau[i + 3] = M_acc[i + 3] + coriolis_ang[i] + damping[i + 3];
    }
    (void)rot; // rotation matrix not used in this simplified model
    return tau;
}
#include <array>
#include <cassert>
#include <cmath>

// Forward declaration of the solution function (assume it's in the same translation unit).
std::array<double, 6> inverse_dynamics(
    const std::array<double, 6>& acc,
    const std::array<double, 6>& vel,
    const std::array<std::array<double, 3>, 3>& rot);

int main() {
    // Identity rotation matrix (not used but provided).
    std::array<std::array<double, 3>, 3> I = {{
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    }};

    // Test 1: zero acceleration, unit velocity (all ones). 
    // Expected: M_acc=0, Coriolis terms and damping computed manually.
    std::array<double, 6> acc0 = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    std::array<double, 6> vel1 = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    auto tau1 = inverse_dynamics(acc0, vel1, I);
    // Manual calculation:
    // vel_lin = (1,1,1), vel_ang=(1,1,1)
    // lin_part = M11*vel_lin = (25,25,25)
    // ang_part = M22*vel_ang = (8,8,8)
    // coriolis_lin = cross((25,25,25), (1,1,1)) = (25*1-25*1, 25*1-25*1, 25*1-25*1) = (0,0,0)
    // coriolis_ang = cross((25,25,25), (1,1,1)) + cross((8,8,8), (1,1,1)) = (0,0,0)+(0,0,0) = (0,0,0)
    // damping_lin = 0.2 * sqrt(3) * (1,1,1) ≈ (0.3464,0.3464,0.3464)
    // damping_ang = 0.5 * sqrt(3) * (1,1,1) ≈ (0.8660,0.8660,0.8660)
    // Expected tau = (0+0+0.3464, 0+0+0.3464, 0+0+0.3464, 0+0+0.8660, ...)
    double expected_lin = 0.2 * std::sqrt(3.0);
    double expected_ang = 0.5 * std::sqrt(3.0);
    for (int i = 0; i < 3; ++i) {
        assert(std::abs(tau1[i] - expected_lin) < 1e-9);
        assert(std::abs(tau1[i+3] - expected_ang) < 1e-9);
    }

    // Test 2: zero velocity, zero acceleration → tau should be zero.
    std::array<double, 6> vel0 = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    auto tau2 = inverse_dynamics(acc0, vel0, I);
    for (auto v : tau2) {
        assert(std::abs(v) < 1e-9);
    }

    // Test 3: acceleration only in x-linear, zero velocity.
    std::array<double, 6> accx = {2.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    auto tau3 = inverse_dynamics(accx, vel0, I);
    // Expected: M_acc = M * acc = (25*2, 0, 0, 0, 0, 0) = (50,0,0,0,0,0)
    assert(std::abs(tau3[0] - 50.0) < 1e-9);
    for (int i = 1; i < 6; ++i) {
        assert(std::abs(tau3[i]) < 1e-9);
    }

    // Test 4: velocity only in z-linear, zero acceleration.
    std::array<double, 6> velz = {0.0, 0.0, 1.0, 0.0, 0.0, 0.0};
    auto tau4 = inverse_dynamics(acc0, velz, I);
    // lin_mag = 1, ang_mag = 0, coriolis_lin = cross((0,0,25), (0,0,0)) = (0,0,0), coriolis_ang = cross((0,0,25),(0,0,0)) + cross((0,0,0),(0,0,0)) = (0,0,0)
    // damping = (0,0,0.2*1*1, 0,0,0) = (0,0,0.2,0,0,0)
    assert(std::abs(tau4[2] - 0.2) < 1e-9);
    for (int i = 0; i < 6; ++i) {
        if (i != 2) assert(std::abs(tau4[i]) < 1e-9);
    }

    // Test 5: angular velocity only around x-axis.
    std::array<double, 6> velwx = {0.0, 0.0, 0.0, 1.0, 0.0, 0.0};
    auto tau5 = inverse_dynamics(acc0, velwx, I);
    // ang_mag=1, lin_mag=0, coriolis_lin = cross((0,0,0),(1,0,0)) = (0,0,0), coriolis_ang = cross((0,0,0),(0,0,0)) + cross((8,0,0),(1,0,0)) = (0,0,0)
    // damping = (0,0,0, 0.5*1*1,0,0) = (0,0,0,0.5,0,0)
    assert(std::abs(tau5[3] - 0.5) < 1e-9);
    for (int i = 0; i < 6; ++i) {
        if (i != 3) assert(std::abs(tau5[i]) < 1e-9);
    }

    return 0;
}
// The solution involves decomposing the 6D state into linear (first 3 components) and angular (last 3 components) parts. For the Coriolis matrix, we use the standard form: C(ν) = [ [0₃, -S(M₁₁·ν₁ + M₁₂·ν₂)], [-S(M₁₁·ν₁ + M₁₂·ν₂), -S(M₂₁·ν₁ + M₂₂·ν₂)] ], where S is the skew-symmetric matrix operator (S(v)·w = v×w), and M is partitioned into four 3x3 blocks (M₁₁, M₁₂, M₂₁, M₂₂). The damping is diagonal: D = diag(d_lin·|linear_vel|, d_lin·|linear_vel|, d_lin·|linear_vel|, d_quad·|ang_vel|, d_quad·|ang_vel|, d_quad·|ang_vel|), where we use the magnitude of the corresponding velocity subvector. For the given test cases, we compute C(ν)·ν and D(ν)·ν manually. The gravity term is zero. Edge cases: zero velocity leads to zero Coriolis and damping, so only inertia term remains. Time complexity is O(1) since all operations are on fixed-size matrices; space complexity is O(1) as we only allocate temporary 6-vectors.
