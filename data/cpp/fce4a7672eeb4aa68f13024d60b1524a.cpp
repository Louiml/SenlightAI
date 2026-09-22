Write a C++ function named `symmetricEigenDecomposition` that takes a 2x2 symmetric matrix represented as an `Eigen::Matrix2f` (using double precision would be acceptable, but stick with float as in the prompt) and returns a `std::pair<Eigen::Vector2f, Eigen::Matrix2f>` containing the eigenvalues (in ascending order) as the first element and the corresponding eigenvectors as columns of the matrix in the second element. The function must compute the decomposition manually (without using `SelfAdjointEigenSolver`) and must handle the degenerate case where the matrix has repeated eigenvalues (e.g., identity matrix) by returning orthonormal eigenvectors. The input is guaranteed to be symmetric (i.e., `A(0,1) == A(1,0)`), but you should still verify this and return an empty pair or throw an exception if violated. The function must be `const`-correct, taking the input matrix by const reference, and must not modify the input.

#include <Eigen/Dense>
#include <cassert>
#include <cmath>

// Declaration from the solution (for linking).
std::pair<Eigen::Vector2f, Eigen::Matrix2f> symmetricEigenDecomposition(const Eigen::Matrix2f& A);

int main() {
    // Test 1: Standard symmetric matrix from the prompt.
    {
        Eigen::Matrix2f A;
        A << 1, 2, 2, 3; // eigenvalues: (2 - sqrt(5)), (2 + sqrt(5)) ~ -0.236, 4.236
        auto result = symmetricEigenDecomposition(A);
        Eigen::Vector2f eig = result.first;
        Eigen::Matrix2f vec = result.second;
        // Check eigenvalues are ascending.
        assert(eig(0) <= eig(1));
        // Check eigenvalues match characteristic polynomial: trace = 4, det = -1.
        assert(std::abs(eig(0) + eig(1) - 4.0f) < 1e-5f);
        assert(std::abs(eig(0) * eig(1) + 1.0f) < 1e-5f);
        // Check eigenvectors are unit length.
        assert(std::abs(vec.col(0).norm() - 1.0f) < 1e-5f);
        assert(std::abs(vec.col(1).norm() - 1.0f) < 1e-5f);
        // Check A * v = lambda * v.
        assert((A * vec.col(0) - eig(0) * vec.col(0)).norm() < 1e-5f);
        assert((A * vec.col(1) - eig(1) * vec.col(1)).norm() < 1e-5f);
        // Check orthogonality.
        assert(std::abs(vec.col(0).dot(vec.col(1))) < 1e-5f);
    }

    // Test 2: Diagonal matrix.
    {
        Eigen::Matrix2f A;
        A << 3, 0, 0, -2; // eigenvalues -2, 3
        auto result = symmetricEigenDecomposition(A);
        Eigen::Vector2f eig = result.first;
        assert(std::abs(eig(0) + 2.0f) < 1e-6f);
        assert(std::abs(eig(1) - 3.0f) < 1e-6f);
        // Eigenvectors should be standard basis (up to sign).
        Eigen::Matrix2f vec = result.second;
        assert(std::abs(std::abs(vec(0,0)) - 1.0f) < 1e-6f); // first col is (1,0) or (-1,0)
        assert(std::abs(vec(1,0)) < 1e-6f);
        assert(std::abs(vec(0,1)) < 1e-6f);
        assert(std::abs(std::abs(vec(1,1)) - 1.0f) < 1e-6f);
    }

    // Test 3: Off-diagonal positive, simple case.
    {
        Eigen::Matrix2f A;
        A << 2, 1, 1, 2; // eigenvalues 1, 3
        auto result = symmetricEigenDecomposition(A);
        Eigen::Vector2f eig = result.first;
        assert(std::abs(eig(0) - 1.0f) < 1e-6f);
        assert(std::abs(eig(1) - 3.0f) < 1e-6f);
        Eigen::Matrix2f vec = result.second;
        // Eigenvectors should be (1,-1)/sqrt(2) for λ=1 and (1,1)/sqrt(2) for λ=3.
        assert((vec.col(0) - Eigen::Vector2f(1.0f, -1.0f)/std::sqrt(2.0f)).norm() < 1e-5f ||
               (vec.col(0) + Eigen::Vector2f(1.0f, -1.0f)/std::sqrt(2.0f)).norm() < 1e-5f);
        assert((vec.col(1) - Eigen::Vector2f(1.0f, 1.0f)/std::sqrt(2.0f)).norm() < 1e-5f ||
               (vec.col(1) + Eigen::Vector2f(1.0f, 1.0f)/std::sqrt(2.0f)).norm() < 1e-5f);
    }

    // Test 4: Identity matrix (degenerate repeated eigenvalues).
    {
        Eigen::Matrix2f A = Eigen::Matrix2f::Identity();
        auto result = symmetricEigenDecomposition(A);
        assert(std::abs(result.first(0) - 1.0f) < 1e-6f);
        assert(std::abs(result.first(1) - 1.0f) < 1e-6f);
        // Should be orthonormal basis, we accept the standard basis.
        Eigen::Matrix2f vec = result.second;
        assert(std::abs(vec.determinant() - 1.0f) < 1e-5f || std::abs(vec.determinant() + 1.0f) < 1e-5f);
        assert(std::abs(vec.col(0).norm() - 1.0f) < 1e-6f);
        assert(std::abs(vec.col(1).norm() - 1.0f) < 1e-6f);
        assert(std::abs(vec.col(0).dot(vec.col(1))) < 1e-6f);
    }

    // Test 5: Another symmetric matrix with negative off-diagonal.
    {
        Eigen::Matrix2f A;
        A << 4, -1, -1, 4; // eigenvalues 3, 5
        auto result = symmetricEigenDecomposition(A);
        assert(std::abs(result.first(0) - 3.0f) < 1e-5f);
        assert(std::abs(result.first(1) - 5.0f) < 1e-5f);
        Eigen::Matrix2f vec = result.second;
        assert((A * vec.col(0) - result.first(0) * vec.col(0)).norm() < 1e-5f);
        assert((A * vec.col(1) - result.first(1) * vec.col(1)).norm() < 1e-5f);
    }

    // Test 6: Non-symmetric input should throw.
    {
        Eigen::Matrix2f A;
        A << 1, 2, 3, 4;
        bool threw = false;
        try {
            symmetricEigenDecomposition(A);
        } catch (...) {
            threw = true;
        }
        assert(threw);
    }

    return 0;
}

#include <Eigen/Dense>
#include <utility>
#include <cmath>
#include <stdexcept>

// Compute eigenvalues (ascending) and orthonormal eigenvectors for a 2x2 symmetric matrix.
// Returns pair: first = eigenvalues (smallest to largest), second = eigenvectors as columns.
std::pair<Eigen::Vector2f, Eigen::Matrix2f> symmetricEigenDecomposition(const Eigen::Matrix2f& A) {
    // Verify symmetry (with a small tolerance for floating point).
    if (std::abs(A(0,1) - A(1,0)) > 1e-6f) {
        throw std::invalid_argument("Matrix must be symmetric");
    }

    const float a = A(0,0);
    const float b = A(0,1);
    const float c = A(1,1);

    const float trace = a + c;
    const float diff = (a - c) / 2.0f;
    const float discriminant = std::sqrt(diff * diff + b * b);

    // Eigenvalues in ascending order.
    const float lambda1 = trace / 2.0f - discriminant;
    const float lambda2 = trace / 2.0f + discriminant;

    Eigen::Vector2f eigenvalues;
    eigenvalues << lambda1, lambda2;

    Eigen::Matrix2f eigenvectors;

    // Degenerate case: scaled identity matrix (any orthonormal basis works).
    if (std::abs(b) < 1e-6f && std::abs(diff) < 1e-6f) {
        eigenvectors << 1, 0, 0, 1; // Identity columns
    } else {
        // Compute eigenvector for lambda1 (smaller eigenvalue).
        // Solve (A - lambda1 I) v = 0. For b != 0, use v = (b, lambda1 - a).
        Eigen::Vector2f v1;
        if (std::abs(b) > 1e-6f) {
            v1 << b, lambda1 - a;
        } else {
            // b == 0 but a != c, so matrix is diagonal. Then eigenvectors are standard basis.
            // For lambda1, if lambda1 == a (i.e., a < c) then v = (1,0); else v = (0,1).
            if (lambda1 == a) {
                v1 << 1, 0;
            } else {
                v1 << 0, 1;
            }
        }
        v1.normalize();

        // Second eigenvector is orthogonal to v1 (since symmetric matrix has orthogonal eigenvectors).
        // For 2D, orthogonal vector is (-v1.y, v1.x). Ensure it corresponds to lambda2.
        Eigen::Vector2f v2(-v1(1), v1(0));

        // In rare cases orientation might be flipped, but it's still an eigenvector.
        eigenvectors << v1(0), v2(0),
                        v1(1), v2(1);
    }

    return std::make_pair(eigenvalues, eigenvectors);
}

// The manual algorithm for a 2x2 symmetric matrix uses the standard closed-form solution. Let the matrix be `[[a, b], [b, c]]`. The eigenvalues are the roots of the characteristic polynomial: `λ² - (a+c)λ + (ac - b²) = 0`. Using the quadratic formula, the two eigenvalues are `λ1 = (a+c)/2 + sqrt(((a-c)/2)² + b²)` and `λ2 = (a+c)/2 - sqrt(((a-c)/2)² + b²)`, where we assign `λ1 >= λ2` to match the ascending order requirement (we can then reverse or appropriately order). The eigenvectors are found by solving `(A - λI)v = 0`. For a 2x2 case, a non-zero eigenvector for eigenvalue λ is `(b, λ - a)` (or `(λ - c, b)`), but this fails when both components are zero (e.g., identity matrix). For the degenerate case where `b == 0` and `a == c`, the matrix is a scaled identity, and any orthonormal basis works; we choose the standard basis `(1,0)` and `(0,1)` and ensure orthonormality. For the non-degenerate case, we compute one eigenvector, normalize it, and the second eigenvector is the orthogonal complement (since the matrix is symmetric and eigenvectors are orthogonal). To order eigenvalues ascending, we must ensure the corresponding eigenvectors are paired correctly. Edge cases include: when `b` is very small but non-zero, the formula `(b, λ - a)` is still valid; when the matrix has repeated eigenvalues but is not a scaled identity (e.g., `[[2,0],[0,2]]` is a scaled identity, but a repeated eigenvalue can also occur with `b != 0`? No, for a 2x2 symmetric matrix, a repeated eigenvalue implies `b == 0` and `a == c`, so it's a scaled identity). Time complexity is O(1) since fixed size, and auxiliary space is O(1) besides the output.
