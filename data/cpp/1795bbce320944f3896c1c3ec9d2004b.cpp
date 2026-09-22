// Write a C++ function that takes a single `double` radius value and returns a `double` representing the area of a circle, using π = 3.14159 as an approximation. The function must perform the calculation with double precision, accept any non-negative radius (including zero), and return the result. The task is to implement this as a standalone, reusable function with proper `const` correctness, and then verify it using test assertions.
// The area of a circle is calculated using the formula \( A = \pi r^2 \). Here, π is provided as a constant approximation 3.14159. The main algorithm is straightforward: read the radius as a double, multiply it by itself, and multiply the result by π. Edge cases include a radius of zero, which should return zero, and very large or very small radii, which require no special handling since `double` provides sufficient range and precision for typical inputs. Negative radii are not physically meaningful, so the function should assert or assume non-negative input; for robustness, we can add an `assert` in the implementation to catch invalid usage during debugging. Time complexity is \(O(1)\) and space complexity is \(O(1)\), as the calculation involves a constant number of operations.
#include <cassert>

// Calculate the area of a circle given its radius.
// Assumes radius is non-negative.
double circleArea(double radius) {
    const double PI = 3.14159;
    assert(radius >= 0.0 && "Radius must be non-negative");
    return PI * radius * radius;
}
#include <cassert>
#include <cmath>

// Free function declaration (must match the solution)
double circleArea(double radius);

int main() {
    // tolerance for floating-point comparison
    const double EPS = 1e-9;

    // Test with zero radius
    assert(std::fabs(circleArea(0.0) - 0.0) < EPS);

    // Test with radius 1 -> π * 1^2 = 3.14159
    assert(std::fabs(circleArea(1.0) - 3.14159) < EPS);

    // Test with radius 2 -> π * 4 = 12.56636
    assert(std::fabs(circleArea(2.0) - 12.56636) < EPS);

    // Test with radius 3.5 -> π * 12.25 = 38.4844775
    assert(std::fabs(circleArea(3.5) - 38.4844775) < EPS);

    // Test with a negative radius (should trigger the assert in debug mode)
    // This is commented out because it would abort the program.
    // assert(circleArea(-1.0) == 3.14159); // intentionally not tested
}
