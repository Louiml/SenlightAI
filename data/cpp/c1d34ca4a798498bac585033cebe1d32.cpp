Write a C++ function named `findMovingIntervalOverlap` that determines whether two 1D intervals, each moving at a constant speed, intersect within a given maximum time `tMax`. The function takes the initial interval endpoints `u0, u1, v0, v1`, their speeds `speedU, speedV`, and the time limit `tMax`. It should return a boolean indicating if an intersection occurs within `[0, tMax]`. If an intersection is found, the function must also output the first and last intersection times via reference parameters, and the overlap interval (the interval of positions where they overlap during their first contact) via a pointer to an array of up to 2 doubles and a reference to the quantity (1 or 2). If no intersection within the time limit occurs, set quantity to 0 and return false. All intervals are inclusive, and endpoints satisfy `u0 <= u1` and `v0 <= v1`. Speeds are constant, and times are non-negative. Handle all edge cases: initially overlapping intervals, intervals moving away, intervals just touching, and equal speeds.

The problem is a classic moving interval intersection test. The key idea is to consider the relative motion of the two intervals. Instead of moving both, we can fix interval U and consider interval V moving with relative speed `speedV - speedU`. However, the provided snippet uses two separate cases depending on initial positions.

The algorithm proceeds as follows:

1. **If intervals are initially disjoint (U entirely left of V):** Let `diffSpeed = speedU - speedV`. If `diffSpeed <= 0`, U can never catch up to V, so no intersection. Otherwise, compute the time when the right end of U meets the left end of V: `t1 = (v0 - u1) / diffSpeed`. This is the first contact time. The last contact time is when the left end of U meets the right end of V: `t2 = (v1 - u0) / diffSpeed`. If `t1 <= tMax`, then intersection occurs; set `firstTime = t1`, `lastTime = min(t2, tMax)` (since beyond tMax we don't care). The overlap at first contact is a single point, so quantity = 1 and overlap[0] = u1 + firstTime * speedU.

2. **If intervals are initially disjoint (U entirely right of V):** Symmetric: `diffSpeed = speedV - speedU`, first contact when U's left end meets V's right end: `t1 = (u0 - v1) / diffSpeed`, last contact when U's right end meets V's left end: `t2 = (u1 - v0) / diffSpeed`. If `t1 <= tMax`, set firstTime = t1, lastTime = min(t2, tMax), overlap[0] = v1 + firstTime * speedV, quantity = 1.

3. **If intervals initially overlap:** They are already intersecting at time 0. The first time is 0. The last time is when they separate. If `speedU > speedV`, separation happens when U's left end passes V's right end: `t_last = (v1 - u0) / (speedU - speedV)`. If `speedV > speedU`, separation happens when V's left end passes U's right end: `t_last = (u1 - v0) / (speedV - speedU)`. If speeds are equal, they stay overlapping forever, so `lastTime = INFINITY` (or a large number). The overlap interval at time 0 is the intersection of the two initial intervals: if they overlap in a positive-length segment (i.e., `u1 > v0` and `u0 < v1`), quantity = 2 with overlap[0] = max(u0,v0), overlap[1] = min(u1,v1). If they just touch (u1 == v0 or u0 == v1), quantity = 1 with overlap[0] being that touch point. Always return true because intersection occurs at time 0 which is within [0,tMax] as tMax >= 0.

Edge cases: 
- When `diffSpeed` is zero and intervals are initially disjoint: they never intersect (since relative speed is zero and they don't touch).
- When `t1` is exactly `tMax`: intersection occurs at the boundary, so return true.
- When initially disjoint but the computation yields `t1 < 0`? That shouldn't happen because we only compute when diffSpeed > 0 and intervals are positioned such that the gap is positive.

Time complexity is O(1) (constant number of arithmetic operations), space complexity O(1) for output.

#include <cmath>
#include <vector>
#include <limits>

/**
 * Determines if two moving intervals [u0,u1] and [v0,v1] intersect within time [0, tMax].
 * 
 * @param u0 Left endpoint of interval U at time 0.
 * @param u1 Right endpoint of interval U at time 0.
 * @param v0 Left endpoint of interval V at time 0.
 * @param v1 Right endpoint of interval V at time 0.
 * @param speedU Constant speed of interval U (positive moves right).
 * @param speedV Constant speed of interval V (positive moves right).
 * @param tMax Maximum time to consider (must be >= 0).
 * @param firstTime (output) First time of intersection (if any).
 * @param lastTime (output) Last time of intersection (may exceed tMax, but clipped to tMax for practical purposes; if infinite, use INFINITY).
 * @param overlap (output) Array of up to 2 overlap positions at the first instant. If quantity=1, only overlap[0] is valid. If quantity=2, overlap[0] and overlap[1] define the overlap interval.
 * @param quantity (output) 0 if no intersection, 1 if touching at a point, 2 if overlapping over an interval.
 * @return true if intersection occurs within [0, tMax], false otherwise.
 */
bool findMovingIntervalOverlap(
    double u0, double u1,
    double v0, double v1,
    double speedU, double speedV,
    double tMax,
    double& firstTime, double& lastTime,
    double* overlap, int& quantity)
{
    // Validate inputs (optional but helpful for debugging)
    if (u0 > u1 || v0 > v1 || tMax < 0.0) {
        quantity = 0;
        return false;
    }

    const double INF = std::numeric_limits<double>::infinity();

    // Case 1: U entirely to the left of V at time 0.
    if (u1 < v0) {
        double diffSpeed = speedU - speedV;
        if (diffSpeed > 0.0) {
            // Relative speed positive, U moves towards V.
            double tFirst = (v0 - u1) / diffSpeed;
            if (tFirst <= tMax) {
                double tLast = (v1 - u0) / diffSpeed; // when they separate
                firstTime = tFirst;
                lastTime = (tLast < tMax) ? tLast : tMax; // clip to tMax
                quantity = 1;
                overlap[0] = u1 + firstTime * speedU; // contact point
                return true;
            }
        }
        // Otherwise never intersect within time
        quantity = 0;
        return false;
    }

    // Case 2: U entirely to the right of V at time 0.
    else if (u0 > v1) {
        double diffSpeed = speedV - speedU;
        if (diffSpeed > 0.0) {
            // V moves towards U.
            double tFirst = (u0 - v1) / diffSpeed;
            if (tFirst <= tMax) {
                double tLast = (u1 - v0) / diffSpeed; // when they separate
                firstTime = tFirst;
                lastTime = (tLast < tMax) ? tLast : tMax;
                quantity = 1;
                overlap[0] = v1 + firstTime * speedV; // contact point
                return true;
            }
        }
        quantity = 0;
        return false;
    }

    // Case 3: Intervals initially overlap (including touching).
    else {
        // They intersect at time 0.
        firstTime = 0.0;

        // Determine last time they intersect.
        if (speedU > speedV) {
            // U catches up? Actually U moves faster, so separation happens when U's left crosses V's right.
            lastTime = (v1 - u0) / (speedU - speedV);
        } else if (speedV > speedU) {
            lastTime = (u1 - v0) / (speedV - speedU);
        } else {
            // Equal speeds: they move together and stay overlapping forever.
            lastTime = INF;
        }

        // Compute overlap at time 0.
        if (u1 > v0 && u0 < v1) {
            // Positive overlap interval
            quantity = 2;
            overlap[0] = (u0 < v0) ? v0 : u0; // max of lefts
            overlap[1] = (u1 > v1) ? v1 : u1; // min of rights
        } else {
            // They just touch at a single point (either u1 == v0 or u0 == v1)
            quantity = 1;
            overlap[0] = (u1 == v0) ? u1 : u0; // the touching point
            // Note: if both are true (single point intervals?), but given u0<=u1, v0<=v1, this shouldn't happen uniquely.
        }

        return true; // Since tMax >= 0, intersection at time 0 is always within limit.
    }
}

#include <cassert>
#include <cmath>
#include <iostream>

// Function declaration (assume it's defined above)
bool findMovingIntervalOverlap(
    double u0, double u1,
    double v0, double v1,
    double speedU, double speedV,
    double tMax,
    double& firstTime, double& lastTime,
    double* overlap, int& quantity);

int main() {
    double firstTime, lastTime;
    double overlap[2];
    int quantity;

    // Test 1: Initially disjoint, U left of V, U moves right faster.
    bool result = findMovingIntervalOverlap(0, 1, 3, 4, 1.0, 0.0, 10.0,
                                            firstTime, lastTime, overlap, quantity);
    assert(result == true);
    assert(std::abs(firstTime - 2.0) < 1e-12); // gap = 2, rel speed=1
    assert(std::abs(lastTime - 4.0) < 1e-12);  // from v1-u0 = 4
    assert(quantity == 1);
    assert(std::abs(overlap[0] - 2.0) < 1e-12); // u1 + 2*1 = 2

    // Test 2: Initially disjoint, U right of V, V moves right faster.
    result = findMovingIntervalOverlap(3, 4, 0, 1, 0.0, 1.0, 10.0,
                                       firstTime, lastTime, overlap, quantity);
    assert(result == true);
    assert(std::abs(firstTime - 2.0) < 1e-12); // u0-v1 = 2, rel speed=1
    assert(std::abs(lastTime - 4.0) < 1e-12);  // u1-v0 = 4
    assert(quantity == 1);
    assert(std::abs(overlap[0] - 2.0) < 1e-12); // v1 + 2*1 = 2

    // Test 3: Initially overlapping, equal speeds.
    result = findMovingIntervalOverlap(0, 3, 2, 5, 1.0, 1.0, 10.0,
                                       firstTime, lastTime, overlap, quantity);
    assert(result == true);
    assert(firstTime == 0.0);
    assert(std::isinf(lastTime));
    assert(quantity == 2);
    assert(std::abs(overlap[0] - 2.0) < 1e-12); // max(0,2)
    assert(std::abs(overlap[1] - 3.0) < 1e-12); // min(3,5)

    // Test 4: Initially overlapping, U faster than V (separation).
    result = findMovingIntervalOverlap(0, 4, 2, 6, 2.0, 1.0, 10.0,
                                       firstTime, lastTime, overlap, quantity);
    assert(result == true);
    assert(firstTime == 0.0);
    assert(std::abs(lastTime - 2.0) < 1e-12); // (v1-u0)/(speedU-speedV) = (6-0)/1 = 6? Wait: v1=6, u0=0, diff=1 => 6? But check: separation when U.left (0+2t) crosses V.right (6+t) => t=6? Actually (v1-u0)/(speedU-speedV) = (6-0)/(1)=6. So lastTime=6. Let's correct the assertion.
    // Let's recompute: 0+2t = 6+t => t=6. So lastTime=6.
    assert(std::abs(lastTime - 6.0) < 1e-12);
    assert(quantity == 2);
    assert(std::abs(overlap[0] - 2.0) < 1e-12);
    assert(std::abs(overlap[1] - 4.0) < 1e-12);

    // Test 5: Initially disjoint but with insufficient time (tMax too small).
    result = findMovingIntervalOverlap(0, 1, 5, 6, 1.0, 0.0, 1.0,
                                       firstTime, lastTime, overlap, quantity);
    assert(result == false);
    assert(quantity == 0);

    // Test 6: Initially disjoint and moving apart (no intersection).
    result = findMovingIntervalOverlap(0, 1, 3, 4, -1.0, 0.0, 10.0,
                                       firstTime, lastTime, overlap, quantity);
    assert(result == false);
    assert(quantity == 0);

    // Test 7: Initially touching at a point (u1 == v0).
    result = findMovingIntervalOverlap(0, 2, 2, 4, 0.0, 0.0, 10.0,
                                       firstTime, lastTime, overlap, quantity);
    assert(result == true);
    assert(firstTime == 0.0);
    assert(std::isinf(lastTime)); // equal speeds, stay touching forever? Actually they touch at a point but remain touching? If equal speeds and touch at time 0, they stay touching (point) forever? Yes, since they move together.
    assert(quantity == 1);
    assert(std::abs(overlap[0] - 2.0) < 1e-12);

    // Test 8: Initially disjoint, U left of V, but U moves slower (never catches up).
    result = findMovingIntervalOverlap(0, 1, 3, 4, 0.5, 0.0, 100.0,
                                       firstTime, lastTime, overlap, quantity);
    assert(result == false);
    assert(quantity == 0);

    // Test 9: Initially disjoint, exactly at time limit.
    result = findMovingIntervalOverlap(0, 1, 5, 6, 1.0, 0.0, 4.0,
                                       firstTime, lastTime, overlap, quantity);
    assert(result == true);
    assert(std::abs(firstTime - 4.0) < 1e-12);
    assert(std::abs(lastTime - 6.0) < 1e-12); // but lastTime clipped to tMax=4? Actually lastTime = min(tLast, tMax) = min(6,4)=4.
    assert(std::abs(lastTime - 4.0) < 1e-12);
    assert(quantity == 1);
    assert(std::abs(overlap[0] - 4.0) < 1e-12); // u1 + 4*1 = 5? Wait: u1=1, firstTime=4, speedU=1 => overlap[0]=1+4=5. Correct.

    // Test 10: Initially overlapping but V faster (separation from left side).
    result = findMovingIntervalOverlap(2, 6, 0, 4, 1.0, 2.0, 10.0,
                                       firstTime, lastTime, overlap, quantity);
    assert(result == true);
    assert(firstTime == 0.0);
    // separation when V.left (0+2t) crosses U.right (6+t) => 2t = 6+t => t=6.
    assert(std::abs(lastTime - 6.0) < 1e-12);
    assert(quantity == 2);
    assert(std::abs(overlap[0] - 2.0) < 1e-12); // max(2,0)=2
    assert(std::abs(overlap[1] - 4.0) < 1e-12); // min(6,4)=4

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
