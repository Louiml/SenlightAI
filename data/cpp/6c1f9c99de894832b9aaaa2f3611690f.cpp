Write a C++ function `computeMatrixLog` that takes a constant reference to a 3x3 `Eigen::MatrixXd` (a fixed-size matrix of doubles) and returns its matrix logarithm as a `MatrixXd` using the Eigen `log()` member function from the `<unsupported/Eigen/MatrixFunctions>` header. The function must handle both general invertible matrices and the specific case of a rotation matrix. In your solution, also ensure that the input is a 3x3 matrix (you may use `assert` for this) and that the result is computed correctly for a rotation matrix whose eigenvalues are complex with magnitude 1. Your function should be `const`‑correct: it must not modify the input matrix. Provide an implementation that works for the standard case and is robust for matrices with non‑positive real eigenvalues where the matrix logarithm is multi‑valued (Eigen returns the principal branch). No `main` function is required in the solution; just the free function.
The matrix logarithm is defined as the inverse of the matrix exponential, and for diagonalizable matrices it can be computed via the eigendecomposition: if \(A = V \Lambda V^{-1}\), then \(\log(A) = V \log(\Lambda) V^{-1}\), where \(\log(\Lambda)\) applies the natural logarithm to each eigenvalue. For a rotation matrix like the one in the snippet, the eigenvalues are \(e^{\pm i\theta}\) and \(1\), so the principal logarithm yields a skew-symmetric matrix representing the rotation angle. Eigen’s `log()` function implements this via a Padé approximation or inverse scaling and squaring, which is numerically stable for matrices without eigenvalues on the negative real axis. Edge cases: if the matrix has an eigenvalue equal to 0, the logarithm is undefined (since log(0) is infinite), so the function should rely on Eigen to produce a NaN or assert. For the given rotation matrix, the result is the skew-symmetric matrix with angle \(\pi/4\) in the upper 2x2 block and zero elsewhere. Time complexity is \(O(n^3)\) for the matrix operations, and space complexity is \(O(n^2)\) for temporary matrices. The function is simple: just call `A.log()` but first assert that `A.rows()==3 && A.cols()==3` to meet the task specification.
#include <unsupported/Eigen/MatrixFunctions>
#include <cassert>
#include <Eigen/Dense>

/**
 * Compute the matrix logarithm of a 3x3 matrix.
 * 
 * Uses Eigen's built-in log() which returns the principal branch.
 * The input must be a 3x3 matrix; otherwise assertion fails.
 * The input is not modified.
 * 
 * @param A constant reference to a 3x3 double matrix.
 * @return MatrixXd containing the matrix logarithm of A.
 */
Eigen::MatrixXd computeMatrixLog(const Eigen::MatrixXd& A) {
    assert(A.rows() == 3 && A.cols() == 3 && "Input must be a 3x3 matrix");
    // Eigen's log() returns a MatrixXd of same size.
    return A.log();
}
#include <unsupported/Eigen/MatrixFunctions>
#include <iostream>
#include <cassert>
#include <cmath>
#include <Eigen/Dense>

// Declaration of the solution function (provided separately)
Eigen::MatrixXd computeMatrixLog(const Eigen::MatrixXd& A);

int main() {
    using std::sqrt;
    
    // Define a rotation matrix around z-axis by 45 degrees (pi/4)
    Eigen::MatrixXd A(3,3);
    A << 0.5*sqrt(2), -0.5*sqrt(2), 0,
         0.5*sqrt(2),  0.5*sqrt(2), 0,
         0,            0,           1;
    
    // Compute the matrix logarithm
    Eigen::MatrixXd result = computeMatrixLog(A);
    
    // Expected result: for a rotation matrix with angle theta=pi/4,
    // the logarithm should be the skew-symmetric matrix:
    // [0, -theta, 0; theta, 0, 0; 0, 0, 0] but with sign depending on convention.
    // Eigen returns the principal branch, which for this matrix gives:
    // [0, -0.785398, 0; 0.785398, 0, 0; 0, 0, 0]
    // We check with a tolerance.
    
    double tol = 1e-12;
    
    // Check all entries manually (since comparing matrices with == is not allowed)
    // Using a small tolerance for floating point comparison.
    assert(std::abs(result(0,0) - 0.0) < tol);
    assert(std::abs(result(0,1) - (-0.7853981633974483)) < tol);
    assert(std::abs(result(0,2) - 0.0) < tol);
    assert(std::abs(result(1,0) - 0.7853981633974483) < tol);
    assert(std::abs(result(1,1) - 0.0) < tol);
    assert(std::abs(result(1,2) - 0.0) < tol);
    assert(std::abs(result(2,0) - 0.0) < tol);
    assert(std::abs(result(2,1) - 0.0) < tol);
    assert(std::abs(result(2,2) - 0.0) < tol);
    
    // Test an identity matrix (log(I) = 0)
    Eigen::MatrixXd I = Eigen::MatrixXd::Identity(3,3);
    Eigen::MatrixXd logI = computeMatrixLog(I);
    for (int i=0; i<3; ++i)
        for (int j=0; j<3; ++j)
            assert(std::abs(logI(i,j) - (i==j?0.0:0.0)) < tol);
    
    // Test a diagonal matrix with positive eigenvalues
    Eigen::MatrixXd D(3,3);
    D << 2.0, 0, 0,
         0, 3.0, 0,
         0, 0, 4.0;
    Eigen::MatrixXd logD = computeMatrixLog(D);
    assert(std::abs(logD(0,0) - std::log(2.0)) < tol);
    assert(std::abs(logD(1,1) - std::log(3.0)) < tol);
    assert(std::abs(logD(2,2) - std::log(4.0)) < tol);
    
    // Test that a non-3x3 matrix would fail (but we cannot assert in a test easily)
    // We skip that to keep the test clean; the assert in the function handles it.
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
