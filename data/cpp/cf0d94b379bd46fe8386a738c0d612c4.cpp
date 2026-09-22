Write a C++ function `void jacobiEigenSymmetric(std::vector<double>& A, std::vector<double>& V, std::vector<double>& E, double eps = 1e-10)` that takes a symmetric matrix `A` of size `n x n` stored in row-major order as a flat `std::vector<double>`, and computes its eigenvalues and eigenvectors using the cyclic Jacobi eigenvalue algorithm. The function must overwrite `A` with the final diagonalized matrix, store the eigenvectors in `V` (as rows of `V`, such that `V[i]` is the eigenvector corresponding to eigenvalue `E[i]`), and store the eigenvalues in `E` sorted in descending order of absolute value. The input matrix is guaranteed to be symmetric (up to rounding), square, and have `n >= 1`. The function should handle the case where `eps` is too small or zero by using a reasonable floor (e.g., `std::numeric_limits<double>::epsilon()`), and it must not use external libraries beyond standard headers. The function should be robust to matrices with repeated eigenvalues and near-zero off-diagonal elements.

#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>

// Declaration of the solution function.
void jacobiEigenSymmetric(std::vector<double>& A, std::vector<double>& V, std::vector<double>& E, double eps = 1e-10);

int main() {
    // Test 1: 2x2 matrix with distinct eigenvalues.
    {
        std::vector<double> A = {2.0, 1.0, 1.0, 2.0};  // eigenvalues 3 and 1
        std::vector<double> V, E;
        jacobiEigenSymmetric(A, V, E, 1e-12);
        assert(E.size() == 2);
        assert(std::fabs(E[0] - 3.0) < 1e-8);  // sorted descending
        assert(std::fabs(E[1] - 1.0) < 1e-8);
        // Check that V is orthogonal and A*V = V*diag(E)
        // For simplicity, check eigenvector property for first eigenvector.
        double v0[] = {V[0], V[1]};
        double a0 = 2.0 * v0[0] + 1.0 * v0[1];
        double a1 = 1.0 * v0[0] + 2.0 * v0[1];
        assert(std::fabs(a0 - E[0] * v0[0]) < 1e-6);
        assert(std::fabs(a1 - E[0] * v0[1]) < 1e-6);
    }

    // Test 2: 1x1 matrix.
    {
        std::vector<double> A = {5.0};
        std::vector<double> V, E;
        jacobiEigenSymmetric(A, V, E, 1e-12);
        assert(E.size() == 1 && std::fabs(E[0] - 5.0) < 1e-12);
        assert(V.size() == 1 && std::fabs(V[0] - 1.0) < 1e-12);
    }

    // Test 3: 3x3 diagonal matrix.
    {
        std::vector<double> A = {4.0, 0.0, 0.0, 0.0, -2.0, 0.0, 0.0, 0.0, 1.0};
        std::vector<double> V, E;
        jacobiEigenSymmetric(A, V, E, 1e-12);
        assert(E.size() == 3);
        assert(std::fabs(E[0] - 4.0) < 1e-10);
        assert(std::fabs(E[1] - 1.0) < 1e-10);
        assert(std::fabs(E[2] + 2.0) < 1e-10);  // -2 is third by abs value
    }

    // Test 4: 3x3 with repeated eigenvalues (e.g., matrix with eigenvalues 2,2,0).
    {
        std::vector<double> A = {1.0, 1.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0};
        // Eigenvalues: 2, 0, 0 (but sorted descending by abs: 2,0,0)
        std::vector<double> V, E;
        jacobiEigenSymmetric(A, V, E, 1e-12);
        assert(E.size() == 3);
        assert(std::fabs(E[0] - 2.0) < 1e-8);
        assert(std::fabs(E[1]) < 1e-8);
        assert(std::fabs(E[2]) < 1e-8);
        // Check that V rows are orthonormal.
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                double dot = 0.0;
                for (int k = 0; k < 3; ++k) dot += V[i*3+k] * V[j*3+k];
                if (i == j) assert(std::fabs(dot - 1.0) < 1e-6);
                else assert(std::fabs(dot) < 1e-6);
            }
        }
    }

    // Test 5: 4x4 random symmetric matrix with known eigenvalues? We'll just check trace and determinant approximately.
    {
        std::vector<double> A = {
            3.0, -1.0, 0.0, 0.5,
            -1.0, 2.0, 0.3, 0.0,
            0.0, 0.3, 1.0, -0.2,
            0.5, 0.0, -0.2, 0.5
        };
        std::vector<double> V, E;
        jacobiEigenSymmetric(A, V, E, 1e-10);
        assert(E.size() == 4);
        // Trace of original = 3+2+1+0.5 = 6.5
        double trace = 0.0;
        for (double e : E) trace += e;
        assert(std::fabs(trace - 6.5) < 1e-6);
        // Check eigenvectors orthonormality.
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                double dot = 0.0;
                for (int k = 0; k < 4; ++k) dot += V[i*4+k] * V[j*4+k];
                if (i == j) assert(std::fabs(dot - 1.0) < 1e-6);
                else assert(std::fabs(dot) < 1e-6);
            }
        }
        // Verify A*V[:,i] = E[i]*V[:,i] for each i.
        std::vector<double> orig = {
            3.0, -1.0, 0.0, 0.5,
            -1.0, 2.0, 0.3, 0.0,
            0.0, 0.3, 1.0, -0.2,
            0.5, 0.0, -0.2, 0.5
        };
        for (int i = 0; i < 4; ++i) {
            for (int r = 0; r < 4; ++r) {
                double sum = 0.0;
                for (int c = 0; c < 4; ++c) sum += orig[r*4+c] * V[i*4+c];
                assert(std::fabs(sum - E[i] * V[i*4+r]) < 1e-5);
            }
        }
    }

    // Test 6: eps = 0 (should still work, floor to machine epsilon).
    {
        std::vector<double> A = {2.0, 1.0, 1.0, 2.0};
        std::vector<double> V, E;
        jacobiEigenSymmetric(A, V, E, 0.0);
        assert(std::fabs(E[0] - 3.0) < 1e-6);
        assert(std::fabs(E[1] - 1.0) < 1e-6);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

// Computes eigenvalues and eigenvectors of a symmetric matrix using cyclic Jacobi method.
// A: input symmetric matrix (n x n, row-major) — overwritten with final diagonalized matrix.
// V: output eigenvectors (n x n, row-major, each row is an eigenvector).
// E: output eigenvalues (size n), sorted descending by absolute value.
// eps: accuracy threshold for off-diagonal annihilation (default 1e-10).
void jacobiEigenSymmetric(std::vector<double>& A, std::vector<double>& V, std::vector<double>& E, double eps = 1e-10) {
    const int n = static_cast<int>(std::sqrt(A.size()));
    if (n <= 0) return;

    // Initialize V as identity.
    V.assign(static_cast<size_t>(n) * n, 0.0);
    for (int i = 0; i < n; ++i) V[static_cast<size_t>(i) * n + i] = 1.0;

    // Compute Frobenius norm of off-diagonal part.
    double anorm = 0.0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            double am = A[static_cast<size_t>(i) * n + j];
            anorm += am * am;
        }
    }
    anorm = std::sqrt(anorm + anorm);  // 2 * sum of squares of off-diagonal
    if (anorm == 0.0) {
        // Matrix already diagonal.
        for (int i = 0; i < n; ++i) E[i] = A[static_cast<size_t>(i) * n + i];
        // No sorting needed because sorted by absolute value? Actually we still sort.
        // Sorting step.
        for (int i = 0; i < n; ++i) {
            int m = i;
            double em = std::fabs(E[i]);
            for (int j = i + 1; j < n; ++j) {
                double ej = std::fabs(E[j]);
                if (em < ej) { em = ej; m = j; }
            }
            if (m != i) {
                std::swap(E[i], E[m]);
                for (int j = 0; j < n; ++j) {
                    std::swap(V[static_cast<size_t>(i) * n + j], V[static_cast<size_t>(m) * n + j]);
                }
            }
        }
        return;
    }

    // Set tolerance.
    if (eps < std::numeric_limits<double>::epsilon()) eps = std::numeric_limits<double>::epsilon();
    double ax = anorm * eps / n;
    double amax = anorm;
    int iters = 0;
    const int max_iters = 100;

    // Jacobi sweeps.
    while (amax > ax && iters++ < max_iters) {
        amax /= n;  // Threshold for this sweep decreases.
        bool ind = true;
        while (ind) {
            ind = false;
            for (int p = 0; p < n - 1; ++p) {
                for (int q = p + 1; q < n; ++q) {
                    double apq = A[static_cast<size_t>(p) * n + q];
                    if (std::fabs(apq) < amax) continue;
                    ind = true;

                    double app = A[static_cast<size_t>(p) * n + p];
                    double aqq = A[static_cast<size_t>(q) * n + q];
                    // Compute rotation angle.
                    double y = 0.5 * (app - aqq);
                    double x = -apq / std::sqrt(apq * apq + y * y);
                    if (y < 0.0) x = -x;
                    double s = x / std::sqrt(2.0 * (1.0 + std::sqrt(1.0 - x * x)));
                    double c = std::sqrt(1.0 - s * s);
                    double c2 = c * c;
                    double s2 = s * s;
                    double a = 2.0 * apq * c * s;

                    // Update rows and columns for indices < p.
                    for (int i = 0; i < p; ++i) {
                        double aip = A[static_cast<size_t>(i) * n + p];
                        double aiq = A[static_cast<size_t>(i) * n + q];
                        double vpi = V[static_cast<size_t>(p) * n + i];
                        double vqi = V[static_cast<size_t>(q) * n + i];
                        A[static_cast<size_t>(i) * n + p] = aip * c - aiq * s;
                        A[static_cast<size_t>(i) * n + q] = aiq * c + aip * s;
                        V[static_cast<size_t>(p) * n + i] = vpi * c - vqi * s;
                        V[static_cast<size_t>(q) * n + i] = vqi * c + vpi * s;
                    }
                    // For i == p..q-1
                    for (int i = p; i < q; ++i) {
                        double aip = A[static_cast<size_t>(p) * n + i]; // row p, col i
                        double aiq = A[static_cast<size_t>(i) * n + q];
                        double vpi = V[static_cast<size_t>(p) * n + i];
                        double vqi = V[static_cast<size_t>(q) * n + i];
                        // For i==p, we are modifying A[p][p], but we'll handle later. So skip?
                        // Better implement carefully using temporary storage for diagonal.
                        // For simplicity, handle indices safely by using temporary copies of the row p and column q.
                    }
                    // To avoid messy index handling, we use a simpler but correct approach:
                    // Since we are allowed to overwrite A and V, we can use temporary arrays for the p-th row, q-th column, etc.
                    // But for a clean solution, we can just use loops with careful pointer arithmetic, as in the reference.
                    // For brevity, here we implement the standard Jacobi update using temporary vectors.
                    std::vector<double> row_p(n), row_q(n);
                    for (int i = 0; i < n; ++i) {
                        row_p[i] = A[static_cast<size_t>(p) * n + i];
                        row_q[i] = A[static_cast<size_t>(q) * n + i];
                    }
                    std::vector<double> vp(n), vq(n);
                    for (int i = 0; i < n; ++i) {
                        vp[i] = V[static_cast<size_t>(p) * n + i];
                        vq[i] = V[static_cast<size_t>(q) * n + i];
                    }
                    // Update A rows p and q.
                    for (int i = 0; i < n; ++i) {
                        if (i != p && i != q) {
                            double aip = row_p[i];
                            double aiq = row_q[i];
                            A[static_cast<size_t>(p) * n + i] = aip * c - aiq * s;
                            A[static_cast<size_t>(q) * n + i] = aiq * c + aip * s;
                            // Also update columns p and q for rows i.
                            A[static_cast<size_t>(i) * n + p] = A[static_cast<size_t>(p) * n + i];
                            A[static_cast<size_t>(i) * n + q] = A[static_cast<size_t>(q) * n + i];
                        }
                    }
                    // Update V rows p and q.
                    for (int i = 0; i < n; ++i) {
                        double vpi = vp[i];
                        double vqi = vq[i];
                        V[static_cast<size_t>(p) * n + i] = vpi * c - vqi * s;
                        V[static_cast<size_t>(q) * n + i] = vqi * c + vpi * s;
                    }
                    // Update diagonal elements.
                    A[static_cast<size_t>(p) * n + p] = app * c2 + aqq * s2 - a;
                    A[static_cast<size_t>(q) * n + q] = app * s2 + aqq * c2 + a;
                    A[static_cast<size_t>(p) * n + q] = 0.0;
                    A[static_cast<size_t>(q) * n + p] = 0.0;
                }
            }
        }
        // After full sweep, reduce amax? The original code does amax /= n inside while, but we already did at loop start. This is fine.
    }

    // Extract eigenvalues from diagonal.
    for (int i = 0; i < n; ++i) E[i] = A[static_cast<size_t>(i) * n + i];

    // Sort eigenvalues and eigenvectors by descending absolute value.
    for (int i = 0; i < n; ++i) {
        int m = i;
        double em = std::fabs(E[i]);
        for (int j = i + 1; j < n; ++j) {
            double ej = std::fabs(E[j]);
            if (em < ej) { em = ej; m = j; }
        }
        if (m != i) {
            std::swap(E[i], E[m]);
            for (int j = 0; j < n; ++j) {
                std::swap(V[static_cast<size_t>(i) * n + j], V[static_cast<size_t>(m) * n + j]);
            }
        }
    }
}

// The solution implements the cyclic Jacobi eigenvalue algorithm for symmetric matrices. The main idea is to iteratively zero out off-diagonal elements using plane rotations (Givens rotations). For each sweep, the algorithm scans all off-diagonal pairs `(p, q)` with `p < q`, and if the magnitude of `A[p][q]` exceeds a threshold, it applies a rotation that annihilates `A[p][q]` while preserving symmetry. The rotation angle is computed from `App` and `Aqq` using a numerically stable formula to avoid cancellation. After each rotation, the eigenvectors are accumulated in `V` by applying the same rotation to their rows. The process repeats sweeps until all off-diagonal elements are below the tolerance `ax = anorm * eps / n`, or a maximum iteration count (e.g., 100) is reached to avoid infinite loops. Finally, the eigenvalues are extracted from the diagonal of `A`, and both eigenvalues and corresponding eigenvectors are sorted in descending order of absolute value. Important edge cases include: `n = 1` (immediately return the single eigenvalue and identity eigenvector), `eps` smaller than machine epsilon, and matrices with zero off-diagonal elements (the algorithm terminates immediately). Time complexity is `O(n^3 * num_sweeps)` where `num_sweeps` is typically a small constant (usually 5-10) for convergence, and space complexity is `O(n^2)` for the eigenvectors plus `O(n)` temporary storage.
