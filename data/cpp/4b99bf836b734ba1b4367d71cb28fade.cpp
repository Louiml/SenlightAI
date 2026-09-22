// Write a C++ function that computes the solution of the simple harmonic oscillator differential equation system \(\dot{u}=v, \dot{v}=-u\) with initial conditions \(u(0)=1, v(0)=0\) using the semi-implicit Euler method. The function must take the step size \(dx\), initial independent variable \(x_0\), final independent variable \(x_{\text{max}}\), an initial condition array (as a `std::valarray<double>` containing \([u_0, v_0]\)), and a boolean flag to optionally write the computed points to a CSV file named `semi_implicit_euler.csv`. It must return the wall-clock time (in seconds) taken for the integration. The function must internally handle the ODE system by first updating the derivative using the current state, updating only the first component using that derivative, then recomputing the derivative and updating the second component; after the final step, it must return the time taken. The function must be self-contained (no reliance on global state) and match the specification exactly.
// The semi-implicit Euler method (also known as the symplectic Euler method) is a variant of the forward Euler method tailored for separable Hamiltonian systems. For the given ODE system, the method proceeds as follows for each integration step of size \(dx\):
// 1. Evaluate the derivative \(\dot{u} = v\) and \(\dot{v} = -u\) at the current state \((u_n, v_n)\).
// 2. Update the first variable: \(u_{n+1} = u_n + dx \cdot \dot{u}_n\).
// 3. Re-evaluate the derivative using the updated \(u_{n+1}\) and the old \(v_n\) (i.e., compute \(\dot{v}_{n+1} = -u_{n+1}\)).
// 4. Update the second variable: \(v_{n+1} = v_n + dx \cdot \dot{v}_{n+1}\).
// This semi-implicit approach improves numerical stability and preserves energy better than the fully explicit Euler method for oscillatory systems. The algorithm iterates from \(x = x_0\) to \(x_{\text{max}}\) in increments of \(dx\), writing each state (including the initial condition) to the CSV file if requested. Edge cases include a non-positive step size (the loop simply won't execute), an empty initial condition array (which would cause out-of-bounds access—the specification assumes valid input), and ensuring the output file is properly opened and closed. The time complexity is \(O(n)\) where \(n = \lfloor (x_{\text{max}} - x_0) / dx \rfloor + 1\) steps, and the space complexity is \(O(1)\) auxiliary space (excluding the output file storage and the input arrays). The function uses `std::valarray<double>` for the state vector to easily handle multiple variables.
#include <cmath>
#include <ctime>
#include <fstream>
#include <valarray>

/**
 * @brief Compute next step using the semi-implicit Euler method.
 * 
 * For the ODE system: u' = v, v' = -u.
 * The method updates u first using the old v, then recomputes v' and updates v.
 * 
 * @param dx Step size for the independent variable.
 * @param x Current value of the independent variable (unused but kept for generality).
 * @param y Current state vector (u, v); modified in place to the next state.
 */
void semi_implicit_euler_step(const double dx, const double& x,
                              std::valarray<double>* y) {
    (void)x; // parameter kept for API compatibility but not used in this problem
    // Evaluate derivatives using the current state.
    double u_dot = (*y)[1];       // du/dt = v
    double v_dot = -(*y)[0];      // dv/dt = -u

    // Update first variable (u) using the old derivative.
    (*y)[0] += dx * u_dot;

    // Re-evaluate derivative for v using the updated u.
    v_dot = -(*y)[0];             // dv/dt = -u_new

    // Update second variable (v) using the new derivative.
    (*y)[1] += dx * v_dot;
}

/**
 * @brief Compute the solution of the harmonic oscillator using semi-implicit Euler.
 * 
 * Integrates from x0 to x_max with step size dx. Optionally records each point
 * to the CSV file "semi_implicit_euler.csv" in the format: x,u,v per line.
 * 
 * @param dx Step size for the independent variable.
 * @param x0 Initial value of the independent variable.
 * @param x_max Final value of the independent variable.
 * @param y Initial condition vector (u0, v0); after the call, holds the final state.
 * @param save_to_file If true, writes the computed points to the CSV file.
 * @return double The wall-clock time taken for the integration in seconds.
 */
double semi_implicit_euler(double dx, double x0, double x_max,
                           std::valarray<double>* y,
                           bool save_to_file = false) {
    std::ofstream fp;
    if (save_to_file) {
        fp.open("semi_implicit_euler.csv", std::ofstream::out);
        if (!fp.is_open()) {
            // If we cannot open the file, fall back to no saving.
            save_to_file = false;
        }
    }

    std::clock_t t1 = std::clock();
    double x = x0;
    do {
        if (save_to_file && fp.is_open()) {
            // Write current state to file.
            fp << x << "," << (*y)[0] << "," << (*y)[1] << "\n";
        }
        semi_implicit_euler_step(dx, x, y);
        x += dx;
    } while (x <= x_max);

    std::clock_t t2 = std::clock();
    if (fp.is_open()) {
        fp.close();
    }
    return static_cast<double>(t2 - t1) / CLOCKS_PER_SEC;
}
#include <cassert>
#include <cmath>
#include <valarray>
#include <fstream>
#include <sstream>

// The solution function is declared here (or include the header if separated).
double semi_implicit_euler(double dx, double x0, double x_max,
                           std::valarray<double>* y,
                           bool save_to_file = false);

int main() {
    // Test 1: Basic integration with small step; check final values approximate cos(10) and -sin(10).
    {
        double dx = 0.01;
        std::valarray<double> y = {1.0, 0.0};
        semi_implicit_euler(dx, 0.0, 10.0, &y, false);
        assert(std::abs(y[0] - std::cos(10.0)) < 0.05);
        assert(std::abs(y[1] - (-std::sin(10.0))) < 0.05);
    }

    // Test 2: Zero step size should not change the state.
    {
        double dx = 0.0;
        std::valarray<double> y = {1.0, 0.0};
        semi_implicit_euler(dx, 0.0, 5.0, &y, false);
        assert(y[0] == 1.0);
        assert(y[1] == 0.0);
    }

    // Test 3: Negative step size should not execute the loop.
    {
        double dx = -0.1;
        std::valarray<double> y = {1.0, 0.0};
        semi_implicit_euler(dx, 0.0, 5.0, &y, false);
        assert(y[0] == 1.0);
        assert(y[1] == 0.0);
    }

    // Test 4: Single step with dx=1 should give u = 1, v = -1.
    {
        double dx = 1.0;
        std::valarray<double> y = {1.0, 0.0};
        semi_implicit_euler(dx, 0.0, 1.0, &y, false);
        // Step: u1 = 1 + 1*0 = 1; v1 = 0 + 1*(-1) = -1
        assert(std::abs(y[0] - 1.0) < 1e-12);
        assert(std::abs(y[1] - (-1.0)) < 1e-12);
    }

    // Test 5: File saving test. Check the CSV file is created and contains expected lines.
    {
        double dx = 1.0;
        std::valarray<double> y = {1.0, 0.0};
        semi_implicit_euler(dx, 0.0, 2.0, &y, true);
        std::ifstream file("semi_implicit_euler.csv");
        assert(file.is_open());
        std::string line;
        int count = 0;
        while (std::getline(file, line)) {
            count++;
        }
        // Expected 3 lines for x=0,1,2.
        assert(count == 3);
        file.close();
        // Clean up file (optional but for test isolation).
        std::remove("semi_implicit_euler.csv");
    }

    // Test 6: Larger dx still produces correct first-step values.
    {
        double dx = 2.0;
        std::valarray<double> y = {2.0, 3.0};
        semi_implicit_euler(dx, 0.0, 2.0, &y, false);
        // Manual calculation:
        // u_dot = 3, v_dot = -2
        // u1 = 2 + 2*3 = 8
        // v_dot_new = -8, v1 = 3 + 2*(-8) = -13
        assert(std::abs(y[0] - 8.0) < 1e-12);
        assert(std::abs(y[1] - (-13.0)) < 1e-12);
    }

    return 0;
}
