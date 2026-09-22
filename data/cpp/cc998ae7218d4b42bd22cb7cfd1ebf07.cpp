Given a collection of 3D points as an array of `Vec3` (with `x`, `y`, `z` members), write a C++ function named `computeOBB` that returns a custom `OBB` struct containing the oriented bounding box: a center point, three orthonormal axes (each represented as `Vec3`), and extents (half-lengths) along those axes. The function must compute the OBB using principal component analysis (PCA): form a 3×3 covariance matrix of the point positions, compute its eigenvectors via the Jacobi eigenvalue algorithm, and use those eigenvectors as the box axes. The axes must be normalized and the extents must be computed by projecting all points onto the axes and taking half the range. Handle edge cases: if fewer than 2 points are given, return a degenerate OBB with zero extents, identity axes, and center at the origin. The solution should not rely on any external math library; implement all matrix/vector operations manually using simple structs and arrays.

The main algorithm involves three steps. First, compute the mean (centroid) of the points and the 3×3 covariance matrix \( \Sigma \), where \( \Sigma_{ij} = \frac{1}{N} \sum_{k=1}^N (p_k^i - \mu_i)(p_k^j - \mu_j) \). This requires one pass over the points to accumulate sums and sums of products, then subtract the outer product of means. Second, compute the eigenvectors of \( \Sigma \) using the Jacobi eigenvalue algorithm, which iteratively zeroes out off-diagonal elements via plane rotations until the matrix is diagonal (up to a tolerance). The Jacobi method converges in at most ~50 iterations for 3×3 matrices; each rotation is an orthogonal similarity transform that preserves eigenvalues. The resulting eigenvectors (columns of the accumulated rotation matrix) are the OBB axes; sort them by descending eigenvalue magnitude to ensure deterministic order. Third, after sorting, project each original point onto each axis and track the minimum and maximum projection values to compute the extents as half the difference. The center is the midpoint of these projection ranges, then converted back to world coordinates by averaging the min and max projected points (equivalently, the centroid in the rotated frame, but the projection-range midpoint gives the tight box center). Edge cases: for fewer than 2 points, the covariance matrix is zero, eigenvectors are arbitrary; return zeros. For 2+ points, if the covariance is degenerate (e.g., collinear points), Jacobi still returns orthogonal vectors; the extent along the zero-variance direction will be near zero, which is acceptable. Time complexity is \(O(N)\) for covariance computation and projection, plus \(O(1)\) for the fixed-size Jacobi iterations; overall \(O(N)\). Space complexity is \(O(1)\) auxiliary beyond the input array.

#include <cmath>
#include <algorithm>
#include <cassert>

struct Vec3 {
    float x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    Vec3 operator+(const Vec3& o) const { return Vec3(x+o.x, y+o.y, z+o.z); }
    Vec3 operator-(const Vec3& o) const { return Vec3(x-o.x, y-o.y, z-o.z); }
    Vec3 operator*(float s) const { return Vec3(x*s, y*s, z*s); }
    Vec3& operator+=(const Vec3& o) { x+=o.x; y+=o.y; z+=o.z; return *this; }
    Vec3& operator-=(const Vec3& o) { x-=o.x; y-=o.y; z-=o.z; return *this; }
    Vec3& operator*=(float s) { x*=s; y*=s; z*=s; return *this; }
    float dot(const Vec3& o) const { return x*o.x + y*o.y + z*o.z; }
    float lengthSq() const { return x*x + y*y + z*z; }
    void normalize() {
        float len = std::sqrt(lengthSq());
        if (len > 1e-9f) { *this *= (1.0f/len); }
    }
};

struct OBB {
    Vec3 center;   // world-space center
    Vec3 axes[3];  // orthonormal axes (already normalized)
    Vec3 extents;  // half-lengths along each axis
};

// Compute the covariance matrix of points (3x3 symmetric).
static void computeCovariance(const Vec3* points, int count, float cov[3][3]) {
    Vec3 mean(0,0,0);
    for (int i = 0; i < count; ++i) mean += points[i];
    mean *= (1.0f / count);

    // Initialize cov to zeros
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            cov[i][j] = 0.0f;

    for (int k = 0; k < count; ++k) {
        Vec3 diff = points[k] - mean;
        cov[0][0] += diff.x * diff.x;
        cov[1][1] += diff.y * diff.y;
        cov[2][2] += diff.z * diff.z;
        cov[0][1] += diff.x * diff.y;
        cov[0][2] += diff.x * diff.z;
        cov[1][2] += diff.y * diff.z;
    }
    float invN = 1.0f / count;
    cov[0][0] *= invN; cov[1][1] *= invN; cov[2][2] *= invN;
    cov[0][1] *= invN; cov[0][2] *= invN; cov[1][2] *= invN;
    cov[1][0] = cov[0][1];
    cov[2][0] = cov[0][2];
    cov[2][1] = cov[1][2];
}

// Jacobi eigen-decomposition for symmetric 3x3 matrix A.
// Output: eigenvalues in evals[3], eigenvectors as columns in evecs[3][3].
static void jacobiEigen(float A[3][3], float evals[3], float evecs[3][3]) {
    // Initialize eigenvector matrix to identity
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j)
            evecs[i][j] = (i == j) ? 1.0f : 0.0f;
        evals[i] = A[i][i];
    }

    float offDiag = 0.0f;
    for (int i = 0; i < 3; ++i)
        for (int j = i+1; j < 3; ++j)
            offDiag += std::fabs(A[i][j]);

    int maxIter = 50;
    while (offDiag > 1e-6f && maxIter-- > 0) {
        // Find largest off-diagonal element
        int p = 0, q = 1;
        float largest = std::fabs(A[0][1]);
        if (std::fabs(A[0][2]) > largest) { largest = std::fabs(A[0][2]); p = 0; q = 2; }
        if (std::fabs(A[1][2]) > largest) { largest = std::fabs(A[1][2]); p = 1; q = 2; }
        if (largest < 1e-12f) break;

        // Compute rotation angle
        float theta = (A[q][q] - A[p][p]) / (2.0f * A[p][q]);
        float t = (theta >= 0) ? 1.0f / (theta + std::sqrt(1.0f + theta*theta))
                               : -1.0f / (-theta + std::sqrt(1.0f + theta*theta));
        float c = 1.0f / std::sqrt(1.0f + t*t);
        float s = t * c;

        // Apply Jacobi rotation to A and accumulate in evecs
        float app = A[p][p], aqq = A[q][q], apq = A[p][q];
        A[p][p] = c*c*app - 2.0f*s*c*apq + s*s*aqq;
        A[q][q] = s*s*app + 2.0f*s*c*apq + c*c*aqq;
        A[p][q] = A[q][p] = 0.0f;

        for (int i = 0; i < 3; ++i) {
            if (i != p && i != q) {
                float aip = A[i][p];
                float aiq = A[i][q];
                A[i][p] = A[p][i] = c*aip - s*aiq;
                A[i][q] = A[q][i] = s*aip + c*aiq;
            }
        }
        // Update columns of eigenvector matrix
        for (int i = 0; i < 3; ++i) {
            float vip = evecs[i][p];
            float viq = evecs[i][q];
            evecs[i][p] = c*vip - s*viq;
            evecs[i][q] = s*vip + c*viq;
        }

        // Recompute off-diagonal magnitude
        offDiag = 0.0f;
        for (int i = 0; i < 3; ++i)
            for (int j = i+1; j < 3; ++j)
                offDiag += std::fabs(A[i][j]);
    }

    // Extract eigenvalues as diagonal
    for (int i = 0; i < 3; ++i) evals[i] = A[i][i];
}

// Main function: compute OBB from points.
OBB computeOBB(const Vec3* points, int count) {
    OBB result;
    if (count < 1) {
        result.center = Vec3(0,0,0);
        result.axes[0] = Vec3(1,0,0);
        result.axes[1] = Vec3(0,1,0);
        result.axes[2] = Vec3(0,0,1);
        result.extents = Vec3(0,0,0);
        return result;
    }

    // Compute covariance
    float cov[3][3];
    computeCovariance(points, count, cov);

    // Eigen-decompose
    float evals[3];
    float evecs[3][3];
    jacobiEigen(cov, evals, evecs);

    // Sort axes by descending eigenvalue (variance)
    int order[3] = {0,1,2};
    std::sort(order, order+3, [&](int a, int b){ return std::fabs(evals[a]) > std::fabs(evals[b]); });

    Vec3 axes[3];
    for (int i = 0; i < 3; ++i) {
        int idx = order[i];
        axes[i] = Vec3(evecs[0][idx], evecs[1][idx], evecs[2][idx]);
        axes[i].normalize();
        // Ensure right-handed system by flipping if needed
        if (i == 1 && axes[0].dot(axes[1]) < 0) axes[i] = axes[i] * -1.0f;
    }
    axes[2] = Vec3( axes[0].y*axes[1].z - axes[0].z*axes[1].y,
                    axes[0].z*axes[1].x - axes[0].x*axes[1].z,
                    axes[0].x*axes[1].y - axes[0].y*axes[1].x );

    // Project points onto axes to find extents
    float minProj[3] = {1e30f, 1e30f, 1e30f};
    float maxProj[3] = {-1e30f, -1e30f, -1e30f};
    for (int k = 0; k < count; ++k) {
        for (int i = 0; i < 3; ++i) {
            float p = points[k].dot(axes[i]);
            if (p < minProj[i]) minProj[i] = p;
            if (p > maxProj[i]) maxProj[i] = p;
        }
    }

    // Extents are half the range
    Vec3 extents;
    for (int i = 0; i < 3; ++i) extents = (i==0) ? Vec3((maxProj[i]-minProj[i])*0.5f, 0,0)
                                                 : (i==1) ? Vec3(extents.x, (maxProj[i]-minProj[i])*0.5f, 0)
                                                          : Vec3(extents.x, extents.y, (maxProj[i]-minProj[i])*0.5f);

    // Center is midpoint in rotated frame, then back to world
    Vec3 centerInAxes;
    for (int i = 0; i < 3; ++i) centerInAxes = (i==0) ? Vec3((minProj[i]+maxProj[i])*0.5f,0,0)
                                                       : (i==1) ? Vec3(centerInAxes.x, (minProj[i]+maxProj[i])*0.5f,0)
                                                                : Vec3(centerInAxes.x, centerInAxes.y, (minProj[i]+maxProj[i])*0.5f);
    Vec3 worldCenter;
    for (int i = 0; i < 3; ++i) worldCenter += axes[i] * centerInAxes[i];

    result.center = worldCenter;
    result.axes[0] = axes[0];
    result.axes[1] = axes[1];
    result.axes[2] = axes[2];
    result.extents = extents;
    return result;
}

#include <cassert>
#include <cmath>
#include <vector>

int main() {
    // Test 1: Single point -> zero extents, center at point
    Vec3 single[1] = { Vec3(2.0f, -3.0f, 0.5f) };
    OBB obb1 = computeOBB(single, 1);
    assert(fabs(obb1.center.x - 2.0f) < 1e-5f);
    assert(fabs(obb1.center.y + 3.0f) < 1e-5f);
    assert(fabs(obb1.center.z - 0.5f) < 1e-5f);
    assert(fabs(obb1.extents.x) < 1e-5f && fabs(obb1.extents.y) < 1e-5f && fabs(obb1.extents.z) < 1e-5f);

    // Test 2: Points on a line along x-axis -> extents along x, zero along others
    Vec3 line[3] = { Vec3(0,0,0), Vec3(1,0,0), Vec3(2,0,0) };
    OBB obb2 = computeOBB(line, 3);
    // The x-axis should dominate; check extents
    float ex = std::fabs(obb2.extents.x);
    float ey = std::fabs(obb2.extents.y);
    float ez = std::fabs(obb2.extents.z);
    assert(fabs(ex - 1.0f) < 1e-3f);
    assert(ey < 1e-3f && ez < 1e-3f);
    // Center should be at (1,0,0)
    assert(fabs(obb2.center.x - 1.0f) < 1e-3f);
    assert(fabs(obb2.center.y) < 1e-3f && fabs(obb2.center.z) < 1e-3f);

    // Test 3: Axis-aligned box with known extents
    std::vector<Vec3> box;
    for (float x : {-1.0f, 1.0f})
        for (float y : {-2.0f, 2.0f})
            for (float z : {-3.0f, 3.0f})
                box.push_back(Vec3(x,y,z));
    OBB obb3 = computeOBB(box.data(), (int)box.size());
    // Check extents (up to ordering of axes)
    float exts[3] = {obb3.extents.x, obb3.extents.y, obb3.extents.z};
    std::sort(exts, exts+3);
    assert(fabs(exts[0]-1.0f) < 1e-3f);
    assert(fabs(exts[1]-2.0f) < 1e-3f);
    assert(fabs(exts[2]-3.0f) < 1e-3f);
    assert(fabs(obb3.center.x) < 1e-4f && fabs(obb3.center.y) < 1e-4f && fabs(obb3.center.z) < 1e-4f);

    // Test 4: Rotated box - points forming a rotated rectangle
    std::vector<Vec3> rotated;
    float angle = M_PI/6.0f; // 30 degrees
    for (float dx : {-1.0f, 1.0f})
        for (float dy : {-2.0f, 2.0f})
            rotated.push_back(Vec3(dx*cos(angle) - dy*sin(angle), dx*sin(angle) + dy*cos(angle), 0.0f));
    OBB obb4 = computeOBB(rotated.data(), (int)rotated.size());
    // Extents should be approximately 1 and 2 (since we rotate in plane, z extent zero)
    float ex4[3] = {obb4.extents.x, obb4.extents.y, obb4.extents.z};
    std::sort(ex4, ex4+3);
    assert(fabs(ex4[0]) < 1e-3f); // z extent near zero
    assert(fabs(ex4[1]-1.0f) < 0.1f);
    assert(fabs(ex4[2]-2.0f) < 0.1f);
    // Center near origin
    assert(fabs(obb4.center.x) < 1e-3f && fabs(obb4.center.y) < 1e-3f && fabs(obb4.center.z) < 1e-3f);

    // Test 5: Degenerate - two identical points
    Vec3 dup[2] = { Vec3(5,5,5), Vec3(5,5,5) };
    OBB obb5 = computeOBB(dup, 2);
    assert(fabs(obb5.extents.x) < 1e-4f && fabs(obb5.extents.y) < 1e-4f && fabs(obb5.extents.z) < 1e-4f);
    assert(fabs(obb5.center.x - 5.0f) < 1e-4f);

    // Test 6: Empty input (count=0)
    OBB obb6 = computeOBB(nullptr, 0);
    assert(fabs(obb6.extents.x) < 1e-6f && fabs(obb6.extents.y) < 1e-6f && fabs(obb6.extents.z) < 1e-6f);

    return 0;
}
