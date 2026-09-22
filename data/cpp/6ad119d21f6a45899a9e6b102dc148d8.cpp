// Write a standalone C++ function that estimates the intrinsic camera parameters (focal lengths \(f_x, f_y\), principal point \(c_x, c_y\), and radial/tangential distortion coefficients \(k_1, k_2, p_1, p_2, k_3\)) given a set of observations where each observation consists of 3D points in a target frame and their corresponding 2D image pixel coordinates, using a pinhole camera model with Brown-Conrady distortion. The function must take as input: a vector of observation sets (each being a vector of correspondences), an initial guess for the intrinsics, and an initial guess for the pose of the target relative to the camera for each observation set. It must return the refined intrinsics (including distortion coefficients) that minimize the reprojection error across all observations. The function must be self-contained, not rely on any external optimization libraries (like Ceres), and instead implement a simple iterative nonlinear least-squares solver (e.g., Gauss-Newton or Levenberg-Marquardt) with numerical Jacobians. The solution must handle edge cases such as zero-depth points, near-zero distortion coefficients, and multiple observation sets with different poses. The function signature should be: `IntrinsicResult optimizeIntrinsics(const std::vector<CorrespondenceSet>& observations, const CameraIntrinsics& initial_intrinsics, const std::vector<Pose6d>& initial_poses, double tolerance, int max_iterations)`, where `CorrespondenceSet` is a vector of `Correspondence` structs (each with `in_target` as `Eigen::Vector3d` and `in_image` as `Eigen::Vector2d`), `CameraIntrinsics` holds fx, fy, cx, cy and a 5-element distortion vector, `Pose6d` is a 6D pose (3-axis-angle rotation, 3 translation), and `IntrinsicResult` holds the optimized intrinsics, distortion coefficients, and final cost.
// The solution requires setting up a nonlinear least-squares problem where the residuals are the 2D reprojection errors (predicted image point minus observed image point) for each 3D point in each observation set. The parameters to optimize are: the 4 intrinsic parameters (fx, fy, cx, cy), the 5 distortion coefficients, and the 6-DOF pose (angle-axis rotation + translation) for each observation set. This gives \(4 + 5 + 6N\) parameters where \(N\) is the number of observation sets. The main algorithm is a Levenberg-Marquardt (or Gauss-Newton) iteration:
// 1. Initialize the parameter vector: first 9 entries are intrinsics+distortion, then for each observation set, append 6 pose parameters.
// 2. For each iteration, compute the residual vector (size \(2 \times\) total correspondences) and the Jacobian matrix numerically (using central differences) with respect to all parameters. Alternatively, compute analytic Jacobians, but numerical is simpler for a standalone task.
// 3. Solve the normal equations \((J^T J + \lambda \operatorname{diag}(J^T J)) \delta = -J^T r\) for a step \(\delta\). Update parameters and adjust the damping factor \(\lambda\) based on whether cost decreased.
// 4. Iterate until cost change is below `tolerance` or max iterations reached.
// Edge cases: (a) When a 3D point has zero z in camera coordinates, the projection division by z must be avoided—handle by either skipping that correspondence or setting a large residual. (b) The pose parameters are in angle-axis form; update by adding the step directly is valid for small steps, but for robustness, we can convert to rotation matrix and recompute after each iteration, but adding to the 6D vector works for small steps. (c) If the initial numbers of observations are zero, return the initial guess. (d) Numerical Jacobians require careful step size (e.g., \(10^{-6}\)) relative to parameter magnitude. Time complexity per iteration is \(O(P \times R)\) where \(P\) is total parameters and \(R\) is total residuals, due to computing the full Jacobian numerically. Space complexity is \(O(P \times R)\) for storing the Jacobian. Typical convergence requires tens of iterations.
#include <vector>
#include <cmath>
#include <Eigen/Dense>
#include <stdexcept>

// Struct to hold a 2D-3D correspondence
struct Correspondence {
    Eigen::Vector3d in_target;
    Eigen::Vector2d in_image;
};

// One observation set is a vector of correspondences
using CorrespondenceSet = std::vector<Correspondence>;

// Camera intrinsic parameters including distortion
struct CameraIntrinsics {
    double fx, fy, cx, cy;
    // distortion: k1, k2, p1, p2, k3
    double k1, k2, p1, p2, k3;
};

// 6D pose: angle-axis rotation (3) + translation (3)
struct Pose6d {
    double ax, ay, az; // angle-axis rotation components
    double tx, ty, tz; // translation
};

// Result of optimization
struct IntrinsicResult {
    CameraIntrinsics intrinsics;
    double final_cost;
    bool converged;
};

// Project a point in camera coordinates to image coordinates using pinhole + distortion
Eigen::Vector2d projectPoint(const CameraIntrinsics& intr, const Eigen::Vector3d& pt_cam) {
    const double xp1 = pt_cam.x();
    const double yp1 = pt_cam.y();
    const double zp1 = pt_cam.z();

    // Normalize to image plane; if z=0, return a far away point
    double xp, yp;
    if (std::abs(zp1) < 1e-12) {
        xp = xp1;
        yp = yp1;
    } else {
        xp = xp1 / zp1;
        yp = yp1 / zp1;
    }

    const double xp2 = xp * xp;
    const double yp2 = yp * yp;
    const double r2 = xp2 + yp2;
    const double r4 = r2 * r2;
    const double r6 = r2 * r4;

    // Brown-Conrady distortion
    const double xpp = xp * (1.0 + intr.k1 * r2 + intr.k2 * r4 + intr.k3 * r6)
                       + 2.0 * intr.p1 * xp * yp + intr.p2 * (r2 + 2.0 * xp2);
    const double ypp = yp * (1.0 + intr.k1 * r2 + intr.k2 * r4 + intr.k3 * r6)
                       + intr.p1 * (r2 + 2.0 * yp2) + 2.0 * intr.p2 * xp * yp;

    return Eigen::Vector2d(intr.fx * xpp + intr.cx, intr.fy * ypp + intr.cy);
}

// Convert angle-axis to rotation matrix
Eigen::Matrix3d angleAxisToMatrix(const double ax, const double ay, const double az) {
    double angle = std::sqrt(ax*ax + ay*ay + az*az);
    if (angle < 1e-12) return Eigen::Matrix3d::Identity();
    Eigen::Vector3d axis(ax, ay, az);
    axis.normalize();
    Eigen::AngleAxisd aa(angle, axis);
    return aa.toRotationMatrix();
}

// Transform a point from target frame to camera frame given a pose
Eigen::Vector3d transformPoint(const Pose6d& pose, const Eigen::Vector3d& pt_target) {
    Eigen::Matrix3d R = angleAxisToMatrix(pose.ax, pose.ay, pose.az);
    Eigen::Vector3d t(pose.tx, pose.ty, pose.tz);
    return R * pt_target + t;
}

// Compute the residual vector and Jacobian numerically
// Parameters order: [fx, fy, cx, cy, k1, k2, p1, p2, k3, then 6 per observation set: (ax,ay,az,tx,ty,tz)]
void computeResidualAndJacobian(
    const std::vector<CorrespondenceSet>& obs,
    const std::vector<Pose6d>& poses,
    const CameraIntrinsics& intr,
    Eigen::VectorXd& residual,
    Eigen::MatrixXd& J,
    double eps = 1e-6)
{
    // Total parameters
    const int n_params = 9 + 6 * obs.size();
    // Total residuals = 2 per correspondence
    int total_corrs = 0;
    for (const auto& cs : obs) total_corrs += cs.size();
    const int n_res = 2 * total_corrs;

    residual.resize(n_res);
    J.resize(n_res, n_params);

    // Build parameter vector
    Eigen::VectorXd params(n_params);
    params[0] = intr.fx;
    params[1] = intr.fy;
    params[2] = intr.cx;
    params[3] = intr.cy;
    params[4] = intr.k1;
    params[5] = intr.k2;
    params[6] = intr.p1;
    params[7] = intr.p2;
    params[8] = intr.k3;
    for (size_t i = 0; i < poses.size(); ++i) {
        params[9 + 6*i + 0] = poses[i].ax;
        params[9 + 6*i + 1] = poses[i].ay;
        params[9 + 6*i + 2] = poses[i].az;
        params[9 + 6*i + 3] = poses[i].tx;
        params[9 + 6*i + 4] = poses[i].ty;
        params[9 + 6*i + 5] = poses[i].tz;
    }

    // Helper lambda to set intrinsics from params
    auto setIntrFromParams = [](CameraIntrinsics& i, const Eigen::VectorXd& p) {
        i.fx = p[0]; i.fy = p[1]; i.cx = p[2]; i.cy = p[3];
        i.k1 = p[4]; i.k2 = p[5]; i.p1 = p[6]; i.p2 = p[7]; i.k3 = p[8];
    };

    // Fill residual and J numerically
    int res_idx = 0;
    for (size_t i = 0; i < obs.size(); ++i) {
        // Current pose for this observation set
        Pose6d cur_pose;
        cur_pose.ax = params[9 + 6*i + 0];
        cur_pose.ay = params[9 + 6*i + 1];
        cur_pose.az = params[9 + 6*i + 2];
        cur_pose.tx = params[9 + 6*i + 3];
        cur_pose.ty = params[9 + 6*i + 4];
        cur_pose.tz = params[9 + 6*i + 5];

        for (const auto& corr : obs[i]) {
            // Transform point
            Eigen::Vector3d pt_cam = transformPoint(cur_pose, corr.in_target);
            // Project with current intrinsics
            CameraIntrinsics temp_intr;
            setIntrFromParams(temp_intr, params);
            Eigen::Vector2d pred = projectPoint(temp_intr, pt_cam);
            residual[res_idx] = pred.x() - corr.in_image.x();
            residual[res_idx+1] = pred.y() - corr.in_image.y();

            // Numerical Jacobian for each parameter
            for (int p = 0; p < n_params; ++p) {
                Eigen::VectorXd params_plus = params;
                Eigen::VectorXd params_minus = params;
                double step = eps * (std::abs(params[p]) + eps);
                params_plus[p] += step;
                params_minus[p] -= step;

                CameraIntrinsics intr_plus, intr_minus;
                setIntrFromParams(intr_plus, params_plus);
                setIntrFromParams(intr_minus, params_minus);

                // Pose for this observation set in perturbed params
                Pose6d pose_plus, pose_minus;
                pose_plus.ax = params_plus[9 + 6*i + 0];
                pose_plus.ay = params_plus[9 + 6*i + 1];
                pose_plus.az = params_plus[9 + 6*i + 2];
                pose_plus.tx = params_plus[9 + 6*i + 3];
                pose_plus.ty = params_plus[9 + 6*i + 4];
                pose_plus.tz = params_plus[9 + 6*i + 5];
                pose_minus.ax = params_minus[9 + 6*i + 0];
                pose_minus.ay = params_minus[9 + 6*i + 1];
                pose_minus.az = params_minus[9 + 6*i + 2];
                pose_minus.tx = params_minus[9 + 6*i + 3];
                pose_minus.ty = params_minus[9 + 6*i + 4];
                pose_minus.tz = params_minus[9 + 6*i + 5];

                Eigen::Vector3d pt_plus = transformPoint(pose_plus, corr.in_target);
                Eigen::Vector3d pt_minus = transformPoint(pose_minus, corr.in_target);
                Eigen::Vector2d pred_plus = projectPoint(intr_plus, pt_plus);
                Eigen::Vector2d pred_minus = projectPoint(intr_minus, pt_minus);

                J(res_idx, p) = (pred_plus.x() - pred_minus.x()) / (2.0 * step);
                J(res_idx+1, p) = (pred_plus.y() - pred_minus.y()) / (2.0 * step);
            }
            res_idx += 2;
        }
    }
}

// Main optimization function
IntrinsicResult optimizeIntrinsics(
    const std::vector<CorrespondenceSet>& observations,
    const CameraIntrinsics& initial_intrinsics,
    const std::vector<Pose6d>& initial_poses,
    double tolerance = 1e-6,
    int max_iterations = 100)
{
    if (observations.size() != initial_poses.size()) {
        throw std::invalid_argument("Number of observation sets must match number of initial poses");
    }
    if (observations.empty()) {
        IntrinsicResult result;
        result.intrinsics = initial_intrinsics;
        result.final_cost = 0.0;
        result.converged = true;
        return result;
    }

    // Copy initial guesses
    CameraIntrinsics intr = initial_intrinsics;
    std::vector<Pose6d> poses = initial_poses;

    const int n_params = 9 + 6 * poses.size();
    Eigen::VectorXd params(n_params);
    auto setParams = [&]() {
        params[0] = intr.fx; params[1] = intr.fy; params[2] = intr.cx; params[3] = intr.cy;
        params[4] = intr.k1; params[5] = intr.k2; params[6] = intr.p1; params[7] = intr.p2;
        params[8] = intr.k3;
        for (size_t i = 0; i < poses.size(); ++i) {
            params[9 + 6*i + 0] = poses[i].ax;
            params[9 + 6*i + 1] = poses[i].ay;
            params[9 + 6*i + 2] = poses[i].az;
            params[9 + 6*i + 3] = poses[i].tx;
            params[9 + 6*i + 4] = poses[i].ty;
            params[9 + 6*i + 5] = poses[i].tz;
        }
    };
    auto setFromParams = [&]() {
        intr.fx = params[0]; intr.fy = params[1]; intr.cx = params[2]; intr.cy = params[3];
        intr.k1 = params[4]; intr.k2 = params[5]; intr.p1 = params[6]; intr.p2 = params[7];
        intr.k3 = params[8];
        for (size_t i = 0; i < poses.size(); ++i) {
            poses[i].ax = params[9 + 6*i + 0];
            poses[i].ay = params[9 + 6*i + 1];
            poses[i].az = params[9 + 6*i + 2];
            poses[i].tx = params[9 + 6*i + 3];
            poses[i].ty = params[9 + 6*i + 4];
            poses[i].tz = params[9 + 6*i + 5];
        }
    };
    setParams();

    // Levenberg-Marquardt
    double lambda = 1e-3;
    double prev_cost = -1.0;

    int total_corrs = 0;
    for (const auto& cs : observations) total_corrs += cs.size();
    const int n_res = 2 * total_corrs;

    Eigen::VectorXd residual;
    Eigen::MatrixXd J;

    bool converged = false;
    for (int iter = 0; iter < max_iterations; ++iter) {
        // Compute residual and Jacobian at current params
        computeResidualAndJacobian(observations, poses, intr, residual, J);

        double cost = residual.squaredNorm() / 2.0;
        if (prev_cost >= 0.0 && std::abs(prev_cost - cost) < tolerance * (1.0 + std::abs(cost))) {
            converged = true;
            break;
        }
        prev_cost = cost;

        // Solve (J^T J + lambda * diag(J^T J)) delta = -J^T r
        Eigen::MatrixXd JTJ = J.transpose() * J;
        Eigen::VectorXd JTr = J.transpose() * residual;
        Eigen::MatrixXd A = JTJ + lambda * JTJ.diagonal().asDiagonal();
        Eigen::VectorXd delta = A.ldlt().solve(-JTr);

        // Check step
        double new_cost = -1.0;
        double rho = 0.0;
        {
            Eigen::VectorXd params_new = params + delta;
            CameraIntrinsics intr_new;
            intr_new.fx = params_new[0]; intr_new.fy = params_new[1];
            intr_new.cx = params_new[2]; intr_new.cy = params_new[3];
            intr_new.k1 = params_new[4]; intr_new.k2 = params_new[5];
            intr_new.p1 = params_new[6]; intr_new.p2 = params_new[7];
            intr_new.k3 = params_new[8];
            std::vector<Pose6d> poses_new(poses.size());
            for (size_t i = 0; i < poses.size(); ++i) {
                poses_new[i].ax = params_new[9 + 6*i + 0];
                poses_new[i].ay = params_new[9 + 6*i + 1];
                poses_new[i].az = params_new[9 + 6*i + 2];
                poses_new[i].tx = params_new[9 + 6*i + 3];
                poses_new[i].ty = params_new[9 + 6*i + 4];
                poses_new[i].tz = params_new[9 + 6*i + 5];
            }
            Eigen::VectorXd residual_new;
            Eigen::MatrixXd J_new;
            computeResidualAndJacobian(observations, poses_new, intr_new, residual_new, J_new);
            new_cost = residual_new.squaredNorm() / 2.0;
            // Compute rho = (cost - new_cost) / (delta^T (lambda*diag*delta - JTr))? Simpler: use gain ratio
            double denom = delta.dot(lambda * JTJ.diagonal().asDiagonal() * delta - JTr);
            if (std::abs(denom) < 1e-12) {
                rho = 0.0;
            } else {
                rho = (cost - new_cost) / denom;
            }
        }

        if (new_cost < cost) {
            // Accept step
            params += delta;
            setFromParams();
            lambda = std::max(lambda * 0.1, 1e-10);
            prev_cost = new_cost;
        } else {
            lambda = std::min(lambda * 10.0, 1e10);
        }
    }

    IntrinsicResult result;
    result.intrinsics = intr;
    // Compute final cost
    Eigen::VectorXd final_res;
    Eigen::MatrixXd final_J;
    computeResidualAndJacobian(observations, poses, intr, final_res, final_J);
    result.final_cost = final_res.squaredNorm() / 2.0;
    result.converged = converged;
    return result;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <Eigen/Dense>
#include <iostream>

// Include the solution here (or include the header if separate)

int main() {
    // Create a synthetic camera with known intrinsics
    CameraIntrinsics true_intr;
    true_intr.fx = 800.0;
    true_intr.fy = 820.0;
    true_intr.cx = 320.0;
    true_intr.cy = 240.0;
    true_intr.k1 = 0.1;
    true_intr.k2 = -0.05;
    true_intr.p1 = 0.001;
    true_intr.p2 = -0.002;
    true_intr.k3 = 0.005;

    // Create three observation sets with different poses
    std::vector<CorrespondenceSet> observations(3);
    std::vector<Pose6d> true_poses(3);
    // Define a set of 3D points in target frame (e.g., a planar grid)
    std::vector<Eigen::Vector3d> points_3d;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            points_3d.push_back(Eigen::Vector3d(0.1 * i, 0.1 * j, 0.0));
        }
    }

    // Generate observations for each pose
    for (size_t obs_idx = 0; obs_idx < 3; ++obs_idx) {
        // Define pose: rotation around Y and translation
        double angle = 0.2 * (obs_idx + 1);
        double tx = 0.0, ty = 0.0, tz = 0.5 + 0.1 * obs_idx;
        Pose6d pose;
        pose.ax = 0.0;
        pose.ay = angle;
        pose.az = 0.0;
        pose.tx = tx;
        pose.ty = ty;
        pose.tz = tz;
        true_poses[obs_idx] = pose;

        // Transform and project
        for (const auto& pt : points_3d) {
            Eigen::Matrix3d R = angleAxisToMatrix(pose.ax, pose.ay, pose.az);
            Eigen::Vector3d t(pose.tx, pose.ty, pose.tz);
            Eigen::Vector3d pt_cam = R * pt + t;
            // Use the same projectPoint as in solution (need to copy it here)
            double xp = pt_cam.x() / pt_cam.z();
            double yp = pt_cam.y() / pt_cam.z();
            double r2 = xp*xp + yp*yp;
            double r4 = r2*r2;
            double r6 = r2*r4;
            double xpp = xp*(1 + true_intr.k1*r2 + true_intr.k2*r4 + true_intr.k3*r6)
                        + 2*true_intr.p1*xp*yp + true_intr.p2*(r2 + 2*xp*xp);
            double ypp = yp*(1 + true_intr.k1*r2 + true_intr.k2*r4 + true_intr.k3*r6)
                        + true_intr.p1*(r2 + 2*yp*yp) + 2*true_intr.p2*xp*yp;
            Eigen::Vector2d img(true_intr.fx * xpp + true_intr.cx, true_intr.fy * ypp + true_intr.cy);
            // Add small noise to avoid perfect fit
            img.x() += 0.001 * std::sin(obs_idx * 100 + img.x());
            img.y() += 0.001 * std::cos(obs_idx * 100 + img.y());
            observations[obs_idx].push_back({pt, img});
        }
    }

    // Initial guesses: slightly perturbed intrinsics and wrong poses
    CameraIntrinsics init_intr = true_intr;
    init_intr.fx *= 1.05;
    init_intr.fy *= 0.95;
    init_intr.cx += 10;
    init_intr.cy -= 10;
    init_intr.k1 = 0.0;
    init_intr.k2 = 0.0;
    init_intr.p1 = 0.0;
    init_intr.p2 = 0.0;
    init_intr.k3 = 0.0;

    std::vector<Pose6d> init_poses = true_poses;
    for (auto& p : init_poses) {
        p.ay += 0.1;
        p.tz -= 0.05;
    }

    // Run optimization (initial_poses are ignored in the standalone solution's signature, but we pass them; the solution takes initial_poses but we can pass init_poses)
    IntrinsicResult result = optimizeIntrinsics(observations, init_intr, init_poses, 1e-8, 200);

    // Verify convergence
    assert(result.converged == true);

    // Check intrinsics are close to true values (within 1% tolerance)
    assert(std::abs(result.intrinsics.fx - true_intr.fx) < 10.0);
    assert(std::abs(result.intrinsics.fy - true_intr.fy) < 10.0);
    assert(std::abs(result.intrinsics.cx - true_intr.cx) < 5.0);
    assert(std::abs(result.intrinsics.cy - true_intr.cy) < 5.0);
    // Distortion coefficients may have larger relative error but should be reasonable
    assert(std::abs(result.intrinsics.k1 - true_intr.k1) < 0.02);
    assert(std::abs(result.intrinsics.k2 - true_intr.k2) < 0.02);
    assert(std::abs(result.intrinsics.p1 - true_intr.p1) < 0.005);
    assert(std::abs(result.intrinsics.p2 - true_intr.p2) < 0.005);
    assert(std::abs(result.intrinsics.k3 - true_intr.k3) < 0.01);

    // Final cost should be small (reprojection error)
    assert(result.final_cost < 0.01);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
