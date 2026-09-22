Implement a C++ function that simulates a simplified version of the contact point cache management from the Bullet Physics engine. Given an array of 3D points (as `std::array<std::array<double,3>, 4>` representing up to four cached contact points) and a new candidate point (`std::array<double,3>`), your function must determine which of the four cached points should be replaced (index 0–3) to maximize the area of the triangle formed by the new point and two of the remaining three cached points, while also ensuring the deepest (most penetrating) cached point is never replaced if possible. Specifically, the algorithm should: 1) Identify the cached point with the smallest "distance" (here simply its z-coordinate as a proxy for penetration depth — lower z means deeper). 2) For each candidate replacement index i (0–3), if i is not the deepest point, compute the squared area of the triangle formed by the new point and the two cached points other than i (using the first two coordinates as x–y, ignoring z). 3) Choose the candidate with the largest such area; if all candidates except the deepest are tied or if the deepest point is the only option, return the index of the deepest point as a fallback (though this must never be replaced if any other candidate exists). Return -1 if the input is invalid (e.g., fewer than 4 cached points). The function signature should be: `int selectReplacement(const std::array<std::array<double,3>,4>& cache, const std::array<double,3>& newPoint);`
// The core idea mirrors the `sortCachedPoints` logic but simplified to a pure geometric selection without modifying the cache. The algorithm works as follows:  
// 1. **Find deepest point**: Iterate through the four cached points, track the index with the smallest z-coordinate (deepest). If there are ties, keep the first occurrence.  
// 2. **Compute candidate areas**: For each index `i` from 0 to 3, if `i` is not the deepest index, consider replacing `i`. The new triangle would be formed by `newPoint`, and the two cached points that are neither `i` nor the deepest (since the deepest is preserved). Actually, the correct approach from the snippet: for each candidate replacement index `i`, we compute the area using the new point and the three other cached points, but we exclude the deepest point from being replaced. In the original code, for each candidate `i` (which is the point to be replaced), it computes the area using the new point and two of the remaining three points (the ones that are not `i`). However, the original also ensures the deepest is never replaced by skipping that index in the area computation. So our simplified version: For each `i` that is not the deepest index, compute the squared area of the triangle formed by `newPoint`, and the two cached points with indices not equal to `i` and not equal to the deepest? Actually no—the original code computes for each `i` (candidate to replace) the triangle formed by newPoint and the other three points? Let me re-read: In the code, for each `i` from 0 to 3, it says `if (maxPenetrationIndex != i)` then compute area using newPoint and two other points. Specifically, for `i=0`, it uses m_pointCache[1], m_pointCache[2], m_pointCache[3]? Wait, the original uses `pt.m_localPointA - m_pointCache[1]` and `m_pointCache[3] - m_pointCache[2]` – that's using three of the four cached points (indices 1,2,3) and the new point. So it's computing the area of the triangle formed by newPoint and two of the cached points (specifically, it forms a cross product of vectors from newPoint to one cached point, and between two other cached points). The squared area is the squared length of that cross product, which corresponds to the parallelogram area, and dividing by 2 would give triangle area, but we only need a comparison. So each candidate `i` represents replacing that point, and the area is computed using the other three cached points plus the new point. The deepest point is never considered as a replacement candidate (since we skip that `i`).  
//
// Thus our algorithm:  
// - Find `deepestIdx` = index of cache point with smallest z (first tie).  
// - For each `i` from 0 to 3, if `i == deepestIdx`, skip (we never replace the deepest).  
// - For valid `i`, compute the squared area:  
//   - Let `others` = the three cache indices not equal to `i` (this includes the deepest, unless `i` is deepest). Actually, for a given `i`, we need two vectors: one from newPoint to one of the remaining points, and one between two other remaining points. To simplify, we can compute the area of the triangle formed by newPoint and any two of the remaining three cache points. To match the original logic, we can follow exactly: for candidate `i`, compute cross product of (newPoint - cache[j]) and (cache[k] - cache[l]) where {j,k,l} are the three indices not equal to i, arranged as in the snippet. But to keep it simple and general, we can compute the maximum possible triangle area using any two of the three remaining points? However, the original uses a specific pairing, and we should replicate it for consistency. Let's replicate:  
//     - If `i != 0`: `a0 = newPoint - cache[1]`, `b0 = cache[3] - cache[2]` → cross, squared area.  
//     - If `i != 1`: `a1 = newPoint - cache[0]`, `b1 = cache[3] - cache[2]` → cross.  
//     - If `i != 2`: `a2 = newPoint - cache[0]`, `b2 = cache[3] - cache[1]` → cross.  
//     - If `i != 3`: `a3 = newPoint - cache[0]`, `b3 = cache[2] - cache[1]` → cross.  
//   But note: if the deepest index is, say, 0, then for candidate `i=1`, we use cache[0], cache[2], cache[3] – that's fine because deepest is not replaced. However, the original also computes for `i` that might be deepest? It excludes those. So we only compute for `i` not equal to deepest. For each such `i`, we compute the squared area as above (using the other three).  
// - Track the `i` with the maximum squared area. If multiple have the same maximum, pick the smallest index? The original uses `closestAxis4()` which returns the index of the maximum value; ties probably break to smallest index. We'll do the same: first maximum wins.  
// - If no valid `i` exists (meaning all four are deepest? impossible because there is always at least one non-deepest unless all four have exactly the same z? But if all have same z, then deepest is the first one, and the other three are also "deepest" but the deepest index is just one; so we have three valid candidates). If for some reason all indices are skipped (e.g., only one point? but we have exactly 4), return the deepest index as fallback.  
// - Edge cases: If the cache has fewer than 4 elements (but signature guarantees 4), or if vectors are degenerate (zero-length cross), area=0, still fine.  
// - Time complexity: O(1) since constant size. Space: O(1).
#include <array>
#include <cmath>
#include <algorithm>

// Compute squared cross product magnitude (twice triangle area squared) for 2D vectors (x,y components)
static double cross2D(const std::array<double,3>& a, const std::array<double,3>& b) {
    return (a[0]*b[1] - a[1]*b[0]) * (a[0]*b[1] - a[1]*b[0]);
}

// Select which of the 4 cached contact points to replace with newPoint.
// Uses the deepest (smallest z) point as a keeper, and among the other three,
// picks the one whose replacement yields the largest triangle area.
// Returns -1 if input is invalid (cache size != 4).
int selectReplacement(const std::array<std::array<double,3>,4>& cache,
                      const std::array<double,3>& newPoint) {
    // Find deepest cached point: smallest z-coordinate (first occurrence wins ties)
    int deepestIdx = 0;
    for (int i = 1; i < 4; ++i) {
        if (cache[i][2] < cache[deepestIdx][2]) {
            deepestIdx = i;
        }
    }

    int bestIdx = -1;
    double bestArea = -1.0;

    // For each candidate replacement index (skip the deepest)
    for (int i = 0; i < 4; ++i) {
        if (i == deepestIdx) continue;

        double area = 0.0;
        // Replicate the simplified version: compute cross product of
        // (newPoint - cache[j]) and (cache[k] - cache[l]) for the three other indices
        // using the same pairings as the original code.
        if (i != 0) {
            std::array<double,3> a0 = {newPoint[0] - cache[1][0],
                                        newPoint[1] - cache[1][1],
                                        newPoint[2] - cache[1][2]};
            std::array<double,3> b0 = {cache[3][0] - cache[2][0],
                                        cache[3][1] - cache[2][1],
                                        cache[3][2] - cache[2][2]};
            area = cross2D(a0, b0);
        } else if (i != 1) {
            std::array<double,3> a1 = {newPoint[0] - cache[0][0],
                                        newPoint[1] - cache[0][1],
                                        newPoint[2] - cache[0][2]};
            std::array<double,3> b1 = {cache[3][0] - cache[2][0],
                                        cache[3][1] - cache[2][1],
                                        cache[3][2] - cache[2][2]};
            area = cross2D(a1, b1);
        } else if (i != 2) {
            std::array<double,3> a2 = {newPoint[0] - cache[0][0],
                                        newPoint[1] - cache[0][1],
                                        newPoint[2] - cache[0][2]};
            std::array<double,3> b2 = {cache[3][0] - cache[1][0],
                                        cache[3][1] - cache[1][1],
                                        cache[3][2] - cache[1][2]};
            area = cross2D(a2, b2);
        } else { // i == 3
            std::array<double,3> a3 = {newPoint[0] - cache[0][0],
                                        newPoint[1] - cache[0][1],
                                        newPoint[2] - cache[0][2]};
            std::array<double,3> b3 = {cache[2][0] - cache[1][0],
                                        cache[2][1] - cache[1][1],
                                        cache[2][2] - cache[1][2]};
            area = cross2D(a3, b3);
        }

        if (area > bestArea) {
            bestArea = area;
            bestIdx = i;
        }
    }

    // If no candidate was found (shouldn't happen for valid input), fallback to deepest index
    if (bestIdx == -1) {
        bestIdx = deepestIdx;
    }
    return bestIdx;
}
#include <cassert>

int main() {
    // Case 1: Simple square points, new point at center, deepest is index 0 (z=0)
    {
        std::array<std::array<double,3>,4> cache = {{
            {{0,0,0}}, {{1,0,1}}, {{1,1,1}}, {{0,1,1}}
        }};
        std::array<double,3> newPoint = {{0.5,0.5,0.5}};
        int result = selectReplacement(cache, newPoint);
        // Deepest is idx0 (z=0). Candidates: 1,2,3. Areas:
        // i=1: cross((new-cache0),(cache3-cache2)) → (0.5,0.5) × ( -1,0 ) = 0.5
        // i=2: cross((new-cache0),(cache3-cache1)) → (0.5,0.5) × (-1,-1) = 0 (collinear?)
        // i=3: cross((new-cache0),(cache2-cache1)) → (0.5,0.5) × (0,1) = 0.5
        // Both 0.5, first max is i=1.
        assert(result == 1);
    }

    // Case 2: Deepest is index 1, and replacement should be index 0 for large area
    {
        std::array<std::array<double,3>,4> cache = {{
            {{0,0,5}}, {{10,10,0}}, {{1,1,5}}, {{2,2,5}}
        }};
        std::array<double,3> newPoint = {{0,5,5}};
        // Deepest is idx1 (z=0). Candidates: 0,2,3.
        // Compute areas:
        // i=0: a=(new-cache1)=(-10,-5), b=(cache3-cache2)=(1,1) → cross = -10*1 - (-5*1) = -5 → area=25
        // i=2: a=(new-cache1)=(-10,-5), b=(cache3-cache1)=(-8,-8) → cross = (-10)*(-8) - (-5)*(-8)=80-40=40 → area=1600
        // i=3: a=(new-cache1)=(-10,-5), b=(cache2-cache1)=(-9,-9) → cross = 90-45=45 → area=2025
        // So best is i=3.
        int result = selectReplacement(cache, newPoint);
        assert(result == 3);
    }

    // Case 3: All z equal, deepest is index 0, max area among indices 1,2,3
    {
        std::array<std::array<double,3>,4> cache = {{
            {{0,0,0}}, {{10,0,0}}, {{0,10,0}}, {{10,10,0}}
        }};
        std::array<double,3> newPoint = {{1,1,0}};
        // Candidates skip deepest (0). Compute:
        // i=1: a=(new-cache0)=(1,1), b=(cache3-cache2)=(10,0) → cross=1*0-1*10=-10 → area=100
        // i=2: a=(new-cache0)=(1,1), b=(cache3-cache1)=(0,10) → cross=1*10-1*0=10 → area=100
        // i=3: a=(new-cache0)=(1,1), b=(cache2-cache1)=(-10,10) → cross=1*10 - 1*(-10)=20 → area=400
        // So best is i=3.
        int result = selectReplacement(cache, newPoint);
        assert(result == 3);
    }

    // Case 4: New point far away, ensures a particular candidate wins
    {
        std::array<std::array<double,3>,4> cache = {{
            {{0,0,1}}, {{1,0,1}}, {{0,1,1}}, {{1,1,1}}
        }};
        std::array<double,3> newPoint = {{100,0,1}};
        // All z=1, deepest is idx0. Candidates 1,2,3.
        // i=1: a=(new-cache0)=(100,0), b=(cache3-cache2)=(1,-1) → cross=100*(-1) - 0*1 = -100 → area=10000
        // i=2: a=(new-cache0)=(100,0), b=(cache3-cache1)=(0,1) → cross=100*1 - 0*0=100 → area=10000
        // i=3: a=(new-cache0)=(100,0), b=(cache2-cache1)=(-1,1) → cross=100*1 - 0*(-1)=100 → area=10000
        // First max is i=1.
        int result = selectReplacement(cache, newPoint);
        assert(result == 1);
    }

    // Case 5: Degenerate zero area for some, ensure correct selection
    {
        std::array<std::array<double,3>,4> cache = {{
            {{0,0,0}}, {{1,1,0}}, {{2,2,0}}, {{3,3,0}}
        }};
        std::array<double,3> newPoint = {{0,10,0}};
        // Deepest is idx0 (z=0, first with smallest z since all 0). Candidates 1,2,3.
        // All cache points are collinear on line y=x, so cross products with newPoint?
        // i=1: a=(new-cache0)=(0,10), b=(cache3-cache2)=(1,1) → cross=0*1 - 10*1 = -10 → area=100
        // i=2: a=(new-cache0)=(0,10), b=(cache3-cache1)=(2,2) → cross=0*2 - 10*2 = -20 → area=400
        // i=3: a=(new-cache0)=(0,10), b=(cache2-cache1)=(1,1) → cross=0*1 - 10*1 = -10 → area=100
        // Best is i=2.
        int result = selectReplacement(cache, newPoint);
        assert(result == 2);
    }

    return 0;
}
