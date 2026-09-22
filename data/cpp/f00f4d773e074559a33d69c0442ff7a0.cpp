// Given two 3D line segments represented by their center points, direction vectors, and half-lengths, write a C++ function `segmentDistance` that computes the squared distance between the two segments, along with the closest points on each segment (as offsets from their centers). The function should handle parallel, intersecting, and skew segments correctly, and must be robust to degenerate cases where direction vectors are zero or segments reduce to points. The signature should be: `void segmentDistance(const Vector3& centerA, const Vector3& dirA, float halfLenA, const Vector3& centerB, const Vector3& dirB, float halfLenB, Vector3& closestA, Vector3& closestB, float& squaredDist);` The output `closestA` and `closestB` are the closest points on segments A and B in world coordinates (i.e., center + offset). The algorithm must be deterministic and use only `float` precision.

// The classic approach for closest points between two line segments is to compute the parameters `tA` and `tB` that minimize the distance between infinite lines, then clamp these parameters to the segment ranges and refine if necessary. The algorithm proceeds as:
// 1. Compute `dA = dirA`, `dB = dirB`, and `r = centerB - centerA`.
// 2. Compute the dot products: `a = dA·dA`, `b = dA·dB`, `c = dB·dB`, `d = dA·r`, `e = dB·r`. Since direction vectors are unit-length in typical usage, `a` and `c` equal 1, but the general formula uses them for robustness.
// 3. Compute the denominator `denom = a*c - b*b`. If `denom` is near zero, the lines are parallel. In that case, set `tA = 0` (any point works) and compute `tB = d / c`? Actually the standard solution: if parallel, set `tA = 0`, then `tB = e / c`? Wait, the code snippet uses a simplified version assuming unit directions. For a general version, when parallel, choose `tA = 0` and then `tB = (b*tA - d) / c`? Let's derive: The optimal `tA` and `tB` for infinite lines solve the linear system:
// `a*tA - b*tB = d`
// `-b*tA + c*tB = -e` (depending on sign conventions). Solving gives `tA = (b*e - c*d) / denom` and `tB = (a*e - b*d) / denom`. When `denom` is near zero, lines are parallel; then we can set `tA = 0` and compute `tB = (b*0 - d) / c`? Wait, from the second equation: `-b*tA + c*tB = -e`, with `tA=0` gives `tB = -e / c`. However, the snippet uses a different sign convention because it defines `translation = centerB - centerA` and then `ptsVector = translation - offsetA + offsetB`. I'll follow the classic algorithm from Ericson's "Real-Time Collision Detection" which is robust.
// 4. Clamp `tA` to `[-halfLenA, halfLenA]`, then compute `tB` based on clamped `tA`, then clamp `tB`, then possibly recompute `tA` if `tB` was clamped. This ensures the result is the true closest points on the bounded segments.
// 5. Compute `closestA = centerA + dirA*tA` and `closestB = centerB + dirB*tB`. Then `squaredDist = (closestB - closestA).length2()`.
// Edge cases: degenerate directions (zero vector) – treat as point, but the input is assumed valid; parallel segments – handle by the denominator check; overlapping segments – the clamped parameters still give a valid result with zero distance. Time complexity is O(1), space O(1).

#include <cmath>
#include <cfloat>

struct Vector3 {
    float x, y, z;
    Vector3(float x=0, float y=0, float z=0) : x(x), y(y), z(z) {}
    Vector3 operator+(const Vector3& o) const { return Vector3(x+o.x, y+o.y, z+o.z); }
    Vector3 operator-(const Vector3& o) const { return Vector3(x-o.x, y-o.y, z-o.z); }
    Vector3 operator*(float s) const { return Vector3(x*s, y*s, z*s); }
    float dot(const Vector3& o) const { return x*o.x + y*o.y + z*o.z; }
    float length2() const { return dot(*this); }
    float length() const { return std::sqrt(length2()); }
};

// Clamp a parameter to the segment's half-length range.
static float clampParam(float t, float halfLen) {
    if (t < -halfLen) return -halfLen;
    if (t > halfLen) return halfLen;
    return t;
}

// Compute the closest points between two 3D line segments and the squared distance.
void segmentDistance(
    const Vector3& centerA, const Vector3& dirA, float halfLenA,
    const Vector3& centerB, const Vector3& dirB, float halfLenB,
    Vector3& closestA, Vector3& closestB, float& squaredDist)
{
    const Vector3 r = centerB - centerA;
    const float a = dirA.dot(dirA);
    const float b = dirA.dot(dirB);
    const float c = dirB.dot(dirB);
    const float d = dirA.dot(r);
    const float e = dirB.dot(r);
    
    float tA, tB;
    const float denom = a*c - b*b;
    
    if (denom < FLT_EPSILON) {
        // Segments are parallel (or degenerate). Choose tA=0 and best tB.
        tA = 0.0f;
        // From the line equations, minimize distance for given tA.
        // tB = (b*tA - d) / c when denom is zero? Actually from equation c*tB = b*tA - d? 
        // Let's derive: distance^2 = a*tA^2 - 2*b*tA*tB + c*tB^2 - 2*d*tA + 2*e*tB + const.
        // For parallel, b^2 = a*c, so the quadratic is degenerate. Set tA=0,
        // then minimize c*tB^2 + 2*e*tB -> derivative: 2*c*tB + 2*e = 0 -> tB = -e/c.
        if (std::fabs(c) > FLT_EPSILON) {
            tB = -e / c;
        } else {
            tB = 0.0f;
        }
    } else {
        // Solve the 2x2 system for infinite lines.
        tA = (b*e - c*d) / denom;
        tB = (a*e - b*d) / denom;
    }
    
    // Clamp tA and adjust tB if needed.
    tA = clampParam(tA, halfLenA);
    
    // Recompute tB from the clamped tA using the original relation:
    // From the system: b*tA - c*tB = d  ->  c*tB = b*tA - d  ->  tB = (b*tA - d)/c.
    // But this is only valid for the infinite line optimum; if tA is clamped,
    // we still need to minimize over tB given fixed tA.
    // The distance squared is quadratic in tB: c*tB^2 + (-2*b*tA + 2*e)*tB + (a*tA^2 - 2*d*tA + const).
    // The minimum is at tB = (b*tA - e)/c.
    if (std::fabs(c) > FLT_EPSILON) {
        tB = (b*tA - e) / c;
    } else {
        tB = 0.0f;
    }
    tB = clampParam(tB, halfLenB);
    
    // If tB was clamped, we may need to re-clamp tA based on the clamped tB,
    // because the optimal tA for a given tB is tA = (b*tB + d)/a (from a*tA - b*tB = d).
    if (std::fabs(a) > FLT_EPSILON) {
        float tA_new = (b*tB + d) / a;
        tA_new = clampParam(tA_new, halfLenA);
        // Only update if the new tA is within the segment; but since tA was already
        // clamped, this extra step handles the case where tB clamping changed the optimum.
        // We don't loop further because after this second clamp, both are within bounds.
        tA = tA_new;
    }
    
    closestA = centerA + dirA * tA;
    closestB = centerB + dirB * tB;
    const Vector3 diff = closestB - closestA;
    squaredDist = diff.length2();
}

#include <cassert>
#include <cmath>

int main() {
    // Two perpendicular segments crossing at their centers.
    Vector3 a(0,0,0), da(1,0,0); float halfA = 1.0f;
    Vector3 b(0,0,0), db(0,1,0); float halfB = 1.0f;
    Vector3 p1, p2; float dist;
    segmentDistance(a, da, halfA, b, db, halfB, p1, p2, dist);
    assert(std::fabs(dist - 0.0f) < 1e-5f);
    assert(p1.x == 0 && p1.y == 0 && p1.z == 0);
    assert(p2.x == 0 && p2.y == 0 && p2.z == 0);

    // Parallel segments with a lateral offset.
    Vector3 a2(0,0,0), da2(1,0,0); float halfA2 = 1.0f;
    Vector3 b2(0,2,0), db2(1,0,0); float halfB2 = 1.0f;
    segmentDistance(a2, da2, halfA2, b2, db2, halfB2, p1, p2, dist);
    assert(std::fabs(dist - 4.0f) < 1e-5f); // lateral offset squared = 4
    assert(p1.y == 0 && p2.y == 2);

    // Overlapping collinear segments.
    Vector3 a3(-1,0,0), da3(1,0,0); float halfA3 = 1.0f;
    Vector3 b3(0,0,0), db3(1,0,0); float halfB3 = 1.0f;
    segmentDistance(a3, da3, halfA3, b3, db3, halfB3, p1, p2, dist);
    assert(std::fabs(dist) < 1e-5f);
    assert(p1.x == p2.x); // closest points coincide

    // Skew segments with known distance.
    Vector3 a4(0,0,0), da4(1,0,0); float halfA4 = 1.0f;
    Vector3 b4(0,1,1), db4(0,0,1); float halfB4 = 1.0f;
    segmentDistance(a4, da4, halfA4, b4, db4, halfB4, p1, p2, dist);
    // Closest points: (0,0,1) on A and (0,1,1) on B? Actually A spans x from -1 to 1 at y=0,z=0; B spans z from 0 to 2 at x=0,y=1. The closest points are (0,0,0) on A and (0,1,0) on B, distance sqrt(1). But check: A's z=0, B's y=1, so distance squared = 1.
    assert(std::fabs(dist - 1.0f) < 1e-5f);

    // Segment reduced to a point (halfLen = 0).
    Vector3 a5(3,0,0), da5(1,0,0); float halfA5 = 0.0f;
    Vector3 b5(0,0,0), db5(0,1,0); float halfB5 = 1.0f;
    segmentDistance(a5, da5, halfA5, b5, db5, halfB5, p1, p2, dist);
    // Point on A is (3,0,0); B is line from (0,-1,0) to (0,1,0); closest on B is (0,0,0); distance squared = 9.
    assert(std::fabs(dist - 9.0f) < 1e-5f);

    // Non-orthogonal neither parallel nor intersecting.
    Vector3 a6(0,0,0), da6(1,1,0); float halfA6 = 1.0f; // direction normalized? Not required but works.
    Vector3 b6(0,0,0), db6(1,-1,0); float halfB6 = 1.0f;
    segmentDistance(a6, da6, halfA6, b6, db6, halfB6, p1, p2, dist);
    // Both pass through origin with directions (1,1,0) and (1,-1,0), so they cross at origin – distance 0.
    assert(std::fabs(dist) < 1e-5f);

    // Check that closest points are within segment bounds.
    Vector3 a7(0,0,0), da7(1,0,0); float halfA7 = 0.5f;
    Vector3 b7(10,0,0), db7(1,0,0); float halfB7 = 0.5f;
    segmentDistance(a7, da7, halfA7, b7, db7, halfB7, p1, p2, dist);
    assert(p1.x == 0.5f && p2.x == 9.5f); // clamped to endpoints
    assert(std::fabs(dist - (9.0f*9.0f)) < 1e-5f); // distance between endpoints is 9

    return 0;
}
