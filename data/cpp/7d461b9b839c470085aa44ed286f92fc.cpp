Implement a standalone C++ function that performs one iteration of the trust-region truncated conjugate gradient (CG) method for minimizing a quadratic model. The function should take as parameters: the trust-region radius `delta` (a positive double), a dense symmetric positive-definite Hessian matrix `H` (represented as a 1D array in row-major order of size `n*n`), a gradient vector `g` (size `n`), and the dimension `n`. It should return the solution vector `s` (size `n`) that approximately minimizes `0.5 * s^T H s + g^T s` subject to the norm constraint `||s||_2 <= delta`. Use the standard CG algorithm with a tolerance of `0.1 * ||g||_2` as the stopping criterion, and if the CG step exits the trust region, perform the standard dogleg-style truncation to project the step onto the boundary. The function should return the number of CG iterations performed before termination.

// The solution uses the truncated conjugate gradient method to solve the constrained quadratic subproblem. Initialize `s=0`, `r = -g` (negative gradient, residual), `d = r` (search direction), and compute `rTr = r^T r`. The CG iteration proceeds while the residual norm exceeds `0.1 * ||g||` and no trust-region violation occurs. At each step, compute `Hd = H * d` (matrix-vector product without explicit O(n^2) loops; use double loops with index `H[i*n+j] * d[j]`), then `alpha = rTr / (d^T Hd)`. Update `s = s + alpha*d`. If `||s|| > delta`, the step has left the trust region; truncate it by finding the scalar `t` such that `||s + t*d|| = delta` and `t` is the smallest positive root of the quadratic `||s + t*d||^2 = delta^2`. Solve using the standard formula: compute `std = s^T d`, `sts = ||s||^2`, `dtd = ||d||^2`, `dsq = delta^2`, and `rad = sqrt(std^2 + dtd*(dsq - sts))`; if `std >= 0`, `t = (dsq - sts)/(std + rad)`; else `t = (rad - std)/dtd`. Then set `s = s + t*d` and stop. If no boundary hit, update `r = r - alpha*Hd`, compute new `rnewTrnew`, set `beta = rnewTrnew / rTr`, update `d = r + beta*d`, and continue. The algorithm terminates when either the residual is small enough or the trust-region boundary is reached. Edge cases include the first iteration where `s` is zero, and when `alpha` becomes negative due to rounding, but the formulas handle that correctly. Time complexity is O(n^2) per CG iteration due to Hessian-vector product, with at most O(n) iterations (though in practice it stops earlier), giving O(n^3) worst-case. Space complexity is O(n) besides the Hessian.

#include <cmath>
#include <vector>
#include <algorithm>

// Solve the trust-region subproblem using truncated CG.
// Given symmetric positive-definite Hessian H (row-major, size n*n),
// gradient g, and trust-region radius delta, return approximate minimizer s.
// Returns number of CG iterations performed.
int trust_region_cg(double delta, const std::vector<double>& H,
                    const std::vector<double>& g, int n,
                    std::vector<double>& s) {
    s.assign(n, 0.0);
    std::vector<double> r(n), d(n), Hd(n);
    
    // Initialize residual r = -g, direction d = r
    for (int i = 0; i < n; ++i) {
        r[i] = -g[i];
        d[i] = r[i];
    }
    
    // Compute ||g|| for tolerance
    double gnorm = 0.0;
    for (double val : g) gnorm += val * val;
    gnorm = std::sqrt(gnorm);
    double cgtol = 0.1 * gnorm;
    
    double rTr = 0.0;
    for (double val : r) rTr += val * val;
    
    int cg_iter = 0;
    
    while (true) {
        // Check residual norm
        double rnorm = 0.0;
        for (double val : r) rnorm += val * val;
        rnorm = std::sqrt(rnorm);
        if (rnorm <= cgtol) break;
        
        // Compute Hd = H * d (matrix-vector product)
        for (int i = 0; i < n; ++i) {
            Hd[i] = 0.0;
            for (int j = 0; j < n; ++j) {
                Hd[i] += H[i * n + j] * d[j];
            }
        }
        
        // Compute d^T Hd
        double dHd = 0.0;
        for (int i = 0; i < n; ++i) dHd += d[i] * Hd[i];
        
        // Compute alpha = rTr / (d^T Hd)
        double alpha = rTr / dHd;
        
        // Update s = s + alpha * d
        for (int i = 0; i < n; ++i) s[i] += alpha * d[i];
        
        // Check trust region boundary
        double snorm = 0.0;
        for (double val : s) snorm += val * val;
        snorm = std::sqrt(snorm);
        
        if (snorm > delta) {
            // Truncate: find t such that ||s + t*d|| = delta, t > 0
            // Note: s currently overshoots, need to step back along d
            // Better approach: we added alpha*d, so we need to adjust
            // Solve ||s_old + t*d|| = delta, where s_old = s - alpha*d
            std::vector<double> s_old(n);
            for (int i = 0; i < n; ++i) s_old[i] = s[i] - alpha * d[i];
            
            // Compute coefficients: ||s_old + t*d||^2 = delta^2
            double std = 0.0, sts = 0.0, dtd = 0.0;
            for (int i = 0; i < n; ++i) {
                std += s_old[i] * d[i];
                sts += s_old[i] * s_old[i];
                dtd += d[i] * d[i];
            }
            double dsq = delta * delta;
            double rad = std::sqrt(std * std + dtd * (dsq - sts));
            double t;
            if (std >= 0) {
                t = (dsq - sts) / (std + rad);
            } else {
                t = (rad - std) / dtd;
            }
            // Final s = s_old + t*d
            for (int i = 0; i < n; ++i) s[i] = s_old[i] + t * d[i];
            break;  // stop at boundary
        }
        
        // Update residual r = r - alpha * Hd
        for (int i = 0; i < n; ++i) r[i] -= alpha * Hd[i];
        
        // Compute new rTr
        double rnewTrnew = 0.0;
        for (double val : r) rnewTrnew += val * val;
        
        // Compute beta
        double beta = rnewTrnew / rTr;
        
        // Update direction d = r + beta * d
        for (int i = 0; i < n; ++i) d[i] = r[i] + beta * d[i];
        
        rTr = rnewTrnew;
        cg_iter++;
    }
    
    return cg_iter;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>

// Declare the function we're testing
int trust_region_cg(double delta, const std::vector<double>& H,
                    const std::vector<double>& g, int n,
                    std::vector<double>& s);

int main() {
    // Test 1: 1D problem, simple quadratic H=[2], g=[-2], delta=1
    // Solution should be s=1 (on boundary)
    {
        int n = 1;
        std::vector<double> H = {2.0};
        std::vector<double> g = {-2.0};
        std::vector<double> s;
        int cg = trust_region_cg(1.0, H, g, n, s);
        assert(std::fabs(s[0] - 1.0) < 1e-6);
        (void)cg; // avoid unused warning
    }
    
    // Test 2: 2D identity Hessian, gradient [-1, -1], delta=2
    // Solution should be s=[1,1] (on boundary, since ||s|| = sqrt(2) < 2)
    {
        int n = 2;
        std::vector<double> H = {1,0,0,1};
        std::vector<double> g = {-1.0, -1.0};
        std::vector<double> s;
        trust_region_cg(2.0, H, g, n, s);
        assert(std::fabs(s[0] - 1.0) < 1e-6);
        assert(std::fabs(s[1] - 1.0) < 1e-6);
    }
    
    // Test 3: Trust region boundary hit exactly: gradient [-2,-2], identity, delta=1
    // Unconstrained optimum is [2,2], but boundary constraint forces ||s||=1
    {
        int n = 2;
        std::vector<double> H = {1,0,0,1};
        std::vector<double> g = {-2.0, -2.0};
        std::vector<double> s;
        trust_region_cg(1.0, H, g, n, s);
        double norm = std::sqrt(s[0]*s[0] + s[1]*s[1]);
        assert(std::fabs(norm - 1.0) < 1e-6);
        // Direction should be along gradient: s should be positive multiples
        assert(s[0] > 0 && s[1] > 0);
        assert(std::fabs(s[0] - s[1]) < 1e-6); // symmetric
    }
    
    // Test 4: Zero gradient should return zero step
    {
        int n = 3;
        std::vector<double> H = {2,0,0,0,2,0,0,0,2};
        std::vector<double> g = {0.0, 0.0, 0.0};
        std::vector<double> s;
        trust_region_cg(1.0, H, g, n, s);
        for (int i = 0; i < n; ++i) {
            assert(std::fabs(s[i]) < 1e-12);
        }
    }
    
    // Test 5: Large diagonal Hessian, large delta (unconstrained optimum inside)
    // H = diag(1,2,3), g = [-0.5, -1, -1.5]  -> optimum at [0.5,0.5,0.5]
    {
        int n = 3;
        std::vector<double> H = {1,0,0, 0,2,0, 0,0,3};
        std::vector<double> g = {-0.5, -1.0, -1.5};
        std::vector<double> s;
        trust_region_cg(10.0, H, g, n, s);
        assert(std::fabs(s[0] - 0.5) < 1e-6);
        assert(std::fabs(s[1] - 0.5) < 1e-6);
        assert(std::fabs(s[2] - 0.5) < 1e-6);
    }
    
    std::cout << "All trust-region CG tests passed!" << std::endl;
    return 0;
}
