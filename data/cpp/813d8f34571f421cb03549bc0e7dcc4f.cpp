/*
Write a C++ function named `firstEigenvectorOfOnes` that takes an integer `n` (with `n >= 2`) and returns a `std::vector<double>` containing the first (smallest-magnitude) eigenvector of the `n x n` matrix where every entry is 1. Use the `Eigen` library's `SelfAdjointEigenSolver` to compute the eigenvectors, and extract the column corresponding to the smallest eigenvalue. Normalize the returned vector so that its first component is positive (i.e., if the eigenvector's first element is negative, multiply the entire vector by -1) to ensure deterministic output. If `n` is less than 2, return an empty vector. The function should be `const`-correct and avoid unnecessary copies.
*/
#include <vector>
#include <Eigen/Dense>

// Returns the first (smallest-eigenvalue) eigenvector of the n x n all-ones matrix.
// The vector is normalized so that its first component is non-negative.
// Returns an empty vector if n < 2.
std::vector<double> firstEigenvectorOfOnes(int n) {
    if (n < 2) {
        return {};
    }

    // Create an n x n matrix with all entries equal to 1.
    Eigen::MatrixXd ones = Eigen::MatrixXd::Ones(n, n);

    // Compute eigenvalues and eigenvectors (sorted in increasing eigenvalue order).
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(ones);

    // Extract the eigenvector corresponding to the smallest eigenvalue (index 0).
    Eigen::VectorXd vec = es.eigenvectors().col(0);

    // Normalize sign: ensure the first component is non-negative.
    if (vec(0) < 0.0) {
        vec = -vec;
    }

    // Convert to a standard vector.
    return std::vector<double>(vec.data(), vec.data() + n);
}
#include <cassert>
#include <cmath>
#include <vector>

// Solution function declaration (assumed above or included via header).
std::vector<double> firstEigenvectorOfOnes(int n);

int main() {
    // Test invalid input.
    assert(firstEigenvectorOfOnes(1).empty());
    assert(firstEigenvectorOfOnes(0).empty());

    // n = 2: eigenvector must be (1, -1) after sign normalization (scaled to unit length).
    auto v2 = firstEigenvectorOfOnes(2);
    assert(v2.size() == 2);
    assert(std::abs(v2[0] - 1.0/std::sqrt(2)) < 1e-12);
    assert(std::abs(v2[1] + 1.0/std::sqrt(2)) < 1e-12);
    // Check orthogonality to all-ones vector.
    assert(std::abs(v2[0] + v2[1]) < 1e-12);

    // n = 3: eigenvector has sum zero and unit norm.
    auto v3 = firstEigenvectorOfOnes(3);
    assert(v3.size() == 3);
    double sum = v3[0] + v3[1] + v3[2];
    assert(std::abs(sum) < 1e-12);
    double norm = std::sqrt(v3[0]*v3[0] + v3[1]*v3[1] + v3[2]*v3[2]);
    assert(std::abs(norm - 1.0) < 1e-12);
    // First component must be non-negative.
    assert(v3[0] >= 0.0);

    // n = 4: check sum zero, unit norm, first component non-negative.
    auto v4 = firstEigenvectorOfOnes(4);
    assert(v4.size() == 4);
    sum = 0.0;
    for (double x : v4) sum += x;
    assert(std::abs(sum) < 1e-12);
    norm = 0.0;
    for (double x : v4) norm += x*x;
    norm = std::sqrt(norm);
    assert(std::abs(norm - 1.0) < 1e-12);
    assert(v4[0] >= 0.0);

    // n = 5: verify vector is not all zeros.
    auto v5 = firstEigenvectorOfOnes(5);
    bool allZero = true;
    for (double x : v5) if (std::abs(x) > 1e-12) { allZero = false; break; }
    assert(!allZero);

    // Additional check: eigenvalue for the first eigenvector should be 0.
    Eigen::MatrixXd ones5 = Eigen::MatrixXd::Ones(5,5);
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(ones5);
    assert(std::abs(es.eigenvalues()(0)) < 1e-12);

    return 0;
}
// The `n x n` all-ones matrix has a known spectral structure: one eigenvalue equal to `n` (with eigenvector proportional to the vector of all ones), and `n-1` eigenvalues equal to 0 (with eigenvectors spanning the subspace orthogonal to the all-ones vector). When using `SelfAdjointEigenSolver`, eigenvalues are sorted in increasing order, so the first eigenvalue is 0 (for `n >= 2`) and the corresponding eigenvector is any vector orthogonal to the all-ones vector. The `SelfAdjointEigenSolver` computes a specific orthonormal basis for the eigenspace, and the column at index 0 is deterministic for a given matrix. However, the sign of the eigenvectors is arbitrary; hence we normalize the sign by making the first component non-negative. The main implementation steps: create an `n x n` `MatrixXd` filled with ones, construct a `SelfAdjointEigenSolver<MatrixXd>` on that matrix, extract the eigenvector column 0, then apply sign normalization. Edge cases: `n < 2` should return an empty vector; for `n = 2`, the eigenvector for eigenvalue 0 must be orthogonal to (1,1), so it is proportional to (1,-1) or (-1,1) – normalization makes it (1,-1)/sqrt(2). For larger `n`, the returned vector components will satisfy that the sum of its components is zero. Time complexity is dominated by the eigen decomposition, which for a dense `n x n` matrix is \(O(n^3)\), and space complexity is \(O(n^2)\) for the matrix and eigenvectors.
