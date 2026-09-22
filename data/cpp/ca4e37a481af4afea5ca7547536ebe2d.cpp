Write a C++ function named `dominantEigenvalueOfOnesMatrix` that, given a positive integer `n`, constructs an `n x n` matrix where every entry is `1` (using `Eigen::MatrixXd`), computes its eigenvalues via `Eigen::EigenSolver`, and returns the eigenvalue with the largest magnitude (absolute value) as a `double`. For an `n x n` all-ones matrix, the eigenvalues are `n` (with multiplicity 1) and `0` (with multiplicity `n-1`). Therefore, the function must return exactly `n` as the dominant eigenvalue (which is also equal to the spectral radius). Handle edge cases where `n == 1` (the single eigenvalue is `1`) and `n` up to at least 1000 (the matrix is dense but small enough for direct eigenvalue computation). The function must be `const`-correct, use `Eigen::MatrixXd` and `Eigen::EigenSolver<MatrixXd>`, and must not include a `main` function. The returned value should be within a tolerance of `1e-9` of the exact integer `n` due to floating-point arithmetic in the solver.

#include <cassert>
#include <cmath>

// The solution function is declared here (in a real project would be included).
double dominantEigenvalueOfOnesMatrix(int n);

int main() {
    // n=1: matrix [1] => eigenvalue 1
    assert(std::fabs(dominantEigenvalueOfOnesMatrix(1) - 1.0) < 1e-9);
    
    // n=2: matrix [[1,1],[1,1]] => eigenvalues 2 and 0 => dominant 2
    assert(std::fabs(dominantEigenvalueOfOnesMatrix(2) - 2.0) < 1e-9);
    
    // n=3: as in the snippet, 3x3 ones => eigenvalues 3,0,0 => dominant 3
    assert(std::fabs(dominantEigenvalueOfOnesMatrix(3) - 3.0) < 1e-9);
    
    // n=5: eigenvalues 5,0,0,0,0 => dominant 5
    assert(std::fabs(dominantEigenvalueOfOnesMatrix(5) - 5.0) < 1e-9);
    
    // n=10: dominant should be 10
    assert(std::fabs(dominantEigenvalueOfOnesMatrix(10) - 10.0) < 1e-9);
    
    // n=100: larger dense matrix, still dominant 100
    assert(std::fabs(dominantEigenvalueOfOnesMatrix(100) - 100.0) < 1e-9);
    
    // n=1000: performance test, should be 1000 (within numerical tolerance)
    assert(std::fabs(dominantEigenvalueOfOnesMatrix(1000) - 1000.0) < 1e-6);
    
    // Check that the function returns a non-negative value (by construction).
    double result = dominantEigenvalueOfOnesMatrix(7);
    assert(result > 0.0);
    
    return 0;
}

#include <Eigen/Dense>
#include <complex>
#include <cmath>

// Return the dominant (largest magnitude) eigenvalue of an n x n all-ones matrix.
// For such a matrix, eigenvalues are n (once) and 0 (n-1 times), so dominant is n.
double dominantEigenvalueOfOnesMatrix(int n) {
    // Construct an n x n matrix filled with ones.
    Eigen::MatrixXd ones = Eigen::MatrixXd::Ones(n, n);
    
    // Compute eigenvalues using the dense eigen solver.
    Eigen::EigenSolver<Eigen::MatrixXd> solver(ones);
    Eigen::VectorXcd eigenvalues = solver.eigenvalues();
    
    // Find the eigenvalue with the largest magnitude.
    double maxMagnitude = 0.0;
    for (int i = 0; i < eigenvalues.size(); ++i) {
        double magnitude = std::abs(eigenvalues(i));
        if (magnitude > maxMagnitude) {
            maxMagnitude = magnitude;
        }
    }
    
    return maxMagnitude;
}

// The all-ones matrix `J = 1 * 1^T` (where `1` is a column vector of ones) has rank 1. Its eigenvectors: the vector `1` has eigenvalue `n` (since `J * 1 = n * 1`), and any vector orthogonal to `1` is in the null space, giving eigenvalue `0` with multiplicity `n-1`. To compute numerically, we use `Eigen::EigenSolver<MatrixXd>` on the dense matrix, which returns complex eigenvalues (as `VectorXcd`) because the solver is general; however, for this symmetric real matrix the eigenvalues are real (within numerical precision). We iterate over all eigenvalues, take the absolute value (magnitude) of each complex value, and keep the maximum. For `n == 1`, the matrix is `[1]` and the eigenvalue is `1`. The time complexity is `O(n^3)` due to the dense eigenvalue solver, and space complexity is `O(n^2)` for the matrix plus `O(n^2)` for solver internals. Edge cases: numerical precision may produce values like `n-1e-15` or `1e-16` for zeros; we only need the dominant one, which should be close to `n`. We also guard against `n <= 0` by treating `n == 0` as invalid but since the task says positive integer, we assume `n >= 1`. The function returns `double`, and the test uses `assert(fabs(result - expected) < 1e-9)`.
