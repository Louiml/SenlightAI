Write a C++ function `findConvexHullExtremePoints` that takes a vector of 3D points (represented as a simple struct with `double x, y, z`) and returns a small struct containing the minimum and maximum bounds (axis-aligned bounding box) of the points after applying a uniform scaling factor. The function must also return the indices of the points that achieve each of the six extreme coordinates (minimum x, maximum x, minimum y, maximum y, minimum z, maximum z). If there are no points, return a struct with default values and indices set to -1. The scaling factor is a positive non‑zero double. The input vector is not modified. The output struct should have fields `minX, maxX, minY, maxY, minZ, maxZ` (each as a double) and `minXIndex, maxXIndex, minYIndex, maxYIndex, minZIndex, maxZIndex` (each as an `int`). When multiple points share the same extreme value, choose the smallest index.

The task is a geometric bounding box computation with index tracking. The core algorithm is straightforward: iterate through each point exactly once. For each point, compute the scaled coordinates by multiplying each component by the given scaling factor. Maintain six running extreme values and six corresponding indices. Initialize all extreme values to the first point’s scaled coordinates and all indices to 0. For each subsequent point, compare its scaled x, y, z against the current min and max; update the value and index if it is strictly smaller for min or strictly larger for max (using `>` and `<` ensures ties keep the smallest index). Edge cases: empty input vector – must handle by returning a struct with all bounds set to 0.0 and all indices set to -1. Scaling factor is guaranteed positive and non‑zero, so multiplication is safe; if you want to be robust, you could check for zero but it’s not required. Complexity: time is O(n) where n is the number of points, space is O(1) extra (excluding the output struct). The solution is const‑correct: the input is passed as `const std::vector<Point3D>&`.

#include <vector>
#include <cstddef>

// Simple 3D point struct
struct Point3D {
    double x;
    double y;
    double z;
};

// Output struct holding bounds and indices of extreme points
struct ExtremePoints {
    double minX;
    double maxX;
    double minY;
    double maxY;
    double minZ;
    double maxZ;
    int minXIndex;
    int maxXIndex;
    int minYIndex;
    int maxYIndex;
    int minZIndex;
    int maxZIndex;
};

// Find the axis-aligned bounding box extremes after scaling all points.
// Returns the scaled bounds and the indices of the original points that achieve them.
// For an empty input, returns a struct with all zeros and all indices set to -1.
ExtremePoints findConvexHullExtremePoints(const std::vector<Point3D>& points, double scaling) {
    ExtremePoints result;
    if (points.empty()) {
        result.minX = 0.0; result.maxX = 0.0;
        result.minY = 0.0; result.maxY = 0.0;
        result.minZ = 0.0; result.maxZ = 0.0;
        result.minXIndex = -1; result.maxXIndex = -1;
        result.minYIndex = -1; result.maxYIndex = -1;
        result.minZIndex = -1; result.maxZIndex = -1;
        return result;
    }

    // Initialize with the first point
    result.minX = points[0].x * scaling;
    result.maxX = points[0].x * scaling;
    result.minY = points[0].y * scaling;
    result.maxY = points[0].y * scaling;
    result.minZ = points[0].z * scaling;
    result.maxZ = points[0].z * scaling;
    result.minXIndex = 0; result.maxXIndex = 0;
    result.minYIndex = 0; result.maxYIndex = 0;
    result.minZIndex = 0; result.maxZIndex = 0;

    // Iterate over the remaining points
    for (std::size_t i = 1; i < points.size(); ++i) {
        double sx = points[i].x * scaling;
        double sy = points[i].y * scaling;
        double sz = points[i].z * scaling;

        if (sx < result.minX) { result.minX = sx; result.minXIndex = static_cast<int>(i); }
        if (sx > result.maxX) { result.maxX = sx; result.maxXIndex = static_cast<int>(i); }
        if (sy < result.minY) { result.minY = sy; result.minYIndex = static_cast<int>(i); }
        if (sy > result.maxY) { result.maxY = sy; result.maxYIndex = static_cast<int>(i); }
        if (sz < result.minZ) { result.minZ = sz; result.minZIndex = static_cast<int>(i); }
        if (sz > result.maxZ) { result.maxZ = sz; result.maxZIndex = static_cast<int>(i); }
    }

    return result;
}

#include <cassert>
#include <cmath>

int main() {
    // Empty input
    {
        std::vector<Point3D> pts;
        ExtremePoints r = findConvexHullExtremePoints(pts, 2.0);
        assert(r.minXIndex == -1 && r.maxXIndex == -1);
        assert(r.minYIndex == -1 && r.maxYIndex == -1);
        assert(r.minZIndex == -1 && r.maxZIndex == -1);
        assert(r.minX == 0.0 && r.maxX == 0.0);
    }

    // Single point with scaling
    {
        std::vector<Point3D> pts = {{1.0, 2.0, 3.0}};
        ExtremePoints r = findConvexHullExtremePoints(pts, 0.5);
        assert(std::fabs(r.minX - 0.5) < 1e-9);
        assert(std::fabs(r.maxX - 0.5) < 1e-9);
        assert(std::fabs(r.minY - 1.0) < 1e-9);
        assert(std::fabs(r.maxZ - 1.5) < 1e-9);
        assert(r.minXIndex == 0 && r.maxZIndex == 0);
    }

    // Multiple points, ties choose smallest index
    {
        std::vector<Point3D> pts = {
            {2.0, 1.0, 5.0},   // index 0
            {-1.0, 3.0, 2.0},  // index 1
            {4.0, -2.0, 0.0},  // index 2
            {4.0, 1.0, 7.0},   // index 3 (tie for maxX, but index 2 is smaller)
            {0.0, 3.0, 7.0}    // index 4 (tie for maxY and maxZ)
        };
        ExtremePoints r = findConvexHullExtremePoints(pts, 1.0);
        assert(std::fabs(r.minX - (-1.0)) < 1e-9 && r.minXIndex == 1);
        assert(std::fabs(r.maxX - 4.0) < 1e-9 && r.maxXIndex == 2); // tie with index 3, pick 2
        assert(std::fabs(r.minY - (-2.0)) < 1e-9 && r.minYIndex == 2);
        assert(std::fabs(r.maxY - 3.0) < 1e-9 && r.maxYIndex == 1); // tie with index 4, pick 1
        assert(std::fabs(r.minZ - 0.0) < 1e-9 && r.minZIndex == 2);
        assert(std::fabs(r.maxZ - 7.0) < 1e-9 && r.maxZIndex == 3); // tie with index 4, pick 3
    }

    // Scaling factor changes values but indices remain based on original points
    {
        std::vector<Point3D> pts = {
            {1.0, -3.0, 2.0},
            {5.0, 0.0, -1.0},
            {-2.0, 4.0, 6.0}
        };
        ExtremePoints r = findConvexHullExtremePoints(pts, 3.0);
        assert(std::fabs(r.minX - (-6.0)) < 1e-9 && r.minXIndex == 2);
        assert(std::fabs(r.maxX - 15.0) < 1e-9 && r.maxXIndex == 1);
        assert(std::fabs(r.minY - (-9.0)) < 1e-9 && r.minYIndex == 0);
        assert(std::fabs(r.maxY - 12.0) < 1e-9 && r.maxYIndex == 2);
        assert(std::fabs(r.minZ - (-3.0)) < 1e-9 && r.minZIndex == 1);
        assert(std::fabs(r.maxZ - 18.0) < 1e-9 && r.maxZIndex == 2);
    }

    return 0;
}
