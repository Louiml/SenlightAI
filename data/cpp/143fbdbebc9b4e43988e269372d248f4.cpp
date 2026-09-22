Write a C++ function that takes a vector of 2D points (each represented as a struct with `float x` and `float y`), and returns a struct representing a contact manifold containing: (1) the average normal vector (unit length) of all points relative to the origin, (2) the count of points, and (3) a vector of "contact points" where each contact point stores the original coordinates, the distance from the origin, and a boolean flag indicating whether that point is a "deepest" point (i.e., its distance is within 0.001 of the minimum distance among all points). If the input is empty, return a manifold with zero points, a zero normal, and an empty contact list. The normal must be computed by averaging the normalized direction vectors from the origin to each point; if the average is near-zero (length < 1e-6), fall back to a unit vector along the x-axis. Ensure the normal is always unit length. The function must be `const`-correct and operate on a `std::vector<Point>` passed by const reference.
// The solution processes each point individually to compute its direction vector (normalized) and its distance from the origin. Accumulate the sum of all direction vectors to obtain the average normal, then normalize it; if the average magnitude is too small, substitute a default normal. Simultaneously, track the minimum distance across all points. After the loop, build the output manifold: the normal (ensured unit length), the original point count, and a list of contact points, each with its coordinates, distance, and the "deepest" flag set if the distance is within 0.001 of the minimum (accounting for floating-point tolerance). Edge cases: empty input (return default manifold), single point (normal is that point’s direction; deepest flag true), all points collinear with origin (average normal may be zero, so fallback), and points with zero distance (direction undefined—treat as (1,0,0) for averaging, distance 0). Time complexity is O(n) for n points, space complexity O(n) for the output contact list. The function uses constant extra space besides the output.
#include <vector>
#include <cmath>
#include <algorithm>

struct Point {
    float x;
    float y;
};

struct ContactPoint {
    Point coords;
    float distance;
    bool isDeepest;
};

struct Manifold {
    Point normal;               // unit length
    int pointCount;
    std::vector<ContactPoint> points;
};

// Compute a contact manifold from a list of 2D points.
// Average direction vectors to get normal; find deepest points.
Manifold computeManifold(const std::vector<Point>& points) {
    Manifold result;
    result.pointCount = static_cast<int>(points.size());
    if (points.empty()) {
        result.normal = {1.0f, 0.0f};
        result.points.clear();
        return result;
    }

    // Accumulate average direction
    float sumX = 0.0f;
    float sumY = 0.0f;

    // Also compute distances and track minimum
    std::vector<float> distances(points.size());
    float minDist = std::numeric_limits<float>::max();

    for (size_t i = 0; i < points.size(); ++i) {
        float px = points[i].x;
        float py = points[i].y;
        float dist = std::sqrt(px * px + py * py);
        distances[i] = dist;
        minDist = std::min(minDist, dist);

        // Direction vector: if dist is nearly zero, use (1,0)
        float dx, dy;
        if (dist < 1e-6f) {
            dx = 1.0f;
            dy = 0.0f;
        } else {
            dx = px / dist;
            dy = py / dist;
        }
        sumX += dx;
        sumY += dy;
    }

    // Normalize average
    float avgLen = std::sqrt(sumX * sumX + sumY * sumY);
    if (avgLen < 1e-6f) {
        result.normal = {1.0f, 0.0f};
    } else {
        result.normal = {sumX / avgLen, sumY / avgLen};
    }

    // Build contact points
    result.points.reserve(points.size());
    for (size_t i = 0; i < points.size(); ++i) {
        ContactPoint cp;
        cp.coords = points[i];
        cp.distance = distances[i];
        cp.isDeepest = (std::fabs(cp.distance - minDist) < 0.001f);
        result.points.push_back(cp);
    }

    return result;
}
#include <cassert>
#include <cmath>
#include <iostream>

// Re-declare the structures and function (for self-contained test)
struct Point { float x, y; };
struct ContactPoint { Point coords; float distance; bool isDeepest; };
struct Manifold { Point normal; int pointCount; std::vector<ContactPoint> points; };

// Include the solution here (or paste the above function)
// For brevity, assume function is defined above this test.

int main() {
    // Empty input
    Manifold m0 = computeManifold({});
    assert(m0.pointCount == 0);
    assert(std::fabs(m0.normal.x - 1.0f) < 1e-6 && std::fabs(m0.normal.y) < 1e-6);
    assert(m0.points.empty());

    // Single point at (3,4) -> distance 5, normal (0.6,0.8), deepest true
    Manifold m1 = computeManifold({{3.0f, 4.0f}});
    assert(m1.pointCount == 1);
    assert(std::fabs(m1.normal.x - 0.6f) < 1e-5 && std::fabs(m1.normal.y - 0.8f) < 1e-5);
    assert(std::fabs(m1.points[0].distance - 5.0f) < 1e-6);
    assert(m1.points[0].isDeepest == true);

    // Two points: (1,0) and (0,2) -> average normal (0.5,1.0)/1.118 -> unit
    Manifold m2 = computeManifold({{1.0f, 0.0f}, {0.0f, 2.0f}});
    assert(m2.pointCount == 2);
    float len = std::sqrt(m2.normal.x * m2.normal.x + m2.normal.y * m2.normal.y);
    assert(std::fabs(len - 1.0f) < 1e-5);
    // Deepest: only first point (dist 1) vs second (dist 2)
    assert(m2.points[0].isDeepest == true);
    assert(m2.points[1].isDeepest == false);

    // All points on origin -> sum of directions zero, fallback normal (1,0)
    Manifold m3 = computeManifold({{0.0f, 0.0f}, {0.0f, 0.0f}});
    assert(m3.normal.x == 1.0f && m3.normal.y == 0.0f);
    assert(m3.points[0].isDeepest == true && m3.points[1].isDeepest == true);
    assert(std::fabs(m3.points[0].distance) < 1e-6);

    // Multiple points with close distances: (2,0) and (2.0005,0) -> both deepest within tolerance
    Manifold m4 = computeManifold({{2.0f, 0.0f}, {2.0005f, 0.0f}});
    assert(m4.points[0].isDeepest == true && m4.points[1].isDeepest == true);
    // Normal should be (1,0) exactly
    assert(std::fabs(m4.normal.x - 1.0f) < 1e-6 && std::fabs(m4.normal.y) < 1e-6);

    // Negative coordinates: (-3,4) -> normal (-0.6,0.8)
    Manifold m5 = computeManifold({{-3.0f, 4.0f}});
    assert(std::fabs(m5.normal.x + 0.6f) < 1e-5 && std::fabs(m5.normal.y - 0.8f) < 1e-5);
    assert(std::fabs(m5.points[0].distance - 5.0f) < 1e-6);
    assert(m5.points[0].isDeepest == true);

    std::cout << "All tests passed.\n";
    return 0;
}
