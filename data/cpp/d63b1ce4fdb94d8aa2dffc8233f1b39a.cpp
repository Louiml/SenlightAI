Write a C++ function that takes a vector of 3D points (each represented as a simple struct with `x`, `y`, `z` as `double` members) and a positive integer `k`, and returns a vector of `double` values where each element `i` is the estimated curvature at point `i`. The curvature should be computed by first finding the `k` nearest neighbors of each point (including the point itself) using Euclidean distance, then fitting a plane to those neighbors via principal component analysis (PCA): compute the covariance matrix of the neighbor coordinates, find its smallest eigenvalue, and use the formula `curvature = lambda_min / (lambda_min + lambda_mid + lambda_max)`, where the lambdas are the three eigenvalues sorted in ascending order. Handle edge cases: if `k` exceeds the number of points, clamp `k` to the total point count; if all neighbors are collinear (resulting in a zero smallest eigenvalue), return a curvature of 0 for that point; the input vector must be non-empty.
#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Single point -> curvature 0
    std::vector<Point3D> pts1 = {{0,0,0}};
    auto c1 = computeCurvatures(pts1, 5);
    assert(c1.size() == 1);
    assert(std::abs(c1[0]) < 1e-9);

    // Points on a plane (z=0) -> curvature should be 0
    std::vector<Point3D> plane = {{0,0,0}, {1,0,0}, {0,1,0}, {1,1,0}, {0.5,0.5,0}};
    auto cp = computeCurvatures(plane, 3);
    assert(cp.size() == 5);
    for (double v : cp) assert(std::abs(v) < 1e-9);

    // Points on a sphere-like curvature: a simple tetrahedron gives non-zero curvature
    std::vector<Point3D> tetra = {{0,0,0}, {1,0,0}, {0,1,0}, {0,0,1}};
    auto ct = computeCurvatures(tetra, 4);
    assert(ct.size() == 4);
    // All curvatures should be > 0 but less than 0.5 (since smallest eigenvalue is positive)
    for (double v : ct) {
        assert(v > 0.0);
        assert(v < 0.5);
    }

    // k larger than n should clamp
    auto c_bigk = computeCurvatures(plane, 100);
    assert(c_bigk.size() == 5);

    // Non-empty input required
    bool threw = false;
    try {
        std::vector<Point3D> empty;
        computeCurvatures(empty, 3);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

struct Point3D {
    double x, y, z;
};

// Compute eigenvalues of a symmetric 3x3 matrix using Jacobi rotation.
// The matrix is given as {a00, a01, a02, a11, a12, a22} (upper triangle).
std::array<double, 3> eigenvalues3x3(double a00, double a01, double a02,
                                     double a11, double a12, double a22) {
    // Symmetric matrix
    double m[3][3] = {{a00, a01, a02},
                      {a01, a11, a12},
                      {a02, a12, a22}};
    double e[3] = {0.0};
    const int max_iter = 50;
    for (int iter = 0; iter < max_iter; ++iter) {
        // Find largest off-diagonal element
        int p = 0, q = 1;
        double max_off = std::abs(m[0][1]);
        if (std::abs(m[0][2]) > max_off) { max_off = std::abs(m[0][2]); p = 0; q = 2; }
        if (std::abs(m[1][2]) > max_off) { p = 1; q = 2; }
        if (max_off < 1e-12) break;
        // Compute rotation angle
        double theta = (m[q][q] - m[p][p]) / (2.0 * m[p][q]);
        double t = (theta >= 0.0) ? 1.0 / (theta + std::sqrt(1.0 + theta * theta))
                                  : -1.0 / (-theta + std::sqrt(1.0 + theta * theta));
        double c = 1.0 / std::sqrt(1.0 + t * t);
        double s = t * c;
        // Apply rotation to p,q
        double app = m[p][p], aqq = m[q][q], apq = m[p][q];
        m[p][p] = c*c*app - 2*c*s*apq + s*s*aqq;
        m[q][q] = s*s*app + 2*c*s*apq + c*c*aqq;
        m[p][q] = 0.0;
        m[q][p] = 0.0;
        // Update other elements
        for (int i = 0; i < 3; ++i) {
            if (i != p && i != q) {
                double aip = m[i][p];
                double aiq = m[i][q];
                m[i][p] = c*aip - s*aiq;
                m[p][i] = m[i][p];
                m[i][q] = s*aip + c*aiq;
                m[q][i] = m[i][q];
            }
        }
    }
    e[0] = m[0][0]; e[1] = m[1][1]; e[2] = m[2][2];
    std::sort(e, e+3);
    return {e[0], e[1], e[2]};
}

// Compute curvature for each point based on k-nearest neighbors and PCA.
std::vector<double> computeCurvatures(const std::vector<Point3D>& points, int k) {
    const int n = static_cast<int>(points.size());
    if (n == 0) throw std::invalid_argument("Empty point cloud");
    int kk = std::min(k, n);
    kk = std::max(kk, 3); // at least 3 neighbors for a plane fit
    std::vector<double> curvatures(n, 0.0);

    for (int i = 0; i < n; ++i) {
        // Collect distances and indices
        std::vector<std::pair<double, int>> dists;
        dists.reserve(n);
        for (int j = 0; j < n; ++j) {
            double dx = points[i].x - points[j].x;
            double dy = points[i].y - points[j].y;
            double dz = points[i].z - points[j].z;
            dists.emplace_back(dx*dx + dy*dy + dz*dz, j);
        }
        std::sort(dists.begin(), dists.end());
        // Take first kk neighbors (includes self with distance 0)
        std::vector<Point3D> neigh;
        neigh.reserve(kk);
        for (int t = 0; t < kk; ++t) {
            int idx = dists[t].second;
            neigh.push_back(points[idx]);
        }
        // Compute centroid
        double cx = 0, cy = 0, cz = 0;
        for (const auto& p : neigh) { cx += p.x; cy += p.y; cz += p.z; }
        cx /= kk; cy /= kk; cz /= kk;
        // Compute covariance matrix (upper triangle)
        double a00 = 0, a01 = 0, a02 = 0, a11 = 0, a12 = 0, a22 = 0;
        for (const auto& p : neigh) {
            double dx = p.x - cx;
            double dy = p.y - cy;
            double dz = p.z - cz;
            a00 += dx*dx; a01 += dx*dy; a02 += dx*dz;
            a11 += dy*dy; a12 += dy*dz; a22 += dz*dz;
        }
        double scale = static_cast<double>(kk);
        a00 /= scale; a01 /= scale; a02 /= scale;
        a11 /= scale; a12 /= scale; a22 /= scale;

        auto eigs = eigenvalues3x3(a00, a01, a02, a11, a12, a22);
        // Ensure non-negative (numerical errors might make tiny negatives)
        double l0 = std::max(0.0, eigs[0]);
        double l1 = std::max(0.0, eigs[1]);
        double l2 = std::max(0.0, eigs[2]);
        double sum = l0 + l1 + l2;
        if (sum < 1e-12) {
            curvatures[i] = 0.0;
        } else {
            curvatures[i] = l0 / sum;
        }
    }
    return curvatures;
}
// The solution requires two main steps per point: (1) finding the `k` nearest neighbors, and (2) computing eigenvalues of a 3×3 covariance matrix. For nearest neighbor search, a brute-force O(n·k·d) approach is acceptable since the problem is self-contained and does not require advanced data structures; for each point, iterate over all other points, compute squared Euclidean distance to avoid sqrt, and maintain a max-heap or simply collect and sort distances to select the `k` smallest. Sorting distances for each point gives O(n·(n log n)) per point in the worst case, leading to O(n² log n) overall—fine for moderate inputs but not for very large clouds. For eigenvalue computation, the covariance matrix is symmetric positive semi-definite; we can use the Jacobi eigenvalue algorithm or, more simply, compute the eigenvalues using the analytical solution for a 3×3 symmetric matrix (cardano's method) or use an iterative QR-like approach. A robust, numerically stable approach is to use the `Eigen` library, but that would violate "self-contained"; therefore, we implement a small PCA using the power iteration for the largest eigenvalue and then deflation, or use the closed-form for symmetric 3×3 using trigonometric solutions. A practical choice: use the standard algorithm for eigenvalues of a 3×3 symmetric matrix (e.g., using `std::array` and a numerical routine). Edge cases: if `k` is 0 or 1, the covariance matrix may be degenerate; return 0 curvature. If a point has fewer than 3 unique neighbors (e.g., all duplicates), the covariance matrix may be singular; clamp eigenvalues to non-negative and if the trace is zero, set all eigenvalues to zero. Time complexity: O(n·(n log n)) for neighbor selection using sorting, and O(1) for the 3×3 eigensolver per point. Space complexity: O(n) for the result and O(k) per point for neighbor indices.
