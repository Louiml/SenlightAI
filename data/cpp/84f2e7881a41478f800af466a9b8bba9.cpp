Write a C++ function named `computeConeGeometry` that takes a radius `r` and a height `h` (both as `double`), and returns a `std::pair<double, double>` containing the lateral surface area and volume of a right circular cone, in that order. The surface area formula is \( \pi r \left( r + \sqrt{h^2 + r^2} \right) \), and the volume formula is \( \frac{\pi r^2 h}{3} \). Use `M_PI` from `<cmath>` (ensure your compiler defines it, or define it locally) for accuracy. The function must handle zero and positive inputs gracefully; for zero radius or height, the corresponding area/volume should be correctly zero (no division by zero or negative square root). Negative inputs are invalid and should be handled by returning a pair of `NaN` values (use `std::numeric_limits<double>::quiet_NaN()`). Provide a standalone implementation without a `main` function.

#include <cassert>
#include <cmath>
#include <utility>

// Assume the solution function is declared here or included.

int main() {
    // Perfectly vertical cone with r=3, h=4 → slant=5, area=π*3*(3+5)=24π ≈75.3982, volume=π*9*4/3=12π≈37.6991
    auto res1 = computeConeGeometry(3.0, 4.0);
    assert(std::abs(res1.first - 24.0 * M_PI) < 1e-9);
    assert(std::abs(res1.second - 12.0 * M_PI) < 1e-9);

    // Zero radius → area=0, volume=0 (even with height)
    auto res2 = computeConeGeometry(0.0, 5.0);
    assert(res2.first == 0.0);
    assert(res2.second == 0.0);

    // Zero height → area = π*r*r (slant=r), volume=0
    auto res3 = computeConeGeometry(2.0, 0.0);
    assert(std::abs(res3.first - 4.0 * M_PI) < 1e-9);
    assert(res3.second == 0.0);

    // Both zero → both zero
    auto res4 = computeConeGeometry(0.0, 0.0);
    assert(res4.first == 0.0);
    assert(res4.second == 0.0);

    // Negative radius → NaN
    auto res5 = computeConeGeometry(-1.0, 2.0);
    assert(std::isnan(res5.first) && std::isnan(res5.second));

    // Negative height → NaN
    auto res6 = computeConeGeometry(1.0, -2.0);
    assert(std::isnan(res6.first) && std::isnan(res6.second));

    // Symmetric check: r=1, h=1 → slant=sqrt(2), area=π*1*(1+√2)≈7.5846, volume=π/3≈1.0472
    auto res7 = computeConeGeometry(1.0, 1.0);
    assert(std::abs(res7.first - M_PI * (1.0 + std::sqrt(2.0))) < 1e-9);
    assert(std::abs(res7.second - M_PI / 3.0) < 1e-9);
}

#include <cmath>
#include <utility>
#include <limits>

// Returns the lateral surface area and volume of a right circular cone.
// Inputs: r = radius, h = height. For negative inputs, returns {NaN, NaN}.
std::pair<double, double> computeConeGeometry(const double r, const double h) {
    // Handle invalid negative inputs
    if (r < 0.0 || h < 0.0) {
        const double nan_val = std::numeric_limits<double>::quiet_NaN();
        return {nan_val, nan_val};
    }

    // Calculate slant height and then area and volume using M_PI (may need definition)
    const double slant = std::sqrt(h * h + r * r);
    const double area = M_PI * r * (r + slant);
    const double volume = (M_PI * r * r * h) / 3.0;
    return {area, volume};
}

// The solution requires computing the lateral surface area (excluding the base) and the volume of a cone. The lateral surface area uses the slant height \( \sqrt{h^2 + r^2} \). Main algorithm: validate inputs. If either `r` or `h` is negative, return `{NaN, NaN}`. Otherwise, compute `slant = std::sqrt(h*h + r*r)` and `area = M_PI * r * (r + slant)`, `volume = (M_PI * r * r * h) / 3.0`. Edge cases: if `r == 0` or `h == 0`, the formulas yield zero (e.g., area = 0, volume = 0), which is fine. No division by zero occurs because division by 3 is safe. The square root argument is always non-negative for valid inputs. Time complexity is O(1), space complexity O(1). Use `const double` for parameters to avoid modification, and `const double` local variables for clarity.
