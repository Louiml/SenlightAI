// Write a C++ function named `triangleStats` that takes three integer side lengths (`s1`, `s2`, `s3`) and returns a `std::pair<double, double>` containing the perimeter and the area of the triangle, in that order. The perimeter is simply the sum of the three sides. The area should be computed using Heron's formula: first compute the semiperimeter `s = (s1 + s2 + s3) / 2.0`, then the area is `sqrt(s * (s - s1) * (s - s2) * (s - s3))`. If the three sides cannot form a valid triangle (i.e., the sum of any two sides is less than or equal to the third side, or any side is non-positive), the function should return `{-1.0, -1.0}`. Use `std::sqrt` for the square root and `std::pair<double,double>` as the return type. The function should be `const`-correct and not modify its inputs. Provide a self-contained implementation with necessary headers.

The solution approach is straightforward. First, validate the input: a triangle is valid only if all sides are positive (greater than 0) and the triangle inequality holds: `s1 + s2 > s3`, `s1 + s3 > s2`, and `s2 + s3 > s1`. If any condition fails, return `{-1.0, -1.0}`. Otherwise, compute the perimeter as `p = s1 + s2 + s3` (using double to avoid integer overflow for large inputs), and the semiperimeter as `s = p / 2.0`. Then apply Heron's formula: `area = std::sqrt(s * (s - s1) * (s - s2) * (s - s3))`. Use `double` for all arithmetic to avoid integer truncation. Edge cases include degenerate triangles (where the sum of two sides equals the third) which are invalid and must return `-1.0`; also, very large side lengths may cause overflow in integer addition, so cast to `double` before summing. Time complexity is O(1), space complexity is O(1). The main challenge is ensuring the triangle validity check is correct and returning the pair in the correct order (perimeter first, then area).

#include <utility>
#include <cmath>

// Compute perimeter and area of a triangle with given side lengths.
// Returns {-1.0, -1.0} if the sides do not form a valid triangle.
std::pair<double, double> triangleStats(int s1, int s2, int s3) {
    // Convert to double to avoid overflow and to perform precise arithmetic.
    double a = static_cast<double>(s1);
    double b = static_cast<double>(s2);
    double c = static_cast<double>(s3);

    // Validate sides: all positive and satisfy triangle inequality.
    if (a <= 0 || b <= 0 || c <= 0 ||
        a + b <= c || a + c <= b || b + c <= a) {
        return {-1.0, -1.0};
    }

    double perimeter = a + b + c;
    double semiperimeter = perimeter / 2.0;

    // Heron's formula: area = sqrt(s*(s-a)*(s-b)*(s-c))
    double area = std::sqrt(semiperimeter * (semiperimeter - a) * 
                            (semiperimeter - b) * (semiperimeter - c));

    return {perimeter, area};
}

#include <cassert>
#include <cmath>

int main() {
    // Valid triangle: 3-4-5 (right triangle)
    auto result1 = triangleStats(3, 4, 5);
    assert(result1.first == 12.0);
    assert(std::abs(result1.second - 6.0) < 1e-9);

    // Valid equilateral triangle
    auto result2 = triangleStats(6, 6, 6);
    assert(result2.first == 18.0);
    assert(std::abs(result2.second - (std::sqrt(27.0) * 2.0)) < 1e-9); // area = 9*sqrt(3) ≈ 15.588

    // Invalid: degenerate triangle (2+3=5)
    auto result3 = triangleStats(2, 3, 5);
    assert(result3.first == -1.0 && result3.second == -1.0);

    // Invalid: negative side
    auto result4 = triangleStats(-1, 2, 3);
    assert(result4.first == -1.0 && result4.second == -1.0);

    // Invalid: sum of two sides less than third
    auto result5 = triangleStats(1, 2, 10);
    assert(result5.first == -1.0 && result5.second == -1.0);

    // Valid: large numbers (still within double precision)
    auto result6 = triangleStats(2000000000, 2000000000, 2000000000);
    assert(result6.first == 6000000000.0);
    assert(result6.second > 0.0);

    return 0;
}
