/*
Write a C++ function named `computeMixedJointState` that simulates the core kinematic computations of a mixed joint from a multibody dynamics simulation. The function should accept: a 3x3 rotation matrix `pkCk` (orientation of the parent body in the world frame), a 3x3 rotation matrix `body2nCk` (orientation of child body in world frame), a 3-element vector `r12` (position of child relative to parent in world frame), a 3-element vector `omega_k` (angular velocity of child in child's body frame), a 3-element vector `v` (linear velocity of child in world frame), a scalar mass `m`, a 3x3 inertia matrix `I` (in child body frame), and a boolean `numrotsGreaterThan1` indicating whether rotational DOFs are Euler parameters (>1) or simpler. The function should return a struct `MixedJointState` containing: the child body’s angular velocity in world frame (`omega_world`), linear velocity in world frame (`v_world`), linear velocity in child body frame (`v_body`), and kinetic energy (`KE`). The kinetic energy is computed as `0.5 * m * dot(v_world, v_world) + 0.5 * dot(omega_k, I * omega_k)`. The angular velocity in world frame is obtained by rotating `omega_k` from child body frame to world frame using `body2nCk`. The linear velocity in body frame is obtained by rotating `v_world` back to the body frame using the transpose of `body2nCk` (i.e., `v_body = trans(body2nCk) * v_world`). The function should handle both cases of `numrotsGreaterThan1` uniformly for the velocity computations (the rotation matrix is always given). Ensure the function is const-correct where appropriate and uses standard C++ types like `std::array<double,3>` for vectors and `std::array<std::array<double,3>,3>` for matrices. The function should not print anything.
*/

#include <array>
#include <cmath>

// Helper function for dot product of two 3D vectors
double dot3(const std::array<double,3>& a, const std::array<double,3>& b) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

// Helper function for 3x3 matrix times 3D vector
std::array<double,3> matVec3(const std::array<std::array<double,3>,3>& M,
                             const std::array<double,3>& v) {
    return {{
        M[0][0]*v[0] + M[0][1]*v[1] + M[0][2]*v[2],
        M[1][0]*v[0] + M[1][1]*v[1] + M[1][2]*v[2],
        M[2][0]*v[0] + M[2][1]*v[1] + M[2][2]*v[2]
    }};
}

// Helper to transpose 3x3 matrix
std::array<std::array<double,3>,3> transpose3(const std::array<std::array<double,3>,3>& M) {
    return {{
        {{M[0][0], M[1][0], M[2][0]}},
        {{M[0][1], M[1][1], M[2][1]}},
        {{M[0][2], M[1][2], M[2][2]}}
    }};
}

struct MixedJointState {
    std::array<double,3> omega_world;
    std::array<double,3> v_world;
    std::array<double,3> v_body;
    double KE;
};

// Compute the kinematic state of a mixed joint from given inputs.
MixedJointState computeMixedJointState(
    const std::array<std::array<double,3>,3>& /*pkCk*/,  // unused in computation
    const std::array<std::array<double,3>,3>& body2nCk,
    const std::array<double,3>& /*r12*/,               // unused
    const std::array<double,3>& omega_k,
    const std::array<double,3>& v_world,
    double mass,
    const std::array<std::array<double,3>,3>& inertia,
    bool /*numrotsGreaterThan1*/) {                     // unused
    MixedJointState result;
    
    // Angular velocity in world frame: omega_world = body2nCk * omega_k
    result.omega_world = matVec3(body2nCk, omega_k);
    
    // Linear velocity in body frame: v_body = transpose(body2nCk) * v_world
    auto body2nCk_T = transpose3(body2nCk);
    result.v_body = matVec3(body2nCk_T, v_world);
    
    // Copy world velocity
    result.v_world = v_world;
    
    // Kinetic energy
    double translation_KE = 0.5 * mass * dot3(v_world, v_world);
    auto inertia_times_omega = matVec3(inertia, omega_k);
    double rotation_KE = 0.5 * dot3(omega_k, inertia_times_omega);
    result.KE = translation_KE + rotation_KE;
    
    return result;
}

#include <cassert>
#include <cmath>

// Assuming the solution function and helpers are included above

int main() {
    // Identity rotation, identity inertia, unit mass
    std::array<std::array<double,3>,3> identity = {{
        {{1,0,0}}, {{0,1,0}}, {{0,0,1}}
    }};
    std::array<double,3> omega = {{1,0,0}};
    std::array<double,3> v = {{2,0,0}};
    double m = 1.0;
    
    auto state = computeMixedJointState(identity, identity, {{0,0,0}}, omega, v, m, identity, true);
    // omega_world should equal omega (identity)
    assert(std::abs(state.omega_world[0] - 1.0) < 1e-12);
    assert(std::abs(state.omega_world[1]) < 1e-12);
    assert(std::abs(state.omega_world[2]) < 1e-12);
    // v_body should equal v (identity)
    assert(std::abs(state.v_body[0] - 2.0) < 1e-12);
    assert(std::abs(state.v_body[1]) < 1e-12);
    assert(std::abs(state.v_body[2]) < 1e-12);
    // KE = 0.5*1*4 + 0.5*1 = 2.5
    assert(std::abs(state.KE - 2.5) < 1e-12);
    
    // 90-degree rotation about z-axis: body2nCk = [[0,-1,0],[1,0,0],[0,0,1]]
    std::array<std::array<double,3>,3> rotZ90 = {{
        {{0,-1,0}}, {{1,0,0}}, {{0,0,1}}
    }};
    omega = {{1,0,0}};  // in body frame
    v = {{0,1,0}};      // in world frame
    auto state2 = computeMixedJointState(identity, rotZ90, {{0,0,0}}, omega, v, m, identity, false);
    // omega_world = rotZ90 * [1,0,0]^T = [0,1,0]^T
    assert(std::abs(state2.omega_world[0]) < 1e-12);
    assert(std::abs(state2.omega_world[1] - 1.0) < 1e-12);
    assert(std::abs(state2.omega_world[2]) < 1e-12);
    // v_body = transpose(rotZ90) * [0,1,0]^T = [[0,1,0],[-1,0,0],[0,0,1]] * [0,1,0] = [1,0,0]^T
    assert(std::abs(state2.v_body[0] - 1.0) < 1e-12);
    assert(std::abs(state2.v_body[1]) < 1e-12);
    assert(std::abs(state2.v_body[2]) < 1e-12);
    // KE = 0.5*1*1 + 0.5*1*1 = 1.0
    assert(std::abs(state2.KE - 1.0) < 1e-12);
    
    // Zero velocities and euler parameters flag true
    auto state3 = computeMixedJointState(identity, identity, {{0,0,0}}, {{0,0,0}}, {{0,0,0}}, 5.0, identity, true);
    assert(std::abs(state3.KE) < 1e-12);
    assert(std::abs(state3.omega_world[0]) < 1e-12);
    assert(std::abs(state3.v_body[2]) < 1e-12);
    
    return 0;
}

// The solution requires performing basic vector and matrix operations: matrix-vector multiplication for rotating angular velocity and linear velocity between frames, dot products for kinetic energy, and matrix-vector multiplication for applying inertia to angular velocity. The algorithm is straightforward: first compute `omega_world` by multiplying `body2nCk` (3x3) with `omega_k` (3x1). Compute `v_body` by multiplying the transpose of `body2nCk` with `v_world` (since the transpose of a rotation matrix is its inverse). Compute kinetic energy: translational part `0.5 * m * dot(v_world, v_world)`; rotational part first compute `I * omega_k` via matrix-vector multiplication, then `0.5 * dot(omega_k, I*omega_k)`. Edge cases: the function should work for zero vectors (KE becomes zero), and rotation matrices must be orthonormal but no validation needed. Time complexity is O(1) since fixed 3x3 operations; space complexity is O(1) beyond the returned struct.
