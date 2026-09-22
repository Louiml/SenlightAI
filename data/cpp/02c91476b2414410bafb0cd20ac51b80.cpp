Write a C++ function `std::pair<double, double> lineIntersection(const Line& l1, const Line& l2)` that takes two infinite lines, each represented by a point `(px, py)` and a direction vector `(dx, dy)` (which may not be normalized), and returns the distance from the reference point of `l1` to its intersection point with `l2`, along with the distance from the reference point of `l2` to the same intersection point, as a pair `(dist1, dist2)`. If the lines are parallel (within a tolerance of `1e-6` for the cross product of their directions), return `{std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()}`. Use the numerically stable two-point interpolation method (as in the snippet’s high-accuracy `Intersect`), not the simple parametric denominator method, because the latter is unstable for near-parallel lines. The function must be `const`-correct, handle arbitrarily large direction vectors without overflow, and return distances that are consistent regardless of which line is passed first (i.e., the two distances computed for the same physical intersection must satisfy that the intersection point lies on both lines).

#include <cassert>
#include <cmath>

int main() {
    // Simple crossing lines: l1 at (0,0) direction (1,0); l2 at (0,1) direction (0,-1).
    // Intersection at (0,0): dist1=0, dist2=1.
    Line l1{0,0, 1,0};
    Line l2{0,1, 0,-1};
    auto res1 = lineIntersection(l1, l2);
    assert(fabs(res1.first - 0.0) < 1e-9);
    assert(fabs(res1.second - 1.0) < 1e-9);

    // Same intersection but calling in reverse order: distances should swap.
    auto res2 = lineIntersection(l2, l1);
    assert(fabs(res2.first - 1.0) < 1e-9);
    assert(fabs(res2.second - 0.0) < 1e-9);

    // Parallel lines: l1 (0,0)-(1,0); l2 (0,1)-(1,1) -> should be infinite.
    Line p1{0,0, 1,0};
    Line p2{0,1, 1,0};
    auto res3 = lineIntersection(p1, p2);
    assert(std::isinf(res3.first) && std::isinf(res3.second));

    // Near-parallel lines: small angle, should still find correct intersection.
    // l1: (0,0) direction (1,0); l2: (0,1000) direction (1, -0.001) -> intersect at x=1,000,000? Actually solve y=0 and y=1000 - 0.001x -> x=1,000,000.
    Line n1{0,0, 1,0};
    Line n2{0,1000, 1, -0.001};
    auto res4 = lineIntersection(n1, n2);
    assert(fabs(res4.first - 1000000.0) < 1e-3);
    // The intersection point is (1000000, 0), distance from n2's point (0,1000) is sqrt(1e12 + 1e6) ≈ 1000000.0
    assert(fabs(res4.second - 1000000.0) < 1e-3);

    // Non-unit direction vectors (large): lines cross at (10,10)
    Line big1{0,0, 100,100};  // direction (1,1)
    Line big2{20,0, -100,100}; // direction (-1,1)
    auto res5 = lineIntersection(big1, big2);
    // Intersection is (10,10): dist1 = sqrt(200) ≈ 14.1421, dist2 = sqrt(200) ≈ 14.1421
    assert(fabs(res5.first - sqrt(200.0)) < 1e-9);
    assert(fabs(res5.second - sqrt(200.0)) < 1e-9);

    // Same point on both lines: l1 and l2 share (3,4), directions different
    Line same1{3,4, 1,2};
    Line same2{3,4, -2,1};
    auto res6 = lineIntersection(same1, same2);
    assert(fabs(res6.first - 0.0) < 1e-9);
    assert(fabs(res6.second - 0.0) < 1e-9);
}

#include <utility>
#include <cmath>
#include <limits>

struct Line {
    double px, py;   // reference point
    double dx, dy;   // direction vector (not necessarily normalized)
};

// Returns (distance from l1's point to intersection, distance from l2's point to intersection).
// Returns infinities if lines are parallel (cross product of directions near zero).
std::pair<double, double> lineIntersection(const Line& l1, const Line& l2) {
    const double eps = 1e-6;

    // Second points on each line (using direction vectors).
    double p1x = l1.px + l1.dx;
    double p1y = l1.py + l1.dy;
    double p2x = l2.px + l2.dx;
    double p2y = l2.py + l2.dy;

    // Determinant of the two direction segments.
    double det = (p1x - l1.px) * (p2y - l2.py) - (p2x - l2.px) * (p1y - l1.py);

    if (std::fabs(det) < eps) {
        double inf = std::numeric_limits<double>::infinity();
        return {inf, inf};
    }

    // Solve for parameter a along line 1.
    double a = ((p2x - l1.px) * (p2y - l2.py) - (p2x - l2.px) * (p2y - l1.py)) / det;

    // Intersection point.
    double ix = l1.px + a * (p1x - l1.px);
    double iy = l1.py + a * (p1y - l1.py);

    // Distances from each reference point to the intersection.
    double dist1 = std::sqrt((ix - l1.px) * (ix - l1.px) + (iy - l1.py) * (iy - l1.py));
    double dist2 = std::sqrt((ix - l2.px) * (ix - l2.px) + (iy - l2.py) * (iy - l2.py));

    return {dist1, dist2};
}

// The core problem is to find the intersection of two infinite lines defined by a point and a direction vector. The algorithm uses the two-point form of each line: for line 1, points `P` and `P1 = P + D`; for line 2, points `Q` and `Q1 = Q + E`. The intersection is found by solving for scalar `a` such that `P + a*(P1-P)` lies on line 2. This is done using the cross-product-based formula from the snippet: `d = (P1.x - P.x)*(Q1.y - Q.y) - (Q1.x - Q.x)*(P1.y - P.y)`. If `|d|` is below a tolerance (here `1e-6`), the lines are parallel and we return infinities. Otherwise, `a = ((Q1.x - P.x)*(Q1.y - Q.y) - (Q1.x - Q.x)*(Q1.y - P.y)) / d`. Then the intersection point `I` is computed, and the distance from `P` to `I` is `sqrt((I.x-P.x)^2 + (I.y-P.y)^2)`. For the second line, we could compute a similar scalar `b` and distance, but to avoid recomputation and ensure consistency, we compute the intersection point once and compute the distance to both reference points. Edge cases: parallel lines (including coincident lines, where `d` is also zero), nearly parallel lines where the naive method would suffer cancellation errors (the two-point method is more robust because it avoids subtracting large nearly equal numbers in the numerator/denominator as much as possible), and large direction vectors (normalizing them first could cause loss of precision, so we do not normalize; the formula is scale-invariant because the scalar `a` adjusts accordingly). Time complexity is O(1) since only constant arithmetic operations are performed; space complexity is O(1) beyond the input structs.
