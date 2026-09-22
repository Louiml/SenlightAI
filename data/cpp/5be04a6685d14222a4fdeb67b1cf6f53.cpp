/*
Implement a C++ function `timeOfImpactConservativeAdvancement` that, given two convex shapes (represented by a simplified `ConvexShape` class with a `support` function returning the farthest point in a given direction), their initial and final transforms (represented by a minimal `Transform` class containing an origin vector and a 3x3 rotation matrix), and a `ToIConfig` struct specifying maximum iterations and an allowed penetration tolerance, returns a `ToIResult` containing the fraction of motion (in `[0,1]`) when the shapes first touch, the contact normal, and the hit point. The function must implement the conservative advancement algorithm: compute relative linear velocity, initialize the simplex solver (here a simple vector-based set storing support points), iteratively find the closest point between the Minkowski difference of the two shapes at the current interpolated transforms using a simplified GJK sub-simplex (support points and projection onto the simplex), advance the interpolation parameter when the supporting plane separates the origin from the relative velocity direction, and terminate when the squared distance falls below `1e-4` or max iterations reach 32. If the motion is separating (dot product of closest vector with relative velocity is non-negative), or if the final normal points away from the relative motion beyond the allowed penetration, return `hasHit=false`; otherwise return `true` with the interpolated fraction, normalized normal, and hit point (the support point on shape B). Edge cases include zero relative velocity, degenerate simplex (zero-area triangle), and cases where the origin is inside the Minkowski difference at start (fraction 0 but no reliable normal—return false). The function must be `const`-correct and use only standard library includes.
*/
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

// Minimal 3D vector with dot and cross products.
struct Vec3 {
    double x, y, z;
    Vec3(double x=0, double y=0, double z=0) : x(x), y(y), z(z) {}
    Vec3 operator+(const Vec3& o) const { return {x+o.x, y+o.y, z+o.z}; }
    Vec3 operator-(const Vec3& o) const { return {x-o.x, y-o.y, z-o.z}; }
    Vec3 operator*(double s) const { return {x*s, y*s, z*s}; }
    double dot(const Vec3& o) const { return x*o.x + y*o.y + z*o.z; }
    Vec3 cross(const Vec3& o) const { return {y*o.z - z*o.y, z*o.x - x*o.z, x*o.y - y*o.x}; }
    double length2() const { return dot(*this); }
    double length() const { return std::sqrt(length2()); }
    Vec3 normalized() const { double l = length(); return {x/l, y/l, z/l}; }
};

// Minimal 3x3 rotation matrix (stored as three column vectors).
struct Mat3 {
    Vec3 col0, col1, col2;
    Mat3() : col0{1,0,0}, col1{0,1,0}, col2{0,0,1} {}
    Vec3 operator*(const Vec3& v) const {
        return col0*v.x + col1*v.y + col2*v.z;
    }
};

// Transform: rotation and translation.
struct Transform {
    Mat3 basis;
    Vec3 origin;
    Vec3 operator()(const Vec3& localPoint) const {
        return basis * localPoint + origin;
    }
};

// Convex shape interface: support function returns the farthest point in direction d.
struct ConvexShape {
    virtual ~ConvexShape() = default;
    virtual Vec3 support(const Vec3& d) const = 0;
};

// A simple sphere shape for testing.
struct SphereShape : ConvexShape {
    double radius;
    explicit SphereShape(double r) : radius(r) {}
    Vec3 support(const Vec3& d) const override {
        double l = d.length();
        if (l < 1e-12) return {0,0,0};
        return d * (radius / l);
    }
};

// Configuration for time-of-impact.
struct ToIConfig {
    int maxIterations = 32;
    double allowedPenetration = 0.0;
    double epsilon = 1e-4;
};

// Result structure.
struct ToIResult {
    bool hasHit = false;
    double fraction = 0.0;
    Vec3 normal{0,0,0};
    Vec3 hitPoint{0,0,0};
};

// Simple simplex solver supporting up to 3 points.
class SimplexSolver {
public:
    void reset() { points.clear(); }
    bool inSimplex(const Vec3& p) const {
        for (const auto& q : points) {
            if ((p - q).length2() < 1e-12) return true;
        }
        return false;
    }
    void addVertex(const Vec3& p, const Vec3& hitA, const Vec3& hitB) {
        points.push_back(p);
        hitPointsA.push_back(hitA);
        hitPointsB.push_back(hitB);
    }
    // Compute the closest point to the origin on the convex hull of points.
    // Returns true if a valid closest point was found, and sets `closest`.
    bool closest(Vec3& closest) {
        size_t n = points.size();
        if (n == 0) return false;
        if (n == 1) { closest = points[0]; return true; }
        if (n == 2) {
            // Segment from p0 to p1. Find closest point to origin.
            Vec3 p0 = points[0], p1 = points[1];
            Vec3 d = p1 - p0;
            double denom = d.length2();
            if (denom < 1e-12) { closest = p0; return true; }
            double t = -p0.dot(d) / denom;
            t = std::max(0.0, std::min(1.0, t));
            closest = p0 + d * t;
            return true;
        }
        if (n >= 3) {
            // Triangle: p0, p1, p2. Compute closest point on the triangle to origin.
            Vec3 p0 = points[0], p1 = points[1], p2 = points[2];
            // First, closest on edges.
            auto closestOnSegment = [](const Vec3& a, const Vec3& b) {
                Vec3 d = b - a;
                double denom = d.length2();
                if (denom < 1e-12) return a;
                double t = -a.dot(d) / denom;
                t = std::max(0.0, std::min(1.0, t));
                return a + d * t;
            };
            Vec3 best = closestOnSegment(p0, p1);
            double bestDist2 = best.length2();
            auto check = [&](const Vec3& cand) {
                double d2 = cand.length2();
                if (d2 < bestDist2) { bestDist2 = d2; best = cand; }
            };
            check(closestOnSegment(p1, p2));
            check(closestOnSegment(p2, p0));
            // Then, closest on the face. Use barycentric coordinates projection.
            Vec3 n = (p1-p0).cross(p2-p0);
            double n2 = n.length2();
            if (n2 > 1e-12) {
                // Project origin onto the plane through the triangle.
                Vec3 planePoint = p0;
                double t = -planePoint.dot(n) / n2;
                Vec3 proj = n * t; // since plane passes through origin? Actually plane equation n·x = n·p0.
                // More correct: the plane through p0 with normal n: n·(x - p0)=0, so n·x = n·p0.
                Vec3 projToPlane = n * (n.dot(p0) / n2);
                // Check if projection lies inside the triangle using barycentric.
                Vec3 p = projToPlane;
                Vec3 v0 = p1 - p0, v1 = p2 - p0, v2 = p - p0;
                double d00 = v0.dot(v0);
                double d01 = v0.dot(v1);
                double d11 = v1.dot(v1);
                double d20 = v2.dot(v0);
                double d21 = v2.dot(v1);
                double denom = d00*d11 - d01*d01;
                if (std::abs(denom) > 1e-12) {
                    double v = (d11*d20 - d01*d21) / denom;
                    double w = (d00*d21 - d01*d20) / denom;
                    double u = 1.0 - v - w;
                    if (u >= -1e-9 && v >= -1e-9 && w >= -1e-9) {
                        check(p);
                    }
                }
            }
            // Reduce to three points if we have more (keep the face).
            if (points.size() > 3) {
                points.resize(3);
                hitPointsA.resize(3);
                hitPointsB.resize(3);
            }
            closest = best;
            return true;
        }
        return false;
    }
    // Get the hit point on B for the closest point (simplified: use the last added point's B).
    Vec3 getHitPointB() const {
        if (hitPointsB.empty()) return {0,0,0};
        return hitPointsB.back();
    }
private:
    std::vector<Vec3> points;
    std::vector<Vec3> hitPointsA;
    std::vector<Vec3> hitPointsB;
};

// Linear interpolation between two vectors.
Vec3 lerp(const Vec3& a, const Vec3& b, double t) {
    return a + (b - a) * t;
}

// The main time-of-impact function.
ToIResult timeOfImpactConservativeAdvancement(
        const ConvexShape& shapeA,
        const ConvexShape& shapeB,
        const Transform& startA,
        const Transform& endA,
        const Transform& startB,
        const Transform& endB,
        const ToIConfig& config) {
    ToIResult result;
    SimplexSolver simplex;
    simplex.reset();

    Vec3 linVelA = endA.origin - startA.origin;
    Vec3 linVelB = endB.origin - startB.origin;

    double lambda = 0.0;
    Transform interpA = startA;
    Transform interpB = startB;

    Vec3 r = linVelA - linVelB;
    // Initial support direction along -r.
    Vec3 dirA = r * (-1.0);
    Vec3 supA = startA(shapeA.support(dirA));
    Vec3 supB = startB(shapeB.support(r));
    Vec3 v = supA - supB;

    int maxIter = config.maxIterations;
    double dist2 = v.length2();
    double epsilon = config.epsilon;
    bool hasResult = false;
    Vec3 n{0,0,0};

    while (dist2 > epsilon && maxIter > 0) {
        // Compute support at current interpolated transforms in direction -v.
        Vec3 dir = v * (-1.0);
        Vec3 supA_new = interpA(shapeA.support(dir));
        Vec3 supB_new = interpB(shapeB.support(v));
        Vec3 w = supA_new - supB_new;

        double VdotW = v.dot(w);
        if (lambda > 1.0) {
            result.hasHit = false;
            return result;
        }

        if (VdotW > 0.0) {
            double VdotR = v.dot(r);
            if (VdotR >= -1e-12) {
                // Moving away or parallel.
                result.hasHit = false;
                return result;
            } else {
                lambda = lambda - VdotW / VdotR;
                // Interpolate origins.
                interpA.origin = lerp(startA.origin, endA.origin, lambda);
                interpB.origin = lerp(startB.origin, endB.origin, lambda);
                // Update v to current w? The Bullet code reuses w but recomputes in next iteration.
                n = v;
                hasResult = true;
            }
        }

        if (!simplex.inSimplex(w)) {
            simplex.addVertex(w, supA_new, supB_new);
        }

        if (simplex.closest(v)) {
            dist2 = v.length2();
            hasResult = true;
        } else {
            dist2 = 0.0;
        }
        --maxIter;
    }

    result.fraction = lambda;
    if (n.length() > 1e-12) {
        result.normal = n.normalized();
    } else {
        result.normal = {0,0,0};
    }

    // Do not report if moving away from contact normal or penetration is allowed too much.
    if (result.normal.dot(r) >= -config.allowedPenetration) {
        result.hasHit = false;
        return result;
    }

    // Hit point from the simplex (the support point on B).
    result.hitPoint = simplex.getHitPointB();
    result.hasHit = true;
    return result;
}
#include <cassert>
#include <cmath>

int main() {
    // Two spheres moving directly toward each other along x-axis.
    SphereShape sphereA(1.0);
    SphereShape sphereB(1.0);

    // Shape A moves from x=-5 to x=0. Shape B is static at x=0.
    Transform startA; startA.origin = {-5,0,0};
    Transform endA; endA.origin = {0,0,0};
    Transform startB; startB.origin = {0,0,0};
    Transform endB; endB.origin = {0,0,0};

    ToIConfig cfg;
    ToIResult res = timeOfImpactConservativeAdvancement(sphereA, sphereB, startA, endA, startB, endB, cfg);
    // Distance between centers when they touch is 2.0; start distance is 5.0.
    // Fraction = (5-2)/5 = 0.6.
    assert(res.hasHit);
    assert(std::abs(res.fraction - 0.6) < 1e-3);
    // Normal should point from A to B (positive x).
    assert(res.normal.x > 0.0);
    assert(std::abs(res.normal.y) < 1e-6);
    assert(std::abs(res.normal.z) < 1e-6);
    // Hit point on B at about x=1.0 (surface of sphere B).
    assert(std::abs(res.hitPoint.x - 1.0) < 1e-2);

    // Moving away: A moves from x=-5 to x=-10, B static at x=0. Should have no hit.
    Transform startA2; startA2.origin = {-5,0,0};
    Transform endA2; endA2.origin = {-10,0,0};
    ToIResult res2 = timeOfImpactConservativeAdvancement(sphereA, sphereB, startA2, endA2, startB, endB, cfg);
    assert(!res2.hasHit);

    // No relative motion: both static, overlapping? Not overlapping, so no hit.
    Transform staticA; staticA.origin = {-3,0,0};
    Transform staticB; staticB.origin = {0,0,0};
    ToIResult res3 = timeOfImpactConservativeAdvancement(sphereA, sphereB, staticA, staticA, staticB, staticB, cfg);
    assert(!res3.hasHit);

    // Moving but never touching: A moves from x=-5 to x=1.5, B at x=0. Closest distance is 0.5, no touch.
    Transform startA4; startA4.origin = {-5,0,0};
    Transform endA4; endA4.origin = {1.5,0,0};
    ToIResult res4 = timeOfImpactConservativeAdvancement(sphereA, sphereB, startA4, endA4, startB, endB, cfg);
    // They touch when center distance = 2.0. Start x=-5, end x=1.5, so at x=-2.0 distance=2.0, fraction = (-2+5)/(1.5+5)=3/6.5≈0.4615.
    assert(res4.hasHit);
    assert(std::abs(res4.fraction - 3.0/6.5) < 1e-2);

    // Both moving towards each other: A from -5 to 0, B from 5 to 0.
    Transform startB5; startB5.origin = {5,0,0};
    Transform endB5; endB5.origin = {0,0,0};
    ToIResult res5 = timeOfImpactConservativeAdvancement(sphereA, sphereB, startA, endA, startB5, endB5, cfg);
    // Relative speed doubled; they meet when distance decreases from 10 to 2 over relative speed 10 units over 1 time.
    // Fraction = 8/10 = 0.8.
    assert(res5.hasHit);
    assert(std::abs(res5.fraction - 0.8) < 1e-2);

    // Zero relative velocity but overlapping initially? Not overlapping, so no hit.
    Transform overlapA; overlapA.origin = {1.0,0,0}; // distance 1 < 2, already penetrating.
    ToIResult res6 = timeOfImpactConservativeAdvancement(sphereA, sphereB, overlapA, overlapA, staticB, staticB, cfg);
    // No motion, so no advancement; algorithm likely returns false due to normal check.
    assert(!res6.hasHit);
    return 0;
}
// The solution follows the Bullet Physics implementation of `btSubsimplexConvexCast`. The core algorithm: (1) Compute relative velocity `r = (finA.origin - startA.origin) - (finB.origin - startB.origin)`. (2) Initialize the interpolated transforms at `lambda=0`. (3) Compute the initial support point of the Minkowski difference `A ⊖ B` in the direction `-v` (where `v` is initially the difference of supports along `-r`). (4) In a loop up to 32 times: compute the new support point `w` in direction `-v` at current interpolated transforms; if `v · w > 0`, then use the projection of the origin onto the ray `lambda` direction to advance `lambda` — specifically, if `v · r >= -epsilon^2` return false (separating), else `lambda -= (v·w)/(v·r)` and update interpolated origins via linear interpolation; if `lambda > 1` return false (no impact in range). Add `w` to the simplex solver if not already present. Use the simplex solver to compute the closest point `v` to the origin on the convex hull of the accumulated support points; if the simplex is full-dimensional, compute the closest point via projection onto edges/faces; otherwise `v` is the closest vertex. Update `dist2 = v.length2()`. After the loop, set fraction to `lambda`, normal to normalized `v` if length > epsilon, else zero. Return false if `normal.dot(r) >= -allowedPenetration` (moving away or too much penetration). Compute hit point as the last support point on shape B corresponding to the closest simplex vertex. Time complexity: O(maxIterations * (supportCost + simplexClosestCost)). Support cost is O(dimensions) for simple shapes; simplex closest cost is O(1) for up to 4 points, but here we restrict to a simplified simplex with up to 3 points (triangle) — the code uses a list of support points and finds the closest point on the segment or triangle via standard geometry. Space complexity O(1) for the simplex storage.
