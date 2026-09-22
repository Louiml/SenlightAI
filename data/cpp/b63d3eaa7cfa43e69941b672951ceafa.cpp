Write a standalone C++ function named `computeJointTorques` that implements an inverse dynamics controller for a 7-degree-of-freedom robotic manipulator. The function must accept the following parameters: a 7-element vector of desired joint positions, a 7-element vector of desired joint velocities, a 7-element vector of desired joint accelerations, a proportional gain `kp`, a derivative gain `kd`, and a reference to a robot state object that provides access to the current joint positions, joint velocities, inertia matrix, Coriolis forces, and gravity torques via simple getter methods. The function returns a 7-element vector of computed control torques using the computed-torque method: `tau = M(q) * (qdd_des + kd*e_dot + kp*e) + C(q,qd) + G(q)`, where `e = qd_des - q` and `e_dot = qd_des_dot - qd`. The robot state must be passed as a reference to an abstract interface (or a simple struct) that defines the necessary getters. The implementation must be self-contained, include all necessary headers, and be free of any external dependencies beyond the C++ standard library.

The solution requires defining a simple robot state interface that exposes the current joint positions (`getJointPositions`), joint velocities (`getJointVelocities`), inertia matrix (`getInertiaMatrix`), Coriolis vector (`getCoriolis`), and gravity vector (`getGravity`). The main algorithm computes the tracking errors in joint space, forms the desired acceleration command as a combination of feedforward acceleration plus PD correction, and then multiplies by the inertia matrix, adding the nonlinear terms. Edge cases include ensuring all input vectors are of the correct size (7) and handling degenerate cases where gains are non-negative. Time complexity is dominated by the matrix-vector multiplication \(O(n^2)\) for an \(n \times n\) inertia matrix (here \(n=7\), so constant time), and the space complexity is \(O(n)\) for storing intermediate vectors. The solution must use `const` correctness for inputs and accessor methods, and return the computed torque vector by value.

#include <vector>
#include <cassert>

// Abstract robot state interface providing required dynamic quantities.
class RobotState {
public:
    virtual ~RobotState() = default;
    virtual std::vector<double> getJointPositions() const = 0;
    virtual std::vector<double> getJointVelocities() const = 0;
    virtual std::vector<std::vector<double>> getInertiaMatrix() const = 0;
    virtual std::vector<double> getCoriolis() const = 0;
    virtual std::vector<double> getGravity() const = 0;
};

// Compute inverse dynamics joint torques using a computed-torque PD controller.
// qdd_des: desired joint accelerations (size 7)
// qd_des: desired joint velocities (size 7)
// q_des: desired joint positions (size 7)
// kp: proportional gain (positive scalar)
// kd: derivative gain (positive scalar)
// robot: reference to a RobotState implementation
// returns: 7-dimensional torque vector
std::vector<double> computeJointTorques(
    const std::vector<double>& q_des,
    const std::vector<double>& qd_des,
    const std::vector<double>& qdd_des,
    double kp,
    double kd,
    const RobotState& robot) {
    
    // Validate input sizes.
    const size_t n = 7;
    assert(q_des.size() == n);
    assert(qd_des.size() == n);
    assert(qdd_des.size() == n);
    assert(robot.getJointPositions().size() == n);
    assert(robot.getJointVelocities().size() == n);
    
    // Fetch current state.
    const std::vector<double> q = robot.getJointPositions();
    const std::vector<double> qd = robot.getJointVelocities();
    const std::vector<std::vector<double>> M = robot.getInertiaMatrix();
    const std::vector<double> C = robot.getCoriolis();
    const std::vector<double> G = robot.getGravity();
    
    // Compute position and velocity errors.
    std::vector<double> e(n), e_dot(n);
    for (size_t i = 0; i < n; ++i) {
        e[i] = q_des[i] - q[i];
        e_dot[i] = qd_des[i] - qd[i];
    }
    
    // Compute desired acceleration command: qdd_cmd = qdd_des + kd*e_dot + kp*e
    std::vector<double> qdd_cmd(n);
    for (size_t i = 0; i < n; ++i) {
        qdd_cmd[i] = qdd_des[i] + kd * e_dot[i] + kp * e[i];
    }
    
    // Compute M * qdd_cmd
    std::vector<double> tau(n, 0.0);
    for (size_t i = 0; i < n; ++i) {
        double sum = 0.0;
        for (size_t j = 0; j < n; ++j) {
            sum += M[i][j] * qdd_cmd[j];
        }
        tau[i] = sum;
    }
    
    // Add Coriolis and gravity.
    for (size_t i = 0; i < n; ++i) {
        tau[i] += C[i] + G[i];
    }
    
    return tau;
}

#include <cassert>
#include <cmath>

// Simple concrete robot state for testing.
class SimpleRobot : public RobotState {
public:
    SimpleRobot(std::vector<double> q, std::vector<double> qd,
                std::vector<std::vector<double>> M,
                std::vector<double> C, std::vector<double> G)
        : q_(std::move(q)), qd_(std::move(qd)), M_(std::move(M)),
          C_(std::move(C)), G_(std::move(G)) {}
    
    std::vector<double> getJointPositions() const override { return q_; }
    std::vector<double> getJointVelocities() const override { return qd_; }
    std::vector<std::vector<double>> getInertiaMatrix() const override { return M_; }
    std::vector<double> getCoriolis() const override { return C_; }
    std::vector<double> getGravity() const override { return G_; }

private:
    std::vector<double> q_, qd_, C_, G_;
    std::vector<std::vector<double>> M_;
};

int main() {
    // Helper to create an identity-like inertia matrix.
    auto identity7 = []() {
        std::vector<std::vector<double>> M(7, std::vector<double>(7, 0.0));
        for (int i = 0; i < 7; ++i) M[i][i] = 1.0;
        return M;
    };
    
    // Case 1: Zero errors, zero dynamics → torque should be zero.
    std::vector<double> q_des(7, 0.0), qd_des(7, 0.0), qdd_des(7, 0.0);
    std::vector<double> q(7, 0.0), qd(7, 0.0), C(7, 0.0), G(7, 0.0);
    SimpleRobot robot1(q, qd, identity7(), C, G);
    auto tau1 = computeJointTorques(q_des, qd_des, qdd_des, 1.0, 1.0, robot1);
    for (int i = 0; i < 7; ++i) assert(std::fabs(tau1[i]) < 1e-12);
    
    // Case 2: Nonzero desired acceleration, zero errors, unit inertia, zero C/G.
    // Torque should equal desired acceleration.
    std::vector<double> qdd_des2 = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};
    SimpleRobot robot2(q, qd, identity7(), C, G);
    auto tau2 = computeJointTorques(q_des, qd_des, qdd_des2, 1.0, 1.0, robot2);
    for (int i = 0; i < 7; ++i) assert(std::fabs(tau2[i] - qdd_des2[i]) < 1e-12);
    
    // Case 3: Test proportional term: desired position offset from current.
    // With unit inertia, zero velocity/accel, zero C/G, tau should be kp * (q_des - q).
    std::vector<double> q_des3 = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    std::vector<double> q3(7, 0.0); // current position all zeros
    double kp = 5.0, kd = 2.0;
    SimpleRobot robot3(q3, qd, identity7(), C, G);
    auto tau3 = computeJointTorques(q_des3, qd_des, qdd_des, kp, kd, robot3);
    for (int i = 0; i < 7; ++i) assert(std::fabs(tau3[i] - kp * 1.0) < 1e-12);
    
    // Case 4: Test derivative term: desired velocity offset from current.
    // With zero position error, zero accel, unit inertia, tau should be kd * (qd_des - qd).
    std::vector<double> qd_des4(7, 2.0); // desired velocity all 2
    std::vector<double> qd4(7, 0.0);     // current velocity all 0
    SimpleRobot robot4(q, qd4, identity7(), C, G);
    auto tau4 = computeJointTorques(q_des, qd_des4, qdd_des, kp, kd, robot4);
    for (int i = 0; i < 7; ++i) assert(std::fabs(tau4[i] - kd * 2.0) < 1e-12);
    
    // Case 5: Test Coriolis and gravity addition.
    std::vector<double> C5(7, 1.0), G5(7, 2.0);
    SimpleRobot robot5(q, qd, identity7(), C5, G5);
    auto tau5 = computeJointTorques(q_des, qd_des, qdd_des, kp, kd, robot5);
    for (int i = 0; i < 7; ++i) assert(std::fabs(tau5[i] - 3.0) < 1e-12);
    
    return 0;
}
