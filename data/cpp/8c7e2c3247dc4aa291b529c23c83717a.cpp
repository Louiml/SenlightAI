/*
Write a C++ function `std::vector<unsigned int> findPointsWithinDistance(const std::vector<Point>& points, const Point& query, float radius)` that efficiently returns the indices of all points whose Euclidean distance from `query` is strictly less than `radius`. The `Point` struct must contain `float x, y, z` and support subtraction and dot product for distance computation. The function must first compute a reference plane (using a fixed normal vector like `(0.8523f, 0.34321f, 0.5736f)` normalized), project each point onto that plane to get a scalar distance value, sort points by that scalar, then use binary search to find the candidate range around `query`’s projection before filtering by actual 3D squared distance. This mirrors the spatial-sort acceleration technique from the snippet. The input vector may be empty; in that case return an empty result. Points at exactly `radius` distance must be excluded. The returned indices must be in ascending order of the original input positions.
*/

#include <vector>
#include <algorithm>
#include <cmath>
#include <cstddef>

struct Point {
    float x, y, z;
    
    Point() : x(0), y(0), z(0) {}
    Point(float px, float py, float pz) : x(px), y(py), z(pz) {}
    
    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y, z - other.z);
    }
    
    float dot(const Point& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    float squaredLength() const {
        return x * x + y * y + z * z;
    }
};

// Returns indices of points strictly within `radius` of `query`.
// Uses a reference-plane projection and sorting for efficient range queries.
std::vector<unsigned int> findPointsWithinDistance(
    const std::vector<Point>& points,
    const Point& query,
    float radius) {
    
    std::vector<unsigned int> result;
    if (points.empty() || radius <= 0.0f) {
        return result;
    }
    
    // Fixed reference plane normal (normalized)
    Point planeNormal(0.8523f, 0.34321f, 0.5736f);
    float normLen = std::sqrt(planeNormal.squaredLength());
    planeNormal.x /= normLen;
    planeNormal.y /= normLen;
    planeNormal.z /= normLen;
    
    // Compute centroid
    Point centroid(0.0f, 0.0f, 0.0f);
    for (const Point& p : points) {
        centroid.x += p.x;
        centroid.y += p.y;
        centroid.z += p.z;
    }
    float invN = 1.0f / static_cast<float>(points.size());
    centroid.x *= invN;
    centroid.y *= invN;
    centroid.z *= invN;
    
    // Build entries: original index and scalar distance along plane normal
    struct Entry {
        unsigned int index;
        float dist;
    };
    std::vector<Entry> entries;
    entries.reserve(points.size());
    for (unsigned int i = 0; i < points.size(); ++i) {
        Point diff = points[i] - centroid;
        entries.push_back({i, diff.dot(planeNormal)});
    }
    
    // Sort by scalar distance (ascending)
    std::sort(entries.begin(), entries.end(),
        [](const Entry& a, const Entry& b) { return a.dist < b.dist; });
    
    // Query projection
    Point qDiff = query - centroid;
    float qDist = qDiff.dot(planeNormal);
    float minDist = qDist - radius;
    float maxDist = qDist + radius;
    
    // Find range of entries whose dist is in [minDist, maxDist)
    // Use lower_bound and upper_bound for clarity; dist is strictly less than maxDist
    auto lowerIt = std::lower_bound(entries.begin(), entries.end(), minDist,
        [](const Entry& e, float value) { return e.dist < value; });
    auto upperIt = std::upper_bound(entries.begin(), entries.end(), maxDist,
        [](float value, const Entry& e) { return value < e.dist; });
    
    // Filter by actual 3D squared distance (< radius^2)
    float radiusSquared = radius * radius;
    for (auto it = lowerIt; it != upperIt; ++it) {
        Point diff = points[it->index] - query;
        if (diff.squaredLength() < radiusSquared) {
            result.push_back(it->index);
        }
    }
    
    // Result indices are already in ascending order because entries are sorted by dist,
    // but original indices might not be ascending. Sort to match original order? 
    // The task says "indices in ascending order of the original input positions", meaning 
    // the returned indices should be sorted by their numeric value (0,1,2...), not by distance.
    // So let's sort the result accordingly.
    std::sort(result.begin(), result.end());
    
    return result;
}

#include <cassert>
#include <vector>

// Point struct and function declaration would be here (from solution) — assume included.

int main() {
    // Test 1: Empty input
    std::vector<Point> empty;
    std::vector<unsigned int> r = findPointsWithinDistance(empty, Point(0,0,0), 1.0f);
    assert(r.empty());

    // Test 2: Radius zero or negative → empty
    std::vector<Point> pts = {Point(0,0,0), Point(1,0,0), Point(0,1,0)};
    r = findPointsWithinDistance(pts, Point(0,0,0), 0.0f);
    assert(r.empty());
    r = findPointsWithinDistance(pts, Point(0,0,0), -5.0f);
    assert(r.empty());

    // Test 3: Simple points on x-axis, query at origin, radius 1.5
    // Points: (0,0,0) dist 0; (1,0,0) dist 1; (2,0,0) dist 2; (-1,0,0) dist 1
    pts = {Point(0,0,0), Point(1,0,0), Point(2,0,0), Point(-1,0,0)};
    r = findPointsWithinDistance(pts, Point(0,0,0), 1.5f);
    // Should include indices 0, 1, 3 (distances 0, 1, 1) but not 2 (distance 2)
    assert(r.size() == 3);
    assert(r[0] == 0 && r[1] == 1 && r[2] == 3);

    // Test 4: Boundary exactly at radius excluded
    pts = {Point(0,0,0), Point(1,0,0)};
    r = findPointsWithinDistance(pts, Point(0,0,0), 1.0f);
    // Point (1,0,0) is exactly at distance 1.0, not strictly less → excluded
    assert(r.size() == 1);
    assert(r[0] == 0);

    // Test 5: All points close → all included (indices sorted)
    pts = {Point(0.1f,0.2f,0.3f), Point(-0.1f,0.0f,0.2f), Point(0.0f,0.0f,0.0f)};
    r = findPointsWithinDistance(pts, Point(0,0,0), 2.0f);
    assert(r.size() == 3);
    assert(r == std::vector<unsigned int>({0, 1, 2}));

    // Test 6: Larger set with mixed distances — verify correctness against brute force
    pts = {Point(3,4,12), Point(1,0,0), Point(0,0,1), Point(2,2,2), Point(-5,0,0), Point(0,0,0)};
    Point query(1,1,1);
    float radius = 3.0f;
    r = findPointsWithinDistance(pts, query, radius);
    // Brute force check
    std::vector<unsigned int> expected;
    for (unsigned int i = 0; i < pts.size(); ++i) {
        Point diff = pts[i] - query;
        if (diff.squaredLength() < radius * radius) {
            expected.push_back(i);
        }
    }
    assert(r == expected);

    // Test 7: Negative radius already covered; also test query far away → empty
    r = findPointsWithinDistance(pts, Point(100,100,100), 1.0f);
    assert(r.empty());
}

// The solution mirrors the `SpatialSort` approach: it precomputes a plane normal `n` (fixed arbitrary vector, normalized), computes `centroid` as the average of all points, then for each point computes a scalar `dist = dot(p - centroid, n)`. Sorting points by this scalar creates a 1D ordering that approximates spatial locality. To answer a query: compute `qDist = dot(query - centroid, n)`; find the range of sorted scalars within `[qDist - radius, qDist + radius]` using binary search (two bounds via `std::lower_bound` and `std::upper_bound`, or manual binary search as in snippet). Then iterate only over that candidate range and check if `squaredDistance(point, query) < radius*radius`. This filters out points that are near on the plane but far in 3D. The fixed plane normal might be degenerate if all points lie on that plane (then all scalars equal, and the method degrades to a full scan), but correctness is unaffected — only performance. Complexity: preprocessing is O(n log n) for sorting, O(n) for centroid. Each query is O(log n + k) where k is the number of candidates within the scalar range; worst-case O(n) if all points share similar projection, but that’s acceptable. Edge cases: empty input → return empty; radius <= 0 → return empty (since strictly less than radius, no point qualifies if radius ≤ 0); negative coordinates handled fine; floating-point precision: use squared distance to avoid sqrt. The result indices should be original positions in the input vector, not sorted order.
