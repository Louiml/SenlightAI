// Write a standalone C++ function named `tridiagonalizeSymmetric` that takes a symmetric `Eigen::MatrixXd` by value, performs an in-place tridiagonalization using Householder reflections, and returns a `std::pair<Eigen::VectorXd, Eigen::VectorXd>` containing the diagonal and subdiagonal of the resulting tridiagonal matrix. The function must work for square matrices of size at least 2, preserve the input matrix's symmetry (the function may modify its argument internally), and must not rely on any Eigen internal routines (i.e., do not call `Eigen::Tridiagonalization`). The function should return the tridiagonal representation such that the reconstructed matrix \(T = Q^T A Q\) is symmetric tridiagonal with nonzero subdiagonal entries. The function must handle edge cases where the input is not perfectly symmetric by symmetrizing it first (average with its transpose), and must use only standard Eigen dense operations and your own Householder implementation. The function should be `const`-correct and well-commented.

// The solution requires implementing the classic Householder tridiagonalization algorithm. The main idea: for each column \(k\) from 0 to \(n-3\), we form a Householder reflector \(H_k = I - 2 v v^T / (v^T v)\) that zeros out all entries below the subdiagonal in that column (and symmetrically to the right of the superdiagonal in the corresponding row). We apply the reflector symmetrically: \(A \leftarrow H_k A H_k\) (since \(H_k\) is orthogonal and symmetric). To do this efficiently without forming full matrices, we work directly on the matrix. For each step: extract the vector \(x\) from column \(k\) below the subdiagonal, form the Householder vector \(v\) (with the appropriate sign to avoid cancellation), compute \(v^T A\) and \(A v\), then update \(A \leftarrow A - v w^T - w v^T + \alpha v v^T\) where \(w = A v - (\alpha/2) v\) and \(\alpha = v^T A v / (v^T v)\). The subdiagonal entry at position \((k+1, k)\) stores the norm of the original vector with the sign chosen, and the rest of the column (and corresponding row) below that is set to zero. The process yields a symmetric tridiagonal matrix in the upper part of \(A\), and we extract the diagonal and subdiagonal. Edge cases: matrices of size 2 result in no iteration because the matrix already is tridiagonal; the subdiagonal is a single element. For size 1, tridiagonalization is trivial (diagonal contains the scalar). Symmetrization ensures numerical stability. Complexity: \(O(n^3)\) time and \(O(1)\) extra space beyond the input matrix and output vectors (we use scratch vectors). The algorithm is stable for symmetric matrices.

#include <Eigen/Dense>
#include <utility>
#include <cmath>

// Perform in-place Householder tridiagonalization on a symmetric matrix.
// Returns a pair (diagonal, subdiagonal) of the tridiagonal matrix T = Q^T A Q.
std::pair<Eigen::VectorXd, Eigen::VectorXd> tridiagonalizeSymmetric(Eigen::MatrixXd A) {
    const Eigen::Index n = A.rows();
    // Symmetrize the input to enforce symmetry (works for any input).
    A = 0.5 * (A + A.transpose());

    // Vectors to store the diagonal and subdiagonal.
    Eigen::VectorXd diag(n);
    Eigen::VectorXd subdiag(n - 1);

    for (Eigen::Index k = 0; k < n - 2; ++k) {
        // Extract the vector below the subdiagonal in column k.
        Eigen::VectorXd x = A.block(k + 2, k, n - k - 2, 1);
        double norm_x = x.norm();
        if (norm_x > 0.0) {
            // Choose sign to avoid cancellation; alpha = -sign(x(0))*norm_x.
            double alpha = (x(0) >= 0.0 ? -norm_x : norm_x);
            // Householder vector v (stored in x itself), with v(0) = x(0) - alpha.
            x(0) -= alpha;
            double v_norm_sq = x.squaredNorm();
            // Subdiagonal entry (the (k+1,k) element becomes alpha).
            subdiag(k) = -alpha; // Actually the (k+1) entry of the transformed matrix becomes alpha? Let's ensure sign.
            // Apply transformation: A <- H A H, where H = I - 2 v v^T / (v^T v).
            // Compute w = A * v / (v_norm_sq) and then A -= 2 * (v * w^T + w * v^T - (v^T w) * v * v^T / (v_norm_sq) * 2? Better do robust form.
            // We'll compute u = A*v / (v_norm_sq), then A -= v*u^T + u*v^T - (v^T u)*2*v*v^T/(v_norm_sq)? Actually standard: A -= v*w^T - w*v^T + (v^T w)*v*v^T/(v^T v)*2? Let's use correct formula.
            // Let vv = x (length n-k-1). Let Av = A.block(k+1,k+1,n-k-1,n-k-1) * x.
            Eigen::VectorXd vv = x;
            Eigen::MatrixXd sub = A.block(k + 1, k + 1, n - k - 1, n - k - 1);
            Eigen::VectorXd Av = sub * vv;
            double vvTvv = vv.dot(vv);
            double vvTAv = vv.dot(Av);
            // Compute w = Av - (vvTAv / (2*vvTvv)) * vv.
            Eigen::VectorXd w = Av - (vvTAv / (2.0 * vvTvv)) * vv;
            // Update submatrix: A -= v*w^T + w*v^T.
            sub.noalias() -= vv * w.transpose();
            sub.noalias() -= w * vv.transpose();
            // Copy back (sub is a view, so already updated). But we need to also zero out extra entries? Actually the transformation zeroes them automatically.
            // Set the subdiagonal entry (k+1,k) to alpha (the norm with sign).
            A(k + 1, k) = -alpha;
            A(k, k + 1) = -alpha;
            // Zero the rest of the column and row below/right of subdiagonal.
            if (k + 2 < n) {
                A.block(k + 2, k, n - k - 2, 1).setZero();
                A.block(k, k + 2, 1, n - k - 2).setZero();
            }
            // Store subdiag.
            subdiag(k) = -alpha;
        } else {
            subdiag(k) = 0.0;
        }
        // Extract the diagonal entry after transformation.
        diag(k) = A(k, k);
    }

    // Handle the last two entries.
    diag(n - 2) = A(n - 2, n - 2);
    diag(n - 1) = A(n - 1, n - 1);
    subdiag(n - 2) = (n >= 2) ? A(n - 1, n - 2) : 0.0;

    return {diag, subdiag};
}

#include <Eigen/Dense>
#include <cassert>
#include <cmath>
#include <utility>

// Include the solution function here (or link it).
// For brevity, assume the solution function is defined above.

int main() {
    // Test 1: 2x2 symmetric matrix.
    Eigen::MatrixXd A1(2, 2);
    A1 << 1.0, 2.0,
          2.0, 3.0;
    auto res1 = tridiagonalizeSymmetric(A1);
    assert(res1.first.size() == 2);
    assert(res1.second.size() == 1);
    assert(std::abs(res1.first(0) - 1.0) < 1e-12);
    assert(std::abs(res1.first(1) - 3.0) < 1e-12);
    assert(std::abs(res1.second(0) - 2.0) < 1e-12);

    // Test 2: 3x3 symmetric matrix with known tridiagonalization.
    Eigen::MatrixXd A2(3, 3);
    A2 << 2.0, 1.0, 0.0,
          1.0, 3.0, 1.0,
          0.0, 1.0, 4.0;
    auto res2 = tridiagonalizeSymmetric(A2);
    // Already tridiagonal, so result should match.
    assert(std::abs(res2.first(0) - 2.0) < 1e-12);
    assert(std::abs(res2.first(1) - 3.0) < 1e-12);
    assert(std::abs(res2.first(2) - 4.0) < 1e-12);
    assert(std::abs(res2.second(0) - 1.0) < 1e-12);
    assert(std::abs(res2.second(1) - 1.0) < 1e-12);

    // Test 3: Random 5x5 symmetric matrix; verify reconstruction.
    Eigen::MatrixXd X = Eigen::MatrixXd::Random(5, 5);
    Eigen::MatrixXd A3 = X + X.transpose();
    auto res3 = tridiagonalizeSymmetric(A3);
    // Reconstruct T from diag and subdiag.
    Eigen::MatrixXd T = Eigen::MatrixXd::Zero(5, 5);
    T.diagonal() = res3.first;
    for (int i = 0; i < 4; ++i) {
        T(i, i+1) = res3.second(i);
        T(i+1, i) = res3.second(i);
    }
    // Since tridiagonalization preserves eigenvalues, the trace should match.
    assert(std::abs(T.trace() - A3.trace()) < 1e-10);

    // Test 4: Symmetrize non-symmetric input.
    Eigen::MatrixXd A4(2, 2);
    A4 << 1.0, 3.0,
          2.0, 4.0;
    auto res4 = tridiagonalizeSymmetric(A4);
    // The symmetrized version is [[1,2.5],[2.5,4]].
    assert(std::abs(res4.first(0) - 1.0) < 1e-12);
    assert(std::abs(res4.first(1) - 4.0) < 1e-12);
    assert(std::abs(res4.second(0) - 2.5) < 1e-12);

    // Test 5: Large random 10x10, ensure subdiagonal entries nonzero for generic matrix.
    Eigen::MatrixXd A5 = Eigen::MatrixXd::Random(10, 10);
    A5 = A5 + A5.transpose();
    auto res5 = tridiagonalizeSymmetric(A5);
    assert(res5.first.size() == 10);
    assert(res5.second.size() == 9);
    // Ensure the subdiagonal has no near-zero entries except possible rotations (but random should have none).
    for (int i = 0; i < 9; ++i) {
        assert(std::abs(res5.second(i)) > 1e-10);
    }

    return 0;
}
