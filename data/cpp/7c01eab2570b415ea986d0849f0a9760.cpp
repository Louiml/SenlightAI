/*
Write a standalone C++ function `computeStateTrajectory` that simulates the iLQR control method for a simplified 2D bicopter model. The model has a 6-dimensional state vector `x = [x_g, y_g, theta, dx_g, dy_g, dtheta]` representing position, orientation, and their derivatives, and a 2-dimensional control vector `u = [f_l, f_r]` representing left and right thruster forces. The function must take an initial state vector (as `Eigen::VectorXd`) and return a `std::vector<Eigen::VectorXd>` representing the state trajectory over a fixed time horizon (T = 1.0 seconds, dt = 0.01 seconds), computed using the iterative Linear Quadratic Regulator (iLQR) algorithm with a quadratic cost that penalizes deviation from a target final state and control effort. The dynamics are semi-implicit Euler as given in the snippet, with parameters: mass `m = 1.0`, gravitational acceleration `g = 9.81`, rod length `l = 0.2`, cost weights `Q_t = 0.0` (stage state cost), `Q_T = 1000.0` (terminal state cost), `R_t = 1e-3` (control cost), and `nbIter = 20` iLQR iterations. The target final state is `[-2.0, 1.0, 0.0, 0.0, 0.0, 0.0]`. Implement the function without using external header files beyond `Eigen/Dense`, `Eigen/KroneckerProduct` (optional, though not needed), and standard library vector. The function must be self-contained, include the helper functions (`step`, `rollout`, `get_A`, `get_B`, `get_Su`, `cost`, `flatten`, `reshape`), and use a line search with 20 steps (halving alpha from 1.0) to ensure cost decrease.
*/

#include <Eigen/Dense>
#include <vector>
#include <cmath>

// Parameters struct identical to the snippet, but static for self-contained use.
struct ILQRParam {
    double dt, T, l, m, g;
    Eigen::VectorXd X_d;
    Eigen::MatrixXd Q, R;
    unsigned int nbIter, nbDoF;
    ILQRParam() {
        nbDoF = 6;
        dt = 1e-2;
        T = 1.0;
        l = 0.2;
        m = 1.0;
        g = 9.81;
        Eigen::VectorXd x_d = Eigen::VectorXd::Zero(nbDoF);
        x_d.head(3) << -2., 1., 0.;
        double Q_t = 0.0;
        double Q_T = 1e3;
        double R_t = 1e-3;
        unsigned int nbSteps = static_cast<unsigned int>(T / dt);
        X_d = Eigen::VectorXd::Zero(nbSteps * nbDoF);
        X_d.tail(nbDoF) = x_d;
        Q = Q_t * Eigen::MatrixXd::Identity(nbSteps * nbDoF, nbSteps * nbDoF);
        Q.bottomRightCorner(nbDoF, nbDoF) = Q_T * Eigen::MatrixXd::Identity(nbDoF, nbDoF);
        R = R_t * Eigen::MatrixXd::Identity((nbSteps - 1) * 2, (nbSteps - 1) * 2);
        nbIter = 20;
    }
};

// Helper functions (same as snippet, adapted to const correctness)
Eigen::VectorXd flatten(const Eigen::MatrixXd& M) {
    Eigen::MatrixXd M_T = M.transpose();
    return Eigen::Map<Eigen::VectorXd>(M_T.data(), M_T.size());
}

Eigen::MatrixXd reshape(Eigen::VectorXd& v, unsigned int cols, unsigned int rows) {
    return Eigen::Map<Eigen::MatrixXd>(v.data(), rows, cols).transpose();
}

Eigen::VectorXd step(const Eigen::VectorXd& x, const Eigen::VectorXd& u) {
    ILQRParam param;
    Eigen::VectorXd x_(param.nbDoF);
    x_(3) = x(3) - param.dt * std::sin(x(2)) * (u(0) + u(1)) / param.m;
    x_(4) = x(4) + param.dt * (std::cos(x(2)) * (u(0) + u(1)) / param.m - param.g);
    x_(5) = x(5) + param.dt * 6. * (u(1) - u(0)) / (param.m * param.l);
    x_.head(3) = x.head(3) + param.dt * 0.5 * (x.tail(3) + x_.tail(3));
    return x_;
}

Eigen::MatrixXd rollout(const Eigen::VectorXd& x_init, const Eigen::MatrixXd& U) {
    ILQRParam param;
    unsigned int nbSteps = U.rows() + 1;
    Eigen::MatrixXd X = Eigen::MatrixXd::Zero(nbSteps, param.nbDoF);
    X.row(0) = x_init;
    for (unsigned int i = 0; i < nbSteps - 1; ++i) {
        X.row(i + 1) = step(X.row(i), U.row(i));
    }
    return X;
}

Eigen::MatrixXd get_A(const Eigen::VectorXd& x, const Eigen::VectorXd& u) {
    ILQRParam param;
    Eigen::MatrixXd A = Eigen::MatrixXd::Identity(param.nbDoF, param.nbDoF);
    A.topRightCorner(3, 3) = param.dt * Eigen::MatrixXd::Identity(3, 3);
    A(0, 2) = -0.5 * param.dt * param.dt * (u(0) + u(1)) * std::cos(x(2)) / param.m;
    A(1, 2) = -0.5 * param.dt * param.dt * (u(0) + u(1)) * std::sin(x(2)) / param.m;
    A(3, 2) = -param.dt * (u(0) + u(1)) * std::cos(x(2)) / param.m;
    A(4, 2) = -param.dt * (u(0) + u(1)) * std::sin(x(2)) / param.m;
    return A;
}

Eigen::MatrixXd get_B(const Eigen::VectorXd& x) {
    ILQRParam param;
    Eigen::MatrixXd B = Eigen::MatrixXd::Zero(param.nbDoF, 2);
    Eigen::MatrixXd M_inv = 1. / param.m * Eigen::MatrixXd::Identity(3, 3);
    M_inv(2, 2) = 12. / (param.m * param.l * param.l);
    Eigen::MatrixXd G(3, 2);
    G << -std::sin(x(2)), -std::sin(x(2)),
         std::cos(x(2)),  std::cos(x(2)),
         -0.5 * param.l, 0.5 * param.l;
    B.topRows(3) = 0.5 * param.dt * param.dt * M_inv * G;
    B.bottomRows(3) = param.dt * M_inv * G;
    return B;
}

Eigen::MatrixXd get_Su(const Eigen::MatrixXd& X, const Eigen::MatrixXd& U) {
    ILQRParam param;
    unsigned int nbSteps = X.rows();
    Eigen::MatrixXd Su = Eigen::MatrixXd::Zero(param.nbDoF * nbSteps, 2 * (nbSteps - 1));
    for (unsigned int j = 0; j < nbSteps - 1; ++j) {
        Su.block((j + 1) * param.nbDoF, j * 2, param.nbDoF, 2) = get_B(X.row(j));
        for (unsigned int i = 0; i < nbSteps - 2 - j; ++i) {
            Su.block((j + 2 + i) * param.nbDoF, j * 2, param.nbDoF, 2) =
                get_A(X.row(i + j + 1), U.row(i + j + 1)) * Su.block((j + 1 + i) * param.nbDoF, j * 2, param.nbDoF, 2);
        }
    }
    return Su;
}

double cost(const Eigen::MatrixXd& X, const Eigen::MatrixXd& U) {
    ILQRParam param;
    Eigen::VectorXd diff = flatten(X) - param.X_d;
    return diff.dot(param.Q * diff) + flatten(U).dot(param.R * flatten(U));
}

// Main iLQR function
std::vector<Eigen::VectorXd> computeStateTrajectory(const Eigen::VectorXd& x_init) {
    ILQRParam param;
    unsigned int nbSteps = static_cast<unsigned int>(param.T / param.dt);
    Eigen::MatrixXd U = 0.5 * param.m * param.g * Eigen::MatrixXd::Ones(nbSteps - 1, 2);
    for (unsigned int k = 0; k < param.nbIter; ++k) {
        Eigen::MatrixXd X = rollout(x_init, U);
        double current_cost = cost(X, U);
        Eigen::MatrixXd Su = get_Su(X, U);
        Eigen::VectorXd delta_u = (Su.transpose() * param.Q * Su + param.R).llt().solve(
            Su.transpose() * param.Q * (param.X_d - flatten(X)) - param.R * flatten(U));
        // Line search
        double alpha = 1.0;
        double best_cost = current_cost;
        Eigen::MatrixXd U_best = U;
        for (unsigned int i = 0; i < 20; ++i) {
            Eigen::VectorXd u_tmp = flatten(U) + alpha * delta_u;
            Eigen::MatrixXd U_tmp = reshape(u_tmp, nbSteps - 1, 2);
            Eigen::MatrixXd X_tmp = rollout(x_init, U_tmp);
            double cost_tmp = cost(X_tmp, U_tmp);
            if (cost_tmp < best_cost) {
                best_cost = cost_tmp;
                U_best = U_tmp;
            }
            alpha = alpha / 2.;
        }
        U = U_best;
    }
    Eigen::MatrixXd X_final = rollout(x_init, U);
    std::vector<Eigen::VectorXd> X_vec(nbSteps);
    for (unsigned int i = 0; i < nbSteps; ++i) {
        X_vec[i] = X_final.row(i);
    }
    return X_vec;
}

#include <Eigen/Dense>
#include <vector>
#include <cmath>
#include <cassert>

// The solution function is assumed to be included above (or copied here)
// For completeness, we include the declaration:
std::vector<Eigen::VectorXd> computeStateTrajectory(const Eigen::VectorXd& x_init);

int main() {
    ILQRParam param; // from the solution (needs to be visible)
    Eigen::VectorXd x_init = Eigen::VectorXd::Zero(param.nbDoF);
    auto traj = computeStateTrajectory(x_init);
    // Check trajectory length
    assert(traj.size() == (unsigned int)(param.T / param.dt) + 1);
    // Check initial state matches
    assert(traj.front().isApprox(x_init, 1e-9));
    // Check final state is close to target within tolerance (since iLQR converges)
    Eigen::VectorXd target = param.X_d.tail(param.nbDoF);
    Eigen::VectorXd final_state = traj.back();
    // Position should be near target, orientation and velocities should be near zero
    assert(std::fabs(final_state(0) - target(0)) < 0.5);
    assert(std::fabs(final_state(1) - target(1)) < 0.5);
    assert(std::fabs(final_state(2)) < 0.5);
    assert(std::fabs(final_state(3)) < 1.0);
    assert(std::fabs(final_state(4)) < 1.0);
    assert(std::fabs(final_state(5)) < 1.0);
    // Check that all states have finite values
    for (const auto& state : traj) {
        assert(state.allFinite());
    }
    // Check that trajectory is continuous (no huge jumps between consecutive states)
    for (size_t i = 1; i < traj.size(); ++i) {
        Eigen::VectorXd delta = traj[i] - traj[i-1];
        // Bound on position and velocity changes (sanity check)
        assert(delta.norm() < 10.0);
    }
    // Optional: verify final cost is less than initial cost (just a heuristic)
    // Construct initial U and compute cost
    unsigned int nbSteps = static_cast<unsigned int>(param.T / param.dt);
    Eigen::MatrixXd U_init = 0.5 * param.m * param.g * Eigen::MatrixXd::Ones(nbSteps - 1, 2);
    Eigen::MatrixXd X_init_roll = rollout(x_init, U_init);
    double cost_init = cost(X_init_roll, U_init);
    // Recompute trajectory with final U? We don't have U, but we can approximate by rolling out with final trajectory's control differences.
    // For simplicity, just assert trajectory ends at a lower state error than initial.
    double final_pos_error = (final_state.head(3) - target.head(3)).norm();
    double init_pos_error = (X_init_roll.row(nbSteps-1).head(3).transpose() - target.head(3)).norm();
    assert(final_pos_error < init_pos_error);
    // If all passes, return 0
    return 0;
}

// The solution implements the iLQR algorithm which iteratively improves a control sequence to minimize a quadratic cost function. The main steps per iteration: (1) rollout the current control sequence to get the state trajectory using the dynamic model; (2) compute the cost; (3) linearize the dynamics along the trajectory by computing Jacobian matrices `A` (state transition) and `B` (control influence); (4) build the sensitivity matrix `Su` that maps control perturbations to state perturbations; (5) solve for the optimal control update `delta_u` via a linear least-squares problem using the normal equations `(Su^T Q Su + R) delta_u = Su^T Q (X_d - X) - R U`; (6) perform a line search on `delta_u` with decreasing alpha to ensure the cost decreases. The function returns the final state trajectory after all iterations. Edge cases: if the initial control is all zeros, the dynamics may produce unstable trajectories, so the initial guess is set to `0.5 * m * g` (half the force needed to hover) as in the snippet. The time complexity is dominated by constructing `Su` which is `O(n^2 * d)` where `n = T/dt = 100` steps and `d = 6` states, and solving the linear system of size `(2*(n-1)) x (2*(n-1))`, which is `O((2n)^3)` per iteration, giving `O(nbIter * n^3)` overall. Space complexity is `O(n^2)` due to storing `Su` and the cost matrices `Q` and `R` which are `(n*d) x (n*d)` and `(2*(n-1)) x (2*(n-1))` respectively.
