// Write a C++ function named `classifyTriangle` that takes three integer side lengths and returns a `std::string` describing the triangle type. The function must first check whether the three sides satisfy the triangle inequality (i.e., the sum of any two sides must be greater than the third side). If they do not, return `"No triangle"`. If they do form a triangle, classify it as `"Equilateral triangle"` (all three sides equal), `"Isosceles triangle"` (exactly two sides equal), or `"Scalene triangle"` (all sides different). Additionally, if the sides form a right triangle (i.e., the square of one side equals the sum of the squares of the other two), append `" and Right triangle"` to the already-determined classification (e.g., `"Scalene triangle and Right triangle"`). The returned string must exactly match these values, with no leading/trailing spaces. The input side lengths are positive integers (each > 0) and no other validation is required. Example: for inputs `(3, 4, 5)`, the function returns `"Scalene triangle and Right triangle"`; for `(5, 5, 5)`, returns `"Equilateral triangle"`; for `(1, 2, 3)`, returns `"No triangle"`.

// The solution approach is straightforward and follows the logic from the provided code snippet. First, check the triangle inequality condition: `a + b > c && b + c > a && c + a > b`. If this fails, return `"No triangle"`. If it passes, classify based on side equality: if all sides equal → `"Equilateral triangle"`; else if any two sides equal → `"Isosceles triangle"`; else → `"Scalene triangle"`. Then, check the right-triangle condition: `a*a + b*b == c*c || b*b + c*c == a*a || c*c + a*a == b*b`. If true, append `" and Right triangle"` to the classification string. Important edge cases: (1) An equilateral triangle can never be a right triangle, but the code handles it anyway (it simply won't satisfy the right condition). (2) An isosceles right triangle (e.g., `(1, 1, sqrt(2))`) is impossible with integer sides, but the code handles it if somehow given (e.g., no integer solution exists, so it's fine). (3) The order of checks matters: first verify triangle inequality, then classify type, then check right—appending the right description separately. Time complexity is O(1) since we only perform constant-time comparisons and arithmetic. Space complexity is O(1) for the function itself (the returned string allocation is not counted in auxiliary space). The function is pure, deterministic, and works for any positive integer inputs within the range of `int`.

#include <string>

// Classify a triangle given three side lengths.
// Returns "No triangle" if sides do not form a triangle.
// Otherwise returns type ("Equilateral triangle", "Isosceles triangle", or "Scalene triangle")
// and appends " and Right triangle" if the triangle is also a right triangle.
std::string classifyTriangle(int a, int b, int c) {
    // Verify triangle inequality.
    if (!(a + b > c && b + c > a && c + a > b)) {
        return "No triangle";
    }

    // Determine triangle type.
    std::string result;
    if (a == b && b == c) {
        result = "Equilateral triangle";
    } else if (a == b || b == c || c == a) {
        result = "Isosceles triangle";
    } else {
        result = "Scalene triangle";
    }

    // Check right triangle condition and append if true.
    if (a * a + b * b == c * c ||
        b * b + c * c == a * a ||
        c * c + a * a == b * b) {
        result += " and Right triangle";
    }

    return result;
}

#include <cassert>
#include <string>

// Declaration of the function under test (already provided above).
std::string classifyTriangle(int a, int b, int c);

int main() {
    // Basic triangle types.
    assert(classifyTriangle(5, 5, 5) == "Equilateral triangle");
    assert(classifyTriangle(5, 5, 3) == "Isosceles triangle");
    assert(classifyTriangle(3, 4, 5) == "Scalene triangle and Right triangle");
    assert(classifyTriangle(1, 2, 3) == "No triangle");
    assert(classifyTriangle(2, 2, 4) == "No triangle"); // 2+2 == 4, not > 4.
    assert(classifyTriangle(3, 3, 6) == "No triangle");
    assert(classifyTriangle(6, 8, 10) == "Scalene triangle and Right triangle");
    assert(classifyTriangle(5, 12, 13) == "Scalene triangle and Right triangle");
    assert(classifyTriangle(10, 24, 26) == "Scalene triangle and Right triangle");
    assert(classifyTriangle(8, 15, 17) == "Scalene triangle and Right triangle");
    return 0;
}
