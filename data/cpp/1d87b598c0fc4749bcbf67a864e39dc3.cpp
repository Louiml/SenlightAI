/*
Write a C++ function named `splitTrianglesByMean` that takes a vector of 3D centroid points (as `std::array<float, 3>` or a simple struct with `x`, `y`, `z`), along with a vector of triangle indices (each triangle represented as a `std::array<size_t, 3>` referencing vertices in a separate vertex list), and returns a `std::pair<std::vector<size_t>, std::vector<size_t>>` containing the indices of triangles assigned to the "left" and "right" partitions. The function must compute the mean centroid of all input triangles, then the sample variance (sum of squared distances from the mean, divided by count) along each axis, and choose the axis with the largest variance (the "highest deviation component"). A split plane is placed at the mean value along that axis. A triangle is assigned to the left partition if its centroid coordinate along the chosen axis is less than or equal to the split value; otherwise, it goes to the right partition. The function must preserve the relative order of triangle indices within each partition. Assume the input lists are non-empty and all triangle indices are valid. The function should be `const`-correct, use `std::vector` and `std::array`, and avoid dynamic memory allocation beyond the output vectors.
*/
#include <vector>
#include <array>
#include <utility>
#include <cstddef>

// Represents a 3D point (centroid) with x, y, z.
struct Point3D {
    float x, y, z;
};

// Compute mean of all centroids.
Point3D computeMean(const std::vector<Point3D>& centroids) {
    Point3D mean{0.0f, 0.0f, 0.0f};
    if (centroids.empty()) return mean;
    for (const auto& p : centroids) {
        mean.x += p.x;
        mean.y += p.y;
        mean.z += p.z;
    }
    const float inv = 1.0f / static_cast<float>(centroids.size());
    mean.x *= inv;
    mean.y *= inv;
    mean.z *= inv;
    return mean;
}

// Split triangle indices based on mean centroid along highest variance axis.
std::pair<std::vector<size_t>, std::vector<size_t>> splitTrianglesByMean(
    const std::vector<Point3D>& centroids,
    const std::vector<std::array<size_t, 3>>& triangles)
{
    // Centroids are given per triangle index; triangles vector may be unused
    // except to compute centroids? Here we assume centroids already correspond.
    // For completeness, if triangles are needed to compute centroids from vertices,
    // we'd need vertices too. The task says centroids are given directly.
    (void)triangles; // Not used if centroids are precomputed.

    const size_t n = centroids.size();
    Point3D mean = computeMean(centroids);

    // Compute variance (mean squared deviation) per axis.
    float varX = 0.0f, varY = 0.0f, varZ = 0.0f;
    for (const auto& p : centroids) {
        float dx = p.x - mean.x;
        float dy = p.y - mean.y;
        float dz = p.z - mean.z;
        varX += dx * dx;
        varY += dy * dy;
        varZ += dz * dz;
    }
    if (n > 0) {
        varX /= static_cast<float>(n);
        varY /= static_cast<float>(n);
        varZ /= static_cast<float>(n);
    }

    // Choose axis with highest variance.
    int axis = 0; // 0=x, 1=y, 2=z
    float maxVar = varX;
    if (varY > maxVar) { maxVar = varY; axis = 1; }
    if (varZ > maxVar) { axis = 2; }

    float split = mean.x;
    if (axis == 1) split = mean.y;
    else if (axis == 2) split = mean.z;

    std::pair<std::vector<size_t>, std::vector<size_t>> result;
    for (size_t i = 0; i < n; ++i) {
        float coord = centroids[i].x;
        if (axis == 1) coord = centroids[i].y;
        else if (axis == 2) coord = centroids[i].z;

        if (coord <= split) {
            result.first.push_back(i);
        } else {
            result.second.push_back(i);
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <array>
#include <utility>

// Assume the solution above is included before this.

int main() {
    // Test 1: Simple 2D-like split along Y (highest variance in y).
    std::vector<Point3D> centroids = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {1.0f, 1.0f, 0.0f}
    };
    std::vector<std::array<size_t, 3>> triangles = {{0,1,2},{1,2,3},{0,2,3},{0,1,3}};
    auto res = splitTrianglesByMean(centroids, triangles);
    // Mean = (0.5,0.5,0). Variances: x=0.25, y=0.25, z=0. Highest tie -> x chosen (axis=0).
    // Split x=0.5. Left: centroids with x<=0.5 -> indices 0 and 2. Right: 1 and 3.
    assert(res.first == std::vector<size_t>({0, 2}));
    assert(res.second == std::vector<size_t>({1, 3}));

    // Test 2: All centroids identical -> all go left (axis x, split = value).
    std::vector<Point3D> same = {{2.0f, 3.0f, 4.0f}, {2.0f, 3.0f, 4.0f}, {2.0f, 3.0f, 4.0f}};
    std::vector<std::array<size_t, 3>> tri2 = {{0,1,2},{0,1,2},{0,1,2}};
    auto res2 = splitTrianglesByMean(same, tri2);
    assert(res2.first.size() == 3);
    assert(res2.second.empty());

    // Test 3: Single triangle.
    std::vector<Point33> single = {{1.0f, -1.0f, 0.5f}};
    std::vector<std::array<size_t, 3>> tri3 = {{0,0,0}};
    auto res3 = splitTrianglesByMean(single, tri3);
    assert(res3.first == std::vector<size_t>({0}));
    assert(res3.second.empty());

    // Test 4: High variance in Z.
    std::vector<Point3D> zvar = {
        {0.0f, 0.0f, -10.0f},
        {0.0f, 0.0f, 10.0f},
        {0.0f, 0.0f, -5.0f},
        {0.0f, 0.0f, 5.0f}
    };
    std::vector<std::array<size_t, 3>> tri4 = {{0,1,2},{1,2,3},{0,2,3},{0,1,3}};
    auto res4 = splitTrianglesByMean(zvar, tri4);
    // Mean z=0. Variances: x=y=0, z=62.5? Actually avg of 100+100+25+25 = 250/4=62.5. Axis z chosen.
    // Split z=0. Left: indices with z<=0 -> 0 and 2. Right: 1 and 3.
    assert(res4.first == std::vector<size_t>({0, 2}));
    assert(res4.second == std::vector<size_t>({1, 3}));

    // Test 5: Negative coordinates.
    std::vector<Point3D> neg = {
        {-3.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {2.0f, 0.0f, 0.0f},
        {4.0f, 0.0f, 0.0f}
    };
    std::vector<std::array<size_t, 3>> tri5 = {{0,1,2},{0,1,2},{0,1,2},{0,1,2}};
    auto res5 = splitTrianglesByMean(neg, tri5);
    // Mean x=0.5? Actually (-3-1+2+4)/4 = 2/4=0.5. Variances: x only nonzero, so axis x.
    // Split 0.5. Left: -3 and -1 (indices 0,1). Right: 2 and 4 (indices 2,3).
    assert(res5.first == std::vector<size_t>({0, 1}));
    assert(res5.second == std::vector<size_t>({2, 3}));

    return 0;
}
// The algorithm proceeds in three steps: (1) compute the mean centroid vector as the average of all triangle centroids; (2) compute the variance along each of the three axes by summing squared differences between each centroid and the mean, then dividing by the number of triangles; (3) select the axis with the largest variance (using `std::max_element` or manual comparison), and compare each centroid's coordinate on that axis to the mean value. Triangles whose coordinate is ≤ mean go to the left partition; otherwise to the right. Edge cases: all centroids identical yields zero variance for all axes, so choosing the first axis (e.g., index 0) is fine; the split value equals the unique centroid, and all triangles go left. For a single triangle, it will always go left. Time complexity is O(n) for n triangles (each centroid processed twice), and space complexity is O(n) for the two output vectors. The variance computation uses floating-point, so no precision issues beyond standard `float` comparisons.
