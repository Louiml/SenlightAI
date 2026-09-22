// Write a C++ function that takes an integer order parameter `n` (0 or greater) and the coordinates of a starting point `x, y`, as well as a segment length `l` and an initial angle `angle` in degrees, and returns a `std::vector<std::pair<int,int>>` containing the integer pixel coordinates (rounded to nearest integer) of the vertices of a Koch curve of that order. The curve is generated recursively: at order 0, it is a single straight segment from `(x, y)` to the endpoint computed by moving distance `l` in the given angle. For higher orders, each segment is replaced by four sub-segments of length `l/3` with the middle two forming a triangular bump: the first sub-segment goes at the original angle, the second at `angle - 60` degrees, the third at `angle + 60` degrees, and the fourth back at the original angle. The function must not draw anything; it only returns the list of vertices in the order they appear along the curve. For `n = 0`, the function returns exactly two vertices. All angle calculations must be in degrees, and you must use the standard library’s `std::cos` and `std::sin` with conversion to radians. The returned vertices must be computed without any external graphics library.

#include <cassert>
#include <vector>
#include <utility>
#include <cmath>

// Include the solution function here (assumed to be above).
// For brevity, we assume it is defined before the tests.

int main() {
    // Order 0: straight line from (0,0) to (100,0) (angle 0).
    auto v0 = koch_curve_vertices(0, 0.0, 0.0, 100.0, 0.0);
    assert(v0.size() == 2);
    assert(v0[0] == std::make_pair(0, 0));
    assert(v0[1] == std::make_pair(100, 0));

    // Order 0 with angle 90: vertical line.
    auto v90 = koch_curve_vertices(0, 0.0, 0.0, 100.0, 90.0);
    assert(v90.size() == 2);
    assert(v90[0] == std::make_pair(0, 0));
    assert(v90[1] == std::make_pair(0, 100));

    // Order 1: total vertices should be 5.
    auto v1 = koch_curve_vertices(1, 0.0, 0.0, 100.0, 0.0);
    assert(v1.size() == 5);
    // Start point.
    assert(v1[0] == std::make_pair(0, 0));
    // First vertex after 33.33 length → (33, 0) rounded.
    assert(v1[1] == std::make_pair(33, 0));
    // Second vertex: angle -60, length 33.33 from (33.33,0) → x = 33.33 + 33.33*cos(-60)=33.33+16.66=50, y=0+33.33*sin(-60)≈ -28.87 → round to (50, -29).
    assert(v1[2] == std::make_pair(50, -29));
    // Third vertex: angle +60 from (50, -28.87) → x=50+16.66=66.66→67, y=-28.87+28.87=0 → (67,0).
    assert(v1[3] == std::make_pair(67, 0));
    // End point: (100,0).
    assert(v1[4] == std::make_pair(100, 0));

    // Order 2: vertex count = 4^2 + 1 = 17.
    auto v2 = koch_curve_vertices(2, 0.0, 0.0, 81.0, 0.0);
    assert(v2.size() == 17);

    // Order 3: vertex count = 4^3 + 1 = 65.
    auto v3 = koch_curve_vertices(3, 0.0, 0.0, 81.0, 0.0);
    assert(v3.size() == 65);

    // Negative coordinates and rounding.
    auto vneg = koch_curve_vertices(0, -5.5, 3.2, 10.0, 180.0);
    assert(vneg.size() == 2);
    assert(vneg[0] == std::make_pair(-6, 3)); // -5.5 rounded to -6, 3.2→3
    // End: move left 10 units → x = -5.5-10 = -15.5 → -16, y unchanged 3.2→3.
    assert(vneg[1] == std::make_pair(-16, 3));

    // Zero length: all points same.
    auto vzero = koch_curve_vertices(2, 0.0, 0.0, 0.0, 0.0);
    // For n=2, 17 points all at (0,0).
    assert(vzero.size() == 17);
    for (const auto& p : vzero) {
        assert(p == std::make_pair(0, 0));
    }

    // Large angle beyond 360 wraps correctly via cos/sin.
    auto vwrap = koch_curve_vertices(0, 0.0, 0.0, 10.0, 720.0);
    assert(vwrap.size() == 2);
    assert(vwrap[1] == std::make_pair(10, 0));

    return 0;
}

#include <vector>
#include <utility>
#include <cmath>
#include <cstddef>

// Return the vertices of a Koch curve of given order as integer pixel coordinates.
// n_order: recursion depth (0 = straight line). x, y: start point. length: segment length.
// angle: initial direction in degrees. Returns vector of points along the curve.
std::vector<std::pair<int, int>> koch_curve_vertices(
    int n_order,
    double x,
    double y,
    double length,
    double angle
) {
    const double pi = 3.14159265358979323846;
    const double angle_rad = angle * pi / 180.0;
    const double cos_a = std::cos(angle_rad);
    const double sin_a = std::sin(angle_rad);

    // Base case: order 0, return start and end point.
    if (n_order == 0) {
        double end_x = x + length * cos_a;
        double end_y = y + length * sin_a;
        return {{static_cast<int>(std::lround(x)), static_cast<int>(std::lround(y))},
                {static_cast<int>(std::lround(end_x)), static_cast<int>(std::lround(end_y))}};
    }

    // Recursive case: split segment into four sub-curves.
    double sub_length = length / 3.0;
    std::vector<std::pair<int, int>> vertices;

    // First sub-curve: same angle.
    auto seg1 = koch_curve_vertices(n_order - 1, x, y, sub_length, angle);
    // The first vertex is already in seg1; we add all but the first (which duplicates start).
    vertices.insert(vertices.end(), seg1.begin(), seg1.end() - 1);

    // Update current position to the end of the first sub-curve.
    double x1 = x + sub_length * cos_a;
    double y1 = y + sub_length * sin_a;

    // Second sub-curve: angle - 60.
    auto seg2 = koch_curve_vertices(n_order - 1, x1, y1, sub_length, angle - 60.0);
    // Add all but the first point (which is the current position already added).
    vertices.insert(vertices.end(), seg2.begin() + 1, seg2.end() - 1);

    // Update current position to the end of the second sub-curve.
    double x2 = x1 + sub_length * std::cos((angle - 60.0) * pi / 180.0);
    double y2 = y1 + sub_length * std::sin((angle - 60.0) * pi / 180.0);

    // Third sub-curve: angle + 60.
    auto seg3 = koch_curve_vertices(n_order - 1, x2, y2, sub_length, angle + 60.0);
    vertices.insert(vertices.end(), seg3.begin() + 1, seg3.end() - 1);

    // Update current position to the end of the third sub-curve.
    double x3 = x2 + sub_length * std::cos((angle + 60.0) * pi / 180.0);
    double y3 = y2 + sub_length * std::sin((angle + 60.0) * pi / 180.0);

    // Fourth sub-curve: back to original angle.
    auto seg4 = koch_curve_vertices(n_order - 1, x3, y3, sub_length, angle);
    // Add all but the first point (which is the current position already added).
    vertices.insert(vertices.end(), seg4.begin() + 1, seg4.end());

    return vertices;
}

// The solution is a direct recursive generation of the Koch curve. The base case (`n == 0`) computes the endpoint of the segment by moving from `(x, y)` in direction `angle` (converted to radians) for distance `l`, rounds both coordinates to nearest integer (using `std::lround` or `static_cast<int>(value + 0.5)` for non-negative values, but to be safe with negative values use `std::lround`), and returns that pair. For `n > 0`, we reduce `l` to `l/3` and recursively generate the four sub-curves, carefully updating the current position after each recursive call by adding the appropriate `l` in that direction. The angles used are `angle`, `angle - 60`, `angle + 60`, and back to `angle`. The key edge case is handling negative coordinates and rounding to nearest integer correctly (use `std::lround`). Another edge case is `n = 0` where the function must return exactly two points: the start and the end. Also, `l` must be non-negative; if `l` is zero, all points collapse to the same coordinate. Time complexity: For order `n`, the number of segments is \(4^n\), so the recursion runs in \(O(4^n)\) time, and the space complexity is also \(O(4^n)\) for storing the vertices in the vector (excluding recursion stack depth of \(O(n)\)). For typical small `n` (0–6) this is feasible.
