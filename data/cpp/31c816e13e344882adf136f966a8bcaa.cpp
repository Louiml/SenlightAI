Write a standalone C++ function `fitGaussianMixture` that accepts a flat array of `double` data points, the number of points `numData`, the dimensionality `dim`, the number of mixture components `components`, a convergence tolerance `tol`, and a maximum iteration count `maxIter`. The function computes a Gaussian Mixture Model (GMM) using the Expectation-Maximization (EM) algorithm and fills three output arrays: `weights` (size `components`), `means` (size `components * dim`), and `covariances` (size `components * dim * dim`). The output arrays must be initialized with reasonable starting values by the caller, and the function must update them in-place. The implementation must be self-contained (no external libraries), handle degenerate cases like zero variance by adding a small diagonal jitter, and return the number of iterations actually performed. The function must not print anything or depend on any global state.

The core algorithm is Expectation-Maximization for a Gaussian mixture. The function receives flattened data: element `data[i * dim + d]` is the d-th coordinate of the i-th point. The EM loop consists of two steps:
- **E-step:** For each point and each component, compute the probability density under the component's Gaussian (using the multivariate normal formula with determinant and inverse of the covariance matrix). Multiply by the component weight, normalize across components, and store the responsibility. To avoid underflow, use the log-likelihood form and subtract the maximum log value for each point before exponentiating.
- **M-step:** Update each component's weight as the average responsibility, re-estimate the mean as the responsibility-weighted average of points, and recompute the covariance as the responsibility-weighted outer product of deviations from the new mean. Add a small regularization (e.g., `1e-6 * identity`) to each covariance to ensure positive definiteness.
- **Convergence:** After each EM iteration, compute the total log-likelihood. If the improvement is less than `tol`, stop. If no improvement or if `maxIter` is reached, stop anyway.
- **Edge cases:** If all responsibilities for a component become zero, reset that component to a random point with a small variance. If `dim == 1`, the covariance reduces to a scalar variance. The function must handle `numData < components` (e.g., by reducing effective components) but for simplicity, we assume `numData >= components`. The initial weights and means are provided by caller, but we ensure they are normalized to sum to 1 after each update.
- **Time complexity:** Each EM iteration is O(numData * components * dim^2) due to covariance inversion and determinant computation (using Cholesky or Gaussian elimination). The number of iterations is bounded by `maxIter`. Space complexity is O(components * dim^2 + numData * components) for covariance matrices and responsibility matrix.

#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <cassert>

// Compute the multivariate normal density log for a single point.
// Returns the log of the density, and optionally the inverse covariance and log determinant.
inline double logGaussian(const double* x, const double* mean, const double* cov, int dim, double* invCov, double& logDet) {
    // Gaussian elimination to invert covariance and compute log determinant.
    std::vector<double> A(cov, cov + dim * dim);
    std::vector<double> inv(dim * dim, 0.0);
    for (int i = 0; i < dim; ++i) inv[i * dim + i] = 1.0;

    logDet = 0.0;
    for (int col = 0; col < dim; ++col) {
        // Find pivot
        int pivot = col;
        double maxVal = std::fabs(A[col * dim + col]);
        for (int row = col + 1; row < dim; ++row) {
            if (std::fabs(A[row * dim + col]) > maxVal) {
                maxVal = std::fabs(A[row * dim + col]);
                pivot = row;
            }
        }
        if (maxVal < 1e-12) return -std::numeric_limits<double>::infinity(); // singular
        if (pivot != col) {
            for (int k = 0; k < dim; ++k) {
                std::swap(A[col * dim + k], A[pivot * dim + k]);
                std::swap(inv[col * dim + k], inv[pivot * dim + k]);
            }
        }
        double diag = A[col * dim + col];
        logDet += std::log(diag);
        // Normalize row
        for (int k = 0; k < dim; ++k) {
            A[col * dim + k] /= diag;
            inv[col * dim + k] /= diag;
        }
        // Eliminate other rows
        for (int row = 0; row < dim; ++row) {
            if (row == col) continue;
            double factor = A[row * dim + col];
            if (std::fabs(factor) < 1e-15) continue;
            for (int k = 0; k < dim; ++k) {
                A[row * dim + k] -= factor * A[col * dim + k];
                inv[row * dim + k] -= factor * inv[col * dim + k];
            }
        }
    }
    // Copy inverse
    for (int i = 0; i < dim * dim; ++i) invCov[i] = inv[i];

    // Compute mahalanobis distance squared: (x - mean)^T * invCov * (x - mean)
    std::vector<double> diff(dim);
    for (int d = 0; d < dim; ++d) diff[d] = x[d] - mean[d];
    std::vector<double> tmp(dim, 0.0);
    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < dim; ++j)
            tmp[i] += invCov[i * dim + j] * diff[j];
    double maha = 0.0;
    for (int i = 0; i < dim; ++i) maha += diff[i] * tmp[i];

    const double logPi = std::log(2.0 * M_PI);
    return -0.5 * (dim * logPi + logDet + maha);
}

// Main function: fit a Gaussian mixture model to data.
// data: input matrix flattened row-major (numData x dim)
// weights, means, covariances: output arrays initialized with starting values by caller
// Returns number of EM iterations performed.
int fitGaussianMixture(const double* data, double* weights, double* means, double* covariances,
                       int numData, int dim, int components, double tol, int maxIter) {
    const double eps = 1e-6; // regularization for covariance

    // Allocate responsibility matrix
    std::vector<std::vector<double>> resp(numData, std::vector<double>(components, 0.0));

    double prevLogLik = -std::numeric_limits<double>::infinity();
    int iter = 0;

    for (iter = 0; iter < maxIter; ++iter) {
        // ---- E-step ----
        double totalLogLik = 0.0;
        for (int i = 0; i < numData; ++i) {
            std::vector<double> logDens(components);
            std::vector<double> invCov(dim * dim);
            double logDet;
            for (int c = 0; c < components; ++c) {
                const double* mean = means + c * dim;
                const double* cov = covariances + c * dim * dim;
                logDens[c] = std::log(weights[c] + 1e-300) + logGaussian(data + i * dim, mean, cov, dim, invCov.data(), logDet);
            }
            // Stable softmax
            double maxLog = *std::max_element(logDens.begin(), logDens.end());
            double sum = 0.0;
            for (int c = 0; c < components; ++c) {
                resp[i][c] = std::exp(logDens[c] - maxLog);
                sum += resp[i][c];
            }
            for (int c = 0; c < components; ++c) resp[i][c] /= sum;
            totalLogLik += maxLog + std::log(sum);
        }

        if (iter > 0 && std::fabs(totalLogLik - prevLogLik) < tol) {
            // Converged
            break;
        }
        prevLogLik = totalLogLik;

        // ---- M-step ----
        // Update weights and means
        std::vector<double> newWeights(components, 0.0);
        std::vector<std::vector<double>> newMeans(components, std::vector<double>(dim, 0.0));
        for (int c = 0; c < components; ++c) {
            double sumResp = 0.0;
            for (int i = 0; i < numData; ++i) {
                sumResp += resp[i][c];
                for (int d = 0; d < dim; ++d) {
                    newMeans[c][d] += resp[i][c] * data[i * dim + d];
                }
            }
            if (sumResp < 1e-12) {
                // Reset this component to a random point and small variance
                int pick = std::rand() % numData;
                for (int d = 0; d < dim; ++d) newMeans[c][d] = data[pick * dim + d];
                newWeights[c] = 1.0 / components;
                // Set covariance to small identity
                for (int r = 0; r < dim; ++r)
                    for (int col = 0; col < dim; ++col)
                        covariances[c * dim * dim + r * dim + col] = (r == col ? eps : 0.0);
            } else {
                for (int d = 0; d < dim; ++d) newMeans[c][d] /= sumResp;
                newWeights[c] = sumResp / numData;
            }
        }
        // Copy means to output
        for (int c = 0; c < components; ++c)
            for (int d = 0; d < dim; ++d) means[c * dim + d] = newMeans[c][d];

        // Update covariances
        for (int c = 0; c < components; ++c) {
            double sumResp = 0.0;
            for (int i = 0; i < numData; ++i) sumResp += resp[i][c];
            if (sumResp < 1e-12) continue; // already reset

            std::vector<double> newCov(dim * dim, 0.0);
            for (int i = 0; i < numData; ++i) {
                std::vector<double> diff(dim);
                for (int d = 0; d < dim; ++d) diff[d] = data[i * dim + d] - means[c * dim + d];
                for (int r = 0; r < dim; ++r)
                    for (int col = 0; col < dim; ++col)
                        newCov[r * dim + col] += resp[i][c] * diff[r] * diff[col];
            }
            for (int r = 0; r < dim; ++r)
                for (int col = 0; col < dim; ++col) {
                    covariances[c * dim * dim + r * dim + col] = newCov[r * dim + col] / sumResp;
                    if (r == col) covariances[c * dim * dim + r * dim + col] += eps;
                }
        }

        // Normalize weights to sum to 1 (they should already, but ensure)
        double wSum = 0.0;
        for (int c = 0; c < components; ++c) wSum += newWeights[c];
        for (int c = 0; c < components; ++c) weights[c] = newWeights[c] / wSum;
        prevLogLik = totalLogLik;
    }

    return iter;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>

int main() {
    // Test 1: 1D two-component mixture, data clearly separated.
    {
        std::vector<double> data = { -5, -4.9, -5.1, -4.8, 5, 5.1, 4.9, 5.2 };
        const int dim = 1, comp = 2, numData = data.size();
        std::vector<double> weights = {0.5, 0.5};
        std::vector<double> means = { -1, 1 };  // initial guesses
        std::vector<double> cov = {1.0, 1.0};   // scalar variance per component
        int iters = fitGaussianMixture(data.data(), weights.data(), means.data(), cov.data(),
                                       numData, dim, comp, 1e-4, 100);
        assert(iters > 0);
        // After EM, one mean should be near -5, other near +5
        double m1 = std::min(means[0], means[1]);
        double m2 = std::max(means[0], means[1]);
        assert(std::fabs(m1 + 5.0) < 0.5);
        assert(std::fabs(m2 - 5.0) < 0.5);
    }

    // Test 2: 2D two components with diagonal covariance.
    {
        std::vector<double> data;
        for (int i = 0; i < 50; ++i) {
            data.push_back(0.0 + 0.1 * i); // x around 0-5
            data.push_back(0.0 + 0.2 * i); // y around 0-10
        }
        for (int i = 0; i < 50; ++i) {
            data.push_back(10.0 + 0.1 * i);
            data.push_back(20.0 + 0.1 * i);
        }
        const int numData = 100, dim = 2, comp = 2;
        std::vector<double> weights = {0.5, 0.5};
        std::vector<double> means = {0, 0, 10, 10};
        std::vector<double> cov(comp * dim * dim, 0.0);
        for (int c = 0; c < comp; ++c) {
            cov[c * dim * dim + 0 * dim + 0] = 1.0;
            cov[c * dim * dim + 1 * dim + 1] = 1.0;
        }
        fitGaussianMixture(data.data(), weights.data(), means.data(), cov.data(),
                           numData, dim, comp, 1e-3, 200);
        // Find component with larger x mean
        double mx1 = means[0], mx2 = means[2];
        double my1 = means[1], my2 = means[3];
        if (mx1 > mx2) { std::swap(mx1, mx2); std::swap(my1, my2); }
        assert(std::fabs(mx1 - 2.5) < 1.0);
        assert(std::fabs(my1 - 5.0) < 1.0);
        assert(std::fabs(mx2 - 12.5) < 1.0);
        assert(std::fabs(my2 - 22.5) < 1.0);
    }

    // Test 3: Single component, should just compute mean and variance.
    {
        std::vector<double> data = {1,2,3,4,5};
        const int dim = 1, comp = 1, numData = 5;
        std::vector<double> weights = {1.0};
        std::vector<double> means = {0.0};
        std::vector<double> cov = {1.0};
        fitGaussianMixture(data.data(), weights.data(), means.data(), cov.data(),
                           numData, dim, comp, 1e-6, 10);
        assert(std::fabs(means[0] - 3.0) < 1e-3);
        assert(std::fabs(cov[0] - 2.0) < 1e-2);
    }

    // Test 4: Weights should sum to ~1.
    {
        std::vector<double> data;
        for (int i = 0; i < 20; ++i) data.push_back(0.0 + i * 0.1);
        for (int i = 0; i < 20; ++i) data.push_back(10.0 + i * 0.1);
        const int dim = 1, comp = 2, numData = data.size();
        std::vector<double> weights = {0.2, 0.8};
        std::vector<double> means = {0, 10};
        std::vector<double> cov = {1, 1};
        fitGaussianMixture(data.data(), weights.data(), means.data(), cov.data(),
                           numData, dim, comp, 1e-4, 50);
        double sum = 0.0;
        for (double w : weights) sum += w;
        assert(std::fabs(sum - 1.0) < 1e-6);
    }

    // Test 5: Degenerate data (all same point) should not crash.
    {
        std::vector<double> data(10, 3.0);
        const int dim = 1, comp = 2, numData = data.size();
        std::vector<double> weights = {0.5, 0.5};
        std::vector<double> means = {0, 1};
        std::vector<double> cov = {1, 1};
        int it = fitGaussianMixture(data.data(), weights.data(), means.data(), cov.data(),
                                    numData, dim, comp, 1e-6, 10);
        assert(it >= 0);
    }

    std::cout << "All tests passed" << std::endl;
    return 0;
}
