/*
Write a C++ function named `quadrant_info` that takes two integer coordinates `x` and `y` as parameters by value and returns a `std::string` describing the position of the point `(x, y)` on the Cartesian plane. The function must return the exact lowercase phrases: `"first quadrant"`, `"second quadrant"`, `"third quadrant"`, `"fourth quadrant"`, `"x-axis"` (if the point lies on the x‑axis but not the origin), `"y-axis"` (if the point lies on the y‑axis but not the origin), and `"origin"` (if both coordinates are zero). The logic must handle all integer combinations, including negative numbers, zero, and positive numbers, with no ambiguity. The function should not print anything; it only returns the string.
*/
#include <string>

// Return a description of the position of point (x, y) on the Cartesian plane.
std::string quadrant_info(int x, int y) {
    if (x == 0 && y == 0) {
        return "origin";
    }
    if (y == 0) {
        return "x-axis";
    }
    if (x == 0) {
        return "y-axis";
    }
    if (x > 0 && y > 0) {
        return "first quadrant";
    }
    if (x < 0 && y > 0) {
        return "second quadrant";
    }
    if (x < 0 && y < 0) {
        return "third quadrant";
    }
    // x > 0 && y < 0
    return "fourth quadrant";
}
#include <cassert>
#include <string>

// Declaration for testing.
std::string quadrant_info(int x, int y);

int main() {
    assert(quadrant_info(3, 4) == "first quadrant");
    assert(quadrant_info(-2, 5) == "second quadrant");
    assert(quadrant_info(-7, -8) == "third quadrant");
    assert(quadrant_info(9, -1) == "fourth quadrant");
    assert(quadrant_info(0, 0) == "origin");
    assert(quadrant_info(-3, 0) == "x-axis");
    assert(quadrant_info(0, 6) == "y-axis");
    assert(quadrant_info(0, -4) == "y-axis");
    assert(quadrant_info(12, 0) == "x-axis");
    assert(quadrant_info(-1, -1) == "third quadrant");
}
// The solution uses a series of conditional checks based on the signs of `x` and `y`. The main algorithm first tests for the origin (`x == 0 && y == 0`) because it is the most specific case and must be checked before axis checks. Then it checks for points on the x‑axis (`y == 0` and `x != 0`) and the y‑axis (`x == 0` and `y != 0`) — these must be handled before quadrant checks to avoid misclassifying axis points as quadrant points. After eliminating origin and axis cases, the remaining possibilities have both `x` and `y` non‑zero, so the four quadrant conditions can be checked directly: `x > 0 && y > 0`, `x < 0 && y > 0`, `x < 0 && y < 0`, `x > 0 && y < 0`. The edge cases include zero coordinates (which all fall into origin or axis categories) and negative values (which simply need correct sign comparisons). Time complexity is O(1) because only a fixed number of constant‑time comparisons are performed. Space complexity is O(1) for the function itself, excluding the returned string’s internal storage.
