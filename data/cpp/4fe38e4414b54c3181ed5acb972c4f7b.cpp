// Write a standalone C++ function `triangulatePoint` that, given a collection of 2D image point observations and corresponding 3×4 projection matrices (camera matrices) for multiple views, computes the 3D point that minimizes the algebraic error across all views using the Direct Linear Transform (DLT) method. The function must accept a `std::vector<cv::Point2d>` (or a simple 2×N matrix) of image coordinates and a `std::vector` of 3×4 matrices (represented as `std::array<std::array<double,4>,3>` or a flat `std::vector<double>` of size 12 per camera), and return a 3D point as `cv::Point3d` or `std::array<double,3>`. The DLT method constructs a 2N×4 design matrix by stacking rows derived from each view’s projection equation, then solves for the null space via Singular Value Decomposition (SVD) and returns the last column of the right singular vector matrix (the eigenvector corresponding to the smallest singular value), normalized so its 4th homogeneous coordinate equals 1. Handle at least 2 views, and assume all inputs are valid (no degenerate all-zero projection matrices).
// The solution uses the Direct Linear Transform (DLT) for multi-view triangulation. For each view with projection matrix \(P_i\) (3×4) and image point \((u_i, v_i)\), the projection equations \(u_i = \frac{P_{i0} \cdot X}{P_{i2} \cdot X}\) and \(v_i = \frac{P_{i1} \cdot X}{P_{i2} \cdot X}\) lead to two linear constraints in the homogeneous 4D point \(X\): \((u_i P_{i2} - P_{i0}) \cdot X = 0\) and \((v_i P_{i2} - P_{i1}) \cdot X = 0\). Stacking these for all \(N\) views gives a \(2N \times 4\) matrix \(A\). The 3D point (in homogeneous coordinates) is the non-trivial solution to \(A X = 0\), found as the right singular vector associated with the smallest singular value of \(A\) (or equivalently, the eigenvector of \(A^T A\) for its smallest eigenvalue). After obtaining the 4D vector \(X_h\), convert to 3D by dividing by its 4th component (assuming it’s non-zero; if near zero, the point is at infinity, but we assume a finite point). Edge cases: ensure at least 2 views; handle near-degenerate configurations where the smallest singular value is not uniquely small, but the method still returns a best-fit in the least-squares sense. Time complexity: \(O(N)\) to build the matrix, plus SVD on a \(2N \times 4\) matrix, which for fixed 4 columns is effectively \(O(N)\) with a constant factor. Space: \(O(N)\) for the matrix.
#include <vector>
#include <array>
#include <cmath>
#include <stdexcept>

// Solve for the 3D point from multi-view correspondences using DLT.
// Input: views - list of pairs (projection matrix as 3x4 array, image point as {u,v})
// Return: 3D point as array {x,y,z}
std::array<double,3> triangulatePoint(
    const std::vector<std::pair<std::array<std::array<double,4>,3>, std::array<double,2>>>& views)
{
    const size_t n = views.size();
    if (n < 2) throw std::invalid_argument("At least 2 views required.");

    // Build the 2N x 4 design matrix A.
    // We'll store A row-wise for SVD via normal equations (A^T A) since A has only 4 columns.
    // A^T A is 4x4, so we accumulate it directly.
    double AtA[4][4] = {{0}};
    for (size_t i = 0; i < n; ++i) {
        const auto& P = views[i].first;
        const double u = views[i].second[0];
        const double v = views[i].second[1];
        // Row for u: (u * P_row2 - P_row0)
        // Row for v: (v * P_row2 - P_row1)
        double r1[4], r2[4];
        for (int j = 0; j < 4; ++j) {
            r1[j] = u * P[2][j] - P[0][j];
            r2[j] = v * P[2][j] - P[1][j];
        }
        // Accumulate outer products into AtA
        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                AtA[r][c] += r1[r]*r1[c] + r2[r]*r2[c];
            }
        }
    }

    // Compute eigenvector of AtA corresponding to smallest eigenvalue using Jacobi eigenvalue algorithm (for 4x4 symmetric).
    // Copy to local matrix.
    double mat[4][4];
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
            mat[r][c] = AtA[r][c];

    double eigenvectors[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

    // Jacobi rotation method
    const int maxIter = 100;
    for (int iter = 0; iter < maxIter; ++iter) {
        // Find largest off-diagonal element
        int p = 0, q = 1;
        double maxOffDiag = std::fabs(mat[0][1]);
        for (int r = 0; r < 4; ++r) {
            for (int c = r+1; c < 4; ++c) {
                if (std::fabs(mat[r][c]) > maxOffDiag) {
                    maxOffDiag = std::fabs(mat[r][c]);
                    p = r; q = c;
                }
            }
        }
        if (maxOffDiag < 1e-12) break;
        double theta = (mat[q][q] - mat[p][p]) / (2.0 * mat[p][q]);
        double t = (theta >= 0 ? 1.0 : -1.0) / (std::fabs(theta) + std::sqrt(theta*theta + 1.0));
        double c = 1.0 / std::sqrt(t*t + 1.0);
        double s = t * c;
        // Apply rotation
        for (int k = 0; k < 4; ++k) {
            double old_pk = mat[p][k];
            double old_qk = mat[q][k];
            mat[p][k] = c * old_pk - s * old_qk;
            mat[q][k] = s * old_pk + c * old_qk;
        }
        for (int k = 0; k < 4; ++k) {
            double old_kp = mat[k][p];
            double old_kq = mat[k][q];
            mat[k][p] = c * old_kp - s * old_kq;
            mat[k][q] = s * old_kp + c * old_kq;
        }
        // Update eigenvectors
        for (int k = 0; k < 4; ++k) {
            double old_kp = eigenvectors[k][p];
            double old_kq = eigenvectors[k][q];
            eigenvectors[k][p] = c * old_kp - s * old_kq;
            eigenvectors[k][q] = s * old_kp + c * old_kq;
        }
    }

    // The eigenvector corresponding to smallest eigenvalue is the column with smallest diagonal element.
    int minIdx = 0;
    for (int i = 1; i < 4; ++i) {
        if (mat[i][i] < mat[minIdx][minIdx]) minIdx = i;
    }
    double Xh[4] = {eigenvectors[0][minIdx], eigenvectors[1][minIdx], eigenvectors[2][minIdx], eigenvectors[3][minIdx]};

    // Dehomogenize
    if (std::fabs(Xh[3]) < 1e-12) {
        // Point at infinity; return direction scaled arbitrarily.
        double norm = std::sqrt(Xh[0]*Xh[0] + Xh[1]*Xh[1] + Xh[2]*Xh[2]);
        if (norm < 1e-12) throw std::runtime_error("Degenerate solution");
        return {Xh[0]/norm, Xh[1]/norm, Xh[2]/norm};
    }
    double w = Xh[3];
    return {Xh[0]/w, Xh[1]/w, Xh[2]/w};
}
#include <cassert>
#include <cmath>
#include <vector>
#include <array>
#include <iostream>

// The solution function is assumed to be declared above.
// Helper to create a simple projection matrix for testing.
std::array<std::array<double,4>,3> makeP(double fx, double fy, double cx, double cy, double rx, double ry, double rz, double tx, double ty, double tz) {
    // Simplified: no rotation, just translation and scale (orthographic-like) for test simplicity.
    // For a proper test, we'll use identity rotation and simple translation.
    (void)rx; (void)ry; (void)rz;
    std::array<std::array<double,4>,3> P;
    P[0] = {fx, 0, cx, tx};
    P[1] = {0, fy, cy, ty};
    P[2] = {0, 0, 1, tz};
    return P;
}

int main() {
    // Test 1: Two views, point at (1,2,3) with simple cameras.
    // Camera 1: focal 100, center (0,0), at origin.
    auto P1 = makeP(100, 100, 0, 0, 0,0,0, 0,0,0);
    // Camera 2: translated by (10,0,0) but same orientation, so it sees the same point shifted.
    auto P2 = makeP(100, 100, 0, 0, 0,0,0, -10,0,0);
    // Point (1,2,3) projected into camera 1: u1 = 100*1/3, v1 = 100*2/3.
    double u1 = 100.0*1/3, v1 = 100.0*2/3;
    // In camera 2, the point coordinate in camera frame is (1-10, 2, 3) = (-9,2,3) so u2 = 100*(-9)/3, v2 = 100*2/3.
    double u2 = 100.0*(-9)/3, v2 = 100.0*2/3;
    std::vector<std::pair<std::array<std::array<double,4>,3>, std::array<double,2>>> views = {
        {P1, {u1, v1}},
        {P2, {u2, v2}}
    };
    auto X = triangulatePoint(views);
    // Note: The DLT might produce a scaled version; compare normalized direction and scale.
    double normX = std::sqrt(X[0]*X[0] + X[1]*X[1] + X[2]*X[2]);
    // Since the exact solution is (1,2,3) but due to floating point, check relative.
    double diff = std::fabs(normX - std::sqrt(1+4+9));
    assert(diff < 1e-6);

    // Test 2: Three views, point at origin (0,0,10) with varying translations.
    auto P3 = makeP(50, 50, 0,0, 0,0,0, 0,0,0);
    auto P4 = makeP(50, 50, 0,0, 0,0,0, 5,1,0);
    auto P5 = makeP(50, 50, 0,0, 0,0,0, -3,2,0);
    // Project (0,0,10): u = 0, v = 0 for all cameras (since translation only affects position but point is on axis? Actually translation moves camera, so image changes).
    // For camera with translation (tx,ty,tz), the point in camera coords is (0-tx, 0-ty, 10-tz). So u = 50*( -tx)/(10-tz), v=50*(-ty)/(10-tz).
    auto proj = [](const std::array<std::array<double,4>,3>& P, double x, double y, double z) -> std::array<double,2> {
        double Xh[4] = {x,y,z,1.0};
        double u = P[0][0]*Xh[0] + P[0][1]*Xh[1] + P[0][2]*Xh[2] + P[0][3]*Xh[3];
        double v = P[1][0]*Xh[0] + P[1][1]*Xh[1] + P[1][2]*Xh[2] + P[1][3]*Xh[3];
        double w = P[2][0]*Xh[0] + P[2][1]*Xh[1] + P[2][2]*Xh[2] + P[2][3]*Xh[3];
        return {u/w, v/w};
    };
    auto p1 = proj(P3, 0,0,10);
    auto p2 = proj(P4, 0,0,10);
    auto p3 = proj(P5, 0,0,10);
    std::vector<std::pair<std::array<std::array<double,4>,3>, std::array<double,2>>> views2 = {
        {P3, p1}, {P4, p2}, {P5, p3}
    };
    auto Y = triangulatePoint(views2);
    // Check that Y is close to (0,0,10) in direction and scale.
    double normY = std::sqrt(Y[0]*Y[0]+Y[1]*Y[1]+Y[2]*Y[2]);
    assert(std::fabs(normY - 10.0) < 1e-5);
    assert(std::fabs(Y[0]) < 1e-4);
    assert(std::fabs(Y[1]) < 1e-4);
    assert(std::fabs(Y[2] - 10.0) < 1e-4);

    std::cout << "All tests passed." << std::endl;
    return 0;
}
