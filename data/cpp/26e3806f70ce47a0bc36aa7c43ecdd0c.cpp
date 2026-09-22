Write a standalone C++ function `int maxPointsOnLine(const std::vector<Point>& points)` that takes a vector of 2D points (where `Point` is a simple struct with integer `x` and `y` coordinates, default and two-argument constructors) and returns the maximum number of points that lie on any single straight line. The function must handle duplicate points (identical coordinates) correctly—all duplicates count as separate points on any line passing through that coordinate—and must work correctly for vectors of size 0, 1, or 2, as well as for vertical lines (where the slope is infinite) and horizontal lines. Use a hash map to count occurrences of each slope for each anchor point, and avoid floating-point precision issues by using a normalized slope representation (e.g., a reduced fraction pair) instead of raw `float`. The function must be `const`-correct with respect to its input (taking the vector by const reference) and contain only the function (no global state, no `main`). Provide a complete, self-contained implementation that includes necessary headers (`<vector>`, `<map>`, `<utility>`, `<numeric>`, `<algorithm>`, etc.) and the `Point` struct definition.
#include <cassert>
#include <vector>

// The struct and function definition are assumed to be included above.

int main() {
    // Empty vector
    {
        std::vector<Point> pts;
        assert(maxPointsOnLine(pts) == 0);
    }

    // Single point
    {
        std::vector<Point> pts = {Point(1,2)};
        assert(maxPointsOnLine(pts) == 1);
    }

    // Two distinct points
    {
        std::vector<Point> pts = {Point(0,0), Point(3,4)};
        assert(maxPointsOnLine(pts) == 2);
    }

    // Two identical points
    {
        std::vector<Point> pts = {Point(5,5), Point(5,5)};
        assert(maxPointsOnLine(pts) == 2);
    }

    // All points on a horizontal line
    {
        std::vector<Point> pts = {Point(0,0), Point(1,0), Point(2,0), Point(3,0)};
        assert(maxPointsOnLine(pts) == 4);
    }

    // All points on a vertical line
    {
        std::vector<Point> pts = {Point(3,1), Point(3,5), Point(3,-2), Point(3,9)};
        assert(maxPointsOnLine(pts) == 4);
    }

    // Mixed: one diagonal with 3 points, plus one off-line
    {
        std::vector<Point> pts = {Point(0,0), Point(1,1), Point(2,2), Point(5,7)};
        assert(maxPointsOnLine(pts) == 3);
    }

    // Duplicate points plus extras
    {
        std::vector<Point> pts = {Point(1,1), Point(1,1), Point(2,2), Point(3,3)};
        // Three points on line y=x (including duplicates of (1,1)) plus (2,2) and (3,3) are also on it -> all 5 on same line
        assert(maxPointsOnLine(pts) == 5);
    }

    // Duplicate points on a vertical line with a point elsewhere
    {
        std::vector<Point> pts = {Point(2,0), Point(2,0), Point(2,0), Point(2,5), Point(0,0)};
        // Four points on x=2 (three copies of (2,0) plus (2,5)) vs diagonal? Actually (2,0),(2,0),(2,0),(2,5) -> 4 points, plus (0,0) is off that line. So answer 4.
        assert(maxPointsOnLine(pts) == 4);
    }

    // Large random-like check with known result
    {
        std::vector<Point> pts = {Point(0,0), Point(1,1), Point(2,2), Point(3,3), Point(0,1), Point(0,2), Point(0,3)};
        // Horizontal line y=0 has only 1 point (0,0). Diagonal has 4 points (0,0),(1,1),(2,2),(3,3). Vertical x=0 has 4 points (0,0),(0,1),(0,2),(0,3) but (0,0) is common? Actually each line counts all points on it: x=0 contains (0,0),(0,1),(0,2),(0,3) = 4 points. So max is 4.
        assert(maxPointsOnLine(pts) == 4);
    }

    return 0;
}
#include <vector>
#include <map>
#include <numeric>
#include <utility>

struct Point {
    int x;
    int y;
    Point() : x(0), y(0) {}
    Point(int a, int b) : x(a), y(b) {}
};

// Returns the maximum number of points that lie on the same straight line.
int maxPointsOnLine(const std::vector<Point>& points) {
    if (points.empty()) return 0;
    if (points.size() == 1) return 1;

    int globalMax = 0;

    for (size_t i = 0; i < points.size(); ++i) {
        // Map from normalized slope (dx,dy) to count of points with that slope.
        std::map<std::pair<int,int>, int> slopeCount;
        int duplicates = 1; // counts the anchor point itself
        int localMax = 0;

        for (size_t j = 0; j < points.size(); ++j) {
            if (i == j) continue;

            int dx = points[j].x - points[i].x;
            int dy = points[j].y - points[i].y;

            if (dx == 0 && dy == 0) {
                ++duplicates; // duplicate point, counted on every line
                continue;
            }

            // Normalize the slope: reduce by gcd and make dx positive.
            int g = std::gcd(dx, dy);
            dx /= g;
            dy /= g;

            // If dx is negative, flip signs to keep canonical form.
            if (dx < 0) {
                dx = -dx;
                dy = -dy;
            } else if (dx == 0) {
                // Vertical line: enforce dy positive for uniqueness.
                if (dy < 0) dy = -dy;
            }

            std::pair<int,int> slope = {dx, dy};
            int& count = slopeCount[slope];
            ++count;
            if (count > localMax) localMax = count;
        }

        // For this anchor, the max lines count is duplicates + max slope count.
        int candidate = duplicates + localMax;
        if (candidate > globalMax) globalMax = candidate;
    }

    return globalMax;
}
// The core idea is to fix each point as an "anchor" and count how many other points lie on the same line through that anchor. For a given anchor point `i`, each other point `j` (with coordinates different from the anchor) defines a unique slope. Points with the same slope relative to the anchor are collinear with the anchor. However, duplicate points (identical coordinates to the anchor) must be counted separately because they lie on every line through the anchor—they do not define a unique slope but are always collinear. The algorithm: initialize the maximum count to 0, then for each anchor point `i`: (1) reset a slope counter; (2) initialize `duplicates = 1` (counting the anchor itself); (3) iterate over all other points `j`: if `j` is identical to the anchor, increment `duplicates`; otherwise, compute the normalized slope `(dy/dx)` reduced to a canonical form (e.g., use `std::pair<int,int>` where the pair is reduced by dividing by gcd, and dx is always positive—if dx is negative, flip both signs; if dx is zero, represent as `(1,0)` for vertical). Increment the count for that slope; (4) after processing all points, the candidate maximum for this anchor is `duplicates + max_count of any slope`, and update the global maximum accordingly. Edge cases: empty vector returns 0; vector with one point returns 1; vector with two points returns 2 (even if they are duplicates); if all points are identical, the answer is the size of the vector. Using normalized rational slopes avoids floating-point round-off errors. Time complexity is \(O(n^2)\) because for each anchor we process all other points, with each slope insertion being \(O(\log n)\) in a `std::map` (or \(O(1)\) average with `std::unordered_map`, but `std::map` is simpler for pair keys). Space complexity is \(O(n)\) for the slope map per anchor.
