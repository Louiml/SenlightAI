// Implement a C++ function that solves a simplified inverse kinematics problem for a planar 2-link robotic arm using the Jacobian transpose method. Given a fixed arm with two revolute joints (each with an angular position in radians), link lengths `L1` and `L2`, and a desired end-effector target position `(tx, ty)`, the function should iteratively adjust the joint angles to bring the end-effector as close as possible to the target. The input is a starting configuration `(theta1, theta2)` (both in radians), the two link lengths, the target coordinates, and a maximum number of iterations `maxIter`. The function should return the final joint angles (as a `std::pair<double, double>`) after performing up to `maxIter` iterations of the Jacobian transpose update with a small step size (e.g., `alpha = 0.05`). The forward kinematics for the 2-link arm are: `x = L1*cos(theta1) + L2*cos(theta1+theta2)` and `y = L1*sin(theta1) + L2*sin(theta1+theta2)`. The Jacobian for the end-effector position with respect to the joint angles is a 2x2 matrix: rows for x and y, columns for theta1 and theta2. Use the transpose of this Jacobian multiplied by the error vector (target minus current position) to update the angles: `Δθ = alpha * J^T * error`. Stop early if the Euclidean distance from the end-effector to the target is less than a small tolerance (e.g., `1e-4`). The function must be self-contained, include appropriate headers, and use `const` where appropriate.
#include <cassert>
#include <cmath>
#include <utility>

// Function under test (copied here for standalone compilation)
std::pair<double, double> solveIKJacobianTranspose(
    double theta1, double theta2,
    double L1, double L2,
    double tx, double ty,
    int maxIter) {
    const double alpha = 0.05;
    const double tolerance = 1e-4;
    for (int iter = 0; iter < maxIter; ++iter) {
        double x = L1 * std::cos(theta1) + L2 * std::cos(theta1 + theta2);
        double y = L1 * std::sin(theta1) + L2 * std::sin(theta1 + theta2);
        double ex = tx - x;
        double ey = ty - y;
        double distSq = ex * ex + ey * ey;
        if (distSq < tolerance * tolerance) break;
        double sumAngle = theta1 + theta2;
        double j11 = -L1 * std::sin(theta1) - L2 * std::sin(sumAngle);
        double j12 = -L2 * std::sin(sumAngle);
        double j21 =  L1 * std::cos(theta1) + L2 * std::cos(sumAngle);
        double j22 =  L2 * std::cos(sumAngle);
        double delta1 = alpha * (j11 * ex + j21 * ey);
        double delta2 = alpha * (j12 * ex + j22 * ey);
        theta1 += delta1;
        theta2 += delta2;
    }
    return {theta1, theta2};
}

// Helper to compute forward kinematics for testing
std::pair<double, double> forwardKin(double theta1, double theta2, double L1, double L2) {
    double x = L1 * std::cos(theta1) + L2 * std::cos(theta1 + theta2);
    double y = L1 * std::sin(theta1) + L2 * std::sin(theta1 + theta2);
    return {x, y};
}

int main() {
    // Test 1: Simple reachable target, starting from rest
    {
        auto angles = solveIKJacobianTranspose(0.0, 0.0, 1.0, 1.0, 1.5, 0.0, 1000);
        auto pos = forwardKin(angles.first, angles.second, 1.0, 1.0);
        assert(std::abs(pos.first - 1.5) < 1e-3);
        assert(std::abs(pos.second - 0.0) < 1e-3);
    }

    // Test 2: Target directly below origin (requires both links pointing down)
    {
        auto angles = solveIKJacobianTranspose(1.0, -1.0, 1.0, 1.0, 0.0, -2.0, 1000);
        auto pos = forwardKin(angles.first, angles.second, 1.0, 1.0);
        assert(std::abs(pos.first - 0.0) < 1e-3);
        assert(std::abs(pos.second + 2.0) < 1e-3);
    }

    // Test 3: Target at the maximum reach (L1+L2) directly along x-axis
    {
        auto angles = solveIKJacobianTranspose(0.5, -0.3, 2.0, 1.0, 3.0, 0.0, 1000);
        auto pos = forwardKin(angles.first, angles.second, 2.0, 1.0);
        double dist = std::sqrt(pos.first * pos.first + pos.second * pos.second);
        assert(std::abs(dist - 3.0) < 1e-2); // Should reach close to boundary
    }

    // Test 4: Zero-link arm (degenerate)
    {
        auto angles = solveIKJacobianTranspose(0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 10);
        auto pos = forwardKin(angles.first, angles.second, 0.0, 0.0);
        assert(std::abs(pos.first - 0.0) < 1e-6);
        assert(std::abs(pos.second - 0.0) < 1e-6);
    }

    // Test 5: Already converged (target equals current position)
    {
        auto angles = solveIKJacobianTranspose(0.3, 0.7, 1.0, 0.5, 
            (1.0*std::cos(0.3)+0.5*std::cos(1.0)), 
            (1.0*std::sin(0.3)+0.5*std::sin(1.0)), 
            100);
        assert(std::abs(angles.first - 0.3) < 1e-6);
        assert(std::abs(angles.second - 0.7) < 1e-6);
    }

    return 0;
}
#include <utility>
#include <cmath>

/**
 * Solve a 2-link inverse kinematics problem using the Jacobian transpose method.
 * @param theta1  Initial angle of the first joint (radians)
 * @param theta2  Initial angle of the second joint (radians)
 * @param L1      Length of the first link
 * @param L2      Length of the second link
 * @param tx      Target x-coordinate
 * @param ty      Target y-coordinate
 * @param maxIter Maximum number of iterations to perform
 * @return        Final joint angles (theta1, theta2) after iteration
 */
std::pair<double, double> solveIKJacobianTranspose(
    double theta1, double theta2,
    double L1, double L2,
    double tx, double ty,
    int maxIter) {
    
    const double alpha = 0.05;          // step size
    const double tolerance = 1e-4;      // convergence threshold

    for (int iter = 0; iter < maxIter; ++iter) {
        // Forward kinematics: compute current end-effector position
        double x = L1 * std::cos(theta1) + L2 * std::cos(theta1 + theta2);
        double y = L1 * std::sin(theta1) + L2 * std::sin(theta1 + theta2);

        // Error vector
        double ex = tx - x;
        double ey = ty - y;

        // Check convergence
        double distSq = ex * ex + ey * ey;
        if (distSq < tolerance * tolerance) {
            break;
        }

        // Jacobian entries (partial derivatives of x,y wrt theta1,theta2)
        double sumAngle = theta1 + theta2;
        double j11 = -L1 * std::sin(theta1) - L2 * std::sin(sumAngle); // dx/dtheta1
        double j12 = -L2 * std::sin(sumAngle);                         // dx/dtheta2
        double j21 =  L1 * std::cos(theta1) + L2 * std::cos(sumAngle); // dy/dtheta1
        double j22 =  L2 * std::cos(sumAngle);                         // dy/dtheta2

        // Jacobian transpose * error
        double delta1 = alpha * (j11 * ex + j21 * ey);
        double delta2 = alpha * (j12 * ex + j22 * ey);

        // Update joint angles
        theta1 += delta1;
        theta2 += delta2;
    }

    return {theta1, theta2};
}
// The solution is iterative and follows the Jacobian transpose method for inverse kinematics. The main steps are: (1) compute the current end-effector position from the given joint angles using forward kinematics; (2) calculate the error vector as the difference between the target and the current position; (3) compute the 2x2 Jacobian matrix by taking partial derivatives of x and y with respect to theta1 and theta2; (4) update each joint angle by adding `alpha * (J^T * error)` component; (5) repeat until convergence (error norm below tolerance) or the maximum iteration count is reached. The Jacobian for a 2-link arm has entries: `J[0][0] = -L1*sin(theta1) - L2*sin(theta1+theta2)`, `J[0][1] = -L2*sin(theta1+theta2)`, `J[1][0] = L1*cos(theta1) + L2*cos(theta1+theta2)`, `J[1][1] = L2*cos(theta1+theta2)`. The transpose multiplied by error gives update terms: `d_theta1 = alpha * (J[0][0]*error_x + J[1][0]*error_y)` and `d_theta2 = alpha * (J[0][1]*error_x + J[1][1]*error_y)`. Edge cases include singular configurations where the Jacobian becomes rank-deficient (e.g., arm fully extended or folded), but since we use the transpose, it remains stable as long as the step size is small. The algorithm runs in constant time per iteration, so time complexity is O(maxIter), and space complexity is O(1) beyond the input. The method may not converge to an exact solution when the target is unreachable (distance greater than L1+L2), but it will bring the end-effector as close as possible along the reachable workspace boundary; in such cases, the function still returns the best-effort angles after the maximum iterations.
