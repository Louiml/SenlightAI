/*
Given four positive integer coefficients representing a 1D linear elastic wave equation with damping (mass density `rho`, damping coefficient `eta`, Young's modulus `E`, and Poisson ratio `nu`), write a C++ function that simulates one time step of the generalized-α time integration method. The function should take a current displacement vector `u0`, velocity vector `v0`, acceleration vector `a0`, and a forcing term `f` (all as `std::vector<double>` of equal length), plus time step `dt` and the method parameters `alpha_m`, `alpha_f`, `beta`, and `gamma` (all `double`). It should compute and return a `std::vector<double>` containing the new displacement `u` for the next time step, using the explicit formula derived from the Newmark-beta family: `u = u0 + dt*v0 + (0.5 - beta)*dt*dt*a0 + beta*dt*dt*a_new`, where `a_new` is determined by solving the semi-discrete equilibrium `M*a_new + C*v_new + K*u = f` with the generalized-α updates `v_new = v0 + dt*((1-gamma)*a0 + gamma*a_new)` and `u = u0 + dt*v0 + dt*dt*((0.5 - beta)*a0 + beta*a_new)`. Assume the system is scalar (each vector has exactly one component) and that the coefficients are constants. For simplicity, the forcing term `f` is a constant value applied at the current time step. The function must be named `stepGeneralizedAlpha` and take arguments in the order: `rho`, `eta`, `E`, `nu`, `dt`, `alpha_m`, `alpha_f`, `beta`, `gamma`, `f`, `u0`, `v0`, `a0`. It returns `std::vector<double>` with the new displacement.
*/

#include <vector>
#include <stdexcept>

/**
 * Perform one generalized-α time step for a scalar elastic wave equation.
 * Parameters:
 *   rho      - mass density (positive)
 *   eta      - damping coefficient (non-negative)
 *   E        - Young's modulus (positive)
 *   nu       - Poisson ratio (ignored for scalar, stiffness = E)
 *   dt       - time step (positive)
 *   alpha_m, alpha_f, beta, gamma - time integration parameters (typical: alpha_m=0.2, alpha_f=0.4, beta=0.36, gamma=0.7)
 *   f        - constant external force at current time step
 *   u0, v0, a0 - previous displacement, velocity, acceleration (each a single-element vector)
 * Returns a single-element vector containing the new displacement.
 */
std::vector<double> stepGeneralizedAlpha(
    double rho, double eta, double E, double nu,
    double dt, double alpha_m, double alpha_f,
    double beta, double gamma,
    double f,
    const std::vector<double>& u0,
    const std::vector<double>& v0,
    const std::vector<double>& a0)
{
    // Validate input
    if (u0.size() != 1 || v0.size() != 1 || a0.size() != 1)
        throw std::invalid_argument("Vectors must have exactly one element");
    if (dt <= 0.0)
        throw std::invalid_argument("Time step must be positive");
    if (rho <= 0.0)
        throw std::invalid_argument("Mass density must be positive");

    // Stiffness for 1D bar (nu ignored for scalar simplification)
    double K = E;

    // Extract scalar components
    double u0v = u0[0];
    double v0v = v0[0];
    double a0v = a0[0];

    // Effective mass
    double M_eff = rho + dt * gamma * eta + beta * dt * dt * K;

    // Effective force (derived from equilibrium at new time step)
    double F_eff = f - eta * (v0v + dt * (1.0 - gamma) * a0v)
                  - K * (u0v + dt * v0v + dt * dt * (0.5 - beta) * a0v);

    // New acceleration
    double a_new = F_eff / M_eff;

    // New displacement (Newmark update)
    double u_new = u0v + dt * v0v + dt * dt * ((0.5 - beta) * a0v + beta * a_new);

    return std::vector<double>{u_new};
}

#include <cassert>
#include <cmath>
#include <vector>

// Include or declare the solution function here (in a full program, paste the function above)

int main() {
    // Test 1: Zero initial conditions, no damping, unit constants, small dt, no force -> stays zero
    std::vector<double> u0(1, 0.0), v0(1, 0.0), a0(1, 0.0);
    auto u1 = stepGeneralizedAlpha(1.0, 0.0, 1.0, 0.0, 0.1, 0.2, 0.4, 0.36, 0.7, 0.0, u0, v0, a0);
    assert(std::abs(u1[0]) < 1e-12);

    // Test 2: Constant acceleration motion: u0=0, v0=1, a0=0, no force, no damping, dt=1, beta=0 -> exact u = 1
    std::vector<double> u0_2(1, 0.0), v0_2(1, 1.0), a0_2(1, 0.0);
    auto u2 = stepGeneralizedAlpha(1.0, 0.0, 0.0, 0.0, 1.0, 0.2, 0.4, 0.0, 0.7, 0.0, u0_2, v0_2, a0_2);
    assert(std::abs(u2[0] - 1.0) < 1e-12);

    // Test 3: Simple static equilibrium: u0=0, v0=0, a0=0, force=1, rho=1, eta=0, K=1, dt=1, beta=1 -> u=1
    std::vector<double> u0_3(1, 0.0), v0_3(1, 0.0), a0_3(1, 0.0);
    auto u3 = stepGeneralizedAlpha(1.0, 0.0, 1.0, 0.0, 1.0, 0.2, 0.4, 1.0, 0.7, 1.0, u0_3, v0_3, a0_3);
    assert(std::abs(u3[0] - 1.0) < 1e-12);

    // Test 4: Damping effect: u0=1, v0=0, a0=0, eta=1, rho=1, K=1, dt=0.5, beta=0.25, gamma=0.5 -> manual check
    std::vector<double> u0_4(1, 1.0), v0_4(1, 0.0), a0_4(1, 0.0);
    auto u4 = stepGeneralizedAlpha(1.0, 1.0, 1.0, 0.0, 0.5, 0.2, 0.4, 0.25, 0.5, 0.0, u0_4, v0_4, a0_4);
    // M_eff = 1 + 0.5*0.5*1 + 0.25*0.25*1 = 1.25
    // F_eff = 0 - 1*(0 + 0.5*1*0) - 1*(1 + 0.5*0 + 0.25*0) = -1
    // a_new = -1/1.25 = -0.8
    // u = 1 + 0.5*0 + 0.25*0 + 0.25*0.25*(-0.8) = 1 - 0.05 = 0.95
    assert(std::abs(u4[0] - 0.95) < 1e-12);

    // Test 5: Large force with high stiffness: force=100, K=10, rho=1, dt=0.1, beta=0.5, gamma=1.0 -> approximate static
    std::vector<double> u0_5(1, 0.0), v0_5(1, 0.0), a0_5(1, 0.0);
    auto u5 = stepGeneralizedAlpha(1.0, 0.0, 10.0, 0.0, 0.1, 0.2, 0.4, 0.5, 1.0, 100.0, u0_5, v0_5, a0_5);
    // M_eff = 1 + 0.1*1*0 + 0.5*0.01*10 = 1 + 0.05 = 1.05
    // F_eff = 100 - 0 - 10*0 = 100
    // a_new = 100/1.05 ≈ 95.238
    // u = 0 + 0.1*0 + 0.01*0 + 0.5*0.01*95.238 = 0.47619
    assert(std::abs(u5[0] - 0.476190476) < 1e-6);

    // Test 6: Invalid input (zero rho) should throw
    bool threw = false;
    try {
        stepGeneralizedAlpha(0.0, 0.0, 1.0, 0.0, 0.1, 0.2, 0.4, 0.36, 0.7, 0.0, u0, v0, a0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 7: Invalid vector size should throw
    std::vector<double> bad(2, 0.0);
    threw = false;
    try {
        stepGeneralizedAlpha(1.0, 0.0, 1.0, 0.0, 0.1, 0.2, 0.4, 0.36, 0.7, 0.0, bad, v0, a0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}

// The algorithm uses the generalized-α method, which is a variant of the Newmark family. Given the scalar governing equation `rho*a + eta*v + K*u = f` where `K = E/(1-nu)` for a 1D bar (since `lambda` and `mu` reduce to `E/(1-nu)` when `nu` is small, but here we assume `K = E` for simplicity? The problem statement says coefficients are positive integers, so we treat `K = E` as the stiffness coefficient in this scalar simplification). The generalized-α method uses two parameters `alpha_m` and `alpha_f` to weigh the acceleration and force terms at the current and previous steps. However, since we are computing a single time step from given initial states, we use the standard Newmark formulas with the effective stiffness approach:  
// 1. Compute the effective mass `M_eff = rho + dt*gamma*eta + (beta*dt*dt)*K` (since `K = E`).  
// 2. Compute the effective force `F_eff = f - eta*(v0 + dt*(1-gamma)*a0) - K*(u0 + dt*v0 + dt*dt*(0.5-beta)*a0)`.  
// 3. Solve for the new acceleration `a_new = F_eff / M_eff`.  
// 4. Then update `u = u0 + dt*v0 + dt*dt*((0.5-beta)*a0 + beta*a_new)` and return that displacement.
//
// Edge cases: `dt` must be positive; `rho` must be non-zero to avoid division by zero; the vectors must contain exactly one element; if `dt` is zero, the system is invalid. Time complexity is O(1) since the vectors have one element. Space complexity is O(1) for the returned vector.
