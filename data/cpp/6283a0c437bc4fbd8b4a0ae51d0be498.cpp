// Write a standalone C++ function that computes the area of intersection between a disk and an annulus (ring) centered at the origin, given the annulus inner radius `r_inner`, annulus outer radius `r_outer`, and disk radius `r_disk`. The function should return the exact area of the set of points that are both inside the disk (distance from origin ≤ r_disk) and inside the annulus (r_inner ≤ distance ≤ r_outer). Handle all edge cases, including when the disk is entirely inside the annulus, entirely outside, partially overlapping, or when the annulus has zero thickness. The function signature should be `double annulus_disk_intersection_area(double r_inner, double r_outer, double r_disk)`. Ensure all radii are non-negative and that `r_inner ≤ r_outer`; otherwise, return 0.0 for invalid inputs.
#include <cassert>
#include <cmath>

// Declare the function prototype (or rely on header inclusion in tests).
double annulus_disk_intersection_area(double r_inner, double r_outer, double r_disk);

int main()
{
    const double pi = 4.0 * std::atan(1.0);
    
    // Disk entirely inside annulus (r_disk between inner and outer) -> area is π r_disk² - π r_inner²? Wait: if r_disk > r_inner, then 
    // intersection is annulus from r_inner to r_disk. Example: inner=1, outer=5, disk=3 => area π(9 - 1)=8π.
    assert(std::abs(annulus_disk_intersection_area(1.0, 5.0, 3.0) - 8.0 * pi) < 1e-12);
    
    // Annulus entirely inside disk (r_outer < r_disk) -> area π(r_outer² - r_inner²).
    assert(std::abs(annulus_disk_intersection_area(0.5, 2.0, 10.0) - (4.0 - 0.25) * pi) < 1e-12);
    
    // Disk exactly matching inner radius (upper == r_inner) -> area 0.
    assert(std::abs(annulus_disk_intersection_area(2.0, 5.0, 2.0) - 0.0) < 1e-12);
    
    // Disk smaller than inner radius (disk does not reach the annulus) -> area 0.
    assert(std::abs(annulus_disk_intersection_area(3.0, 5.0, 2.0) - 0.0) < 1e-12);
    
    // Annulus with inner radius 0 (i.e., a disk) and r_disk smaller -> intersection is a disk of radius r_disk.
    assert(std::abs(annulus_disk_intersection_area(0.0, 10.0, 4.0) - 16.0 * pi) < 1e-12);
    
    // Degenerate annulus (zero thickness) correctly handled: r_inner == r_outer.
    assert(std::abs(annulus_disk_intersection_area(3.0, 3.0, 5.0) - 0.0) < 1e-12);
    
    // Invalid input: negative radius.
    assert(std::abs(annulus_disk_intersection_area(-1.0, 5.0, 3.0) - 0.0) < 1e-12);
    
    // Invalid input: r_inner > r_outer.
    assert(std::abs(annulus_disk_intersection_area(5.0, 2.0, 3.0) - 0.0) < 1e-12);
    
    // Disk radius 0.
    assert(std::abs(annulus_disk_intersection_area(0.0, 5.0, 0.0) - 0.0) < 1e-12);
    
    // Large radii: disk completely covers annulus, inner radius 0? Actually inner=0, outer=100, disk=1000 -> full annulus area π*10000.
    assert(std::abs(annulus_disk_intersection_area(0.0, 100.0, 1000.0) - 10000.0 * pi) < 1e-6);
    
    return 0;
}
#include <cmath>
#include <algorithm>

// Compute the area of the intersection of a disk (radius r_disk) and an annulus
// (inner radius r_inner, outer radius r_outer) both centered at the origin.
// Returns 0.0 for invalid inputs (negative radii, r_inner > r_outer, or r_disk <= 0).
double annulus_disk_intersection_area(double r_inner, double r_outer, double r_disk)
{
    // Validate inputs: all radii must be non-negative, and r_inner <= r_outer.
    if (r_inner < 0.0 || r_outer < 0.0 || r_disk < 0.0 || r_inner > r_outer)
        return 0.0;
    
    // A disk of radius 0 has area 0.
    if (r_disk == 0.0)
        return 0.0;
    
    // The intersection is the set of points with radius in [r_inner, min(r_outer, r_disk)].
    const double upper = std::min(r_outer, r_disk);
    if (upper <= r_inner)
        return 0.0;
    
    // Area of annulus segment: π (upper² - r_inner²).
    const double pi = 4.0 * std::atan(1.0);
    return pi * (upper * upper - r_inner * r_inner);
}
// The problem reduces to finding the area of the radial range `[r_inner, min(r_outer, r_disk)]` intersected with the plane. For a set of points with radial coordinate `r` from the origin, the area of all points with radius between `a` and `b` (where `0 ≤ a ≤ b`) is `π(b² - a²)`. Therefore, the intersection of a disk (radius ≤ r_disk) and an annulus (r_inner ≤ radius ≤ r_outer) is simply the set of points with radius in `[r_inner, min(r_outer, r_disk)]`. So the area is `π( (min(r_outer, r_disk))² - r_inner² )` if `r_inner ≤ min(r_outer, r_disk)`, else 0. However, be careful: if the annulus is degenerate (r_inner > r_outer) or any radius is negative, return 0.0. Also handle the case where r_inner = 0, which means the annulus is actually a disk of radius r_outer (and the intersection is just the disk of radius min(r_outer, r_disk)). Edge cases: if r_disk = 0, only the origin is inside the disk, but a single point has area 0, so result is 0 unless r_inner = 0 and r_outer ≥ 0 (but even then the disk of radius 0 is just a point, area 0). Actually, if r_disk = 0, the disk is a point at the origin, area 0, so the intersection area is 0 regardless. So we can compute: let `upper = min(r_outer, r_disk)`. If `upper <= r_inner` or `r_disk <= 0` or `r_inner > r_outer` or any radius < 0, return 0.0. Otherwise return `M_PI * (upper*upper - r_inner*r_inner)`. The time complexity is O(1) and space is O(1). Need to include `<cmath>` for M_PI (if not defined, use `4.0 * atan(1.0)` ) and also handle extreme values to avoid overflow? Since we are just squaring doubles, fine. Also consider very large radii that might cause infinity but that's acceptable.
