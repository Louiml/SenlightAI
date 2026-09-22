// Implement a C++ function that performs one iteration of a nonlinear conjugate gradient (CG) method for minimizing a differentiable scalar function of a vector of real variables. The function should take as input a vector of current variables `x` (of type `std::vector<double>`), a vector of gradient values `g` (the gradient of the objective evaluated at `x`, same size), and a step-size parameter `alpha` (a positive double representing the maximum allowed step length). It should also take a reference to a previous search direction `h_prev` (same size, the CG direction from the previous iteration, initialized to zeros on the first call) and a reference to a scalar `beta` that will store the computed CG beta parameter. The function should update `h_prev` in place to the new search direction computed using the Polak–Ribière formula, restricted to be nonnegative (i.e., `beta = max(0, (g_new ⋅ (g_new − g_old)) / (g_old ⋅ g_old))`), but since we don't have the old gradient, we must simulate it: assume that the function is called with the gradient from the current point and also takes a parameter `g_old` (the gradient from the previous iteration, same size). The step size `alpha` must be clamped so that the proposed step `alpha * h` does not exceed a maximum displacement of 0.1 in any coordinate (i.e., after computing `h`, compute `max_abs_h = max(|h[i]|)`, then set `alpha = min(alpha, 0.1 / max_abs_h)` if `max_abs_h > 0`). The function should return the updated search direction `h` (via the reference) and also set `beta` to the computed value. The function should handle the edge case where the denominator `g_old ⋅ g_old` is zero (or extremely small, less than 1e-12) by setting `beta = 0`. Also, if the resulting direction `h = g + beta * h_prev` is not a descent direction (i.e., if `g ⋅ h <= 0`), then reset `h` to `g` (steepest descent). The function signature should be: `void cg_step(const std::vector<double>& x, const std::vector<double>& g, const std::vector<double>& g_old, double& alpha, std::vector<double>& h_prev, double& beta)`.
// The core of the conjugate gradient method is to compute a new search direction that is a combination of the current gradient and the previous search direction, using a scalar parameter beta that ensures conjugacy. The Polak–Ribière formula is `beta = max(0, (g_new ⋅ (g_new − g_old)) / (g_old ⋅ g_old))`. We compute the dot products: `num = Σ (g[i] * (g[i] - g_old[i]))` and `den = Σ (g_old[i] * g_old[i])`. If `den` is below a small threshold (1e-12) to avoid division by zero, we set `beta = 0`. Otherwise, `beta = max(0, num / den)`. The new search direction `h` is then `g[i] + beta * h_prev[i]`. However, we need to ensure it is a descent direction: if `Σ(g[i] * h[i]) <= 0`, we set `h[i] = g[i]` (steepest descent). Next, we compute the maximum absolute component of `h` (`max_abs_h`). If `max_abs_h > 0`, we clamp `alpha` to `min(alpha, 0.1 / max_abs_h)` so that the actual step `alpha * h` never moves any variable by more than 0.1. If `max_abs_h` is zero (degenerate), we set `alpha = 0`. Finally, we assign `h_prev = h` (the new direction becomes the previous for the next call). Edge cases: input vectors may be empty; if so, we do nothing and set `alpha = 0` and `beta = 0`. The algorithm runs in O(n) time and uses O(1) extra space beyond the input vectors (we modify `h_prev` in place, so we need a temporary copy of `h` to compute the dot product with `g` before overwriting, but that can be done with a single pass if we compute the dot product first, then build the new `h_prev`, then clamp alpha). Actually, to compute the descent check we need the new direction, so we can compute it into a temporary vector, then check, then copy back to `h_prev`, or we can compute it directly into `h_prev` but first save the old `g` and `g_old` dot products; the cleanest is to compute `h_new` in a local vector. Complexity is O(n).
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

/**
 * Perform one step of Polak–Ribière nonlinear conjugate gradient.
 * Updates the search direction h_prev in-place and adjusts the step size alpha.
 * 
 * @param x        Current variable vector (unused in the computation, but kept for API consistency).
 * @param g        Current gradient vector at x.
 * @param g_old    Gradient vector from the previous iteration.
 * @param alpha    In/out maximum step length; will be clamped to ensure max displacement <= 0.1.
 * @param h_prev   In/out previous search direction; on output holds the new search direction.
 * @param beta     Output: computed Polak–Ribière beta parameter (nonnegative).
 */
void cg_step(const std::vector<double>& x,
             const std::vector<double>& g,
             const std::vector<double>& g_old,
             double& alpha,
             std::vector<double>& h_prev,
             double& beta) {
    // Handle empty input gracefully.
    if (g.empty()) {
        alpha = 0.0;
        beta = 0.0;
        h_prev.clear();
        return;
    }

    const size_t n = g.size();
    // Use stable size in case g_old has different length (assume same; if not, treat as mismatch).
    const size_t m = std::min(n, g_old.size());

    // Compute Polak–Ribière numerator and denominator.
    double num = 0.0, den = 0.0;
    for (size_t i = 0; i < m; ++i) {
        const double diff = g[i] - g_old[i];
        num += g[i] * diff;
        den += g_old[i] * g_old[i];
    }

    // Avoid division by zero or near-zero denominator.
    const double eps = 1e-12;
    if (den < eps) {
        beta = 0.0;
    } else {
        beta = std::max(0.0, num / den);
    }

    // Build candidate new direction h = g + beta * h_prev.
    std::vector<double> h_new(n, 0.0);
    for (size_t i = 0; i < n; ++i) {
        h_new[i] = g[i] + beta * h_prev[i];
    }

    // Verify descent condition: g ⋅ h_new <= 0 means not descent, reset to steepest descent.
    double dot_gh = 0.0;
    for (size_t i = 0; i < n; ++i) {
        dot_gh += g[i] * h_new[i];
    }
    if (dot_gh <= 0.0) {
        h_new = g;  // steepest descent
        beta = 0.0; // reset beta as we used steepest descent
    }

    // Clamp alpha so that max(|alpha * h_new|) <= 0.1.
    double max_abs_h = 0.0;
    for (double v : h_new) {
        max_abs_h = std::max(max_abs_h, std::fabs(v));
    }
    if (max_abs_h > 0.0) {
        const double max_allowed_alpha = 0.1 / max_abs_h;
        alpha = std::min(alpha, max_allowed_alpha);
        // Ensure alpha is nonnegative; if negative input, set to 0.
        alpha = std::max(0.0, alpha);
    } else {
        // Degenerate direction.
        alpha = 0.0;
    }

    // Store the new direction into h_prev for the next call.
    h_prev.swap(h_new);
}
#include <cassert>
#include <vector>
#include <cmath>
#include <iostream>

// Declaration of the function to test.
void cg_step(const std::vector<double>& x,
             const std::vector<double>& g,
             const std::vector<double>& g_old,
             double& alpha,
             std::vector<double>& h_prev,
             double& beta);

int main() {
    // Test 1: Basic Polak–Ribière with positive beta.
    {
        std::vector<double> x = {0.0, 0.0};
        std::vector<double> g = {1.0, 2.0};
        std::vector<double> g_old = {0.5, 1.0};
        std::vector<double> h_prev = {0.0, 0.0}; // initial direction
        double alpha = 1.0;
        double beta = -1.0;
        cg_step(x, g, g_old, alpha, h_prev, beta);
        // Compute expected beta: num = 1*(0.5) + 2*(1.0) = 2.5, den = 0.25+1 = 1.25, beta = 2.0
        assert(std::fabs(beta - 2.0) < 1e-12);
        // h_new = g + beta*h_prev = [1,2] (since h_prev was zero)
        assert(std::fabs(h_prev[0] - 1.0) < 1e-12);
        assert(std::fabs(h_prev[1] - 2.0) < 1e-12);
        // max_abs_h = 2.0, so alpha clamped to min(1.0, 0.1/2.0=0.05) = 0.05
        assert(std::fabs(alpha - 0.05) < 1e-12);
    }

    // Test 2: Denominator zero => beta=0.
    {
        std::vector<double> x = {0.0};
        std::vector<double> g = {3.0};
        std::vector<double> g_old = {0.0};
        std::vector<double> h_prev = {1.0};
        double alpha = 0.5;
        double beta = 1.0;
        cg_step(x, g, g_old, alpha, h_prev, beta);
        assert(beta == 0.0);
        // h_new = g + 0*1 = 3.0, descent check: 3*3 > 0, ok.
        assert(std::fabs(h_prev[0] - 3.0) < 1e-12);
        // max_abs_h=3, alpha clamped to min(0.5, 0.1/3≈0.0333) = 0.0333...
        assert(std::fabs(alpha - 0.1/3.0) < 1e-12);
    }

    // Test 3: Non-descent direction resets to steepest descent.
    {
        // g = [-1, -1], g_old = [1,1] gives num = (-1)*(-2)+(-1)*(-2)=4, den=2, beta=2
        // h_prev initial = [1,0], so h_new = [-1,-1] + 2*[1,0] = [1,-1]
        // dot_gh = (-1)*1 + (-1)*(-1) = -1+1 = 0, so not descent (<=0), reset to g.
        std::vector<double> x = {0.0, 0.0};
        std::vector<double> g = {-1.0, -1.0};
        std::vector<double> g_old = {1.0, 1.0};
        std::vector<double> h_prev = {1.0, 0.0};
        double alpha = 1.0;
        double beta = 10.0;
        cg_step(x, g, g_old, alpha, h_prev, beta);
        assert(beta == 0.0);
        assert(std::fabs(h_prev[0] - (-1.0)) < 1e-12);
        assert(std::fabs(h_prev[1] - (-1.0)) < 1e-12);
        // max_abs_h=1, alpha clamped to min(1, 0.1) = 0.1
        assert(std::fabs(alpha - 0.1) < 1e-12);
    }

    // Test 4: Nonnegative beta (negative numerator => beta=0).
    {
        std::vector<double> x = {0.0};
        std::vector<double> g = {2.0};
        std::vector<double> g_old = {3.0}; // g older than g? Actually diff=-1, num=2*(-1)=-2, den=9 => beta=0
        std::vector<double> h_prev = {1.0};
        double alpha = 1.0;
        double beta = -1.0;
        cg_step(x, g, g_old, alpha, h_prev, beta);
        assert(beta == 0.0);
        // h_new = g = [2]
        assert(std::fabs(h_prev[0] - 2.0) < 1e-12);
        assert(std::fabs(alpha - 0.1/2.0) < 1e-12);
    }

    // Test 5: Empty vector.
    {
        std::vector<double> x, g, g_old, h_prev;
        double alpha = 1.0;
        double beta = 1.0;
        cg_step(x, g, g_old, alpha, h_prev, beta);
        assert(alpha == 0.0);
        assert(beta == 0.0);
        assert(h_prev.empty());
    }

    // Test 6: Large alpha clamped to 0.1/max.
    {
        std::vector<double> x = {0.0, 0.0, 0.0};
        std::vector<double> g = {10.0, -5.0, 3.0};
        std::vector<double> g_old = {9.0, -4.0, 2.0};
        std::vector<double> h_prev = {0.0, 0.0, 0.0};
        double alpha = 100.0;
        double beta = -1.0;
        cg_step(x, g, g_old, alpha, h_prev, beta);
        // beta = max(0, (10*1 + -5*(-1) + 3*1) / (81+16+4)) = (10+5+3)/101 = 18/101 ≈0.1782
        assert(std::fabs(beta - 18.0/101.0) < 1e-12);
        // h_new = g + beta*0 = [10,-5,3]
        assert(std::fabs(h_prev[0] - 10.0) < 1e-12);
        assert(std::fabs(h_prev[1] + 5.0) < 1e-12);
        assert(std::fabs(h_prev[2] - 3.0) < 1e-12);
        // max_abs_h = 10, so alpha = min(100, 0.1/10=0.01) = 0.01
        assert(std::fabs(alpha - 0.01) < 1e-12);
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}
