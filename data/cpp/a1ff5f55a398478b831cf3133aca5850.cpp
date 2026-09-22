/*
Write a C++ function named `computeHessenbergH` that accepts a square matrix `A` of type `Eigen::MatrixXf` and returns the upper Hessenberg matrix `H` from its Hessenberg decomposition (i.e., the real Schur-like reduction to upper Hessenberg form via orthogonal similarity transformations). The function must internally use `Eigen::HessenbergDecomposition` and must work for any square dimension (including 1×1 and 2×2). The function should not modify the input matrix `A` (take it by `const` reference) and must return `Eigen::MatrixXf` containing the Hessenberg matrix `H`. Ensure you handle edge cases: if the input is empty (0×0), return an empty matrix; for dimension 1 or 2, the Hessenberg form is the matrix itself (since no subdiagonal reduction is needed). The function must be self-contained with all necessary Eigen headers included. Provide a reference solution that demonstrates reusability (though not required in the function itself) and is suitable for separate compilation without a `main`.
*/
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

// Computes and returns the upper Hessenberg matrix H from the Hessenberg decomposition of A.
// A is not modified. Returns an empty matrix if A is empty.
Eigen::MatrixXf computeHessenbergH(const Eigen::MatrixXf& A) {
    // Handle empty input.
    if (A.rows() == 0 || A.cols() == 0) {
        return Eigen::MatrixXf(0, 0);
    }

    // Ensure square matrix (optional assertion; assume input is square per task).
    assert(A.rows() == A.cols() && "A must be square");

    // Create a Hessenberg decomposition object and compute it.
    Eigen::HessenbergDecomposition<Eigen::MatrixXf> hd(A.rows());
    hd.compute(A);

    // Return the Hessenberg matrix H.
    return hd.matrixH();
}
#include <Eigen/Dense>
#include <cassert>
#include <cmath>

// Function declaration (solution function is defined in the solution section).
Eigen::MatrixXf computeHessenbergH(const Eigen::MatrixXf& A);

int main() {
    // Test 1: Empty matrix -> empty result.
    Eigen::MatrixXf empty(0,0);
    assert(computeHessenbergH(empty).size() == 0);

    // Test 2: 1x1 matrix -> same value.
    Eigen::MatrixXf one(1,1);
    one << 42.0f;
    Eigen::MatrixXf h1 = computeHessenbergH(one);
    assert(h1.rows() == 1 && h1.cols() == 1);
    assert(std::abs(h1(0,0) - 42.0f) < 1e-5);

    // Test 3: 2x2 matrix -> H equals original (since already Hessenberg).
    Eigen::MatrixXf two(2,2);
    two << 1.0f, 2.0f,
           3.0f, 4.0f;
    Eigen::MatrixXf h2 = computeHessenbergH(two);
    assert(h2.isApprox(two, 1e-5));

    // Test 4: 3x3 matrix -> H is upper Hessenberg (subdiagonal entries may be non-zero, below subdiagonal zero).
    Eigen::MatrixXf three(3,3);
    three << 1.0f, 2.0f, 3.0f,
             4.0f, 5.0f, 6.0f,
             7.0f, 8.0f, 9.0f;
    Eigen::MatrixXf h3 = computeHessenbergH(three);
    // Check that entries below first subdiagonal are zero (i.e., (2,0) is zero).
    assert(std::abs(h3(2,0)) < 1e-5);
    // Check that H is square and has same dimensions.
    assert(h3.rows() == 3 && h3.cols() == 3);

    // Test 5: 4x4 random matrix -> verify that H is upper Hessenberg and that A is similar to H (orthogonal transformation).
    Eigen::MatrixXf four = Eigen::MatrixXf::Random(4,4);
    Eigen::MatrixXf h4 = computeHessenbergH(four);
    // Check upper Hessenberg: entries below first subdiagonal are zero.
    for (int i = 2; i < 4; ++i) {
        for (int j = 0; j < i-1; ++j) {
            assert(std::abs(h4(i,j)) < 1e-5);
        }
    }
    // Verify eigenvalues match (similarity invariant) – optional but strong check.
    Eigen::VectorXcf eig_before = Eigen::EigenSolver<Eigen::MatrixXf>(four).eigenvalues();
    Eigen::VectorXcf eig_after = Eigen::EigenSolver<Eigen::MatrixXf>(h4).eigenvalues();
    // Sort eigenvalues for comparison.
    std::sort(eig_before.data(), eig_before.data() + eig_before.size(),
              [](const std::complex<float>& a, const std::complex<float>& b) { return a.real() < b.real() || (a.real() == b.real() && a.imag() < b.imag()); });
    std::sort(eig_after.data(), eig_after.data() + eig_after.size(),
              [](const std::complex<float>& a, const std::complex<float>& b) { return a.real() < b.real() || (a.real() == b.real() && a.imag() < b.imag()); });
    for (int i = 0; i < 4; ++i) {
        assert(std::abs(eig_before(i) - eig_after(i)) < 1e-4);
    }

    // Test 6: Reusability scenario (similar to snippet) – compute twice with different matrices.
    Eigen::MatrixXf A = Eigen::MatrixXf::Random(4,4);
    Eigen::MatrixXf H1 = computeHessenbergH(A);
    Eigen::MatrixXf H2 = computeHessenbergH(2.0f * A);
    // H2 should be different from H1, but still upper Hessenberg.
    assert(!H1.isApprox(H2, 1e-5));
    for (int i = 2; i < 4; ++i) {
        for (int j = 0; j < i-1; ++j) {
            assert(std::abs(H2(i,j)) < 1e-5);
        }
    }

    return 0;
}
// The solution uses Eigen's `HessenbergDecomposition` class. The algorithm internally performs a sequence of Householder reflections to reduce the input matrix to upper Hessenberg form, preserving eigenvalues. The main steps are: (1) Construct a `HessenbergDecomposition` object for the given matrix dimension; (2) Call `compute(A)` to perform the reduction; (3) Extract the upper Hessenberg matrix using `matrixH()`. Edge cases: For an empty matrix, we early-return an empty `MatrixXf`. For dimension 1 or 2, Eigen's algorithm trivially returns the matrix itself (or with a zero subdiagonal if it exists), but the standard `matrixH()` returns the correct Hessenberg form. The time complexity is \(O(n^3)\) for an \(n \times n\) matrix due to the Householder transformations. Space complexity is \(O(n^2)\) for storing the matrix and the decomposition's internal data. The function is `const` correct because it takes `A` by `const&` and returns a new matrix by value.
