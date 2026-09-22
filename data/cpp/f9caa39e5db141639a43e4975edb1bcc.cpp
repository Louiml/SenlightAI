// Write a C++ function `btConvexConcaveTOI` that, given a convex bounding sphere radius (a positive `btScalar`), a start transform, an end transform (both in the local space of a stationary concave triangle mesh), and a triangle (three `btVector3` vertices in that same local space), returns the time-of-impact fraction in `[0,1]` where a moving sphere of that radius first touches the triangle, using a swept-sphere vs triangle approximation. The function must return `1.0` if no collision occurs before or at the end of the motion. The triangle vertices are provided as an array of exactly three `btVector3`; assume the triangle is non-degenerate and the motion is linear. Use continuous collision detection via a subsimplex convex cast (sphere vs triangle) with an identity transform for the triangle. The result fraction must be the minimum over both the forward and backward sweeps if needed (i.e., the sphere may hit the triangle from either direction); if the sphere is already intersecting the triangle at the start, return `0.0`. Edge cases: if the start and end are identical, return `1.0` (no motion), and if the sphere radius is non-positive, return `1.0`. Provide a free function with the signature `btScalar btConvexConcaveTOI(btScalar radius, const btTransform& from, const btTransform& to, btVector3* triangle)`. Use `btConvexCast::CastResult` with an initial `m_fraction = 1.0`, and update it with the cast result. Include the necessary Bullet headers (`btTransform.h`, `btVector3.h`, `btSphereShape.h`, `btTriangleShape.h`, `btSubsimplexConvexCast.h`, `btConvexCast.h`, `btVoronoiSimplexSolver.h`) and ensure `const` correctness.
// The problem reduces to a swept-sphere vs triangle intersection test. We approximate the moving convex body by a sphere of given radius, and we test its motion from `from` to `to` against the fixed triangle. This is done by casting the sphere's center along the segment from `from.getOrigin()` to `to.getOrigin()` while the triangle is treated as a static obstacle. Because the sphere has a constant radius, we can inflate the triangle by that radius conceptually, but Bullet's `btSubsimplexConvexCast` directly handles the swept sphere vs stationary triangle when given the sphere shape and the triangle shape. The cast takes two transforms: the moving sphere's start and end transforms (with position-only changes; orientation is irrelevant for a sphere), and the triangle's start and end transforms (both identity because the triangle is stationary). The `calcTimeOfImpact` function returns `true` if there is a collision within the specified initial fraction, and it sets `castResult.m_fraction` to the earliest time of impact. We initialize `m_fraction = 1.0` and reduce it whenever a positive hit fraction is found. If the sphere starts intersecting the triangle (fraction 0), that is also captured. We must also consider that the sphere might hit the triangle only after moving forward, and the cast handles both directions because the sphere is symmetric. However, `btSubsimplexConvexCast` only checks forward motion from the start transform to the end transform; reverse motion is not needed because movement is linear and the start is fixed. To handle the edge case where the sphere is already inside the triangle at the start, the cast returns a fraction of 0, which we keep. If the radius is not positive, the sweep degenerates; we return 1.0 immediately. The time complexity is O(1) per call (constant number of operations), and space complexity is O(1) additional aside from temporary Bullet objects (sphere shape, triangle shape, simplex solver, cast result). The main algorithmic challenge is correctly initializing the Bullet objects and interpreting the fraction: Bullet's `calcTimeOfImpact` may return true with a fraction that is greater than the current best; we only update if the new fraction is smaller. Also, we must reset `castResult.m_fraction` to `1.0` initially, because Bullet expects it to be the maximum allowed fraction. We also must set `castResult.m_allowedPenetration = 0.0` to avoid fake hits due to penetration tolerance, though this is not strictly required for correctness in a simple test. The function should also account for the fact that the sphere may have zero radius (point), but we already reject non-positive radii.
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletCollision/CollisionShapes/btTriangleShape.h>
#include <BulletCollision/NarrowPhaseCollision/btSubsimplexConvexCast.h>
#include <BulletCollision/NarrowPhaseCollision/btConvexCast.h>
#include <BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>

// Compute the time of impact (fraction in [0,1]) when a sphere of given radius
// moves linearly from 'from' to 'to' and first touches the given triangle.
// Returns 1.0 if no collision before the end, or if inputs are degenerate.
// The triangle vertices are given in the same local space as the transforms.
btScalar btConvexConcaveTOI(btScalar radius, 
                            const btTransform& from, 
                            const btTransform& to, 
                            btVector3* triangle) {
    // Reject degenerate inputs: non-positive radius or no motion.
    if (radius <= btScalar(0.0)) return btScalar(1.0);
    if (from.getOrigin() == to.getOrigin()) return btScalar(1.0);

    // Shapes: a sphere with the given radius, and the triangle.
    btSphereShape sphereShape(radius);
    btTriangleShape triShape(triangle[0], triangle[1], triangle[2]);

    // Simplex solver for the convex cast.
    btVoronoiSimplexSolver simplexSolver;

    // The convex cast computes the swept sphere vs triangle.
    btSubsimplexConvexCast convexCaster(&sphereShape, &triShape, &simplexSolver);

    // Identity transforms for the stationary triangle.
    btTransform triFrom;
    triFrom.setIdentity();
    btTransform triTo;
    triTo.setIdentity();

    // Result container; initialize fraction to 1.0 (no collision yet).
    btConvexCast::CastResult castResult;
    castResult.m_fraction = btScalar(1.0);
    castResult.m_allowedPenetration = btScalar(0.0);

    // Perform the cast. If successful, castResult.m_fraction is the earliest hit.
    if (convexCaster.calcTimeOfImpact(from, to, triFrom, triTo, castResult)) {
        // Return the computed fraction, clamped to [0,1].
        if (castResult.m_fraction < btScalar(0.0)) return btScalar(0.0);
        if (castResult.m_fraction > btScalar(1.0)) return btScalar(1.0);
        return castResult.m_fraction;
    }

    // No collision detected.
    return btScalar(1.0);
}
#include <cassert>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletCollision/CollisionShapes/btTriangleShape.h>
#include <BulletCollision/NarrowPhaseCollision/btSubsimplexConvexCast.h>
#include <BulletCollision/NarrowPhaseCollision/btConvexCast.h>
#include <BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>

// Declaration of the function under test.
btScalar btConvexConcaveTOI(btScalar radius, 
                            const btTransform& from, 
                            const btTransform& to, 
                            btVector3* triangle);

int main() {
    // Define a horizontal triangle in the XY plane at z=0.
    btVector3 triangle[3] = {
        btVector3(0, 0, 0),
        btVector3(10, 0, 0),
        btVector3(0, 10, 0)
    };

    // Test 1: Sphere drops straight down onto the triangle from above.
    // Start at z=5, end at z=0 (which is exactly on the plane).
    // The sphere radius is 1, so it first touches at z=1 (i.e., after 4/5 of motion).
    btTransform from1;
    from1.setIdentity();
    from1.setOrigin(btVector3(2, 2, 5));
    btTransform to1;
    to1.setIdentity();
    to1.setOrigin(btVector3(2, 2, 0));
    btScalar t1 = btConvexConcaveTOI(1.0, from1, to1, triangle);
    assert(t1 > 0.79 && t1 < 0.81); // Expected ~0.8

    // Test 2: Sphere moves horizontally, never touching the triangle (far away).
    btTransform from2;
    from2.setIdentity();
    from2.setOrigin(btVector3(100, 100, 100));
    btTransform to2;
    to2.setIdentity();
    to2.setOrigin(btVector3(200, 200, 100));
    btScalar t2 = btConvexConcaveTOI(1.0, from2, to2, triangle);
    assert(t2 == 1.0);

    // Test 3: Sphere already intersecting the triangle at start.
    // Center at (1,1,0) with radius 1 → touches the triangle at start.
    btTransform from3;
    from3.setIdentity();
    from3.setOrigin(btVector3(1, 1, 0));
    btTransform to3;
    to3.setIdentity();
    to3.setOrigin(btVector3(1, 1, 5));
    btScalar t3 = btConvexConcaveTOI(1.0, from3, to3, triangle);
    assert(t3 == 0.0);

    // Test 4: No motion (start equals end).
    btTransform from4;
    from4.setIdentity();
    from4.setOrigin(btVector3(1, 1, 1));
    btTransform to4 = from4;
    btScalar t4 = btConvexConcaveTOI(1.0, from4, to4, triangle);
    assert(t4 == 1.0);

    // Test 5: Non-positive radius → always 1.0.
    btScalar t5 = btConvexConcaveTOI(0.0, from1, to1, triangle);
    assert(t5 == 1.0);

    // Test 6: Sphere passes through the triangle exactly at the midpoint.
    // Start above at z=2, end below at z=-2, radius=1.
    // First contact occurs at z=1, which is 1/4 of the way (from z=2 to z=-2, total delta -4).
    btTransform from6;
    from6.setIdentity();
    from6.setOrigin(btVector3(3, 3, 2));
    btTransform to6;
    to6.setIdentity();
    to6.setOrigin(btVector3(3, 3, -2));
    btScalar t6 = btConvexConcaveTOI(1.0, from6, to6, triangle);
    assert(t6 > 0.24 && t6 < 0.26); // Expected ~0.25

    // Test 7: Sphere moves parallel to triangle but close enough to graze it.
    // Triangle is in z=0, sphere radius=1, path from (5,5,1) to (5,5,-1) → touches at z=0.
    btTransform from7;
    from7.setIdentity();
    from7.setOrigin(btVector3(5, 5, 1));
    btTransform to7;
    to7.setIdentity();
    to7.setOrigin(btVector3(5, 5, -1));
    btScalar t7 = btConvexConcaveTOI(1.0, from7, to7, triangle);
    assert(t7 > 0.49 && t7 < 0.51); // Expected ~0.5

    return 0;
}
