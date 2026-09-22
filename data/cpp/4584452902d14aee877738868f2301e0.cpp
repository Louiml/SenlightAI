Write a C++ function `collectPath` that takes an integer `n` representing the number of points and a vector of pairs `(x, y)` representing 2D coordinates, and returns a string describing a monotone path from `(0, 0)` through all given points in non-decreasing `x` and `y` order, using `'R'` for right (increase `x`) and `'U'` for up (increase `y`). The input points are guaranteed to be distinct but not sorted. The function must return either the complete path string (e.g., `"RRUUR"`) if such a path exists, or the string `"NO"` if no valid monotone path can visit all points in non-decreasing `x` and `y` order. For example, given points `(1,0)`, `(2,1)`, the valid path is `"RRU"`; given points `(1,1)`, `(0,0)`, it is impossible because the second point has smaller `x` than the first, so return `"NO"`. The path must start at `(0,0)` and end at the last point after all movements.
// The key is to sort the points by `x` ascending, and for equal `x`, by `y` ascending, so that any valid monotone path must visit them in this order. Then simulate moving from the current position (initially `(0,0)`) to each next point. At each step, compute the needed `dx` and `dy`. If either `dx` or `dy` is negative (meaning the point is strictly to the left or below the current position), then it is impossible, and return `"NO"`. Otherwise, append `dx` copies of `'R'` and `dy` copies of `'U'` to the answer. Since duplicates cannot exist and sorting ensures non-decreasing `x`, the only failure case is when `y` decreases between consecutive sorted points. Edge cases include a single point (path is just `'R'` and/or `'U'` movements from origin) and points already on the origin (no movement needed). The complexity is dominated by sorting: `O(n log n)` time and `O(n)` space for the sorted vector and result string.
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

// Precondition: points are distinct, not necessarily sorted.
// Returns "NO" if no monotone path exists; otherwise returns the path string.
std::string collectPath(int n, const std::vector<std::pair<int, int>>& points) {
    // Sort by x ascending, then y ascending to define the required visiting order.
    std::vector<std::pair<int, int>> ordered = points;
    std::sort(ordered.begin(), ordered.end());

    std::string result;
    int curX = 0;
    int curY = 0;

    for (const auto& p : ordered) {
        int nextX = p.first;
        int nextY = p.second;
        int dx = nextX - curX;
        int dy = nextY - curY;

        // If either delta is negative, we cannot go left/down, so impossible.
        if (dx < 0 || dy < 0) {
            return "NO";
        }

        result.append(dx, 'R');
        result.append(dy, 'U');

        curX = nextX;
        curY = nextY;
    }

    return result;
}
#include <cassert>
#include <vector>
#include <utility>
#include <string>

// The solution function is defined above; test by including it or reading from the same file.

int main() {
    // Basic valid path
    std::vector<std::pair<int, int>> p1 = {{1, 0}, {2, 1}};
    assert(collectPath(2, p1) == "RRU");

    // Already sorted and simple
    std::vector<std::pair<int, int>> p2 = {{1, 1}, {3, 2}};
    assert(collectPath(2, p2) == "RURU");

    // Impossible because y decreases after sorting
    std::vector<std::pair<int, int>> p3 = {{1, 2}, {2, 1}};
    assert(collectPath(2, p3) == "NO");

    // Single point
    std::vector<std::pair<int, int>> p4 = {{0, 2}};
    assert(collectPath(1, p4) == "UU");

    // Point at origin requires no movement
    std::vector<std::pair<int, int>> p5 = {{0, 0}};
    assert(collectPath(1, p5) == "");

    // Unsorted input that is still valid after sorting
    std::vector<std::pair<int, int>> p6 = {{2, 1}, {0, 0}, {1, 0}};
    assert(collectPath(3, p6) == "RU");

    // Duplicate x but increasing y
    std::vector<std::pair<int, int>> p7 = {{1, 1}, {1, 3}};
    assert(collectPath(2, p7) == "RUU");

    // Large delta
    std::vector<std::pair<int, int>> p8 = {{3, 4}};
    assert(collectPath(1, p8) == "RRRUUUU");

    // Impossible: point with negative coordinates cannot be reached
    std::vector<std::pair<int, int>> p9 = {{-1, 0}};
    assert(collectPath(1, p9) == "NO");

    // Multiple points with complications
    std::vector<std::pair<int, int>> p10 = {{1, 0}, {1, 2}, {2, 3}};
    assert(collectPath(3, p10) == "RUURU");

    return 0;
}
