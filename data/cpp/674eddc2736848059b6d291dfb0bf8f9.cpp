/*
Write a standalone C++ function that simulates a fixed-point iteration on a square matrix. Given a matrix size `n`, a positive integer number of iterations `k`, and a scalar parameter `epsilon`, the function should initialize an `n x n` matrix `m` such that `m(i,j) = (i+j+1)/(n*n)` for all indices `i,j` in `[0,n)`, and an identity matrix `I` of the same size. Then, for each of `k` iterations, update `m` as `m = I + epsilon * (m + m*m)`, where `m*m` denotes standard matrix multiplication. The function must return the value of the top-left element `m(0,0)` after all iterations. Assume `n >= 1`, `k >= 1`, and `epsilon` is a non-negative double. The matrix type should be `Eigen::MatrixXd` (double precision). Handle the case where the product involves large values gracefully (no overflow checking required, but use double). The solution should use Eigen library headers and be `const`-correct where possible, though the returned value is a double.
*/

#include <Eigen/Core>

// Simulate the iterative matrix update and return the top-left element.
// n: matrix size (>=1), k: number of iterations (>=1), epsilon: scaling factor (>=0).
double fixedPointTopLeft(int n, int k, double epsilon) {
    Eigen::MatrixXd I = Eigen::MatrixXd::Identity(n, n);
    Eigen::MatrixXd m(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            m(i, j) = static_cast<double>(i + j + 1) / static_cast<double>(n * n);
        }
    }
    for (int iter = 0; iter < k; ++iter) {
        // Compute m = I + epsilon * (m + m*m)
        m = I + epsilon * (m + m * m);
    }
    return m(0, 0);
}

#include <cassert>
#include <cmath>

int main() {
    // n=1, k=1, epsilon=0 -> m = I + 0 = 1 (since m(0,0) initial 1/1=1, but then overwritten)
    assert(std::fabs(fixedPointTopLeft(1, 1, 0.0) - 1.0) < 1e-12);
    // n=1, k=1, epsilon=1 -> m = I + (m + m*m) = 1 + (1+1) = 3
    assert(std::fabs(fixedPointTopLeft(1, 1, 1.0) - 3.0) < 1e-12);
    // n=2, k=1, epsilon=0.0001 -> compute manually or via a simple reference
    double val = fixedPointTopLeft(2, 1, 0.0001);
    // Reference: initial m = [[0.25, 0.5],[0.75, 1.0]] (since n=2, n*n=4, (i+j+1)/4)
    // m*m = [[0.25*0.25+0.5*0.75, 0.25*0.5+0.5*1.0],[0.75*0.25+1.0*0.75, 0.75*0.5+1.0*1.0]] = [[0.4375, 0.625],[0.9375, 1.375]]
    // m + m*m = [[0.6875, 1.125],[1.6875, 2.375]]
    // epsilon * that = [[0.00006875, 0.0001125],[0.00016875, 0.0002375]]
    // + I = [[1.00006875, 0.0001125],[0.00016875, 1.0002375]]
    // so m(0,0) = 1.00006875
    assert(std::fabs(val - 1.00006875) < 1e-10);
    // n=2, k=2, epsilon=0 => after first iteration m=I, after second still I, so m(0,0)=1
    assert(std::fabs(fixedPointTopLeft(2, 2, 0.0) - 1.0) < 1e-12);
    // n=3, k=1, epsilon=1 -> just check positivity (no overflow)
    assert(fixedPointTopLeft(3, 1, 1.0) > 0.0);
    // n=3, k=10, epsilon=0.01 -> result should be finite
    double fin = fixedPointTopLeft(3, 10, 0.01);
    assert(std::isfinite(fin));
    // n=1, k=100, epsilon=0.5 -> iterate 1x1: m = 1 + 0.5*(m + m^2) starting m=1
    double val1 = 1.0;
    for (int i=0; i<100; ++i) val1 = 1.0 + 0.5*(val1 + val1*val1);
    assert(std::fabs(fixedPointTopLeft(1, 100, 0.5) - val1) < 1e-12);
    // n=4, k=1, epsilon=0.001 -> compare with a direct manual loop using Eigen (not needed, just sanity)
    assert(fixedPointTopLeft(4, 1, 0.001) > 0.999);
    return 0;
}

// The core algorithm is a straightforward iterative matrix update. Initialize the `n x n` matrix `m` with the formula given, using nested loops or Eigen's coefficient-wise operations. Initialize `I` as `Eigen::MatrixXd::Identity(n, n)`. Then for each iteration `t = 0` to `k-1`, compute the matrix product `m*m` (Eigen's `*` operator for `MatrixXd` performs standard matrix multiplication in `O(n^3)` time per iteration), then compute `m + m*m`, scale by `epsilon`, add `I`, and assign back to `m`. After all iterations, return `m(0,0)`. Edge cases: `n=1` works fine because matrix multiplication of 1x1 is just scalar multiplication; `epsilon=0` means `m` stays `I` after the first iteration, so result will be 1.0 for any `k >= 1` because `m = I + 0 = I`; for `k=1`, we do exactly one update. Time complexity: each iteration does one matrix multiplication `O(n^3)` and several `O(n^2)` additions/scales, so total `O(k * n^3)`. Space complexity: we need a few `n x n` matrices, so `O(n^2)` auxiliary space (plus the returned double). The solution uses `Eigen::MatrixXd` and requires linking with Eigen (header-only, but need `#include <Eigen/Core>` and `-I` path). No special error handling needed except ensuring `n` and `k` are positive.
