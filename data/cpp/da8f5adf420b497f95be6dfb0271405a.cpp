// Write a standalone C++ function `computeOverlapGF` that computes a single component of a Gaussian overlap integral recurrence relation. Given a primitive Gaussian exponent `a_exp`, an array of `n` exponents `b_exps`, an array of `n` distances `pa_x`, and input arrays `df`, `fd`, and `ff` (each of length `n`) representing intermediate overlap integrals from lower angular momentum terms, the function must fill an output array `gf` of length `n` with the result of the recurrence formula: `gf[i] = 3.0 * df[i] * fe_0 + 2.0 * fd[i] * fe_0 + ff[i] * pa_x[i]`, where `fe_0 = 0.5 / (a_exp + b_exps[i])`. The function should accept all arrays as `const double*` (except `gf` which is `double*`), take `size_t n` for the number of elements, and use a simple loop without vectorization pragmas.
#include <cassert>
#include <cmath>
#include <cstddef>

// Declaration of the solution function (assume it is in a header or above)
namespace overlap {
void computeOverlapGF(double* gf,
                      const double* df,
                      const double* fd,
                      const double* ff,
                      const double* pa_x,
                      const double* b_exps,
                      double a_exp,
                      std::size_t n);
}

int main()
{
    const std::size_t n = 4;

    // Test case 1: simple values
    double a_exp = 1.0;
    double b_exps[4] = {1.0, 2.0, 3.0, 4.0};
    double pa_x[4] = {0.1, -0.2, 0.3, -0.4};
    double df[4] = {1.0, 2.0, 3.0, 4.0};
    double fd[4] = {0.5, 1.5, 2.5, 3.5};
    double ff[4] = {0.25, 0.75, 1.25, 1.75};
    double gf[4];

    overlap::computeOverlapGF(gf, df, fd, ff, pa_x, b_exps, a_exp, n);

    // Manually compute expected results
    for (std::size_t i = 0; i < n; ++i)
    {
        double fe_0 = 0.5 / (a_exp + b_exps[i]);
        double expected = 3.0 * df[i] * fe_0 + 2.0 * fd[i] * fe_0 + ff[i] * pa_x[i];
        assert(std::fabs(gf[i] - expected) < 1e-12);
    }

    // Test case 2: all zeros (output should be zeros)
    double b_exp_zero[3] = {1.0, 1.0, 1.0};
    double pa_x_zero[3] = {0.0, 0.0, 0.0};
    double df_zero[3] = {0.0, 0.0, 0.0};
    double fd_zero[3] = {0.0, 0.0, 0.0};
    double ff_zero[3] = {0.0, 0.0, 0.0};
    double gf_zero[3];
    overlap::computeOverlapGF(gf_zero, df_zero, fd_zero, ff_zero, pa_x_zero, b_exp_zero, 1.0, 3);
    for (std::size_t i = 0; i < 3; ++i)
    {
        assert(gf_zero[i] == 0.0);
    }

    // Test case 3: n = 0 (should not crash)
    overlap::computeOverlapGF(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 1.0, 0);

    // Test case 4: one element with known numbers
    double b_exps_one[1] = {3.0};
    double pa_x_one[1] = {2.0};
    double df_one[1] = {4.0};
    double fd_one[1] = {-1.0};
    double ff_one[1] = {5.0};
    double gf_one[1];
    overlap::computeOverlapGF(gf_one, df_one, fd_one, ff_one, pa_x_one, b_exps_one, 1.0, 1);
    double fe_0 = 0.5 / (1.0 + 3.0); // 0.125
    double expected_one = 3.0 * 4.0 * 0.125 + 2.0 * (-1.0) * 0.125 + 5.0 * 2.0;
    assert(std::fabs(gf_one[0] - expected_one) < 1e-12);

    return 0;
}
#include <cstddef>

namespace overlap {

// Compute a single overlap integral component using the recurrence:
// gf[i] = 3*df[i]/(a_exp + b_exps[i]) + 2*fd[i]/(a_exp + b_exps[i]) + ff[i]*pa_x[i]
// The factor 0.5 is folded into the coefficients, so the formula becomes:
// gf[i] = 3.0 * df[i] * fe_0 + 2.0 * fd[i] * fe_0 + ff[i] * pa_x[i],
// where fe_0 = 0.5 / (a_exp + b_exps[i]).
//
// Parameters:
//   gf    - output array of length n (overwritten)
//   df    - input array of length n (overlap from D/F combination)
//   fd    - input array of length n (overlap from F/D combination)
//   ff    - input array of length n (overlap from F/F combination)
//   pa_x  - input array of length n (x-component of PA vector)
//   b_exps - input array of length n (exponents of the second Gaussian)
//   a_exp - exponent of the first Gaussian (positive scalar)
//   n     - number of elements to process
void computeOverlapGF(double* gf,
                      const double* df,
                      const double* fd,
                      const double* ff,
                      const double* pa_x,
                      const double* b_exps,
                      double a_exp,
                      std::size_t n)
{
    for (std::size_t i = 0; i < n; ++i)
    {
        // Precompute the reciprocal factor
        const double fe_0 = 0.5 / (a_exp + b_exps[i]);

        // Apply the recurrence
        gf[i] = 3.0 * df[i] * fe_0 + 2.0 * fd[i] * fe_0 + ff[i] * pa_x[i];
    }
}

} // namespace overlap
// The solution is a straightforward element‑wise recurrence computation. For each index `i` from 0 to `n-1`, compute `fe_0` as half of the reciprocal of the sum of `a_exp` and `b_exps[i]`, then combine the three input arrays with coefficients 3, 2, and 1 respectively, adding the product of `ff[i]` with `pa_x[i]`. The function must be `const`‑correct by marking input pointers as `const double*` and the output pointer as `double*` (non‑const). Edge cases include `n = 0` (loop does nothing, which is safe) and potential division by zero if `a_exp + b_exps[i]` equals zero; for physical Gaussian exponents these are positive, so this is not a practical concern. The time complexity is \(O(n)\), and the auxiliary space complexity is \(O(1)\) beyond the input/output arrays. The function should be placed inside a namespace to avoid name clashes, and the loop should be a simple `for` loop without any OpenMP directives.
