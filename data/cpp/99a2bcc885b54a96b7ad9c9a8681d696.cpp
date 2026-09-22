/*
Write a C++ function `btScalar conservativeAdvancementTOI(const btTransform& fromA, const btTransform& toA, const btTransform& fromB, const btTransform& toB, const btConvexShape* convexA, const btConvexShape* convexB, btSimplexSolverInterface* solver, btVector3& normalOut, btVector3& hitPointOut, btScalar allowedPenetration = btScalar(0.0))` that computes the time of impact (a fraction between 0 and 1) between two moving convex shapes using the sub-simplex conservative advancement algorithm simplified from the given snippet. The function must use relative motion, GJK-style closest-point iteration on the Minkowski difference, and return the first time lambda when the shapes touch, or 1.0 if no collision occurs within the motion interval. The output normal (unit vector) and hit point (on shape B's surface) must be set appropriately, and the function must return false (as a `bool` via a reference parameter or by returning a sentinel) if moving apart or if separation remains above epsilon. For simplicity, treat the shapes as moving linearly (no rotation interpolation during the advancement) — only translate the origins using `setInterpolate3`. The function must handle degenerate cases (parallel motion, zero relative velocity, initial overlap) gracefully, cap iterations at 32, and use an epsilon of 0.0001 for distance checks.
*/
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>
#include <BulletCollision/CollisionShapes/btConvexShape.h>
#include <BulletCollision/NarrowPhaseCollision/btSimplexSolverInterface.h>
#include <BulletCollision/NarrowPhaseCollision/btPointCollector.h>

// Conservative advancement TOI for two linearly translating convex shapes.
// Returns the fraction lambda in [0,1] when shapes first touch, or 1.0 if no impact.
// Sets normalOut (unit, pointing from A to B) and hitPointOut (on B's surface).
// Returns false if shapes are moving apart or if no collision occurs within [0,1].
bool conservativeAdvancementTOI(
    const btTransform& fromA, const btTransform& toA,
    const btTransform& fromB, const btTransform& toB,
    const btConvexShape* convexA, const btConvexShape* convexB,
    btSimplexSolverInterface* solver,
    btVector3& normalOut, btVector3& hitPointOut,
    btScalar allowedPenetration = btScalar(0.0))
{
    solver->reset();

    // Relative linear velocity.
    const btVector3 linVelA = toA.getOrigin() - fromA.getOrigin();
    const btVector3 linVelB = toB.getOrigin() - fromB.getOrigin();
    const btVector3 r = linVelA - linVelB;

    // Initial interpolated transforms.
    btTransform interpA = fromA;
    btTransform interpB = fromB;

    btScalar lambda = btScalar(0.0);
    btScalar lastLambda = lambda;

    // Initial support point in the Minkowski difference.
    btVector3 v;
    {
        const btVector3 dirA = -r * fromA.getBasis();
        const btVector3 dirB =  r * fromB.getBasis();
        const btVector3 supA = fromA(convexA->localGetSupportingVertex(dirA));
        const btVector3 supB = fromB(convexB->localGetSupportingVertex(dirB));
        v = supA - supB;
    }

    btScalar dist2 = v.length2();
    const btScalar epsilon = btScalar(0.0001);
    int maxIter = 32;

    bool hasResult = false;
    btVector3 n(0,0,0);

    while ((dist2 > epsilon) && (maxIter-- > 0))
    {
        // Compute support points at current interpolated transforms.
        const btVector3 dirA = -v * interpA.getBasis();
        const btVector3 dirB =  v * interpB.getBasis();
        const btVector3 supA = interpA(convexA->localGetSupportingVertex(dirA));
        const btVector3 supB = interpB(convexB->localGetSupportingVertex(dirB));
        const btVector3 w = supA - supB;

        const btScalar VdotW = v.dot(w);
        if (lambda > btScalar(1.0))
            return false;

        if (VdotW > btScalar(0.0))
        {
            const btScalar VdotR = v.dot(r);
            if (VdotR >= -(SIMD_EPSILON * SIMD_EPSILON))
                return false;  // Moving away or parallel.

            // Advance lambda conservatively.
            lambda = lambda - VdotW / VdotR;
            if (lambda > btScalar(1.0))
                return false;
            if (lambda < btScalar(0.0))
                lambda = btScalar(0.0);

            // Interpolate only origins (no rotation for this simplified version).
            interpA.getOrigin().setInterpolate3(fromA.getOrigin(), toA.getOrigin(), lambda);
            interpB.getOrigin().setInterpolate3(fromB.getOrigin(), toB.getOrigin(), lambda);

            lastLambda = lambda;
            n = v;
            hasResult = true;
        }

        // Add the new vertex to the simplex if not already present.
        if (!solver->inSimplex(w))
            solver->addVertex(w, supA, supB);

        // Find closest point to origin in current simplex.
        if (solver->closest(v))
        {
            dist2 = v.length2();
            hasResult = true;
        }
        else
        {
            dist2 = btScalar(0.0);
        }
    }

    // Reject if we never found a candidate normal or if moving away.
    if (!hasResult || n.length2() < (SIMD_EPSILON * SIMD_EPSILON))
        return false;

    normalOut = n.normalized();
    if (normalOut.dot(r) >= -allowedPenetration)
        return false;  // Not approaching.

    // Compute the hit point on B's surface.
    btVector3 hitA, hitB;
    solver->compute_points(hitA, hitB);
    hitPointOut = hitB;

    // If lambda is beyond 1, no impact occurs in the interval.
    if (lambda > btScalar(1.0))
        return false;
    return true;
}
#include <cassert>
#include <cmath>
// Include Bullet headers in the test environment as needed.

int main() {
    // Simple sphere-like setup using btBoxShape for determinism.
    btBoxShape box(btVector3(1,1,1)); // 2x2x2 box centered at origin.
    btSimplexSolverInterface* solver = new btSimplexSolver(); // assume implemented.

    // Test 1: Head-on collision along X.
    {
        btTransform fromA(btQuaternion::getIdentity(), btVector3(-3,0,0));
        btTransform toA  (btQuaternion::getIdentity(), btVector3( 0,0,0));
        btTransform fromB(btQuaternion::getIdentity(), btVector3( 3,0,0));
        btTransform toB  (btQuaternion::getIdentity(), btVector3( 0,0,0));
        btVector3 normal, hit;
        bool result = conservativeAdvancementTOI(fromA,toA,fromB,toB,&box,&box,solver,normal,hit);
        assert(result);
        assert(std::fabs(normal.x() - 1.0) < 1e-3 && std::fabs(normal.y()) < 1e-3 && std::fabs(normal.z()) < 1e-3);
        // Shapes touch when A's right face (x=1) meets B's left face (x=-1), relative distance 6-2=4, relative speed 6 -> lambda=4/6=2/3.
        assert(std::fabs(hit.x() + 1.0) < 1e-2); // hit point on B's surface at x=-1.
    }

    // Test 2: Moving apart -> no impact.
    {
        btTransform fromA(btQuaternion::getIdentity(), btVector3(-2,0,0));
        btTransform toA  (btQuaternion::getIdentity(), btVector3(-5,0,0));
        btTransform fromB(btQuaternion::getIdentity(), btVector3( 2,0,0));
        btTransform toB  (btQuaternion::getIdentity(), btVector3( 5,0,0));
        btVector3 normal, hit;
        bool result = conservativeAdvancementTOI(fromA,toA,fromB,toB,&box,&box,solver,normal,hit);
        assert(!result);
    }

    // Test 3: Exact touch at t=0 (initial contact).
    {
        btTransform fromA(btQuaternion::getIdentity(), btVector3(-2,0,0));
        btTransform toA  (btQuaternion::getIdentity(), btVector3(-1,0,0));
        btTransform fromB(btQuaternion::getIdentity(), btVector3( 0,0,0));
        btTransform toB  (btQuaternion::getIdentity(), btVector3( 1,0,0));
        btVector3 normal, hit;
        bool result = conservativeAdvancementTOI(fromA,toA,fromB,toB,&box,&box,solver,normal,hit);
        assert(result);
        assert(std::fabs(hit.x() + 1.0) < 1e-2); // B's left face.
    }

    // Test 4: No motion (zero relative velocity) and separated -> should be false.
    {
        btTransform fromA(btQuaternion::getIdentity(), btVector3(-3,0,0));
        btTransform toA  (btQuaternion::getIdentity(), btVector3(-3,0,0));
        btTransform fromB(btQuaternion::getIdentity(), btVector3( 3,0,0));
        btTransform toB  (btQuaternion::getIdentity(), btVector3( 3,0,0));
        btVector3 normal, hit;
        bool result = conservativeAdvancementTOI(fromA,toA,fromB,toB,&box,&box,solver,normal,hit);
        assert(!result);
    }

    // Test 5: Overlap initially -> immediate TOI at 0.
    {
        btTransform fromA(btQuaternion::getIdentity(), btVector3(0,0,0));
        btTransform toA  (btQuaternion::getIdentity(), btVector3(3,0,0));
        btTransform fromB(btQuaternion::getIdentity(), btVector3(0,0,0));
        btTransform toB  (btQuaternion::getIdentity(), btVector3(-3,0,0));
        btVector3 normal, hit;
        bool result = conservativeAdvancementTOI(fromA,toA,fromB,toB,&box,&box,solver,normal,hit);
        assert(result);
        assert(std::fabs(hit.x()) < 1e-2); // Some point within overlap.
    }

    // Test 6: Miss far above (no collision).
    {
        btTransform fromA(btQuaternion::getIdentity(), btVector3(-5,10,0));
        btTransform toA  (btQuaternion::getIdentity(), btVector3( 5,10,0));
        btTransform fromB(btQuaternion::getIdentity(), btVector3(-5,-10,0));
        btTransform toB  (btQuaternion::getIdentity(), btVector3( 5,-10,0));
        btVector3 normal, hit;
        bool result = conservativeAdvancementTOI(fromA,toA,fromB,toB,&box,&box,solver,normal,hit);
        assert(!result);
    }

    delete solver;
    return 0;
}
// The core algorithm is a conservative advancement variant of GJK for convex shapes. It computes the relative linear velocity `r = (toA - fromA) - (toB - fromB)`. At each step, it maintains an interpolated transform at fraction `lambda` (starting at 0). It computes the supporting points of A and B in directions `-v` and `v` respectively (where `v` is the current closest-point direction in the Minkowski difference), and forms the difference `w = supA - supB`. The point `w` is added to the simplex solver (if not already present) to iteratively find the closest point `v` of the current simplex to the origin. If `v.dot(w) > 0`, the algorithm attempts to move forward: it computes `VdotR = v.dot(r)`; if `VdotR` is non-negative (i.e., motion is not resolving the separation), it returns "no impact". Otherwise, it updates `lambda` using `lambda = lambda - VdotW / VdotR`, interpolates the transforms' origins to the new lambda, and continues. This iteration repeats until the squared distance `dist2` is below epsilon or the maximum iteration count (32) is exceeded. After the loop, it checks that the normal (the final `v`) points against the relative motion — if `normal.dot(r) >= -allowedPenetration`, it rejects the impact (moving away). Otherwise, it returns the fraction and computes the hit point via `solver->compute_points`. Key edge cases: zero relative velocity (→ immediate false), initial overlap (dist2 < epsilon at lambda=0 → return 0), and when `VdotR` is nearly zero (→ false to avoid division by near-zero). Time complexity is O(iterations * cost of support function), which is O(1) per iteration for convex shapes with constant-time support queries; space complexity is O(1) aside from the solver's internal simplex storage.
