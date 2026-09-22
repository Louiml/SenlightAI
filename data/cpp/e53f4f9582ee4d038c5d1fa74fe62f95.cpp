// Write a C++ function named `smoothDiscontinuity` that takes a `std::vector` of 3D points (represented as a simple struct with `x`, `y`, `z` fields) and an integer index `discontinuity` where a sudden jump occurs between consecutive points. The function should smooth the transition at that index by blending the points before and after the discontinuity toward a common midpoint, using a cosine-based falloff so that points far from the discontinuity are barely affected, while points near it are pulled toward the midpoint. Specifically, for the left side (indices less than `discontinuity`), each point should be pushed toward the midpoint by a displacement that gradually increases from 0 at index 0 to a maximum at index `discontinuity-1`. For the right side (indices greater than or equal to `discontinuity`), each point should be pushed toward the midpoint by a displacement that gradually decreases from a maximum at index `discontinuity` to 0 at the end of the vector. The displacement should be computed as the difference between the midpoint and the original point at the discontinuity boundary, scaled by a smoothing factor that follows a cosine curve over each side. The function should modify the vector in place, and should handle edge cases where the discontinuity is too close to the ends by simply returning without modification. The midpoint is the average of the point just before and just after the discontinuity. Use only standard C++ libraries and a simple `Vec3` struct with `double` coordinates.

// The key idea is to blend two segments of a polyline smoothly at a given boundary point. Let `L = points[discontinuity-1]` and `R = points[discontinuity]` be the two points that form the discontinuity. Compute a midpoint `M = (L + R) / 2`. For the left segment (indices `0` to `discontinuity-1`), we want to gradually move each point toward `M` as we approach the boundary. Define a normalized parameter `t = i / (discontinuity - 1)` for `i` from 0 to `discontinuity-1` (if `discontinuity > 1`). The displacement for each left point is `(M - L) * (1 - cos(π * t)) / 2`, because at `t=0` (far left) the factor is 0, and at `t=1` (near boundary) the factor is 1. This ensures the left endpoint is moved fully to `M`, and the far end unchanged. Similarly, for the right segment (indices `discontinuity` to `size-1`), define `u = (i - discontinuity) / (size - discontinuity)` for `i` from `discontinuity` to `size-1` (if right side length > 1). The displacement for right points is `(M - R) * (1 - cos(π * u)) / 2`. At `u=0` (near boundary) factor is 0, at `u=1` (far end) factor is 1, so the right boundary point is moved to `M` and the far end unchanged. If either side has length 1 (i.e., discontinuity is 0 or size-1), we cannot define a meaningful smoothing, so return early. The algorithm runs in O(n) time, where n is the number of points, and uses O(1) extra space. Edge cases: discontinuity must be at least 1 and at most size-1, otherwise return. Also ensure the left side has at least 2 points (discontinuity ≥ 2) and right side at least 2 points (size - discontinuity ≥ 2), otherwise no smoothing is performed.

#include <vector>
#include <cmath>
#include <cstddef>

struct Vec3 {
    double x, y, z;
    // Simple arithmetic operators for convenience
    Vec3 operator+(const Vec3& o) const { return {x+o.x, y+o.y, z+o.z}; }
    Vec3 operator-(const Vec3& o) const { return {x-o.x, y-o.y, z-o.z}; }
    Vec3 operator*(double s) const { return {x*s, y*s, z*s}; }
    Vec3 operator/(double s) const { return {x/s, y/s, z/s}; }
    Vec3& operator+=(const Vec3& o) { x+=o.x; y+=o.y; z+=o.z; return *this; }
};

// Smooth the discontinuity at index 'discontinuity' by blending points toward a midpoint.
void smoothDiscontinuity(std::vector<Vec3>& points, int discontinuity) {
    const int n = static_cast<int>(points.size());
    if (n < 3) return;
    if (discontinuity <= 0 || discontinuity >= n) return;
    // Need at least one point on each side; if either side is exactly one point, we can't smooth.
    if (discontinuity < 2 || n - discontinuity < 2) return;

    // Midpoint between the two boundary points.
    Vec3 L = points[discontinuity - 1];
    Vec3 R = points[discontinuity];
    Vec3 M = (L + R) / 2.0;

    // Smooth left side: indices 0 .. discontinuity-1.
    double leftLen = static_cast<double>(discontinuity - 1);
    Vec3 leftDisplacement = M - L;
    for (int i = 0; i <= discontinuity - 1; ++i) {
        double t = static_cast<double>(i) / leftLen;  // 0 at far left, 1 at boundary
        double factor = (1.0 - std::cos(M_PI * t)) / 2.0;
        points[i] = points[i] + (leftDisplacement * factor);
    }

    // Smooth right side: indices discontinuity .. n-1.
    double rightLen = static_cast<double>(n - discontinuity);
    Vec3 rightDisplacement = M - R;
    for (int i = discontinuity; i < n; ++i) {
        double u = static_cast<double>(i - discontinuity) / rightLen; // 0 at boundary, 1 at far right
        double factor = (1.0 - std::cos(M_PI * u)) / 2.0;
        points[i] = points[i] + (rightDisplacement * factor);
    }
}

#include <cassert>
#include <cmath>

// Helper to compare doubles with tolerance
bool approx(double a, double b, double eps=1e-9) { return std::fabs(a-b) < eps; }

int main() {
    // Case 1: simple jump at middle
    std::vector<Vec3> pts1 = {{0,0,0}, {1,0,0}, {10,0,0}, {11,0,0}, {12,0,0}};
    smoothDiscontinuity(pts1, 2);
    // Boundary points should be exactly midpoint (5,0,0) because factor=1 at boundary on both sides
    assert(approx(pts1[1].x, 5.0) && approx(pts1[1].y, 0.0) && approx(pts1[1].z, 0.0));
    assert(approx(pts1[2].x, 5.0) && approx(pts1[2].y, 0.0) && approx(pts1[2].z, 0.0));
    // Far ends unchanged
    assert(approx(pts1[0].x, 0.0));
    assert(approx(pts1[4].x, 12.0));

    // Case 2: no change for invalid discontinuity (too close to end)
    std::vector<Vec3> pts2 = {{0,0,0}, {1,0,0}, {2,0,0}};
    smoothDiscontinuity(pts2, 1); // right side only 1 point -> return
    assert(approx(pts2[0].x, 0.0) && approx(pts2[1].x, 1.0) && approx(pts2[2].x, 2.0));

    // Case 3: discontinuity at index 2 with longer sides
    std::vector<Vec3> pts3 = {{0,0,0}, {2,0,0}, {4,0,0}, {20,0,0}, {22,0,0}, {24,0,0}};
    smoothDiscontinuity(pts3, 3);
    // Midpoint = (4+20)/2 = 12
    assert(approx(pts3[2].x, 12.0)); // left boundary moved to midpoint
    assert(approx(pts3[3].x, 12.0)); // right boundary moved to midpoint
    // Far left point unchanged (factor=0)
    assert(approx(pts3[0].x, 0.0));
    // Far right point unchanged (factor=0)
    assert(approx(pts3[5].x, 24.0));

    // Case 4: all zeros, does not crash
    std::vector<Vec3> pts4(5, {1.0, 2.0, 3.0});
    smoothDiscontinuity(pts4, 2);
    for (auto& p : pts4) {
        assert(approx(p.x, 1.0) && approx(p.y, 2.0) && approx(p.z, 3.0));
    }

    // Case 5: 2D-like z constant, check symmetry
    std::vector<Vec3> pts5 = {{0,0,0}, {1,0,0}, {2,0,0}, {9,0,0}, {10,0,0}, {11,0,0}};
    smoothDiscontinuity(pts5, 3);
    // Midpoint = (2+9)/2 = 5.5
    assert(approx(pts5[2].x, 5.5));
    assert(approx(pts5[3].x, 5.5));
    // Left second point index 1: t=1/2, factor = (1 - cos(pi/2))/2 = 0.5
    // original 1 + (5.5-2)*0.5 = 1 + 1.75 = 2.75
    assert(approx(pts5[1].x, 2.75));
    // Right second-to-last index 4: u=1/2, similar factor 0.5
    // original 10 + (5.5-9)*0.5 = 10 - 1.75 = 8.25
    assert(approx(pts5[4].x, 8.25));

    return 0;
}
