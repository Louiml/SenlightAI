// Write a C++ function that determines whether two axis-aligned spheres in 3D space intersect (i.e., overlap or touch). The function must accept two spheres defined by their center coordinates (as three floating-point values) and radii, and return `true` if they intersect (including exactly touching) and `false` otherwise. The spheres are considered non-intersecting only if the distance between their centers is strictly greater than the sum of their radii. The function should be standalone, use only standard library components, and have a descriptive name such as `spheresIntersect`. It must handle edge cases such as identical spheres, one sphere inside another without touching, zero radius (point-like sphere), and negative radii (which should be treated as invalid, but for simplicity you may clamp them to zero or assume they are non-negative). Do not include a `main` function in the solution; just provide the function.

#include <cmath>
#include <cassert>

// Declaration from the solution.
bool spheresIntersect(double x1, double y1, double z1, double r1,
                      double x2, double y2, double z2, double r2);

int main() {
    // Identical spheres
    assert(spheresIntersect(0,0,0,1, 0,0,0,1) == true);
    // Far apart, no intersection
    assert(spheresIntersect(0,0,0,1, 10,0,0,1) == false);
    // Just touching externally (distance == sum)
    assert(spheresIntersect(0,0,0,1, 2,0,0,1) == true);
    // One inside another (concentric, different radii)
    assert(spheresIntersect(0,0,0,5, 0,0,0,1) == true);
    // One inside another but not concentric
    assert(spheresIntersect(0,0,0,5, 1,0,0,1) == true);
    // Point sphere touching a normal sphere
    assert(spheresIntersect(0,0,0,0, 2,0,0,2) == true);
    // Point sphere far from another sphere
    assert(spheresIntersect(0,0,0,0, 10,0,0,1) == false);
    // Negative radius treated as zero
    assert(spheresIntersect(0,0,0,-1, 0,0,0,1) == true);
    // 3D separation along y and z axes
    assert(spheresIntersect(0,0,0,1, 0,3,4,1) == false); // distance = 5 > 2
    assert(spheresIntersect(0,0,0,1, 0,1,0,1) == true);  // distance = 1 <= 2
    return 0;
}

#include <cmath>

// Determine if two spheres (by center coordinates and radii) intersect.
// Returns true if they overlap or touch, false otherwise.
bool spheresIntersect(
    double x1, double y1, double z1, double r1,
    double x2, double y2, double z2, double r2)
{
    // Clamp negative radii to zero to avoid nonsensical comparisons.
    if (r1 < 0.0) r1 = 0.0;
    if (r2 < 0.0) r2 = 0.0;

    // Compute squared distance to avoid a square root for performance.
    double dx = x1 - x2;
    double dy = y1 - y2;
    double dz = z1 - z2;
    double distanceSquared = dx * dx + dy * dy + dz * dz;

    double radiusSum = r1 + r2;

    // Compare squared distance with squared radius sum.
    // Use <= to include touching (distance == sum).
    return distanceSquared <= radiusSum * radiusSum;
}

// The core algorithm is straightforward: compute the Euclidean distance between the two sphere centers using the standard formula \(\sqrt{(x_1-x_2)^2 + (y_1-y_2)^2 + (z_1-z_2)^2}\), then compare this distance with the sum of the radii. If the distance is less than or equal to the sum of the radii, the spheres intersect; otherwise, they do not. The edge cases to handle are: (1) When one sphere is inside another but not touching, the distance is less than the absolute difference of radii, but since we compare with the sum, this will correctly return `true` (they do intersect in the sense of overlapping region); (2) When the spheres just touch externally, the distance equals the sum of radii, which should return `true`; (3) Identical spheres have distance 0 and sum of radii positive, so return `true`; (4) If a radius is negative, it's physically invalid, but we can treat it as zero by clamping to avoid negative sums that would break the comparison—though the problem likely assumes valid input; (5) Floating-point precision: using `<=` is safer for "touching" to avoid floating-point rounding issues, but if strict "overlap only" is desired, use `<`. The task explicitly says "including exactly touching", so use `distance <= sumRadii`. Time complexity is O(1) with constant operations, and space complexity is O(1).
