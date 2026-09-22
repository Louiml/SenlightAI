// Write a standalone C++ function that takes a positive integer `n` and returns a `std::vector<double>` containing the eigenvalues of the `n x n` matrix where every entry is 1.0 (the "all-ones" matrix), sorted in ascending order. The function must use the Eigen library's `SelfAdjointEigenSolver` on a dynamically sized `MatrixXd` initialized with `MatrixXd::Ones(n,n)`. You must handle the special case `n = 1` correctly (the single eigenvalue is 1.0) and ensure that for `n = 0` the function returns an empty vector. Do not assume any upper bound on `n`; the function should work for any valid `n`. The returned eigenvalues should be the exact values computed by Eigen, with no additional rounding or sorting beyond what Eigen provides (which is ascending).
#include <cassert>
#include <cmath>
#include <vector>

// The solution function is declared here (for completeness in a standalone test).
std::vector<double> allOnesEigenvalues(int n);

int main() {
    // n = 1: single eigenvalue 1
    auto e1 = allOnesEigenvalues(1);
    assert(e1.size() == 1);
    assert(std::fabs(e1[0] - 1.0) < 1e-12);

    // n = 2: eigenvalues 0 and 2
    auto e2 = allOnesEigenvalues(2);
    assert(e2.size() == 2);
    assert(std::fabs(e2[0] - 0.0) < 1e-12);
    assert(std::fabs(e2[1] - 2.0) < 1e-12);

    // n = 3: eigenvalues 0, 0, 3
    auto e3 = allOnesEigenvalues(3);
    assert(e3.size() == 3);
    assert(std::fabs(e3[0] - 0.0) < 1e-12);
    assert(std::fabs(e3[1] - 0.0) < 1e-12);
    assert(std::fabs(e3[2] - 3.0) < 1e-12);

    // n = 4: eigenvalues 0,0,0,4
    auto e4 = allOnesEigenvalues(4);
    assert(e4.size() == 4);
    for (int i = 0; i < 3; ++i) {
        assert(std::fabs(e4[i] - 0.0) < 1e-12);
    }
    assert(std::fabs(e4[3] - 4.0) < 1e-12);

    // n = 0: empty vector
    auto e0 = allOnesEigenvalues(0);
    assert(e0.empty());
}
#include <vector>
#include <Eigen/Dense>

// Return the eigenvalues of the n x n all-ones matrix in ascending order.
std::vector<double> allOnesEigenvalues(int n) {
    if (n == 0) {
        return {};
    }
    Eigen::MatrixXd ones = Eigen::MatrixXd::Ones(n, n);
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(ones);
    Eigen::VectorXd evals = es.eigenvalues();
    return std::vector<double>(evals.data(), evals.data() + evals.size());
}
// The solution creates an `n x n` `MatrixXd` filled with ones using `MatrixXd::Ones(n,n)`, then constructs a `SelfAdjointEigenSolver<MatrixXd>` with that matrix as an argument. This solver computes eigenvalues (and optionally eigenvectors) for symmetric real matrices; since the all-ones matrix is symmetric, it is valid. For `n > 1`, the matrix has eigenvalues: `n` (for the eigenvector of all ones) and `0` (with multiplicity `n-1`). Eigen returns them in ascending order, so the vector will contain `n-1` zeros followed by `n`. For `n = 1`, the matrix is `[1]` with eigenvalue `1`. For `n = 0`, the matrix is empty, but Eigen may not handle this gracefully, so we check and return an empty vector early. The time complexity of the self-adjoint eigensolver is \(O(n^3)\) due to the QR algorithm, and space complexity is \(O(n^2)\) for the matrix and the solver's internal storage. The solution extracts `es.eigenvalues()` which is a `VectorXd`, and we convert to `std::vector<double>`.
