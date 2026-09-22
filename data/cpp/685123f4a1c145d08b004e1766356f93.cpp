/*
Write a C++ function named `computeSE3ErrorAndJacobians` that, given two 3D rigid-body transformation matrices `Xi` and `Xj` (each represented as a 4x4 Eigen `Isometry3d`) and a measurement transformation `Z` (also an `Isometry3d`), computes the 6-dimensional error vector `error` (in the order: translation x, y, z, then the rotation vector part of a quaternion, i.e., the imaginary part qx, qy, qz) and the two 6x6 Jacobian matrices `Jxi` and `Jxj` with respect to perturbations in `Xi` and `Xj` respectively. The error is defined as: `delta = Z.inverse() * Xi.inverse() * Xj`; the translation component of `error` is the translation of `delta`, and the rotation component is the imaginary part of the unit quaternion extracted from `delta`'s rotation matrix. The Jacobians must be computed using numerical differentiation: perturb each of the 6 degrees of freedom (3 translation, 3 rotation about the identity axes) of `Xi` and `Xj` by a small epsilon (e.g., 1e-6) and compute the finite-difference gradient. The function should modify the passed references `error`, `Jxi`, and `Jxj` and return `void`. Use Eigen's `Isometry3d`, `Quaterniond`, and `Matrix<double,6,6>`. Assume all inputs are valid rigid transformations (but ensure the code works even if rotations are not perfectly normalized by normalizing the extracted quaternion). The function must be const-correct where appropriate.
*/

#include <Eigen/Geometry>
#include <Eigen/Core>

using Eigen::Isometry3d;
using Eigen::Quaterniond;
using Eigen::Matrix;
using Eigen::Vector3d;

// Helper to build a small delta transformation for numerical differentiation.
Isometry3d makeDelta(int dof, double eps) {
    Isometry3d delta = Isometry3d::Identity();
    if (dof < 3) { // translation perturbation
        Vector3d t = Vector3d::Zero();
        t[dof] = eps;
        delta.translation() = t;
    } else { // rotation perturbation
        int axis = dof - 3;
        Vector3d axisVec = Vector3d::Zero();
        axisVec[axis] = 1.0;
        delta.linear() = Eigen::AngleAxisd(eps, axisVec).toRotationMatrix();
    }
    return delta;
}

// Extract the error vector (translation + imaginary quaternion) from a delta Isometry3d.
Matrix<double,6,1> computeErrorVector(const Isometry3d& delta) {
    Matrix<double,6,1> error;
    // Translation component
    error.block<3,1>(0,0) = delta.translation();
    // Rotation component from quaternion imaginary part (normalized)
    Quaterniond q(delta.linear());
    q.normalize();
    if (q.w() < 0.0) {
        q.coeffs() = -q.coeffs();
    }
    error.block<3,1>(3,0) = q.vec();
    return error;
}

// Main function: compute error and Jacobians with respect to Xi and Xj.
void computeSE3ErrorAndJacobians(const Isometry3d& Xi, const Isometry3d& Xj,
                                 const Isometry3d& Z,
                                 Matrix<double,6,1>& error,
                                 Matrix<double,6,6>& Jxi,
                                 Matrix<double,6,6>& Jxj) {
    const double eps = 1e-6;

    // Compute nominal error
    Isometry3d delta = Z.inverse() * Xi.inverse() * Xj;
    error = computeErrorVector(delta);

    // Jacobian w.r.t. Xi
    for (int i = 0; i < 6; ++i) {
        Isometry3d Xi_pert = Xi * makeDelta(i, eps);
        Isometry3d delta_pert = Z.inverse() * Xi_pert.inverse() * Xj;
        Matrix<double,6,1> err_pert = computeErrorVector(delta_pert);
        Jxi.col(i) = (err_pert - error) / eps;
    }

    // Jacobian w.r.t. Xj
    for (int i = 0; i < 6; ++i) {
        Isometry3d Xj_pert = Xj * makeDelta(i, eps);
        Isometry3d delta_pert = Z.inverse() * Xi.inverse() * Xj_pert;
        Matrix<double,6,1> err_pert = computeErrorVector(delta_pert);
        Jxj.col(i) = (err_pert - error) / eps;
    }
}

#include <Eigen/Geometry>
#include <cassert>
#include <cmath>

int main() {
    using Eigen::Isometry3d;
    using Eigen::Quaterniond;
    using Eigen::Vector3d;
    using Eigen::Matrix;

    // Test 1: Identity poses and identity measurement -> error zero, Jacobians approximate known pattern.
    Isometry3d Xi = Isometry3d::Identity();
    Isometry3d Xj = Isometry3d::Identity();
    Isometry3d Z = Isometry3d::Identity();
    Matrix<double,6,1> error;
    Matrix<double,6,6> Jxi, Jxj;
    computeSE3ErrorAndJacobians(Xi, Xj, Z, error, Jxi, Jxj);
    assert(error.norm() < 1e-9);
    // For identity, Jxi for translation: [I, 0]? Actually from formula delta = Z^-1 * Xi^-1 * Xj.
    // Perturbing Xi translation t -> delta changes by -t. So Jxi transl part = -I.
    for (int i=0;i<3;i++) {
        for (int j=0;j<3;j++) {
            assert(std::abs(Jxi(i,j) - (i==j ? -1.0 : 0.0)) < 1e-5);
        }
    }

    // Test 2: Pure translation measurement with known poses.
    Xi = Isometry3d::Identity();
    Xj = Isometry3d::Identity();
    Xj.translation() = Vector3d(1.0, 2.0, 3.0);
    Z = Isometry3d::Identity();
    Z.translation() = Vector3d(1.0, 2.0, 3.0);
    computeSE3ErrorAndJacobians(Xi, Xj, Z, error, Jxi, Jxj);
    assert(error.norm() < 1e-9);

    // Test 3: Known error for a simple case.
    Xi = Isometry3d::Identity();
    Xj = Isometry3d::Identity();
    Xj.translation() = Vector3d(1.0, 0.0, 0.0);
    Z = Isometry3d::Identity(); // measurement identity -> error should reflect (0,0,0) because delta = Xi^-1 * Xj = [0,0,0]? Wait Xi^-1*Xj = [1,0,0] for translation.
    // Let's set Z such that error is zero: Z = Xi^-1*Xj = [1,0,0].
    Z.translation() = Vector3d(1.0, 0.0, 0.0);
    computeSE3ErrorAndJacobians(Xi, Xj, Z, error, Jxi, Jxj);
    assert(error.norm() < 1e-9);

    // Test 4: Non-trivial rotation, check that for identical poses and measurement error is zero.
    Xi = Isometry3d::Identity();
    Xi.linear() = Eigen::AngleAxisd(0.3, Vector3d::UnitZ()).toRotationMatrix();
    Xj = Xi; // same pose
    Z = Isometry3d::Identity(); // delta = Z^-1 * Xi^-1 * Xj = I -> error zero
    computeSE3ErrorAndJacobians(Xi, Xj, Z, error, Jxi, Jxj);
    assert(error.norm() < 1e-9);

    // Test 5: Check that Jacobian columns are non-zero and finite.
    Xi = Isometry3d::Identity();
    Xj = Isometry3d::Identity();
    Xj.translation() = Vector3d(0.5, -0.2, 0.1);
    Xj.linear() = Eigen::AngleAxisd(0.1, Vector3d::UnitY()).toRotationMatrix();
    Z = Isometry3d::Identity();
    computeSE3ErrorAndJacobians(Xi, Xj, Z, error, Jxi, Jxj);
    for (int i=0;i<6;i++) {
        for (int j=0;j<6;j++) {
            assert(std::isfinite(Jxi(i,j)));
            assert(std::isfinite(Jxj(i,j)));
        }
    }

    // Test 6: Perturbation consistency: if Xi is perturbed by epsilon along x, the error change should match first column of Jxi.
    Isometry3d Xi_base = Isometry3d::Identity();
    Isometry3d Xj_base = Isometry3d::Identity();
    Xj_base.translation() = Vector3d(0.3, 0.0, 0.0);
    Isometry3d Z_base = Isometry3d::Identity();
    computeSE3ErrorAndJacobians(Xi_base, Xj_base, Z_base, error, Jxi, Jxj);
    // Manually perturb Xi along x by 1e-6
    Isometry3d Xi_pert = Xi_base;
    Xi_pert.translation()[0] += 1e-6;
    Isometry3d delta_pert = Z_base.inverse() * Xi_pert.inverse() * Xj_base;
    Matrix<double,6,1> error_pert = computeErrorVector(delta_pert);
    Matrix<double,6,1> expected_change = (error_pert - error) / 1e-6;
    assert((Jxi.col(0) - expected_change).norm() < 1e-3);
}

// The solution applies the left perturbation model: for each of the two input poses, we generate six perturbed versions by multiplying the original pose by a small delta transformation. The delta for translation perturbation `t_k` (for k=1,2,3) is `Isometry3d::Identity()` translated by `eps` in the k-th axis. The delta for rotation perturbation `r_k` (for k=4,5,6) is `Isometry3d::Identity()` rotated by `eps` radians about the k-th axis (using `AngleAxisd(eps, AxisVector)`) and then the translation is kept zero. For each perturbation, we recompute the error using the formula `delta = Z.inverse() * X_perturbed.inverse() * X_other` (or the appropriate variable depending on which pose is being perturbed). The error difference divided by `eps` gives a column of the Jacobian. Important edge cases: ensure the quaternion from the delta rotation is normalized before extracting its imaginary part; handle the sign ambiguity of the quaternion (we assume the quaternion's real part is non-negative by normalizing the quaternion and, if the real part is negative, negating the entire quaternion – this ensures continuity). The error vector's rotation part is the imaginary part of the normalized quaternion. For numerical stability, use a double epsilon like 1e-6. The time complexity is O(1) because the number of perturbations is fixed (12), and each error computation is O(1) for 4x4 matrices. Space complexity is O(1) aside from the output matrices.
