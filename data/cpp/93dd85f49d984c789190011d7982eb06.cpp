// Given a set of \(n\) points in a 2D plane, where \(2 \le n \le 100{,}000\) and each coordinate is an integer in the range \([-10^9, 10^9]\), write a C++ function that computes the minimum Euclidean distance between any pair of distinct points. The input is provided as two vectors: one containing all x-coordinates and one containing all y-coordinates of equal length. The function must return the minimum distance as a `double`. Points may be duplicated, in which case the minimum distance is \(0\). The solution must be efficient enough for large \(n\) and avoid an \(O(n^2)\) brute-force approach. Handle floating-point precision carefully when comparing distances.

The problem is the classic "closest pair of points" problem solvable in \(O(n \log n)\) time using a divide-and-conquer strategy.  
**Approach:**  
1. Sort the points by x-coordinate.  
2. Recursively split the sorted array into left and right halves.  
3. For each half, compute the minimum distance recursively (base cases: 2 or 3 points handled directly by brute force).  
4. Let \(d = \min(\text{leftMin}, \text{rightMin})\).  
5. Consider a vertical strip of width \(2d\) centered at the middle x-coordinate.  
6. Filter points inside this strip and sort them by y-coordinate.  
7. For each point in the sorted strip, compare it with at most the next 7 points (theoretically, only 7 points can fit in a \(d \times d\) box).  
8. Update the overall minimum accordingly.  
**Edge cases:**  
- Exactly 2 points: return their distance.  
- Duplicate points: distance \(0\), handled naturally.  
- Large coordinates: use `int64_t` for squared differences to avoid overflow, but we can use `double` with direct Euclidean formula since coordinates fit in 32-bit and differences up to \(2\cdot10^9\) squared is \(4\cdot10^{18}\), which fits in `int64_t`. We'll compute using `double` for simplicity.  
- Floating-point precision: use direct `std::hypot` or `std::sqrt(pow)` but compare with a small epsilon (e.g., `1e-9`) if needed; in this solution we return exact `double` and compare with tolerance in tests.  
**Time complexity:** \(O(n \log n)\) for sorting plus each recursion level processes \(O(n)\) points in the strip, giving overall \(O(n \log n)\).  
**Space complexity:** \(O(n)\) auxiliary space for recursion and strip vectors.

#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <cassert>

struct Point {
    int64_t x, y;
    double DistanceTo(const Point& other) const {
        int64_t dx = x - other.x;
        int64_t dy = y - other.y;
        return std::sqrt(static_cast<double>(dx*dx + dy*dy));
    }
};

// Helper: brute force for small sets (size 2 or 3)
double BruteForce(const std::vector<Point>& pts, size_t l, size_t r) {
    double minDist = std::numeric_limits<double>::infinity();
    for (size_t i = l; i < r; ++i) {
        for (size_t j = i + 1; j < r; ++j) {
            minDist = std::min(minDist, pts[i].DistanceTo(pts[j]));
        }
    }
    return minDist;
}

// Recursive helper: points are already sorted by x in [l, r)
double ClosestPairRec(std::vector<Point>& pts, size_t l, size_t r) {
    if (r - l <= 3) {
        return BruteForce(pts, l, r);
    }
    size_t mid = l + (r - l) / 2;
    int64_t midX = pts[mid].x;
    double leftMin = ClosestPairRec(pts, l, mid);
    double rightMin = ClosestPairRec(pts, mid, r);
    double d = std::min(leftMin, rightMin);

    // Build strip: points whose x is within d from midX
    std::vector<Point> strip;
    for (size_t i = l; i < r; ++i) {
        if (std::abs(pts[i].x - midX) < d) {
            strip.push_back(pts[i]);
        }
    }
    // Sort strip by y
    std::sort(strip.begin(), strip.end(), [](const Point& a, const Point& b) {
        return a.y < b.y;
    });

    // Check each point against next up to 7 points
    for (size_t i = 0; i < strip.size(); ++i) {
        for (size_t j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < d; ++j) {
            d = std::min(d, strip[i].DistanceTo(strip[j]));
        }
    }
    return d;
}

// Public function: min Euclidean distance between any two points
// pointsX and pointsY must be of equal length and size >= 2.
double MinimalDistanceFast(const std::vector<int64_t>& pointsX, const std::vector<int64_t>& pointsY) {
    assert(pointsX.size() == pointsY.size());
    assert(pointsX.size() >= 2);
    std::vector<Point> pts;
    pts.reserve(pointsX.size());
    for (size_t i = 0; i < pointsX.size(); ++i) {
        pts.push_back(Point{pointsX[i], pointsY[i]});
    }
    std::sort(pts.begin(), pts.end(), [](const Point& a, const Point& b) {
        return a.x < b.x;
    });
    return ClosestPairRec(pts, 0, pts.size());
}

#include <cassert>
#include <cmath>
#include <vector>

// The solution function is declared above (assume included from the solution section)

int main() {
    // Test 1: simple two points
    {
        std::vector<int64_t> x = {0, 3};
        std::vector<int64_t> y = {0, 4};
        double ans = MinimalDistanceFast(x, y);
        assert(std::abs(ans - 5.0) < 1e-9);
    }
    // Test 2: duplicate points -> distance 0
    {
        std::vector<int64_t> x = {7, 1, 4, 7};
        std::vector<int64_t> y = {7, 100, 8, 7};
        double ans = MinimalDistanceFast(x, y);
        assert(ans == 0.0);
    }
    // Test 3: known small set
    {
        std::vector<int64_t> x = {4, -2, -3, -1, 2, -4, 1, -1, 3, -4, -2};
        std::vector<int64_t> y = {4, -2, -4, 3, 3, 0, 1, -1, -1, 2, 4};
        double ans = MinimalDistanceFast(x, y);
        assert(std::abs(ans - std::sqrt(2.0)) < 1e-9);
    }
    // Test 4: another small set
    {
        std::vector<int64_t> x = {-2, 4, 3, -4, -1, 2};
        std::vector<int64_t> y = {1, -5, 2, 4, -4, -1};
        double ans = MinimalDistanceFast(x, y);
        assert(std::abs(ans - std::sqrt(10.0)) < 1e-9);
    }
    // Test 5: all points on a vertical line
    {
        std::vector<int64_t> x = {0, 0, 0, 0};
        std::vector<int64_t> y = {0, 10, 5, 2};
        double ans = MinimalDistanceFast(x, y);
        assert(std::abs(ans - 2.0) < 1e-9);
    }
    // Test 6: random-ish but known small
    {
        std::vector<int64_t> x = {-5, -5, -1, -4, 0, 0, 3, 5, 1, 5, 1, -5, -7};
        std::vector<int64_t> y = {6, 4, 4, 1, 5, 2, 1, 6, 0, 0, -2, -3, -5};
        double ans = MinimalDistanceFast(x, y);
        assert(std::abs(ans - std::sqrt(2.0)) < 1e-9);
    }
    // Test 7: large coordinates, far apart
    {
        std::vector<int64_t> x = {-1000000000, 1000000000};
        std::vector<int64_t> y = {-1000000000, 1000000000};
        double expected = std::sqrt(8.0e18); // (2e9)^2 * 2
        double ans = MinimalDistanceFast(x, y);
        assert(std::abs(ans - expected) < 1.0); // tolerance due to sqrt precision
    }
    // Test 8: three points forming a right triangle
    {
        std::vector<int64_t> x = {0, 3, 0};
        std::vector<int64_t> y = {0, 0, 4};
        double ans = MinimalDistanceFast(x, y);
        assert(std::abs(ans - 3.0) < 1e-9); // min(3,4,5) = 3
    }
    // Test 9: n=2 with negative coordinates
    {
        std::vector<int64_t> x = {-5, -2};
        std::vector<int64_t> y = {7, 3};
        double ans = MinimalDistanceFast(x, y);
        double expected = std::sqrt(3*3 + 4*4);
        assert(std::abs(ans - expected) < 1e-9);
    }
    // Test 10: many collinear points
    {
        std::vector<int64_t> x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::vector<int64_t> y = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        double ans = MinimalDistanceFast(x, y);
        assert(std::abs(ans - 1.0) < 1e-9);
    }
    return 0;
}
