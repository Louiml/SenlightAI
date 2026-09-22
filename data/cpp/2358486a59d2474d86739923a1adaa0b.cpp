Write a C++ function named `sphereVolume` that takes an integer radius as input and returns the volume of a sphere with that radius using the formula \( V = \frac{4}{3} \pi r^3 \). The function must use `3.14` as the approximation for \(\pi\), accept a non-negative radius (including 0), and return the result as a `float`. The function should be self-contained and must not read from or write to any standard input/output streams; it merely computes and returns the volume.

// The solution directly applies the geometric formula. The main algorithm is straightforward: read the integer radius, compute the cube using `std::pow(radius, 3)` (or `radius * radius * radius`), multiply by `4 * 3.14`, and divide by 3. Since the radius is an integer, `std::pow` returns a `double`, but the result is assigned to `float`, which is safe given typical magnitudes. Edge cases: radius = 0 must yield 0.0 (which naturally occurs because the cube is 0). Negative radii are not physically meaningful; the problem implies non-negative input, but if a negative radius were passed, the formula would produce a negative volume, which is mathematically consistent but physically nonsensical—we choose not to validate and simply compute per the formula. Time complexity is \(O(1)\) because the calculation involves a constant number of operations. Space complexity is \(O(1)\) as no auxiliary data structures are used. The use of `std::pow` may introduce slight floating-point imprecision, but for typical integer radii (e.g., up to a few thousand), the error is negligible; alternatively, using integer multiplication `r*r*r` avoids `pow` overhead but still assigns to `float`.

#include <cmath>

// Compute the volume of a sphere with the given integer radius.
// Uses 3.14 as an approximation for pi. Returns the volume as a float.
float sphereVolume(int radius) {
    const float pi = 3.14f;
    const float cube = static_cast<float>(std::pow(radius, 3));
    return (4.0f * pi * cube) / 3.0f;
}

#include <cassert>
#include <cmath>

// Assume sphereVolume is declared above this main function.
int main() {
    // Radius 0 -> volume 0
    assert(std::fabs(sphereVolume(0) - 0.0f) < 1e-5);
    // Radius 1 -> (4*3.14*1)/3
    float expected1 = (4.0f * 3.14f * 1.0f) / 3.0f;
    assert(std::fabs(sphereVolume(1) - expected1) < 1e-5);
    // Radius 3 -> (4*3.14*27)/3
    float expected3 = (4.0f * 3.14f * 27.0f) / 3.0f;
    assert(std::fabs(sphereVolume(3) - expected3) < 1e-5);
    // Radius 10 -> (4*3.14*1000)/3
    float expected10 = (4.0f * 3.14f * 1000.0f) / 3.0f;
    assert(std::fabs(sphereVolume(10) - expected10) < 1e-5);
    // Radius 7 -> (4*3.14*343)/3
    float expected7 = (4.0f * 3.14f * 343.0f) / 3.0f;
    assert(std::fabs(sphereVolume(7) - expected7) < 1e-5);
    return 0;
}
