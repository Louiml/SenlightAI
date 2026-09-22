// Write a C++ function named `areaOfOverlappingCircle` that takes three `double` parameters: the radius of a circle, the distance between the centers of two identical circles, and returns the area of the region where the two circles overlap. The circles have equal radius and their centers are separated by the given distance. If the distance is zero or negative, treat it as zero (fully coincident circles), in which case the overlap area equals the area of one full circle. If the distance is greater than or equal to the sum of the two radii (i.e., `distance >= 2 * radius`), the circles do not overlap, so return `0.0`. Otherwise, the overlap region consists of two identical circular segments; compute the area using the formula:  
// `area = 2 * radius^2 * acos(distance / (2 * radius)) - (distance / 2) * sqrt(4 * radius^2 - distance^2)`.  
// Use `constexpr double PI = 3.14159265358979323846;` and round the result to two decimal places (e.g., using `std::round` and multiplying by 100) before returning. Ensure the function handles invalid input (non-positive radius) by returning `0.0`.

// The algorithm first validates the inputs: if `radius <= 0` return `0.0`. Then normalize `distance` to be non-negative (use `std::fabs`). If `distance >= 2 * radius`, return `0.0` (no overlap). If `distance == 0`, the circles coincide, and the overlap area is `PI * radius * radius`. Otherwise, use the lens formula for two equal circles: the overlap area is the sum of two circular segments. Each segment’s area is `radius^2 * acos(d/(2r)) - (d/2) * sqrt(4r^2 - d^2)`, so multiply by 2. Edge cases: when `distance` is extremely close to `2r`, the `acos` argument approaches -1, and the sqrt term approaches 0, but to avoid floating-point issues (e.g., argument slightly outside [-1,1]), clamp the argument to `[-1,1]` before `acos`. Also clamp inside the square root to avoid negative values due to rounding. Finally round the result to two decimal places using `std::round(value * 100.0) / 100.0`. Time complexity is O(1), space O(1). The solution uses `std::fabs`, `std::acos`, `std::sqrt`, `std::round` from `<cmath>`.

#include <cmath>

// Returns the overlap area of two identical circles given radius and center distance.
// Returns 0.0 for invalid radius or when circles don't overlap.
double areaOfOverlappingCircle(double radius, double distance) {
    constexpr double PI = 3.14159265358979323846;
    if (radius <= 0.0) return 0.0;
    distance = std::fabs(distance);
    if (distance >= 2.0 * radius) return 0.0;
    if (distance == 0.0) {
        return std::round(PI * radius * radius * 100.0) / 100.0;
    }
    // Clamp argument for acos to avoid domain errors due to floating point.
    double ratio = distance / (2.0 * radius);
    if (ratio > 1.0) ratio = 1.0;
    if (ratio < -1.0) ratio = -1.0;
    double term1 = 2.0 * radius * radius * std::acos(ratio);
    double underSqrt = 4.0 * radius * radius - distance * distance;
    if (underSqrt < 0.0) underSqrt = 0.0;
    double term2 = distance * std::sqrt(underSqrt) / 2.0;
    double rawArea = term1 - term2;
    return std::round(rawArea * 100.0) / 100.0;
}

#include <cassert>
#include <cmath>

int main() {
    // Full overlap (distance = 0) => area of one circle with radius 1
    assert(areaOfOverlappingCircle(1.0, 0.0) == 3.14);
    // Non-overlapping (distance >= 2*radius)
    assert(areaOfOverlappingCircle(1.0, 2.0) == 0.0);
    assert(areaOfOverlappingCircle(1.0, 3.0) == 0.0);
    // Invalid radius
    assert(areaOfOverlappingCircle(0.0, 0.5) == 0.0);
    assert(areaOfOverlappingCircle(-2.0, 1.0) == 0.0);
    // Negative distance treated as zero
    assert(areaOfOverlappingCircle(1.0, -0.0) == 3.14);
    // Known example: r=1, d=1 -> overlap area = 2.0*acos(0.5) - 0.5*sqrt(3) ≈ 2.0944 - 0.8660 = 1.2284 -> rounds to 1.23
    assert(areaOfOverlappingCircle(1.0, 1.0) == 1.23);
    // r=2, d=2 -> overlap = 2*4*acos(0.5) - 1*sqrt(12) = 8*1.0472 - 3.4641 = 8.3776 - 3.4641 = 4.9135 -> rounds to 4.91
    assert(areaOfOverlappingCircle(2.0, 2.0) == 4.91);
    // Just barely overlapping: r=5, d=9.999 -> lens very small but >0, compute manually? Instead just verify result is >0 and < full circle area.
    // Use a known precise value: r=3, d=4 -> area = 2*9*acos(2/3) - 2*sqrt(20) ≈ 18*0.84107 - 8.94427 = 15.1393 - 8.94427 = 6.1950 -> rounds to 6.19
    assert(areaOfOverlappingCircle(3.0, 4.0) == 6.19);
    // Test clamping: distance exactly 2*r but with floating error
    assert(areaOfOverlappingCircle(1.0, 2.0000000001) == 0.0);
}
