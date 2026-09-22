Write a C++ function `computeMotion(double length, double force)` that takes a positive length \(L\) (in meters) and a positive force \(F\) (in newtons), and returns a `std::pair<double, double>` containing the final velocity \(v = \sqrt{2 \cdot F \cdot L}\) (in meters per second) and the number of round trips per hour \(n = \frac{v \cdot 3600}{2 \cdot L}\) (dimensionless). The function must handle edge cases where either input is zero by returning `{0.0, 0.0}` to avoid division by zero, and it must be `const`-correct and use `std::sqrt` from `<cmath>`. The function should be self-contained and not read from standard input.

#include <cassert>
#include <cmath>

int main() {
    // Standard positive case: L=2, F=8 => v=sqrt(32)=5.656854..., trips= (5.656854*3600)/(4)=5091.1688...
    auto result1 = computeMotion(2.0, 8.0);
    assert(std::abs(result1.first - 5.656854249492380) < 1e-9);
    assert(std::abs(result1.second - 5091.168824543142) < 1e-6);

    // Zero force case: returns {0,0}
    auto result2 = computeMotion(5.0, 0.0);
    assert(result2.first == 0.0 && result2.second == 0.0);

    // Zero length case: returns {0,0}
    auto result3 = computeMotion(0.0, 100.0);
    assert(result3.first == 0.0 && result3.second == 0.0);

    // Both zero
    auto result4 = computeMotion(0.0, 0.0);
    assert(result4.first == 0.0 && result4.second == 0.0);

    // Small values: L=1, F=1 => v=sqrt(2)=1.41421356..., trips= (1.4142*3600)/2=2545.5844...
    auto result5 = computeMotion(1.0, 1.0);
    assert(std::abs(result5.first - 1.4142135623730951) < 1e-9);
    assert(std::abs(result5.second - 2545.584412271571) < 1e-6);

    // Large values: L=100, F=50 => v=sqrt(10000)=100, trips= (100*3600)/200=1800
    auto result6 = computeMotion(100.0, 50.0);
    assert(std::abs(result6.first - 100.0) < 1e-9);
    assert(std::abs(result6.second - 1800.0) < 1e-9);

    // Negative forces (should be treated mathematically; result may be NaN in sqrt)
    // We don't test this case because the problem specifies positive forces.
    // Edge: Very small length and force to check no premature zero
    auto result7 = computeMotion(1e-9, 1e-9);
    // v = sqrt(2e-18) = 1.41421356e-9, trips = (v*3600)/(2e-9) = 2545.5844...
    assert(std::abs(result7.first - 1.4142135623730951e-9) < 1e-18);
    assert(std::abs(result7.second - 2545.584412271571) < 1e-6);

    return 0;
}

#include <cmath>
#include <utility>

// Compute final velocity (m/s) and number of round trips per hour.
// Returns {0.0, 0.0} if either input is zero to avoid division by zero.
std::pair<double, double> computeMotion(double length, double force) {
    if (length == 0.0 || force == 0.0) {
        return {0.0, 0.0};
    }
    const double velocity = std::sqrt(2.0 * force * length);
    const double tripsPerHour = (velocity * 3600.0) / (2.0 * length);
    return {velocity, tripsPerHour};
}

// The solution is straightforward: compute the velocity directly from the formula \(v = \sqrt{2FL}\). Since \(L\) and \(F\) are given as positive values (or zero as a sentinel), we check if either is zero and return `{0.0, 0.0}` to avoid division by zero. Otherwise, compute `v` using `std::sqrt(2.0 * force * length)`, then compute the round-trip count per hour using `n = (v * 3600.0) / (2.0 * length)`. The only tricky case is when `length == 0`, where division would be undefined; the zero-check handles it. Time complexity is \(O(1)\) and space complexity is \(O(1)\), with double precision floating-point arithmetic. The returned pair contains the velocity first and the round-trips per hour second, matching the original snippet’s output order.
