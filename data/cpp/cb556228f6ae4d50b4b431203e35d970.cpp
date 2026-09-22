// Given two points \(A\) and \(B\) in the plane with integer or real coordinates, write a C++ function `double shortestPathLength(pt A, pt B)` that returns the length of the shortest path from \(A\) to \(B\) if you are allowed to travel only along straight segments inside the circle centered at the origin with radius equal to the maximum distance from the origin among the two points, but you may travel radially inward/outward freely only up to the smaller radius. More precisely: there is a "safe" inner disk of radius \(r_1 = \min(|A|, |B|)\) where you can move freely in straight lines, and an outer annulus between radii \(r_1\) and \(r_2 = \max(|A|, |B|)\) where you can only move radially (along lines through the origin). The path may switch between radial and circular motion only at distances \(r_1\) or \(r_2\). Compute the minimal total path length. The function should work for any two points (including origin, coincident points, and points on the same ray). Return the result as a double with high precision.

#include <cassert>
#include <cmath>
#include <iostream>

// (pt struct and solution function as above)

int main() {
    // Same point at origin
    assert(std::fabs(shortestPathLength(pt(0,0), pt(0,0)) - 0.0) < 1e-9);

    // One point at origin, another at distance 5
    assert(std::fabs(shortestPathLength(pt(0,0), pt(3,4)) - 5.0) < 1e-9);

    // Points on same ray, same side: distances 2 and 6, angle 0
    assert(std::fabs(shortestPathLength(pt(2,0), pt(6,0)) - 4.0) < 1e-9);

    // Points on same ray but opposite sides: distances 2 and 6, angle pi
    assert(std::fabs(shortestPathLength(pt(2,0), pt(-6,0)) - (4.0 + 2.0*2.0*std::sin(3.141592653589793/2.0))) < 1e-9);
    // That is 4 + 4 = 8

    // Points at same distance 5, angle 90 degrees (pi/2)
    pt A(5,0), B(0,5);
    double r1 = 5.0, r2 = 5.0;
    double delta = 3.141592653589793 / 2.0;
    double expected = (r2 - r1) + 2.0*r1*std::sin(delta/2.0);
    assert(std::fabs(shortestPathLength(A, B) - expected) < 1e-9);

    // Points with different radii, angle 60 degrees
    pt C(3,0);      // r=3
    pt D(0,8);      // r=8, angle 90 deg from C
    r1 = 3.0; r2 = 8.0;
    delta = 3.141592653589793 / 2.0;
    expected = (r2 - r1) + 2.0*r1*std::sin(delta/2.0);
    assert(std::fabs(shortestPathLength(C, D) - expected) < 1e-9);

    // Points with radius 0 and 0 already covered; test nearly antipodal
    pt E(1,0), F(-1,0.0001);
    double val = shortestPathLength(E, F);
    assert(val > 1.9999 && val < 2.0001); // r1=r2=1, delta nearly pi, chord ~2

    // Test with r1=0 (origin) already done; test r1=r2=0 covered.

    // Test with same ray but different distances, delta=0
    assert(std::fabs(shortestPathLength(pt(1,2), pt(2,4)) - (std::hypot(2,4)-std::hypot(1,2))) < 1e-9);

    // Test with different radii and angle 180 degrees: e.g., r1=2, r2=5
    pt G(2,0), H(-5,0);
    r1 = 2.0; r2 = 5.0; delta = 3.141592653589793;
    expected = (r2 - r1) + 2.0*r1*std::sin(delta/2.0); // 3 + 4 = 7
    assert(std::fabs(shortestPathLength(G, H) - 7.0) < 1e-9);

    std::cout << "All tests passed.\n";
    return 0;
}

#include <cmath>
#include <algorithm>

struct pt {
    double x, y;
    pt(double x = 0, double y = 0) : x(x), y(y) {}
    pt operator+(const pt& p) const { return pt(x + p.x, y + p.y); }
    pt operator-(const pt& p) const { return pt(x - p.x, y - p.y); }
    double operator*(const pt& p) const { return x * p.x + y * p.y; }  // dot
    double operator%(const pt& p) const { return x * p.y - y * p.x; }  // cross
};

// Compute the shortest path length from A to B given radial-only motion
// in the annulus between radii r1 and r2, and free straight-line motion
// inside the inner disk of radius r1.
double shortestPathLength(const pt& A, const pt& B) {
    const double pi = std::acos(-1.0);
    double rA = std::hypot(A.x, A.y);
    double rB = std::hypot(B.x, B.y);
    double r1 = std::min(rA, rB);
    double r2 = std::max(rA, rB);

    // If both points are at the origin, distance is zero.
    if (r1 == 0.0 && r2 == 0.0) return 0.0;

    // If one point is at the origin, no inner disk motion needed.
    if (r1 == 0.0) return r2;

    // Compute the absolute angular difference between A and B.
    // Use atan2 of cross and dot to get an angle in (-pi, pi].
    double cross = A % B;
    double dot = A * B;
    double delta = std::atan2(cross, dot);
    delta = std::fabs(delta);  // now in [0, pi]

    // Inside the inner disk, the shortest path is a straight chord:
    // length = 2 * r1 * sin(delta / 2). The radial segment adds (r2 - r1).
    return (r2 - r1) + 2.0 * r1 * std::sin(delta / 2.0);
}

// The key insight is to decompose the path into two parts: an outer radial segment from the farther point directly to the inner circle along a radius, and then motion inside the inner disk. The inner disk allows free straight-line movement, so the best is to go from the point on the inner circle (obtained by radial projection of the farther point) to the nearer point. However, because the inner disk is convex, the shortest path between two points on its boundary and a point inside is just the straight segment, but we must account for the fact that we can choose where on the inner circle we enter. Let \(r_1 \le r_2\) be the distances from the origin. The path: start at the farther point (distance \(r_2\)), walk radially inward to a point \(P\) on the circle of radius \(r_1\), then walk straight to the nearer point (distance \(r_1\) from origin). The radial segment length is \(r_2 - r_1\). The straight segment from \(P\) to the nearer point has length depending on the angle between \(P\) and the nearer point. To minimize, we choose \(P\) to be the point on the inner circle that minimizes distance to the nearer point, but we also have a constraint: the angle between the radial line from origin to the farther point and the radial line to \(P\) can vary. Actually, the optimal is achieved by taking \(P\) on the inner circle such that the path from \(P\) to the nearer point is tangent? Wait, let's think clearly.
//
// The problem reduces to: given two radii \(r_1 \le r_2\), and an angle \(\delta\) between the two radial directions (where \(\delta \in [0, \pi]\) is the absolute angular difference between vectors A and B). The path: from outer point at radius \(r_2\), go radially to a point at radius \(r_1\) at some angle \(\theta\) (which can be any). Then go straight to the inner point (which is at radius \(r_1\) and angle 0 after rotation). The total length = \((r_2 - r_1) + \sqrt{r_1^2 + r_1^2 - 2 r_1 r_1 \cos(\theta - 0)}? Wait, the inner point is at radius \(r_1\), so the distance from P (which is at radius \(r_1\)) to the inner point is \(2 r_1 \sin(|\theta|/2)\) if we set the inner point at angle 0. But we can also choose to not start the radial motion at the outer point's radial direction? Actually we must start at the outer point exactly, so the radial segment must be along the ray from origin through that outer point. So the point P must lie on the ray from origin through the outer point, at radius \(r_1\). So \(\theta = \delta\) (the angle of the outer point relative to inner point). Thus, the path length is \((r_2 - r_1) + 2 r_1 \sin(\delta/2)\). This is the formula from the original snippet: `min(2.0, delta)*r1 + r2-r1` where `delta` is in radians? Actually the snippet uses `delta` as angle in radians and takes `min(2.0, delta)`, which suggests that if the angular difference is larger than 2 radians, it might be better to go around the circle at radius \(r_1\) instead of cutting across? Wait, the snippet computes `min(2.0, delta)*r1` – that would be `min(2, delta)` times r1, not `2*sin(delta/2)`. Let's re-examine.
//
// The snippet: `double delta = mod(angle(a, pt(), b));` – that returns angle in radians between vectors a and b. Then `min(2.0, delta)*r1 + r2-r1`. That suggests they use the chord length approximation? Actually, the chord length between two points on a circle of radius r1 separated by angle delta is \(2 r1 \sin(\delta/2)\). The snippet uses `min(2.0, delta)` which is not the chord length. Perhaps they approximate the arc length? Actually if you travel along the circle of radius r1 from the radial projection of the outer point to the inner point, the arc length is \(r1 \cdot \delta\). But you could also go straight across the disk, which is the chord \(2 r1 \sin(\delta/2)\). For small delta, the chord is shorter than arc; for larger delta, up to delta = 2, the chord is less than or equal to the arc? Actually arc length = r1*delta, chord = 2r1 sin(delta/2). For delta from 0 to pi, chord <= arc? Let's check: at delta=pi, chord = 2r1, arc = pi r1 ≈ 3.14 r1, so chord is smaller. So chord is always <= arc for delta in [0,pi]. So why would they take min(2.0, delta)? That is odd. The snippet's formula `min(2.0, delta)*r1` would give for delta=pi: 2*r1, which is the chord length? Actually chord length for delta=pi is 2r1, yes. For delta=1 radian, min(2,1)=1, so length = r1, but true chord = 2r1 sin(0.5) ≈ 0.958 r1, so that's an overestimate. The snippet seems to be a heuristic or maybe a different problem interpretation. But our task should be precise. The correct minimal path: from outer point to inner point, you have to go radially from radius r2 to r1 along the outer point's ray, then inside the disk you can go straight. So the distance from the point at (r1, delta) on the inner circle to the point at (r1, 0) is the chord length \(2 r1 \sin(\delta/2)\), where delta is the angular difference between A and B. So the total is \(r2 - r1 + 2 r1 \sin(\delta/2)\). However, note that if delta = 0 (same ray), sin(0)=0, so total = r2-r1 (radial only). If delta = pi (opposite sides), chord = 2r1, so total = r2 - r1 + 2r1 = r2 + r1. That makes sense. Edge cases: if one point is at origin (r1=0), then the path is just the radial distance to the other point, and our formula gives (r2-0) + 0 = r2, correct. If both points at origin, r1=r2=0, delta defined as angle? atan2(0,0) is undefined, so handle that. If points are coincident and not origin, r1=r2, delta=0, total=0. If points are on same ray but different distances, delta=0, total = r2-r1. If points are antipodal (delta=pi), total = r2-r1+2r1 = r2+r1. The formula works for delta in [0,pi]. Since we take absolute angle difference modulo 2pi, we should take the minimal angle between the two vectors, which is in [0,pi]. So compute delta = acos( dot/(|a||b|) ) or use atan2 with cross/dot and take absolute, then if > pi, subtract from 2pi? Actually angle() function in snippet uses atan2(cross, dot) which returns in (-pi, pi], and absolute gives [0,pi]. So delta in [0,pi]. So correct formula: total = r2 - r1 + 2*r1*sin(delta/2). But note: if r1 is very small, the chord term is negligible. Also, if delta is very small, chord ~ r1*delta, which is less than radial? Actually chord ~ r1*delta, so total ~ r2-r1 + r1*delta, which is the same as going around the circle but slightly less? Actually if you go along the circle of radius r1 from the radial projection of outer point to inner point, the arc length is r1*delta. The chord is approximately r1*delta - (r1*delta^3)/24, so chord is slightly smaller. So always chord is better than arc for delta in [0,pi]. So the correct minimal path length is \(r_2 - r_1 + 2 r_1 \sin(\delta/2)\). The snippet's `min(2.0, delta)*r1` is an approximation that underestimates for some deltas? Actually for delta=1, min(2,1)=1, gives r1, but chord=0.958r1, so snippet overestimates. For delta=3 (which is > pi? but delta in [0,pi], max ~3.1416, so min(2,3)=2, gives 2r1, but chord=2r1 sin(1.5)=2r1*0.997 ≈ 1.994r1, slightly less. So snippet overestimates a bit. So our task should use the exact formula. I'll design the task to compute the exact minimal length using the chord inside the inner disk.
//
// Alternative interpretation: The snippet might be from a problem where you can travel inside the entire disk of radius r2 (the larger radius) but with the constraint that you can only travel radially in the outer annulus? Actually let's re-read the original code: it does `min(2.0, delta)*r1 + r2-r1`. That looks like they are taking the minimum of 2 and delta (angle) and multiplying by r1. They might be approximating the arc length as delta*r1 but capping at 2*r1 (diameter). That suggests the path: you go radially from outer to inner, then you can go along the circle of radius r1 from the point directly below outer to the inner point, but if the angle is large, you might go through the center? Actually going through the center would be distance 2*r1? No, from point on circle to opposite point through center is 2*r1, but chord is shorter for delta<pi? Actually chord is always <= 2*r1, and for delta=pi, chord=2*r1. So the snippet uses `min(2, delta)*r1` which equals 2*r1 for delta>=2, but chord at delta=2 is 2*r1*sin(1) ≈ 1.68*r1, so that's an overestimate. So the snippet is not exact. Our task should be precise.
//
// For the task, I'll define: You are on a planet with a spherical city centered at origin. You can move freely inside a disk of radius r1 (the inner city), but in the annulus between r1 and r2 you can only move along radial lines (like roads). The two points A and B are at distances r1 and r2 (with r1 <= r2). Find the shortest path. The optimal is: go radially from the outer point to the inner circle, then take the straight chord inside the inner disk to the inner point. So the formula is as above. Edge cases: if one point is at origin, r1=0, chord term is 0, path length = r2 (just radial). If both at origin, return 0. If both non-origin and delta=0, return r2-r1. If delta close to pi, chord close to 2r1. We'll implement using precise double.
//
// Time complexity O(1), space O(1). No loops. The main algorithm: compute distances, order them, compute angle via atan2 of cross/dot, take absolute, clamp to [0, pi] (already). Then compute.
//
// I'll write the solution function with a struct pt as in the snippet.
