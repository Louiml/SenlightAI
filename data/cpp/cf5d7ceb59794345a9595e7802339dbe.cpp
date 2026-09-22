// Write a C++ function `int countRestartedIterations(int matrixSize, const std::vector<double>& diagonal, const std::vector<double>& rhs, double tolerance, int maxRestarts)` that simulates the behavior of a BiCGSTAB solver with a restart strategy for a **diagonal dominance–free** sparse linear system. The function must construct a tridiagonal matrix of the given size where the main diagonal entries come from the supplied vector `diagonal` (length `matrixSize`), and the off-diagonals are all `1.0`. It then builds a right-hand side vector from `rhs`. Using Eigen's `BiCGSTAB` with the `SparseMatrix<double>` type, it starts from a zero initial guess, sets the maximum iterations per inner solve to `matrixSize / 2 + 1` (at least 1), and performs up to `maxRestarts` outer iterations. After each inner solve, it checks `solver.info()`: if it returns `Success`, the function returns the current restart count; otherwise, it uses the last solution as the new guess (by calling `solveWithGuess`), applies a hard cap of `100` on total inner solve attempts, and continues. The function must return the restart index at which convergence first occurs, or `-1` if it never converges within the allowed restarts or if the total attempt cap is reached. Ensure input validity: `matrixSize` ≥ 2, `diagonal` and `rhs` have exactly `matrixSize` entries; otherwise, return `-1`. The function must be `const`-correct regarding input vectors and use `Eigen::Success` and `Eigen::BiCGSTAB` properly. Include all necessary Eigen headers and use `std::vector` inputs. The off-diagonal pattern creates a tridiagonal system that is not diagonally dominant when diagonal entries are small, simulating a tough convergence scenario.

The core challenge is to emulate an iterative refinement loop with restarting. The main algorithm: (1) Build an Eigen `SparseMatrix<double>` of size N×N by filling the main diagonal with `diagonal[i]` and setting `A.coeffRef(i, i-1) = A.coeffRef(i-1, i) = 1.0` for adjacent indices, then compress. (2) Convert the `std::vector<double>` for the right-hand side into an Eigen `VectorXd`. (3) Create a `BiCGSTAB<SparseMatrix<double>>` solver and precompute the factorization by calling `solver.compute(A)` (or pass `A` in the constructor). (4) Initialize the guess `x` to a zero vector. (5) In a loop from `restart = 0` to `maxRestarts-1`, call `solver.setMaxIterations(innerMax)` (where `innerMax = matrixSize/2 + 1`, at least 1), then solve using `solveWithGuess(b, x)`. After each solve, increment a total attempt counter; if the counter exceeds 100, return `-1`. Check `solver.info()`: if it equals `Eigen::Success`, return the current restart count; otherwise, update `x` to the last solution and continue to the next restart. Edge cases: If `diagonal` or `rhs` have wrong sizes, or `matrixSize` < 2, return `-1`. If the matrix happens to be singular, the solver may return `NoConvergence` or `NumericalIssue`, which should be treated as failure. The function returns the first `restart` index (0-based) where the inner solve reports `Success`. Time complexity: Each inner solve uses BiCGSTAB's iterative method which is O(N) per iteration (due to sparse matrix-vector products) times the number of inner iterations (up to `innerMax`), and we have up to `maxRestarts` restarts, so overall O(N * innerMax * maxRestarts) worst-case, but typically much less if convergence is quick. Space complexity: O(N) for the sparse matrix (since it has ~3N nonzeros), O(N) for the vectors, and O(N) for the solver's internal workspace, so O(N) total.

#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>
#include <vector>

// Simulate a BiCGSTAB solver with restarts on a tridiagonal system.
// Returns the restart index on success, or -1 if convergence fails or inputs invalid.
int countRestartedIterations(int matrixSize,
                             const std::vector<double>& diagonal,
                             const std::vector<double>& rhs,
                             double /*tolerance*/,  // kept for signature, not used
                             int maxRestarts) {
    // Validate inputs
    if (matrixSize < 2 ||
        static_cast<int>(diagonal.size()) != matrixSize ||
        static_cast<int>(rhs.size()) != matrixSize) {
        return -1;
    }

    // Build tridiagonal sparse matrix A: main diagonal from 'diagonal', off-diagonals 1.0
    Eigen::SparseMatrix<double> A(matrixSize, matrixSize);
    std::vector<Eigen::Triplet<double>> triplets;
    triplets.reserve(3 * matrixSize - 2);
    for (int i = 0; i < matrixSize; ++i) {
        triplets.emplace_back(i, i, diagonal[i]);
        if (i > 0) {
            triplets.emplace_back(i, i - 1, 1.0);
            triplets.emplace_back(i - 1, i, 1.0);
        }
    }
    A.setFromTriplets(triplets.begin(), triplets.end());

    // Convert RHS to Eigen vector
    Eigen::VectorXd b = Eigen::VectorXd::Map(rhs.data(), matrixSize);

    // Initialize solver
    Eigen::BiCGSTAB<Eigen::SparseMatrix<double>> solver;
    solver.compute(A);

    // Start from zero guess
    Eigen::VectorXd x = Eigen::VectorXd::Zero(matrixSize);

    int innerMax = matrixSize / 2 + 1;
    if (innerMax < 1) innerMax = 1;

    int totalAttempts = 0;
    for (int restart = 0; restart < maxRestarts; ++restart) {
        solver.setMaxIterations(innerMax);
        x = solver.solveWithGuess(b, x);
        ++totalAttempts;
        if (totalAttempts > 100) return -1;

        if (solver.info() == Eigen::Success) {
            return restart;
        }
        // If not successful, x already holds the last iterate; continue to next restart
    }
    return -1;
}

#include <cassert>
#include <vector>
#include <cmath>

// Forward declaration of the function under test (if not already included)
int countRestartedIterations(int, const std::vector<double>&, const std::vector<double>&, double, int);

int main() {
    // Test 1: Valid input with a simple diagonally dominant matrix -> should converge on restart 0.
    std::vector<double> diag1 = {4.0, 4.0, 4.0, 4.0};
    std::vector<double> rhs1 = {1.0, 2.0, 3.0, 4.0};
    int result1 = countRestartedIterations(4, diag1, rhs1, 1e-8, 10);
    assert(result1 == 0);  // Should converge immediately.

    // Test 2: Invalid input size (diagonal length mismatch) -> returns -1.
    std::vector<double> diag2 = {1.0, 2.0};
    std::vector<double> rhs2 = {1.0, 2.0, 3.0};
    int result2 = countRestartedIterations(3, diag2, rhs2, 1e-8, 5);
    assert(result2 == -1);

    // Test 3: Matrix size < 2 -> returns -1.
    std::vector<double> diag3 = {1.0};
    std::vector<double> rhs3 = {1.0};
    int result3 = countRestartedIterations(1, diag3, rhs3, 1e-8, 5);
    assert(result3 == -1);

    // Test 4: Zero RHS and positive diagonal -> converges immediately (restart 0) because solution is zero.
    std::vector<double> diag4 = {3.0, 3.0, 3.0};
    std::vector<double> rhs4 = {0.0, 0.0, 0.0};
    int result4 = countRestartedIterations(3, diag4, rhs4, 1e-8, 3);
    assert(result4 == 0);

    // Test 5: Near singular matrix (all diagonal = 0) -> will not converge; should return -1 after exhausting restarts.
    std::vector<double> diag5 = {0.0, 0.0, 0.0, 0.0};
    std::vector<double> rhs5 = {1.0, 1.0, 1.0, 1.0};
    int result5 = countRestartedIterations(4, diag5, rhs5, 1e-8, 5);
    assert(result5 == -1);

    // Test 6: Larger matrix, strongly diagonal dominant -> converges quickly.
    int n = 50;
    std::vector<double> diag6(n, 10.0);
    std::vector<double> rhs6(n, 1.0);
    int result6 = countRestartedIterations(n, diag6, rhs6, 1e-8, 3);
    assert(result6 == 0);

    // Test 7: maxRestarts = 0 -> always return -1 unless we magically converge (impossible with zero restarts).
    std::vector<double> diag7 = {2.0, 2.0};
    std::vector<double> rhs7 = {1.0, 1.0};
    int result7 = countRestartedIterations(2, diag7, rhs7, 1e-8, 0);
    assert(result7 == -1);

    // Test 8: Negative diagonal but still non-singular, check it doesn't crash and returns something valid.
    std::vector<double> diag8 = {-5.0, -5.0, -5.0};
    std::vector<double> rhs8 = {1.0, 0.0, -1.0};
    int result8 = countRestartedIterations(3, diag8, rhs8, 1e-8, 10);
    // Should converge (maybe after restarts) or -1; we just assert it's not a crash.
    assert(result8 >= -1 && result8 < 10);

    return 0;
}
