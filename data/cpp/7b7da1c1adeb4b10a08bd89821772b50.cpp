/*
Create a C++ function named `analyzeTriangle` that takes three floating-point side lengths (representing a triangle's sides) and returns a `std::pair<float, float>` containing the semiperimeter first and the area second, computed using Heron's formula. The function should validate that a valid triangle can exist (all sides positive and the sum of any two sides strictly greater than the third); if invalid, return `{-1.0f, -1.0f}` to indicate an error. The computed values should be rounded to two decimal places to avoid floating-point precision issues. Do not include any input/output operations—the function must be pure and self-contained.
*/

#include <cmath>
#include <utility>
#include <algorithm>

// Returns {semiperimeter, area} for a triangle with given side lengths.
// Returns {-1.0f, -1.0f} if the sides cannot form a valid triangle.
std::pair<float, float> analyzeTriangle(float a, float b, float c) {
    // Validate positive sides
    if (a <= 0.0f || b <= 0.0f || c <= 0.0f) {
        return {-1.0f, -1.0f};
    }
    // Validate triangle inequality
    if (a + b <= c || a + c <= b || b + c <= a) {
        return {-1.0f, -1.0f};
    }

    // Perform computations in double for precision
    double da = a, db = b, dc = c;
    double semiperimeter = (da + db + dc) / 2.0;

    // Heron's formula with safety clamp
    double product = semiperimeter * (semiperimeter - da) * (semiperimeter - db) * (semiperimeter - dc);
    if (product < 0.0 && product > -1e-12) {
        product = 0.0;
    }
    double area = std::sqrt(std::max(product, 0.0));

    // Round to two decimal places
    float roundedSemi = std::round(semiperimeter * 100.0) / 100.0;
    float roundedArea = std::round(area * 100.0) / 100.0;
    return {roundedSemi, roundedArea};
}

#include <cassert>
#include <cmath>

int main() {
    // Valid equilateral triangle sides 3,3,3 => semiperimeter=4.5, area≈3.897
    auto res1 = analyzeTriangle(3.0f, 3.0f, 3.0f);
    assert(std::fabs(res1.first - 4.5f) < 0.001f);
    assert(std::fabs(res1.second - 3.897f) < 0.01f);

    // Valid right triangle 3,4,5 => semiperimeter=6, area=6
    auto res2 = analyzeTriangle(3.0f, 4.0f, 5.0f);
    assert(res2.first == 6.0f);
    assert(res2.second == 6.0f);

    // Invalid: zero side
    auto res3 = analyzeTriangle(0.0f, 4.0f, 5.0f);
    assert(res3.first == -1.0f && res3.second == -1.0f);

    // Invalid: negative side
    auto res4 = analyzeTriangle(-1.0f, 2.0f, 3.0f);
    assert(res4.first == -1.0f && res4.second == -1.0f);

    // Invalid: degenerate (sum equals third side)
    auto res5 = analyzeTriangle(1.0f, 2.0f, 3.0f);
    assert(res5.first == -1.0f && res5.second == -1.0f);

    // Invalid: sum less than third side
    auto res6 = analyzeTriangle(1.0f, 2.0f, 4.0f);
    assert(res6.first == -1.0f && res6.second == -1.0f);

    // Very small sides (near-zero but positive) — should be valid but tiny area
    auto res7 = analyzeTriangle(0.001f, 0.001f, 0.001f);
    assert(res7.first > 0.0f && res7.second > 0.0f);
    assert(std::fabs(res7.first - 0.0015f) < 0.0001f);

    // Large sides (float precision safe within range)
    auto res8 = analyzeTriangle(1000.0f, 1000.0f, 1000.0f);
    assert(std::fabs(res8.first - 1500.0f) < 0.01f);
    assert(std::fabs(res8.second - 433012.70f) < 1.0f);

    return 0;
}

// The solution requires three steps: validation, semiperimeter computation, and area computation.  
// **Validation:** A triangle exists if all sides are > 0 and satisfy the triangle inequality (a+b > c, a+c > b, b+c > a). Edge cases include zero/negative sides, degenerate triangles (sum equals the third side), and extremely large values that might cause overflow—use double precision for intermediate arithmetic even if inputs are float.  
// **Semiperimeter:** `s = (a + b + c) / 2.0`. Since inputs are float, the sum may lose precision; cast to double early.  
// **Area:** Heron's formula: `sqrt(s * (s - a) * (s - b) * (s - c))`. The product inside sqrt could suffer from catastrophic cancellation if sides are nearly degenerate, but for valid triangles it's positive. To handle floating-point inaccuracies, clamp the inner product to a small epsilon (e.g., `1e-12`) if it becomes slightly negative due to rounding.  
// **Rounding:** Use `std::round(value * 100.0) / 100.0` to round to two decimals.  
// **Complexity:** O(1) time, O(1) auxiliary space.  
// **Return type:** `std::pair<float, float>` where first = semiperimeter, second = area. If invalid, return `{-1.0f, -1.0f}`.
