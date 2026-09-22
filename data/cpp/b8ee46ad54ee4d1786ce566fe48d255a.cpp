/*
Write a C++ function that implements a numerical inverse kinematics solver for a simple 2-degree-of-freedom planar robot arm. Given a desired end-effector position (x, y) and the current joint angles (theta1, theta2), the function should iteratively adjust the joint angles using the Jacobian pseudo-inverse method until the end-effector reaches the desired position within a tolerance of 1e-6, or up to 1000 iterations. The forward kinematics are: x = L1*cos(theta1) + L2*cos(theta1+theta2), y = L1*sin(theta1) + L2*sin(theta1+theta2), with link lengths L1=1.0 and L2=0.5. The function should return a struct containing the solved joint angles and a boolean indicating convergence. If the algorithm does not converge, it should return the initial joint angles.
*/

#include <cmath>
#include <algorithm>
#include <stdexcept>

struct IKResult {
    double theta1;
    double theta2;
    bool converged;
};

// Forward kinematics for a 2R planar arm
void forwardKinematics(double theta1, double theta2, double L1, double L2, double& x, double& y) {
    x = L1 * std::cos(theta1) + L2 * std::cos(theta1 + theta2);
    y = L1 * std::sin(theta1) + L2 * std::sin(theta1 + theta2);
}

// Numerical inverse kinematics using Jacobian pseudo-inverse
IKResult inverseKinematics2R(double desired_x, double desired_y, double initial_theta1, double initial_theta2, double L1 = 1.0, double L2 = 0.5) {
    const double tolerance = 1e-6;
    const int max_iterations = 1000;
    
    double theta1 = initial_theta1;
    double theta2 = initial_theta2;
    
    for (int iter = 0; iter < max_iterations; ++iter) {
        // Compute current position
        double x, y;
        forwardKinematics(theta1, theta2, L1, L2, x, y);
        
        // Error vector
        double ex = desired_x - x;
        double ey = desired_y - y;
        
        // Jacobian matrix elements
        double J11 = -L1 * std::sin(theta1) - L2 * std::sin(theta1 + theta2);
        double J12 = -L2 * std::sin(theta1 + theta2);
        double J21 =  L1 * std::cos(theta1) + L2 * std::cos(theta1 + theta2);
        double J22 =  L2 * std::cos(theta1 + theta2);
        
        // Compute determinant to check singularity
        double det = J11 * J22 - J12 * J21;
        if (std::abs(det) < 1e-12) {
            // Singular configuration, cannot proceed
            return {initial_theta1, initial_theta2, false};
        }
        
        // Inverse of Jacobian (since 2x2, invert directly)
        double inv_det = 1.0 / det;
        double invJ11 =  J22 * inv_det;
        double invJ12 = -J12 * inv_det;
        double invJ21 = -J21 * inv_det;
        double invJ22 =  J11 * inv_det;
        
        // Compute joint change: dq = J_inv * error
        double dq1 = invJ11 * ex + invJ12 * ey;
        double dq2 = invJ21 * ex + invJ22 * ey;
        
        // Scale down if too large to prevent overshooting
        double max_abs = std::max(std::abs(dq1), std::abs(dq2));
        while (max_abs > 1.0) {
            dq1 /= 10.0;
            dq2 /= 10.0;
            max_abs /= 10.0;
        }
        
        // Update joint angles
        theta1 += dq1;
        theta2 += dq2;
        
        // Check convergence
        if (max_abs < tolerance) {
            return {theta1, theta2, true};
        }
    }
    
    // Did not converge
    return {initial_theta1, initial_theta2, false};
}

#include <cassert>
#include <cmath>
#include <iostream>

// Include the solution function here (or link)

int main() {
    // Test 1: Known configuration - theta1=0, theta2=0 -> (1.5, 0)
    IKResult r1 = inverseKinematics2R(1.5, 0.0, 0.0, 0.0);
    assert(r1.converged);
    assert(std::abs(r1.theta1) < 1e-5);
    assert(std::abs(r1.theta2) < 1e-5);
    
    // Test 2: Straight up - theta1=pi/2, theta2=0 -> (0, 1.5)
    IKResult r2 = inverseKinematics2R(0.0, 1.5, 0.0, 0.0);
    assert(r2.converged);
    assert(std::abs(r2.theta1 - M_PI/2) < 1e-4);
    assert(std::abs(r2.theta2) < 1e-4);
    
    // Test 3: Elbow bend - theta1=0, theta2=pi/2 -> (1.0, 0.5)
    IKResult r3 = inverseKinematics2R(1.0, 0.5, 0.0, 0.0);
    assert(r3.converged);
    assert(std::abs(r3.theta1) < 1e-4);
    assert(std::abs(r3.theta2 - M_PI/2) < 1e-4);
    
    // Test 4: Non-convergence - unreachable point (e.g., (5,0))
    IKResult r4 = inverseKinematics2R(5.0, 0.0, 0.0, 0.0);
    // This should not converge because max reach is 1.5
    assert(!r4.converged);
    assert(r4.theta1 == 0.0);
    assert(r4.theta2 == 0.0);
    
    // Test 5: Starting from different initial guess still converges
    IKResult r5 = inverseKinematics2R(0.5, 0.5, 0.3, -0.2);
    assert(r5.converged);
    // Verify forward kinematics from solution
    double x, y;
    forwardKinematics(r5.theta1, r5.theta2, 1.0, 0.5, x, y);
    assert(std::abs(x - 0.5) < 1e-4);
    assert(std::abs(y - 0.5) < 1e-4);
    
    // Test 6: Reachable point at interior works
    IKResult r6 = inverseKinematics2R(1.0, 0.0, 0.0, 0.0);
    assert(r6.converged);
    double x6, y6;
    forwardKinematics(r6.theta1, r6.theta2, 1.0, 0.5, x6, y6);
    assert(std::abs(x6 - 1.0) < 1e-4);
    assert(std::abs(y6) < 1e-4);
    
    // Test 7: Negative coordinates
    IKResult r7 = inverseKinematics2R(-0.5, 0.5, 0.0, 0.0);
    assert(r7.converged);
    double x7, y7;
    forwardKinematics(r7.theta1, r7.theta2, 1.0, 0.5, x7, y7);
    assert(std::abs(x7 + 0.5) < 1e-4);
    assert(std::abs(y7 - 0.5) < 1e-4);
    
    // Test 8: Multiple iterations needed, verify final accuracy
    IKResult r8 = inverseKinematics2R(0.1, 0.2, -1.0, 1.0);
    assert(r8.converged);
    double x8, y8;
    forwardKinematics(r8.theta1, r8.theta2, 1.0, 0.5, x8, y8);
    assert(std::abs(x8 - 0.1) < 1e-4);
    assert(std::abs(y8 - 0.2) < 1e-4);
    
    // Test 9: Full extension along negative x
    IKResult r9 = inverseKinematics2R(-1.5, 0.0, 0.0, 0.0);
    assert(r9.converged);
    assert(std::abs(r9.theta1 - M_PI) < 1e-4);
    assert(std::abs(r9.theta2) < 1e-4);
    
    // Test 10: Singular point - fully outstretched (already at limit)
    // This is at boundary but still converges
    IKResult r10 = inverseKinematics2R(0.0, -1.5, 0.0, 0.0);
    assert(r10.converged);
    double x10, y10;
    forwardKinematics(r10.theta1, r10.theta2, 1.0, 0.5, x10, y10);
    assert(std::abs(x10) < 1e-4);
    assert(std::abs(y10 + 1.5) < 1e-4);
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The solution uses a standard Jacobian-based iterative approach. For a 2R planar arm, the Jacobian J maps joint velocities to end-effector velocities: J = [[-L1*sin(t1)-L2*sin(t1+t2), -L2*sin(t1+t2)], [L1*cos(t1)+L2*cos(t1+t2), L2*cos(t1+t2)]]. At each iteration, compute the error vector e = (desired - current). Compute the pseudo-inverse of J (for a 2x2 non-singular matrix, this is just the inverse). Then update joint angles: dq = J_inv * e. To avoid overshooting, scale dq down if its max absolute value exceeds 1.0 (divide by 10 repeatedly). Update the joint angles and recompute forward kinematics. Continue until the norm of dq is less than 1e-6 or 1000 iterations are reached. Edge cases: singular Jacobian (when L1*cos(t2) - L2*cos(t2) = 0 or equivalent) — the pseudo-inverse would fail; in this simplified task, we handle by checking if determinant is near zero and break with non-convergence. Time complexity: O(N) where N is number of iterations (max 1000), each iteration does constant work. Space complexity: O(1).
