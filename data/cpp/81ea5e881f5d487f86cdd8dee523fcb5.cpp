Write a C++ function `simulateChemicalSystem` that models a simplified Chapman-like atmospheric chemistry mechanism with five species (O1D, O1, O3, NO, NO2) and two fixed species (M, O2). The function must accept a vector of variable concentrations, a vector of fixed concentrations, a temperature-like time parameter, and a boolean flag indicating whether to compute the time derivative or the Jacobian matrix. For time derivative computation, use the following rate equations: d[O1D]/dt = 1.2e-3*[O3] - 3.5e-11*[O1D]*[M], d[O1]/dt = 3.5e-11*[O1D]*[M] - 4.9e-34*[O1]*[O2]*[M] + 9.3e-12*[NO2]*[O1D] - 1.0e-13*[O1]*[O3] + 2.2e-11*[NO2]*[O1D], d[O3]/dt = 4.9e-34*[O1]*[O2]*[M] - 1.0e-13*[O1]*[O3] - 1.2e-3*[O3] + 9.3e-12*[NO]*[O3], d[NO]/dt = 9.3e-12*[NO2]*[O1D] - 9.3e-12*[NO]*[O3] + 1.0e-13*[O1]*[O3] - 9.3e-12*[NO2]*[O1D], d[NO2]/dt = 9.3e-12*[NO]*[O3] - 9.3e-12*[NO2]*[O1D] + 1.0e-13*[O1]*[O3] + 2.2e-11*[NO2]*[O1D] - 9.3e-12*[NO2]*[O1D]. For Jacobian computation, return a 5x5 matrix (nested `std::array<std::array<double,5>,5>`) where entry (i,j) is the partial derivative of d[i] with respect to var[j], using the explicit formulas derived from the rate equations (you may compute numeric approximations using finite differences with h = 1e-6 * max(1, |var[j]|) to keep the implementation simple). The function must validate that input vectors have exactly sizes 5 and 2 respectively, otherwise return an empty vector/zero matrix. The function must be `const` correct and not modify inputs.

#include <cassert>
#include <cmath>
#include <vector>
#include <array>

// Declare the function (already defined above)
std::vector<double> simulateChemicalSystem(
    const std::vector<double>& var,
    const std::vector<double>& fix,
    double t,
    bool derivative);

int main() {
    // Valid inputs
    std::vector<double> var = {9.906E+01, 6.624E+08, 5.326E+11, 8.725E+08, 2.240E+08};
    std::vector<double> fix = {8.120E+16, 1.697E+16};
    double t = 43200.0;

    // Test derivative
    auto deriv = simulateChemicalSystem(var, fix, t, true);
    assert(deriv.size() == 5);
    // Check a few values (rough, using relative tolerance)
    assert(std::abs(deriv[0] - (1.2e-3*var[2] - 3.5e-11*var[0]*fix[0])) < 1e-6 * std::abs(deriv[0]+1));

    // Test Jacobian size and symmetry correctness via a simpler check:
    // For a constant function (all derivatives zero), Jacobian should be zero
    std::vector<double> zero_var = {0,0,0,0,0};
    auto jac = simulateChemicalSystem(zero_var, fix, t, false);
    assert(jac.size() == 25);
    for (double v : jac) assert(v == 0.0);

    // Test that Jacobian of derivative with respect to var[0] is approximately correct
    // Using central difference manually for comparison
    double h = 1e-6 * std::max(1.0, std::abs(var[0]));
    auto plus_var = var; plus_var[0] += h;
    auto minus_var = var; minus_var[0] -= h;
    auto d_plus = simulateChemicalSystem(plus_var, fix, t, true);
    auto d_minus = simulateChemicalSystem(minus_var, fix, t, true);
    double fd_val = (d_plus[0] - d_minus[0]) / (2*h);
    auto jac_calc = simulateChemicalSystem(var, fix, t, false);
    assert(std::abs(jac_calc[0] - fd_val) < 1e-4 * std::max(1.0, std::abs(fd_val)));

    // Test invalid input size returns empty
    auto bad = simulateChemicalSystem({1,2}, fix, t, true);
    assert(bad.empty());
    auto bad_jac = simulateChemicalSystem({1,2}, fix, t, false);
    assert(bad_jac.size() == 25 && bad_jac[0] == 0.0);

    return 0;
}

#include <array>
#include <vector>
#include <cmath>
#include <stdexcept>

constexpr size_t NSPEC = 5;
constexpr size_t NFIX = 2;

// Compute time derivative or Jacobian for a simplified Chapman mechanism.
// mode = true  -> derivative, returns vector of size 5
// mode = false -> Jacobian, returns 5x5 array of partial derivatives
std::vector<double> simulateChemicalSystem(
    const std::vector<double>& var,
    const std::vector<double>& fix,
    double t,
    bool derivative) {

    // Validate input sizes
    if (var.size() != NSPEC || fix.size() != NFIX) {
        if (derivative) return {};
        return std::vector<double>(NSPEC * NSPEC, 0.0);
    }

    // Local references to concentrations
    double O1D = var[0];
    double O1  = var[1];
    double O3  = var[2];
    double NO  = var[3];
    double NO2 = var[4];
    double M   = fix[0];
    double O2  = fix[1];

    // Lambda to compute derivatives given current concentrations
    auto compute_deriv = [&](double a, double b, double c, double d, double e,
                             double m, double o2) -> std::array<double, NSPEC> {
        return {
            1.2e-3 * c - 3.5e-11 * a * m,
            3.5e-11 * a * m - 4.9e-34 * b * o2 * m + 9.3e-12 * e * a - 1.0e-13 * b * c + 2.2e-11 * e * a,
            4.9e-34 * b * o2 * m - 1.0e-13 * b * c - 1.2e-3 * c + 9.3e-12 * d * c,
            9.3e-12 * e * a - 9.3e-12 * d * c + 1.0e-13 * b * c - 9.3e-12 * e * a,
            9.3e-12 * d * c - 9.3e-12 * e * a + 1.0e-13 * b * c + 2.2e-11 * e * a - 9.3e-12 * e * a
        };
    };

    // If derivative requested
    if (derivative) {
        auto res = compute_deriv(O1D, O1, O3, NO, NO2, M, O2);
        return std::vector<double>(res.begin(), res.end());
    }

    // Jacobian via finite differences
    std::vector<double> J(NSPEC * NSPEC, 0.0);
    auto base = compute_deriv(O1D, O1, O3, NO, NO2, M, O2);

    for (size_t j = 0; j < NSPEC; ++j) {
        double delta = 1e-6 * std::max(1.0, std::abs(var[j]));
        double orig = var[j];
        var[j] = orig + delta;
        auto up = compute_deriv(var[0], var[1], var[2], var[3], var[4], M, O2);
        var[j] = orig - delta;
        auto down = compute_deriv(var[0], var[1], var[2], var[3], var[4], M, O2);
        var[j] = orig;

        for (size_t i = 0; i < NSPEC; ++i) {
            J[i * NSPEC + j] = (up[i] - down[i]) / (2.0 * delta);
        }
    }

    return J;
}

// The solution breaks into two clear paths based on the boolean flag. For the derivative path, we directly apply the five listed equations, reading from the input arrays and writing into a result array. For the Jacobian path, we use central or forward finite differences: for each variable j, we perturb var[j] by a small delta, recompute the full derivative vector, and store the column (or row) in the matrix. The finite difference approach avoids manual differentiation errors, handles all coupling terms automatically, and is acceptable for the task. Edge cases: if any input vector is the wrong size, return an empty result (size 0 for vector, all zeros for matrix). For finite differences, we must ensure delta is not zero and handle negative values appropriately. Time complexity: derivative computation is O(1) (only 5 species), Jacobian via finite differences is O(5) derivative evaluations = O(1) constant time. Space complexity is O(1) for fixed-size arrays. A more efficient closed-form Jacobian could be derived, but finite differences is sufficient and reduces risk of mistakes.
