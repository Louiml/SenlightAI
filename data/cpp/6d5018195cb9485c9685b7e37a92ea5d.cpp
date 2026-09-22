/*
Write a C++ function that takes an integer `n` (where `n >= 1`) and returns a `std::vector<double>` containing the first eigenvector of the `n x n` matrix of all ones, normalized to unit length. The eigenvector must correspond to the largest eigenvalue (which for this matrix is `n`). The function must use the Eigen library's `SelfAdjointEigenSolver` and return the column vector directly. Assume the matrix is symmetric (which it is) and handle the case `n = 1` (where the eigenvector should be `[1.0]`). The output vector must be sorted such that the eigenvector components are in the same order as Eigen's default column order.
*/
#include <vector>
#include <Eigen/Dense>

// Return the eigenvector corresponding to the largest eigenvalue of the n x n all-ones matrix.
std::vector<double> largestEigenvectorOfOnesMatrix(int n) {
    Eigen::MatrixXd ones = Eigen::MatrixXd::Ones(n, n);
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(ones);
    // Eigen sorts eigenvalues ascending; the largest is the last column.
    Eigen::VectorXd vec = solver.eigenvectors().col(n - 1);
    return std::vector<double>(vec.data(), vec.data() + vec.size());
}
#include <cassert>
#include <cmath>
#include <vector>

// The solution function is assumed to be declared above.
int main() {
    // n = 1
    std::vector<double> v1 = largestEigenvectorOfOnesMatrix(1);
    assert(v1.size() == 1);
    assert(std::fabs(v1[0] - 1.0) < 1e-12);

    // n = 2: matrix [[1,1],[1,1]] eigenvalues 2 and 0, eigenvectors (1,1)/sqrt(2) and (1,-1)/sqrt(2)
    std::vector<double> v2 = largestEigenvectorOfOnesMatrix(2);
    assert(v2.size() == 2);
    assert(std::fabs(v2[0] - 1.0/std::sqrt(2.0)) < 1e-12);
    assert(std::fabs(v2[1] - 1.0/std::sqrt(2.0)) < 1e-12);

    // n = 3: check all components equal and norm is 1
    std::vector<double> v3 = largestEigenvectorOfOnesMatrix(3);
    assert(v3.size() == 3);
    double norm = 0.0;
    for (double val : v3) norm += val * val;
    assert(std::fabs(norm - 1.0) < 1e-12);
    assert(std::fabs(v3[0] - v3[1]) < 1e-12);
    assert(std::fabs(v3[0] - v3[2]) < 1e-12);

    // n = 4: check all positive (since largest eigenvector aligns with all-ones)
    std::vector<double> v4 = largestEigenvectorOfOnesMatrix(4);
    for (double val : v4) assert(val > 0.0);

    // n = 5: check norm again
    std::vector<double> v5 = largestEigenvectorOfOnesMatrix(5);
    double norm5 = 0.0;
    for (double val : v5) norm5 += val * val;
    assert(std::fabs(norm5 - 1.0) < 1e-12);
}
// The `n x n` matrix of ones has rank 1, so it has exactly one non-zero eigenvalue equal to `n`, and the remaining `n-1` eigenvalues are zero. The eigenvector corresponding to the largest eigenvalue is the all-ones vector (unnormalized). However, Eigen's `SelfAdjointEigenSolver` returns eigenvectors sorted in increasing eigenvalue order, so the largest eigenvalue is the last column. We need to extract `eigenvectors().col(n-1)` and return it as a `std::vector<double>`. For `n=1`, the matrix is `[1]`, the eigenvector is `[1]`. The solution constructs the matrix using `MatrixXd::Ones(n,n)`, runs the solver, and copies the last column into a vector. Edge cases: `n=1` works naturally (solver returns the single eigenvector). Time complexity is `O(n^3)` due to the eigen decomposition, and space complexity is `O(n^2)` for the matrix and solver internal storage.
