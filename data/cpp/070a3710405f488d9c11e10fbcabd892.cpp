// Write a C++ function `estimateCameraPose` that takes as input a vector of 3D world points (each represented as `std::array<double, 3>`), a vector of corresponding 2D image points (each represented as `std::array<double, 2>`), and a 3x3 camera intrinsic matrix `K` (represented as `std::array<std::array<double, 3>, 3>`). The function must estimate and return the camera pose as a `Pose` struct containing a 3x3 rotation matrix `R` and a 3x1 translation vector `t`, such that for each correspondence, the projected point approximately matches the image point under the pinhole model. The estimation must use the Direct Linear Transform (DLT) approach: build the measurement matrix `A` from the correspondences (each point contributes two rows), solve the homogeneous least-squares problem via the eigenvector corresponding to the smallest eigenvalue of `AᵀA`, reshape to obtain the 3x4 projection matrix `P`, then decompose `P = K [R | t]` to recover `R` and `t`. Return the pose as `Pose{R, t}`. Assume at least 6 correspondences are provided (the function does not need to check this, but the caller will provide valid data). The rotation matrix output does not need to be orthonormalized. Use only standard C++ (no external libraries); you may use `std::vector`, `std::array`, and basic linear algebra implemented manually (e.g., matrix multiplication, transpose, eigen-decomposition of a symmetric 12x12 matrix using Jacobi eigenvalue algorithm or a simpler power iteration-based approach—the task only requires correctness of the smallest eigenvector, so you may compute it via inverse iteration). You are allowed to use `std::numeric_limits` for initialization.

The solution requires solving a homogeneous linear system of the form `A p = 0` where `p` is the 12-vector of the flattened projection matrix (row-major, so `p = [P11, P12, P13, P14, P21, ..., P34]`). For each 3D-to-2D correspondence `(X, Y, Z, 1) ↔ (u, v)`, two rows of `A` are formed as in the classic DLT: row `2i` is `[X, Y, Z, 1, 0, 0, 0, 0, -u*X, -u*Y, -u*Z, -u]` and row `2i+1` is `[0, 0, 0, 0, X, Y, Z, 1, -v*X, -v*Y, -v*Z, -v]`. Because we need a non-trivial solution (the zero vector is trivial), we minimize `||A p||²` subject to `||p||=1`. This is solved by finding the eigenvector of `AᵀA` (a symmetric 12x12 matrix) corresponding to the smallest eigenvalue. Since `AᵀA` is symmetric, we can use the Jacobi eigenvalue algorithm to compute all eigenvalues and eigenvectors; then pick the eigenvector with the smallest eigenvalue. After obtaining `p`, we reshape it (with `P(i,j) = p[i*4 + j]`) into a 3x4 matrix. Then the intrinsic matrix `K` is extracted: `P = K [R | t]`, so `[R | t] = K⁻¹ P`. We compute `K⁻¹` as the inverse of the given 3x3 matrix (which is upper triangular with zero below diagonal, but we implement general 3x3 inverse using cofactors). The first 3 columns of `K⁻¹ P` give `R`, the last column gives `t`. Edge cases: ensure at least 6 correspondences (caller guarantees this); the eigen-decomposition must handle numerical stability—Jacobi is robust for symmetric matrices. Time complexity is `O(n)` for building `A` (where `n` is number of correspondences), then `O(d³)` for the eigen-decomposition with `d=12` (constant, but effectively `O(12³)` which is constant-time asymptotically), so overall `O(n)` time. Space complexity is `O(n)` for storing the input and the `A` matrix, plus `O(1)` extra for the fixed-size matrices (24 rows × 12 columns, but we can build `AᵀA` directly in `O(1)` since it's 12x12, by accumulating outer products—this reduces space to `O(1)`). For the reference solution, we choose to build `AᵀA` directly as 12x12 matrix without storing `A` to save space, which is still correct.

#include <array>
#include <vector>
#include <cmath>
#include <limits>

struct Pose {
    std::array<std::array<double, 3>, 3> R;
    std::array<double, 3> t;
};

// Helper: multiply two 3x3 matrices.
std::array<std::array<double, 3>, 3> matMul3(
    const std::array<std::array<double, 3>, 3>& A,
    const std::array<std::array<double, 3>, 3>& B) {
    std::array<std::array<double, 3>, 3> C{};
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}

// Helper: inverse of a 3x3 matrix (using adjugate method).
std::array<std::array<double, 3>, 3> matInv3(
    const std::array<std::array<double, 3>, 3>& M) {
    double det = M[0][0] * (M[1][1]*M[2][2] - M[1][2]*M[2][1])
               - M[0][1] * (M[1][0]*M[2][2] - M[1][2]*M[2][0])
               + M[0][2] * (M[1][0]*M[2][1] - M[1][1]*M[2][0]);
    // Assume det != 0 (camera intrinsics are invertible).
    double invDet = 1.0 / det;
    std::array<std::array<double, 3>, 3> adj{};
    adj[0][0] =  M[1][1]*M[2][2] - M[1][2]*M[2][1];
    adj[0][1] = -(M[0][1]*M[2][2] - M[0][2]*M[2][1]);
    adj[0][2] =  M[0][1]*M[1][2] - M[0][2]*M[1][1];
    adj[1][0] = -(M[1][0]*M[2][2] - M[1][2]*M[2][0]);
    adj[1][1] =  M[0][0]*M[2][2] - M[0][2]*M[2][0];
    adj[1][2] = -(M[0][0]*M[1][2] - M[0][2]*M[1][0]);
    adj[2][0] =  M[1][0]*M[2][1] - M[1][1]*M[2][0];
    adj[2][1] = -(M[0][0]*M[2][1] - M[0][1]*M[2][0]);
    adj[2][2] =  M[0][0]*M[1][1] - M[0][1]*M[1][0];
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            adj[i][j] *= invDet;
    return adj;
}

// Helper: Jacobi eigen-decomposition for symmetric matrix A (size n).
// Returns eigenvectors as columns of V, eigenvalues in diag.
void jacobiEigen(std::array<std::array<double, 12>, 12>& A,
                 std::array<std::array<double, 12>, 12>& V,
                 std::array<double, 12>& diag) {
    const int n = 12;
    // Initialize V to identity.
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            V[i][j] = (i == j) ? 1.0 : 0.0;
    }
    // Copy diagonal.
    for (int i = 0; i < n; ++i)
        diag[i] = A[i][i];

    const int maxIter = 100;
    for (int iter = 0; iter < maxIter; ++iter) {
        // Find largest off-diagonal element.
        double maxOff = 0.0;
        int p = 0, q = 1;
        for (int i = 0; i < n; ++i) {
            for (int j = i+1; j < n; ++j) {
                double val = std::fabs(A[i][j]);
                if (val > maxOff) {
                    maxOff = val;
                    p = i;
                    q = j;
                }
            }
        }
        if (maxOff < 1e-10) break; // converged

        // Compute rotation angle.
        double app = A[p][p];
        double aqq = A[q][q];
        double apq = A[p][q];
        double theta = 0.5 * std::atan2(2.0*apq, aqq - app);
        double c = std::cos(theta);
        double s = std::sin(theta);

        // Apply rotation to A (similarity transform).
        // Only update rows/cols p and q.
        for (int k = 0; k < n; ++k) {
            double akp = A[k][p];
            double akq = A[k][q];
            if (k != p && k != q) {
                A[k][p] = c*akp - s*akq;
                A[p][k] = A[k][p];
                A[k][q] = s*akp + c*akq;
                A[q][k] = A[k][q];
            }
        }
        // Update diagonal and off-diagonal at (p,q).
        A[p][p] = c*c*app - 2.0*s*c*apq + s*s*aqq;
        A[q][q] = s*s*app + 2.0*s*c*apq + c*c*aqq;
        A[p][q] = 0.0;
        A[q][p] = 0.0;

        // Update eigenvectors.
        for (int k = 0; k < n; ++k) {
            double vkp = V[k][p];
            double vkq = V[k][q];
            V[k][p] = c*vkp - s*vkq;
            V[k][q] = s*vkp + c*vkq;
        }
        // Update diag array.
        diag[p] = A[p][p];
        diag[q] = A[q][q];
    }
}

// Main function: estimate camera pose using DLT.
Pose estimateCameraPose(
    const std::vector<std::array<double, 3>>& world_points,
    const std::vector<std::array<double, 2>>& image_points,
    const std::array<std::array<double, 3>, 3>& K) {

    // Build the 12x12 matrix AtA = sum over each point of r_i * r_i^T
    // where r_i is the row of A (we build AtA directly).
    std::array<std::array<double, 12>, 12> AtA{};
    for (int i = 0; i < 12; ++i)
        for (int j = 0; j < 12; ++j)
            AtA[i][j] = 0.0;

    size_t n = world_points.size();
    for (size_t idx = 0; idx < n; ++idx) {
        double X = world_points[idx][0];
        double Y = world_points[idx][1];
        double Z = world_points[idx][2];
        double u = image_points[idx][0];
        double v = image_points[idx][1];

        // First row (2*idx)
        double r1[12] = {X, Y, Z, 1.0, 0, 0, 0, 0, -u*X, -u*Y, -u*Z, -u};
        // Second row (2*idx+1)
        double r2[12] = {0, 0, 0, 0, X, Y, Z, 1.0, -v*X, -v*Y, -v*Z, -v};

        // Add outer products.
        for (int a = 0; a < 12; ++a) {
            for (int b = 0; b < 12; ++b) {
                AtA[a][b] += r1[a]*r1[b] + r2[a]*r2[b];
            }
        }
    }

    // Compute eigenvectors/values of AtA using Jacobi.
    std::array<std::array<double, 12>, 12> V{};
    std::array<double, 12> diag{};
    std::array<std::array<double, 12>, 12> AtA_copy = AtA;
    jacobiEigen(AtA_copy, V, diag);

    // Find smallest eigenvalue and its corresponding eigenvector.
    int minIdx = 0;
    double minVal = diag[0];
    for (int i = 1; i < 12; ++i) {
        if (diag[i] < minVal) {
            minVal = diag[i];
            minIdx = i;
        }
    }
    // The eigenvector is column minIdx of V.
    double p[12];
    for (int i = 0; i < 12; ++i)
        p[i] = V[i][minIdx];

    // Reshape p into 3x4 projection matrix P (row-major: P(i,j)=p[i*4+j]).
    std::array<std::array<double, 4>, 3> P{};
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 4; ++j)
            P[i][j] = p[i*4 + j];

    // Decompose: P = K [R | t], so [R | t] = K^{-1} P.
    auto Kinv = matInv3(K);

    // Compute K^{-1} * P (3x3 times 3x4).
    std::array<std::array<double, 4>, 3> Rt{};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            Rt[i][j] = 0.0;
            for (int k = 0; k < 3; ++k)
                Rt[i][j] += Kinv[i][k] * P[k][j];
        }
    }

    // Extract R and t.
    Pose pose;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j)
            pose.R[i][j] = Rt[i][j];
        pose.t[i] = Rt[i][3];
    }

    return pose;
}

#include <cassert>
#include <cmath>
#include <array>
#include <vector>

// The solution function is declared here (assume it's included above).

int main() {
    // Test 1: Without noise, using a simple synthetic scenario.
    // Camera intrinsics: focal lengths 100, principal point (50, 50).
    std::array<std::array<double, 3>, 3> K{{
        {{100, 0, 50}},
        {{0, 100, 50}},
        {{0, 0, 1}}
    }};

    // Known rotation: identity, translation: (0,0,10).
    std::vector<std::array<double, 3>> world = {
        {{0, 0, 0}},
        {{1, 0, 0}},
        {{0, 1, 0}},
        {{1, 1, 0}},
        {{0, 0, 1}},
        {{1, 0, 1}},
        {{0, 1, 1}},
        {{1, 1, 1}}
    };

    // Project them: u = K * (X/t_z + cx) etc. with t_z=10.
    std::vector<std::array<double, 2>> image;
    for (const auto& w : world) {
        double u = 50 + 100 * w[0] / 10.0;
        double v = 50 + 100 * w[1] / 10.0;
        image.push_back({{u, v}});
    }

    Pose pose = estimateCameraPose(world, image, K);

    // Since rotation is identity, R should be close to I, t close to (0,0,10).
    // Use a tolerance because DLT gives scaled solution and might have sign flip.
    // We normalize by checking monotonic projections work.
    // Instead, we just verify that re-projection matches.
    bool ok = true;
    for (size_t i = 0; i < world.size(); ++i) {
        // Project using estimated pose: X_c = R*X + t, then u = K*x_c/z_c.
        double xc = pose.R[0][0]*world[i][0] + pose.R[0][1]*world[i][1] + pose.R[0][2]*world[i][2] + pose.t[0];
        double yc = pose.R[1][0]*world[i][0] + pose.R[1][1]*world[i][1] + pose.R[1][2]*world[i][2] + pose.t[1];
        double zc = pose.R[2][0]*world[i][0] + pose.R[2][1]*world[i][1] + pose.R[2][2]*world[i][2] + pose.t[2];
        if (std::fabs(zc) < 1e-6) { ok = false; break; }
        double up = K[0][0]*xc/zc + K[0][2];
        double vp = K[1][1]*yc/zc + K[1][2];
        if (std::fabs(up - image[i][0]) > 1e-4 || std::fabs(vp - image[i][1]) > 1e-4) {
            ok = false;
            break;
        }
    }
    assert(ok);

    // Test 2: Scale invariance – multiplying pose by scale should still work.
    auto pose2 = pose;
    // No check here, just verify function runs without crash on different input.
    std::vector<std::array<double, 3>> world2 = {{0,0,0}};
    std::vector<std::array<double, 2>> image2 = {{50,50}};
    // This has only 1 point, but our function would produce degenerate result; we don't test that.
    // Instead, test with 6 points arbitrarily.
    std::vector<std::array<double, 3>> w3 = {
        {{1,2,3}}, {{4,5,6}}, {{7,8,9}}, {{10,11,12}}, {{13,14,15}}, {{16,17,18}}
    };
    std::vector<std::array<double, 2>> im3;
    // Generate projections using the same K and identity pose with t_z=5.
    for (const auto& w : w3) {
        double u = 50 + 100 * w[0] / 5.0;
        double v = 50 + 100 * w[1] / 5.0;
        im3.push_back({{u, v}});
    }
    Pose p3 = estimateCameraPose(w3, im3, K);
    // Verify re-projection.
    bool ok3 = true;
    for (size_t i = 0; i < w3.size(); ++i) {
        double xc = p3.R[0][0]*w3[i][0] + p3.R[0][1]*w3[i][1] + p3.R[0][2]*w3[i][2] + p3.t[0];
        double yc = p3.R[1][0]*w3[i][0] + p3.R[1][1]*w3[i][1] + p3.R[1][2]*w3[i][2] + p3.t[1];
        double zc = p3.R[2][0]*w3[i][0] + p3.R[2][1]*w3[i][1] + p3.R[2][2]*w3[i][2] + p3.t[2];
        if (std::fabs(zc) < 1e-6) { ok3 = false; break; }
        double up = K[0][0]*xc/zc + K[0][2];
        double vp = K[1][1]*yc/zc + K[1][2];
        if (std::fabs(up - im3[i][0]) > 1e-4 || std::fabs(vp - im3[i][1]) > 1e-4) {
            ok3 = false;
            break;
        }
    }
    assert(ok3);

    return 0;
}
