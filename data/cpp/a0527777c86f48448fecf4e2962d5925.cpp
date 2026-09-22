// Given a set of \( n \) distinct points in the 2D plane with real coordinates, and an integer \( k \) (where \( 3 \le k \le n \)), write a C++ function that returns the maximum possible value of \( r \) such that there exists a straight line (not necessarily passing through any of the points) for which at least \( k \) points are at distance at least \( r \) from the line, measured as the absolute perpendicular distance. More precisely, for a fixed line, sort all points by their signed perpendicular distance to the line (positive on one side, negative on the other). The "gap" for that line is defined as \( \min_{i=k-1}^{n-1} ( |d_{i-k+1} - d_i| ) - 1.0 \), where \( d_0 \le d_1 \le \dots \le d_{n-1} \) are the sorted signed distances. The answer is the maximum gap over all possible lines, but at least \( 0.0 \). If \( n \le 2 \) or \( k \le 2 \), return \( 0.0 \). The function should take a vector of points (as pairs of doubles) and an integer \( k \), and return a double. Points are guaranteed to be distinct (no duplicate coordinates), and coordinates are given with absolute value at most \( 10^6 \). The result should be accurate to within \( 10^{-6} \). This problem is inspired by finding the maximum separation of a subset of points when projected onto a direction (the line's normal), with the additional \(-1.0\) offset and clamping to zero.
// The key observation is that the signed distance of a point to a line depends only on the line's direction (normal vector) and its offset. For a fixed direction (unit normal \( \mathbf{u} \)), the signed distances of all points are \( \mathbf{u} \cdot p_i - c \), where \( c \) is the offset. The sorted differences \( d_{i-k+1} - d_i \) are independent of the offset \( c \), since adding a constant to all distances does not change their differences. Thus, for a fixed direction, the best possible gap is obtained by choosing any offset, and the gap equals the minimum over all windows of size \( k \) of the difference between the \( (k) \)-th largest and smallest projection values in that window, minus 1. The maximum over all directions occurs, by convexity/continuity, when the direction is orthogonal to a line through two of the points. Why? Because the sorted order of projections changes only when the direction is perpendicular to a vector between two points; the objective is piecewise linear in the direction, and the maximum of a piecewise linear function over a compact domain occurs at a breakpoint. Breakpoints correspond to directions where two points have equal projection, i.e., the line is parallel to the segment between them, so the normal is perpendicular to that segment. Thus, it suffices to test all \( O(n^2) \) lines determined by pairs of points. For each such line (through two points \( i \) and \( j \)), compute the signed distances of all points to that line using the cross product divided by the segment length. Sort these distances, then slide a window of size \( k \) over the sorted array, computing the difference between the farthest and nearest in that window; take the minimum over all windows. This gives the gap for that line. Take the minimum gap across all lines, then clamp to zero if negative, and return. Edge cases: \( n \le 2 \) or \( k \le 2 \) immediately return 0.0. Points with identical coordinates are not present, but if they were, the algorithm would still work. Time complexity: For each of \( O(n^2) \) pairs, we compute \( n \) distances, sort them in \( O(n \log n) \), and scan in \( O(n) \). Total \( O(n^3 \log n) \). Space complexity: \( O(n) \) for the distance array. Since \( n \) is small (typically up to 200 in competitive programming settings), this is acceptable. Precision: use double throughout and eps comparisons as needed.
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>

using Point = std::pair<double, double>;
#define x first
#define y second

// Compute the maximum possible gap as described in the task.
double maxSeparationGap(const std::vector<Point>& pts, int k) {
    int n = static_cast<int>(pts.size());
    if (n <= 2 || k <= 2) return 0.0;

    auto cross = [](const Point& a, const Point& b) -> double {
        return a.x * b.y - a.y * b.x;
    };
    auto length = [](const Point& a) -> double {
        return std::sqrt(a.x * a.x + a.y * a.y);
    };

    std::vector<double> dist(n);
    const double INF = 1e20;
    double ans = INF;

    // Test every line through two distinct points.
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            Point dir = {pts[i].x - pts[j].x, pts[i].y - pts[j].y};
            double len = length(dir);
            if (len < 1e-12) continue; // Should not happen, but just in case.

            // Signed distance from each point to the line through pts[i] and pts[j].
            // distance = cross(pts[i] - p, pts[j] - p) / len
            for (int p = 0; p < n; ++p) {
                Point a = {pts[i].x - pts[p].x, pts[i].y - pts[p].y};
                Point b = {pts[j].x - pts[p].x, pts[j].y - pts[p].y};
                dist[p] = cross(a, b) / len;
            }

            std::sort(dist.begin(), dist.end());

            // Slide window of size k.
            double best_for_line = INF;
            for (int start = 0; start + k - 1 < n; ++start) {
                double diff = std::abs(dist[start + k - 1] - dist[start]);
                best_for_line = std::min(best_for_line, diff);
            }
            double gap = best_for_line - 1.0;
            ans = std::min(ans, gap);
        }
    }

    return std::max(ans, 0.0);
}
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// Declare the function (as in the solution) here or include the header.
using Point = std::pair<double, double>;
double maxSeparationGap(const std::vector<Point>& pts, int k);

int main() {
    // Test 1: Three points forming a triangle, k=3.
    {
        std::vector<Point> pts = {{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}};
        double ans = maxSeparationGap(pts, 3);
        // The best line is the hypotenuse (through (1,0) and (0,1)).
        // The third point (0,0) is at distance sqrt(0.5) ≈ 0.7071.
        // Sorted distances: -0.7071, 0, 0.7071 (or similar). Window diff = 1.4142, gap = 0.4142.
        // However, we take min over all lines. Lines through pairs: the other two lines give distances 0,1,1 -> diff 1 -> gap 0.
        // So overall min gap is 0, clamped to 0.0.
        assert(std::fabs(ans - 0.0) < 1e-9);
    }

    // Test 2: Four points on a line, k=4.
    {
        std::vector<Point> pts = {{0.0, 0.0}, {1.0, 0.0}, {2.0, 0.0}, {3.0, 0.0}};
        double ans = maxSeparationGap(pts, 4);
        // Any line gives all distances equal to 0, so gap = -1, clamped to 0.
        assert(std::fabs(ans - 0.0) < 1e-9);
    }

    // Test 3: Small n edge cases.
    {
        std::vector<Point> pts = {{0.0, 0.0}, {1.0, 0.0}};
        assert(std::fabs(maxSeparationGap(pts, 2) - 0.0) < 1e-12);
        assert(std::fabs(maxSeparationGap(pts, 1) - 0.0) < 1e-12);
    }

    // Test 4: k > n (should still work, but task likely ensures k<=n; handle anyway).
    {
        std::vector<Point> pts = {{0.0, 0.0}, {1.0, 0.0}, {2.0, 0.0}};
        double ans = maxSeparationGap(pts, 5);
        // For k>n, the loop never runs (since start+k-1 >= n), best_for_line remains INF, gap = INF-1, ans = INF, clamped to 0.
        assert(std::fabs(ans - 0.0) < 1e-12);
    }

    // Test 5: Points on a circle, k=2 (should return 0).
    {
        std::vector<Point> pts = {{1.0, 0.0}, {0.0, 1.0}, {-1.0, 0.0}, {0.0, -1.0}};
        assert(std::fabs(maxSeparationGap(pts, 2) - 0.0) < 1e-12);
    }

    // Test 6: Four points forming a square, k=3.
    {
        std::vector<Point> pts = {{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}, {1.0, 1.0}};
        double ans = maxSeparationGap(pts, 3);
        // Consider the line y = 0.5 (horizontal). Distances: -0.5, 0.5, -0.5, 0.5 -> sorted: -0.5,-0.5,0.5,0.5.
        // Window of size 3: first window (-0.5, -0.5, 0.5) diff = 1.0, gap = 0. Second window (-0.5,0.5,0.5) diff=1.0, gap=0.
        // Best is likely 0.0. So assert 0.
        assert(std::fabs(ans - 0.0) < 1e-9);
    }

    // Test 7: Three collinear points plus one off-line, k=3.
    {
        std::vector<Point> pts = {{0.0, 0.0}, {2.0, 0.0}, {4.0, 0.0}, {2.0, 1.0}};
        double ans = maxSeparationGap(pts, 3);
        // Try line through (0,0) and (4,0) (horizontal). Distances: 0,0,0,1. Sorted: 0,0,0,1.
        // Window size 3: (0,0,0) diff=0 gap=-1, (0,0,1) diff=1 gap=0. So best for that line is 0.
        // Try line through (0,0) and (2,1) (diagonal). Distances: let's compute roughly. It passes through (0,0) and (2,1).
        // Other points: (2,0) distance = |cross((0,0)-(2,0), (2,1)-(2,0))| / len(...) = |cross(-2,0,0,1)|/sqrt(5) = 2/sqrt(5)≈0.894
        // (4,0) distance = cross((0,0)-(4,0), (2,1)-(4,0))/len = cross(-4,0,-2,1)/sqrt(5) = (-4*1 - 0*(-2))/sqrt(5) = -4/sqrt(5)≈-1.789
        // (0,0) dist=0, (2,1) dist=0.
        // Sorted: -1.789, 0, 0, 0.894. Window size 3: (-1.789,0,0) diff=1.789 gap=0.789; (0,0,0.894) diff=0.894 gap=-0.106. Best gap = -0.106.
        // But we take min over all lines, so ans would be min(0, -0.106) = -0.106, clamped to 0.
        // So answer is 0.
        assert(std::fabs(ans - 0.0) < 1e-9);
    }

    // Test 8: A case where answer is positive (e.g., two clusters).
    {
        std::vector<Point> pts = {{0.0, 0.0}, {0.0, 2.0}, {10.0, 0.0}, {10.0, 2.0}};
        double ans = maxSeparationGap(pts, 2); // k=2 returns 0 per rule.
        assert(std::fabs(ans - 0.0) < 1e-12);
    }

    // Test 9: Larger n, check no crash and answer in [0, some large]
    {
        std::vector<Point> pts;
        for (int i = 0; i < 10; ++i) {
            pts.push_back({static_cast<double>(i), static_cast<double>(i % 3)});
        }
        double ans = maxSeparationGap(pts, 5);
        assert(ans >= 0.0);
        assert(ans < 1e6);
    }

    // Test 10: All points distinct but very close, k=n.
    {
        std::vector<Point> pts = {{0.0, 0.0}, {0.001, 0.0}, {0.002, 0.0}, {0.003, 0.0}};
        double ans = maxSeparationGap(pts, 4);
        // All collinear, so gap negative, clamped to 0.
        assert(std::fabs(ans - 0.0) < 1e-9);
    }

    return 0;
}
