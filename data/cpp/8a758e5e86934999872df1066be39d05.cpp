Write a C++ function `simulatePendulum(double theta0, double omega0, int steps, double dt)` that simulates a simple pendulum's state (angle and angular velocity) using the given model: `dtheta/dt = omega` and `domega/dt = -(g/L)*sin(theta)`, where `g = 9.81` and `L = 0.5`. For each time step, use the provided Euler-like update pattern that first copies the current state into a buffer and then updates the state using the buffered (old) values, so the updates are synchronous (not in-place sequential). The function should return a `std::pair<double, double>` representing the final state `(theta, omega)` after `steps` steps. You must assume initial inputs are given in radians and may be any real numbers (including negative, zero, and large magnitudes where `sin` remains well-defined). Use double precision for all calculations; avoid using `M_PI` or any constant that might not be available — you can use `std::acos(-1.0)` for pi if needed, but for this task pi is not needed.

The core algorithm is a synchronous Euler integration: at each of the `steps` iterations, we first store the current theta and omega in local buffer variables (mirroring the `xbuf` behavior in the snippet), then compute the new state from those buffered values. Specifically, new theta = old omega * dt, and new omega = old omega - (g/L)*sin(old theta)*dt. This ensures that both updates use the same old state, matching the original snippet’s behavior where the buffer prevents using the newly updated theta when computing omega. Edge cases: `steps = 0` must return the initial state unchanged; negative steps should be treated as zero (no simulation) — we can clamp to zero. Large values of `steps` are fine because the loop is simple arithmetic; no errors accumulate beyond floating-point rounding. The `sin` function handles any real input. Time complexity is O(steps) and space complexity is O(1) auxiliary.

#include <utility>
#include <cmath>

// Simulate a simple pendulum using synchronous Euler integration.
// Returns the final (theta, omega) after 'steps' steps of size 'dt'.
std::pair<double, double> simulatePendulum(double theta0, double omega0, int steps, double dt) {
    const double g = 9.81;
    const double L = 0.5;
    
    // If steps is negative, treat as zero steps.
    if (steps <= 0) {
        return {theta0, omega0};
    }
    
    double theta = theta0;
    double omega = omega0;
    
    for (int i = 0; i < steps; ++i) {
        // Buffer the current state (old values) before updating.
        double theta_old = theta;
        double omega_old = omega;
        
        // Synchronous updates using the buffered old values.
        theta = omega_old * dt;
        omega = omega_old - (g / L) * std::sin(theta_old) * dt;
    }
    
    return {theta, omega};
}

#include <cassert>
#include <cmath>
#include <utility>

// The solution function (declare to avoid needing the header in test snippet)
std::pair<double, double> simulatePendulum(double theta0, double omega0, int steps, double dt);

int main() {
    // Step count zero returns initial state.
    auto r1 = simulatePendulum(0.5, 0.2, 0, 0.01);
    assert(std::fabs(r1.first - 0.5) < 1e-12);
    assert(std::fabs(r1.second - 0.2) < 1e-12);
    
    // One step from rest: theta becomes omega*dt = 0, omega becomes -g/L*sin(theta0)*dt.
    auto r2 = simulatePendulum(1.0, 0.0, 1, 0.1);
    double expected_omega = -(9.81 / 0.5) * std::sin(1.0) * 0.1;
    assert(std::fabs(r2.first - 0.0) < 1e-12);
    assert(std::fabs(r2.second - expected_omega) < 1e-12);
    
    // Two steps from initial (theta=0, omega=0) stays at (0,0) exactly.
    auto r3 = simulatePendulum(0.0, 0.0, 10, 0.5);
    assert(std::fabs(r3.first) < 1e-15);
    assert(std::fabs(r3.second) < 1e-15);
    
    // Negative step count is treated as no simulation.
    auto r4 = simulatePendulum(0.3, -0.7, -5, 0.01);
    assert(std::fabs(r4.first - 0.3) < 1e-12);
    assert(std::fabs(r4.second + 0.7) < 1e-12);
    
    // Large angle: ensures sin works for arbitrary values.
    auto r5 = simulatePendulum(1000.0, 2.5, 3, 0.01);
    // Manually compute using the same algorithm:
    double th = 1000.0, om = 2.5;
    for (int i = 0; i < 3; ++i) {
        double th_old = th, om_old = om;
        th = om_old * 0.01;
        om = om_old - (9.81/0.5) * std::sin(th_old) * 0.01;
    }
    assert(std::fabs(r5.first - th) < 1e-9);
    assert(std::fabs(r5.second - om) < 1e-9);
    
    // Non-zero dt with many steps (e.g., 100 steps) checks accumulation.
    auto r6 = simulatePendulum(0.1, 0.0, 100, 0.001);
    double th6 = 0.1, om6 = 0.0;
    for (int i = 0; i < 100; ++i) {
        double old_th = th6, old_om = om6;
        th6 = old_om * 0.001;
        om6 = old_om - (9.81/0.5) * std::sin(old_th) * 0.001;
    }
    assert(std::fabs(r6.first - th6) < 1e-12);
    assert(std::fabs(r6.second - om6) < 1e-12);
}
