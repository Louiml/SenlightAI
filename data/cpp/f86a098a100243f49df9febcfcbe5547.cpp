// Write a C++ function `long long closestPairDistance(const std::vector<std::pair<int,int>>& points)` that takes a non-empty vector of 2D integer points and returns the squared Euclidean distance between the closest pair of points. The input vector is unsorted and may contain duplicate points (distance 0). The function must implement a divide-and-conquer algorithm with a `O(n log n)` worst-case time complexity. The returned value is the square of the minimum distance (i.e., do not take the square root), which avoids floating-point issues. The algorithm must correctly handle: small inputs (1 or 2 points), points with identical coordinates, and cases where the closest pair lies across the dividing line. The function should be `const`-correct and rely only on standard headers.

The solution uses the classic divide-and-conquer closest pair algorithm. First, sort the points by x-coordinate. Recursively split the sorted array into two halves at the middle index until subarrays have size 2 or 3, where we compute all pairwise distances directly. For a combined region, the minimal distance `d` from the left and right halves is computed. Then we build a vertical strip of points whose x-coordinate differs from the middle line by at most `ceil(sqrt(d))`. Sorting this strip by y-coordinate, we compare each point with at most the next 7 points (a known geometric bound) to check for closer pairs across the split. The time complexity is `O(n log n)` for the sort and recursion plus `O(n log n)` for the strip sorting per level, giving an overall `O(n log n)`; space complexity is `O(n)` due to recursion stack and strip vector. Edge cases: n=0 (should not be called per specification), n=1 (return 0), n=2 (return distance), and duplicates (return 0).

#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>

// Helper to compute squared Euclidean distance between two points.
static long long squaredDist(const std::pair<int,int>& a, const std::pair<int,int>& b) {
    long long dx = static_cast<long long>(b.first) - a.first;
    long long dy = static_cast<long long>(b.second) - a.second;
    return dx * dx + dy * dy;
}

// Divide-and-conquer function operating on a subrange [left, right] of sorted points.
static long long closestPairRec(const std::vector<std::pair<int,int>>& sorted, int left, int right) {
    if (right - left == 1) {
        return squaredDist(sorted[left], sorted[right]);
    }
    if (right - left == 2) {
        long long d1 = squaredDist(sorted[left], sorted[left+1]);
        long long d2 = squaredDist(sorted[left+1], sorted[right]);
        long long d3 = squaredDist(sorted[left], sorted[right]);
        return std::min({d1, d2, d3});
    }

    int mid = (left + right) / 2;
    long long dLeft = closestPairRec(sorted, left, mid);
    long long dRight = closestPairRec(sorted, mid + 1, right);
    long long d = std::min(dLeft, dRight);

    // Build vertical strip around the middle x-coordinate.
    double limit = std::ceil(std::sqrt(static_cast<double>(d)));
    long long limitLL = static_cast<long long>(limit);
    std::vector<std::pair<int,int>> strip;
    for (int i = left; i <= right; ++i) {
        long long dx = std::abs(static_cast<long long>(sorted[i].first) - sorted[mid].first);
        if (dx <= limitLL) {
            strip.push_back(sorted[i]);
        }
    }

    // Sort strip by y-coordinate.
    std::sort(strip.begin(), strip.end(), [](const std::pair<int,int>& a, const std::pair<int,int>& b) {
        return a.second < b.second;
    });

    for (size_t i = 0; i < strip.size(); ++i) {
        for (size_t j = i + 1; j <= i + 7 && j < strip.size(); ++j) {
            d = std::min(d, squaredDist(strip[i], strip[j]));
        }
    }
    return d;
}

// Public function: returns squared distance of closest pair.
long long closestPairDistance(const std::vector<std::pair<int,int>>& points) {
    if (points.size() < 2) return 0;  // Edge case: distance to itself is 0.
    std::vector<std::pair<int,int>> sorted = points;
    std::sort(sorted.begin(), sorted.end(), [](const std::pair<int,int>& a, const std::pair<int,int>& b) {
        return a.first < b.first;
    });
    return closestPairRec(sorted, 0, static_cast<int>(sorted.size()) - 1);
}

#include <cassert>
#include <vector>
#include <utility>

// Declare the solution function (implemented above).
long long closestPairDistance(const std::vector<std::pair<int,int>>& points);

int main() {
    // Single point: distance to itself is 0.
    std::vector<std::pair<int,int>> p1 = {{0,0}};
    assert(closestPairDistance(p1) == 0);

    // Two points.
    std::vector<std::pair<int,int>> p2 = {{1,2},{4,6}};
    assert(closestPairDistance(p2) == 25); // (3^2 + 4^2 = 25)

    // Duplicate points: distance 0.
    std::vector<std::pair<int,int>> p3 = {{5,5},{5,5},{10,10}};
    assert(closestPairDistance(p3) == 0);

    // Standard case: closest pair is (0,0)-(3,4) → 25, others far apart.
    std::vector<std::pair<int,int>> p4 = {{0,0},{3,4},{100,100},{200,200}};
    assert(closestPairDistance(p4) == 25);

    // Negative coordinates and cross-split pair.
    std::vector<std::pair<int,int>> p5 = {{-10,0},{0,0},{1,1},{10,0}};
    // Closest: (0,0)-(1,1) → 2, but (0,0) and (1,1) are on different sides? Actually after sort: -10,0,1,10 → split mid=1? left=0,right=3 → mid=1. left half: [-10,0], right half: [1,1] and [10,0]. Cross pair (0,0)-(1,1) distance=2, and (1,1)-(10,0)=82. So answer=2.
    assert(closestPairDistance(p5) == 2);

    // Random larger (implicitly correct small answer).
    std::vector<std::pair<int,int>> p6 = {{0,0},{1,0},{2,0},{3,0},{4,1}};
    // Closest pair among (0,0)-(1,0)=1, (4,1) far. Answer=1.
    assert(closestPairDistance(p6) == 1);

    // All collinear equal spacing.
    std::vector<std::pair<int,int>> p7 = {{0,0},{3,0},{6,0},{9,0}};
    assert(closestPairDistance(p7) == 9);

    // Points with same x but different y.
    std::vector<std::pair<int,int>> p8 = {{0,0},{0,2},{0,5}};
    assert(closestPairDistance(p8) == 4);

    // Large coordinates to test overflow handling (though long long should be fine).
    std::vector<std::pair<int,int>> p9 = {{-100000, -100000}, {100000, 100000}};
    assert(closestPairDistance(p9) == 80000000000LL); // (200000^2 + 200000^2)

    // Edge: exactly 3 points, closest is pair across the middle.
    std::vector<std::pair<int,int>> p10 = {{0,0},{1,10},{2,0}};
    // Sorted: (0,0),(1,10),(2,0). Closest: (0,0)-(2,0) = 4, (0,0)-(1,10)=101, (1,10)-(2,0)=101 → answer=4.
    assert(closestPairDistance(p10) == 4);

    return 0;
}
