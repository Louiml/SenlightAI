// Write a C++ function that takes a positive integer `n` and returns a `std::vector<double>` containing the eigenvalues of an `n x n` matrix where every entry is 1.0. You must compute these eigenvalues by explicitly constructing the all-ones matrix using the Eigen library's `MatrixXd` and `SelfAdjointEigenSolver`. The function should handle the case `n = 1` correctly, and for general `n`, the eigenvalues are known to be `n` (with multiplicity 1) and `0` (with multiplicity `n-1`). Your function must return the eigenvalues in ascending order, as produced by the solver. Ensure your implementation is const-correct and uses appropriate Eigen types.
#include <cassert>
#include <cmath>
#include <vector>

// The solution function is assumed to be defined above (or included).
// Here we test it.

int main() {
    // n = 1: single eigenvalue 1
    std::vector<double> e1 = eigenvaluesOfAllOnesMatrix(1);
    assert(e1.size() == 1);
    assert(std::fabs(e1[0] - 1.0) < 1e-12);

    // n = 2: eigenvalues 0 and 2
    std::vector<double> e2 = eigenvaluesOfAllOnesMatrix(2);
    assert(e2.size() == 2);
    assert(std::fabs(e2[0] - 0.0) < 1e-12);
    assert(std::fabs(e2[1] - 2.0) < 1e-12);

    // n = 3: eigenvalues 0,0,3
    std::vector<double> e3 = eigenvaluesOfAllOnesMatrix(3);
    assert(e3.size() == 3);
    assert(std::fabs(e3[0] - 0.0) < 1e-12);
    assert(std::fabs(e3[1] - 0.0) < 1e-12);
    assert(std::fabs(e3[2] - 3.0) < 1e-12);

    // n = 5: eigenvalues 0,0,0,0,5
    std::vector<double> e5 = eigenvaluesOfAllOnesMatrix(5);
    assert(e5.size() == 5);
    for (int i = 0; i < 4; ++i) {
        assert(std::fabs(e5[i] - 0.0) < 1e-12);
    }
    assert(std::fabs(e5[4] - 5.0) < 1e-12);

    // n = 10: check last eigenvalue is 10, first nine are 0
    std::vector<double> e10 = eigenvaluesOfAllOnesMatrix(10);
    assert(e10.size() == 10);
    for (int i = 0; i < 9; ++i) {
        assert(std::fabs(e10[i] - 0.0) < 1e-12);
    }
    assert(std::fabs(e10[9] - 10.0) < 1e-12);
}
#include <vector>
#include <Eigen/Dense>

// Compute eigenvalues of an n x n all-ones matrix using Eigen's self-adjoint solver.
// Returns eigenvalues in ascending order. Assumes n >= 1.
std::vector<double> eigenvaluesOfAllOnesMatrix(int n) {
    // Construct the n x n matrix filled with ones.
    Eigen::MatrixXd ones = Eigen::MatrixXd::Ones(n, n);
    
    // Solve the self-adjoint eigenproblem.
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(ones);
    
    // Extract the eigenvalues as a column vector.
    Eigen::VectorXd eigenvalues = solver.eigenvalues();
    
    // Convert to std::vector<double>.
    std::vector<double> result(eigenvalues.data(), eigenvalues.data() + eigenvalues.size());
    return result;
}
// The matrix in question is the `n x n` all-ones matrix, which is symmetric (and real), so the `SelfAdjointEigenSolver` from Eigen is appropriate and will return real eigenvalues in ascending order. Construct the matrix with `MatrixXd::Ones(n, n)`, then create a `SelfAdjointEigenSolver<MatrixXd>` object with that matrix. Call `.eigenvalues()` to get a `VectorXd` (a column vector of doubles). Convert it to a `std::vector<double>` by iterating over its entries (or using the Eigen `data()` pointer together with `size()`). Edge cases: For `n=1`, the matrix is just `[1]`, and the solver returns a single eigenvalue `1.0`. For `n ≥ 2`, the eigenvalues are `0` repeated `n-1` times and `n` once; the solver will return them sorted, so `0`s first then `n`. There's no need to manually compute eigenvalues; the solver handles numerical precision. Time complexity is `O(n^3)` due to the eigen decomposition, and space complexity is `O(n^2)` for the matrix plus `O(n)` for the eigenvalue vector.
