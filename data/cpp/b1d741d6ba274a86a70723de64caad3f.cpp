/*
Write a standalone C++ function that, given a symmetric matrix represented as a `std::vector<std::vector<double>>`, computes and returns a `std::vector<double>` containing the eigenvalues sorted in ascending order. The function must verify that the input matrix is square and symmetric (within a small tolerance of `1e-9`), throwing an `std::invalid_argument` exception if not. You may use the Eigen library for the actual eigenvalue computation, specifically `SelfAdjointEigenSolver`, but the function signature must accept `const std::vector<std::vector<double>>&` and return the eigenvalues as a sorted `std::vector<double>`. Ensure the implementation does not modify the input and handles the case of an empty matrix gracefully by returning an empty vector. Test the function with identity matrices, random symmetric matrices, and a known non-symmetric case.
*/

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <Eigen/Dense>

// Computes and returns the eigenvalues of a symmetric matrix, sorted in ascending order.
std::vector<double> symmetricEigenvalues(const std::vector<std::vector<double>>& matrix) {
    if (matrix.empty()) return {};

    const size_t n = matrix.size();
    for (const auto& row : matrix) {
        if (row.size() != n) {
            throw std::invalid_argument("Matrix must be square");
        }
    }

    const double tolerance = 1e-9;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            if (std::abs(matrix[i][j] - matrix[j][i]) > tolerance) {
                throw std::invalid_argument("Matrix must be symmetric");
            }
        }
    }

    Eigen::MatrixXd A(n, n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            A(i, j) = matrix[i][j];
        }
    }

    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(A, Eigen::EigenvaluesOnly);
    if (solver.info() != Eigen::Success) {
        throw std::runtime_error("Eigen decomposition failed");
    }

    const Eigen::VectorXd eigenvals = solver.eigenvalues();
    std::vector<double> result(eigenvals.data(), eigenvals.data() + eigenvals.size());
    std::sort(result.begin(), result.end());
    return result;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <stdexcept>

// Assuming symmetricEigenvalues is defined above.

int main() {
    // Empty matrix
    assert(symmetricEigenvalues({}).empty());

    // 1x1 matrix
    {
        auto vals = symmetricEigenvalues({{5.0}});
        assert(vals.size() == 1 && std::abs(vals[0] - 5.0) < 1e-9);
    }

    // 2x2 identity
    {
        auto vals = symmetricEigenvalues({{1.0, 0.0}, {0.0, 1.0}});
        assert(vals.size() == 2);
        assert(std::abs(vals[0] - 1.0) < 1e-9 && std::abs(vals[1] - 1.0) < 1e-9);
    }

    // 2x2 with known eigenvalues 3 and -2
    {
        // Matrix [[0.5, 2.5], [2.5, 0.5]]? Actually let's use [[1,2],[2,1]] => eigenvalues 3 and -1
        auto vals = symmetricEigenvalues({{1.0, 2.0}, {2.0, 1.0}});
        assert(vals.size() == 2);
        assert(std::abs(vals[0] - (-1.0)) < 1e-9);
        assert(std::abs(vals[1] - 3.0) < 1e-9);
    }

    // 3x3 diagonal
    {
        auto vals = symmetricEigenvalues({{4.0,0,0},{0,-1.0,0},{0,0,2.0}});
        assert(vals.size() == 3);
        assert(std::abs(vals[0] - (-1.0)) < 1e-9);
        assert(std::abs(vals[1] - 2.0) < 1e-9);
        assert(std::abs(vals[2] - 4.0) < 1e-9);
    }

    // Non-square throws
    {
        bool threw = false;
        try { symmetricEigenvalues({{1.0, 2.0}}); } catch (const std::invalid_argument&) { threw = true; }
        assert(threw);
    }

    // Non-symmetric throws
    {
        bool threw = false;
        try { symmetricEigenvalues({{1.0, 2.0}, {3.0, 4.0}}); } catch (const std::invalid_argument&) { threw = true; }
        assert(threw);
    }

    // Slightly asymmetric within tolerance (should pass)
    {
        auto vals = symmetricEigenvalues({{1.0, 2.0 + 1e-12}, {2.0, 3.0}});
        assert(vals.size() == 2);
        // Eigenvalues of [[1,2],[2,3]] are approx 4.236, -0.236
        assert(std::abs(vals[0] + 0.2360679775) < 1e-6);
        assert(std::abs(vals[1] - 4.2360679775) < 1e-6);
    }

    return 0;
}

// The solution converts the 2D vector input into an Eigen `MatrixXd` by checking that every row has the same size as the number of rows (square). Symmetry is verified by comparing `A(i,j)` with `A(j,i)` for all `i<j` using a tolerance to handle floating-point representation errors. After validation, `SelfAdjointEigenSolver<MatrixXd>` is used with `EigenvaluesOnly` to avoid computing eigenvectors. The eigenvalues are extracted into an Eigen vector, then copied into a `std::vector<double>` and sorted with `std::sort` to ensure ascending order. Edge cases: empty matrix → return empty; 1x1 matrix → valid, returns the single value; non-square or asymmetric → throw. Time complexity: validation is O(n²) and eigen decomposition is O(n³) for an n×n matrix, so overall O(n³). Space complexity: O(n²) for the Eigen matrix plus O(n) for the result.
