Write a C++ function `bool isInsideSphere(double x, double y, double z, double cx, double cy, double cz, double radius)` that determines whether a 3D point `(x, y, z)` lies strictly inside a sphere with center `(cx, cy, cz)` and radius `radius`. The function returns `true` if the point is strictly inside (distance from center is less than radius), and `false` if the point is on the surface or outside. Use double precision for all computations, and handle edge cases such as zero radius (a point can only be inside if it exactly equals the center) and points exactly on the surface (these are not considered inside).
#include <cassert>
#include <cmath>

// Assume the function is declared above.

int main() {
    // Point at center is inside any positive radius sphere.
    assert(isInsideSphere(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5.0) == true);
    
    // Point on the surface is not strictly inside.
    assert(isInsideSphere(5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5.0) == false);
    
    // Point clearly outside.
    assert(isInsideSphere(10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5.0) == false);
    
    // Point just inside (distance less than radius).
    assert(isInsideSphere(4.999, 0.0, 0.0, 0.0, 0.0, 0.0, 5.0) == true);
    
    // Point just outside (distance greater than radius).
    assert(isInsideSphere(5.001, 0.0, 0.0, 0.0, 0.0, 0.0, 5.0) == false);
    
    // Non-axis aligned point: inside a sphere centered at (1,2,3) with radius 2.
    assert(isInsideSphere(1.0, 2.0, 4.5, 1.0, 2.0, 3.0, 2.0) == true);
    
    // Exactly at radius 1 away from center (surface of sphere radius 1).
    assert(isInsideSphere(1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0) == false);
    
    // Zero radius sphere: only point exactly at center would be inside, but strict inequality excludes it.
    assert(isInsideSphere(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0) == false);
    
    // Very large radius and point near origin.
    assert(isInsideSphere(1e6, 1e6, 1e6, 0.0, 0.0, 0.0, 1e7) == true);
    
    // Negative radius (we treat as invalid; but since radiusSq is positive, point far away might still pass incorrectly). We avoid negative radius in practice. Here we just test that a negative radius never returns true for a point far away because distance squared > radiusSq (positive still).
    assert(isInsideSphere(0.0, 0.0, 0.0, 100.0, 0.0, 0.0, -5.0) == false);
    
    return 0;
}
#include <cmath>

/**
 * Determines whether a 3D point lies strictly inside a sphere.
 * @param x, y, z        Coordinates of the point.
 * @param cx, cy, cz     Coordinates of the sphere center.
 * @param radius         Radius of the sphere (must be non-negative).
 * @return true if distance from point to center is strictly less than radius.
 */
bool isInsideSphere(double x, double y, double z,
                    double cx, double cy, double cz,
                    double radius) {
    const double dx = x - cx;
    const double dy = y - cy;
    const double dz = z - cz;
    const double distSq = dx * dx + dy * dy + dz * dz;
    const double radiusSq = radius * radius;
    return distSq < radiusSq;
}
// The solution computes the squared Euclidean distance from the point to the sphere center: `dx = x - cx`, `dy = y - cy`, `dz = z - cz`, then `distSq = dx*dx + dy*dy + dz*dz`. The point is inside if `distSq < radius*radius`. Using squared distances avoids unnecessary square-root computation and improves numeric stability. Edge cases include: if `radius < 0`, treat it as invalid — but per the task we assume radius is non-negative; if `radius == 0`, the condition becomes `distSq < 0.0`, which is never true (since `distSq` is non-negative), so only an exact match at the center would be inside, but in floating-point arithmetic an exact match is rare; we keep the strict inequality, so a point exactly at the center with radius 0 returns `false` (since `distSq == 0` and `radiusSq == 0`). For points exactly on the sphere surface, `distSq == radiusSq`, so they return `false`. Time complexity is O(1), space complexity O(1). No special handling for very large coordinates is needed because double precision gives about 15–17 decimal digits, and we avoid overflow by using `dx*dx` which is fine for typical coordinate ranges; if coordinates are extremely large (near `1e154`), `dx*dx` might overflow to infinity, but that is an extreme edge case outside typical problem scope.
