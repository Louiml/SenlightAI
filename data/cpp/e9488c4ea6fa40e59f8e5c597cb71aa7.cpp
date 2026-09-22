// Write a C++ function named `classifyTriangle` that takes three integer side lengths `a`, `b`, and `c` (each strictly positive) and returns a `std::string` describing the triangle type. The function must return `"Invalid"` if the three lengths cannot form a valid triangle (i.e., the sum of any two sides is not strictly greater than the third side, or equivalently, the sum of the two smaller sides is ≤ the largest side). Otherwise, return `"Equilateral"` if all three sides are equal, `"Isosceles"` if exactly two sides are equal, or `"Scalene"` if all sides are different. The function must handle any positive integer inputs and must not read from standard input; it should be pure and side-effect free.
// The solution requires first determining whether the three lengths satisfy the triangle inequality theorem. The most straightforward check is to find the maximum of the three sides using `std::max({a,b,c})` and then verify that the sum of the other two sides is strictly greater than that maximum. Equivalently, the condition `a + b + c > 2 * maxSide` must hold (since if the largest side is `m`, the other two sum to `total - m`, and we require `total - m > m` → `total > 2m`). If this fails, return `"Invalid"`. Otherwise, classify by comparing the sides: if all three are equal, it is equilateral; if at least two are equal (but not all three), it is isosceles; otherwise, it is scalene. Edge cases include very large values (all within `int` range, since we sum them, but we could use `long long` for safety) and sides like `0` or negative values, which are inherently invalid—however, the problem statement assumes positive inputs, so we can rely on the triangle inequality check to catch invalid triples like `(1,1,2)` and `(5,3,1)`. Time complexity is O(1) and space complexity is O(1), with the returned string requiring O(1) memory for fixed-length classifications.
#include <string>
#include <algorithm>

// Classify a triangle given three positive side lengths.
// Returns "Invalid", "Equilateral", "Isosceles", or "Scalene".
std::string classifyTriangle(int a, int b, int c) {
    const int max_side = std::max({a, b, c});
    const long long total = static_cast<long long>(a) + b + c;

    // Triangle inequality: sum of two smaller sides must exceed the largest.
    if (total <= 2LL * max_side) {
        return "Invalid";
    }

    if (a == b && b == c) {
        return "Equilateral";
    }
    if (a == b || b == c || a == c) {
        return "Isosceles";
    }
    return "Scalene";
}
#include <cassert>
#include <string>

// The solution function is declared here
std::string classifyTriangle(int a, int b, int c);

int main() {
    // Valid equilateral
    assert(classifyTriangle(5, 5, 5) == "Equilateral");
    // Valid isosceles (two equal)
    assert(classifyTriangle(3, 3, 4) == "Isosceles");
    assert(classifyTriangle(4, 3, 3) == "Isosceles");
    assert(classifyTriangle(3, 4, 3) == "Isosceles");
    // Valid scalene
    assert(classifyTriangle(3, 4, 5) == "Scalene");
    assert(classifyTriangle(7, 8, 9) == "Scalene");
    // Invalid triples
    assert(classifyTriangle(1, 1, 2) == "Invalid");
    assert(classifyTriangle(5, 3, 1) == "Invalid");
    assert(classifyTriangle(1, 2, 1) == "Invalid");
    // Degenerate zero value (not positive, but check it is invalid)
    assert(classifyTriangle(0, 0, 0) == "Invalid");
    // Large values
    assert(classifyTriangle(2000000000, 2000000000, 2000000000) == "Equilateral");
    assert(classifyTriangle(2000000000, 1999999999, 1) == "Invalid");
    return 0;
}
