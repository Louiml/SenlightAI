// Write a C++ function that generates all integer grid points lying on a line segment between two endpoints `(x1, y1)` and `(x2, y2)` using Bresenham’s line algorithm (integer arithmetic only, no floating-point or rounding). The function must return a `std::vector<std::pair<int, int>>` containing the points in the order they are plotted, starting from `(x1, y1)` and ending at `(x2, y2)`. The coordinates may be any integers (including negative values and zero), and the endpoints can be identical (in which case the vector contains exactly one point). Your implementation should handle all line slopes, including perfectly horizontal, vertical, and diagonal lines, and must avoid division by zero. Define the free function with signature `std::vector<std::pair<int, int>> bresenhamLine(int x1, int y1, int x2, int y2)` and ensure it is `const`-correct (parameters are passed by value, but the function must not modify them internally beyond local copies). No `main` function is required; only the function definition with necessary `#include` directives.
// The solution implements the classic Bresenham line algorithm that uses only integer arithmetic. First, compute the absolute differences `dx = abs(x2 - x1)` and `dy = abs(y2 - y1)`, and the step signs `incx = (x2 >= x1) ? 1 : -1` and `incy = (y2 >= y1) ? 1 : -1` (handles negative coordinate differences correctly). Then decide whether the line is shallow (`dx > dy`) or steep (`dy >= dx`). For a shallow line, the loop iterates over `dx` steps, starting from `(x1, y1)`, maintaining an error term `e = 2*dy - dx`. At each step, if `e >= 0`, we increment `y` by `incy` and add `2*(dy - dx)` to `e`; otherwise, we add `2*dy` to `e`. Always increment `x` by `incx` and push the current pixel. For a steep line, the roles of `x` and `y` are swapped: loop over `dy` steps, adjust `x` when `e >= 0`, and always increment `y`. If both endpoints are identical, the algorithm naturally returns a single point because the loop count (`dx` or `dy`) is zero; the initial point is still pushed before the loop. Edge cases: horizontal (`dy=0`) and vertical (`dx=0`) lines are handled by the symmetry of the algorithm, and diagonal lines (`dx == dy`) work fine. No division by zero occurs because we never divide; we only multiply and compare. The algorithm runs in `O(max(dx, dy) + 1)` time, which is exactly the number of points generated (proportional to the line length), and uses `O(1)` additional space excluding the output vector.
#include <vector>
#include <utility>
#include <cstdlib> // for abs

// Generate all integer grid points on the line from (x1, y1) to (x2, y2)
// using Bresenham's integer-only line algorithm.
// Returns a vector of points in plotting order, including both endpoints.
std::vector<std::pair<int, int>> bresenhamLine(int x1, int y1, int x2, int y2) {
    std::vector<std::pair<int, int>> points;

    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);

    // Determine step direction for each axis.
    int incx = (x2 >= x1) ? 1 : -1;
    int incy = (y2 >= y1) ? 1 : -1;

    int x = x1;
    int y = y1;

    // Always include the starting point.
    points.push_back(std::make_pair(x, y));

    if (dx > dy) {
        // Shallow line: x increases faster.
        int e = 2 * dy - dx;
        int inc1 = 2 * (dy - dx);
        int inc2 = 2 * dy;

        for (int i = 0; i < dx; ++i) {
            if (e >= 0) {
                y += incy;
                e += inc1;
            } else {
                e += inc2;
            }
            x += incx;
            points.push_back(std::make_pair(x, y));
        }
    } else {
        // Steep line (dx <= dy, including vertical and diagonal).
        int e = 2 * dx - dy;
        int inc1 = 2 * (dx - dy);
        int inc2 = 2 * dx;

        for (int i = 0; i < dy; ++i) {
            if (e >= 0) {
                x += incx;
                e += inc1;
            } else {
                e += inc2;
            }
            y += incy;
            points.push_back(std::make_pair(x, y));
        }
    }

    return points;
}
#include <cassert>
#include <vector>
#include <utility>

// Assume bresenhamLine is defined above.

int main() {
    // Test 1: Simple horizontal line from (0,0) to (5,0)
    auto p1 = bresenhamLine(0, 0, 5, 0);
    assert(p1.size() == 6);
    assert(p1[0] == std::make_pair(0, 0));
    assert(p1[3] == std::make_pair(3, 0));
    assert(p1[5] == std::make_pair(5, 0));

    // Test 2: Vertical line with negative coordinates from (2,-1) to (2,-5)
    auto p2 = bresenhamLine(2, -1, 2, -5);
    assert(p2.size() == 5);
    assert(p2[0] == std::make_pair(2, -1));
    assert(p2[4] == std::make_pair(2, -5));
    // Ensure all x are 2 and y decreases by 1 each step.
    for (int i = 0; i < static_cast<int>(p2.size()); ++i) {
        assert(p2[i].first == 2);
        assert(p2[i].second == -1 - i);
    }

    // Test 3: Perfect diagonal from (0,0) to (4,4)
    auto p3 = bresenhamLine(0, 0, 4, 4);
    assert(p3.size() == 5);
    for (int i = 0; i < 5; ++i) {
        assert(p3[i] == std::make_pair(i, i));
    }

    // Test 4: Negative slope from (0,3) to (4,0) — should have all 5 points
    auto p4 = bresenhamLine(0, 3, 4, 0);
    assert(p4.size() == 5);
    assert(p4[0] == std::make_pair(0, 3));
    assert(p4[4] == std::make_pair(4, 0));
    // Check that y decreases monotonically and x increases monotonically.
    for (int i = 1; i < static_cast<int>(p4.size()); ++i) {
        assert(p4[i].first > p4[i-1].first);
        assert(p4[i].second < p4[i-1].second);
    }

    // Test 5: Steep line from (1,1) to (2,5) — dx=1, dy=4
    auto p5 = bresenhamLine(1, 1, 2, 5);
    assert(p5.size() == 5); // dy+1 = 5
    assert(p5[0] == std::make_pair(1, 1));
    assert(p5[4] == std::make_pair(2, 5));

    // Test 6: Same endpoint (single point)
    auto p6 = bresenhamLine(3, -2, 3, -2);
    assert(p6.size() == 1);
    assert(p6[0] == std::make_pair(3, -2));

    // Test 7: Reversed line — from (5,5) to (0,0)
    auto p7 = bresenhamLine(5, 5, 0, 0);
    assert(p7.size() == 6);
    assert(p7[0] == std::make_pair(5, 5));
    assert(p7[5] == std::make_pair(0, 0));

    // Test 8: Slightly sloped shallow line from (-2,0) to (3,1)
    auto p8 = bresenhamLine(-2, 0, 3, 1);
    assert(p8.size() == 6); // dx+1 = 6
    // Check first and last.
    assert(p8[0] == std::make_pair(-2, 0));
    assert(p8[5] == std::make_pair(3, 1));

    return 0;
}
