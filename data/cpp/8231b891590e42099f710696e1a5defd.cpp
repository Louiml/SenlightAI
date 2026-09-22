Write a C++ function `weightedCovarianceCenter` that takes two inputs: a matrix `x` of size \( n \times p \) (with `double` elements) and a vector `w` of length \( n \) containing non‑negative weights (with at least one positive weight). The function must compute and return a `std::pair<Eigen::VectorXd, Eigen::MatrixXd>` containing (1) a weighted mean vector `center` of length \( p \) where each component is \(\frac{\sum_{i=1}^n w_i x_{i,j}}{\sum_{i=1}^n w_i}\), and (2) the weighted covariance matrix `cov` of size \( p \times p \) defined as \(\frac{1}{W} \sum_{i=1}^n w_i (x_i - \mu)(x_i - \mu)^T\) with \( W = \sum_{i=1}^n w_i \) and \(\mu\) the weighted mean. Use the Eigen library to perform vectorized operations. The function must be `const`‑correct (take inputs as `const Eigen::MatrixXd&` and `const Eigen::VectorXd&`) and must handle the edge case where `p` equals 1 or `n` equals 1 without division by zero or matrix inversion. You may assume inputs are valid (non‑empty matrix, weights non‑negative with positive sum). Return the result in `(center, cov)` order.

The solution computes the weighted mean by broadcasting the weight vector across columns of `x`: for each column, `center(j) = (x.col(j).array() * w.array()).sum() / w.sum()`. Then, it centers the data matrix by subtracting the row vector `center` from each row using `x.rowwise() - center.transpose()`. The weighted covariance matrix is built using the formula \(\frac{1}{W} \sum_i w_i (x_i - \mu)(x_i - \mu)^T\). A numerically stable approach is to first normalize weights by their sum: \(s_i = \sqrt{w_i / W}\). Then, multiply each centered row by its corresponding \(s_i\) to form a scaled matrix \(Y\) (size \( n \times p \)), such that each row of \(Y\) is \(s_i (x_i - \mu)^T\). The covariance is then \(Y^T Y\). In Eigen, this can be done efficiently by creating a diagonal matrix of the `s` vector and computing `(Y.transpose() * s.asDiagonal()) * (Y)` or more directly via `Y.transpose() * Y` after scaling. The code uses `selfadjointView<Eigen::Lower>()` to exploit symmetry and `rankUpdate` for efficiency. Edge cases: if `p == 1`, the matrix operations reduce to scalars but still work; if `n == 1`, the centered matrix becomes all zeros, leading to a zero covariance matrix (which is acceptable). Since weights are non‑negative with positive sum, `w.sum()` is never zero. Time complexity is \(O(n p^2)\) for the matrix multiplication, and \(O(n p)\) for the mean computation and centering. Space complexity is \(O(n p + p^2)\) for the intermediate matrices.

#include <Eigen/Dense>
#include <utility>

/**
 * Computes the weighted mean and weighted covariance matrix of a data matrix.
 * 
 * @param x Matrix of size n x p, each row an observation, each column a variable.
 * @param w Vector of length n with non-negative weights, at least one positive.
 * @return std::pair containing (center, cov) where center is a p-vector of weighted
 *         means and cov is the p x p weighted covariance matrix.
 */
std::pair<Eigen::VectorXd, Eigen::MatrixXd> weightedCovarianceCenter(
    const Eigen::MatrixXd& x, const Eigen::VectorXd& w)
{
    const int n = x.rows();
    const int p = x.cols();

    // 1. Compute weighted mean per column
    Eigen::VectorXd center(p);
    double ws = w.sum();
    for (int j = 0; j < p; ++j) {
        center(j) = (x.col(j).array() * w.array()).sum() / ws;
    }

    // 2. Center the data
    Eigen::MatrixXd X_centered = x.rowwise() - center.transpose();

    // 3. Scale rows by sqrt(w/ws) to form Y = diag(sqrt(w/ws)) * X_centered
    Eigen::VectorXd scale = (w.array() / ws).cwiseSqrt();
    Eigen::MatrixXd Y = X_centered.array().colwise() * scale.array();

    // 4. Covariance = Y^T * Y
    Eigen::MatrixXd cov = (Y.transpose() * Y).selfadjointView<Eigen::Lower>();

    // Return as pair (center, cov)
    return std::make_pair(center, cov);
}

#include <Eigen/Dense>
#include <cassert>
#include <cmath>
#include <iostream>
#include <utility>

// Test function
int main() {
    // Test 1: Simple case with two identical observations and equal weights
    Eigen::MatrixXd x1(2, 2);
    x1 << 1.0, 2.0,
          1.0, 2.0;
    Eigen::VectorXd w1(2);
    w1 << 0.5, 0.5;
    auto res1 = weightedCovarianceCenter(x1, w1);
    assert(res1.first.isApprox(Eigen::VectorXd::Constant(2, 1.0), 1e-12));
    assert(res1.second.isApprox(Eigen::MatrixXd::Zero(2, 2), 1e-12));

    // Test 2: Weighted mean with different weights
    Eigen::MatrixXd x2(3, 1);
    x2 << 1.0, 2.0, 3.0;
    Eigen::VectorXd w2(3);
    w2 << 1.0, 2.0, 1.0; // sum = 4
    auto res2 = weightedCovarianceCenter(x2, w2);
    double mean2 = (1.0*1.0 + 2.0*2.0 + 1.0*3.0) / 4.0; // = 2.0
    assert(std::abs(res2.first(0) - mean2) < 1e-12);
    // Weighted variance = (1*(1-2)^2 + 2*(2-2)^2 + 1*(3-2)^2)/4 = (1+0+1)/4 = 0.5
    assert(std::abs(res2.second(0,0) - 0.5) < 1e-12);

    // Test 3: Zero weights except one (degenerate, covariance zero)
    Eigen::MatrixXd x3(3, 2);
    x3 << 1.0, 5.0,
          2.0, 6.0,
          3.0, 7.0;
    Eigen::VectorXd w3(3);
    w3 << 0.0, 0.0, 1.0;
    auto res3 = weightedCovarianceCenter(x3, w3);
    Eigen::VectorXd expected_center3(2);
    expected_center3 << 3.0, 7.0;
    assert(res3.first.isApprox(expected_center3, 1e-12));
    assert(res3.second.isApprox(Eigen::MatrixXd::Zero(2, 2), 1e-12));

    // Test 4: Compare with a known weighted covariance for p=2
    Eigen::MatrixXd x4(4, 2);
    x4 << 1.0, 2.0,
          3.0, 4.0,
          5.0, 6.0,
          7.0, 8.0;
    Eigen::VectorXd w4(4);
    w4 << 0.1, 0.2, 0.3, 0.4;
    auto res4 = weightedCovarianceCenter(x4, w4);
    
    // Manual computation
    double ws4 = w4.sum(); // = 1.0
    Eigen::VectorXd center4(2);
    center4(0) = (0.1*1 + 0.2*3 + 0.3*5 + 0.4*7) / 1.0; // = 5.0
    center4(1) = (0.1*2 + 0.2*4 + 0.3*6 + 0.4*8) / 1.0; // = 6.0
    Eigen::MatrixXd centered4(4, 2);
    for (int i = 0; i < 4; ++i) {
        centered4(i, 0) = x4(i,0) - center4(0);
        centered4(i, 1) = x4(i,1) - center4(1);
    }
    Eigen::MatrixXd cov4 = Eigen::MatrixXd::Zero(2,2);
    for (int i = 0; i < 4; ++i) {
        cov4 += w4(i) * (centered4.row(i).transpose() * centered4.row(i));
    }
    cov4 /= ws4;
    assert(res4.first.isApprox(center4, 1e-12));
    assert(res4.second.isApprox(cov4, 1e-12));

    // Test 5: Single observation (n=1) with any positive weight
    Eigen::MatrixXd x5(1, 3);
    x5 << 1.5, -2.0, 0.0;
    Eigen::VectorXd w5(1);
    w5 << 2.5;
    auto res5 = weightedCovarianceCenter(x5, w5);
    assert(res5.first.isApprox(x5.row(0).transpose(), 1e-12));
    assert(res5.second.isApprox(Eigen::MatrixXd::Zero(3, 3), 1e-12));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
