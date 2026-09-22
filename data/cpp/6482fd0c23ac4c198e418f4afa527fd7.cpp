Write a C++ function named `circleArea` that takes a single floating-point argument `radius` and returns the area of a circle with that radius, using `3.1415926` as the value of π (pi). The function must be `const`-correct and should not modify its input parameter. You do not need to handle negative radii in the function itself—the caller is responsible for passing a non-negative radius—but your solution should clearly document this precondition. The function should compute the area using the formula `π * r * r` (i.e., pi times radius squared) and return the result as a `double` (not a `float`) for better precision. You must **not** write a `main` function or any entry-point wrapper; only provide the free function definition with appropriate headers (`<cmath>` is not needed; you can just multiply). Your solution should be self-contained and use `const` for the parameter where appropriate.

#include <cassert>
#include <cmath>

int main() {
    // Basic positive radius
    assert(std::abs(circleArea(1.0) - 3.1415926) < 1e-6);
    // Zero radius
    assert(circleArea(0.0) == 0.0);
    // Radius 2: area = 3.1415926 * 4 = 12.5663704
    assert(std::abs(circleArea(2.0) - 12.5663704) < 1e-6);
    // Radius 10: area = 3.1415926 * 100 = 314.15926
    assert(std::abs(circleArea(10.0) - 314.15926) < 1e-5);
    // Radius 0.5: area = 3.1415926 * 0.25 = 0.78539815
    assert(std::abs(circleArea(0.5) - 0.78539815) < 1e-6);
    // Large radius 1000: area = 3.1415926 * 1e6 = 3141592.6
    assert(std::abs(circleArea(1000.0) - 3141592.6) < 0.1);
    // Radius 3: area = 3.1415926 * 9 = 28.2743334
    assert(std::abs(circleArea(3.0) - 28.2743334) < 1e-6);
    // Radius 4: area = 3.1415926 * 16 = 50.2654816
    assert(std::abs(circleArea(4.0) - 50.2654816) < 1e-6);
    // Radius 5: area = 3.1415926 * 25 = 78.539815
    assert(std::abs(circleArea(5.0) - 78.539815) < 1e-5);
    // Radius 7: area = 3.1415926 * 49 = 153.9380374
    assert(std::abs(circleArea(7.0) - 153.9380374) < 1e-5);
    return 0;
}

#include <cstddef> // Not strictly needed but included for completeness; not used.

// Return the area of a circle with the given radius.
// Precondition: radius >= 0.
// Uses π = 3.1415926 and returns a double for better precision.
double circleArea(const double radius) {
    constexpr double PI = 3.1415926;
    return PI * radius * radius;
}

// The solution is straightforward: multiply the radius by itself, then multiply by the constant π. The main algorithmic decision is to use a `double` for the return type to minimize floating-point rounding errors compared to `float`, especially for large radii. For the constant π, we can define it as a `constexpr double` inside the function or as a file-scope constant; using a `constexpr` is preferred because it is compile-time and carries no runtime overhead. Edge cases: the formula works for `radius = 0` (returns 0), and for very small or large values it may suffer from floating-point precision limits, but that is inherent to the domain. Time complexity is O(1) because only a fixed number of arithmetic operations are performed. Space complexity is O(1) as no additional storage is used.
