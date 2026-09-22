Write a standalone C++ function named `integrateJacobiElliptic` that numerically integrates the system of ordinary differential equations  
x1' = x2 * x3,  
x2' = -x1 * x3,  
x3' = -0.51 * x1 * x2,  
with initial condition (x1(0), x2(0), x3(0)) = (0, 1, 1) over the time interval [0, 100]. The function must accept a step size `dt` (e.g., 0.01) and an output file name (e.g., `"elliptic_output.dat"`) as parameters. It should use the Boost Odeint library with the `bulirsch_stoer_dense_out` stepper with absolute and relative tolerances of 1e-9, and write tab-separated columns `t`, `x1`, `x2`, `x3` to the file with 16 decimal digits of precision. The function must return `void` and should not read from standard input or output to console. Ensure the function is self-contained by including only the necessary Boost headers (e.g., `boost/numeric/odeint.hpp` and `boost/array.hpp`) and standard headers. The solution should work for any positive `dt` (e.g., 0.001, 0.01, 0.1) and any valid filename string.

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

// Forward declaration of the solution function (should be defined elsewhere)
void integrateJacobiElliptic(double dt, const std::string& filename);

int main() {
    // Test 1: Integration with dt=0.01 produces a file with exactly 10001 lines (t=0 to 100 inclusive)
    integrateJacobiElliptic(0.01, "test_elliptic1.dat");
    {
        std::ifstream in("test_elliptic1.dat");
        int line_count = 0;
        std::string line;
        while (std::getline(in, line)) {
            if (!line.empty()) ++line_count;
        }
        assert(line_count == 10001);
    }

    // Test 2: First line should be t=0, x1=0, x2=1, x3=1
    {
        std::ifstream in("test_elliptic1.dat");
        std::string first_line;
        std::getline(in, first_line);
        std::istringstream iss(first_line);
        double t, x1, x2, x3;
        iss >> t >> x1 >> x2 >> x3;
        assert(std::abs(t - 0.0) < 1e-9);
        assert(std::abs(x1 - 0.0) < 1e-9);
        assert(std::abs(x2 - 1.0) < 1e-9);
        assert(std::abs(x3 - 1.0) < 1e-9);
    }

    // Test 3: Last line should have t=100 (within tolerance) and finite values
    {
        std::ifstream in("test_elliptic1.dat");
        std::string last_line;
        std::string line;
        while (std::getline(in, line)) {
            if (!line.empty()) last_line = line;
        }
        std::istringstream iss(last_line);
        double t, x1, x2, x3;
        iss >> t >> x1 >> x2 >> x3;
        assert(std::abs(t - 100.0) < 0.02); // allow one step tolerance
        assert(std::isfinite(x1) && std::isfinite(x2) && std::isfinite(x3));
    }

    // Test 4: Different dt=0.1 produces exactly 1001 lines
    integrateJacobiElliptic(0.1, "test_elliptic2.dat");
    {
        std::ifstream in("test_elliptic2.dat");
        int line_count = 0;
        std::string line;
        while (std::getline(in, line)) {
            if (!line.empty()) ++line_count;
        }
        assert(line_count == 1001);
    }

    // Test 5: The solution at time t=0 and t=100 are periodic (x1≈0, x2≈±1, x3≈±1) due to Jacobi elliptic properties
    {
        std::ifstream in("test_elliptic1.dat");
        std::string last_line;
        std::string line;
        while (std::getline(in, line)) {
            if (!line.empty()) last_line = line;
        }
        std::istringstream iss(last_line);
        double t, x1, x2, x3;
        iss >> t >> x1 >> x2 >> x3;
        // Since the period in time for the Jacobi elliptic functions with modulus sqrt(0.51) is ~4*K(k),
        // after 100 units the values might not be exactly back to initial, but they should be bounded.
        // Just assert the values are within [-1.1, 1.1].
        assert(std::abs(x1) < 1.1);
        assert(std::abs(x2) < 1.1);
        assert(std::abs(x3) < 1.1);
    }

    return 0;
}

#include <fstream>
#include <boost/array.hpp>
#include <boost/numeric/odeint.hpp>

// Integrate the Jacobi elliptic ODE system using adaptive dense output stepper.
void integrateJacobiElliptic(double dt, const std::string& filename) {
    typedef boost::array<double, 3> state_type;

    // System RHS: x1' = x2*x3, x2' = -x1*x3, x3' = -0.51*x1*x2
    auto rhs = [](const state_type& x, state_type& dxdt, double /*t*/) {
        const double m = 0.51;
        dxdt[0] = x[1] * x[2];
        dxdt[1] = -x[0] * x[2];
        dxdt[2] = -m * x[0] * x[1];
    };

    // Output writer
    std::ofstream out(filename);
    out.precision(16);
    auto write_out = [&out](const state_type& x, double t) {
        out << t << '\t' << x[0] << '\t' << x[1] << '\t' << x[2] << '\n';
    };

    // Initial condition
    state_type x = {{0.0, 1.0, 1.0}};
    double t0 = 0.0;
    double t_end = 100.0;

    // Adaptive dense-output Bulirsch–Stoer stepper
    boost::numeric::odeint::bulirsch_stoer_dense_out<state_type> stepper(1e-9, 1e-9, 1.0, 0.0);

    // Integrate with constant output intervals
    boost::numeric::odeint::integrate_const(stepper, rhs, x, t0, t_end, dt, write_out);
}

// The core problem is to numerically integrate a non-linear system of first-order ODEs using an adaptive-step-size solver from Boost Odeint. The `bulirsch_stoer_dense_out` stepper provides high accuracy with dense output, making it suitable for long integration intervals. The `integrate_const` function is used to ensure that output is generated at fixed intervals of `dt`, even though the internal stepping is adaptive. The system is autonomous (no explicit time dependence), so the time `t` in the RHS function is ignored. Important edge cases include: ensuring the output file is opened and closed properly, setting the output precision to 16 digits, and avoiding overflow or NaN by relying on the adaptive solver’s error control. The ODE system has periodic solutions (Jacobi elliptic functions), so the numerical solution should remain bounded; but for robustness, the stepper’s tolerance handles potential stiffness. Space complexity is O(1) beyond the file stream and stepper state. Time complexity is O(number of output steps × average internal substeps), which is proportional to 1/dt and the solver’s internal adaptivity. The function should not call `main` and must be standalone.
