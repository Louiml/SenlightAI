/*
Write a standalone C++ function that, given a symmetric positive-definite metric tensor \(G\) (represented as a square `std::vector<std::vector<double>>`), a velocity vector \(v\) (as a `std::vector<double>`), a density value \(\rho\), a viscosity value \(\mu\), an inverse permeability value \(\kappa\), the stabilization constant \(CI\), and an optional time-step delta \(dt\) (default = 0.0, meaning no time contribution), computes and returns a `std::pair<double,double>` containing the stabilization parameters \((\tau_M, \tau_C)\) as defined by the incompressible Navier-Stokes SUPG/PSPG formulation. In particular, define:
- \(G_{ij}\) is the metric tensor (assumed symmetric, but not necessarily diagonal).
- \(v_i\) are the velocity components.
- Let \(a = \rho^2 (v^T G v) + CI \cdot \mu^2 \cdot \text{sum}_{i,j} G_{ij}^2 + \kappa^2\). If \(dt > 0\), add \((\frac{2\rho}{dt})^2\) to \(a\).
- Compute \(T = \sqrt{a}\) (use the positive square root). Then \(\tau_M = \max(1/T, \epsilon)\) and \(\tau_C = 1/(\tau_M \cdot \text{trace}(G))\), where \(\epsilon = 10^{-12}\) (a small threshold).
- If the computed \(a\) is less than \(\epsilon\), set both \(\tau_M\) and \(\tau_C\) to 0 (i.e., treat as degenerate, no stabilization).
- Handle the case where \(\text{trace}(G) \le 0\) by returning \(\tau_C = 0\) (since division by non‑positive trace is invalid). The input vectors/tensors are non‑empty and correctly sized (i.e., \(\dim(v) = \dim(G)\) and \(G\) is square).

Your function must be named `compute_stabilization_parameters` and return a `std::pair<double, double>`.
*/

#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>

/**
 * Computes stabilization parameters tauM and tauC for incompressible flow using a metric tensor.
 * 
 * @param G          Symmetric positive-definite metric tensor (square matrix).
 * @param v          Velocity vector (same dimension as G's rows/cols).
 * @param rho        Density (positive).
 * @param mu         Viscosity (non-negative).
 * @param kappa      Inverse permeability (non-negative).
 * @param CI         Stabilization constant (positive).
 * @param dt         Time step; if > 0, adds time contribution. Default 0 (steady).
 * @return           Pair (tauM, tauC). Returns {0,0} for degenerate cases.
 */
std::pair<double, double> compute_stabilization_parameters(
    const std::vector<std::vector<double>>& G,
    const std::vector<double>&              v,
    double rho,
    double mu,
    double kappa,
    double CI,
    double dt = 0.0)
{
    // Basic validation: empty tensors or size mismatch -> degenerate
    if (G.empty() || G.size() != G[0].size() || G.size() != v.size()) {
        return {0.0, 0.0};
    }

    const double EPSILON = 1e-12;
    const std::size_t n = v.size();

    // Compute v^T G v
    double vTGv = 0.0;
    for (std::size_t i = 0; i < n; ++i) {
        double rowSum = 0.0;
        for (std::size_t j = 0; j < n; ++j) {
            rowSum += G[i][j] * v[j];
        }
        vTGv += v[i] * rowSum;
    }

    // Compute Frobenius norm squared of G: sum_{i,j} G_ij^2
    double frobSq = 0.0;
    double traceG = 0.0;
    for (std::size_t i = 0; i < n; ++i) {
        traceG += G[i][i];
        for (std::size_t j = 0; j < n; ++j) {
            double val = G[i][j];
            frobSq += val * val;
        }
    }

    // Build the scalar a
    double a = rho * rho * vTGv 
             + CI * mu * mu * frobSq 
             + kappa * kappa;

    // Add time contribution if dt is positive
    if (dt > 0.0) {
        double timeTerm = 2.0 * rho / dt;
        a += timeTerm * timeTerm;
    }

    // Degenerate case: a too small or negative due to rounding
    if (a < EPSILON) {
        return {0.0, 0.0};
    }

    // Compute tauM, ensuring lower bound
    double tauM = 1.0 / std::sqrt(a);
    tauM = std::max(tauM, EPSILON);

    // Compute tauC; invalid trace gives zero contribution
    if (traceG <= 0.0) {
        return {tauM, 0.0};
    }

    double tauC = 1.0 / (tauM * traceG);
    return {tauM, tauC};
}

#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// (Assume the solution function is declared above)

int main() {
    // Test 1: Simple scalar case (1D)
    {
        std::vector<std::vector<double>> G = {{2.0}};
        std::vector<double> v = {3.0};
        double rho = 1.0, mu = 0.1, kappa = 0.0, CI = 4.0;
        auto [tauM, tauC] = compute_stabilization_parameters(G, v, rho, mu, kappa, CI, 0.0);
        // a = 1^2*3^2*2 + 4*(0.1^2)*(2^2) + 0 = 18 + 0.16 = 18.16
        // tauM = 1/sqrt(18.16) ≈ 0.2347
        // traceG = 2 -> tauC = 1/(tauM*2)
        double expected_tauM = 1.0 / std::sqrt(18.16);
        assert(std::fabs(tauM - expected_tauM) < 1e-9);
        assert(std::fabs(tauC - 1.0/(expected_tauM*2.0)) < 1e-9);
    }

    // Test 2: 2D isotropic metric G = I, v = (1,2)
    {
        std::vector<std::vector<double>> G = {{1.0, 0.0}, {0.0, 1.0}};
        std::vector<double> v = {1.0, 2.0};
        double rho = 2.0, mu = 0.5, kappa = 3.0, CI = 1.0;
        auto [tauM, tauC] = compute_stabilization_parameters(G, v, rho, mu, kappa, CI, 0.0);
        // vTGv = 1^2+2^2 = 5, frobSq = 1+1 = 2, trace=2
        // a = 4*5 + 1*0.25*2 + 9 = 20+0.5+9 = 29.5
        // tauM = 1/sqrt(29.5), tauC = 1/(tauM*2)
        double a = 29.5;
        double exp_tauM = 1.0/std::sqrt(a);
        assert(std::fabs(tauM - exp_tauM) < 1e-9);
        assert(std::fabs(tauC - 1.0/(exp_tauM*2.0)) < 1e-9);
    }

    // Test 3: Time contribution adds to a
    {
        std::vector<std::vector<double>> G = {{1.0}};
        std::vector<double> v = {0.0};
        double rho = 2.0, mu = 0.0, kappa = 0.0, CI = 1.0;
        double dt = 0.5;
        auto [tauM, tauC] = compute_stabilization_parameters(G, v, rho, mu, kappa, CI, dt);
        // a = 0 + 0 + 0 + (2*2/0.5)^2 = (8)^2 = 64
        // tauM = 1/8 = 0.125, trace=1 -> tauC = 1/(0.125*1)=8
        assert(std::fabs(tauM - 0.125) < 1e-9);
        assert(std::fabs(tauC - 8.0) < 1e-9);
    }

    // Test 4: Degenerate when a < epsilon (all zero)
    {
        std::vector<std::vector<double>> G = {{0.0}};
        std::vector<double> v = {0.0};
        auto [tauM, tauC] = compute_stabilization_parameters(G, v, 1.0, 0.0, 0.0, 1.0, 0.0);
        assert(tauM == 0.0);
        assert(tauC == 0.0);
    }

    // Test 5: Trace zero (e.g., G = [[1,0],[0,-1]] not positive definite but still)
    {
        std::vector<std::vector<double>> G = {{1.0, 0.0}, {0.0, -1.0}};
        std::vector<double> v = {1.0, 0.0};
        // frobSq = 1+1=2, vTGv = 1, a = 1+CI*mu^2*2 + kappa^2 - we set mu=0,kappa=0,CI=1
        auto [tauM, tauC] = compute_stabilization_parameters(G, v, 1.0, 0.0, 0.0, 1.0, 0.0);
        // a = 1, tauM=1, trace=0 -> tauC=0
        assert(std::fabs(tauM - 1.0) < 1e-9);
        assert(tauC == 0.0);
    }

    // Test 6: Size mismatch leads to zero pair
    {
        std::vector<std::vector<double>> G = {{1.0, 0.0}, {0.0, 1.0}};
        std::vector<double> v = {1.0, 2.0, 3.0}; // wrong size
        auto [tauM, tauC] = compute_stabilization_parameters(G, v, 1.0, 0.0, 0.0, 1.0, 0.0);
        assert(tauM == 0.0 && tauC == 0.0);
    }

    // Test 7: Lower bound on tauM
    {
        std::vector<std::vector<double>> G = {{1.0}};
        std::vector<double> v = {0.0};
        // a = kappa^2 = 1e-6 (but this is < epsilon? No, 1e-6 > 1e-12, so tauM=1000, fine)
        auto [tauM, tauC] = compute_stabilization_parameters(G, v, 0.0, 0.0, 1e-6, 1.0, 0.0);
        // a = 1e-12 exactly? Actually kappa^2 = 1e-12, which is not < epsilon, so tauM = 1e6
        assert(std::fabs(tauM - 1e6) < 1e-6);
        assert(std::fabs(tauC - 1.0/(1e6*1.0)) < 1e-9);
    }

    return 0;
}

// The core algorithm follows directly from the provided C++ snippet's `eval_SP` function. Steps:
// 1. Validate input sizes: if the metric tensor is empty or its dimensions don't match the velocity vector length, return `{0,0}` (or throw, but the specification says handle gracefully). In practice, assume valid input.
// 2. Compute the velocity-weighted metric term: `vTGv = sum_{i,j} v[i]*G[i][j]*v[j]` (using double loops, O(n²)).
// 3. Compute the Frobenius-norm squared of G: `sum_{i,j} G[i][j]*G[i][j]` (O(n²)).
// 4. Compute the trace of G: `sum_i G[i][i]` (O(n)).
// 5. Accumulate `a = rho² * vTGv + CI * mu² * frobSq + kappa²`. If `dt > 0`, add `(2*rho/dt)²`.
// 6. If `a <= epsilon` (or `a < 0` due to rounding), return `{0,0}` because the stabilization is degenerate.
// 7. Otherwise compute `tauM = max(1.0/sqrt(a), epsilon)`. Note: `epsilon` is used as a floor for both `a` and `tauM`. In the original code, `a` is thresholded to `epsilon` before taking the square root, and then `tauM = max(sqrt(a)^-1, epsilon)`. Here we simplify: we compute `a` directly and if `a < epsilon`, we treat as degenerate. For `a >= epsilon`, we compute `tauM = max(1/sqrt(a), epsilon)`.
// 8. Compute traceG = trace(G). If `traceG <= 0`, return `{tauM, 0}`. Otherwise `tauC = 1.0 / (tauM * traceG)`.
// 9. Return `{tauM, tauC}`.
//
// Edge cases: 
// - If `a` is extremely large, `sqrt` and `1/sqrt` are fine. 
// - If `dt` is provided as negative (shouldn't happen), treat as no time term if `dt <= 0` (since time step can't be negative). 
// - If `G` is not symmetric, the formula still works but the problem states it's symmetric, so we don't need to symmetrize.
// - The threshold epsilon is a small positive constant; use `1e-12` as in the original code.
//
// Time complexity: O(n²) due to two nested loops for the two sums. Space complexity: O(1) beyond input storage.
