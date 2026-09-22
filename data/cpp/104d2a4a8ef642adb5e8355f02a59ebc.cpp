// Write a C++ function that accepts a 4x4 matrix of single-precision floats (using `Eigen::MatrixXcf`) and returns a `std::pair<Eigen::MatrixXcf, bool>` where the first element is the upper-triangular matrix `T` from the complex Schur decomposition (computed without the unitary matrix `U`), and the second element is a boolean indicating whether the Schur decomposition succeeded (i.e., the input matrix is finite and the decomposition algorithm converged). The function must be named `computeComplexSchurT` and must use the `ComplexSchur` solver from Eigen with the `computeU = false` option. The returned triangular matrix should be a real-valued (though stored in complex format) upper-triangular matrix with zeros below the diagonal. The input matrix must be read-only (const reference), and the output must be correct for any 4x4 complex matrix, including matrices with repeated eigenvalues, zero matrices, diagonal matrices, and matrices with random entries. Handle the case where the input contains non-finite values (NaN or Inf) by returning `false` in the boolean and an empty matrix as the first element.

#include <Eigen/Eigenvalues>
#include <cassert>
#include <cmath>
#include <complex>

int main() {
    // Test 1: Zero matrix yields zero triangular matrix.
    Eigen::MatrixXcf zero(4,4);
    zero.setZero();
    auto [T1, ok1] = computeComplexSchurT(zero);
    assert(ok1);
    assert(T1.rows() == 4 && T1.cols() == 4);
    assert(T1.isApprox(zero, 1e-6f));

    // Test 2: Identity matrix yields Identity as T.
    Eigen::MatrixXcf I = Eigen::MatrixXcf::Identity(4,4);
    auto [T2, ok2] = computeComplexSchurT(I);
    assert(ok2);
    assert(T2.isApprox(I, 1e-6f));

    // Test 3: Diagonal matrix with distinct real eigenvalues.
    Eigen::MatrixXcf diag(4,4);
    diag << 1.0f, 0, 0, 0,
            0, 2.0f, 0, 0,
            0, 0, 3.0f, 0,
            0, 0, 0, 4.0f;
    auto [T3, ok3] = computeComplexSchurT(diag);
    assert(ok3);
    assert(T3.isApprox(diag, 1e-6f));

    // Test 4: Non-triangular matrix: T must be upper-triangular.
    Eigen::MatrixXcf A(4,4);
    A << 1.0f, 2.0f, 3.0f, 4.0f,
         5.0f, 6.0f, 7.0f, 8.0f,
         9.0f, 10.0f, 11.0f, 12.0f,
         13.0f, 14.0f, 15.0f, 16.0f;
    auto [T4, ok4] = computeComplexSchurT(A);
    assert(ok4);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < i; ++j) {
            assert(std::abs(T4(i,j)) < 1e-5f);
        }
    }
    // Trace of T should equal trace of A.
    std::complex<float> traceA(0,0), traceT(0,0);
    for (int i = 0; i < 4; ++i) {
        traceA += A(i,i);
        traceT += T4(i,i);
    }
    assert(std::abs(traceA - traceT) < 1e-4f);

    // Test 5: Matrix with NaN should return failure.
    Eigen::MatrixXcf nanMat(4,4);
    nanMat.setConstant(std::numeric_limits<float>::quiet_NaN());
    auto [T5, ok5] = computeComplexSchurT(nanMat);
    assert(!ok5);
    assert(T5.rows() == 0 && T5.cols() == 0);

    // Test 6: Matrix with Inf should return failure.
    Eigen::MatrixXcf infMat(4,4);
    infMat.setConstant(std::numeric_limits<float>::infinity());
    auto [T6, ok6] = computeComplexSchurT(infMat);
    assert(!ok6);
    assert(T6.rows() == 0);

    // Test 7: Wrong size should return failure.
    Eigen::MatrixXcf wrong(3,3);
    wrong.setRandom();
    auto [T7, ok7] = computeComplexSchurT(wrong);
    assert(!ok7);

    // Test 8: Repeated eigenvalues (Jordan-like block) still works.
    Eigen::MatrixXcf J(4,4);
    J << 1.0f, 1.0f, 0, 0,
         0, 1.0f, 1.0f, 0,
         0, 0, 1.0f, 1.0f,
         0, 0, 0, 1.0f;
    auto [T8, ok8] = computeComplexSchurT(J);
    assert(ok8);
    // Must be upper-triangular.
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < i; ++j) {
            assert(std::abs(T8(i,j)) < 1e-5f);
        }
    }

    return 0;
}

#include <Eigen/Eigenvalues>
#include <utility>
#include <limits>

// Compute the complex Schur triangular matrix T (without U) for a 4x4 float matrix.
// Returns a pair: first is the triangular matrix T (empty if input invalid), second is success flag.
std::pair<Eigen::MatrixXcf, bool> computeComplexSchurT(const Eigen::MatrixXcf& A) {
    // Ensure the input is exactly 4x4; if not, return failure.
    if (A.rows() != 4 || A.cols() != 4) {
        return {Eigen::MatrixXcf(0,0), false};
    }
    // Check for non-finite entries.
    if (!A.allFinite()) {
        return {Eigen::MatrixXcf(0,0), false};
    }
    // Perform Schur decomposition without computing U.
    Eigen::ComplexSchur<Eigen::MatrixXcf> schur(A, false);
    if (schur.info() != Eigen::Success) {
        return {Eigen::MatrixXcf(0,0), false};
    }
    // Return the triangular matrix T as a copy.
    return {schur.matrixT(), true};
}

// The core algorithm relies on Eigen's `ComplexSchur` class, which computes the Schur decomposition of a square matrix `A` into `A = U * T * U^H`, where `T` is upper-triangular and `U` is unitary. Since the task requires only `T` and not `U`, we pass `false` to the constructor's second parameter to skip the computation of `U`, saving time and memory. The decomposition is performed in-place on a copy of the input to avoid modifying the const reference. First, validate that the input is 4x4 (assert or return false) and that all entries are finite using `allFinite()`. If any non-finite value is found, return an empty matrix and `false`. Otherwise, create a `ComplexSchur<MatrixXcf>` object with the input matrix and `false`, then check `info()` — if it returns `Success`, extract `matrixT()` and return it with `true`; otherwise return empty and `false`. Edge cases: matrices with repeated eigenvalues are handled correctly; zero and diagonal matrices produce a diagonal `T`; random matrices converge reliably in Eigen for 4x4. The time complexity is dominated by the Schur decomposition, which is \(O(n^3)\) for \(n=4\), so a constant-time operation practically. Space complexity is \(O(n^2)\) for the internal storage and the returned matrix. The implementation must include `<Eigen/Eigenvalues>` and `<utility>` for `std::pair`.
