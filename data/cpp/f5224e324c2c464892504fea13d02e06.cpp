// Write a C++ function `double pathLength(const std::vector<std::pair<double,double>>& points)` that takes a vector of 2D points (x, y) in order and returns the total Euclidean length of the polyline path connecting them sequentially. The input may contain any number of points (including zero or one, which should return 0.0), coordinates may be negative, and the function must use `const` references where appropriate, handle large coordinate values without overflow, and be robust to duplicate consecutive points (contributing zero distance).

#include <cassert>
#include <cmath>
#include <vector>

int main() {
    // empty path
    std::vector<std::pair<double,double>> empty;
    assert(pathLength(empty) == 0.0);

    // single point
    std::vector<std::pair<double,double>> one = {{3.0, 4.0}};
    assert(pathLength(one) == 0.0);

    // straight line horizontal
    std::vector<std::pair<double,double>> line = {{0.0, 0.0}, {5.0, 0.0}};
    assert(std::abs(pathLength(line) - 5.0) < 1e-9);

    // 3-4-5 triangle
    std::vector<std::pair<double,double>> triangle = {{0.0, 0.0}, {3.0, 0.0}, {3.0, 4.0}};
    assert(std::abs(pathLength(triangle) - 8.0) < 1e-9);

    // negative coordinates
    std::vector<std::pair<double,double>> neg = {{-1.0, -1.0}, {2.0, 3.0}};
    assert(std::abs(pathLength(neg) - 5.0) < 1e-9);

    // duplicate consecutive points add zero
    std::vector<std::pair<double,double>> dup = {{0.0, 0.0}, {0.0, 0.0}, {4.0, 0.0}};
    assert(std::abs(pathLength(dup) - 4.0) < 1e-9);

    // multiple segments
    std::vector<std::pair<double,double>> multi = {{0.0, 0.0}, {1.0, 1.0}, {1.0, 3.0}, {4.0, 3.0}};
    double expected = std::sqrt(2.0) + 2.0 + 3.0; // ≈ 6.414
    assert(std::abs(pathLength(multi) - expected) < 1e-9);

    return 0;
}

#include <vector>
#include <cmath>
#include <utility>

// Compute total Euclidean path length through a sequence of 2D points.
// Returns 0.0 for zero or one point.
double pathLength(const std::vector<std::pair<double, double>>& points) {
    double total = 0.0;
    for (std::size_t i = 1; i < points.size(); ++i) {
        double dx = points[i].first - points[i-1].first;
        double dy = points[i].second - points[i-1].second;
        total += std::hypot(dx, dy);
    }
    return total;
}

// The solution iterates over consecutive pairs of points, computing the Euclidean distance via `std::hypot(dx, dy)` which avoids overflow/underflow for very large or very small coordinate differences. For each index `i` from 1 to `n-1`, we accumulate the distance between `points[i-1]` and `points[i]`. Edge cases: empty or single-point input returns 0.0; duplicate consecutive points add zero distance. Time complexity is O(n) for n points, space complexity is O(1) beyond the input container.
