// Write a C++ function that takes a square matrix `A` of type `Eigen::MatrixXd` and returns the matrix exponential `exp(A)` computed via the `Eigen::MatrixFunctions` library’s `matrixFunction` method with a complex exponential callback. The function must handle any square matrix (including non-diagonalizable ones) and return the result as an `Eigen::MatrixXcd` (a complex-double matrix). The function should be named `computeMatrixExponential` and must preserve the original matrix (i.e., take the input by const reference). Additionally, ensure the code compiles with the necessary Eigen headers and uses `const` correctness throughout. The task is to implement the function only, without a main routine.

The solution uses Eigen’s built-in matrix function machinery, which computes the matrix exponential via the Padé approximation or scaling-and-squaring method internally, depending on the matrix size and conditioning. The `matrixFunction` method requires a callable that maps a complex scalar to another complex scalar; here we provide a lambda or free function that returns `std::exp(x)` for a complex input `x`. The method returns a `MatrixXcd` because the exponential of a real matrix can be complex (e.g., for rotation matrices). Edge cases: the zero matrix returns identity; diagonal matrices return the exponential of each diagonal entry; non-diagonalizable matrices are handled correctly by the library’s internal algorithms (it uses a Schur decomposition followed by solving a Sylvester equation for triangular blocks). Time complexity is typically \(O(n^3)\) for an \(n \times n\) matrix due to the Schur decomposition and matrix multiplications, and space complexity is \(O(n^2)\) for the temporary matrices. The input matrix is taken by const reference to avoid copies and ensure the original is unchanged.

#include <unsupported/Eigen/MatrixFunctions>
#include <complex>

// Compute the matrix exponential of a square matrix A.
// Returns a complex matrix because exp(A) may have complex entries even for real A.
Eigen::MatrixXcd computeMatrixExponential(const Eigen::MatrixXd& A) {
    // Define a callable that computes the complex exponential.
    auto exp_complex = [](std::complex<double> x, int) -> std::complex<double> {
        return std::exp(x);
    };
    // Use Eigen's matrixFunction which internally handles the algorithm.
    Eigen::MatrixXcd result;
    A.matrixFunction(exp_complex, result);
    return result;
}

#include <unsupported/Eigen/MatrixFunctions>
#include <cassert>
#include <cmath>
#include <complex>

// Declare the solution function (normally from header, but here for self-contained test).
Eigen::MatrixXcd computeMatrixExponential(const Eigen::MatrixXd& A);

int main() {
    const double pi = std::acos(-1.0);

    // Test 1: Zero matrix -> identity
    Eigen::MatrixXd zero(2,2);
    zero.setZero();
    Eigen::MatrixXcd expZero = computeMatrixExponential(zero);
    Eigen::MatrixXcd expectedZero = Eigen::MatrixXcd::Identity(2,2);
    assert(expZero.isApprox(expectedZero));

    // Test 2: Diagonal matrix -> exp of diagonal entries
    Eigen::MatrixXd diag(2,2);
    diag << 1.0, 0.0,
            0.0, -2.0;
    Eigen::MatrixXcd expDiag = computeMatrixExponential(diag);
    Eigen::MatrixXcd expectedDiag(2,2);
    expectedDiag << std::exp(1.0), 0.0,
                    0.0, std::exp(-2.0);
    assert(expDiag.isApprox(expectedDiag));

    // Test 3: 2x2 rotation matrix (non-diagonalizable over reals) -> complex result
    Eigen::MatrixXd rot(2,2);
    rot << 0.0, -pi/2,
           pi/2, 0.0;
    Eigen::MatrixXcd expRot = computeMatrixExponential(rot);
    // Expected: cos(pi/2)=0, sin(pi/2)=1 -> [[0, -1],[1, 0]]*? Actually exp([[0,-a],[a,0]]) = [[cos a, -sin a],[sin a, cos a]]
    Eigen::MatrixXcd expectedRot(2,2);
    expectedRot << std::cos(pi/2), -std::sin(pi/2),
                   std::sin(pi/2),  std::cos(pi/2);
    assert(expRot.isApprox(expectedRot));

    // Test 4: Identity matrix -> exp(1) * identity
    Eigen::MatrixXd id(3,3);
    id.setIdentity();
    Eigen::MatrixXcd expId = computeMatrixExponential(id);
    Eigen::MatrixXcd expectedId = std::exp(1.0) * Eigen::MatrixXcd::Identity(3,3);
    assert(expId.isApprox(expectedId));

    // Test 5: Non-diagonalizable Jordan block [[0,1],[0,0]] -> [[1,1],[0,1]]
    Eigen::MatrixXd jordan(2,2);
    jordan << 0.0, 1.0,
              0.0, 0.0;
    Eigen::MatrixXcd expJordan = computeMatrixExponential(jordan);
    Eigen::MatrixXcd expectedJordan(2,2);
    expectedJordan << 1.0, 1.0,
                      0.0, 1.0;
    assert(expJordan.isApprox(expectedJordan));

    // Test 6: Random 3x3 matrix, verify by comparing with a known approximation using series (to 5 terms)
    Eigen::MatrixXd A(3,3);
    A << 0.1, 0.2, -0.3,
         0.4, -0.5, 0.6,
         -0.7, 0.8, 0.9;
    Eigen::MatrixXcd expA = computeMatrixExponential(A);
    // Use series approximation: sum_{k=0}^{5} A^k / k!
    Eigen::MatrixXcd sum = Eigen::MatrixXcd::Identity(3,3);
    Eigen::MatrixXcd term = Eigen::MatrixXcd::Identity(3,3);
    Eigen::MatrixXcd A_cd = A.cast<std::complex<double>>();
    double factorial = 1.0;
    for (int k = 1; k <= 5; ++k) {
        term = term * A_cd / static_cast<double>(k);
        sum += term;
    }
    assert(expA.isApprox(sum, 1e-3)); // loose tolerance due to truncated series

    // Test 7: Original matrix unchanged (const correctness)
    Eigen::MatrixXd original = A;
    computeMatrixExponential(A);
    assert(A == original);
}
