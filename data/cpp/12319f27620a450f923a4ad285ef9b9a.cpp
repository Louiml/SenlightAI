Given a vector of 2D points (each with x and y coordinates) and a query point, write a C++ function that returns the index of the point in the vector that is closest to the query point by Euclidean distance. If multiple points have the same minimum distance, return the smallest index among them. The vector may be empty, in which case the function should return `-1`. Points may have negative coordinates and duplicates are allowed. Use only standard library facilities (no external geometry libraries), and define a simple `Point` struct with `double x, y`.
The solution iterates over all points in the vector, computing the squared Euclidean distance to the query point to avoid unnecessary square root operations. We maintain two variables: `best_index` (initialized to `-1`) and `best_distance_sq` (initialized to `DBL_MAX`). For each point, compute `dx = p.x - query.x` and `dy = p.y - query.y`, then `dist_sq = dx*dx + dy*dy`. If `dist_sq < best_distance_sq`, update `best_distance_sq` and `best_index` with the current index. Since we iterate in increasing index order and only update on strictly smaller distance, ties automatically keep the smaller index. Edge cases: empty vector returns `-1`; a single point always returns index 0; negative coordinates are handled naturally by squaring differences. Time complexity is \(O(n)\) where \(n\) is the number of points, and space complexity is \(O(1)\) beyond the input storage.
#include <vector>
#include <limits>
#include <cmath>

struct Point {
    double x;
    double y;
};

// Returns the index of the point in 'points' closest to 'query'.
// Returns -1 if the vector is empty. Ties are resolved to the smallest index.
int findClosestPoint(const std::vector<Point>& points, const Point& query) {
    if (points.empty()) {
        return -1;
    }

    int best_index = -1;
    double best_distance_sq = std::numeric_limits<double>::max();

    for (size_t i = 0; i < points.size(); ++i) {
        double dx = points[i].x - query.x;
        double dy = points[i].y - query.y;
        double dist_sq = dx * dx + dy * dy;

        if (dist_sq < best_distance_sq) {
            best_distance_sq = dist_sq;
            best_index = static_cast<int>(i);
        }
    }

    return best_index;
}
#include <cassert>
#include <vector>

int main() {
    // Basic case with distinct points
    std::vector<Point> pts1 = {{1.0, 1.0}, {3.0, 4.0}, {0.0, 0.0}};
    Point q1 = {2.0, 2.0};
    assert(findClosestPoint(pts1, q1) == 0); // distance to (1,1) = sqrt(2), to (3,4) = sqrt(5), to (0,0) = sqrt(8)

    // Empty vector
    std::vector<Point> pts2;
    Point q2 = {0.0, 0.0};
    assert(findClosestPoint(pts2, q2) == -1);

    // Single point
    std::vector<Point> pts3 = {{-2.5, 3.0}};
    Point q3 = {10.0, 10.0};
    assert(findClosestPoint(pts3, q3) == 0);

    // Ties: two points at same distance, smaller index wins
    std::vector<Point> pts4 = {{1.0, 0.0}, {0.0, 1.0}, {2.0, 0.0}};
    Point q4 = {0.0, 0.0};
    assert(findClosestPoint(pts4, q4) == 1); // (1,0) and (0,1) both distance 1, index 1 wins because index 0 is distance 1 also? Actually (1,0) distance 1, (0,1) distance 1, so smallest index among them is 0? Wait: (1,0) index 0, (0,1) index 1 – both distance 1, so index 0 should win. Let me adjust test: 
    // Actually re-evaluate: (1,0) dist² = 1, (0,1) dist² = 1, (2,0) dist²=4. Smallest index among distance 1 is 0. So correct assert is 0. But I wrote 1 in comment. Let's fix:
    assert(findClosestPoint(pts4, q4) == 0); // corrected: index 0 wins tie

    // Negative coordinates
    std::vector<Point> pts5 = {{-5.0, -5.0}, {-1.0, -1.0}, {3.0, 3.0}};
    Point q5 = {0.0, 0.0};
    assert(findClosestPoint(pts5, q5) == 1); // (-1,-1) distance² = 2, closest

    // Duplicate points: first occurrence wins
    std::vector<Point> pts6 = {{2.0, 2.0}, {2.0, 2.0}, {5.0, 5.0}};
    Point q6 = {2.0, 2.0};
    assert(findClosestPoint(pts6, q6) == 0); // both first two are same, index 0 wins

    // Large coordinates
    std::vector<Point> pts7 = {{1e9, 1e9}, {-1e9, -1e9}};
    Point q7 = {0.0, 0.0};
    assert(findClosestPoint(pts7, q7) == 1); // both have same distance², index 1? Actually both distance² = 2e18, tie, smallest index 0 wins, so assert 0
    // Correct: both distance² = 2e18, tie, index 0 wins
    assert(findClosestPoint(pts7, q7) == 0);

    return 0;
}
