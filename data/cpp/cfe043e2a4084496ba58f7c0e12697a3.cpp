/*
Write a C++ function `fitGaussianToProfile` that takes a sorted vector of x-coordinates (`std::vector<double>`), a corresponding vector of measured intensity values (`std::vector<double>`, same size), and an initial estimate for the mean as a double. The function must fit a Gaussian model of the form `y = A * exp(-0.5 * ((x - mu)/sigma)^2) + B` (with four parameters: amplitude `A`, mean `mu`, standard deviation `sigma`, and constant offset `B`) using a simple iterative nonlinear least-squares approach based on Gauss-Newton optimization with a finite-difference Jacobian. The function should return a `GaussianFit` struct containing the fitted parameters (`A`, `mu`, `sigma`, `B`) and the number of iterations performed. Use ordinary least squares without weighting, require at least 4 data points, and handle cases where the initial estimate might not be near the optimum by using a maximum of 100 iterations and a convergence tolerance of 1e-6 on the relative change in the sum of squared residuals. Ensure the function is const-correct wherever appropriate and does not modify the input vectors.
*/
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>

struct GaussianFit {
    double A;      // amplitude
    double mu;     // mean
    double sigma;  // standard deviation
    double B;      // constant offset
    int iterations;
};

namespace {
    // Model function: y = A * exp(-0.5 * ((x - mu)/sigma)^2) + B
    double modelValue(double x, double A, double mu, double sigma, double B) {
        double t = (x - mu) / sigma;
        return A * std::exp(-0.5 * t * t) + B;
    }

    // Compute residuals vector (size m) given parameters
    void computeResiduals(const std::vector<double>& x,
                          const std::vector<double>& y,
                          const double* params,
                          std::vector<double>& residual) {
        double A = params[0], mu = params[1], sigma = params[2], B = params[3];
        for (size_t i = 0; i < x.size(); ++i) {
            residual[i] = y[i] - modelValue(x[i], A, mu, sigma, B);
        }
    }

    // Solve 4x4 linear system Ax = b using Gaussian elimination with partial pivoting
    // Returns true on success, false if singular
    bool solveLinearSystem(double A[4][4], double b[4], double x[4]) {
        // Augmented matrix
        double aug[4][5];
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) aug[i][j] = A[i][j];
            aug[i][4] = b[i];
        }

        for (int col = 0; col < 4; ++col) {
            // Find pivot
            int pivotRow = col;
            double maxVal = std::fabs(aug[col][col]);
            for (int row = col + 1; row < 4; ++row) {
                if (std::fabs(aug[row][col]) > maxVal) {
                    maxVal = std::fabs(aug[row][col]);
                    pivotRow = row;
                }
            }
            if (maxVal < 1e-12) return false; // singular

            if (pivotRow != col) {
                for (int j = 0; j < 5; ++j) std::swap(aug[col][j], aug[pivotRow][j]);
            }

            // Eliminate below
            for (int row = col + 1; row < 4; ++row) {
                double factor = aug[row][col] / aug[col][col];
                for (int j = col; j < 5; ++j) {
                    aug[row][j] -= factor * aug[col][j];
                }
            }
        }

        // Back substitution
        for (int row = 3; row >= 0; --row) {
            double sum = aug[row][4];
            for (int j = row + 1; j < 4; ++j) {
                sum -= aug[row][j] * x[j];
            }
            x[row] = sum / aug[row][row];
        }
        return true;
    }
}

// Fit a Gaussian model to data points (x, y) with an initial mean estimate.
// Returns fitted parameters and iteration count.
GaussianFit fitGaussianToProfile(const std::vector<double>& x,
                                 const std::vector<double>& y,
                                 double initialMean) {
    const size_t m = x.size();
    if (m < 4) {
        throw std::invalid_argument("Need at least 4 data points for Gaussian fit");
    }

    // Initial parameters
    double params[4];
    params[1] = initialMean;  // mu
    params[2] = 1.0;          // sigma, default
    params[3] = *std::min_element(y.begin(), y.end());  // B = min intensity
    params[0] = *std::max_element(y.begin(), y.end()) - params[3];  // A = range

    // Guard against zero amplitude
    if (params[0] < 1e-12) params[0] = 1.0;

    std::vector<double> residual(m);
    std::vector<double> residualNew(m);

    double S_old = 0.0;
    computeResiduals(x, y, params, residual);
    for (auto r : residual) S_old += r * r;

    int iter = 0;
    const int maxIter = 100;
    const double tol = 1e-6;

    while (iter < maxIter) {
        // Compute Jacobian (m x 4) using central finite differences
        const double h = 1e-6;
        std::vector<std::vector<double>> J(m, std::vector<double>(4));
        for (size_t i = 0; i < m; ++i) {
            double xi = x[i];
            // Model as a lambda for clarity
            auto model_p = [&](const double* p) {
                return modelValue(xi, p[0], p[1], p[2], p[3]);
            };
            for (int j = 0; j < 4; ++j) {
                double p_plus[4] = {params[0], params[1], params[2], params[3]};
                double p_minus[4] = {params[0], params[1], params[2], params[3]};
                p_plus[j] += h;
                p_minus[j] -= h;
                J[i][j] = (model_p(p_plus) - model_p(p_minus)) / (2.0 * h);
            }
        }

        // Compute J^T * J and J^T * r
        double JTJ[4][4] = {{0}};
        double JTr[4] = {0};
        for (size_t i = 0; i < m; ++i) {
            double r_i = residual[i];
            for (int a = 0; a < 4; ++a) {
                JTr[a] += J[i][a] * r_i;
                for (int b = 0; b < 4; ++b) {
                    JTJ[a][b] += J[i][a] * J[i][b];
                }
            }
        }

        // Solve (J^T J) * delta = -JTr
        double negJTr[4] = {-JTr[0], -JTr[1], -JTr[2], -JTr[3]};
        double delta[4];
        bool ok = solveLinearSystem(JTJ, negJTr, delta);

        if (!ok) {
            // Singular: take a small step in negative gradient direction
            delta[0] = -0.001 * JTr[0];
            delta[1] = -0.001 * JTr[1];
            delta[2] = -0.001 * JTr[2];
            delta[3] = -0.001 * JTr[3];
        }

        // Update parameters
        for (int j = 0; j < 4; ++j) {
            params[j] += delta[j];
        }

        // Protect sigma from non-positive values
        if (params[2] < 1e-6) params[2] = 1e-6;

        // Compute new residuals and sum of squares
        computeResiduals(x, y, params, residualNew);
        double S_new = 0.0;
        for (auto r : residualNew) S_new += r * r;

        // Check convergence
        double relChange = std::fabs(S_new - S_old) / (S_old + 1e-12);
        S_old = S_new;
        residual = residualNew;
        ++iter;

        if (relChange < tol) break;
    }

    GaussianFit fit;
    fit.A = params[0];
    fit.mu = params[1];
    fit.sigma = params[2];
    fit.B = params[3];
    fit.iterations = iter;
    return fit;
}
#include <cassert>
#include <cmath>
#include <vector>
#include "gaussian_fit.h"  // assuming the solution is in this header

int main() {
    // Test 1: Synthetic data from a known Gaussian (A=2.0, mu=5.0, sigma=1.5, B=0.5)
    {
        std::vector<double> x;
        std::vector<double> y;
        for (int i = 0; i <= 20; ++i) {
            double xi = i * 0.5; // from 0 to 10
            x.push_back(xi);
            double t = (xi - 5.0) / 1.5;
            y.push_back(2.0 * std::exp(-0.5 * t * t) + 0.5);
        }
        GaussianFit fit = fitGaussianToProfile(x, y, 4.5);
        assert(std::fabs(fit.A - 2.0) < 0.1);
        assert(std::fabs(fit.mu - 5.0) < 0.1);
        assert(std::fabs(fit.sigma - 1.5) < 0.1);
        assert(std::fabs(fit.B - 0.5) < 0.1);
        assert(fit.iterations > 0 && fit.iterations <= 100);
    }

    // Test 2: Shifted Gaussian with different amplitude and offset
    {
        std::vector<double> x = {-3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0};
        std::vector<double> y;
        for (double xi : x) {
            double t = (xi - 0.0) / 0.7;
            y.push_back(5.0 * std::exp(-0.5 * t * t) + 1.0);
        }
        GaussianFit fit = fitGaussianToProfile(x, y, 0.2);
        assert(std::fabs(fit.A - 5.0) < 0.5);
        assert(std::fabs(fit.mu - 0.0) < 0.2);
        assert(std::fabs(fit.sigma - 0.7) < 0.2);
        assert(std::fabs(fit.B - 1.0) < 0.2);
    }

    // Test 3: Constant data (zero amplitude) should still produce a valid fit
    {
        std::vector<double> x = {0.0, 1.0, 2.0, 3.0, 4.0};
        std::vector<double> y = {2.0, 2.0, 2.0, 2.0, 2.0};
        GaussianFit fit = fitGaussianToProfile(x, y, 1.0);
        assert(std::fabs(fit.B - 2.0) < 0.1);
        assert(fit.A < 0.5); // amplitude should be near zero
    }

    // Test 4: Not enough data points throws exception
    {
        std::vector<double> x = {1.0, 2.0, 3.0};
        std::vector<double> y = {1.0, 2.0, 1.0};
        bool threw = false;
        try {
            fitGaussianToProfile(x, y, 2.0);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    // Test 5: Input vectors are not modified (const correctness)
    {
        std::vector<double> x = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0};
        std::vector<double> y_orig = {1.0, 2.0, 3.0, 2.0, 1.0, 0.5};
        std::vector<double> x_copy = x;
        std::vector<double> y_copy = y_orig;
        GaussianFit fit = fitGaussianToProfile(x, y_orig, 2.5);
        assert(x == x_copy);
        assert(y_orig == y_copy);
    }

    // Test 6: Narrow Gaussian with sigma smaller than initial guess
    {
        std::vector<double> x;
        std::vector<double> y;
        for (int i = 0; i <= 40; ++i) {
            double xi = (i - 20) * 0.1; // -2.0 to 2.0
            x.push_back(xi);
            double t = xi / 0.2;
            y.push_back(3.0 * std::exp(-0.5 * t * t) + 0.0);
        }
        GaussianFit fit = fitGaussianToProfile(x, y, 0.0);
        assert(std::fabs(fit.mu) < 0.05);
        assert(std::fabs(fit.sigma - 0.2) < 0.1);
    }

    return 0;
}
// The solution implements a Gauss-Newton optimizer for the nonlinear least-squares problem. The main algorithm proceeds iteratively: (1) compute residuals `r_k = y_k - model(x_k, params)` and the sum of squared residuals `S = Σ r_k²`; (2) compute the Jacobian matrix J (size m×4) numerically via central finite differences, where each column corresponds to a parameter's partial derivative: for perturbation `h`, `J_{k,j} = (model(x_k, params + h*e_j) - model(x_k, params - h*e_j)) / (2h)`; (3) solve the normal equations `(J^T J) Δ = -J^T r` for parameter update Δ using Gaussian elimination with partial pivoting; (4) update parameters `params += Δ`; (5) recompute residuals and S; if the relative change `|S_new - S_old| / (S_old + 1e-12)` is below the tolerance, stop. Edge cases: if `J^T J` is singular or near-singular (determinant below 1e-12), fall back to a small step in the negative gradient direction (0.001 * gradient) to avoid stagnation. The initial `sigma` is set to 1.0 if not provided, `A` is set to the max intensity minus min intensity, `B` is set to the min intensity. Time complexity per iteration is dominated by the Jacobian computation: 8 model evaluations per data point (4 parameters × 2 perturbations) and solving a 4×4 system, giving O(m) per iteration and O(100m) total in the worst case. Space complexity is O(m) for residuals and the Jacobian.
