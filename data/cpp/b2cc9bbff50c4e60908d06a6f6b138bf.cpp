Write a C++ function `mpcFirstStep` that, given a vector of 6 initial state values `x, y, psi, v, cte, epsi` and a vector of 3 polynomial coefficients representing a reference line (`f(x) = c0 + c1*x + c2*x^2`), simulates **one discrete time step** of the kinematic bicycle model with a **fixed steering angle `delta = 0.2` radians** and **fixed acceleration `a = 0.5`** (both in SI units). The kinematic model uses `Lf = 2.67` (distance from center of gravity to front axle), `dt = 0.05`. The cross-track error `cte` evolves as `cte_{t+1} = (f(x_t) - y_t) + v_t * sin(epsi_t) * dt`, and the heading error `epsi` evolves as `epsi_{t+1} = (psi_t - psides_t) + v_t * delta / Lf * dt`, where `psides_t = atan(c1 + 2*c2*x_t)` is the desired heading angle from the polynomial derivative. The function must return a `std::vector<double>` containing the updated state at time `t+1` in the order `{x, y, psi, v, cte, epsi}`. Handle the case where the polynomial is linear by setting `c2 = 0` if not provided (i.e., the input vector may have only 2 coefficients, in which case the missing coefficient is treated as 0). Use `const` references where appropriate and include only necessary headers.
The task simplifies the original MPC by removing the full optimization loop and instead computing a single forward simulation step using fixed control inputs (`delta = 0.2`, `a = 0.5`). The core algorithm applies the kinematic bicycle model equations: for each time step, we compute the next state variables. The kinematic equations are exactly those used in the original code snippet: `x_{t+1} = x_t + v_t * cos(psi_t) * dt`, `y_{t+1} = y_t + v_t * sin(psi_t) * dt`, `psi_{t+1} = psi_t + v_t * delta / Lf * dt`, `v_{t+1} = v_t + a * dt`. For `cte` and `epsi`, we need the desired line value and heading angle at `x_t`. The polynomial is given as `f(x) = c0 + c1*x + c2*x^2`, and we must handle the case where `c2` is missing by treating it as 0. Thus, we compute `f0 = c0 + c1*x + c2*x^2` and `psides = atan(c1 + 2*c2*x)`. Then apply the update formulas as in the snippet. Edge cases: empty input vectors? The problem specifies at least 2 coefficients must be provided; we can default to `c2=0` if the vector has size 2, and if it has size 3 use the third, otherwise we might abort or throw. For simplicity, if the vector size is less than 2, we assume `c1=0` and `c0=0` (this is not expected in normal use but defensive). Time complexity is `O(1)` because we only perform a fixed number of arithmetic operations. Space complexity is `O(1)` for the returned vector, which always has 6 elements.
#include <vector>
#include <cmath>

/**
 * Simulates one step of the kinematic bicycle model with fixed controls.
 * @param state A vector of 6 doubles: {x, y, psi, v, cte, epsi}
 * @param coeffs Polynomial coefficients {c0, c1, c2} for f(x) = c0 + c1*x + c2*x^2.
 *               If only 2 coefficients are provided, c2 is treated as 0.
 * @return Updated state {x_next, y_next, psi_next, v_next, cte_next, epsi_next}
 */
std::vector<double> mpcFirstStep(const std::vector<double>& state,
                                 const std::vector<double>& coeffs) {
    // Extract current state
    double x = state[0];
    double y = state[1];
    double psi = state[2];
    double v = state[3];
    double cte = state[4];
    double epsi = state[5];

    // Fixed controls
    const double delta = 0.2;
    const double a = 0.5;

    // Model constants
    const double Lf = 2.67;
    const double dt = 0.05;

    // Polynomial coefficients: default missing coefficients to 0
    double c0 = coeffs.size() > 0 ? coeffs[0] : 0.0;
    double c1 = coeffs.size() > 1 ? coeffs[1] : 0.0;
    double c2 = coeffs.size() > 2 ? coeffs[2] : 0.0;

    // Desired path value and heading at current x
    double f0 = c0 + c1 * x + c2 * x * x;
    double psides = std::atan(c1 + 2 * c2 * x);

    // Update state using kinematic bicycle model
    double x_next = x + v * std::cos(psi) * dt;
    double y_next = y + v * std::sin(psi) * dt;
    double psi_next = psi + v * delta / Lf * dt;
    double v_next = v + a * dt;
    double cte_next = (f0 - y) + v * std::sin(epsi) * dt;
    double epsi_next = (psi - psides) + v * delta / Lf * dt;

    return {x_next, y_next, psi_next, v_next, cte_next, epsi_next};
}
#include <cassert>
#include <cmath>
#include <vector>

// Declaration of the solution function
std::vector<double> mpcFirstStep(const std::vector<double>& state,
                                 const std::vector<double>& coeffs);

int main() {
    // Test 1: Basic case with linear polynomial (c2=0)
    std::vector<double> state1 = {0.0, 0.0, 0.0, 10.0, 0.0, 0.0};
    std::vector<double> coeffs1 = {1.0, 0.5}; // f(x)=1+0.5x
    auto result1 = mpcFirstStep(state1, coeffs1);
    // Expected: x = 0 + 10*cos(0)*0.05 = 0.5
    // y = 0 + 10*sin(0)*0.05 = 0
    // psi = 0 + 10*0.2/2.67*0.05 = 0.037453...
    // v = 10 + 0.5*0.05 = 10.025
    // cte = (1+0.5*0 - 0) + 10*sin(0)*0.05 = 1.0
    // epsi = (0 - atan(0.5)) + 10*0.2/2.67*0.05 = -atan(0.5)+0.037453...
    assert(std::abs(result1[0] - 0.5) < 1e-9);
    assert(std::abs(result1[1] - 0.0) < 1e-9);
    assert(std::abs(result1[2] - 0.0374531835) < 1e-6);
    assert(std::abs(result1[3] - 10.025) < 1e-9);
    assert(std::abs(result1[4] - 1.0) < 1e-9);
    assert(std::abs(result1[5] - (std::atan(0.5) * -1 + 0.0374531835)) < 1e-6);

    // Test 2: Quadratic polynomial, nonzero initial cte and epsi
    std::vector<double> state2 = {2.0, 1.0, 0.1, 5.0, 0.5, 0.05};
    std::vector<double> coeffs2 = {0.0, 1.0, 3.0}; // f(x)=x+3x^2
    auto result2 = mpcFirstStep(state2, coeffs2);
    // Compute manually
    double x = 2.0, y = 1.0, psi = 0.1, v = 5.0, cte = 0.5, epsi = 0.05;
    double f0 = 0 + 1*2 + 3*4 = 14.0;
    double psides = atan(1 + 2*3*2) = atan(13.0);
    double expected_x = 2 + 5*cos(0.1)*0.05;
    double expected_y = 1 + 5*sin(0.1)*0.05;
    double expected_psi = 0.1 + 5*0.2/2.67*0.05;
    double expected_v = 5 + 0.5*0.05 = 5.025;
    double expected_cte = (14.0 - 1.0) + 5*sin(0.05)*0.05;
    double expected_epsi = (0.1 - atan(13.0)) + 5*0.2/2.67*0.05;
    assert(std::abs(result2[0] - expected_x) < 1e-9);
    assert(std::abs(result2[1] - expected_y) < 1e-9);
    assert(std::abs(result2[2] - expected_psi) < 1e-9);
    assert(std::abs(result2[3] - expected_v) < 1e-9);
    assert(std::abs(result2[4] - expected_cte) < 1e-9);
    assert(std::abs(result2[5] - expected_epsi) < 1e-9);

    // Test 3: Zero velocity, zero controls effect
    std::vector<double> state3 = {0,0,0,0,0,0};
    std::vector<double> coeffs3 = {5, 2}; // f(x)=5+2x
    auto result3 = mpcFirstStep(state3, coeffs3);
    // x stays 0, y stays 0, psi stays 0, v becomes 0.025, cte = 5, epsi = -atan(2)
    assert(std::abs(result3[0] - 0.0) < 1e-12);
    assert(std::abs(result3[1] - 0.0) < 1e-12);
    assert(std::abs(result3[2] - 0.0) < 1e-12);
    assert(std::abs(result3[3] - 0.025) < 1e-12);
    assert(std::abs(result3[4] - 5.0) < 1e-12);
    assert(std::abs(result3[5] - std::atan(-2.0)) < 1e-12);

    return 0;
}
