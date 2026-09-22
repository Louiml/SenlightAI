Write a C++ function that determines whether a given point `(x1, y1)` lies inside or on the boundary of a circle defined by its center `(cx, cy)` and radius `r`, where all coordinates and the radius are integers. The function should take these five integer parameters and return a boolean value (`true` if the point is inside/on the circle, `false` otherwise). Use exact integer arithmetic (e.g., comparing squared distances) to avoid floating-point precision issues—do not use `sqrt` or `pow` with doubles. The function must be usable in a loop that processes multiple queries, but the function itself should handle only one query.

// The core geometric condition is: a point is inside or on the circle if the Euclidean distance from the center to the point is less than or equal to the radius. Directly computing the distance using `sqrt` introduces floating‑point rounding errors, which can cause incorrect decisions when the point lies exactly on the circle or very close to it. Instead, we can square both sides of the inequality to avoid square roots:  
// `distance² = (x1 - cx)² + (y1 - cy)²` and we need `distance² <= r²`.  
// All values are integers, so the squares and their sum are integers, and the comparison is exact.  
// Edge cases:  
// - The point may coincide with the center (distance 0), which is always inside unless radius is negative—but radius is given as non‑negative, so it’s always inside.  
// - The point may be exactly on the circumference; the condition uses `<=` so that case returns `true`.  
// - Large coordinate values: since coordinates are up to 10⁹ in magnitude, squaring them may exceed 32‑bit int range, so use `long long` for the squared differences and sum.  
// Time complexity is `O(1)` per query. Space complexity is `O(1)`. If there are `n` queries, the overall time is `O(n)`.

#include <cstdint>

// Determine whether point (px, py) is inside or on the circle
// with center (cx, cy) and radius r. Uses integer arithmetic to avoid precision loss.
bool isInsideCircle(long long px, long long py, long long cx, long long cy, long long r) {
    // Squared distance from center to point
    long long dx = px - cx;
    long long dy = py - cy;
    long long distSq = dx * dx + dy * dy;
    long long rSq = r * r;
    
    // Inside or on the boundary if distance² <= r²
    return distSq <= rSq;
}

#include <cassert>

int main() {
    // Point exactly at the center
    assert(isInsideCircle(0, 0, 0, 0, 5) == true);
    
    // Point on the boundary
    assert(isInsideCircle(3, 4, 0, 0, 5) == true);
    
    // Point outside
    assert(isInsideCircle(6, 0, 0, 0, 5) == false);
    
    // Negative coordinates
    assert(isInsideCircle(-1, -1, 0, 0, 2) == true);
    assert(isInsideCircle(-3, -3, 0, 0, 2) == false);
    
    // Large values to test overflow safety
    assert(isInsideCircle(1000000000LL, 1000000000LL, 0, 0, 2000000000LL) == true);
    assert(isInsideCircle(1000000000LL, 1000000000LL, 0, 0, 1000000000LL) == false);
    
    // Zero radius: only the center is inside
    assert(isInsideCircle(2, 3, 2, 3, 0) == true);
    assert(isInsideCircle(2, 4, 2, 3, 0) == false);
    
    // Point exactly at the boundary with negative center
    assert(isInsideCircle(-2, 0, -2, 0, 1) == true);
    
    // Different quadrant
    assert(isInsideCircle(-4, 3, 0, 0, 5) == true);
    assert(isInsideCircle(-5, 0, 0, 0, 4) == false);
    
    return 0;
}
