Write a C++ function named `firstEigenvectorOfOnesMatrix` that creates a 3x3 matrix of all ones using the Eigen library's `MatrixXcf` type (single-precision complex), computes its eigenvectors using `ComplexEigenSolver`, and returns the second column (index 1) of the eigenvector matrix as a `VectorXcf`. The function should take no arguments and return the eigenvector corresponding to the eigenvalue with the second smallest magnitude (which for a 3x3 ones matrix will be the zero eigenvalue, but you don't need to verify this—just return column 1). Ensure the function is `const`-correct and works with the Eigen library included. The returned vector should be normalized as Eigen does by default. Use the provided code snippet as a starting point, but make it a standalone free function.

#include <cassert>
#include <cmath>
#include <Eigen/Eigenvalues>

int main() {
    // Test 1: Basic shape and type check.
    Eigen::VectorXcf v = firstEigenvectorOfOnesMatrix();
    assert(v.size() == 3);

    // Test 2: The eigenvector should be non-zero.
    assert(v.norm() > 1e-6f);

    // Test 3: Since all entries of the ones matrix are equal, the eigenvector
    // for the zero eigenvalue should have entries summing to approximately zero.
    // But we are specifically taking column 1, not the zero-eigenvalue one (column 2).
    // Instead, check that the returned vector is an eigenvector: A*v = lambda*v.
    // We don't know which eigenvalue it corresponds to, but we can verify it's an eigenvector.
    Eigen::MatrixXcf A = Eigen::MatrixXcf::Ones(3, 3);
    Eigen::VectorXcf Av = A * v;
    // Find scale factor: since all rows identical, Av[0] = sum(v). If it's an eigenvector,
    // Av = lambda * v, so we check that Av[0]/v[0] == Av[1]/v[1] == Av[2]/v[2] when v entries non-zero.
    // For safety, check that Av is proportional to v (cross products zero).
    assert(std::abs(Av(0) * v(1) - Av(1) * v(0)) < 1e-4f);
    assert(std::abs(Av(0) * v(2) - Av(2) * v(0)) < 1e-4f);
    assert(std::abs(Av(1) * v(2) - Av(2) * v(1)) < 1e-4f);

    // Test 4: Normalization check (Eigen returns normalized eigenvectors by default).
    assert(std::abs(v.norm() - 1.0f) < 1e-5f);

    // Test 5: Verify that the matrix of ones has a known eigenvector for eigenvalue 3:
    // (1,1,1)/sqrt(3) is the eigenvector for eigenvalue 3, but that's column 0, not column 1.
    // We just verify that column 1 is not equal to column 0 (they are distinct).
    Eigen::MatrixXcf ones = Eigen::MatrixXcf::Ones(3, 3);
    Eigen::ComplexEigenSolver<Eigen::MatrixXcf> ces(ones);
    assert((ces.eigenvectors().col(1) - ces.eigenvectors().col(0)).norm() > 1e-3f);

    // Test 6: Since the matrix has eigenvalues {3,0,0}, column 1 and column 2
    // correspond to the zero eigenvalue and are orthogonal to (1,1,1).
    // The dot product of v with (1,1,1) should be near zero.
    Eigen::VectorXcf ones_vec = Eigen::VectorXcf::Ones(3);
    assert(std::abs(v.dot(ones_vec)) < 1e-3f);

    return 0;
}

#include <Eigen/Eigenvalues>
#include <Eigen/Core>

// Returns the second eigenvector (column index 1) of a 3x3 ones matrix.
// Uses ComplexEigenSolver on a single-precision complex matrix.
Eigen::VectorXcf firstEigenvectorOfOnesMatrix() {
    // Create a 3x3 matrix of ones with complex single-precision type.
    Eigen::MatrixXcf ones = Eigen::MatrixXcf::Ones(3, 3);
    
    // Compute eigenvalues and eigenvectors.
    Eigen::ComplexEigenSolver<Eigen::MatrixXcf> ces(ones);
    
    // Return the second eigenvector (column 1).
    return ces.eigenvectors().col(1);
}

// The solution uses Eigen's `ComplexEigenSolver` on a 3x3 matrix of ones (`MatrixXcf::Ones(3,3)`). After constructing the solver, the eigenvectors are accessed via `eigenvectors()` which returns a `MatrixXcf` where each column is an eigenvector. We need to return the second column (`col(1)`), which is a `VectorXcf`. The solver automatically computes eigenvalues and eigenvectors; no manual normalization is required. Edge cases: the matrix is symmetric and real, so all eigenvectors are real up to multiplication by a complex constant, but the solver returns them as complex vectors. The result is deterministic. Time complexity is O(n^3) for eigen decomposition (n=3 here), but constant. Space complexity is O(n^2) for the matrix and result, again constant. No special edge cases for fixed 3x3 input.
