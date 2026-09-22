Write a C++ function that takes two Eigen dense matrices as input: a square coefficient matrix `A` and a right-hand side matrix `B` (both of arbitrary but compatible dimensions, meaning `A` must be square and `B` must have the same number of rows as `A`). The function should solve the linear system `A * X = B` using complete pivoting LU decomposition (via `fullPivLu()`) and return the solution matrix `X`. Additionally, the function must compute the relative error of the solution, defined as `(A*X - B).norm() / B.norm()`, where `.norm()` is the L2 (Frobenius) norm. If the matrix `A` is singular or the system is ill-conditioned (i.e., the decomposition reports `!isInvertible()`), the function should throw a `std::runtime_error` with a descriptive message. The function must be `const`-correct, accept both arguments by `const` reference, and return a `std::pair<MatrixXd, double>` containing the solution matrix and the relative error. Handle edge cases where `B` has zero rows (empty matrix) gracefully — in that case, return an empty solution and relative error of 0.0. Assume the inputs are valid (no mismatched dimensions in the caller) for simplicity, but the function should still verify that `A` is square and throw a `std::invalid_argument` if not.

#include <Eigen/Dense>
#include <stdexcept>
#include <cassert>
#include <cmath>

// Include the solution function here (or include a header)
// For self-contained test, paste the solveWithRelativeError function above.

int main() {
    // 1. Simple 2x2 system: A = [[2,0],[0,3]], B = [[4],[9]] => X = [[2],[3]]
    Eigen::MatrixXd A1(2,2);
    A1 << 2, 0, 0, 3;
    Eigen::MatrixXd B1(2,1);
    B1 << 4, 9;
    auto res1 = solveWithRelativeError(A1, B1);
    assert((res1.first - Eigen::Vector2d(2.0, 3.0)).norm() < 1e-12);
    assert(res1.second < 1e-12);

    // 2. Identity matrix with 3 right-hand sides
    Eigen::MatrixXd A2 = Eigen::MatrixXd::Identity(3,3);
    Eigen::MatrixXd B2(3,2);
    B2 << 1, 2, 3, 4, 5, 6;
    auto res2 = solveWithRelativeError(A2, B2);
    assert((res2.first - B2).norm() < 1e-12);
    assert(res2.second < 1e-12);

    // 3. Diagonal with different scales
    Eigen::MatrixXd A3(2,2);
    A3 << 10, 0, 0, 0.1;
    Eigen::MatrixXd B3(2,1);
    B3 << 20, 0.2;
    auto res3 = solveWithRelativeError(A3, B3);
    assert(std::abs(res3.first(0,0) - 2.0) < 1e-12);
    assert(std::abs(res3.first(1,0) - 2.0) < 1e-12);
    assert(res3.second < 1e-12);

    // 4. Singular matrix should throw
    Eigen::MatrixXd A4(2,2);
    A4 << 1, 2, 2, 4;
    Eigen::MatrixXd B4(2,1);
    B4 << 3, 6;
    bool threw = false;
    try {
        solveWithRelativeError(A4, B4);
    } catch (const std::runtime_error&) {
        threw = true;
    }
    assert(threw);

    // 5. Non-square A throws invalid_argument
    Eigen::MatrixXd A5(2,3);
    A5.setRandom();
    Eigen::MatrixXd B5(2,1);
    B5.setRandom();
    bool threwInvalid = false;
    try {
        solveWithRelativeError(A5, B5);
    } catch (const std::invalid_argument&) {
        threwInvalid = true;
    }
    assert(threwInvalid);

    // 6. Empty B (0 rows) returns empty solution and 0.0 error
    Eigen::MatrixXd A6 = Eigen::MatrixXd::Identity(2,2);
    Eigen::MatrixXd B6(0,3);
    auto res6 = solveWithRelativeError(A6, B6);
    assert(res6.first.rows() == 0 && res6.first.cols() == 3);
    assert(res6.second == 0.0);

    // 7. Large random system, check relative error is very small
    Eigen::MatrixXd A7 = Eigen::MatrixXd::Random(5,5);
    Eigen::MatrixXd B7 = Eigen::MatrixXd::Random(5,4);
    auto res7 = solveWithRelativeError(A7, B7);
    assert(res7.second < 1e-10);

    return 0;
}

#include <Eigen/Dense>
#include <stdexcept>
#include <utility>

// Solves the linear system A*X = B using complete pivoting LU decomposition.
// Returns a pair containing the solution matrix X and the relative error.
// Throws std::invalid_argument if A is not square.
// Throws std::runtime_error if A is singular.
std::pair<Eigen::MatrixXd, double> solveWithRelativeError(
    const Eigen::MatrixXd& A,
    const Eigen::MatrixXd& B)
{
    // Verify A is square
    if (A.rows() != A.cols()) {
        throw std::invalid_argument("Coefficient matrix A must be square.");
    }

    // Handle empty B (zero rows)
    if (B.rows() == 0) {
        return {Eigen::MatrixXd(0, B.cols()), 0.0};
    }

    // Perform LU decomposition with complete pivoting
    Eigen::FullPivLU<Eigen::MatrixXd> lu(A);

    // Check invertibility; throw if singular
    if (!lu.isInvertible()) {
        throw std::runtime_error("Matrix A is singular; system cannot be solved.");
    }

    // Solve the system
    Eigen::MatrixXd X = lu.solve(B);

    // Compute relative error: ||A*X - B|| / ||B||
    double residualNorm = (A * X - B).norm();
    double bNorm = B.norm();
    double relativeError = residualNorm / bNorm;

    return {X, relativeError};
}

// The main algorithm is straightforward: use Eigen's `fullPivLu()` on the coefficient matrix `A`. This decomposition is the most robust for general dense matrices because it performs complete pivoting, detecting exact singularity. After obtaining the decomposition object, we check `isInvertible()`. If false, the matrix is singular (within numerical precision) and we throw a `std::runtime_error`. Otherwise, we call `.solve(B)` to obtain the solution matrix `X`. Then we compute the residual `R = A*X - B` (using matrix multiplication and subtraction) and its L2 norm via `.norm()`. The relative error is `R.norm() / B.norm()`. If `B` is empty (zero rows), both norms are zero and the relative error is undefined; we handle that by returning a zero-size solution and 0.0 relative error. For a non-empty `B`, if `B.norm()` is extremely small, the relative error may be huge or NaN, but that is acceptable as it reflects an ill-conditioned problem. Time complexity: LU decomposition with complete pivoting is O(n^3) for an n×n matrix, and solving for multiple right-hand sides (B has k columns) adds O(n^2 * k). Space complexity: O(n^2) for the decomposition and solution matrices. Edge cases: singular `A`, empty `B`, non-square `A` (throw `invalid_argument`), and near-singular matrices where `isInvertible()` may still be true but the relative error is large — we do not treat that as an error since the task only requires singularity detection.
