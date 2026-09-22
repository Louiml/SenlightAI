Write a C++ function named `computeEigenvaluesOfSymmetric` that takes a 4x4 floating-point matrix (using `Eigen::Matrix4f`) as input, verifies it is symmetric (i.e., `matrix == matrix.transpose()`), and returns an `Eigen::Vector4f` containing its eigenvalues sorted in ascending order. Use `Eigen::SelfAdjointEigenSolver` to compute the eigenvalues, and ensure the function is `noexcept` where possible. The input matrix is guaranteed to be symmetric; if it is not, throw `std::invalid_argument`. The function must not modify the input matrix and must be usable with `const` matrices.
// The solution relies on the `SelfAdjointEigenSolver` class from Eigen, which is specifically designed for real symmetric matrices and guarantees real eigenvalues. The main steps: (1) Check symmetry by comparing `matrix` with `matrix.transpose()` using `isApprox` to account for floating-point tolerance; if not symmetric, throw `std::invalid_argument`. (2) Construct a `SelfAdjointEigenSolver<Matrix4f>` object and call `compute(matrix)` directly in the constructor or separately. (3) Extract eigenvalues using `.eigenvalues()`, which is already sorted in ascending order (per Eigen's documentation). (4) Return this vector by value. Edge cases: The input is exactly 4x4, so no dimension checks are needed; floating-point symmetry must be handled with a tolerance (e.g., `isApprox` with default precision). Time complexity is O(4^3) = O(64) operations (constant), and space complexity is O(1) for the solver (ignoring Eigen internals). The function is `const`-correct by taking a `const Matrix4f&` and not modifying it.
#include <Eigen/Dense>
#include <stdexcept>

// Compute eigenvalues of a symmetric 4x4 matrix, sorted ascending.
// Throws std::invalid_argument if the matrix is not symmetric.
Eigen::Vector4f computeEigenvaluesOfSymmetric(const Eigen::Matrix4f& matrix) {
    // Verify symmetry with a tolerance for floating-point differences.
    if (!matrix.isApprox(matrix.transpose())) {
        throw std::invalid_argument("Matrix must be symmetric.");
    }

    // Use SelfAdjointEigenSolver for symmetric matrices (real eigenvalues).
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix4f> solver(matrix);
    
    // eigenvalues() returns a vector sorted in increasing order.
    return solver.eigenvalues();
}
#include <cassert>
#include <cmath>
#include <Eigen/Dense>

int main() {
    // Test 1: Identity matrix (eigenvalues all 1)
    Eigen::Matrix4f A = Eigen::Matrix4f::Identity();
    Eigen::Vector4f ev = computeEigenvaluesOfSymmetric(A);
    assert((ev - Eigen::Vector4f(1,1,1,1)).norm() < 1e-6);

    // Test 2: Diagonal matrix with distinct eigenvalues
    Eigen::Matrix4f B = Eigen::Matrix4f::Zero();
    B(0,0) = 5; B(1,1) = -3; B(2,2) = 2; B(3,3) = 0;
    ev = computeEigenvaluesOfSymmetric(B);
    assert((ev - Eigen::Vector4f(-3,0,2,5)).norm() < 1e-6);

    // Test 3: Symmetric random matrix (ensure A + A^T is symmetric)
    Eigen::Matrix4f R = Eigen::Matrix4f::Random();
    Eigen::Matrix4f C = R + R.transpose();
    ev = computeEigenvaluesOfSymmetric(C);
    // Eigenvalues must sum to the trace
    float sum = ev.sum();
    assert(std::abs(sum - C.trace()) < 1e-4);

    // Test 4: Zero matrix (all eigenvalues zero)
    Eigen::Matrix4f Z = Eigen::Matrix4f::Zero();
    ev = computeEigenvaluesOfSymmetric(Z);
    assert(ev.norm() < 1e-6);

    // Test 5: Non-symmetric matrix should throw
    bool threw = false;
    try {
        Eigen::Matrix4f NS = Eigen::Matrix4f::Random();
        computeEigenvaluesOfSymmetric(NS);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 6: Matrix with repeated eigenvalues (e.g., identity scaled)
    Eigen::Matrix4f D = 2.0f * Eigen::Matrix4f::Identity();
    ev = computeEigenvaluesOfSymmetric(D);
    assert((ev - Eigen::Vector4f(2,2,2,2)).norm() < 1e-6);

    // Test 7: Symmetric matrix with negative eigenvalues
    Eigen::Matrix4f E = Eigen::Matrix4f::Zero();
    E(0,1) = 1; E(1,0) = 1; E(2,3) = -2; E(3,2) = -2;
    ev = computeEigenvaluesOfSymmetric(E);
    // Eigenvalues are -2, -1, 1, 2 (from the block structure)
    assert(std::abs(ev[0] - (-2.0f)) < 1e-5);
    assert(std::abs(ev[1] - (-1.0f)) < 1e-5);
    assert(std::abs(ev[2] - 1.0f) < 1e-5);
    assert(std::abs(ev[3] - 2.0f) < 1e-5);

    return 0;
}
