/*
Write a C++ function `compute_2D_dynamics_rhs` that, given the current values of the radial coordinate `R`, radial momentum `pR`, angular coordinate `psi` (an angle in radians), angular momentum conjugate `ppsi`, and a reduced mass `mu` (in atomic units), returns a `std::array<double, 4>` containing the time derivatives (right-hand sides) of the system \([R, pR, psi, ppsi]\) according to the Hamiltonian equations of motion for a 2D diatomic scattering problem. The potential energy is given by the function `ar_he_potential_der(R)`, whose derivative with respect to `R` is `ar_he_potential_der(R)`. The equations are:
- `dR/dt = pR / mu`
- `dpR/dt = (ppsi^2) / (mu * R^3) - ar_he_potential_der(R)`
- `dpsi/dt = ppsi / (mu * R^2)`
- `dppsi/dt = 0.0`
Assume `R > 0` (no division by zero), and handle any edge case such as extremely large `ppsi` or very small `R` naturally by the formulas (no additional checks required). The function must be `const`-correct and accept parameters by value. Provide the implementation in a header-only style, including necessary includes (`<array>`, `<cmath>`) and a declaration `double ar_he_potential_der(double R);` that the user must implement separately (or you may provide a stub that returns 0.0 for testing). The solution must not contain a `main` function.
*/
#include <array>
#include <cmath>

// External potential derivative (user-provided or stub for testing)
double ar_he_potential_der(double R);

/**
 * Compute the right-hand sides of the 2D diatomic Hamilton's equations.
 * @param R      Radial distance (must be > 0).
 * @param pR     Radial momentum.
 * @param psi    Angular coordinate (radians).
 * @param ppsi   Angular momentum conjugate to psi.
 * @param mu     Reduced mass (atomic units).
 * @return std::array<double,4> containing [dR/dt, dpR/dt, dpsi/dt, dppsi/dt].
 */
std::array<double, 4> compute_2D_dynamics_rhs(double R, double pR, double psi, double ppsi, double mu) {
    // dR/dt = pR / mu
    double dR_dt = pR / mu;
    
    // dpR/dt = ppsi^2 / (mu * R^3) - dV/dR
    double dpR_dt = (ppsi * ppsi) / (mu * R * R * R) - ar_he_potential_der(R);
    
    // dpsi/dt = ppsi / (mu * R^2)
    double dpsi_dt = ppsi / (mu * R * R);
    
    // dppsi/dt = 0 (conserved)
    double dppsi_dt = 0.0;
    
    return {dR_dt, dpR_dt, dpsi_dt, dppsi_dt};
}
#include <cassert>
#include <cmath>
#include <array>

// Stub potential derivative for testing (constant force -1.0)
double ar_he_potential_der(double R) {
    // In a real scenario this would be the derivative of the Ar–He potential.
    // For testing, use a simple constant derivative.
    return -1.0; // dV/dR = -1.0 means force = +1.0
}

// Include the solution function (in a real test, this would be from the header)
std::array<double, 4> compute_2D_dynamics_rhs(double R, double pR, double psi, double ppsi, double mu);

int main() {
    // Test 1: Simple case with known values
    double R = 2.0, pR = 1.0, psi = 0.5, ppsi = 3.0, mu = 2.0;
    auto out = compute_2D_dynamics_rhs(R, pR, psi, ppsi, mu);
    assert(std::fabs(out[0] - (1.0 / 2.0)) < 1e-12);           // dR/dt = 0.5
    assert(std::fabs(out[1] - (9.0 / (2.0 * 8.0) - (-1.0))) < 1e-12); // (9/(16)) +1 = 1.5625
    assert(std::fabs(out[2] - (3.0 / (2.0 * 4.0))) < 1e-12);   // dpsi/dt = 0.375
    assert(std::fabs(out[3]) < 1e-12);                         // dppsi/dt = 0

    // Test 2: Zero radial momentum -> dR/dt = 0
    out = compute_2D_dynamics_rhs(1.0, 0.0, 0.0, 2.0, 1.0);
    assert(std::fabs(out[0]) < 1e-12);
    assert(std::fabs(out[1] - (4.0 - (-1.0))) < 1e-12); // 4+1=5
    assert(std::fabs(out[2] - 2.0) < 1e-12);

    // Test 3: Zero angular momentum -> dpsi/dt = 0, dpR/dt = -dV/dR = 1.0
    out = compute_2D_dynamics_rhs(3.0, 5.0, 1.0, 0.0, 4.0);
    assert(std::fabs(out[0] - (5.0/4.0)) < 1e-12);
    assert(std::fabs(out[1] - (0.0 - (-1.0))) < 1e-12);
    assert(std::fabs(out[2]) < 1e-12);
    assert(std::fabs(out[3]) < 1e-12);

    // Test 4: Check that psi doesn't affect derivatives (angular symmetry)
    double R2 = 1.5, pR2 = -2.0, ppsi2 = 1.0, mu2 = 0.5;
    auto out_a = compute_2D_dynamics_rhs(R2, pR2, 0.0, ppsi2, mu2);
    auto out_b = compute_2D_dynamics_rhs(R2, pR2, 2.718, ppsi2, mu2);
    for (int i = 0; i < 4; ++i) {
        assert(std::fabs(out_a[i] - out_b[i]) < 1e-12);
    }

    // Test 5: Very small R but positive (no division by zero)
    R = 0.1; pR = 0.0; ppsi = 1.0; mu = 1.0;
    out = compute_2D_dynamics_rhs(R, pR, 0.0, ppsi, mu);
    assert(std::fabs(out[1] - (1.0 / 0.001 - (-1.0))) < 1e-9); // 1000+1=1001
    assert(std::fabs(out[2] - (1.0 / 0.01)) < 1e-9);          // 100

    return 0;
}
// The task is straightforward: implement the four ODE right-hand sides exactly as given. The main algorithm is purely algebraic — evaluate each derivative using the input parameters, with the only non-trivial computation being `ar_he_potential_der(R)`, which represents the derivative of the potential energy with respect to `R` (in atomic units, but we treat it as an external function). The edge case to consider is `R = 0`, which would cause division by zero; since the problem states `R > 0`, we can safely assume positive input. For extremely large `ppsi`, the derivative `dpR/dt` could overflow, but that is outside the scope; we simply compute as given. The time complexity is \(O(1)\) and space complexity \(O(1)\) because we only return a fixed-size array. The main correctness check is that the output components match the formulas exactly, and that the fourth component is always zero, reflecting the conservation of angular momentum (`ppsi` constant) because the Hamiltonian does not depend on `psi`.
