/*
Given a three-dimensional axis-aligned bounding box defined by its minimum and maximum corners (both as 3D floating-point vectors), and a set of points also represented as 3D floating-point vectors, write a C++ function that returns the number of points that lie strictly inside the box (i.e., not on the boundary). The function should be robust to floating-point edge cases by treating points exactly on the boundary as outside. Additionally, the box must be valid (minimum corner components are strictly less than corresponding maximum components); if the box is degenerate (any dimension has min >= max), the function should return 0.
*/

#include <vector>
#include <cstddef>

struct Vec3f {
    float x, y, z;
};

// Count points strictly inside the axis-aligned box defined by min and max corners.
// The box must be non-degenerate (min < max component-wise); degenerate boxes return 0.
// Points on the boundary are considered outside.
std::size_t countPointsInsideBox(const Vec3f& minCorner, const Vec3f& maxCorner,
                                 const std::vector<Vec3f>& points) {
    // Check for degenerate box
    if (minCorner.x >= maxCorner.x || minCorner.y >= maxCorner.y || minCorner.z >= maxCorner.z) {
        return 0;
    }

    std::size_t count = 0;
    for (const Vec3f& p : points) {
        if (p.x > minCorner.x && p.x < maxCorner.x &&
            p.y > minCorner.y && p.y < maxCorner.y &&
            p.z > minCorner.z && p.z < maxCorner.z) {
            ++count;
        }
    }
    return count;
}

#include <cassert>
#include <vector>
#include <cstddef>

struct Vec3f {
    float x, y, z;
};

// (function definition here as above)

int main() {
    // Basic box with points inside and on boundary
    Vec3f min{0.0f, 0.0f, 0.0f};
    Vec3f max{10.0f, 10.0f, 10.0f};
    std::vector<Vec3f> points = {
        {5.0f, 5.0f, 5.0f},   // inside
        {0.0f, 5.0f, 5.0f},   // on face x=min -> outside
        {10.0f, 5.0f, 5.0f},  // on face x=max -> outside
        {0.0f, 0.0f, 0.0f},   // corner -> outside
        {9.999f, 9.999f, 9.999f} // inside (just below max)
    };
    assert(countPointsInsideBox(min, max, points) == 2);

    // Degenerate box (zero width)
    Vec3f minD{1.0f, 1.0f, 1.0f};
    Vec3f maxD{1.0f, 5.0f, 5.0f};
    std::vector<Vec3f> ptsD = {{1.0f, 2.0f, 3.0f}, {2.0f, 2.0f, 2.0f}};
    assert(countPointsInsideBox(minD, maxD, ptsD) == 0);

    // Degenerate box (negative volume)
    Vec3f minN{5.0f, 0.0f, 0.0f};
    Vec3f maxN{0.0f, 10.0f, 10.0f};
    std::vector<Vec3f> ptsN = {{2.0f, 2.0f, 2.0f}};
    assert(countPointsInsideBox(minN, maxN, ptsN) == 0);

    // Empty point list
    std::vector<Vec3f> empty;
    assert(countPointsInsideBox(min, max, empty) == 0);

    // Negative coordinates and points on boundary
    Vec3f minNeg{-5.0f, -5.0f, -5.0f};
    Vec3f maxNeg{0.0f, 0.0f, 0.0f};
    std::vector<Vec3f> ptsNeg = {
        {-3.0f, -3.0f, -3.0f}, // inside
        {-5.0f, -2.0f, -2.0f}, // on min face -> outside
        {0.0f, -1.0f, -1.0f},  // on max face -> outside
        {-4.0f, -4.0f, -4.0f}  // inside
    };
    assert(countPointsInsideBox(minNeg, maxNeg, ptsNeg) == 2);
}

// The solution iterates over each point and checks whether it satisfies `min.x < p.x < max.x`, `min.y < p.y < max.y`, and `min.z < p.z < max.z` simultaneously. A point is counted only if it meets all three strict inequalities. The boundary is excluded by requiring strict inequality on every coordinate. The degenerate box case is handled upfront: if any dimension has `min >= max`, the box has zero volume and no point can be strictly inside, so return 0 immediately. The algorithm runs in O(n) time, where n is the number of points, and uses O(1) auxiliary space (excluding the input container). Floating-point comparisons are done directly; the strict comparisons naturally handle points exactly on a face or edge as outside. No special epsilon is needed because the specification explicitly defines "strictly inside" as using `>` and `<`.
