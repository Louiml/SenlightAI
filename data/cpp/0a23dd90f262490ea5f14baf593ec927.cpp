Write a C++ function `std::pair<double, double> spectralRadiusAndConditionNumber(const Eigen::Matrix2d& A)` that takes a 2×2 real symmetric matrix and returns a pair containing (1) the spectral radius (the largest absolute eigenvalue) and (2) the 2-norm condition number (ratio of the largest singular value to the smallest singular value). For symmetric matrices, the singular values are the absolute values of the eigenvalues, so the condition number is the ratio of the largest absolute eigenvalue to the smallest absolute eigenvalue. The function must use `Eigen::SelfAdjointEigenSolver` to compute eigenvalues. If the matrix is not symmetric or if the smallest eigenvalue is zero (making the condition number infinite), the function should return `std::numeric_limits<double>::infinity()` for the condition number and still return the spectral radius. Ensure the function is `const`-correct and includes all necessary headers.

#include <cassert>
#include <cmath>
#include <Eigen/Dense>
#include <utility>

// Forward declaration of the function under test (in actual code it's in a header).
std::pair<double, double> spectralRadiusAndConditionNumber(const Eigen::Matrix2d& A);

int main() {
    // Identity matrix: eigenvalues 1,1 -> spectral radius 1, condition number 1.
    Eigen::Matrix2d I = Eigen::Matrix2d::Identity();
    auto res1 = spectralRadiusAndConditionNumber(I);
    assert(std::abs(res1.first - 1.0) < 1e-12);
    assert(std::abs(res1.second - 1.0) < 1e-12);

    // Diagonal matrix diag(2, -3): eigenvalues 2, -3 -> spectral radius 3, condition number 3/2=1.5.
    Eigen::Matrix2d D;
    D << 2.0, 0.0, 0.0, -3.0;
    auto res2 = spectralRadiusAndConditionNumber(D);
    assert(std::abs(res2.first - 3.0) < 1e-12);
    assert(std::abs(res2.second - 1.5) < 1e-12);

    // Singular matrix diag(4,0): spectral radius 4, condition number infinity.
    Eigen::Matrix2d S;
    S << 4.0, 0.0, 0.0, 0.0;
    auto res3 = spectralRadiusAndConditionNumber(S);
    assert(std::abs(res3.first - 4.0) < 1e-12);
    assert(std::isinf(res3.second));

    // Non-symmetric matrix [[1,2],[3,4]]: function should return infinity for condition number.
    Eigen::Matrix2d N;
    N << 1.0, 2.0, 3.0, 4.0;
    auto res4 = spectralRadiusAndConditionNumber(N);
    assert(std::isinf(res4.second));

    // A non-diagonal symmetric matrix: eigenvalues of [[2,1],[1,2]] are 3 and 1.
    Eigen::Matrix2d A;
    A << 2.0, 1.0, 1.0, 2.0;
    auto res5 = spectralRadiusAndConditionNumber(A);
    assert(std::abs(res5.first - 3.0) < 1e-12);
    assert(std::abs(res5.second - 3.0) < 1e-12);

    // Matrix with negative definite: [[-5,0],[0,-2]] eigenvalues -5, -2 -> spectral radius 5, condition number 2.5.
    Eigen::Matrix2d Neg;
    Neg << -5.0, 0.0, 0.0, -2.0;
    auto res6 = spectralRadiusAndConditionNumber(Neg);
    assert(std::abs(res6.first - 5.0) < 1e-12);
    assert(std::abs(res6.second - 2.5) < 1e-12);

    return 0;
}

#include <Eigen/Dense>
#include <utility>
#include <limits>
#include <algorithm>

// Compute the spectral radius and 2-norm condition number of a 2x2 symmetric matrix.
// Returns {spectralRadius, conditionNumber}, where conditionNumber is infinity if the matrix is singular or not symmetric.
std::pair<double, double> spectralRadiusAndConditionNumber(const Eigen::Matrix2d& A) {
    // Check symmetry; SelfAdjointEigenSolver requires it.
    if (!A.isApprox(A.transpose())) {
        return {0.0, std::numeric_limits<double>::infinity()};
    }

    Eigen::SelfAdjointEigenSolver<Eigen::Matrix2d> solver(A);
    if (solver.info() != Eigen::Success) {
        return {0.0, std::numeric_limits<double>::infinity()};
    }

    Eigen::Vector2d eigenvalues = solver.eigenvalues();
    double absLambda0 = std::abs(eigenvalues[0]);
    double absLambda1 = std::abs(eigenvalues[1]);

    double spectralRadius = std::max(absLambda0, absLambda1);
    double minSingular = std::min(absLambda0, absLambda1);

    double conditionNumber = (minSingular > 0.0) ? (spectralRadius / minSingular) : std::numeric_limits<double>::infinity();

    return {spectralRadius, conditionNumber};
}

// The algorithm uses `Eigen::SelfAdjointEigenSolver<Matrix2d>` to compute eigenvalues of the input matrix. Since `SelfAdjointEigenSolver` assumes symmetry, we must first verify that the matrix is symmetric by checking `A == A.transpose()`. If it is not symmetric, we return `{0.0, infinity}` because the input contract is violated—though the task expects symmetric input, handling the edge case robustly is good practice. After computing eigenvalues, we take absolute values to get the singular values. The spectral radius is the maximum of these absolute values. The condition number is the maximum divided by the minimum, but if the minimum is zero (or the matrix is singular), we return infinity. Time complexity is O(1) because the matrix size is fixed at 2×2; the eigensolver performs a fixed number of operations. Space complexity is O(1) as well, apart from the temporary solver object.
