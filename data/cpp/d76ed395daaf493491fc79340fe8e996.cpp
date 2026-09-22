/*
Write a standalone C++ function named `solveLeastSquares` that takes two matrix arguments, `A` (a fixed-size `Eigen::Matrix<float, 2, 3>`) and `B` (a fixed-size `Eigen::Matrix2f`), and returns a `bool`. Inside the function, attempt to solve the linear system `A * X = B` for an unknown `X` of size `Eigen::Matrix<float, 3, 2>` using full pivoting LU decomposition. If the system is consistent (i.e., `(A * X).isApprox(B)` evaluates to true), return `true`; otherwise return `false`. Additionally, output both the input matrices, the attempted solution (if any), and a message indicating whether a solution exists. The function must be `const`-correct (take inputs by `const&`), include necessary Eigen headers, and not contain a `main` function. Note: The system may be overdetermined (3 unknowns for 2 equations) or have no exact solution, so the function must handle the case where no exact solution exists.
*/
#include <Eigen/Dense>
#include <iostream>

// Solves A * X = B using full pivoting LU. Returns true if an exact solution exists.
bool solveLeastSquares(const Eigen::Matrix<float, 2, 3>& A,
                       const Eigen::Matrix2f& B) {
    // Print inputs for clarity.
    std::cout << "Here is the matrix A (2x3):\n" << A << std::endl;
    std::cout << "Here is the matrix B (2x2):\n" << B << std::endl;

    // Compute a candidate solution (least-squares if inconsistent).
    Eigen::Matrix<float, 3, 2> X = A.fullPivLu().solve(B);

    // Check if the candidate actually satisfies A*X = B.
    bool has_solution = (A * X).isApprox(B);

    if (has_solution) {
        std::cout << "Here is a solution X to the equation A*X = B:\n"
                  << X << std::endl;
    } else {
        std::cout << "The equation A*X = B does not have any exact solution.\n";
    }

    return has_solution;
}
#include <Eigen/Dense>
#include <cassert>
#include <iostream>

// Declare the function to test (as in the solution).
bool solveLeastSquares(const Eigen::Matrix<float, 2, 3>& A,
                       const Eigen::Matrix2f& B);

int main() {
    // Test 1: A has full row rank (rank 2) and the system is consistent.
    // Choose A and B such that X = [[1,0],[0,1],[0,0]] is a solution.
    Eigen::Matrix<float, 2, 3> A1;
    A1 << 1, 0, 0,
          0, 1, 0;
    Eigen::Matrix2f B1 = Eigen::Matrix2f::Identity();
    assert(solveLeastSquares(A1, B1) == true);

    // Test 2: A has rank 1, but B is not in the column space → inconsistent.
    Eigen::Matrix<float, 2, 3> A2;
    A2 << 1, 0, 0,
          0, 0, 0;  // Second row is zero, so rank is 1.
    Eigen::Matrix2f B2;
    B2 << 1, 0,
          0, 1;   // Not in col space; second row nonzero requirement.
    assert(solveLeastSquares(A2, B2) == false);

    // Test 3: A is zero matrix (rank 0), B is zero → consistent.
    Eigen::Matrix<float, 2, 3> A3 = Eigen::Matrix<float, 2, 3>::Zero();
    Eigen::Matrix2f B3 = Eigen::Matrix2f::Zero();
    assert(solveLeastSquares(A3, B3) == true);

    // Test 4: A is zero, B non-zero → inconsistent.
    Eigen::Matrix2f B4;
    B4 << 0, 0,
          0, 1;
    assert(solveLeastSquares(A3, B4) == false);

    // Test 5: A is square-ish but overdetermined (3 col, 2 rows) with a random consistent system.
    // Construct A and X, then compute B = A*X.
    Eigen::Matrix<float, 2, 3> A5;
    A5 << 2, 0, 1,
          0, 3, 2;
    Eigen::Matrix<float, 3, 2> X_true;
    X_true << 1, 0,
              0, 1,
              1, 1;
    Eigen::Matrix2f B5 = A5 * X_true;
    assert(solveLeastSquares(A5, B5) == true);

    // Test 6: Slightly perturb B from a consistent system to make it inconsistent.
    Eigen::Matrix2f B6 = B5;
    B6(0,0) += 0.1f;  // Perturbation
    // Since isApprox uses a tolerance, this may still be considered consistent if tolerance is large.
    // For a clear inconsistent case, use a larger perturbation.
    B6(0,0) += 100.0f;
    assert(solveLeastSquares(A5, B6) == false);

    // Test 7: A with full row rank but B exactly matches a valid X that is not full rank.
    Eigen::Matrix<float, 2, 3> A7;
    A7 << 1, 2, 3,
          4, 5, 6;
    Eigen::Matrix<float, 3, 2> X7;
    X7 << 1, 0,
          0, 1,
          0, 0; // X is rank 2, but does it satisfy?
    Eigen::Matrix2f B7 = A7 * X7; // This is consistent by construction.
    assert(solveLeastSquares(A7, B7) == true);

    // Test 8: Same A7 but B is random → unlikely consistent (but not guaranteed).
    // Use a fixed seed for reproducibility.
    srand(123);
    Eigen::Matrix2f B8 = Eigen::Matrix2f::Random();
    bool result8 = solveLeastSquares(A7, B8);
    // We cannot assert a specific value, but at least ensure it runs without crash.
    assert(result8 == true || result8 == false);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The core algorithm involves solving a linear system `A * X = B` using Eigend's `fullPivLu()` decomposition, which is robust for non-square and rank-deficient matrices. The decomposition computes a least-squares solution if the system is overdetermined, but we must verify exactity via `isApprox`. Steps: (1) Use `A.fullPivLu().solve(B)` to obtain a candidate `X`. (2) Multiply `A * X` and compare element-wise with `B` using `isApprox`, which uses a tolerance (default relative to the largest coefficient). (3) If the comparison passes, the system has an exact solution; otherwise, it does not. Important edge cases: `A` may have rank ≤ 2, meaning the solution may not exist exactly; the `fullPivLu` handles rank deficiency by returning a least-norm solution, which might not satisfy the equation. Also, matrices with very small or large values require tolerance handling; `isApprox` uses a default precision that works for most cases. Time complexity is \(O(n^3)\) for LU decomposition on a 2x3 matrix (practically constant), and space complexity is \(O(1)\) besides temporary matrices. The function returns bool, not the solution, to emphasize the existence check.
