/*
Write a C++ function that takes a single integer `n` representing the number of sides of a convex polygon and returns the sum of its interior angles in degrees. The formula for the sum of interior angles of an n-sided polygon is `(n - 2) * 180`. The function must handle the case where `n` is less than 3 (invalid for a polygon) by returning 0 as a sentinel value. The function should be named `sumInteriorAngles` and should accept an `int` parameter, returning an `int`.
*/
// Return the sum of interior angles (in degrees) for an n-sided convex polygon.
// Returns 0 for invalid n (n < 3).
int sumInteriorAngles(const int n) {
    if (n < 3) {
        return 0;
    }
    return (n - 2) * 180;
}
int main() {
    // Basic valid cases
    assert(sumInteriorAngles(3) == 180);   // Triangle
    assert(sumInteriorAngles(4) == 360);   // Quadrilateral
    assert(sumInteriorAngles(5) == 540);   // Pentagon
    assert(sumInteriorAngles(6) == 720);   // Hexagon
    assert(sumInteriorAngles(8) == 1080);  // Octagon
    
    // Edge cases: invalid n
    assert(sumInteriorAngles(0) == 0);
    assert(sumInteriorAngles(1) == 0);
    assert(sumInteriorAngles(2) == 0);
    assert(sumInteriorAngles(-5) == 0);
    
    // Large valid n
    assert(sumInteriorAngles(100) == 17640);
}
// The problem is straightforward: given an integer `n` (the number of sides), compute `(n - 2) * 180`. The main algorithm is a single arithmetic operation. Edge cases: if `n` is less than 3, the input is invalid (a polygon must have at least 3 sides), so return 0. Negative numbers and zero also fall under this invalid case. The formula works for `n >= 3`, and the result will always be a positive integer for valid input. Time complexity is O(1) — constant time, and space complexity is O(1) — no additional memory used beyond a few local variables.
