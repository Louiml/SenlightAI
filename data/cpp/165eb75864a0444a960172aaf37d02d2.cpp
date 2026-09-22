// Write a standalone C++ function that, given a symmetric positive semi-definite covariance matrix `Sigma` (as an `std::vector<std::vector<double>>`) and a mean vector `mu`, returns a single random sample from the multivariate normal distribution using the Cholesky decomposition when possible, and falls back to an eigenvalue-based method (symmetric eigendecomposition) when the Cholesky factorization fails (e.g., due to numerical indefiniteness). The function must validate that `Sigma` is square, that `mu` has the same dimension, and that the symmetric eigen-decomposition yields eigenvalues that are not significantly negative (within a tolerance relative to the largest eigenvalue); if any eigenvalue is too negative, throw a `std::invalid_argument`. The returned vector must have the same dimension `p` as `mu`. Use only standard C++ libraries (no external dependencies). For the eigenvalue fallback, you may implement a simple symmetric QR algorithm or, for simplicity, assume that the matrix is at most 3x3 and use a direct algebraic method (e.g., for 2x2, closed-form eigenvalues; for 3x3, use the cubic solver). However, the primary challenge is to correctly handle the fallback and ensure the sample is drawn correctly by computing `X = mu + (Z * U)` for Cholesky (where `U` is upper triangular, `Z` is a row vector of i.i.d. standard normals) and `X = mu + (Z * E * sqrt(D))` for the eigendecomposition, where `E` is the orthonormal eigenvector matrix and `D` is the diagonal matrix of eigenvalues. Use `std::normal_distribution` for random number generation and ensure thread-safety by accepting a random engine reference.

// The solution mirrors the provided RcppArmadillo code but uses only the C++ standard library. The main algorithm proceeds as follows: 1) Validate inputs: `Sigma` must be square (n x n), `mu` must have size n, and the matrix should be symmetric (to within a small tolerance, e.g., 1e-12). If not symmetric, throw `std::invalid_argument`. 2) Attempt Cholesky decomposition: implement a standard in-place or copy-based Cholesky for symmetric positive definite matrices. A simple way is to compute the lower-triangular `L` such that `Sigma = L * L^T`; if any diagonal element becomes non-positive during the process, the decomposition fails. Because we need an upper-triangular `U` for the formula `X = mu + Z * U` where `Z` is a row vector, we can either compute `U = L^T` or directly compute the upper-triangular form. The Cholesky algorithm: for i from 0 to n-1, for j from i to n-1, compute `Sigma[i][j] -= sum_{k<i} Sigma[i][k]*Sigma[j][k]` (if using lower), then set `diag = Sigma[i][i]`; if `diag <= tol * abs(max_diag)` (tol ~ 1e-9), fail. If successful, `U` (upper) is such that `U[i][j] = 0` for i>j, `U[i][i] = sqrt(diag)`, and `U[i][j] = Sigma[i][j] / diag` for i<j. 3) If Cholesky fails, perform symmetric eigendecomposition. For symmetric matrices, the eigenvectors are orthonormal. Implementing a general QR algorithm is nontrivial; since the task asks for a standalone exercise, we can restrict the dimension to be at most 3 and use analytic formulas: for 2x2, eigenvalues are `(a+d)/2 ± sqrt(((a-d)/2)^2 + b^2)`; for 3x3, one can use the trigonometric method for the roots of the characteristic polynomial (or a cubic solver). Then, for each eigenvalue, compute an eigenvector by solving `(Sigma - λI) v = 0` (e.g., via cross product for 3x3 or simple linear solve for 2x2) and orthonormalize via Gram-Schmidt. 4) After obtaining eigenvalues `eigval` (ascending order) and eigenvectors `eigvec` (columns), check positive semi-definiteness: if `eigval[i] <= -tol * abs(eigval.back())` for any i, throw `std::invalid_argument`. 5) Form `pmax = max(eigval, 0)` (element-wise max with 0), compute `rmat = diag(sqrt(pmax))`, and compute `X = mu + Z * eigvec * rmat` (where `Z` is a row vector of standard normals). 6) Return a column vector. Edge cases: zero variance components (eigenvalue exactly 0) are handled by taking sqrt of 0, producing a sample that doesn't vary in that direction; if the matrix is not positive definite (has negative eigenvalues beyond tolerance), throw. Time complexity: Cholesky is O(p^3); eigendecomposition for fixed p ≤ 3 is O(1) but the vector-matrix multiplications are O(p^2). Space: O(p^2) for matrices.

#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <limits>

// Helper to check if a matrix is symmetric within a tolerance.
bool isSymmetric(const std::vector<std::vector<double>>& M, double tol = 1e-12) {
    size_t n = M.size();
    for (size_t i = 0; i < n; ++i) {
        if (M[i].size() != n) return false; // not square
        for (size_t j = 0; j < n; ++j) {
            if (std::abs(M[i][j] - M[j][i]) > tol) return false;
        }
    }
    return true;
}

// 2x2 symmetric eigen decomposition: returns eigenvalues (ascending) and eigenvectors as columns.
void eigen2x2(const std::vector<std::vector<double>>& A, std::vector<double>& eigval, std::vector<std::vector<double>>& eigvec) {
    double a = A[0][0], b = A[0][1], c = A[1][1];
    double tr = a + c;
    double det = a * c - b * b;
    double disc = std::sqrt(std::max(0.0, (tr/2.0)*(tr/2.0) - det));
    eigval = {tr/2.0 - disc, tr/2.0 + disc}; // ascending
    // eigenvectors
    eigvec.assign(2, std::vector<double>(2, 0.0));
    for (int idx = 0; idx < 2; ++idx) {
        double lam = eigval[idx];
        // Solve (A - lam*I)v=0. Use cross-product-like method.
        double v1 = A[0][1];
        double v2 = lam - A[0][0];
        // Ensure nonzero vector; if both zero, use (1,0).
        if (std::abs(v1) < 1e-12 && std::abs(v2) < 1e-12) {
            v1 = 1.0; v2 = 0.0;
        } else {
            // Normalize to avoid scaling issues
            double norm = std::sqrt(v1*v1 + v2*v2);
            v1 /= norm; v2 /= norm;
        }
        eigvec[0][idx] = v1;
        eigvec[1][idx] = v2;
    }
    // Correct sign orientation if needed (not essential for sampling).
}

// 3x3 symmetric eigen decomposition (closed-form via characteristic polynomial).
void eigen3x3(const std::vector<std::vector<double>>& A, std::vector<double>& eigval, std::vector<std::vector<double>>& eigvec) {
    // Use the trigonometric method for eigenvalues.
    double a = A[0][0], b = A[0][1], c = A[0][2];
    double d = A[1][1], e = A[1][2], f = A[2][2];
    // Characteristic polynomial: -λ^3 + I1 λ^2 - I2 λ + I3 = 0
    double I1 = a + d + f;
    double I2 = a*d + a*f + d*f - (b*b + c*c + e*e);
    double I3 = a*d*f + 2*b*c*e - a*e*e - d*c*c - f*b*b;
    // Convert to x^3 + px + q = 0 by letting λ = x + I1/3
    double p = -I2 + I1*I1/3.0;
    double q = 2*I1*I1*I1/27.0 - I1*I2/3.0 + I3;
    double sign_q = (q >= 0) ? 1.0 : -1.0;
    double something = std::pow(std::abs(q)/2.0 + std::sqrt(std::max(0.0, (q*q/4.0) + (p*p*p/27.0))), 1.0/3.0);
    double something2 = something;
    // This is getting messy; instead use the standard trigonometric formula for three real roots.
    // For symmetric matrices, all roots are real.
    double r = std::sqrt(std::max(0.0, -4.0*p*p*p/27.0));
    double phi = std::acos(std::max(-1.0, std::min(1.0, -q/std::max(1e-12, r))));
    double lambda0 = 2.0*std::sqrt(-p/3.0)*std::cos(phi/3.0) + I1/3.0;
    double lambda1 = 2.0*std::sqrt(-p/3.0)*std::cos((phi+2*M_PI)/3.0) + I1/3.0;
    double lambda2 = 2.0*std::sqrt(-p/3.0)*std::cos((phi+4*M_PI)/3.0) + I1/3.0;
    eigval = {std::min({lambda0, lambda1, lambda2}), 
              std::min(std::max(lambda0, lambda1), std::max(lambda0, lambda2)) + std::max(lambda0, std::max(lambda1, lambda2)) - std::max({lambda0, lambda1, lambda2}) - std::min({lambda0, lambda1, lambda2}) - std::min(std::max(lambda0, lambda1), std::max(lambda0, lambda2)) + std::max({lambda0, lambda1, lambda2})}; // This is wrong; simpler: sort.
    // Sort the three values.
    std::vector<double> vals = {lambda0, lambda1, lambda2};
    std::sort(vals.begin(), vals.end());
    eigval = vals;
    
    // Compute eigenvectors by solving (A-λI)v=0 using cross product of two rows.
    eigvec.assign(3, std::vector<double>(3, 0.0));
    for (int idx = 0; idx < 3; ++idx) {
        double lam = eigval[idx];
        // Build matrix B = A - lam*I
        std::vector<std::vector<double>> B = A;
        for (int i=0; i<3; ++i) B[i][i] -= lam;
        // The cross product of rows 0 and 1 gives a vector orthogonal to both rows.
        double vx = B[0][1]*B[1][2] - B[0][2]*B[1][1];
        double vy = B[0][2]*B[1][0] - B[0][0]*B[1][2];
        double vz = B[0][0]*B[1][1] - B[0][1]*B[1][0];
        double norm = std::sqrt(vx*vx + vy*vy + vz*vz);
        if (norm < 1e-12) {
            // Try another pair of rows.
            vx = B[1][1]*B[2][2] - B[1][2]*B[2][1];
            vy = B[1][2]*B[2][0] - B[1][0]*B[2][2];
            vz = B[1][0]*B[2][1] - B[1][1]*B[2][0];
            norm = std::sqrt(vx*vx + vy*vy + vz*vz);
        }
        if (norm < 1e-12) {
            // Degenerate: use basis vector.
            vx = 1.0; vy = 0.0; vz = 0.0; norm = 1.0;
        }
        eigvec[0][idx] = vx / norm;
        eigvec[1][idx] = vy / norm;
        eigvec[2][idx] = vz / norm;
    }
    // Orthonormalize via Gram-Schmidt (since eigenvalues may be repeated).
    for (int i=0; i<3; ++i) {
        for (int j=0; j<i; ++j) {
            double dot = 0.0;
            for (int k=0; k<3; ++k) dot += eigvec[k][j]*eigvec[k][i];
            for (int k=0; k<3; ++k) eigvec[k][i] -= dot * eigvec[k][j];
        }
        double norm = 0.0;
        for (int k=0; k<3; ++k) norm += eigvec[k][i]*eigvec[k][i];
        norm = std::sqrt(norm);
        if (norm < 1e-12) {
            // Fill with a standard basis if zero.
            for (int k=0; k<3; ++k) eigvec[k][i] = (k==i) ? 1.0 : 0.0;
        } else {
            for (int k=0; k<3; ++k) eigvec[k][i] /= norm;
        }
    }
}

// Main function: sample from multivariate normal.
std::vector<double> mvrnorm(const std::vector<double>& mu, 
                            const std::vector<std::vector<double>>& Sigma,
                            std::mt19937& engine,
                            double tol = 1e-9) {
    size_t p = mu.size();
    if (p == 0) return {};
    if (Sigma.size() != p) throw std::invalid_argument("Sigma must be p x p");
    for (const auto& row : Sigma) if (row.size() != p) throw std::invalid_argument("Sigma must be square");
    if (!isSymmetric(Sigma)) throw std::invalid_argument("Sigma must be symmetric");

    std::normal_distribution<double> dist(0.0, 1.0);
    std::vector<double> z(p);
    for (auto& zi : z) zi = dist(engine);

    // Attempt Cholesky (upper triangular U such that Sigma = U^T * U).
    std::vector<std::vector<double>> U(p, std::vector<double>(p, 0.0));
    bool chol_ok = true;
    // Copy Sigma to a working matrix W.
    std::vector<std::vector<double>> W = Sigma;
    double max_diag = 0.0;
    for (size_t i=0; i<p; ++i) max_diag = std::max(max_diag, std::abs(Sigma[i][i]));
    if (max_diag == 0.0) { // all zero -> return zero
        return std::vector<double>(p, 0.0);
    }
    for (size_t j=0; j<p; ++j) {
        for (size_t i=0; i<=j; ++i) {
            double sum = W[i][j];
            for (size_t k=0; k<i; ++k) sum -= U[k][i] * U[k][j];
            if (i == j) {
                if (sum <= tol * std::max(1e-12, max_diag)) {
                    chol_ok = false;
                    break;
                }
                U[i][i] = std::sqrt(sum);
            } else {
                U[i][j] = sum / U[i][i];
            }
        }
        if (!chol_ok) break;
    }

    std::vector<double> result(p, 0.0);
    if (chol_ok) {
        // X = mu + Z * U (Z is row vector)
        for (size_t i=0; i<p; ++i) {
            double val = mu[i];
            for (size_t j=0; j<p; ++j) {
                val += z[i] * U[i][j]; // careful: Z is row, U is upper; result[i] = sum_k z_k * U[k][i]
            }
            // Actually correct: result[i] = mu[i] + sum_{k=0}^{p-1} z[k] * U[k][i]
        }
        // Recompute properly:
        for (size_t i=0; i<p; ++i) {
            double sum = 0.0;
            for (size_t k=0; k<p; ++k) sum += z[k] * U[k][i];
            result[i] = mu[i] + sum;
        }
        return result;
    }

    // Fallback: eigen decomposition for p <= 3.
    if (p > 3) throw std::invalid_argument("Cholesky failed and p>3 not supported for fallback");
    std::vector<double> eigval;
    std::vector<std::vector<double>> eigvec;
    if (p == 1) {
        eigval = {Sigma[0][0]};
        eigvec = {{1.0}};
    } else if (p == 2) {
        eigen2x2(Sigma, eigval, eigvec);
    } else if (p == 3) {
        eigen3x3(Sigma, eigval, eigvec);
    }

    // Check positive semi-definiteness.
    double max_eig = std::abs(eigval.back());
    for (double ev : eigval) {
        if (ev <= -tol * std::max(1e-12, max_eig)) {
            throw std::invalid_argument("Sigma is not positive semi-definite");
        }
    }

    // Compute X = mu + Z * eigvec * diag(sqrt(pmax))
    // First multiply Z * eigvec -> tmp vector of length p.
    std::vector<double> tmp(p, 0.0);
    for (size_t j=0; j<p; ++j) {
        double sum = 0.0;
        for (size_t k=0; k<p; ++k) sum += z[k] * eigvec[k][j];
        tmp[j] = sum;
    }
    // Multiply by sqrt(max(eigval,0))
    for (size_t j=0; j<p; ++j) {
        double pmax = std::max(0.0, eigval[j]);
        result[j] = mu[j] + tmp[j] * std::sqrt(pmax);
    }
    return result;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <random>
#include <iostream>

int main() {
    // Test 1: 2x2 diagonal covariance, compare with known distribution (just check shape/size)
    std::mt19937 engine(12345);
    std::vector<double> mu = {1.0, 2.0};
    std::vector<std::vector<double>> Sigma = {{2.0, 0.0}, {0.0, 3.0}};
    auto sample = mvrnorm(mu, Sigma, engine);
    assert(sample.size() == 2);
    // For many samples, check means approximate mu (not deterministic, but just ensure no crash)
    double sum0 = 0.0, sum1 = 0.0;
    for (int i=0; i<10000; ++i) {
        auto s = mvrnorm(mu, Sigma, engine);
        sum0 += s[0];
        sum1 += s[1];
    }
    assert(std::abs(sum0/10000 - mu[0]) < 0.2);
    assert(std::abs(sum1/10000 - mu[1]) < 0.2);

    // Test 2: positive semi-definite matrix with a zero eigenvalue (1x1)
    std::vector<double> mu1 = {5.0};
    std::vector<std::vector<double>> Sigma1 = {{0.0}};
    auto s1 = mvrnorm(mu1, Sigma1, engine);
    assert(s1.size() == 1);
    assert(std::abs(s1[0] - 5.0) < 1e-12); // always 5

    // Test 3: 2x2 matrix that fails Cholesky but is PSD (e.g., singular)
    std::vector<std::vector<double>> Sigma_psd = {{1.0, 1.0}, {1.0, 1.0}};
    auto s2 = mvrnorm(mu, Sigma_psd, engine);
    assert(s2.size() == 2);
    // Check that the sample lies on the line x = y (since covariance has rank 1)
    // The transformation yields x = mu0 + z0*sqrt(2) + 0*z1, y = mu1 + z0*sqrt(2) + 0*z1
    // So x-y should equal mu0 - mu1 within numerical error.
    assert(std::abs((s2[0]-s2[1]) - (mu[0]-mu[1])) < 1e-9);

    // Test 4: invalid non-symmetric matrix should throw
    std::vector<std::vector<double>> bad = {{1.0, 2.0}, {0.0, 1.0}};
    bool threw = false;
    try {
        mvrnorm(mu, bad, engine);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 5: invalid negative eigenvalue (non-PSD) should throw
    std::vector<std::vector<double>> neg = {{-1.0, 0.0}, {0.0, 1.0}};
    threw = false;
    try {
        mvrnorm(mu, neg, engine);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 6: 3x3 diagonal with valid values
    std::vector<double> mu3 = {0.0, 0.0, 0.0};
    std::vector<std::vector<double>> Sigma3 = {{1.0,0,0},{0,2.0,0},{0,0,3.0}};
    auto s3 = mvrnorm(mu3, Sigma3, engine);
    assert(s3.size() == 3);

    // Test 7: 3x3 rank-deficient matrix (Cholesky fails, eigen fallback works)
    std::vector<std::vector<double>> Sigma3r = {{1.0,1.0,0.0},{1.0,1.0,0.0},{0.0,0.0,2.0}};
    auto s4 = mvrnorm(mu3, Sigma3r, engine);
    assert(s4.size() == 3);
    // Should have x-y = 0 up to error
    assert(std::abs(s4[0]-s4[1]) < 1e-9);

    // Test 8: dimension mismatch throws
    threw = false;
    try {
        std::vector<double> mu_wrong = {1.0};
        mvrnorm(mu_wrong, Sigma, engine);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    std::cout << "All tests passed." << std::endl;
    return 0;
}
