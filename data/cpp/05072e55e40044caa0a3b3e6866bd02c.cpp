/*
Implement a standalone C++ function that solves a nonlinear unconstrained optimization problem using a trust-region method with a truncated conjugate gradient (CG) inner solver. The function must accept a generic objective function object (via an abstract base class with `fun`, `grad`, and `Hv` methods), an initial weight vector, a tolerance (default `1e-6`), and a maximum iteration count (default `1000`). It must return the final weight vector that approximately minimizes the objective, updating the input vector in-place. The implementation must be self-contained (no external BLAS/LAPACK dependence; implement all required vector operations manually) and must handle the trust-region radius update, CG termination at the trust-region boundary, and convergence criteria as described in the reference snippet. The objective function object must support evaluation of the function value, gradient, and Hessian-vector product (Hessian not explicitly formed).
*/
#include <cmath>
#include <cstring>
#include <algorithm>
#include <vector>
#include <stdexcept>

// Abstract base class for objective functions.
class ObjectiveFunction {
public:
    virtual ~ObjectiveFunction() {}
    virtual int get_nr_variable() const = 0;           // Number of variables
    virtual double fun(const double* w) const = 0;     // Function value at w
    virtual void grad(const double* w, double* g) const = 0; // Gradient
    virtual void Hv(const double* d, double* Hd) const = 0;  // Hessian-vector product
};

namespace detail {
    inline double dot(int n, const double* x, const double* y) {
        double sum = 0.0;
        for (int i = 0; i < n; ++i) sum += x[i] * y[i];
        return sum;
    }

    inline double norm(int n, const double* x) {
        return std::sqrt(dot(n, x, x));
    }

    inline void axpy(int n, double a, const double* x, double* y) {
        for (int i = 0; i < n; ++i) y[i] += a * x[i];
    }

    inline void copy(int n, const double* src, double* dst) {
        for (int i = 0; i < n; ++i) dst[i] = src[i];
    }
}

// Trust-region optimization using truncated CG.
// Writes solution to w (in-place). Returns number of outer iterations.
int tron_optimize(ObjectiveFunction& obj, double* w, double eps = 1e-6, int max_iter = 1000) {
    const int n = obj.get_nr_variable();

    // Parameters from the reference snippet.
    const double eta0 = 1e-4, eta1 = 0.25, eta2 = 0.75;
    const double sigma1 = 0.25, sigma2 = 0.5, sigma3 = 4;

    // Working arrays.
    std::vector<double> s(n), r(n), w_new(n), g(n);
    std::vector<double> d(n), Hd(n);

    // Initialize all weights to zero (as per reference).
    for (int i = 0; i < n; ++i) w[i] = 0.0;

    double f = obj.fun(w);
    obj.grad(w, g.data());

    double delta = detail::norm(n, g.data());
    double gnorm1 = delta;
    double gnorm = gnorm1;

    int search = 1;
    if (gnorm <= eps * gnorm1) search = 0;

    int iter = 1;
    while (iter <= max_iter && search) {
        // --- Truncated CG to solve subproblem ---
        double cgtol = 0.1 * gnorm;
        for (int i = 0; i < n; ++i) {
            s[i] = 0.0;
            r[i] = -g[i];
            d[i] = r[i];
        }
        double rTr = detail::dot(n, r.data(), r.data());
        int cg_iter = 0;

        while (true) {
            if (detail::norm(n, r.data()) <= cgtol) break;
            cg_iter++;

            obj.Hv(d.data(), Hd.data());
            double dHd = detail::dot(n, d.data(), Hd.data());
            if (std::fabs(dHd) < 1e-30) break; // avoid division by zero

            double alpha = rTr / dHd;
            detail::axpy(n, alpha, d.data(), s.data());

            if (detail::norm(n, s.data()) > delta) {
                // Reached trust-region boundary; project step onto boundary.
                // Undo previous step
                detail::axpy(n, -alpha, d.data(), s.data());

                double std_ = detail::dot(n, s.data(), d.data());
                double sts = detail::dot(n, s.data(), s.data());
                double dtd = detail::dot(n, d.data(), d.data());
                double dsq = delta * delta;
                double rad = std::sqrt(std_ * std_ + dtd * (dsq - sts));

                double step_len;
                if (std_ >= 0)
                    step_len = (dsq - sts) / (std_ + rad);
                else
                    step_len = (rad - std_) / dtd;

                detail::axpy(n, step_len, d.data(), s.data());
                detail::axpy(n, -step_len, Hd.data(), r.data());
                break;
            }

            // Update residual and direction.
            detail::axpy(n, -alpha, Hd.data(), r.data());
            double rnewTrnew = detail::dot(n, r.data(), r.data());
            double beta = rnewTrnew / rTr;
            for (int i = 0; i < n; ++i) d[i] = r[i] + beta * d[i];
            rTr = rnewTrnew;
        }

        // --- Evaluate candidate point w_new = w + s ---
        detail::copy(n, w, w_new.data());
        detail::axpy(n, 1.0, s.data(), w_new.data());

        double gs = detail::dot(n, g.data(), s.data());
        double prered = -0.5 * (gs - detail::dot(n, s.data(), r.data()));
        double fnew = obj.fun(w_new.data());

        double actred = f - fnew;

        // Adjust initial step bound on first iteration.
        double snorm = detail::norm(n, s.data());
        if (iter == 1)
            delta = std::min(delta, snorm);

        // Determine alpha for trust-region update.
        double alpha;
        if (fnew - f - gs <= 0.0)
            alpha = sigma3;
        else
            alpha = std::max(sigma1, -0.5 * (gs / (fnew - f - gs)));

        // Update trust-region radius.
        if (actred < eta0 * prered)
            delta = std::min(std::max(alpha, sigma1) * snorm, sigma2 * delta);
        else if (actred < eta1 * prered)
            delta = std::max(sigma1 * delta, std::min(alpha * snorm, sigma2 * delta));
        else if (actred < eta2 * prered)
            delta = std::max(sigma1 * delta, std::min(alpha * snorm, sigma3 * delta));
        else
            delta = std::max(delta, std::min(alpha * snorm, sigma3 * delta));

        // Accept step if sufficient reduction.
        if (actred > eta0 * prered) {
            iter++;
            detail::copy(n, w_new.data(), w);
            f = fnew;
            obj.grad(w, g.data());

            gnorm = detail::norm(n, g.data());
            if (gnorm <= eps * gnorm1) break;
        }

        // Safety checks
        if (f < -1.0e+32) break;
        if (std::fabs(actred) <= 0.0 && prered <= 0.0) break;
        if (std::fabs(actred) <= 1.0e-12 * std::fabs(f) &&
            std::fabs(prered) <= 1.0e-12 * std::fabs(f)) break;
    }

    return iter;
}
#include <cassert>
#include <cmath>

// Example 1: Simple quadratic f(w) = 0.5 * (w1^2 + 2*w2^2)
// Gradient = [w1, 2*w2], Hessian = diag(1,2)
class Quadratic : public ObjectiveFunction {
public:
    int get_nr_variable() const override { return 2; }
    double fun(const double* w) const override {
        return 0.5 * (w[0]*w[0] + 2.0*w[1]*w[1]);
    }
    void grad(const double* w, double* g) const override {
        g[0] = w[0];
        g[1] = 2.0 * w[1];
    }
    void Hv(const double* d, double* Hd) const override {
        Hd[0] = d[0];
        Hd[1] = 2.0 * d[1];
    }
};

// Example 2: Rosenbrock-like but still simple enough
// f(w) = (1 - w0)^2 + 100*(w1 - w0^2)^2
// Gradient and Hessian-vector product computed directly.
class Rosenbrock : public ObjectiveFunction {
public:
    int get_nr_variable() const override { return 2; }
    double fun(const double* w) const override {
        double a = 1.0 - w[0];
        double b = w[1] - w[0]*w[0];
        return a*a + 100.0 * b*b;
    }
    void grad(const double* w, double* g) const override {
        g[0] = -2.0*(1.0 - w[0]) - 400.0*w[0]*(w[1] - w[0]*w[0]);
        g[1] = 200.0*(w[1] - w[0]*w[0]);
    }
    void Hv(const double* d, double* Hd) const override {
        double x = w0, y = w1;
        // Hessian: [[2 - 400*(y - 3x^2), -400*x],
        //           [-400*x, 200]]
        double h00 = 2.0 - 400.0*(y - 3.0*x*x);
        double h01 = -400.0 * x;
        double h11 = 200.0;
        Hd[0] = h00 * d[0] + h01 * d[1];
        Hd[1] = h01 * d[0] + h11 * d[1];
    }
private:
    // Store latest w for Hessian-vector product (simplified)
    mutable double w0, w1;
public:
    Rosenbrock() : w0(0), w1(0) {}
    void grad(const double* w, double* g) const override {
        w0 = w[0]; w1 = w[1];
        g[0] = -2.0*(1.0 - w[0]) - 400.0*w[0]*(w[1] - w[0]*w[0]);
        g[1] = 200.0*(w[1] - w[0]*w[0]);
    }
    void fun(const double* w) const override {
        w0 = w[0]; w1 = w[1];
        double a = 1.0 - w[0];
        double b = w[1] - w[0]*w[0];
        return a*a + 100.0 * b*b;
    }
    void Hv(const double* d, double* Hd) const override {
        double h00 = 2.0 - 400.0*(w1 - 3.0*w0*w0);
        double h01 = -400.0 * w0;
        double h11 = 200.0;
        Hd[0] = h00 * d[0] + h01 * d[1];
        Hd[1] = h01 * d[0] + h11 * d[1];
    }
};

int main() {
    // Test 1: Quadratic with exact zero minimum
    Quadratic q;
    double w1[2] = {10.0, -5.0}; // but function will reset to zeros
    int it1 = tron_optimize(q, w1, 1e-8, 100);
    assert(std::fabs(w1[0]) < 1e-6);
    assert(std::fabs(w1[1]) < 1e-6);
    assert(it1 > 0);

    // Test 2: Quadratic with known solution (same as above but starting from zeros)
    double w2[2] = {0.0, 0.0};
    int it2 = tron_optimize(q, w2, 1e-8, 100);
    assert(std::fabs(w2[0]) < 1e-10);
    assert(std::fabs(w2[1]) < 1e-10);
    assert(it2 == 1); // gradient at zero is zero, so one iteration

    // Test 3: Rosenbrock (minimum at [1,1])
    Rosenbrock rb;
    double w3[2] = {0.0, 0.0};
    int it3 = tron_optimize(rb, w3, 1e-6, 1000);
    assert(std::fabs(w3[0] - 1.0) < 1e-3);
    assert(std::fabs(w3[1] - 1.0) < 1e-3);
    assert(it3 > 0);

    // Test 4: Convergence when already at minimum
    Quadratic q2;
    double w4[2] = {0.0, 0.0};
    int it4 = tron_optimize(q2, w4, 1e-10, 100);
    assert(it4 == 1);
    assert(w4[0] == 0.0 && w4[1] == 0.0);

    // Test 5: Larger quadratic with unknown solution but verify gradient norm small
    // Use a 3D quadratic: f = w0^2 + 2*w1^2 + 3*w2^2
    // Hessian diag(2,4,6)
    struct Q3 : ObjectiveFunction {
        int get_nr_variable() const override { return 3; }
        double fun(const double* w) const override {
            return w[0]*w[0] + 2.0*w[1]*w[1] + 3.0*w[2]*w[2];
        }
        void grad(const double* w, double* g) const override {
            g[0] = 2.0*w[0]; g[1] = 4.0*w[1]; g[2] = 6.0*w[2];
        }
        void Hv(const double* d, double* Hd) const override {
            Hd[0] = 2.0*d[0]; Hd[1] = 4.0*d[1]; Hd[2] = 6.0*d[2];
        }
    };
    Q3 q3;
    double w5[3] = {5.0, -3.0, 2.0};
    int it5 = tron_optimize(q3, w5, 1e-8, 100);
    assert(std::fabs(w5[0]) < 1e-6);
    assert(std::fabs(w5[1]) < 1e-6);
    assert(std::fabs(w5[2]) < 1e-6);
    assert(it5 > 0);

    // Test 6: Check that the function returns after max iterations if not converged
    // Use a function with poor conditioning but still converges slowly
    // Not strictly necessary but ensures no infinite loop.
    Rosenbrock rb2;
    double w6[2] = {0.0, 0.0};
    int it6 = tron_optimize(rb2, w6, 1e-12, 5); // too few iterations
    assert(it6 <= 5);

    return 0;
}
// The core algorithm is a trust-region method: at each outer iteration, given current point `w`, gradient `g`, and trust-region radius `delta`, we solve a constrained quadratic subproblem approximately using the truncated CG method (inner iterations). The subproblem minimizes `0.5 * s^T H s + g^T s` subject to `||s|| <= delta`. CG is stopped early if the residual norm drops below `0.1 * ||g||` or if the step reaches the trust-region boundary. After obtaining candidate step `s`, we compute the actual reduction `actred = f(w) - f(w+s)` and predicted reduction `prered = -0.5*(g^T s + s^T r)` where `r = H s` is the final CG residual (which may differ from `-g` if CG stopped early). The ratio `actred/prered` dictates whether to accept the step and how to update `delta` (shrink if ratio low, expand if high). Accepted steps update `w` and recompute gradient; convergence is declared when `||g|| <= eps * ||g_initial||` (since initial `w` is zeros, `g_initial` is the gradient at zero). Additional termination breaks occur if function value goes below `-1e32` or reductions are negligible. Time complexity: each outer iteration costs `O(n)` for vector ops plus `O(n)` per CG inner iteration (each requires one Hessian-vector product). In worst case, `cg_iter` can be `O(n)` per outer iteration, so total `O(n * max_iter * n)` = `O(max_iter * n^2)` for dense Hessian-vector products. Space is `O(n)` for working vectors (s, r, d, Hd, w_new, g). Edge cases: if initial gradient norm is zero (or below tolerance), return immediately; if CG denominator `d^T H d` is zero (should not happen for positive definite but could for indefinite), need to handle by breaking with current residual; if `prered` is zero, avoid division by zero when updating delta.
