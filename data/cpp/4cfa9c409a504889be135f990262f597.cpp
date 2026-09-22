Given the `main.cpp` snippet above, write a standalone C++ function named `computeRacingLine` that, given a vector of 2D points representing waypoints of a racing track (as `std::pair<double, double>`), computes the total length of the racing line by summing the Euclidean distances between consecutive points. The function must handle an empty or single-point input by returning `0.0`. Use only standard libraries (no OpenGL or external dependencies), and ensure the function is `const`-correct (i.e., takes a const reference to the vector). The function should be robust to floating-point precision, e.g., return exact `0.0` for degenerate cases. The output must be a `double` representing the total path length.
// The solution is straightforward: iterate through the vector of points, and for each consecutive pair `(p[i], p[i+1])`, compute the Euclidean distance using `std::hypot(dx, dy)` (or `sqrt(dx*dx + dy*dy)`), and accumulate these distances into a running total. Edge cases: if the vector is empty or contains only one point, there are no pairs, so the total is `0.0`. For performance, the algorithm runs in O(n) time, where n is the number of points, using O(1) auxiliary space (only a few local variables). Floating-point precision: using `std::hypot` is safer for large coordinate values because it avoids overflow/underflow, but for typical values `sqrt` is fine. The function must be `const`-correct by accepting `const std::vector<std::pair<double, double>>&` to avoid copying.
#include <vector>
#include <utility>
#include <cmath>

// Compute the total length of a polyline defined by waypoints.
// Returns 0.0 for empty or single-point input.
double computeRacingLine(const std::vector<std::pair<double, double>>& waypoints) {
    double totalLength = 0.0;
    for (size_t i = 1; i < waypoints.size(); ++i) {
        double dx = waypoints[i].first - waypoints[i - 1].first;
        double dy = waypoints[i].second - waypoints[i - 1].second;
        totalLength += std::hypot(dx, dy);
    }
    return totalLength;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// Assume computeRacingLine is defined above (or include the solution file).

int main() {
    // Test empty input
    std::vector<std::pair<double, double>> empty;
    assert(computeRacingLine(empty) == 0.0);

    // Test single point
    std::vector<std::pair<double, double>> single = {{1.0, 2.0}};
    assert(computeRacingLine(single) == 0.0);

    // Test two points: (0,0) to (3,4) -> distance 5
    std::vector<std::pair<double, double>> two = {{0.0, 0.0}, {3.0, 4.0}};
    assert(std::fabs(computeRacingLine(two) - 5.0) < 1e-9);

    // Test three points: (0,0)->(0,1)->(0,2) -> length 2.0
    std::vector<std::pair<double, double>> three = {{0.0, 0.0}, {0.0, 1.0}, {0.0, 2.0}};
    assert(std::fabs(computeRacingLine(three) - 2.0) < 1e-9);

    // Test negative coordinates and non-axis aligned: (-1,-1)->(1,1)->(2,0)
    // Distance1 = sqrt(8) ≈ 2.828427, Distance2 = sqrt(2) ≈ 1.414214, total ≈ 4.24264
    std::vector<std::pair<double, double>> mixed = {{-1.0, -1.0}, {1.0, 1.0}, {2.0, 0.0}};
    double expected = std::sqrt(8.0) + std::sqrt(2.0);
    assert(std::fabs(computeRacingLine(mixed) - expected) < 1e-9);

    // Test repeated points (zero-length segments)
    std::vector<std::pair<double, double>> repeated = {{5.0, 5.0}, {5.0, 5.0}, {5.0, 5.0}};
    assert(computeRacingLine(repeated) == 0.0);

    // Test large values to verify hypot stability
    std::vector<std::pair<double, double>> large = {{1e200, 0.0}, {0.0, 1e200}};
    double largeExpected = std::sqrt(2.0) * 1e200;
    assert(std::fabs(computeRacingLine(large) - largeExpected) < 1e190); // relative tolerance

    return 0;
}
