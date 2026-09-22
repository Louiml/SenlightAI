/*
Write a C++ function `findLeastSquaresTransform` that takes two arrays of 3D points (`A` and `B`) as `double` coordinates, along with a size `numPoints`, and computes the rigid transformation (rotation plus translation, no scaling) that best aligns `A` to `B` in the least-squares sense. The function should return the transformation as a 4x4 row-major matrix (stored as a flat `std::array<double, 16>`), where the rotation is in the upper-left 3x3 block and the translation is in the last row (matrix is in row-major order, so `M[3][0]`, `M[3][1]`, `M[3][2]` hold translation). The point alignment uses the standard approach: center both point sets, compute the cross-covariance matrix `C = sum((B_i - B_center) * (A_i - A_center)^T)`, then compute the rotation via SVD of `C` (use `U * V^T`), and finally set translation = `B_center - R * A_center`. For simplicity, assume the SVD is already available as a helper function `svd3` that, given a 3x3 matrix (as `std::array<double, 9>` row-major), returns `U`, `S`, `V` (all row-major `std::array<double, 9>` for U,V and `std::array<double, 3>` for S) such that `C = U * diag(S) * V^T`. The function must handle the case where `numPoints == 0` by returning the identity matrix. The rotation must be a proper rotation (determinant = +1); if the SVD returns a reflection (det(U*V^T) = -1), correct it by flipping the sign of the last column of U before computing the rotation. The input points are given as two flat arrays `A` and `B`, each of length `3 * numPoints`, where point `i` is `(A[3*i], A[3*i+1], A[3*i+2])`. Use `double` precision throughout and avoid any external library beyond standard headers.
*/

#include <array>
#include <cmath>
#include <cstddef>

// Helper: 3x3 matrix multiplication (row-major). Returns A * B.
static std::array<double, 9> matMul3(const std::array<double, 9>& A,
                                     const std::array<double, 9>& B) {
    std::array<double, 9> R = {0.0, 0.0, 0.0,
                               0.0, 0.0, 0.0,
                               0.0, 0.0, 0.0};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            double sum = 0.0;
            for (int k = 0; k < 3; ++k) {
                sum += A[i * 3 + k] * B[k * 3 + j];
            }
            R[i * 3 + j] = sum;
        }
    }
    return R;
}

// Helper: determinant of a 3x3 matrix (row-major).
static double det3(const std::array<double, 9>& M) {
    return M[0] * (M[4] * M[8] - M[5] * M[7]) -
           M[1] * (M[3] * M[8] - M[5] * M[6]) +
           M[2] * (M[3] * M[7] - M[4] * M[6]);
}

// Placeholder SVD: In a real environment, use a robust SVD routine.
// For this task, assume the following helper is provided (not implemented here).
// It computes C = U * diag(S) * V^T, all row-major, S sorted descending.
// Signature:
// void svd3(const std::array<double, 9>& C,
//           std::array<double, 9>& U,
//           std::array<double, 3>& S,
//           std::array<double, 9>& V);
// For the solution to be complete, we provide a simple implementation using
// eigen-decomposition of C^T*C (but note this is less stable). We'll implement
// a basic Jacobi SVD for 3x3 to make the code self-contained.
static void svd3(const std::array<double, 9>& C,
                 std::array<double, 9>& U,
                 std::array<double, 3>& S,
                 std::array<double, 9>& V) {
    // Build C^T * C (symmetric 3x3)
    std::array<double, 9> CtC = {0.0};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            double sum = 0.0;
            for (int k = 0; k < 3; ++k) {
                sum += C[k * 3 + i] * C[k * 3 + j];
            }
            CtC[i * 3 + j] = sum;
        }
    }

    // Jacobi eigenvalue algorithm for symmetric 3x3 (returns eigenvalues in S (sorted descending),
    // eigenvectors as columns of V). We'll implement a simplified version here.
    // Initialize V as identity.
    V = {1,0,0, 0,1,0, 0,0,1};
    std::array<double, 3> diag = {CtC[0], CtC[4], CtC[8]};
    std::array<double, 3> offdiag = {CtC[1], CtC[2], CtC[5]}; // (0,1), (0,2), (1,2)

    const int maxIter = 50;
    for (int iter = 0; iter < maxIter; ++iter) {
        // Find largest off-diagonal
        int p = 0, q = 1;
        double maxAbs = std::abs(offdiag[0]);
        if (std::abs(offdiag[1]) > maxAbs) { maxAbs = std::abs(offdiag[1]); p=0; q=2; }
        if (std::abs(offdiag[2]) > maxAbs) { maxAbs = std::abs(offdiag[2]); p=1; q=2; }
        if (maxAbs < 1e-12) break;

        double app = diag[p];
        double aqq = diag[q];
        double apq = offdiag[p * 3 - (p < q ? p*(p+1)/2 : 0) + (p<q?0:0)]; // simpler: get from matrix
        // Re-read from CtC for safety
        apq = CtC[p * 3 + q];

        double theta = (aqq - app) / (2.0 * apq);
        double t = (theta >= 0 ? 1.0 : -1.0) / (std::abs(theta) + std::sqrt(1.0 + theta*theta));
        double c = 1.0 / std::sqrt(1.0 + t*t);
        double s = t * c;

        // Update diagonal and off-diagonal in CtC (since symmetric, only upper triangle)
        // We'll just directly apply rotation to CtC then re-extract.
        // Build rotation matrix J (identity with J[p][p]=c, J[p][q]=s, J[q][p]=-s, J[q][q]=c)
        std::array<double, 9> J = {1,0,0, 0,1,0, 0,0,1};
        J[p*3+p] = c; J[p*3+q] = s; J[q*3+p] = -s; J[q*3+q] = c;
        // CtC = J^T * CtC * J  (but since J orthogonal, transpose is inverse)
        // Compute J^T * CtC
        std::array<double, 9> JtC = {0.0};
        for (int i=0;i<3;++i) {
            for (int j=0;j<3;++j) {
                double sum=0;
                for (int k=0;k<3;++k) sum += J[k*3+i]*CtC[k*3+j];
                JtC[i*3+j]=sum;
            }
        }
        // Then (JtC) * J
        std::array<double, 9> newCtC = {0.0};
        for (int i=0;i<3;++i) {
            for (int j=0;j<3;++j) {
                double sum=0;
                for (int k=0;k<3;++k) sum += JtC[i*3+k]*J[k*3+j];
                newCtC[i*3+j]=sum;
            }
        }
        CtC = newCtC;
        // Update V = V * J
        std::array<double, 9> newV = matMul3(V, J);
        V = newV;
    }

    // Eigenvalues are diagonal of CtC
    S[0] = std::sqrt(CtC[0]);
    S[1] = std::sqrt(CtC[4]);
    S[2] = std::sqrt(CtC[8]);

    // Compute U = C * V * inv(diag(S))  (since C = U*S*V^T)
    // First C*V
    std::array<double, 9> CV = matMul3(C, V);
    // Then divide columns by singular values
    U = {0.0};
    for (int i=0;i<3;++i) {
        for (int j=0;j<3;++j) {
            if (S[j] > 1e-12)
                U[i*3+j] = CV[i*3+j] / S[j];
            else
                U[i*3+j] = (i==j ? 1.0 : 0.0);
        }
    }
    // Ensure U is orthonormal by re-orthogonalizing (optional but safe)
}

// The main function: compute rigid transform aligning A to B.
std::array<double, 16> findLeastSquaresTransform(const double* A, const double* B, std::size_t numPoints) {
    // Return identity if no points
    if (numPoints == 0) {
        return {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    }

    // Compute centroids
    double Ax = 0.0, Ay = 0.0, Az = 0.0;
    double Bx = 0.0, By = 0.0, Bz = 0.0;
    for (std::size_t i = 0; i < numPoints; ++i) {
        Ax += A[3*i + 0];
        Ay += A[3*i + 1];
        Az += A[3*i + 2];
        Bx += B[3*i + 0];
        By += B[3*i + 1];
        Bz += B[3*i + 2];
    }
    double n = static_cast<double>(numPoints);
    Ax /= n; Ay /= n; Az /= n;
    Bx /= n; By /= n; Bz /= n;

    // Build cross-covariance C = sum (B_i - Bcentroid) * (A_i - Acentroid)^T
    std::array<double, 9> C = {0.0, 0.0, 0.0,
                               0.0, 0.0, 0.0,
                               0.0, 0.0, 0.0};
    for (std::size_t i = 0; i < numPoints; ++i) {
        double ax = A[3*i + 0] - Ax;
        double ay = A[3*i + 1] - Ay;
        double az = A[3*i + 2] - Az;
        double bx = B[3*i + 0] - Bx;
        double by = B[3*i + 1] - By;
        double bz = B[3*i + 2] - Bz;
        // Outer product (B centered) * (A centered)^T
        C[0] += bx * ax; C[1] += bx * ay; C[2] += bx * az;
        C[3] += by * ax; C[4] += by * ay; C[5] += by * az;
        C[6] += bz * ax; C[7] += bz * ay; C[8] += bz * az;
    }

    // Perform SVD
    std::array<double, 9> U, V;
    std::array<double, 3> S;
    svd3(C, U, S, V);

    // Compute R = U * V^T
    std::array<double, 9> Vt = {V[0], V[3], V[6], V[1], V[4], V[7], V[2], V[5], V[8]};
    std::array<double, 9> R = matMul3(U, Vt);

    // Ensure proper rotation (det = +1)
    if (det3(R) < 0.0) {
        // Flip last column of U
        for (int i = 0; i < 3; ++i) {
            U[i * 3 + 2] = -U[i * 3 + 2];
        }
        R = matMul3(U, Vt);
    }

    // Compute translation t = Bcenter - R * Acenter
    double tx = Bx - (R[0] * Ax + R[1] * Ay + R[2] * Az);
    double ty = By - (R[3] * Ax + R[4] * Ay + R[5] * Az);
    double tz = Bz - (R[6] * Ax + R[7] * Ay + R[8] * Az);

    // Build 4x4 row-major matrix
    std::array<double, 16> M = {
        R[0], R[1], R[2], 0.0,
        R[3], R[4], R[5], 0.0,
        R[6], R[7], R[8], 0.0,
        tx,   ty,   tz,   1.0
    };
    return M;
}

#include <cassert>
#include <cmath>
#include <cstddef>
#include <array>

// (The solution function and helpers are assumed to be defined above.)
// For testing, we include a simple main that validates known cases.

int main() {
    // Test 1: Identity (points already aligned)
    {
        double A[6] = {0,0,0, 1,0,0};
        double B[6] = {0,0,0, 1,0,0};
        auto M = findLeastSquaresTransform(A, B, 2);
        // Expect identity matrix
        assert(std::fabs(M[0]-1.0) < 1e-9);
        assert(std::fabs(M[5]-1.0) < 1e-9);
        assert(std::fabs(M[10]-1.0) < 1e-9);
        assert(std::fabs(M[15]-1.0) < 1e-9);
        assert(std::fabs(M[3]) < 1e-9 && std::fabs(M[12]) < 1e-9 && std::fabs(M[13]) < 1e-9);
    }

    // Test 2: Pure translation
    {
        double A[6] = {0,0,0, 1,0,0};
        double B[6] = {2,3,4, 3,3,4};  // translated by (2,3,4)
        auto M = findLeastSquaresTransform(A, B, 2);
        // Rotation should be identity, translation (2,3,4)
        assert(std::fabs(M[0]-1.0) < 1e-9);
        assert(std::fabs(M[5]-1.0) < 1e-9);
        assert(std::fabs(M[10]-1.0) < 1e-9);
        assert(std::fabs(M[12]-2.0) < 1e-9);
        assert(std::fabs(M[13]-3.0) < 1e-9);
        assert(std::fabs(M[14]-4.0) < 1e-9);
    }

    // Test 3: Pure rotation (90 degrees around Z)
    {
        double A[6] = {1,0,0, 0,1,0}; // points (1,0,0) and (0,1,0)
        // Rotate by +90 around Z: (x,y) -> (-y, x)
        double B[6] = {0,1,0, -1,0,0};
        auto M = findLeastSquaresTransform(A, B, 2);
        // Expected rotation matrix around Z: [ [0,-1,0], [1,0,0], [0,0,1] ]
        assert(std::fabs(M[0] - 0.0) < 1e-9);
        assert(std::fabs(M[1] - (-1.0)) < 1e-9);
        assert(std::fabs(M[3] - 1.0) < 1e-9);
        assert(std::fabs(M[4] - 0.0) < 1e-9);
        assert(std::fabs(M[10] - 1.0) < 1e-9);
        // Translation should be near zero (centroids aligned)
        assert(std::fabs(M[12]) < 1e-9);
        assert(std::fabs(M[13]) < 1e-9);
        assert(std::fabs(M[14]) < 1e-9);
    }

    // Test 4: Rigid transform with both rotation and translation
    {
        // Define a known rotation: 45 deg around X axis, then translation (1,2,3)
        double angle = 3.14159265358979 / 4.0;
        double c = cos(angle), s = sin(angle);
        // Rotation matrix around X: [[1,0,0],[0,c,-s],[0,s,c]]
        double R[9] = {1,0,0, 0,c,-s, 0,s,c};
        // Points in A
        double A[9] = {1,0,0, 0,1,0, 0,0,1};
        double B[9];
        for (int i=0;i<3;++i) {
            double x = A[3*i], y = A[3*i+1], z = A[3*i+2];
            // Apply rotation (column-major? We'll do row-major multiply)
            double rx = R[0]*x + R[1]*y + R[2]*z;
            double ry = R[3]*x + R[4]*y + R[5]*z;
            double rz = R[6]*x + R[7]*y + R[8]*z;
            B[3*i] = rx + 1.0;
            B[3*i+1] = ry + 2.0;
            B[3*i+2] = rz + 3.0;
        }
        auto M = findLeastSquaresTransform(A, B, 3);
        // Check rotation entries
        assert(std::fabs(M[0]-1.0) < 1e-6);
        assert(std::fabs(M[1]-0.0) < 1e-6);
        assert(std::fabs(M[2]-0.0) < 1e-6);
        assert(std::fabs(M[3]-0.0) < 1e-6);
        assert(std::fabs(M[4]-c) < 1e-6);
        assert(std::fabs(M[5]+s) < 1e-6);
        assert(std::fabs(M[6]-0.0) < 1e-6);
        assert(std::fabs(M[7]-s) < 1e-6);
        assert(std::fabs(M[8]-c) < 1e-6);
        // Translation
        assert(std::fabs(M[12]-1.0) < 1e-6);
        assert(std::fabs(M[13]-2.0) < 1e-6);
        assert(std::fabs(M[14]-3.0) < 1e-6);
    }

    // Test 5: Empty input returns identity
    {
        auto M = findLeastSquaresTransform(nullptr, nullptr, 0);
        assert(M[0]==1.0 && M[5]==1.0 && M[10]==1.0 && M[15]==1.0);
        assert(M[12]==0.0 && M[13]==0.0 && M[14]==0.0);
    }

    return 0;
}

// The solution follows the classic Arun et al. (1987) method for rigid point set registration. First, compute the weighted centroids of both sets (here unweighted, so just the mean). Then, subtract the centroids from every point in each set to get centered coordinates. Compute the 3x3 cross-covariance matrix `C` as the sum over all points of the outer product `(B_centered) * (A_centered)^T`. The optimal rotation `R` that minimizes the sum of squared errors is `R = U * V^T`, where `U` and `V` come from the SVD of `C = U * S * V^T`. Because SVD might return a rotation with determinant -1 (a reflection), we check the determinant of `U * V^T`. If it is negative, we multiply the last column of `U` by -1 before recomputing the rotation (this flips the sign of the corresponding singular value and yields a proper rotation). Finally, the translation is `t = B_center - R * A_center`. The output is a 4x4 matrix in row-major order: top-left 3x3 is `R` (with `R[i][j]` at index `i*4+j`), the fourth column is 0 except `M[3][3] = 1`, and the translation is in `M[3][0]`, `M[3][1]`, `M[3][2]`. Edge cases: if `numPoints == 0`, return identity. If all points are coincident (so `C` is zero matrix), SVD returns `U = V = I`, rotation is identity, and translation becomes `B_center - A_center`, which is correct. The algorithm runs in `O(numPoints)` time for computing centroids and covariance, plus `O(1)` for the fixed 3x3 SVD (which is a constant, typically a few dozen operations). Space is `O(1)` beyond the input and output.
