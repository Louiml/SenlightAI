/*
Write a C++ function named `quadrant` that takes two integer coordinates, `x` and `y`, and returns an integer representing the Cartesian coordinate quadrant: 1 for quadrant I (x > 0, y > 0), 2 for quadrant II (x < 0, y > 0), 3 for quadrant III (x < 0, y < 0), and 4 for quadrant IV (x > 0, y < 0). The function must handle the edge case where either coordinate is zero: in that case, return 0 (to indicate the point lies on an axis or at the origin). Do not read from standard input or print to standard output; the function should be pure and side-effect-free.
*/
#include <cstddef> // not needed, but for completeness

// Determine the Cartesian quadrant for a point (x, y).
// Returns 0 if the point is on an axis or at the origin.
// Returns 1 (+,+), 2 (-,+), 3 (-,-), or 4 (+,-) otherwise.
int quadrant(int x, int y) {
    if (x == 0 || y == 0) {
        return 0;
    }
    if (x > 0) {
        return (y > 0) ? 1 : 4;
    } else { // x < 0
        return (y > 0) ? 2 : 3;
    }
}
#include <cassert>

int quadrant(int, int); // Function declaration from solution

int main() {
    // Basic four quadrants
    assert(quadrant(1, 1) == 1);
    assert(quadrant(-1, 1) == 2);
    assert(quadrant(-1, -1) == 3);
    assert(quadrant(1, -1) == 4);
    // Edge cases with zero coordinates
    assert(quadrant(0, 5) == 0);
    assert(quadrant(5, 0) == 0);
    assert(quadrant(0, 0) == 0);
    // Zero with negative other coordinate
    assert(quadrant(0, -3) == 0);
    assert(quadrant(-3, 0) == 0);
    // Large values and boundary signs
    assert(quadrant(100, -100) == 4);
    assert(quadrant(-100, 100) == 2);
    return 0;
}
// The solution involves a straightforward comparison of the signs of `x` and `y`. The main algorithm checks the sign of `x` first, then within each branch checks the sign of `y` to determine the quadrant. If either coordinate is zero, the function immediately returns 0. This can be implemented with nested conditionals or by combining logical checks. Edge cases include positive and negative zero (treated the same) and points on axes or the origin. Time complexity is O(1) constant time, and space complexity is O(1) as no additional storage is used beyond the input parameters and return value.
