/*
Write a C++ function named `circleArea` that takes an integer radius and a float representation of π (pi) as parameters, and returns the area of a circle computed as `pi * radius * radius` as a float. The function must handle non-negative radius values correctly, and for a radius of 0, it must return 0.0f. The solution should use `const` correctly for parameters that are not modified. The function should be self-contained with necessary headers, and must not include a `main` function or any input/output in the function itself (the input/output will be handled externally in tests).
*/
#include <cstdint>

// Compute the area of a circle given its integer radius and a float value for pi.
// The function uses const parameters to guarantee they are not modified.
float circleArea(const int radius, const float pi) {
    // Direct computation: area = pi * r^2
    return pi * static_cast<float>(radius) * static_cast<float>(radius);
}
#include <cassert>
#include <cmath>

// Forward declaration of the solution function (assumed to be in a separate file).
float circleArea(const int radius, const float pi);

int main() {
    // Test with radius 3 and pi = 3.14f (from the original snippet)
    assert(std::fabs(circleArea(3, 3.14f) - 28.26f) < 0.001f);
    
    // Test with radius 0 -> area is 0
    assert(std::fabs(circleArea(0, 3.14f) - 0.0f) < 0.001f);
    
    // Test with radius 1 -> area is pi
    assert(std::fabs(circleArea(1, 3.14f) - 3.14f) < 0.001f);
    
    // Test with radius 10 and pi = 3.14159f
    assert(std::fabs(circleArea(10, 3.14159f) - 314.159f) < 0.001f);
    
    // Test with a large radius (e.g., 1000) and pi = 3.14f -> area ~ 3,140,000
    assert(std::fabs(circleArea(1000, 3.14f) - 3140000.0f) < 1.0f);
    
    // Test with radius 2 and pi = 3.0f (simplified pi)
    assert(std::fabs(circleArea(2, 3.0f) - 12.0f) < 0.001f);
    
    // Test with radius 5 and pi = 3.14159f, area = 3.14159 * 25 = 78.53975
    assert(std::fabs(circleArea(5, 3.14159f) - 78.53975f) < 0.001f);
    
    // Test with radius 7 and pi = 3.14f, area = 3.14 * 49 = 153.86
    assert(std::fabs(circleArea(7, 3.14f) - 153.86f) < 0.001f);
    
    // Test with radius 100 and pi = 3.14f, area = 3.14 * 10000 = 31400
    assert(std::fabs(circleArea(100, 3.14f) - 31400.0f) < 0.01f);
    
    // Test with radius 1 and pi = 3.14159f
    assert(std::fabs(circleArea(1, 3.14159f) - 3.14159f) < 0.0001f);
    
    return 0;
}
// The solution is straightforward: compute the product of pi, radius, and radius, and return the result as a float. The main algorithm involves a single multiplication expression: `pi * radius * radius`. Since the parameters are not modified, use `const` for both parameters to indicate read-only access. Edge cases: a radius of 0 produces an area of 0, which is naturally handled by multiplication (0 * anything = 0). Negative radii are not physically meaningful, but if passed, the square of the radius makes the result positive; however, the task specifies handling non-negative radii, so no special case is needed. Time complexity is O(1) and space complexity is O(1) since only a constant amount of operations and one return value are used. No loops or variable storage beyond the expression evaluation are required.
