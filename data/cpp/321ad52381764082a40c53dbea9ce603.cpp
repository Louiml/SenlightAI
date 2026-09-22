// Write a C++ function named `isInsideCentralSquare` that takes two integer coordinates `x` and `y` and returns a boolean value indicating whether the point `(x, y)` lies inside or on the boundary of a square centered at the origin with side length 4 (i.e., x and y each between -2 and 2, inclusive). The function should handle all integer inputs, including extreme values, and should not read from or write to any standard input/output streams; it should only compute and return the result.
#include <cassert>

int main() {
    // Points inside the square
    assert(isInsideCentralSquare(0, 0) == true);
    assert(isInsideCentralSquare(2, 2) == true);
    assert(isInsideCentralSquare(-2, -2) == true);
    assert(isInsideCentralSquare(1, -1) == true);
    assert(isInsideCentralSquare(-2, 2) == true);

    // Points outside the square
    assert(isInsideCentralSquare(3, 0) == false);
    assert(isInsideCentralSquare(0, 3) == false);
    assert(isInsideCentralSquare(-3, -3) == false);
    assert(isInsideCentralSquare(100, -100) == false);
    assert(isInsideCentralSquare(-3, 1) == false);
    assert(isInsideCentralSquare(1, 3) == false);

    // Boundary cases
    assert(isInsideCentralSquare(-2, 0) == true);
    assert(isInsideCentralSquare(2, -2) == true);
    assert(isInsideCentralSquare(-2, 2) == true);

    return 0;
}
#include <cstdlib> // for std::abs (though not strictly needed, we use direct comparisons)

// Returns true if the point (x, y) lies inside or on the boundary
// of the axis-aligned square centered at (0,0) with side length 4.
bool isInsideCentralSquare(int x, int y) {
    // Check both coordinates are within [-2, 2] inclusive.
    return (x >= -2 && x <= 2) && (y >= -2 && y <= 2);
}
// The problem reduces to checking whether both coordinates satisfy the inequality `abs(x) <= 2` and `abs(y) <= 2`. This is equivalent to `x >= -2 && x <= 2 && y >= -2 && y <= 2`. The square includes its boundary, so points with coordinates exactly -2 or 2 are considered inside. There are no special edge cases beyond the boundary inclusion and handling of negative values; since the condition uses integer comparisons, overflow is not a concern because the values are only compared to small constants. The algorithm runs in constant O(1) time and uses O(1) space, regardless of the input magnitude.
