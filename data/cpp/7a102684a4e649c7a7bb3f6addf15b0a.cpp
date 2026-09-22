/*
Write a C++ function that takes an integer `N` (where `N >= 3`) and returns the sum of the interior angles of a convex polygon with `N` sides, in degrees. The function should be named `interiorAngleSum` and accept the number of sides as a parameter. It must handle the case where `N` is less than 3 by returning 0 (since a polygon with fewer than 3 sides is degenerate), and it should be robust for very large `N` (up to the maximum `int` value) by performing the calculation safely without overflow, using a suitable type for the result (e.g., `long long`). The function must not read from standard input or produce output; it simply computes and returns the result.
*/
#include <cstdint>

// Return the sum of interior angles of a convex polygon with N sides.
// If N < 3 (degenerate polygon), returns 0.
long long interiorAngleSum(int N) {
    if (N < 3) {
        return 0;
    }
    // Cast N to long long before subtraction to avoid overflow in the multiplication.
    long long sides = static_cast<long long>(N);
    return (sides - 2) * 180;
}
#include <cassert>

// Forward declaration of the solution function.
long long interiorAngleSum(int N);

int main() {
    // Triangle: 3 sides -> 180 degrees.
    assert(interiorAngleSum(3) == 180);
    // Quadrilateral: 4 sides -> 360 degrees.
    assert(interiorAngleSum(4) == 360);
    // Pentagon: 5 sides -> 540 degrees.
    assert(interiorAngleSum(5) == 540);
    // Hexagon: 6 sides -> 720 degrees.
    assert(interiorAngleSum(6) == 720);
    // Decagon: 10 sides -> 1440 degrees.
    assert(interiorAngleSum(10) == 1440);
    // Degenerate: 2 sides -> 0.
    assert(interiorAngleSum(2) == 0);
    // Degenerate: 1 side -> 0.
    assert(interiorAngleSum(1) == 0);
    // Degenerate: 0 sides -> 0.
    assert(interiorAngleSum(0) == 0);
    // Degenerate: negative -> 0.
    assert(interiorAngleSum(-5) == 0);
    // Large N: 1000000 sides -> (1000000-2)*180 = 179999640.
    assert(interiorAngleSum(1000000) == 179999640);
    return 0;
}
// The sum of interior angles of an `N`-sided convex polygon is given by the formula `(N - 2) * 180`. The main algorithm is straightforward: subtract 2 from the input `N`, multiply by 180, and return the result. Edge cases include when `N` is less than 3, where we return 0 because the formula gives a non-positive or nonsensical value for degenerate shapes (e.g., `N=1` gives -180, `N=2` gives 0, but neither represents a valid polygon). For large `N`, the product `(N-2)*180` can exceed the range of a 32-bit `int` (e.g., for `N` near 2^31, the product is about 3.8e11, which fits in `long long` but not `int`), so we use a `long long` return type and cast the intermediate calculation to `long long` to avoid overflow. The time complexity is O(1), as only a constant number of arithmetic operations are performed, and the space complexity is O(1) as well.
