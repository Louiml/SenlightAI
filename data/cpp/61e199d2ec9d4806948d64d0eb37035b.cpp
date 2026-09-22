/*
Write a C++ function named `triangleType` that takes three integer side lengths (assume all are positive and represent valid triangle sides) and returns a `std::string` describing the triangle type: `"equilateral"` if all three sides are equal, `"isosceles"` if exactly two sides are equal, and `"scalene"` if all sides are different. The function must use `const` references for its parameters, be self-contained with proper headers, and follow the logic of the provided snippet (which incorrectly reads input but correctly classifies). The function should be pure and free of any I/O; classification must rely solely on integer comparisons.
*/

#include <string>

// Classify a triangle based on three integer side lengths.
// Precondition: a, b, c are positive integers representing a valid triangle.
// Returns "equilateral", "isosceles", or "scalene".
std::string triangleType(const int& a, const int& b, const int& c) {
    if (a == b && a == c) {
        return "equilateral";
    }
    if (a == b || a == c || b == c) {
        return "isosceles";
    }
    return "scalene";
}

#include <cassert>
#include <string>

// Function declaration (from solution)
std::string triangleType(const int& a, const int& b, const int& c);

int main() {
    // All sides equal
    assert(triangleType(5, 5, 5) == "equilateral");
    // Two sides equal (various positions)
    assert(triangleType(3, 3, 2) == "isosceles");
    assert(triangleType(3, 2, 3) == "isosceles");
    assert(triangleType(2, 3, 3) == "isosceles");
    // All sides different
    assert(triangleType(2, 3, 4) == "scalene");
    // Large values and duplicates
    assert(triangleType(1000, 1000, 1000) == "equilateral");
    assert(triangleType(1, 2, 3) == "scalene");
    // Values that are not sorted
    assert(triangleType(7, 4, 4) == "isosceles");
    // Check with negative (though invalid, function still classifies)
    assert(triangleType(-1, -1, -1) == "equilateral");
    // Two large different and one equal
    assert(triangleType(999, 1000, 999) == "isosceles");
    // All different large values
    assert(triangleType(100, 200, 300) == "scalene");
    return 0;
}

// The main algorithm involves three simple equality checks. First, test if all sides are equal (`a==b && b==c`) → return "equilateral". If not, test if any two sides are equal (`a==b || a==c || b==c`) → return "isosceles". Otherwise, return "scalene". Edge cases include duplicate values (e.g., two equal sides), all sides equal, and all different. Negative or zero values are not handled by the task specification, but for robustness one could argue they are invalid; however, since the prompt only requires classification, we simply compare. Time complexity is O(1) because only constant comparisons are performed; space complexity is O(1) for computation, though the returned string allocates O(length of type name) memory, which is constant. No input validation is needed beyond the given assumptions.
