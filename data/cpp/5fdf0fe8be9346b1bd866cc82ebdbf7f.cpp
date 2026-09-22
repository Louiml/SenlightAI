/*
Write a standalone C++ function that, given two point clouds (as vectors of `std::array<double,3>` representing 3D points) and a correspondence vector (a vector of pairs of indices, where each pair `{i,j}` means that point `i` in the source cloud corresponds to point `j` in the target cloud), computes the optimal rigid transformation (a 4×4 matrix `T` in homogeneous coordinates, stored as a `std::array<std::array<double,4>,4>`) that aligns the source to the target using the singular value decomposition (SVD) based Orthogonal Procrustes method. The function must also filter out correspondences whose Euclidean distance exceeds a given threshold `max_distance` before computing the transformation. Return the transformation matrix and update the correspondence list in-place to contain only the inliers. The input clouds are not modified. Assume both clouds have at least one point, correspondences are valid (within bounds), and you may use standard library only (no PCL or Eigen). Implement your own SVD routine for a 3×3 matrix (e.g., using the Jacobi eigenvalue algorithm) or use a simplified approach for rotation estimation via the Kabsch algorithm (which requires SVD). Provide a robust, numerically stable solution that handles degenerate cases (e.g., collinear points) gracefully.
*/
#include <array>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <stdexcept>

// Type aliases for readability
using Point3D = std::array<double, 3>;
using Correspondence = std::pair<size_t, size_t>;
using Matrix4 = std::array<std::array<double, 4>, 4>;

// Helper: compute Euclidean distance between two points
inline double distance3D(const Point3D& a, const Point3D& b) {
    double dx = a[0] - b[0], dy = a[1] - b[1], dz = a[2] - b[2];
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

// Jacobi SVD for 3x3 symmetric matrix. Returns U, S, V such that A = U * diag(S) * V^T.
// We only need it for H^T H, but provide general symmetric eigensolver.
void jacobiEigen(const std::array<std::array<double,3>,3>& A,
                 std::array<std::array<double,3>,3>& vec,
                 std::array<double,3>& val) {
    // Initialize vec as identity
    for (int i=0; i<3; ++i)
        for (int j=0; j<3; ++j)
            vec[i][j] = (i==j) ? 1.0 : 0.0;
    // Copy A to temp
    std::array<std::array<double,3>,3> B = A;
    const int MAX_ITER = 50;
    for (int iter=0; iter<MAX_ITER; ++iter) {
        // Find largest off-diagonal
        int p=0, q=1;
        double max_off = std::fabs(B[p][q]);
        for (int i=0; i<3; ++i)
            for (int j=i+1; j<3; ++j)
                if (std::fabs(B[i][j]) > max_off) { max_off = std::fabs(B[i][j]); p=i; q=j; }
        if (max_off < 1e-12) break;
        // Compute rotation angle
        double theta = (B[q][q] - B[p][p]) / (2.0 * B[p][q]);
        double t = (theta >= 0) ? 1.0 / (theta + std::sqrt(1.0 + theta*theta)) : 1.0 / (theta - std::sqrt(1.0 + theta*theta));
        double c = 1.0 / std::sqrt(1.0 + t*t);
        double s = t * c;
        // Apply rotation to B and vec
        for (int k=0; k<3; ++k) {
            double Bkp = B[k][p], Bkq = B[k][q];
            B[k][p] = c*Bkp - s*Bkq;
            B[k][q] = s*Bkp + c*Bkq;
        }
        for (int k=0; k<3; ++k) {
            double Bpk = B[p][k], Bqk = B[q][k];
            B[p][k] = c*Bpk - s*Bqk;
            B[q][k] = s*Bpk + c*Bqk;
        }
        for (int k=0; k<3; ++k) {
            double Vkp = vec[k][p], Vkq = vec[k][q];
            vec[k][p] = c*Vkp - s*Vkq;
            vec[k][q] = s*Vkp + c*Vkq;
        }
    }
    // Extract eigenvalues from diagonal of B
    for (int i=0; i<3; ++i) val[i] = B[i][i];
    // Sort eigenvalues descending and corresponding eigenvectors
    for (int i=0; i<3; ++i) {
        for (int j=i+1; j<3; ++j) {
            if (val[j] > val[i]) {
                std::swap(val[i], val[j]);
                for (int r=0; r<3; ++r) std::swap(vec[r][i], vec[r][j]);
            }
        }
    }
}

// Compute rigid transformation that best aligns source to target given correspondences.
// Filters correspondences with distance > max_distance (in-place modification).
// Returns 4x4 homogeneous transformation matrix.
Matrix4 computeRigidTransformSVD(
    const std::vector<Point3D>& source,
    const std::vector<Point3D>& target,
    std::vector<Correspondence>& correspondences,
    double max_distance) {

    // Filter correspondences by distance
    std::vector<Correspondence> filtered;
    filtered.reserve(correspondences.size());
    for (const auto& corr : correspondences) {
        if (distance3D(source[corr.first], target[corr.second]) <= max_distance) {
            filtered.push_back(corr);
        }
    }
    correspondences.swap(filtered);
    if (correspondences.empty()) {
        // Degenerate: return identity
        Matrix4 I{};
        for (int i=0; i<4; ++i) I[i][i] = 1.0;
        return I;
    }

    // Compute centroids
    Point3D centroid_src = {0,0,0};
    Point3D centroid_tgt = {0,0,0};
    for (const auto& corr : correspondences) {
        for (int d=0; d<3; ++d) {
            centroid_src[d] += source[corr.first][d];
            centroid_tgt[d] += target[corr.second][d];
        }
    }
    double n = static_cast<double>(correspondences.size());
    for (int d=0; d<3; ++d) {
        centroid_src[d] /= n;
        centroid_tgt[d] /= n;
    }

    // Build cross-covariance matrix H = sum (src_i - centroid_src) * (tgt_i - centroid_tgt)^T
    std::array<std::array<double,3>,3> H{};
    for (int i=0; i<3; ++i)
        for (int j=0; j<3; ++j)
            H[i][j] = 0.0;
    for (const auto& corr : correspondences) {
        Point3D s, t;
        for (int d=0; d<3; ++d) {
            s[d] = source[corr.first][d] - centroid_src[d];
            t[d] = target[corr.second][d] - centroid_tgt[d];
        }
        for (int i=0; i<3; ++i)
            for (int j=0; j<3; ++j)
                H[i][j] += s[i] * t[j];
    }

    // Compute SVD of H using Jacobi on H^T H
    // A = H^T * H (3x3 symmetric)
    std::array<std::array<double,3>,3> A{};
    for (int i=0; i<3; ++i)
        for (int j=0; j<3; ++j) {
            double sum = 0.0;
            for (int k=0; k<3; ++k) sum += H[k][i] * H[k][j];
            A[i][j] = sum;
        }
    std::array<std::array<double,3>,3> V;
    std::array<double,3> S;
    jacobiEigen(A, V, S); // V is right singular vectors, S are singular values squared (but we use sqrt)

    // Compute U = H * V, then normalize columns
    std::array<std::array<double,3>,3> U{};
    for (int i=0; i<3; ++i)
        for (int j=0; j<3; ++j) {
            double sum = 0.0;
            for (int k=0; k<3; ++k) sum += H[i][k] * V[k][j];
            U[i][j] = sum;
        }
    // Normalize U columns, handle near-zero singular values
    for (int j=0; j<3; ++j) {
        double norm = std::sqrt(U[0][j]*U[0][j] + U[1][j]*U[1][j] + U[2][j]*U[2][j]);
        if (norm < 1e-12) {
            // Singular value zero; set arbitrary orthonormal column (cross product of other two)
            int j1 = (j+1)%3, j2 = (j+2)%3;
            double cx = U[1][j1]*U[2][j2] - U[2][j1]*U[1][j2];
            double cy = U[2][j1]*U[0][j2] - U[0][j1]*U[2][j2];
            double cz = U[0][j1]*U[1][j2] - U[1][j1]*U[0][j2];
            norm = std::sqrt(cx*cx + cy*cy + cz*cz);
            U[0][j] = cx/norm; U[1][j] = cy/norm; U[2][j] = cz/norm;
        } else {
            double inv = 1.0 / norm;
            U[0][j] *= inv; U[1][j] *= inv; U[2][j] *= inv;
        }
    }

    // Compute rotation R = V * U^T
    std::array<std::array<double,3>,3> R{};
    for (int i=0; i<3; ++i)
        for (int j=0; j<3; ++j) {
            double sum = 0.0;
            for (int k=0; k<3; ++k) sum += V[i][k] * U[j][k]; // U^T[k][j] = U[j][k]
            R[i][j] = sum;
        }

    // Ensure right-handed coordinate system: if det(R) < 0, flip sign of last column of V (or last row of U^T)
    double det = R[0][0]*(R[1][1]*R[2][2] - R[1][2]*R[2][1])
               - R[0][1]*(R[1][0]*R[2][2] - R[1][2]*R[2][0])
               + R[0][2]*(R[1][0]*R[2][1] - R[1][1]*R[2][0]);
    if (det < 0) {
        // Flip last column of U (since R = V * U^T, flipping last column of U^T means flipping last row of U?)
        // Actually flip last column of R directly by flipping sign of last column of V*U^T? Simpler: recompute with sign change on last column of U.
        for (int i=0; i<3; ++i) U[i][2] = -U[i][2];
        // Recompute R
        for (int i=0; i<3; ++i)
            for (int j=0; j<3; ++j) {
                double sum = 0.0;
                for (int k=0; k<3; ++k) sum += V[i][k] * U[j][k];
                R[i][j] = sum;
            }
    }

    // Translation: t = centroid_tgt - R * centroid_src
    Point3D t{};
    for (int i=0; i<3; ++i) {
        double sum = 0.0;
        for (int k=0; k<3; ++k) sum += R[i][k] * centroid_src[k];
        t[i] = centroid_tgt[i] - sum;
    }

    // Build 4x4 matrix
    Matrix4 T{};
    for (int i=0; i<4; ++i)
        for (int j=0; j<4; ++j)
            T[i][j] = (i==j) ? 1.0 : 0.0;
    for (int i=0; i<3; ++i)
        for (int j=0; j<3; ++j)
            T[i][j] = R[i][j];
    for (int i=0; i<3; ++i) T[i][3] = t[i];
    return T;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <array>

// Assume the solution function is declared above.
// The following are helper functions for testing.

static bool isClose(double a, double b, double eps=1e-4) {
    return std::fabs(a - b) < eps;
}

static bool isMatrixClose(const Matrix4& a, const Matrix4& b, double eps=1e-4) {
    for (int i=0; i<4; ++i)
        for (int j=0; j<4; ++j)
            if (!isClose(a[i][j], b[i][j], eps)) return false;
    return true;
}

int main() {
    // Test 1: Identity transformation (no change)
    {
        std::vector<Point3D> src = {{1,0,0}, {0,1,0}, {0,0,1}, {1,1,1}};
        std::vector<Point3D> tgt = {{1,0,0}, {0,1,0}, {0,0,1}, {1,1,1}};
        std::vector<Correspondence> corr = {{0,0},{1,1},{2,2},{3,3}};
        Matrix4 T = computeRigidTransformSVD(src, tgt, corr, 10.0);
        assert(isMatrixClose(T, Matrix4{{
            {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}
        }}));
        assert(corr.size() == 4); // all inliers
    }

    // Test 2: Pure translation
    {
        std::vector<Point3D> src = {{1,2,3}, {4,5,6}, {7,8,9}};
        std::vector<Point3D> tgt = {{2,3,4}, {5,6,7}, {8,9,10}}; // shifted by (1,1,1)
        std::vector<Correspondence> corr = {{0,0},{1,1},{2,2}};
        Matrix4 T = computeRigidTransformSVD(src, tgt, corr, 10.0);
        assert(isClose(T[0][3], 1.0) && isClose(T[1][3], 1.0) && isClose(T[2][3], 1.0));
        // rotation should be identity
        assert(isClose(T[0][0], 1.0) && isClose(T[1][1], 1.0) && isClose(T[2][2], 1.0));
    }

    // Test 3: Pure rotation by 90 degrees about Z axis
    {
        std::vector<Point3D> src = {{1,0,0}, {0,1,0}, {0,0,1}};
        std::vector<Point3D> tgt = {{0,1,0}, {-1,0,0}, {0,0,1}}; // rotated
        std::vector<Correspondence> corr = {{0,0},{1,1},{2,2}};
        Matrix4 T = computeRigidTransformSVD(src, tgt, corr, 10.0);
        // Expected rotation: [[0, -1, 0], [1, 0, 0], [0, 0, 1]]
        assert(isClose(T[0][0], 0.0) && isClose(T[0][1], -1.0));
        assert(isClose(T[1][0], 1.0) && isClose(T[1][1], 0.0));
        assert(isClose(T[2][2], 1.0));
        assert(isClose(T[0][3], 0.0) && isClose(T[1][3], 0.0) && isClose(T[2][3], 0.0));
    }

    // Test 4: Filtering out correspondences with large distance
    {
        std::vector<Point3D> src = {{0,0,0}, {10,0,0}, {20,0,0}};
        std::vector<Point3D> tgt = {{0,0,0}, {10,0,0}, {100,0,0}}; // third point far away
        std::vector<Correspondence> corr = {{0,0},{1,1},{2,2}};
        Matrix4 T = computeRigidTransformSVD(src, tgt, corr, 1.0); // only first two pass
        assert(corr.size() == 2);
        assert(corr[0].first == 0 && corr[0].second == 0);
        assert(corr[1].first == 1 && corr[1].second == 1);
        // Transformation should align first two points exactly
        assert(isClose(T[0][3], 0.0) && isClose(T[1][3], 0.0) && isClose(T[2][3], 0.0));
    }

    // Test 5: Degenerate case – all correspondences filtered out
    {
        std::vector<Point3D> src = {{0,0,0}};
        std::vector<Point3D> tgt = {{5,5,5}};
        std::vector<Correspondence> corr = {{0,0}};
        Matrix4 T = computeRigidTransformSVD(src, tgt, corr, 0.1); // distance too large
        assert(corr.empty());
        assert(isMatrixClose(T, Matrix4{{
            {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}
        }}));
    }

    // Test 6: Translation with rotation and noise, check residuals
    {
        // Source points
        std::vector<Point3D> src = {{1,0,0}, {0,1,0}, {0,0,1}};
        // Target: rotate 45 deg about Z, then translate (0.5, -1, 2)
        double cos45 = std::sqrt(0.5), sin45 = std::sqrt(0.5);
        Matrix4 R = {{
            {cos45, -sin45, 0, 0.5},
            {sin45, cos45, 0, -1.0},
            {0, 0, 1, 2.0},
            {0, 0, 0, 1}
        }};
        std::vector<Point3D> tgt;
        for (const auto& p : src) {
            Point3D q;
            for (int i=0; i<3; ++i)
                q[i] = R[i][0]*p[0] + R[i][1]*p[1] + R[i][2]*p[2] + R[i][3];
            tgt.push_back(q);
        }
        std::vector<Correspondence> corr = {{0,0},{1,1},{2,2}};
        Matrix4 T = computeRigidTransformSVD(src, tgt, corr, 10.0);
        assert(isClose(T[0][0], cos45, 1e-3) && isClose(T[0][1], -sin45, 1e-3));
        assert(isClose(T[0][3], 0.5, 1e-3) && isClose(T[1][3], -1.0, 1e-3) && isClose(T[2][3], 2.0, 1e-3));
    }

    return 0;
}
// The core algorithm is the Kabsch–Umeyama method: given two sets of corresponding 3D points, first compute the centroids of each set, then subtract them to get centered coordinates. Build the cross-covariance matrix \(H = \frac{1}{N}\sum_{k=1}^{N} (p_k - \bar{p})(q_k - \bar{q})^T\), where \(p\) are source centered points and \(q\) are target centered points (or vice versa, depending on convention; here we want to align source to target, so we compute \(H = \sum (source_i - \bar{source})(target_i - \bar{target})^T\)). Then perform SVD on \(H\): \(H = U \Sigma V^T\). The optimal rotation is \(R = V U^T\), ensuring a right-handed coordinate system by correcting the determinant: if \(\det(R) < 0\), flip the sign of the last column of \(V\) (or the last row of \(U^T\), depending on convention). The translation is \(t = \bar{target} - R \bar{source}\). For filtering, before computing centroids and H, iterate through the correspondences, compute the Euclidean distance between source[i] and target[j], and erase any correspondence with distance > max_distance. If after filtering no correspondences remain, return an identity matrix (or handle as edge case). For SVD of a 3×3 matrix, implement a simple iterative Jacobi rotation method to diagonalize the symmetric matrix \(H^T H\) or \(H H^T\) to find the left and right singular vectors. A practical approach: compute \(A = H^T H\) (3×3 symmetric), use Jacobi eigenvalue algorithm to get eigenvalues and eigenvectors of \(A\), which give the squared singular values and the right singular vectors \(V\). Then compute \(U = H V\) and normalize columns (if a singular value is near zero, set the corresponding column arbitrarily orthogonal). After obtaining U and V, compute R = V * U^T. Edge cases: if any singular value is zero (rank deficiency), the solution is not unique; still produce a valid rotation by completing orthonormal sets. Time complexity: filtering is O(K) for K correspondences, SVD is O(1) (constant 3×3), overall O(K). Space complexity O(1) aside from the returned matrix.
