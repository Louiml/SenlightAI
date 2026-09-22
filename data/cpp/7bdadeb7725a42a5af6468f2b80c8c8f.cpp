// Implement a C++ function `trustRegionMinimize` that minimizes a convex quadratic function `f(x) = 0.5 * x^T A x - b^T x + c`, where `A` is a symmetric positive definite matrix, `b` is a vector, and `c` is a scalar. The function should take as parameters the matrix `A` (as a `std::vector<std::vector<double>>`), the vector `b` (as `std::vector<double>`), the scalar `c`, an initial trust region radius `delta0` (positive), a convergence tolerance `eps` (positive, e.g., 1e-6), and a maximum number of iterations `maxIter` (int). It must return the approximate minimizer `x` as a `std::vector<double>`. The algorithm should use a trust-region method with a truncated conjugate gradient (CG) inner solver: at each outer iteration, compute the gradient `g = A x - b`, solve the subproblem approximately via CG (with a relative residual tolerance of `0.1 * ||g||` and a step bound `delta`), update the iterate if the reduction ratio `(f(x)-f(x_new))/(predicted reduction)` meets an acceptance threshold (eta0=1e-4), and adjust `delta` based on the ratio (shrink if poor, expand if good). The function should also handle edge cases: if `A` is empty, return an empty vector; if `delta0 <= 0` or `eps <= 0`, return the zero vector; if `maxIter <= 0`, return the zero vector. Ensure numerical stability by avoiding division by zero in CG (if `d^T H d` is very small, break). The function must be self-contained and include only standard headers (no external libraries).

// The solution implements a trust-region optimization method. The main algorithm: 
// 1. Initialize `x` as a zero vector of length `n` (where `n` is the size of `b`). If `n == 0` or invalid parameters, return immediately.
// 2. Compute the objective `f = 0.5 * x^T A x - b^T x + c` and gradient `g = A x - b`.
// 3. Set initial `delta = min(delta0, ||g||)`? Actually standard: `delta = ||g||` or `delta0`; we'll set `delta = min(delta0, ||g||)` to avoid too large initial step.
// 4. Compute `gnorm1 = ||g||`; if `gnorm <= eps*gnorm1`, return `x`.
// 5. Outer loop: for `iter = 1` to `maxIter`:
//    - Solve the trust-region subproblem via truncated CG: find an approximate step `s` that minimizes `m(s) = f + g^T s + 0.5 s^T A s` subject to `||s|| <= delta`. The CG solver is: initialize `r = -g`, `d = r`, `cgtol = 0.1 * ||g||`. Iterate: if `||r|| <= cgtol`, break; compute `Hd = A d`; if `d^T Hd` is too small (e.g., < 1e-12), break; compute `alpha = r^T r / (d^T Hd)`; update `s += alpha*d`; if `||s|| > delta`, find the scalar `alpha_t` to project `s` onto the boundary (`||s|| = delta`) using a root of a quadratic equation, update `s` and `r`, then break. Else update `r -= alpha*Hd`, compute `beta = (r_new^T r_new)/(r^T r)`, update `d = r + beta*d`.
//    - Set `x_new = x + s`, compute `f_new = f(x_new)`, `gs = g^T s`, `prered = -0.5*(gs - s^T r)` (predicted reduction).
//    - Compute `actred = f - f_new`.
//    - If first iteration, set `delta = min(delta, ||s||)`.
//    - Compute `alpha` for delta update: if `f_new - f - gs <= 0`, `alpha = 4.0`; else `alpha = max(0.25, -0.5*gs/(f_new-f-gs))`.
//    - Update `delta` based on `actred` vs `eta0*prered` (eta0=1e-4, eta1=0.25, eta2=0.75, sigma1=0.25, sigma2=0.5, sigma3=4.0): if `actred < eta0*prered`, shrink delta; else if < eta1, keep moderate; else if < eta2, moderate expand; else expand.
//    - If `actred > eta0*prered`, accept step: `x = x_new`, `f = f_new`, update gradient `g = A x - b`, compute `gnorm = ||g||`; if `gnorm <= eps*gnorm1`, break.
//    - Additional stopping: if `f < -1e32` or `actred` and `prered` both too small, break.
// 6. Return `x`.
//
// Edge cases: empty input, non-positive radius/tolerance/maxIter. For numerical stability, if `d^T Hd` is close to zero, break CG. The CG solver must correctly handle the boundary projection: given current `s` and direction `d` with `||s + t*d|| = delta`, we compute `t` solving `||s||^2 + 2t(s·d) + t^2||d||^2 = delta^2`. We take the positive root. Then set `s += t*d`, and update `r = -g - A s` (actually we update `r` accordingly: since we have `r` from CG, but after stepping to boundary, we recompute `r` as `-g - A*s` to be accurate). To keep it simple, after boundary hit, we break and return the current `r` (used for predicted reduction; but we can compute `r` directly from `s` and `g` if needed). In the implementation, we'll just store `r` as `-g - A*s` after boundary projection.
//
// Complexities: Outer iterations at most `maxIter`, each CG iteration at most `n` (since in exact arithmetic CG converges in at most n iterations, but truncated early). Each matrix-vector product `A*d` is O(n^2) for dense matrix, and dot products are O(n). Thus overall worst-case O(maxIter * n^2 * n) = O(maxIter * n^3). For typical small n, fine. Space O(n^2) for storing A plus O(n) for vectors.
//
// Reference solution code: we'll implement `std::vector<double> trustRegionMinimize(const std::vector<std::vector<double>>& A, const std::vector<double>& b, double c, double delta0, double eps, int maxIter)`. We'll include `<vector>`, `<cmath>`, `<algorithm>`, `<stdexcept>` maybe. We'll write helper functions for dot product, norm, matrix-vector multiply, and objective evaluation.
//
// We'll output only the solution function code (no main). Then in the Test section, we'll have a main with asserts.
//
// Let's write the solution.
//
// For the test, we'll use a simple quadratic: A = [[2,0],[0,2]], b = [0,0], c=0. The minimizer is x=[0,0]. But initial x=0, gradient=0, so it returns immediately. To test non-trivial, set b = [2,0], then gradient at x=0 is [-2,0], minimizer x = [1,0] (since A*x = b => x = [1,0]). We'll verify that the returned x is close to [1,0] within tolerance. Another test: A = [[4,1],[1,3]], b=[5,2], exact solution solves [4,1;1,3]x=[5,2] => x=[1,1]. Use small tolerance for direct comparison like `fabs` < 1e-4.
//
// We'll provide multiple asserts with varying sizes and cases.
//
// Let's craft the solution carefully.
//
// Implementation details:
//
// - Dot product: `double dot(const std::vector<double>& a, const std::vector<double>& b)`.
// - Norm: `double norm(const std::vector<double>& a)`.
// - MatVec: `std::vector<double> matVec(const std::vector<std::vector<double>>& A, const std::vector<double>& x)`.
// - Objective: `double obj(const std::vector<std::vector<double>>& A, const std::vector<double>& b, double c, const std::vector<double>& x)`.
//
// Inside trustRegionMinimize, we need to handle the CG solver. We'll implement it as a lambda inside the function to capture A, or as a separate helper function that takes A, g, delta, and returns s and r. For clarity, we'll define a private helper function `trcg` but since we are outputting only a free function, we can implement it as a lambda or as a nested function (not standard). To keep it self-contained, we'll write the CG loop inline within the main function, using a lambda? But lambdas can't be recursive easily. Better to write a static helper function inside the same file, but the task says "descriptively named free function" only. We can include additional helper functions in the solution code, but they must be non-static and in the global scope. The solution code can have multiple functions; the task says "free function that matches the task specification" - we can have helper functions. So we'll write `trustRegionMinimize` and helper `truncatedCG` (also free function). The `truncatedCG` takes A, g, delta, returns s and r via reference parameters. We'll make it `static`? Better not, but it's fine.
//
// Let's write the code.
//
// We must be careful with the CG boundary projection: We have current `s`, direction `d`. We find `alpha` such that `s_new = s + alpha*d` lies on boundary. Solve `||s + alpha*d||^2 = delta^2`. That's `alpha^2 * dtd + 2*alpha*std + sts - delta^2 = 0`. Use quadratic formula: `alpha = (-2*std + sqrt((2*std)^2 - 4*dtd*(sts-delta^2)))/(2*dtd)`. Simplify: `disc = std*std + dtd*(delta^2 - sts)`. If disc < 0, set alpha = -std/dtd or something (shouldn't happen). Take positive root. Then update s and break.
//
// We'll also compute `r` after boundary as `-g - A*s` for accuracy, because CG might have accumulated error.
//
// Now, we need to ensure that in the CG loop, we don't divide by zero: if `dtd` is very small (<1e-12), break.
//
// Also for the first iteration delta adjustment: In the snippet, they do `if (iter == 1) delta = min(delta, snorm)`. We'll follow that.
//
// Now we'll write the solution code.

#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>

// Helper: dot product
static double dot(const std::vector<double>& a, const std::vector<double>& b) {
    double sum = 0.0;
    for (size_t i = 0; i < a.size(); ++i) sum += a[i] * b[i];
    return sum;
}

// Helper: Euclidean norm
static double norm(const std::vector<double>& a) {
    return std::sqrt(dot(a, a));
}

// Helper: matrix-vector product A*x
static std::vector<double> matVec(const std::vector<std::vector<double>>& A, const std::vector<double>& x) {
    int n = (int)A.size();
    std::vector<double> y(n, 0.0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            y[i] += A[i][j] * x[j];
        }
    }
    return y;
}

// Helper: evaluate objective f(x) = 0.5*x^T A x - b^T x + c
static double objective(const std::vector<std::vector<double>>& A, const std::vector<double>& b, double c, const std::vector<double>& x) {
    std::vector<double> Ax = matVec(A, x);
    return 0.5 * dot(x, Ax) - dot(b, x) + c;
}

// Helper: truncated conjugate gradient for trust region subproblem
// Solves: minimize m(s) = g^T s + 0.5 s^T A s subject to ||s|| <= delta
// On output, s is the step, r = -g - A*s (residual of the subproblem).
static void truncatedCG(const std::vector<std::vector<double>>& A, const std::vector<double>& g, double delta,
                        std::vector<double>& s, std::vector<double>& r) {
    int n = (int)g.size();
    s.assign(n, 0.0);
    r.assign(n, 0.0);
    std::vector<double> d(n, 0.0);
    for (int i = 0; i < n; ++i) {
        r[i] = -g[i];
        d[i] = r[i];
    }
    double cgtol = 0.1 * norm(g);
    double rTr = dot(r, r);
    std::vector<double> Hd(n, 0.0);

    int maxInner = n; // CG converges in at most n steps in exact arithmetic
    for (int cgIter = 0; cgIter < maxInner; ++cgIter) {
        if (norm(r) <= cgtol) break;
        Hd = matVec(A, d);
        double dtd = dot(d, Hd);
        if (std::fabs(dtd) < 1e-12) break; // avoid division by zero
        double alpha = rTr / dtd;
        // Attempt full step s + alpha*d
        for (int i = 0; i < n; ++i) s[i] += alpha * d[i];
        double snorm = norm(s);
        if (snorm > delta) {
            // Find step length to boundary: ||s + t*d|| = delta, but we already added alpha*d,
            // so we project back: we have s = s_old + alpha*d. Need to find t such that ||s_old + t*d|| = delta.
            // Simpler: compute from current s (which is beyond boundary) and d, find t to scale back.
            // We'll recompute using quadratic from origin: we need s_new such that ||s_new|| = delta and s_new is on same ray as old s and d.
            // The standard way: compute s_old = s - alpha*d (undo), then find t.
            for (int i = 0; i < n; ++i) s[i] -= alpha * d[i]; // undo
            double std = dot(s, d);
            double sts = dot(s, s);
            double dtd2 = dot(d, d);
            double disc = std * std + dtd2 * (delta * delta - sts);
            if (disc < 0) disc = 0;
            double rad = std::sqrt(disc);
            double t;
            if (std >= 0) {
                t = (delta * delta - sts) / (std + rad);
            } else {
                t = (rad - std) / dtd2;
            }
            for (int i = 0; i < n; ++i) s[i] += t * d[i];
            // Update residual accordingly: r = -g - A*s
            std::vector<double> As = matVec(A, s);
            for (int i = 0; i < n; ++i) r[i] = -g[i] - As[i];
            break;
        }
        // else accept step, update residual
        for (int i = 0; i < n; ++i) r[i] -= alpha * Hd[i];
        double rnewTrnew = dot(r, r);
        if (rnewTrnew < 1e-30) break;
        double beta = rnewTrnew / rTr;
        for (int i = 0; i < n; ++i) d[i] = r[i] + beta * d[i];
        rTr = rnewTrnew;
    }
}

// Main function: minimize convex quadratic via trust-region method with truncated CG.
// Returns approximate minimizer x.
std::vector<double> trustRegionMinimize(const std::vector<std::vector<double>>& A,
                                        const std::vector<double>& b,
                                        double c,
                                        double delta0,
                                        double eps,
                                        int maxIter) {
    int n = (int)b.size();
    if (n == 0 || delta0 <= 0.0 || eps <= 0.0 || maxIter <= 0) {
        return std::vector<double>(n, 0.0);
    }
    // Check A is square and of size n
    if ((int)A.size() != n) {
        // Invalid: return zero vector
        return std::vector<double>(n, 0.0);
    }
    for (int i = 0; i < n; ++i) {
        if ((int)A[i].size() != n) {
            return std::vector<double>(n, 0.0);
        }
    }

    // Constants for trust region update (same as in the snippet)
    const double eta0 = 1e-4, eta1 = 0.25, eta2 = 0.75;
    const double sigma1 = 0.25, sigma2 = 0.5, sigma3 = 4.0;

    std::vector<double> x(n, 0.0);
    double f = objective(A, b, c, x);
    std::vector<double> g = matVec(A, x);
    for (int i = 0; i < n; ++i) g[i] -= b[i]; // gradient = A x - b

    double delta = std::min(delta0, norm(g));
    double gnorm1 = norm(g);
    double gnorm = gnorm1;

    if (gnorm <= eps * gnorm1) {
        return x; // already at minimum
    }

    std::vector<double> s(n), r(n), x_new(n), g_new(n);
    int iter = 1;
    while (iter <= maxIter) {
        // Solve trust region subproblem via truncated CG
        truncatedCG(A, g, delta, s, r);

        // Candidate new point
        for (int i = 0; i < n; ++i) x_new[i] = x[i] + s[i];
        double f_new = objective(A, b, c, x_new);

        double gs = dot(g, s);
        double prered = -0.5 * (gs - dot(s, r));
        double actred = f - f_new;

        double snorm = norm(s);
        if (iter == 1) {
            delta = std::min(delta, snorm);
        }

        // Compute alpha for delta update
        double alpha;
        if (f_new - f - gs <= 0) {
            alpha = sigma3;
        } else {
            alpha = std::max(sigma1, -0.5 * gs / (f_new - f - gs));
        }

        // Update trust region radius
        if (actred < eta0 * prered) {
            delta = std::min(std::max(alpha, sigma1) * snorm, sigma2 * delta);
        } else if (actred < eta1 * prered) {
            delta = std::max(sigma1 * delta, std::min(alpha * snorm, sigma2 * delta));
        } else if (actred < eta2 * prered) {
            delta = std::max(sigma1 * delta, std::min(alpha * snorm, sigma3 * delta));
        } else {
            delta = std::max(delta, std::min(alpha * snorm, sigma3 * delta));
        }

        // Accept step if sufficient reduction
        if (actred > eta0 * prered) {
            iter++;
            x = x_new;
            f = f_new;
            g = matVec(A, x);
            for (int i = 0; i < n; ++i) g[i] -= b[i];
            gnorm = norm(g);
            if (gnorm <= eps * gnorm1) break;
        }

        // Additional stopping criteria (mirroring the snippet)
        if (f < -1.0e+32) break;
        if (std::fabs(actred) <= 0 && prered <= 0) break;
        if (std::fabs(actred) <= 1.0e-12 * std::fabs(f) &&
            std::fabs(prered) <= 1.0e-12 * std::fabs(f)) break;

        if (delta < 1e-12) break; // prevent infinite loop
    }
    return x;
}

#include <cassert>
#include <cmath>
#include <vector>

// The solution function is assumed to be declared above in the same translation unit.
// Here we write main with asserts.

int main() {
    // Case 1: Simple diagonal, b = [2,0], minimizer should be [1,0]
    {
        std::vector<std::vector<double>> A = {{2.0, 0.0}, {0.0, 2.0}};
        std::vector<double> b = {2.0, 0.0};
        double c = 0.0;
        std::vector<double> x = trustRegionMinimize(A, b, c, 1.0, 1e-6, 100);
        assert(x.size() == 2);
        assert(std::fabs(x[0] - 1.0) < 1e-5);
        assert(std::fabs(x[1] - 0.0) < 1e-5);
    }

    // Case 2: 2x2 non-diagonal, exact solution [1,1]
    {
        std::vector<std::vector<double>> A = {{4.0, 1.0}, {1.0, 3.0}};
        std::vector<double> b = {5.0, 2.0};
        double c = 1.0;
        std::vector<double> x = trustRegionMinimize(A, b, c, 0.5, 1e-8, 200);
        assert(x.size() == 2);
        assert(std::fabs(x[0] - 1.0) < 1e-5);
        assert(std::fabs(x[1] - 1.0) < 1e-5);
    }

    // Case 3: Zero gradient at initial point -> returns zero vector
    {
        std::vector<std::vector<double>> A = {{3.0, 0.0}, {0.0, 3.0}};
        std::vector<double> b = {0.0, 0.0};
        double c = 2.0;
        std::vector<double> x = trustRegionMinimize(A, b, c, 1.0, 1e-6, 10);
        assert(x.size() == 2);
        assert(std::fabs(x[0]) < 1e-8);
        assert(std::fabs(x[1]) < 1e-8);
    }

    // Case 4: Empty input
    {
        std::vector<std::vector<double>> A;
        std::vector<double> b;
        std::vector<double> x = trustRegionMinimize(A, b, 0.0, 1.0, 1e-6, 10);
        assert(x.empty());
    }

    // Case 5: Invalid parameters
    {
        std::vector<std::vector<double>> A = {{2.0}};
        std::vector<double> b = {1.0};
        std::vector<double> x = trustRegionMinimize(A, b, 0.0, -1.0, 1e-6, 10);
        assert(x.size() == 1 && x[0] == 0.0);
        std::vector<double> y = trustRegionMinimize(A, b, 0.0, 1.0, -1e-6, 10);
        assert(y.size() == 1 && y[0] == 0.0);
        std::vector<double> z = trustRegionMinimize(A, b, 0.0, 1.0, 1e-6, 0);
        assert(z.size() == 1 && z[0] == 0.0);
    }

    // Case 6: Larger dimension, 5x5 diagonal with known solution
    {
        int n = 5;
        std::vector<std::vector<double>> A(n, std::vector<double>(n, 0.0));
        std::vector<double> b(n);
        for (int i = 0; i < n; ++i) {
            A[i][i] = (i + 1) * 2.0;
            b[i] = (i + 1) * 1.0; // solution x_i = 0.5
        }
        std::vector<double> x = trustRegionMinimize(A, b, 0.0, 2.0, 1e-8, 500);
        assert(x.size() == n);
        for (int i = 0; i < n; ++i) {
            assert(std::fabs(x[i] - 0.5) < 1e-5);
        }
    }

    // Case 7: Scalar (n=1) case
    {
        std::vector<std::vector<double>> A = {{5.0}};
        std::vector<double> b = {10.0};
        std::vector<double> x = trustRegionMinimize(A, b, 1.0, 0.1, 1e-10, 50);
        assert(x.size() == 1);
        assert(std::fabs(x[0] - 2.0) < 1e-5);
    }

    return 0;
}
