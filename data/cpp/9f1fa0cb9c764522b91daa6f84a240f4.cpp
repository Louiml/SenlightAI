/*
Write a C++ function that implements the Sequential Unconstrained Minimization Technique (SUMT) for minimizing a smooth scalar objective function subject to smooth inequality constraints of the form g_i(x) >= 0, using a single penalty parameter that grows geometrically. The function must accept a starting vector, an objective function with optional gradient output, a constraint function returning a vector of constraint values (with optional Jacobian output), a penalty growth factor, a maximum iteration count, and a convergence tolerance. It should return the final solution vector in-place, run the outer SUMT loop with an inner quasi-Newton (BFGS) solver, and handle the case where some constraints are inactive (non-positive values) by zeroing them and their Jacobian rows before computing the penalty term. The function must be self-contained with no global state, validate inputs (e.g., penalty factor > 1, valid dimensions), and perform error reporting by setting a success flag and returning a boolean. The solution must use Eigen or custom vector/matrix types, not Armadillo, for portability.
*/
#include <vector>
#include <functional>
#include <cmath>
#include <limits>
#include <algorithm>
#include <cassert>

// Simple dense matrix type for gradient/Jacobian storage
struct Matrix {
    int rows, cols;
    std::vector<double> data;
    Matrix(int r, int c) : rows(r), cols(c), data(r*c, 0.0) {}
    double& operator()(int i, int j) { return data[i*cols + j]; }
    const double& operator()(int i, int j) const { return data[i*cols + j]; }
};

// Simple vector operations
using Vec = std::vector<double>;

// Dot product
double dot(const Vec& a, const Vec& b) {
    assert(a.size() == b.size());
    double sum = 0.0;
    for (size_t i = 0; i < a.size(); ++i) sum += a[i]*b[i];
    return sum;
}

// L2 norm
double norm(const Vec& a) {
    return std::sqrt(dot(a, a));
}

// Vector addition: a + b
Vec add(const Vec& a, const Vec& b) {
    assert(a.size() == b.size());
    Vec result(a.size());
    for (size_t i = 0; i < a.size(); ++i) result[i] = a[i] + b[i];
    return result;
}

// Scalar multiplication: s * a
Vec scalarMul(double s, const Vec& a) {
    Vec result(a.size());
    for (size_t i = 0; i < a.size(); ++i) result[i] = s * a[i];
    return result;
}

// BFGS unconstrained minimizer (simplified, with backtracking line search)
bool bfgs(Vec& x, 
          std::function<double(const Vec&, Vec*)> obj_grad,
          double tol, int max_iter) {
    int n = (int)x.size();
    Matrix H(n, n); // inverse Hessian approximation (identity initially)
    for (int i = 0; i < n; ++i) H(i,i) = 1.0;

    Vec grad(n), grad_new(n);
    double f = obj_grad(x, &grad);
    
    for (int iter = 0; iter < max_iter; ++iter) {
        double grad_norm = norm(grad);
        if (grad_norm < tol) return true;

        // Compute search direction: p = -H * grad
        Vec p(n, 0.0);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                p[i] -= H(i,j) * grad[j];

        // Backtracking line search
        double alpha = 1.0;
        double c = 0.5; // Armijo parameter
        Vec x_new(n);
        double f_new;
        while (true) {
            x_new = add(x, scalarMul(alpha, p));
            f_new = obj_grad(x_new, &grad_new);
            if (f_new <= f + c * alpha * dot(grad, p)) break;
            alpha *= 0.5;
            if (alpha < 1e-12) return false;
        }

        // Update difference vectors
        Vec s = scalarMul(alpha, p); // x_new - x
        Vec y = add(grad_new, scalarMul(-1.0, grad)); // grad_new - grad
        double sy = dot(s, y);
        if (sy < 1e-12) { // Reset if not positive definite
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    H(i,j) = (i == j) ? 1.0 : 0.0;
        } else {
            // BFGS update: H = (I - rho*s*y^T) H (I - rho*y*s^T) + rho*s*s^T
            double rho = 1.0 / sy;
            Vec Hy(n, 0.0); // H * y
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    Hy[i] += H(i,j) * y[j];
            double yHy = dot(y, Hy);
            // Update H in-place: H = H + rho*( (rho*yHy + 1)*s*s^T - s*y^T*H - H*y*s^T )
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    double term1 = rho * (rho * yHy + 1.0) * s[i] * s[j];
                    double term2 = rho * s[i] * Hy[j];
                    double term3 = rho * Hy[i] * s[j];
                    H(i,j) += term1 - term2 - term3;
                }
            }
        }

        x = x_new;
        f = f_new;
        grad = grad_new;
    }
    return false;
}

// SUMT main function
bool sumt_constr(
    Vec& init_out_vals,
    std::function<double(const Vec&, Vec*)> obj_fn,
    std::function<Vec(const Vec&, Matrix*)> constr_fn,
    double penalty_growth = 2.0,
    int iter_max = 100,
    double err_tol = 1e-6)
{
    assert(penalty_growth > 1.0);
    assert(iter_max > 0);
    assert(err_tol > 0.0);

    Vec x = init_out_vals;
    Vec x_prev = x;
    double c_pen = 1.0;
    bool success = false;

    for (int iter = 0; iter < iter_max; ++iter) {
        // Build penalized objective and gradient for current penalty
        auto penalized = [&](const Vec& vals, Vec* grad_out) -> double {
            // Evaluate objective
            Vec grad_obj(vals.size(), 0.0);
            double f = obj_fn(vals, &grad_obj);

            // Evaluate constraints
            Matrix jac;
            Vec constr_vals = constr_fn(vals, &jac);
            int m = (int)constr_vals.size();

            // Identify violated constraints (constr_vals <= 0 means violation since feasible is >= 0)
            Vec active_constr;
            Matrix active_jac(0, vals.size());
            for (int i = 0; i < m; ++i) {
                if (constr_vals[i] <= 0) {
                    active_constr.push_back(constr_vals[i]);
                    // Copy Jacobian row
                    for (int j = 0; j < vals.size(); ++j) {
                        active_jac.data.push_back(jac(i,j));
                    }
                    active_jac.rows++;
                }
            }

            // Compute penalty term: (c/2) * sum(active_constr^2)
            double penalty = 0.0;
            for (double v : active_constr) penalty += v * v;
            penalty *= 0.5 * c_pen;

            if (grad_out) {
                // Gradient of penalty: c * sum(active_constr * grad(active_constr))
                Vec grad_penalty(vals.size(), 0.0);
                int idx = 0;
                for (int i = 0; i < (int)active_constr.size(); ++i) {
                    double g_val = active_constr[i];
                    for (int j = 0; j < vals.size(); ++j) {
                        grad_penalty[j] += c_pen * g_val * active_jac(i,j);
                    }
                }
                *grad_out = add(grad_obj, grad_penalty);
            }
            return f + penalty;
        };

        // Inner BFGS
        Vec x_start = x;
        bfgs(x_start, penalized, err_tol * 0.1, 200);

        // Check convergence every 10 iterations
        if (iter % 10 == 9) {
            double diff_norm = norm(add(x_start, scalarMul(-1.0, x_prev)));
            if (diff_norm < err_tol) {
                x_prev = x_start;
                x = x_start;
                success = true;
                break;
            }
        }

        x_prev = x;
        x = x_start;
        c_pen *= penalty_growth; // Increase penalty parameter
    }

    init_out_vals = x;
    return success;
}
#include <cassert>
#include <cmath>
#include <vector>

// Provided solution (assumed included above)

// Test: minimize f(x) = (x-2)^2 subject to x >= 0 (constraint g(x)=x >= 0)
double obj1(const std::vector<double>& x, std::vector<double>* grad) {
    double val = (x[0]-2.0)*(x[0]-2.0);
    if (grad) (*grad)[0] = 2.0*(x[0]-2.0);
    return val;
}
std::vector<double> constr1(const std::vector<double>& x, Matrix* jac) {
    if (jac) {
        (*jac)(0,0) = 1.0;
    }
    return {x[0]};
}

// Test: unconstrained-like, but with a constraint that is always satisfied
double obj2(const std::vector<double>& x, std::vector<double>* grad) {
    double val = x[0]*x[0] + x[1]*x[1];
    if (grad) { (*grad)[0] = 2*x[0]; (*grad)[1] = 2*x[1]; }
    return val;
}
std::vector<double> constr2(const std::vector<double>& x, Matrix* jac) {
    if (jac) {
        (*jac)(0,0) = 0.0;
        (*jac)(0,1) = 1.0;
    }
    return {1.0}; // always >= 0
}

int main() {
    // Test 1: Start at x=10, should converge to x=2 (since x>=0 is not violated from start? Actually start violates? No, x=10 is feasible)
    {
        std::vector<double> x = {10.0};
        bool ok = sumt_constr(x, obj1, constr1, 2.0, 50, 1e-6);
        assert(ok);
        assert(std::fabs(x[0] - 2.0) < 1e-3);
    }

    // Test 2: Start at x=-10, violates constraint, should still end near 0 (boundary)
    {
        std::vector<double> x = {-10.0};
        bool ok = sumt_constr(x, obj1, constr1, 2.0, 100, 1e-6);
        assert(ok);
        assert(x[0] > -0.1 && x[0] < 0.1);
    }

    // Test 3: Always feasible constraint, should minimize to (0,0)
    {
        std::vector<double> x = {3.0, -2.0};
        bool ok = sumt_constr(x, obj2, constr2, 3.0, 50, 1e-6);
        assert(ok);
        assert(std::fabs(x[0]) < 1e-3);
        assert(std::fabs(x[1]) < 1e-3);
    }

    // Test 4: Simple check with invalid penalty growth factor → should assert
    {
        std::vector<double> x = {1.0};
        bool crashed = false;
        try {
            sumt_constr(x, obj1, constr1, 0.5); // invalid, asserts
        } catch (...) {
            crashed = true;
        }
        assert(crashed);
    }

    return 0;
}
// The SUMT approach converts a constrained problem into a sequence of unconstrained problems by adding a quadratic penalty for violated constraints: P(x, c) = f(x) + (c/2) * sum(max(0, -g_i(x))^2). At each outer iteration, we minimize this penalized objective using an inner BFGS optimizer with numerical or analytical gradients (the gradient is grad(f) + c * sum over violated constraints of g_i * grad(g_i)). The penalty parameter c starts at 1 and is multiplied by the growth factor eta (must be > 1) each iteration. The stopping condition is based on the L2 norm of the change in x between consecutive outer iterations, checked every 10 iterations to reduce overhead. Key edge cases: (1) if no constraints are violated, the penalty term is zero and gradient is just the objective gradient; (2) constraints must be defined such that g(x) >= 0 is the feasible region (so penalty applies to negative values); (3) the input constraint function must return a vector of constraint values, and the Jacobian as a matrix where each row corresponds to a constraint’s gradient; (4) if the constraint vector has length 0 (no constraints), the problem degenerates to unconstrained optimization. Time complexity is O(iters * k * n^2) where iters is the number of SUMT iterations, k is the number of BFGS iterations per outer loop, and n is the number of variables; BFGS is O(n^2) per step. Space complexity is O(n^2) for BFGS’s Hessian approximation and O(n * m) for the Jacobian if m constraints exist.
