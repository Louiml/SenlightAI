/*
Write a C++ function named `integrateLorenz` that takes three doubles (`initialX`, `initialY`, `initialZ`), a time step size (`dt`), and a total integration time (`totalTime`) as parameters. The function must solve the Lorenz system using the classical fourth-order Runge-Kutta method (RK4) with a fixed time step, returning the final state as a `std::array<double, 3>`. The Lorenz system is defined by the equations: `dx/dt = sigma(y - x)`, `dy/dt = x(R - z) - y`, `dz/dt = xy - bz`, with constants `sigma = 10.0`, `R = 28.0`, and `b = 8.0/3.0`. The function should not rely on any external ODE solver library (like Boost) and must implement RK4 manually. Assume positive `dt` and `totalTime`, and handle cases where `totalTime` is not an exact multiple of `dt` by performing the last step with a reduced step size (or simply stopping at the last full step, but you must clearly state your approach). The function should be `const`-correct and use appropriate `std::array` operations for clarity.
*/
#include <array>
#include <cmath>
#include <algorithm>

// Compute the Lorenz system derivatives at a given state.
std::array<double, 3> lorenzDerivative(const std::array<double, 3>& state) {
    const double sigma = 10.0;
    const double R = 28.0;
    const double b = 8.0 / 3.0;
    return {
        sigma * (state[1] - state[0]),
        R * state[0] - state[1] - state[0] * state[2],
        -b * state[2] + state[0] * state[1]
    };
}

// Integrate the Lorenz system from t=0 to totalTime using fixed-step RK4.
// Returns the final state as a std::array<double, 3>.
std::array<double, 3> integrateLorenz(double initialX, double initialY, double initialZ,
                                      double dt, double totalTime) {
    std::array<double, 3> state = {initialX, initialY, initialZ};
    double t = 0.0;

    while (t < totalTime) {
        // Use a smaller step for the last interval to exactly reach totalTime.
        double dtEff = std::min(dt, totalTime - t);

        // RK4 stages
        std::array<double, 3> k1 = lorenzDerivative(state);
        std::array<double, 3> k2 = lorenzDerivative({state[0] + 0.5 * dtEff * k1[0],
                                                    state[1] + 0.5 * dtEff * k1[1],
                                                    state[2] + 0.5 * dtEff * k1[2]});
        std::array<double, 3> k3 = lorenzDerivative({state[0] + 0.5 * dtEff * k2[0],
                                                    state[1] + 0.5 * dtEff * k2[1],
                                                    state[2] + 0.5 * dtEff * k2[2]});
        std::array<double, 3> k4 = lorenzDerivative({state[0] + dtEff * k3[0],
                                                    state[1] + dtEff * k3[1],
                                                    state[2] + dtEff * k3[2]});

        // Weighted average
        state[0] += (dtEff / 6.0) * (k1[0] + 2.0 * k2[0] + 2.0 * k3[0] + k4[0]);
        state[1] += (dtEff / 6.0) * (k1[1] + 2.0 * k2[1] + 2.0 * k3[1] + k4[1]);
        state[2] += (dtEff / 6.0) * (k1[2] + 2.0 * k2[2] + 2.0 * k3[2] + k4[2]);

        t += dtEff;
    }

    return state;
}
#include <cassert>
#include <cmath>
#include <array>

// Declare the solution function (should match the provided implementation)
std::array<double, 3> integrateLorenz(double initialX, double initialY, double initialZ,
                                      double dt, double totalTime);

// Helper to compare arrays with tolerance
bool closeEnough(const std::array<double, 3>& a, const std::array<double, 3>& b, double tol = 1e-6) {
    return std::abs(a[0] - b[0]) < tol && std::abs(a[1] - b[1]) < tol && std::abs(a[2] - b[2]) < tol;
}

int main() {
    // Test 1: Zero total time returns initial state
    auto result0 = integrateLorenz(1.0, 2.0, 3.0, 0.01, 0.0);
    assert(closeEnough(result0, {1.0, 2.0, 3.0}));

    // Test 2: A short integration from a known start (verify by running equivalent code or analytical check)
    // Using a very small dt and time, the result should be close to initial + dt * derivative
    auto result1 = integrateLorenz(0.0, 1.0, 0.0, 0.01, 0.01);
    // At t=0, derivative for state (0,1,0) is: sigma*(1), R*0 - 1 - 0, -0 + 0 = (10, -1, 0)
    // Approximate final state after one tiny step ~ (0 + 0.01*10, 1 - 0.01*1, 0 + 0) = (0.1, 0.99, 0)
    assert(closeEnough(result1, {0.1, 0.99, 0.0}, 0.01));

    // Test 3: Symmetry - negative initial x should produce negative x component initially (for small time)
    auto result2 = integrateLorenz(-1.0, 0.0, 0.0, 0.001, 0.001);
    assert(result2[0] < 0.0);  // x remains negative for small time

    // Test 4: Increasing total time changes result (non-chaotic for short times)
    auto result3 = integrateLorenz(10.0, 10.0, 10.0, 0.01, 0.1);
    auto result4 = integrateLorenz(10.0, 10.0, 10.0, 0.01, 0.2);
    assert(!closeEnough(result3, result4, 0.5));  // should differ noticeably

    // Test 5: Smaller dt should converge to a more accurate solution (compare to larger dt)
    auto result_small_dt = integrateLorenz(5.0, 5.0, 5.0, 0.0001, 0.1);
    auto result_large_dt = integrateLorenz(5.0, 5.0, 5.0, 0.01, 0.1);
    // They should be close, but not exactly equal
    assert(std::abs(result_small_dt[0] - result_large_dt[0]) < 0.5);
    assert(std::abs(result_small_dt[1] - result_large_dt[1]) < 0.5);
    assert(std::abs(result_small_dt[2] - result_large_dt[2]) < 0.5);

    // Test 6: Non-multiple of dt for totalTime should still reach exactly totalTime
    auto result5 = integrateLorenz(0.0, 0.0, 0.0, 0.1, 0.25);
    // For zero initial state, system stays at zero (derivatives are all zero)
    assert(closeEnough(result5, {0.0, 0.0, 0.0}));

    return 0;
}
// The solution requires implementing the RK4 method for a system of three first-order ordinary differential equations. The main algorithm:  
// 1. Initialize `state = {initialX, initialY, initialZ}` and set `t = 0.0`.  
// 2. While `t < totalTime`:  
//    - Compute `dt_eff = min(dt, totalTime - t)` to handle the last step if total time isn't a multiple of dt.  
//    - Compute the four RK4 slopes: `k1 = f(t, state)`, `k2 = f(t + dt_eff/2, state + (dt_eff/2)*k1)`, `k3 = f(t + dt_eff/2, state + (dt_eff/2)*k2)`, `k4 = f(t + dt_eff, state + dt_eff*k3)`.  
//    - Update `state += (dt_eff/6)*(k1 + 2*k2 + 2*k3 + k4)`.  
//    - Increment `t += dt_eff`.  
// Here `f` is the Lorenz derivative function, which we can implement as a helper lambda or separate function. Edge cases:  
// - `totalTime == 0`: return initial state immediately.  
// - `dt` is very small: many iterations, but no special issue.  
// - The last step uses `dt_eff` to avoid overshooting.  
// Time complexity: O(totalTime / dt) evaluations of the derivative, each O(1). Space complexity: O(1) auxiliary (a few arrays). The solution does not use Boost, making it self-contained.
