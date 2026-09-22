Write a C++ function `computeOBBFromPoints` that takes a pointer to an array of 3D points (`std::array<double,3>` or a simple struct with `x`, `y`, `z` members) and a count, and returns an oriented bounding box (OBB) as a struct containing a center (3D vector), three orthonormal axes (each a 3D unit vector), and extents (half-lengths along each axis) that tightly encloses the points. The function must compute the OBB using principal component analysis (PCA) via the covariance matrix and Jacobi eigenvalue decomposition, as implied by the provided snippet. Handle edge cases: if the count is 0 or the points are degenerate (collinear, coplanar, or identical), the axes may be arbitrary but must remain orthonormal and the box must still contain the points exactly (within floating-point tolerance). The function signature should be something like `OBB computeOBBFromPoints(const Point3D* points, size_t count)`, where `Point3D` and `OBB` are defined as simple structs with `double` members. The implementation must be self-contained, not relying on external libraries like Eigen, and must not include a main function in the solution.

The core idea is to align the OBB axes with the principal directions of the point distribution, obtained from the eigenvectors of the covariance matrix. First, compute the centroid (mean) of the points. Then build the 3x3 covariance matrix where `C[i][j]` = average over all points of `(p_i - mean_i)*(p_j - mean_j)`. This matrix is symmetric and positive semi-definite. To find its eigenvectors (which are orthonormal), we use the Jacobi eigenvalue algorithm: iteratively apply plane rotations to zero out off-diagonal elements until the matrix becomes diagonal (within tolerance). The eigenvectors are accumulated in a rotation matrix. After obtaining the three eigenvectors, we transform all points into the coordinate system defined by these axes (i.e., project each point onto each axis), and find the minimum and maximum projections along each axis. The center is the midpoint between the min and max along each axis in the rotated space, and the extents are half the range. Then transform the center back to original coordinates. Important edge cases: If all points are identical or collinear, the covariance matrix has zero or one non-zero eigenvalue. The Jacobi method still returns orthonormal eigenvectors (it initializes the rotation matrix to identity and the diagonal stays zero), so the axes remain orthonormal but arbitrary; the box will degenerate to a point or line/plane as appropriate. For numerical stability, iterate up to 50 sweeps (as in the snippet), and use a tolerance like `1e-12` for checking off-diagonal convergence. Time complexity is O(n * m) for covariance computation (n points, m=3 dimensions) plus O(iterations * 3^2) for Jacobi, which is effectively O(n). Space complexity is O(1) auxiliary beyond input storage, as we only store a few 3x3 matrices and vectors.

#include <cmath>
#include <cstddef>
#include <cstring>
#include <algorithm>

// Simple 3D point and OBB structs with double precision.
struct Point3D {
    double x, y, z;
};

struct OBB {
    Point3D center;
    Point3D axisX;  // unit vector
    Point3D axisY;  // unit vector
    Point3D axisZ;  // unit vector
    Point3D extents; // half-lengths along axisX, axisY, axisZ
};

// Internal helper functions.

static inline Point3D operator-(const Point3D& a, const Point3D& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline Point3D operator+(const Point3D& a, const Point3D& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

static inline Point3D operator*(const Point3D& a, double s) {
    return {a.x * s, a.y * s, a.z * s};
}

static inline double dot(const Point3D& a, const Point3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline Point3D normalize(const Point3D& a) {
    double len = std::sqrt(dot(a, a));
    if (len > 1e-12) return {a.x / len, a.y / len, a.z / len};
    return {1.0, 0.0, 0.0}; // fallback for zero vector
}

// Jacobi eigenvalue decomposition for symmetric 3x3 matrix.
// Input: matrix A (row-major, 3x3). Output: eigenvectors in columns of V, eigenvalues in d.
static void jacobiEigen(double A[3][3], double V[3][3], double d[3]) {
    // Initialize V to identity.
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            V[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }

    double b[3] = {A[0][0], A[1][1], A[2][2]};
    double z[3] = {0.0, 0.0, 0.0};
    double dtmp[3] = {A[0][0], A[1][1], A[2][2]};

    for (int iter = 0; iter < 50; ++iter) {
        // Compute sum of off-diagonal absolute values.
        double sm = std::fabs(A[0][1]) + std::fabs(A[0][2]) + std::fabs(A[1][2]);
        if (sm < 1e-12) {
            break;
        }
        double tresh = (iter < 3) ? 0.2 * sm / 9.0 : 0.0; // 3x3 matrix, n^2=9

        for (int p = 0; p < 3; ++p) {
            for (int q = p + 1; q < 3; ++q) {
                double g = 100.0 * std::fabs(A[p][q]);
                if (iter > 3 && std::fabs(dtmp[p]) + g == std::fabs(dtmp[p]) &&
                    std::fabs(dtmp[q]) + g == std::fabs(dtmp[q])) {
                    A[p][q] = 0.0;
                } else if (std::fabs(A[p][q]) > tresh) {
                    double h = dtmp[q] - dtmp[p];
                    double t;
                    if (std::fabs(h) + g == std::fabs(h)) {
                        t = A[p][q] / h;
                    } else {
                        double theta = 0.5 * h / A[p][q];
                        t = 1.0 / (std::fabs(theta) + std::sqrt(1.0 + theta * theta));
                        if (theta < 0.0) t = -t;
                    }
                    double c = 1.0 / std::sqrt(1.0 + t * t);
                    double s = t * c;
                    double tau = s / (1.0 + c);

                    h = t * A[p][q];
                    z[p] -= h;
                    z[q] += h;
                    dtmp[p] -= h;
                    dtmp[q] += h;
                    A[p][q] = 0.0;

                    // Apply rotation to rows/columns p and q.
                    for (int j = 0; j < p; ++j) {
                        double g_val = A[j][p];
                        double h_val = A[j][q];
                        A[j][p] = g_val - s * (h_val + g_val * tau);
                        A[j][q] = h_val + s * (g_val - h_val * tau);
                    }
                    for (int j = p + 1; j < q; ++j) {
                        double g_val = A[p][j];
                        double h_val = A[j][q];
                        A[p][j] = g_val - s * (h_val + g_val * tau);
                        A[j][q] = h_val + s * (g_val - h_val * tau);
                    }
                    for (int j = q + 1; j < 3; ++j) {
                        double g_val = A[p][j];
                        double h_val = A[q][j];
                        A[p][j] = g_val - s * (h_val + g_val * tau);
                        A[q][j] = h_val + s * (g_val - h_val * tau);
                    }
                    // Accumulate eigenvectors.
                    for (int j = 0; j < 3; ++j) {
                        double g_val = V[j][p];
                        double h_val = V[j][q];
                        V[j][p] = g_val - s * (h_val + g_val * tau);
                        V[j][q] = h_val + s * (g_val - h_val * tau);
                    }
                }
            }
        }
        for (int p = 0; p < 3; ++p) {
            b[p] += z[p];
            dtmp[p] = b[p];
            z[p] = 0.0;
        }
    }

    // Copy eigenvalues.
    for (int i = 0; i < 3; ++i) d[i] = dtmp[i];
}

// Main function as specified.
OBB computeOBBFromPoints(const Point3D* points, size_t count) {
    OBB result;
    // Default: identity axes, zero center/extents.
    result.center = {0.0, 0.0, 0.0};
    result.axisX = {1.0, 0.0, 0.0};
    result.axisY = {0.0, 1.0, 0.0};
    result.axisZ = {0.0, 0.0, 1.0};
    result.extents = {0.0, 0.0, 0.0};

    if (count == 0 || points == nullptr) {
        return result;
    }

    // Compute centroid.
    Point3D centroid = {0.0, 0.0, 0.0};
    for (size_t i = 0; i < count; ++i) {
        centroid.x += points[i].x;
        centroid.y += points[i].y;
        centroid.z += points[i].z;
    }
    centroid.x /= (double)count;
    centroid.y /= (double)count;
    centroid.z /= (double)count;

    // Compute covariance matrix (symmetric).
    double cov[3][3] = {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
    for (size_t i = 0; i < count; ++i) {
        double dx = points[i].x - centroid.x;
        double dy = points[i].y - centroid.y;
        double dz = points[i].z - centroid.z;
        cov[0][0] += dx * dx;
        cov[1][1] += dy * dy;
        cov[2][2] += dz * dz;
        cov[0][1] += dx * dy;
        cov[0][2] += dx * dz;
        cov[1][2] += dy * dz;
    }
    // Divide by count to get average.
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cov[i][j] /= (double)count;
        }
    }
    // Symmetrize (though construction already symmetric, but ensure).
    cov[1][0] = cov[0][1];
    cov[2][0] = cov[0][2];
    cov[2][1] = cov[1][2];

    // Compute eigenvectors.
    double V[3][3]; // each column is an eigenvector
    double eigenvalues[3];
    jacobiEigen(cov, V, eigenvalues);

    // Build axes from eigenvectors. Ensure right-handed? Not required but good practice.
    Point3D axisX = {V[0][0], V[1][0], V[2][0]};
    Point3D axisY = {V[0][1], V[1][1], V[2][1]};
    Point3D axisZ = {V[0][2], V[1][2], V[2][2]};
    axisX = normalize(axisX);
    axisY = normalize(axisY);
    axisZ = normalize(axisZ);

    // Project all points onto the axes to find tight bounds.
    double minX = dot(points[0] - centroid, axisX);
    double maxX = minX;
    double minY = dot(points[0] - centroid, axisY);
    double maxY = minY;
    double minZ = dot(points[0] - centroid, axisZ);
    double maxZ = minZ;

    for (size_t i = 1; i < count; ++i) {
        Point3D rel = points[i] - centroid;
        double px = dot(rel, axisX);
        double py = dot(rel, axisY);
        double pz = dot(rel, axisZ);
        minX = std::min(minX, px);
        maxX = std::max(maxX, px);
        minY = std::min(minY, py);
        maxY = std::max(maxY, py);
        minZ = std::min(minZ, pz);
        maxZ = std::max(maxZ, pz);
    }

    // Center in rotated space is midpoint, extents half range.
    double centerLocalX = 0.5 * (minX + maxX);
    double centerLocalY = 0.5 * (minY + maxY);
    double centerLocalZ = 0.5 * (minZ + maxZ);

    // Transform center back to original coordinates.
    result.center = centroid + axisX * centerLocalX + axisY * centerLocalY + axisZ * centerLocalZ;

    result.axisX = axisX;
    result.axisY = axisY;
    result.axisZ = axisZ;

    result.extents.x = (maxX - minX) * 0.5;
    result.extents.y = (maxY - minY) * 0.5;
    result.extents.z = (maxZ - minZ) * 0.5;

    return result;
}

#include <cassert>
#include <cmath>
#include <vector>

// Function prototype.
OBB computeOBBFromPoints(const Point3D* points, size_t count);

static bool approx(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) < eps;
}

static bool approxPoint(const Point3D& a, const Point3D& b, double eps = 1e-9) {
    return approx(a.x, b.x, eps) && approx(a.y, b.y, eps) && approx(a.z, b.z, eps);
}

int main() {
    // Test 1: Single point.
    Point3D p1 = {1.0, 2.0, 3.0};
    OBB obb1 = computeOBBFromPoints(&p1, 1);
    assert(approxPoint(obb1.center, p1));
    assert(obb1.extents.x == 0.0 && obb1.extents.y == 0.0 && obb1.extents.z == 0.0);
    assert(approx(dot(obb1.axisX, obb1.axisY), 0.0));
    assert(approx(dot(obb1.axisY, obb1.axisZ), 0.0));
    assert(approx(dot(obb1.axisZ, obb1.axisX), 0.0));

    // Test 2: Two distinct points (line).
    std::vector<Point3D> pts2 = {{0.0, 0.0, 0.0}, {2.0, 0.0, 0.0}};
    OBB obb2 = computeOBBFromPoints(pts2.data(), pts2.size());
    assert(approxPoint(obb2.center, {1.0, 0.0, 0.0}));
    assert(approx(obb2.extents.x, 1.0));
    assert(obb2.extents.y < 1e-9 && obb2.extents.z < 1e-9);

    // Test 3: Axis-aligned box corners.
    std::vector<Point3D> pts3 = {
        {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0},
        {1.0, 1.0, 0.0}, {1.0, 0.0, 1.0}, {0.0, 1.0, 1.0}, {1.0, 1.0, 1.0}
    };
    OBB obb3 = computeOBBFromPoints(pts3.data(), pts3.size());
    assert(approxPoint(obb3.center, {0.5, 0.5, 0.5}));
    assert(approx(obb3.extents.x, 0.5));
    assert(approx(obb3.extents.y, 0.5));
    assert(approx(obb3.extents.z, 0.5));
    // Axes should be aligned with world axes (up to sign).
    assert(approx(std::fabs(obb3.axisX.x), 1.0) || approx(std::fabs(obb3.axisY.x), 1.0) || approx(std::fabs(obb3.axisZ.x), 1.0));

    // Test 4: Points on a plane (2D degenerate).
    std::vector<Point3D> pts4 = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0}};
    OBB obb4 = computeOBBFromPoints(pts4.data(), pts4.size());
    // The extent along the third axis (perpendicular to plane) should be near zero.
    double minExtent = std::min({obb4.extents.x, obb4.extents.y, obb4.extents.z});
    assert(minExtent < 1e-9);

    // Test 5: Rotated box: points at 45 degrees.
    std::vector<Point3D> pts5;
    double ang = M_PI / 4.0;
    for (int i = 0; i < 8; ++i) {
        double u = (i % 2) ? 1.0 : -1.0;
        double v = ((i / 2) % 2) ? 1.0 : -1.0;
        double w = ((i / 4) % 2) ? 1.0 : -1.0;
        pts5.push_back({u * std::cos(ang) + v * std::sin(ang), -u * std::sin(ang) + v * std::cos(ang), w});
    }
    OBB obb5 = computeOBBFromPoints(pts5.data(), pts5.size());
    // Center at origin.
    assert(approx(obb5.center.x, 0.0));
    assert(approx(obb5.center.y, 0.0));
    assert(approx(obb5.center.z, 0.0));
    // Extents should be sqrt(2) along x/y, 1 along z.
    assert(approx(obb5.extents.x, std::sqrt(2.0)));
    assert(approx(obb5.extents.y, std::sqrt(2.0)));
    assert(approx(obb5.extents.z, 1.0));

    // Test 6: All identical points.
    std::vector<Point3D> pts6(5, {2.0, -3.0, 4.0});
    OBB obb6 = computeOBBFromPoints(pts6.data(), pts6.size());
    assert(approxPoint(obb6.center, {2.0, -3.0, 4.0}));
    assert(obb6.extents.x < 1e-9 && obb6.extents.y < 1e-9 && obb6.extents.z < 1e-9);
    // Axes must be orthonormal even if degenerate.
    assert(approx(dot(obb6.axisX, obb6.axisY), 0.0));
    assert(approx(dot(obb6.axisY, obb6.axisZ), 0.0));
    assert(approx(dot(obb6.axisZ, obb6.axisX), 0.0));

    // Test 7: Zero points.
    OBB obb7 = computeOBBFromPoints(nullptr, 0);
    assert(obb7.extents.x == 0.0 && obb7.extents.y == 0.0 && obb7.extents.z == 0.0);

    return 0;
}
