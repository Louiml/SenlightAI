Write a C++ function that takes a 3x3 matrix of `float` values (represented as `Eigen::Matrix3f`) and a 3x2 matrix of `float` values (represented as `Eigen::Matrix<float,3,2>`) and returns a 3x2 matrix that solves the linear system `A * X = B` using full-pivoting LU decomposition. The function must handle the case where the system is singular or nearly singular gracefully: in such cases, return a matrix filled with `NaN` values. The input matrices are guaranteed to have the correct dimensions, but the coefficient matrix may be singular (e.g., rows that are linearly dependent). Use `Eigen`'s `fullPivLu()` solver.
// The problem is a standard linear solve: given `A` (3x3) and `B` (3x2), find `X` (3x2) such that `A*X = B`. This is equivalent to solving two independent linear systems, one for each column of `B`. The main algorithm is to use Eigen's `fullPivLu()` decomposition, which performs LU decomposition with full row and column pivoting. This method is robust for rank-deficient matrices and handles singular systems by providing an `isInvertible()` check. If `A` is invertible, the solve is straightforward using `.solve(B)`. If `A` is singular (rank < 3), the decomposition's `isInvertible()` returns false, and we must handle it by returning a matrix filled with `NaN` to avoid undefined behavior. Edge cases include: (1) a perfectly singular matrix (e.g., all rows identical), (2) a matrix with determinant exactly zero but numerically ill-conditioned, and (3) a matrix that is nearly singular but technically invertible—the solver may produce large values, which is acceptable. Time complexity is dominated by the LU decomposition, which is \(O(n^3)\) for an n×n matrix (here n=3, so constant time but general complexity is cubic). Space complexity is \(O(1)\) for the fixed 3×3 and 3×2 matrices, aside from the temporary solver object.
#include <Eigen/Dense>
#include <cmath>

// Solves A * X = B for 3x3 A and 3x2 B using full-pivoting LU.
// Returns a 3x2 matrix X. If A is singular, returns a matrix filled with NaN.
Eigen::Matrix<float, 3, 2> solveFullPivLu(const Eigen::Matrix3f& A, const Eigen::Matrix<float, 3, 2>& B) {
    Eigen::FullPivLU<Eigen::Matrix3f> lu(A);
    if (!lu.isInvertible()) {
        return Eigen::Matrix<float, 3, 2>::Constant(std::numeric_limits<float>::quiet_NaN());
    }
    return lu.solve(B);
}
#include <cassert>
#include <cmath>
#include <Eigen/Dense>

// Forward declaration of the solution function (or include the solution header).
Eigen::Matrix<float, 3, 2> solveFullPivLu(const Eigen::Matrix3f& A, const Eigen::Matrix<float, 3, 2>& B);

int main() {
    // Test 1: Non-singular system from the snippet.
    Eigen::Matrix3f A1;
    A1 << 1, 2, 3, 4, 5, 6, 7, 8, 10;
    Eigen::Matrix<float, 3, 2> B1;
    B1 << 3, 1, 3, 1, 4, 1;
    Eigen::Matrix<float, 3, 2> X1 = solveFullPivLu(A1, B1);
    // Verify residual: A1 * X1 ≈ B1.
    Eigen::Matrix<float, 3, 2> R1 = A1 * X1 - B1;
    assert(R1.cwiseAbs().maxCoeff() < 1e-4f);

    // Test 2: Identity matrix with simple B.
    Eigen::Matrix3f A2 = Eigen::Matrix3f::Identity();
    Eigen::Matrix<float, 3, 2> B2;
    B2 << 5, -1, 2, 0, 3, 7;
    Eigen::Matrix<float, 3, 2> X2 = solveFullPivLu(A2, B2);
    assert((X2 - B2).cwiseAbs().maxCoeff() < 1e-6f);

    // Test 3: Singular matrix (all rows equal) -> result should be all NaN.
    Eigen::Matrix3f A3;
    A3 << 1, 2, 3, 1, 2, 3, 1, 2, 3;
    Eigen::Matrix<float, 3, 2> B3;
    B3 << 1, 0, 2, 1, 3, 0;
    Eigen::Matrix<float, 3, 2> X3 = solveFullPivLu(A3, B3);
    assert(X3.allFinite() == false); // Check at least one NaN exists
    // Verify every entry is NaN (not required, but we test that it's non-finite).

    // Test 4: Diagonal matrix with distinct values.
    Eigen::Matrix3f A4;
    A4 << 2, 0, 0, 0, 4, 0, 0, 0, 8;
    Eigen::Matrix<float, 3, 2> B4;
    B4 << 6, 2, 8, 4, 16, 8;
    Eigen::Matrix<float, 3, 2> X4 = solveFullPivLu(A4, B4);
    Eigen::Matrix<float, 3, 2> expected4;
    expected4 << 3, 1, 2, 1, 2, 1;
    assert((X4 - expected4).cwiseAbs().maxCoeff() < 1e-6f);

    // Test 5: Nearly singular but invertible (slightly perturbed) -> should still solve.
    Eigen::Matrix3f A5;
    A5 << 1, 2, 3, 4, 5, 6, 7, 8, 10.0001f; // Slight perturbation
    Eigen::Matrix<float, 3, 2> B5;
    B5 << 1, 1, 1, 1, 1, 1;
    Eigen::Matrix<float, 3, 2> X5 = solveFullPivLu(A5, B5);
    assert((A5 * X5 - B5).cwiseAbs().maxCoeff() < 1e-3f); // Residual tolerance relaxed
    
    return 0;
}
