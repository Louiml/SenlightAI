Write a standalone C++ function named `compareRectangleMetrics` that takes two integer parameters representing the length and width of a rectangle, computes its perimeter and area, and returns a `std::string` indicating which value is greater. The returned string must be exactly `"AREA IS GREATER"` if the area is strictly greater than the perimeter, `"PERIMETER IS GREATER"` if the perimeter is strictly greater than the area, and `"EQUAL"` if they are equal. The function should use `const` parameters and be self-contained (only include necessary headers). Assume inputs are positive integers (length and width > 0) to avoid degenerate cases, but handle edge cases such as equal area and perimeter (e.g., a square with side length 4: area=16, perimeter=16) correctly. The function must not read from standard input or print to standard output—it only returns the comparison string. Provide a reference implementation and test it with various cases including small, large, and equal values.

The solution computes the rectangle's area as `length * width` and perimeter as `2 * (length + width)` using integer arithmetic. The comparison logic is straightforward: if `area > perimeter`, return the area-greater message; else if `perimeter > area`, return the perimeter-greater message; otherwise, they are equal and return `"EQUAL"`. Key edge cases: when length and width are both positive integers, area and perimeter are always positive; equality occurs when `length * width == 2 * (length + width)`, which has integer solutions like (3,6), (4,4), (6,3). Also, note that for very large inputs (up to ~46340 for 32-bit int product to avoid overflow), but we assume inputs fit within `int` range; the function uses `int` parameters and returns a `std::string`. Time complexity is O(1) because only a few arithmetic operations are performed. Space complexity is O(1) aside from the returned string. No need for dynamic memory or complex data structures.

#include <string>

// Compare the area and perimeter of a rectangle.
// Parameters: length (positive integer), width (positive integer).
// Returns a string indicating which value is greater, or "EQUAL".
std::string compareRectangleMetrics(const int length, const int width) {
    const int area = length * width;
    const int perimeter = 2 * (length + width);

    if (area > perimeter) {
        return "AREA IS GREATER";
    } else if (perimeter > area) {
        return "PERIMETER IS GREATER";
    } else {
        return "EQUAL";
    }
}

#include <cassert>
#include <string>

// Free function declaration for testing (must match the solution signature).
std::string compareRectangleMetrics(const int length, const int width);

int main() {
    // Typical case where area > perimeter (e.g., 5x5: area=25, perimeter=20)
    assert(compareRectangleMetrics(5, 5) == "AREA IS GREATER");

    // Typical case where perimeter > area (e.g., 1x10: area=10, perimeter=22)
    assert(compareRectangleMetrics(1, 10) == "PERIMETER IS GREATER");

    // Edge case where they are equal (4x4): area=16, perimeter=16
    assert(compareRectangleMetrics(4, 4) == "EQUAL");

    // Another equal case (3x6): area=18, perimeter=18
    assert(compareRectangleMetrics(3, 6) == "EQUAL");

    // Symmetric case: length and width swapped should give same result
    assert(compareRectangleMetrics(6, 3) == "EQUAL");

    // Large square: 100x100, area=10000 > perimeter=400
    assert(compareRectangleMetrics(100, 100) == "AREA IS GREATER");

    // Narrow rectangle: 1x1, area=1 < perimeter=4
    assert(compareRectangleMetrics(1, 1) == "PERIMETER IS GREATER");

    // Slightly unbalanced: 2x3, area=6, perimeter=10
    assert(compareRectangleMetrics(2, 3) == "PERIMETER IS GREATER");

    // Another area-greater case: 10x2, area=20, perimeter=24? Actually check: perimeter=2*(10+2)=24, area=20 => perimeter greater
    // So use a different case: 10x3, area=30, perimeter=26
    assert(compareRectangleMetrics(10, 3) == "AREA IS GREATER");

    // Large values: 200x200, area=40000 > perimeter=800
    assert(compareRectangleMetrics(200, 200) == "AREA IS GREATER");

    return 0;
}
