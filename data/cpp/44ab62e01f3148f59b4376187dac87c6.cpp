/*
Write a standalone C++ function that, given a flat array of `float` values representing a 3x3 symmetric stress tensor for 16 independent material points (stored as 3 diagonal components followed by 3 off-diagonal components per point, in the order: xx, yy, zz, xy, yz, xz), along with per-point scalar parameters `p` (hydrostatic pressure), `mu` (shear modulus), and `kappa` (bulk modulus), computes the first Piola–Kirchhoff stress derivative (a 3x3 matrix) for the Neo-Hookean hyperelastic material model under the assumption that the deformation gradient is the identity. The output is a 9-component derivative matrix per point (in row-major order: dP_xx/dF_xx, dP_xx/dF_xy, ..., dP_zz/dF_zz). The function must take three input arrays: `sigma` (size `n*6`), `p`, `mu`, `kappa` (each size `n`), and an output array `dPdF` (size `n*9`), where `n` is the number of points. For the Neo-Hookean model, the stress derivative is purely isotropic and is given by: `dPdF = mu * (I ⊗ I) + kappa * (I ⊗ I)` for the diagonal blocks, meaning the derivative matrix is `mu + kappa` on the 3 diagonal entries (dP_ii/dF_ii) and zero everywhere else. Additionally, the function must apply a "definiteness fix": if the input flag `apply_definiteness_fix` is true, clamp the smallest eigenvalue of the 3x3 derivative matrix to a minimum of 0.1 (i.e., ensure all diagonal entries are at least 0.1). The function must be named `compute_neo_hookean_stress_derivative_identity` and be declared with appropriate `const` correctness for input parameters.
*/

#include <algorithm>
#include <cstddef>
#include <vector>

// Compute the first Piola-Kirchhoff stress derivative for the Neo-Hookean model
// at the identity deformation gradient for multiple material points.
//
// Parameters:
//   sigma:   Input array of size n*6, containing per-point stress components
//            (xx, yy, zz, xy, yz, xz). Not used in this simplified model.
//   p:       Input array of size n, hydrostatic pressure (not used directly).
//   mu:      Input array of size n, shear modulus.
//   kappa:   Input array of size n, bulk modulus.
//   apply_definiteness_fix: Input array of size n, boolean flags.
//   n:       Number of material points.
//   dPdF:    Output array of size n*9, row-major derivative matrix per point.
//
// The derivative is a 3x3 matrix with diagonal entries (mu + kappa) and zeros
// elsewhere. If the definiteness fix is enabled, diagonal entries are clamped
// to a minimum of 0.1.
void compute_neo_hookean_stress_derivative_identity(
    const float* sigma,
    const float* p,
    const float* mu,
    const float* kappa,
    const bool* apply_definiteness_fix,
    std::size_t n,
    float* dPdF)
{
    // sigma and p are not used in this simplified model; we ignore them
    // but keep them in the signature for completeness.
    (void)sigma;
    (void)p;

    for (std::size_t i = 0; i < n; ++i) {
        // Compute the base diagonal value
        float diag = mu[i] + kappa[i];

        // Apply definiteness fix if requested
        if (apply_definiteness_fix[i]) {
            diag = std::max(diag, 0.1f);
        }

        // Fill the 3x3 matrix in row-major order:
        // (0,0) -> offset 0, (0,1) -> 1, (0,2) -> 2,
        // (1,0) -> 3, (1,1) -> 4, (1,2) -> 5,
        // (2,0) -> 6, (2,1) -> 7, (2,2) -> 8
        float* out = dPdF + i * 9;
        for (int j = 0; j < 9; ++j) {
            out[j] = 0.0f;
        }
        out[0] = diag;
        out[4] = diag;
        out[8] = diag;
    }
}

#include <cassert>
#include <cmath>
#include <cstddef>
#include <vector>
#include <iostream>

// Declaration of the function to test
void compute_neo_hookean_stress_derivative_identity(
    const float* sigma,
    const float* p,
    const float* mu,
    const float* kappa,
    const bool* apply_definiteness_fix,
    std::size_t n,
    float* dPdF);

int main() {
    // Test case 1: Simple positive parameters, no definiteness fix
    {
        std::size_t n = 1;
        std::vector<float> sigma(6, 0.0f);
        std::vector<float> p(n, 1.0f);
        std::vector<float> mu(n, 2.0f);
        std::vector<float> kappa(n, 3.0f);
        std::vector<bool> fix(n, false);
        std::vector<float> dPdF(n * 9, 0.0f);

        compute_neo_hookean_stress_derivative_identity(
            sigma.data(), p.data(), mu.data(), kappa.data(), fix.data(), n, dPdF.data());

        assert(dPdF[0] == 5.0f);  // (0,0)
        assert(dPdF[4] == 5.0f);  // (1,1)
        assert(dPdF[8] == 5.0f);  // (2,2)
        // All off-diagonal entries should be zero
        for (int i = 0; i < 9; ++i) {
            if (i == 0 || i == 4 || i == 8) continue;
            assert(dPdF[i] == 0.0f);
        }
    }

    // Test case 2: Negative sum triggers definiteness fix
    {
        std::size_t n = 1;
        std::vector<float> sigma(6, 0.0f);
        std::vector<float> p(n, 0.0f);
        std::vector<float> mu(n, -1.0f);
        std::vector<float> kappa(n, 0.5f);  // sum = -0.5, fixed to 0.1
        std::vector<bool> fix(n, true);
        std::vector<float> dPdF(n * 9, 0.0f);

        compute_neo_hookean_stress_derivative_identity(
            sigma.data(), p.data(), mu.data(), kappa.data(), fix.data(), n, dPdF.data());

        assert(dPdF[0] == 0.1f);
        assert(dPdF[4] == 0.1f);
        assert(dPdF[8] == 0.1f);
    }

    // Test case 3: Definities fix not applied to negative sum
    {
        std::size_t n = 1;
        std::vector<float> sigma(6, 0.0f);
        std::vector<float> p(n, 0.0f);
        std::vector<float> mu(n, -1.0f);
        std::vector<float> kappa(n, 0.5f);  // sum = -0.5
        std::vector<bool> fix(n, false);
        std::vector<float> dPdF(n * 9, 0.0f);

        compute_neo_hookean_stress_derivative_identity(
            sigma.data(), p.data(), mu.data(), kappa.data(), fix.data(), n, dPdF.data());

        assert(dPdF[0] == -0.5f);
        assert(dPdF[4] == -0.5f);
        assert(dPdF[8] == -0.5f);
    }

    // Test case 4: Multiple points and zero size
    {
        std::size_t n = 3;
        std::vector<float> sigma(6 * n, 0.0f);
        std::vector<float> p(n, 0.0f);
        std::vector<float> mu(n);
        std::vector<float> kappa(n);
        mu[0] = 1.0f; kappa[0] = 2.0f;  // diag = 3.0
        mu[1] = 0.5f; kappa[1] = 0.2f;  // diag = 0.7
        mu[2] = 0.0f; kappa[2] = 0.05f; // diag = 0.05, fixed to 0.1
        std::vector<bool> fix(n, true);
        std::vector<float> dPdF(n * 9, 0.0f);

        compute_neo_hookean_stress_derivative_identity(
            sigma.data(), p.data(), mu.data(), kappa.data(), fix.data(), n, dPdF.data());

        assert(dPdF[0] == 3.0f);   // point 0
        assert(dPdF[13] == 0.7f);  // point 1, (1,1) at offset 9+4=13
        assert(dPdF[26] == 0.1f);  // point 2, (2,2) at offset 18+8=26

        // Test zero size
        std::vector<float> empty_dPdF;
        compute_neo_hookean_stress_derivative_identity(
            sigma.data(), p.data(), mu.data(), kappa.data(), fix.data(), 0, empty_dPdF.data());
        assert(empty_dPdF.empty());
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The task reduces to a per-point simple computation because for the identity deformation gradient, the Neo-Hookean stress derivative simplifies significantly. For each point `i`:
// 1. Extract the six stress components from `sigma` (but they are not directly used in this simplified model; we only need the material parameters and the output).
// 2. Compute the unclamped derivative matrix as a 3x3 matrix with diagonal entries `mu[i] + kappa[i]` and zeros elsewhere.
// 3. If `apply_definiteness_fix[i]` is true, clamp each diagonal entry to a minimum of 0.1 using `std::max(value, 0.1f)`.
// 4. Write the nine components (row-major) into `dPdF[i*9 ... i*9+8]`, with indices: (0,0)->0, (0,1)->1, (0,2)->2, (1,0)->3, (1,1)->4, (1,2)->5, (2,0)->6, (2,1)->7, (2,2)->8.
//
// Edge cases:
// - When `mu + kappa` is negative (unlikely but possible with random input), the definiteness fix clamps it to 0.1 if enabled.
// - The input `sigma` is unused in this simplified model; we can ignore it, but we still need to accept it for signature completeness.
// - The function must handle `n = 0` gracefully (no operations).
//
// Time complexity is O(n) because each point involves constant work. Space complexity is O(1) auxiliary (excluding the output array).
