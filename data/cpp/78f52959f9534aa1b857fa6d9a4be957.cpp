/*
Write a C++ function that reads a single floating-point value representing the radius of a sphere, and returns the volume of that sphere. Use the formula \( V = \frac{4}{3} \pi r^3 \), with \( \pi = 3.14159 \) as a constant. The function should accept a `double` radius (not read from `std::cin`—the calling code will pass the value) and return a `double` volume. Assume the radius is non-negative. Handle the case where the radius is zero (volume is zero) and where the radius is very large (e.g., \(10^6\)) without overflow—use a `double` for computation. Do not use any external libraries beyond standard headers.
*/

#include <cmath>   // for pow (or use r*r*r directly)

// Compute the volume of a sphere given its radius.
// Precondition: radius >= 0.0 (non-negative radius).
// Returns volume = (4/3) * pi * r^3, using pi = 3.14159.
double sphereVolume(double radius) {
    const double PI = 3.14159;
    const double FOUR_THIRDS = 4.0 / 3.0;

    // Use radius * radius * radius to avoid pow overhead and potential precision loss.
    double radiusCubed = radius * radius * radius;

    return FOUR_THIRDS * PI * radiusCubed;
}

#include <cassert>
#include <cmath>

// Declare the solution function (normally would be included from header)
double sphereVolume(double radius);

int main() {
    // Exact comparison for simple values
    assert(sphereVolume(0.0) == 0.0);

    // For radius 1.0, volume = 4/3 * pi = 4.18878666667...
    double expected_r1 = (4.0 / 3.0) * 3.14159 * 1.0;
    assert(std::abs(sphereVolume(1.0) - expected_r1) < 1e-9);

    // Radius 2.0: volume = 4/3 * pi * 8 = 33.5102933333...
    double expected_r2 = (4.0 / 3.0) * 3.14159 * 8.0;
    assert(std::abs(sphereVolume(2.0) - expected_r2) < 1e-9);

    // Radius 3.0: volume = 4/3 * pi * 27 = 113.097333333...
    double expected_r3 = (4.0 / 3.0) * 3.14159 * 27.0;
    assert(std::abs(sphereVolume(3.0) - expected_r3) < 1e-9);

    // Radius 0.5: volume = 4/3 * pi * 0.125 = 0.5235983333...
    double expected_r05 = (4.0 / 3.0) * 3.14159 * 0.125;
    assert(std::abs(sphereVolume(0.5) - expected_r05) < 1e-9);

    // Large radius: 1000.0, volume = 4/3 * pi * 1e9 = 4.18879e9 approx
    double expected_r1000 = (4.0 / 3.0) * 3.14159 * 1e9;
    assert(std::abs(sphereVolume(1000.0) - expected_r1000) < 1e-3);

    // Very large radius: 1e6, volume = 4/3 * pi * 1e18 = 4.18879e18
    double expected_r1e6 = (4.0 / 3.0) * 3.14159 * 1e18;
    assert(std::abs(sphereVolume(1e6) - expected_r1e6) < 1e6);  // tolerance

    return 0;
}

// The task is straightforward: compute the volume using the given formula. The main algorithm involves reading a radius value (passed as a parameter), computing \(r^3\) (either via `r*r*r` or `std::pow(r, 3)`—but `r*r*r` is preferable to avoid potential floating-point inaccuracies and overhead), multiplying by the constant \(4.0/3.0\) and \( \pi \). Important edge cases: a radius of zero returns zero; very large radii should not cause overflow because `double` supports up to ~\(1.8 \times 10^{308}\), so \(10^6\) cubed is \(10^{18}\), well within range. No special handling needed for negative radii since we assume non-negative, but if a negative is passed, `pow` with integer exponent would produce a negative result, which is mathematically invalid for a volume; we could optionally return 0 for negative inputs, but the specification says non-negative. Time complexity is \(O(1)\) and space complexity is \(O(1)\). Use `const double PI = 3.14159;` and `const double FOUR_THIRDS = 4.0 / 3.0;` for clarity.
