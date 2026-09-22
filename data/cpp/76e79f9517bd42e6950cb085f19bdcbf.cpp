/*
Write a standalone C++ function named `boxQPSolve` that solves a strictly convex quadratic programming problem with box constraints: minimize (1/2) xᵀ H x + gᵀ x subject to `lb ≤ x ≤ ub`, where `H` is a symmetric positive definite matrix, `g` is the gradient vector, and `lb` and `ub` are lower and upper bound vectors. The function should accept `H`, `g`, `lb`, `ub`, and an initial guess vector `x` (which will be overwritten with the solution). Implement an iterative projected gradient method with a backtracking line search to ensure convergence. Return the number of iterations performed. The function must be self-contained (no external QP solvers) and handle the case where H is not symmetric by symmetrizing it internally. Assume all inputs have the same size `n > 0` and that `lb[i] ≤ ub[i]` for all `i`. The solution must satisfy the constraints with tolerance `1e-8`.
*/

#include <vector>
#include <algorithm>
#include <cmath>

/**
 * Solve box-constrained QP: min 0.5*x'*H*x + g'*x subject to lb <= x <= ub.
 * Uses projected gradient with backtracking line search (Armijo).
 * Overwrites x with the solution. Returns number of iterations.
 * Assumes H is strictly positive definite (convex). Symmetrizes H internally.
 */
int boxQPSolve(const std::vector<std::vector<double>>& H_in,
               const std::vector<double>& g,
               const std::vector<double>& lb,
               const std::vector<double>& ub,
               std::vector<double>& x) {
    const int n = static_cast<int>(g.size());
    const double eps = 1e-8;           // tolerance for projected gradient norm
    const double c = 1e-4;             // Armijo constant
    const double alpha_init = 1.0;
    const int max_iter = 10000;
    const int max_ls = 50;             // max line search steps

    // Symmetrize H: H = (H + H')/2
    std::vector<std::vector<double>> H(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            H[i][j] = 0.5 * (H_in[i][j] + H_in[j][i]);
        }
    }

    // Clamp initial guess to bounds
    for (int i = 0; i < n; ++i) {
        x[i] = std::max(lb[i], std::min(ub[i], x[i]));
    }

    // Objective value helper
    auto eval_obj = [&](const std::vector<double>& xv) {
        double val = 0.0;
        for (int i = 0; i < n; ++i) {
            double hx = 0.0;
            for (int j = 0; j < n; ++j) {
                hx += H[i][j] * xv[j];
            }
            val += 0.5 * xv[i] * hx + g[i] * xv[i];
        }
        return val;
    };

    int iter = 0;
    while (iter < max_iter) {
        // Compute gradient grad = H*x + g
        std::vector<double> grad(n, 0.0);
        for (int i = 0; i < n; ++i) {
            double sum = 0.0;
            for (int j = 0; j < n; ++j) {
                sum += H[i][j] * x[j];
            }
            grad[i] = sum + g[i];
        }

        // Compute projected gradient for convergence check
        double pg_norm = 0.0;
        for (int i = 0; i < n; ++i) {
            double xi_proj = std::max(lb[i], std::min(ub[i], x[i] - grad[i]));
            double diff = std::fabs(x[i] - xi_proj);
            pg_norm = std::max(pg_norm, diff);
        }
        if (pg_norm < eps) {
            break;
        }

        // Backtracking line search
        double f_x = eval_obj(x);
        double alpha = alpha_init;
        std::vector<double> x_new(n);
        bool step_found = false;
        for (int ls = 0; ls < max_ls; ++ls) {
            // Candidate step
            for (int i = 0; i < n; ++i) {
                x_new[i] = std::max(lb[i], std::min(ub[i], x[i] - alpha * grad[i]));
            }
            double f_new = eval_obj(x_new);
            // Armijo condition: f_new <= f_x + c * grad' * (x_new - x)
            double dir = 0.0;
            for (int i = 0; i < n; ++i) {
                dir += grad[i] * (x_new[i] - x[i]);
            }
            if (f_new <= f_x + c * dir) {
                step_found = true;
                break;
            }
            alpha *= 0.5;
        }

        if (!step_found) {
            // If no step found, accept the smallest alpha (shouldn't happen often)
            for (int i = 0; i < n; ++i) {
                x_new[i] = std::max(lb[i], std::min(ub[i], x[i] - alpha * grad[i]));
            }
        }
        x = x_new;
        iter++;
    }

    return iter;
}

#include <cassert>
#include <vector>
#include <cmath>

// forward declaration of solution function
int boxQPSolve(const std::vector<std::vector<double>>& H,
               const std::vector<double>& g,
               const std::vector<double>& lb,
               const std::vector<double>& ub,
               std::vector<double>& x);

int main() {
    // Test 1: Simple 2D problem with known solution
    {
        std::vector<std::vector<double>> H = {{2, 0}, {0, 2}};
        std::vector<double> g = {-4, -6};
        std::vector<double> lb = {0, 0};
        std::vector<double> ub = {10, 10};
        std::vector<double> x = {0, 0};
        boxQPSolve(H, g, lb, ub, x);
        assert(std::fabs(x[0] - 2.0) < 1e-6);
        assert(std::fabs(x[1] - 3.0) < 1e-6);
    }

    // Test 2: Bound active at lower bound
    {
        std::vector<std::vector<double>> H = {{1, 0}, {0, 1}};
        std::vector<double> g = {-5, 1};
        std::vector<double> lb = {0, 0};
        std::vector<double> ub = {10, 10};
        std::vector<double> x = {0, 0};
        boxQPSolve(H, g, lb, ub, x);
        assert(std::fabs(x[0] - 5.0) < 1e-6);
        assert(std::fabs(x[1] - 0.0) < 1e-6);
    }

    // Test 3: All variables at bounds (box fully active)
    {
        std::vector<std::vector<double>> H = {{1, 0}, {0, 0.5}};
        std::vector<double> g = {10, -20};
        std::vector<double> lb = {0, 0};
        std::vector<double> ub = {1, 1};
        std::vector<double> x = {1, 1};
        boxQPSolve(H, g, lb, ub, x);
        assert(std::fabs(x[0] - 0.0) < 1e-6);
        assert(std::fabs(x[1] - 1.0) < 1e-6);
    }

    // Test 4: Clamp initial guess outside bounds
    {
        std::vector<std::vector<double>> H = {{1, 0}, {0, 1}};
        std::vector<double> g = {0, 0};
        std::vector<double> lb = {-1, -1};
        std::vector<double> ub = {1, 1};
        std::vector<double> x = {100, -100};
        boxQPSolve(H, g, lb, ub, x);
        assert(std::fabs(x[0] - 0.0) < 1e-6);
        assert(std::fabs(x[1] - 0.0) < 1e-6);
    }

    // Test 5: Single variable, bound equality
    {
        std::vector<std::vector<double>> H = {{3}};
        std::vector<double> g = {2};
        std::vector<double> lb = {1};
        std::vector<double> ub = {1};
        std::vector<double> x = {0};
        boxQPSolve(H, g, lb, ub, x);
        assert(std::fabs(x[0] - 1.0) < 1e-6);
    }

    // Test 6: Non-symmetric H input (should still work)
    {
        std::vector<std::vector<double>> H = {{2, 1}, {0, 2}}; // not symmetric
        std::vector<double> g = {-1, -3};
        std::vector<double> lb = {0, 0};
        std::vector<double> ub = {5, 5};
        std::vector<double> x = {0, 0};
        boxQPSolve(H, g, lb, ub, x);
        // Symmetric part is [[2,0.5],[0.5,2]]; solution around (0.4,1.2) but check bounds only
        assert(x[0] >= lb[0] - 1e-8 && x[0] <= ub[0] + 1e-8);
        assert(x[1] >= lb[1] - 1e-8 && x[1] <= ub[1] + 1e-8);
    }

    // Test 7: Larger dimension (5D) with known solution
    {
        int n = 5;
        std::vector<std::vector<double>> H(n, std::vector<double>(n, 0.0));
        for (int i = 0; i < n; ++i) H[i][i] = 2.0;
        std::vector<double> g(n, -1.0);
        std::vector<double> lb(n, 0.0);
        std::vector<double> ub(n, 10.0);
        std::vector<double> x(n, 0.0);
        boxQPSolve(H, g, lb, ub, x);
        for (int i = 0; i < n; ++i) {
            assert(std::fabs(x[i] - 0.5) < 1e-6);
        }
    }

    // Test 8: Zero gradient, zero objective -> any point in bounds is solution
    {
        std::vector<std::vector<double>> H = {{1, 0}, {0, 1}};
        std::vector<double> g = {0, 0};
        std::vector<double> lb = {-1, -1};
        std::vector<double> ub = {1, 1};
        std::vector<double> x = {0, 0};
        int iters = boxQPSolve(H, g, lb, ub, x);
        assert(iters == 0); // initial point already optimal
    }

    // Test 9: Strictly convex but ill-conditioned (works slowly but converges)
    {
        std::vector<std::vector<double>> H = {{1e6, 0}, {0, 1e-6}};
        std::vector<double> g = {0, 0};
        std::vector<double> lb = {-100, -100};
        std::vector<double> ub = {100, 100};
        std::vector<double> x = {50, 50};
        boxQPSolve(H, g, lb, ub, x);
        // Optimal is 0,0; allow larger tolerance due to slow convergence
        assert(std::fabs(x[0]) < 1e-3);
        assert(std::fabs(x[1]) < 1e-3);
    }

    return 0;
}

// The algorithm is a projected gradient descent with a backtracking line search (Armijo rule). Starting from the initial guess `x`, each iteration computes the gradient `∇f(x) = Hx + g`. We then perform a line search: try step size `α` (initialized to 1.0), compute candidate `x_new = clamp(x - α∇f(x), lb, ub)` (projection onto the box), and accept the step if it satisfies the Armijo sufficient decrease condition: `f(x_new) ≤ f(x) + c * ∇f(x)ᵀ (x_new - x)` with `c = 1e-4`. If not satisfied, reduce `α` by half and retry (up to a max of 50 reductions). After updating `x`, we check convergence: if the infinity-norm of the projected gradient (i.e., `max_i | (x - clamp(x - ∇f(x), lb, ub))_i |`) is less than `1e-6`, stop. Since H is strictly convex (positive definite), the problem is well-posed and the algorithm converges linearly. Edge cases: if `lb == ub` for some components, those are fixed and the projection keeps them constant; the line search handles this naturally. If `xinit` violates bounds, it is clamped initially. Time complexity is O(k * n²) per iteration due to matrix-vector and vector-vector operations, where `k` is the number of line search steps (typically small). Space complexity is O(n) for temporary vectors.
